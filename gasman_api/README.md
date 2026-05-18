# GasMan API

Standalone C++ library and tools for running GasMan pharmacokinetic simulations
and converting simulation scenarios between formats.  Compiles on Windows,
macOS, and Linux with a single CMake invocation.

## Repository layout

```
gasman_api/
├── CMakeLists.txt            ← top-level CMake (builds everything)
├── gasman.ini                ← runtime settings (auto-copied next to each binary)
│
├── examples/                 ← sample input/output files + Python example
│   ├── example_1.xml         ← GasMan XML scenario
│   ├── data.json             ← GasMan JSON scenario
│   ├── test.json             ← multi-step Sevoflurane scenario with results
│   ├── simulation_template.csv  ← CSV template for gasman_run / gasman_convert
│   ├── gasman_template.xlsx  ← Excel template for scenario input
│   ├── output.csv            ← sample simulation CSV output
│   ├── python_example.py     ← Python reference: ctypes FFI to gasmanAPI
│   ├── r_example.R           ← R reference: CLI (system2) + DLL (Rcpp shim)
│   └── matlab_example.m      ← MATLAB reference: loadlibrary/calllib + CLI
│
├── gasmanAPI/                ← simulation engine (shared + static library)
│   ├── CMakeLists.txt
│   ├── gasmanAPI.h           ← public C API header (include this in your project)
│   ├── gasmanAPI.cpp         ← exports GasManJsonToCsv() from the shared lib
│   ├── Gas.h / Gas.cpp       ← core pharmacokinetic model
│   ├── GasAnes.h / .cpp      ← anaesthetic agent properties
│   ├── GasApplication.h / .cpp
│   ├── GasDoc.h / .cpp       ← scenario document model
│   ├── GasSample.h / .cpp    ← simulation sample / time-step
│   ├── GasPtrList.h          ← typed pointer list utility
│   ├── ColorUtils.h / .cpp   ← colour helpers
│   ├── StringUtils.h / .cpp  ← string helpers
│   ├── trim.h                ← whitespace trimming utility
│   ├── gasman.ini            ← default runtime settings (copied to compiled/ by build)
│   └── importLibraries/      ← bundled third-party headers (all MIT / public domain)
│       ├── json.hpp          ← nlohmann/json 3.11.3
│       ├── SimpleIni.h       ← SimpleIni 4.22 (Brodie Thiesfield)
│       ├── miniz.h           ← miniz 2.2.0 (Rich Geldreich) — ZIP decompression
│       └── miniz.cpp         ← miniz 2.2.0 implementation
│
├── convert/                  ← standalone CSV → XML / JSON converter
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── CsvReader.h           ← header-only sections-based CSV parser
│   └── GasManWriter.h        ← header-only XML / JSON writer
│
├── run/                      ← standalone simulation runner (no DLL, no Python)
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── XmlToJson.h           ← header-only XML → JSON converter
│   └── XlsxReader.h          ← header-only XLSX → GasManInput reader
│
└── GasManWeb/                ← cross-platform test harness (loads gasmanAPI at runtime)
    ├── CMakeLists.txt
    └── GasManWeb.cpp
```

## Quick start — CMake (all platforms)

Always pass `--fresh` to avoid stale cache errors when the build directory was
previously configured on a different machine or OS.

**Linux / macOS:**

```bash
# From the gasman_api/ directory:
cmake --fresh --preset unix-release
cmake --build build
```

**Windows (command prompt):**

```powershell
cmake --fresh --preset x64-Release
cmake --build build --config Release
```

All binaries land in `gasman_api/compiled/` and `gasman.ini` is copied there
automatically by the post-build step.

| Binary | Platform | Description |
|--------|----------|-------------|
| `gasmanAPI.dll` | Windows | Shared simulation library |
| `gasmanAPI.so` / `gasmanAPI.dylib` | Linux / macOS | Shared simulation library |
| `gasman_run` / `gasman_run.exe` | all | Self-contained runner — no shared lib needed |
| `gasman_convert` / `gasman_convert.exe` | all | CSV → XML / JSON converter |
| `gasman_web` / `gasman_web.exe` | all | Runtime shared-lib test harness |

### Visual Studio (Windows)

Generate a `.sln` and open it:

```powershell
cmake --fresh -S . -B build -G "Visual Studio 17 2022" -A x64
start build\gasman_api.sln
```

---

## Tool reference

### `gasman_run` — self-contained simulation runner

The recommended way to run simulations from the command line.  Links the engine
**statically** — only the binary and `gasman.ini` are needed; no `gasmanAPI.*` at
runtime.  Accepts XML, JSON, XLSX, or CSV input directly.

```
Usage: gasman_run <input.[xml|json|csv|xlsx]> [options]

Options:
  --start  <sec>   Simulation start time in seconds  (default 0)
  --end    <sec>   Simulation end time in seconds    (default 300)
  --every  <sec>   Output row interval in seconds    (default 10)
  --output <file>  Write CSV to <file>               (default: stdout)
  --ini    <file>  Path to gasman.ini                (default: auto-locate)

Examples:
  gasman_run examples/example_1.xml --end 600 --every 5
  gasman_run examples/simulation_template.csv --output results.csv
  gasman_run examples/gasman_template.xlsx --output results.csv
  gasman_run examples/data.json --start 0 --end 300 --every 10 --output out.csv
```

**Supported input formats** (detected from file extension):

| Extension | Source |
|-----------|--------|
| `.xml` | GasMan native XML scenario (exported from the desktop app) |
| `.json` | GasMan JSON scenario — both the desktop app's export format and the API's internal tagName format are accepted |
| `.csv` | Sections-based CSV (see `examples/simulation_template.csv`) |
| `.xlsx` | Excel workbook (see `examples/gasman_template.xlsx`) |
| `.gas` | Not supported — Qt binary format; open in GasMan and export to XML/JSON |
| `.xls` | Not supported — open in Excel and save as `.xlsx` |

### `gasmanAPI` — simulation shared library

Include `gasmanAPI/gasmanAPI.h` (the only public header needed) and link against
`gasmanAPI.dll` / `gasmanAPI.so` / `gasmanAPI.dylib`.

```c
/* Run a simulation — returns CSV on success, "Error: …" on failure.
   The returned pointer is valid until the SAME thread calls again. */
const char* GasManJsonToCsv(
    const char* jsonStr,   // GasMan JSON / XML scenario (UTF-8)
    int         len,       // byte length of jsonStr
    int         startSecond,
    int         endSecond,
    int         everySeconds
);

/* Structured error reporting -------------------------------------------- */

/* Error code from the most recent call on this thread (0 = success). */
int GasManLastError(void);

/* Human-readable string for an error code. */
const char* GasManErrorString(int code);
```

**Error codes** (defined in `gasmanAPI.h`):

| Constant | Value | Meaning |
|---|---|---|
| `GASMAN_OK` | 0 | Success |
| `GASMAN_ERR_INVALID_PARAMS` | 1 | Bad time parameters |
| `GASMAN_ERR_INI_NOT_FOUND` | 2 | `gasman.ini` not found |
| `GASMAN_ERR_INI_READ_FAILED` | 3 | `gasman.ini` unreadable |
| `GASMAN_ERR_JSON_PARSE` | 4 | JSON / XML parse failure |
| `GASMAN_ERR_SIMULATION` | 5 | Simulation engine error |
| `GASMAN_ERR_EXCEPTION` | 6 | Unexpected C++ exception |

**CSV columns** (on success):
```
Time, Agent, FGF, VA, CO, CKT, ALV, ART, VRG, MUS, FAT, VEN, Uptake, Delivered
```

**Thread safety**: concurrent calls are serialised internally via a process-wide
mutex.  Each thread has its own result buffer so callers on different threads can
hold the returned pointer simultaneously.  `GasManLastError()` is per-thread.

**Python example:**

```python
import ctypes, sys, pathlib

_lib_names = {"win32": "gasmanAPI.dll"}
_lib_name  = _lib_names.get(sys.platform, "gasmanAPI.so")

lib = ctypes.CDLL(str(pathlib.Path("compiled") / _lib_name))
lib.GasManJsonToCsv.restype = ctypes.c_char_p

with open("examples/data.json") as f:
    payload = f.read().encode()

csv = lib.GasManJsonToCsv(payload, len(payload), 0, 300, 10)
print(csv.decode())
```

### `gasman_convert` — CSV → XML / JSON converter

Converts a CSV scenario file to GasMan XML or JSON without running a simulation.
Useful for pre-processing before feeding `GasManJsonToCsv`.

```
Usage: gasman_convert <input.csv> [--format xml|json] [--output <file>]

  --format json    Produce GasMan JSON (default)
  --format xml     Produce GasMan XML
  --output <file>  Write to <file> instead of stdout

Examples:
  gasman_convert examples/simulation_template.csv --format json --output examples/data.json
  gasman_convert examples/simulation_template.csv --format xml
```

**CSV format** — edit `examples/simulation_template.csv` as your starting point.

Single-agent (use `del` / `inject`):

```csv
[patient]
weight_kg,70

[volumes]
vrg,6.0  fat,14.5  ven,1.0  alv,2.5  mus,33.0

[flows]
vrg,0.76  fat,0.06  mus,0.18

[agent]
name,Sevoflurane

[settings]
va,fgf,circuit,time,co,del,inject
4,8,Semi,00:00:00,5,3.7,0
4,2,Semi,00:05:00,5,2.5,0
```

Multi-agent (add a second `[agent]` block; use `del1`/`inject1`, `del2`/`inject2`, …):

```csv
[agent]
name,Sevoflurane

[agent]
name,Desflurane

[settings]
va,fgf,circuit,time,co,del1,inject1,del2,inject2
4,8,Semi,00:00:00,5,3.7,0,0,0
4,2,Semi,00:05:00,5,0,0,6.0,0
```

Delivery columns are matched to agents **positionally** — `del1`/`inject1` applies to
the first `[agent]` block, `del2`/`inject2` to the second, etc.

Excel users: fill in `examples/gasman_template.xlsx`, save the relevant sheet as CSV,
then run `gasman_convert`.

### `gasman_web` — runtime DLL test harness

Loads `gasmanAPI.*` at runtime and calls `GasManJsonToCsv`.  Useful for verifying
the shared library works without writing a full integration.

```
Usage: gasman_web [input.json] [startSec] [endSec] [everySec]
Defaults: examples/data.json  0  60  5
```

Searches for the DLL next to the executable, then falls back to the current
working directory.

### `examples/python_example.py` — Python ctypes reference

Reference implementation showing how to call `GasManJsonToCsv` from Python
via ctypes.  Accepts XML, JSON, and Excel input.  Requires `pandas` and
`openpyxl` only for `.xlsx` input.

```bash
pip install pandas openpyxl   # only needed for .xlsx

cd examples
python python_example.py example_1.xml
python python_example.py data.json
python python_example.py gasman_template.xlsx
python python_example.py example_1.xml my_output.csv
```

This file is intentionally kept simple — use it as a starting point for
integrating the shared library into a Python server or notebook.

### `examples/r_example.R` — R reference

Shows two approaches side-by-side:

| Approach | Mechanism | Packages needed |
|---|---|---|
| **A — DLL** | Inline Rcpp C++ shim; calls `GasManJsonToCsv` directly | `Rcpp` |
| **B — CLI** | `system2(gasman_run, ...)` | `jsonlite` (base R otherwise) |

```r
install.packages(c("jsonlite", "Rcpp"))   # once

Rscript r_example.R example_1.xml
Rscript r_example.R example_1.xml my_output.csv
```

Both approaches run automatically when the script is called from the command
line; Approach B (CLI) is used for the output file and Approach A (DLL) is
demonstrated in-memory if Rcpp is available.

`make_scenario()` and `run_cli()` / `load_api()$run()` can be sourced into
your own R scripts for notebook or Shiny integration.

### `examples/matlab_example.m` — MATLAB reference

Shows two approaches side-by-side:

| Approach | Mechanism | Requirements |
|---|---|---|
| **A — DLL** | `loadlibrary` / `calllib` with `gasmanAPI.h` | MATLAB R2018a+ |
| **B — CLI** | `system(gasman_run ...)` | Any MATLAB with `system()` |

```matlab
% From the MATLAB command window (cd to examples/ first):
matlab_example

% Or from the OS shell:
matlab -batch "cd('examples'); matlab_example"
```

`gasman_build_scenario()` constructs a native-format JSON struct that
`jsonencode` serialises correctly.  Replace it with your own patient data
or load values from a spreadsheet with `readmatrix` / `readtable`.

On Linux/macOS, `loadlibrary` compiles a thunk file on first use — this
requires a C compiler (`gcc` / `clang`) on the path.

---

## GasMan JSON formats (reference)

`GasManJsonToCsv` accepts two JSON schemas — use whichever is most convenient.

### Native format (core GasMan desktop app export)

This is what the GasMan desktop application produces under **File → Export → JSON**.
Numbers are JSON numbers; `settings` and `agents` are plain arrays.

```json
{
  "description": "my scenario",
  "params": {
    "patient": {
      "weight":  { "value": 70, "unit": "kilograms" },
      "volumes": { "vrg": 6.0, "fat": 14.5, "ven": 1.0, "alv": 2.5, "mus": 33.0 },
      "flows":   { "vrg": 0.76, "fat": 0.06, "mus": 0.18 }
    },
    "agents": [
      { "name": "Sevoflurane", "lambdaBlood": 0.65, "lambdaVrg": 1.1,
        "lambdaFat": 34, "lambdaMus": 2.4, "volatility": 183,
        "bottleSize": 250, "bottleCost": 75 }
    ]
  },
  "settings": [
    { "time": "00:00:00", "va": 4, "fgf": 8, "co": 5, "circuit": "Semi",
      "agent_settings": [{ "name": "Sevoflurane", "del": 3.7, "inject": 0 }] }
  ]
}
```

### tagName format (API internal / example files)

Used by `data.json`, `test.json`, and the CSV/XLSX → JSON conversion path.
Values are JSON strings; structure mirrors the XML element tree.

```json
{
  "tagName": "gasman",
  "params": {
    "tagName": "params",
    "patient": {
      "tagName": "patient",
      "weight": { "unit": "kilograms", "value": "70" },
      "volumes": { "vrg": "6.0", "fat": "14.5", "ven": "1.0", "alv": "2.5", "mus": "33.0" },
      "flows":   { "vrg": "0.76", "fat": "0.06", "mus": "0.18" }
    },
    "agents": {
      "tagName": "agents",
      "agent": { "tagName": "agent", "name": "Sevoflurane", "lambdaBlood": "0.65", "..." : "..." }
    }
  },
  "settings": {
    "tagName": "settings",
    "setting": [
      { "tagName": "setting", "time": "00:00:00", "va": "4", "fgf": "8", "co": "5",
        "circuit": "Semi",
        "agentsettings": { "agentsetting": { "agentname": "Sevoflurane", "del": "3.7", "inject": "0" } } }
    ]
  }
}
```

---

## What's not yet implemented

The following items are identified as gaps or future work:

- **`.gas` input in `gasman_run`** — GasMan's native `.gas` format is a Qt
  `QDataStream` binary containing a full saved simulation state (all computed
  compartment values, sample arrays, etc.), not just a scenario description.
  Reading it without Qt would require re-implementing `QDataStream` plus the
  entire `GasDoc::Serialize` tree.  Workaround: open in GasMan → export to XML
  or JSON → use `gasman_run`.
- **HTTP / REST wrapper** — no web server or REST API layer exists yet; the
  intended integration path is via `GasManJsonToCsv` from a Python/Node.js
  server using the shared library.

---

Gasman (https://www.gasmanweb.com/) is the brainchild of Dr James (Jim) H. Philip.
Special thanks to Hal Franklin for assisting in getting this project off the ground.

Code released under the MIT License.

### Bundled third-party libraries

All bundled libraries live in `gasmanAPI/importLibraries/` and are governed by
their own licences, reproduced in full inside each file.

| File | Library | Version | Author | Licence |
|------|---------|---------|--------|---------|
| `json.hpp` | [nlohmann/json](https://github.com/nlohmann/json) | 3.11.3 | Niels Lohmann | MIT |
| `SimpleIni.h` | [SimpleIni](https://github.com/brofield/simpleini) | 4.22 | Brodie Thiesfield | MIT |
| `miniz.h` / `miniz.cpp` | [miniz](https://github.com/richgel999/miniz) | 2.2.0 | Rich Geldreich | MIT / public domain (Unlicense) |
