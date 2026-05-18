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
  path <- file.path(.repo_root(), "compiled", paste0(name, ext))
  if (!file.exists(path))
    stop(name, " not found at ", path, ". Build the project first.")
  path
}

.find_library <- function() {
  ext  <- switch(Sys.info()[["sysname"]],
                 Windows = ".dll",
                 Darwin  = ".dylib",
                 ".so")
  path <- file.path(.repo_root(), "compiled", paste0("gasmanAPI", ext))
  if (!file.exists(path))
    stop("gasmanAPI library not found at ", path, ". Build the project first.")
  path
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

  message("Approach A — running simulation via gasman_run CLI ...")
  result <- run_cli(input_file)

  write.csv(result, output_file, row.names = FALSE)
  message(sprintf("Written %d rows to %s  (columns: %s)",
                  nrow(result),
                  output_file,
                  paste(names(result), collapse = ", ")))

  # ── Approach B demo (DLL) ─────────────────────────────────────────────────
  if (requireNamespace("Rcpp", quietly = TRUE)) {
    message("\nApproach B — same simulation via DLL ...")
    tryCatch({
      api      <- load_api()
      scenario <- make_scenario()
      json_str <- toJSON(scenario, auto_unbox = TRUE)
      csv_text <- api$run(json_str, end_sec = 300L, every_sec = 10L)

      # Parse the CSV string into a data.frame
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
