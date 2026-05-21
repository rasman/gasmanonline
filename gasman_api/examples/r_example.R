# r_example.R — GasMan simulation from R
#
# Demonstrates two approaches:
#
#   Approach A  CLI via system2()
#     Calls the gasman_run binary.  No DLL, no compiled code, no extra
#     packages.  Works for .xml, .json, .csv, and .xlsx input.
#     Requires: jsonlite (CRAN)
#
#   Approach B  DLL via Rcpp shim
#     Loads gasmanAPI.dll/.so/.dylib at runtime through a small inline
#     C++ bridge and calls GasManJsonToCsv() directly.  Equivalent to the
#     Python ctypes approach.
#     Requires: Rcpp (CRAN)
#
# Usage:
#   Rscript r_example.R <input.[xml|json|csv|xlsx]> [output.csv]
#
# Install requirements (once):
#   install.packages(c("jsonlite", "Rcpp"))

suppressPackageStartupMessages(library(jsonlite))


# =============================================================================
# Path helpers
# =============================================================================

# This script lives in examples/; gasman_api/ is one level up.
.script_dir <- function() {
  args <- commandArgs(trailingOnly = FALSE)
  f    <- sub("--file=", "", args[grep("--file=", args)])
  if (length(f)) normalizePath(dirname(f)) else getwd()
}

.repo_root <- function() normalizePath(file.path(.script_dir(), ".."),
                                        mustWork = FALSE)

.find_binary <- function(name) {
  ext  <- if (.Platform$OS.type == "windows") ".exe" else ""
  fname <- paste0(name, ext)
  # Search order: script dir → working dir → ../compiled/
  candidates <- c(
    file.path(.script_dir(),         fname),
    file.path(getwd(),               fname),
    file.path(.repo_root(), "compiled", fname)
  )
  path <- Filter(file.exists, candidates)[1]
  if (is.na(path))
    stop(name, " not found. Looked in:\n",
         paste(" ", candidates, collapse = "\n"),
         "\nBuild the project first.")
  path
}

.find_library <- function() {
  ext  <- switch(Sys.info()[["sysname"]],
                 Windows = ".dll",
                 Darwin  = ".dylib",
                 ".so")
  fname <- paste0("gasmanAPI", ext)
  # Search order: script dir → working dir → ../compiled/
  candidates <- c(
    file.path(.script_dir(),         fname),
    file.path(getwd(),               fname),
    file.path(.repo_root(), "compiled", fname)
  )
  path <- Filter(file.exists, candidates)[1]
  if (is.na(path))
    stop("gasmanAPI library not found. Looked in:\n",
         paste(" ", candidates, collapse = "\n"),
         "\nBuild the project first.")
  path
}


# Extract the last setting's time in seconds from a parsed scenario list.
.last_setting_sec <- function(doc) {
  settings <- doc[["settings"]]
  if (!is.list(settings) || length(settings) == 0L) return(300L)
  last_time <- settings[[length(settings)]][["time"]]
  if (is.null(last_time)) return(300L)
  parts <- strsplit(as.character(last_time), ":")[[1L]]
  if (length(parts) != 3L) return(300L)
  sec <- as.integer(parts[1L]) * 3600L + as.integer(parts[2L]) * 60L + as.integer(parts[3L])
  max(300L, sec)
}


# =============================================================================
# Approach A — CLI via system2(gasman_run)
# =============================================================================

#' Run a GasMan simulation using the gasman_run command-line tool.
#'
#' @param input_file  Path to a .xml, .json, .csv, or .xlsx scenario file.
#' @param start_sec   Simulation start time in seconds (default 0).
#' @param end_sec     Simulation end time in seconds (default 300).
#' @param every_sec   Output row interval in seconds (default 10).
#' @return A data.frame with columns Time, Agent, FGF, VA, CO, …
#' @examples
#' result <- run_cli("example_1.xml", end_sec = 600, every_sec = 5)
#' head(result)
run_cli <- function(input_file,
                    start_sec = 0L,
                    end_sec   = 300L,
                    every_sec = 10L) {

  runner   <- .find_binary("gasman_run")
  tmp_csv  <- tempfile(fileext = ".csv")
  on.exit(unlink(tmp_csv), add = TRUE)

  args <- c(
    normalizePath(input_file, mustWork = TRUE),
    "--start",  as.integer(start_sec),
    "--end",    as.integer(end_sec),
    "--every",  as.integer(every_sec),
    "--output", tmp_csv
  )

  out    <- system2(runner, args, stdout = TRUE, stderr = TRUE)
  status <- attr(out, "status") %||% 0L

  if (status != 0L || !file.exists(tmp_csv) || file.size(tmp_csv) == 0) {
    stop("gasman_run failed (exit ", status, "):\n",
         paste(out, collapse = "\n"))
  }

  read.csv(tmp_csv, stringsAsFactors = FALSE)
}

# Null-coalescing helper (available as %||% in rlang, defined here for base R).
`%||%` <- function(x, y) if (is.null(x)) y else x


# =============================================================================
# Approach B — DLL via Rcpp shim
# =============================================================================

#' Load the gasmanAPI shared library and return a list of wrapper functions.
#'
#' Uses an inline Rcpp C++ bridge to load the DLL at runtime and resolve the
#' three exported symbols: GasManJsonToCsv, GasManLastError, GasManErrorString.
#' The shim is compiled once per R session (Rcpp caches the result).
#'
#' @return A list with one element:
#'   $run(json_str, start_sec, end_sec, every_sec) -> character CSV string
#' @examples
#' api <- load_api()
#' csv_text <- api$run(jsonlite::toJSON(scenario, auto_unbox = TRUE))
load_api <- function() {

  if (!requireNamespace("Rcpp", quietly = TRUE))
    stop("Package 'Rcpp' is required for the DLL approach.\n",
         "Run:  install.packages('Rcpp')")

  lib_path <- .find_library()

  # ── Inline C++ bridge ──────────────────────────────────────────────────────
  # Compiled once per session; Rcpp caches the shared object in a temp dir.
  shim_src <- '
#include <Rcpp.h>
#ifdef _WIN32
#  include <windows.h>
   typedef HMODULE lib_handle_t;
#  define open_lib(p)   LoadLibraryA(p)
#  define get_sym(h,n)  GetProcAddress(h, n)
#  define close_lib(h)  FreeLibrary(h)
#else
#  include <dlfcn.h>
   typedef void* lib_handle_t;
#  define open_lib(p)   dlopen(p, RTLD_LAZY | RTLD_LOCAL)
#  define get_sym(h,n)  dlsym(h, n)
#  define close_lib(h)  dlclose(h)
#endif
#include <string>

typedef const char* (*PFN_csv)(const char*, int, int, int, int);
typedef int         (*PFN_err)(void);
typedef const char* (*PFN_str)(int);

static lib_handle_t s_lib = nullptr;
static PFN_csv      s_csv = nullptr;
static PFN_err      s_err = nullptr;
static PFN_str      s_str = nullptr;

// [[Rcpp::export]]
void gasman_load_lib(std::string path) {
  s_lib = open_lib(path.c_str());
  if (!s_lib) Rcpp::stop("Failed to load: %s", path.c_str());
  s_csv = (PFN_csv) get_sym(s_lib, "GasManJsonToCsv");
  s_err = (PFN_err) get_sym(s_lib, "GasManLastError");
  s_str = (PFN_str) get_sym(s_lib, "GasManErrorString");
  if (!s_csv || !s_err || !s_str)
    Rcpp::stop("Symbol resolution failed. Is this a gasmanAPI library?");
}

// [[Rcpp::export]]
std::string gasman_json_to_csv(
    std::string json, int start_sec, int end_sec, int every_sec
) {
  if (!s_csv) Rcpp::stop("Call gasman_load_lib() first.");
  const char* result = s_csv(json.c_str(), (int)json.size(),
                              start_sec, end_sec, every_sec);
  int code = s_err();
  if (code != 0) {
    std::string msg = s_str ? s_str(code) : "unknown error";
    Rcpp::stop("GasManJsonToCsv failed [%d: %s]  %s",
               code, msg.c_str(), result ? result : "");
  }
  return std::string(result);
}
'

  tmp_cpp <- tempfile(fileext = ".cpp")
  on.exit(unlink(tmp_cpp), add = TRUE)
  writeLines(shim_src, tmp_cpp)
  Rcpp::sourceCpp(tmp_cpp, verbose = FALSE)   # compiles + loads shim

  gasman_load_lib(lib_path)
  message("Loaded: ", lib_path)

  list(
    # Run simulation, return CSV as a character string.
    run = function(json_str,
                   start_sec = 0L,
                   end_sec   = 300L,
                   every_sec = 10L) {
      gasman_json_to_csv(json_str,
                         as.integer(start_sec),
                         as.integer(end_sec),
                         as.integer(every_sec))
    }
  )
}

#' Build a minimal GasMan native-format JSON string for testing.
#'
#' Returns an R list in the native GasMan JSON schema which can be serialised
#' with jsonlite::toJSON(scenario, auto_unbox = TRUE).
make_scenario <- function(agent       = "Sevoflurane",
                          weight_kg   = 70,
                          start_time  = "00:00:00",
                          va  = 4, fgf = 8, co = 5,
                          del = 3.7) {
  list(
    description = "",
    params = list(
      patient = list(
        weight  = list(value = weight_kg, unit = "kilograms"),
        volumes = list(vrg = 6.0, fat = 14.5, ven = 1.0, alv = 2.5, mus = 33.0),
        flows   = list(vrg = 0.76, fat = 0.06, mus = 0.18)
      ),
      agents = list(
        list(name         = agent,
             lambdaBlood  = 0.65,
             lambdaVrg    = 1.1,
             lambdaFat    = 34,
             lambdaMus    = 2.4,
             volatility   = 183,
             bottleSize   = 250,
             bottleCost   = 75)
      )
    ),
    settings = list(
      list(time    = start_time,
           va      = va,
           fgf     = fgf,
           co      = co,
           circuit = "Semi",
           agent_settings = list(
             list(name = agent, del = del, inject = 0)
           ))
    )
  )
}


# =============================================================================
# File → JSON string converter  (used by Approach B)
# =============================================================================

#' Convert any supported input file to a JSON string the DLL can consume.
#'
#' Handles .xml (tagName JSON), .json (pass-through), .csv (sections format),
#' and .xlsx (GasMan template layout).  The returned string is ready to pass
#' directly to api$run().
#'
#' @param file_path  Path to a .xml, .json, .csv, or .xlsx scenario file.
#' @return A JSON character string.
parse_file_to_json_str <- function(file_path) {
  ext <- tolower(tools::file_ext(file_path))
  switch(ext,
    json = {
      doc <- jsonlite::fromJSON(file_path, simplifyVector = FALSE)
      doc[["imageData"]] <- NULL          # strip embedded images
      jsonlite::toJSON(doc, auto_unbox = TRUE)
    },
    xml = {
      if (!requireNamespace("xml2", quietly = TRUE))
        stop("Package 'xml2' is required for XML input.\n",
             "Run:  install.packages('xml2')")
      root <- xml2::xml_root(xml2::read_xml(file_path))
      jsonlite::toJSON(.xml_to_list(root), auto_unbox = TRUE)
    },
    csv  = jsonlite::toJSON(.parse_csv(file_path),  auto_unbox = TRUE),
    xlsx = {
      if (!requireNamespace("readxl", quietly = TRUE))
        stop("Package 'readxl' is required for XLSX input.\n",
             "Run:  install.packages('readxl')")
      jsonlite::toJSON(.parse_xlsx(file_path), auto_unbox = TRUE)
    },
    stop("Unsupported file type: '.", ext,
         "' — use .xml, .json, .csv, or .xlsx")
  )
}

# ── XML → tagName list  (mirrors python_example.py _xml_element_to_dict) ────
.xml_to_list <- function(node) {
  result <- list(tagName = xml2::xml_name(node))
  attrs  <- xml2::xml_attrs(node)
  for (nm in names(attrs)) result[[nm]] <- attrs[[nm]]
  children <- xml2::xml_children(node)
  if (length(children) == 0L) {
    txt <- trimws(xml2::xml_text(node))
    if (nzchar(txt)) result[["text"]] <- txt
  }
  for (child in children) {
    tag <- xml2::xml_name(child)
    cv  <- .xml_to_list(child)
    if (is.null(result[[tag]])) {
      result[[tag]] <- cv
    } else {
      prev <- result[[tag]]
      result[[tag]] <- if (is.list(prev) && is.null(names(prev)))
                         c(prev, list(cv))
                       else
                         list(prev, cv)
    }
  }
  result
}

# ── CSV sections format  (mirrors CsvReader.h / python_example.py) ──────────
.parse_csv <- function(file_path) {
  lines <- readLines(file_path, warn = FALSE)
  lines <- lines[!grepl("^\\s*#", lines) & nzchar(trimws(lines))]

  section      <- NULL
  agent_cur    <- list()
  agents       <- list()
  settings_hdr <- NULL
  settings_rows <- list()
  patient <- list(
    weight  = list(value = 70, unit = "kilograms"),
    volumes = list(vrg = 6.0, fat = 14.5, ven = 1.0, alv = 2.5, mus = 33.0),
    flows   = list(vrg = 0.76, fat = 0.06, mus = 0.18)
  )

  for (ln in lines) {
    if (grepl("^\\s*\\[", ln)) {
      if (!is.null(section) && section == "agent" && length(agent_cur) > 0) {
        agents <- c(agents, list(agent_cur)); agent_cur <- list()
      }
      section      <- tolower(trimws(sub("^\\s*\\[([^]]+)\\].*$", "\\1", ln)))
      settings_hdr <- NULL
      next
    }
    parts <- trimws(strsplit(ln, ",")[[1L]])
    if (is.null(section) || length(parts) < 2L) next

    if      (section == "patient")  patient$weight$value         <- as.numeric(parts[2L])
    else if (section == "volumes")  patient$volumes[[parts[1L]]] <- as.numeric(parts[2L])
    else if (section == "flows")    patient$flows[[parts[1L]]]   <- as.numeric(parts[2L])
    else if (section == "agent") {
      key <- parts[1L]
      agent_cur[[key]] <- if (key == "name") parts[2L] else as.numeric(parts[2L])
    } else if (section == "settings") {
      if (is.null(settings_hdr)) settings_hdr <- parts
      else settings_rows <- c(settings_rows, list(setNames(as.list(parts), settings_hdr)))
    }
  }
  if (length(agent_cur) > 0) agents <- c(agents, list(agent_cur))
  if (length(agents)  == 0)  stop("No [agent] block found in CSV.")
  if (length(settings_rows) == 0) stop("No [settings] data found in CSV.")

  settings <- lapply(settings_rows, function(row) {
    n <- length(agents)
    agent_settings <- lapply(seq_len(n), function(i) {
      dk <- if (i == 1L) "del"    else paste0("del",    i)
      ik <- if (i == 1L) "inject" else paste0("inject", i)
      list(name = agents[[i]]$name, del = as.numeric(row[[dk]]),
           inject = as.numeric(row[[ik]]))
    })
    list(time = row[["time"]], va = as.numeric(row[["va"]]),
         fgf  = as.numeric(row[["fgf"]]), co = as.numeric(row[["co"]]),
         circuit = row[["circuit"]], agent_settings = agent_settings)
  })
  list(description = "", params = list(patient = patient, agents = agents),
       settings = settings)
}

# ── XLSX GasMan template  (mirrors python_example.py parse_xlsx) ────────────
.parse_xlsx <- function(file_path) {
  df <- readxl::read_excel(file_path, col_names = FALSE, .name_repair = "minimal")

  find_row <- function(kw) {
    for (i in seq_len(nrow(df)))
      if (any(grepl(kw, df[i, ], ignore.case = TRUE))) return(i)
    stop("Keyword '", kw, "' not found in XLSX.")
  }

  p <- find_row("Patient - Weight")
  a <- find_row("Agents")
  s <- find_row("Settings")

  patient <- list(
    weight  = list(value = as.numeric(df[[p + 1L, 3L]]), unit = "kilograms"),
    volumes = list(vrg = as.numeric(df[[p + 3L, 2L]]),
                   fat = as.numeric(df[[p + 4L, 2L]]),
                   ven = as.numeric(df[[p + 5L, 2L]]),
                   alv = as.numeric(df[[p + 6L, 2L]]),
                   mus = as.numeric(df[[p + 7L, 2L]])),
    flows   = list(vrg = as.numeric(df[[p +  9L, 2L]]),
                   fat = as.numeric(df[[p + 10L, 2L]]),
                   mus = as.numeric(df[[p + 11L, 2L]]))
  )

  agent_hdr <- tolower(gsub("\\s+", "", unlist(df[a + 1L, ])))
  agents <- Filter(
    function(ag) !is.null(ag[["name"]]) && !is.na(ag[["name"]]),
    lapply(seq(a + 2L, s - 1L), function(i) {
      row <- setNames(as.list(unlist(df[i, ])), agent_hdr)
      lapply(row, function(v) suppressWarnings(
        if (!is.na(as.numeric(v))) as.numeric(v) else v))
    })
  )

  set_hdr <- tolower(gsub("\\s+", "_", unlist(df[s + 1L, ])))
  to_time_str <- function(v) {
    s <- as.character(v)
    if (grepl(":", s)) {
      # Normalise HH:MM → HH:MM:SS; engine requires exactly 3 components
      if (lengths(regmatches(s, gregexpr(":", s, fixed = TRUE))) == 1L)
        s <- paste0(s, ":00")
      return(s)
    }
    total <- round(as.numeric(s) * 86400)
    sprintf("%02d:%02d:%02d", total %/% 3600L, (total %% 3600L) %/% 60L, total %% 60L)
  }

  settings <- Filter(Negate(is.null), lapply(seq(s + 2L, nrow(df)), function(i) {
    row <- setNames(as.list(unlist(df[i, ])), set_hdr)
    if (all(is.na(unlist(row)))) return(NULL)
    list(time = to_time_str(row[["time"]]), va = as.numeric(row[["va"]]),
         fgf  = as.numeric(row[["fgf"]]),  co = as.numeric(row[["co"]]),
         circuit = as.character(row[["circuit"]]),
         agent_settings = list(list(name   = agents[[1L]][["name"]],
                                    del    = as.numeric(row[["del"]]),
                                    inject = as.numeric(row[["inject"]]))))
  }))

  list(description = "", params = list(patient = patient, agents = agents),
       settings = settings)
}


# =============================================================================
# Main
# =============================================================================

if (!interactive()) {
  argv <- commandArgs(trailingOnly = TRUE)
  if (length(argv) < 1 || length(argv) > 2) {
    cat("Usage: Rscript r_example.R <input.[xml|json|csv|xlsx]> [output.csv]\n")
    quit(status = 1L)
  }

  input_file  <- argv[1]
  output_file <- if (length(argv) == 2L) argv[2] else "output.csv"

  # Derive end_sec from the last setting so the output covers the full scenario.
  end_sec <- tryCatch({
    json_str_tmp <- parse_file_to_json_str(input_file)
    doc_tmp      <- jsonlite::fromJSON(json_str_tmp, simplifyVector = FALSE)
    .last_setting_sec(doc_tmp)
  }, error = function(e) 300L)

  message("Approach A — running simulation via gasman_run CLI ...")
  result <- run_cli(input_file, end_sec = end_sec)

  write.csv(result, output_file, row.names = FALSE)
  message(sprintf("Written %d rows to %s  (columns: %s)",
                  nrow(result),
                  output_file,
                  paste(names(result), collapse = ", ")))

  # ── Approach B — same file via DLL ───────────────────────────────────────
  if (requireNamespace("Rcpp", quietly = TRUE)) {
    message("\nApproach B — same simulation via DLL ...")
    tryCatch({
      api      <- load_api()
      json_str <- parse_file_to_json_str(input_file)
      csv_text <- api$run(json_str, end_sec = end_sec, every_sec = 10L)

      result_dll <- read.csv(text = csv_text, stringsAsFactors = FALSE)
      message(sprintf("DLL returned %d rows — first row: Time=%s ALV=%s",
                      nrow(result_dll),
                      result_dll$Time[1],
                      result_dll$ALV[1]))
    }, error = function(e) {
      message("DLL approach skipped: ", conditionMessage(e))
    })
  } else {
    message("\nApproach B skipped (Rcpp not installed).")
    message("Run  install.packages('Rcpp')  to enable the DLL approach.")
  }
}
