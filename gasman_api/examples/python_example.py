"""
python_example.py — Example: calling gasmanAPI from Python via ctypes.

Shows how to:
  1. Load gasmanAPI.dll / .so / .dylib at runtime.
  2. Convert an input file (XML, JSON, CSV, or XLSX) into the GasMan native
     JSON format that GasManJsonToCsv() expects.
  3. Call GasManJsonToCsv() and write the CSV result.

Usage:
    python python_example.py <input_file> [output_file] [--interp[=SEC]]

    input_file  — .xml, .json, .csv, or .xlsx scenario
    output_file — optional path for the CSV output (default: output.csv)
    --interp    — upsample the engine output to a finer, evenly spaced grid by
                  first-order (linear) interpolation.  Default step is 1 s
                  (--interp=2 for 2 s, etc.).  The engine never returns data
                  faster than the integration step DT (one nominal breath,
                  ~6 s); this option fills the gaps for display only and adds
                  no physiological information beyond the straight-line
                  assumption between DT ticks.

Requirements:
    pip install pandas openpyxl   (only needed for .xlsx input)
"""

import ctypes
import json
import os
import platform
import sys
import xml.etree.ElementTree as ET
from datetime import time
from pathlib import Path


# ─────────────────────────────────────────────────────────────────────────────
# Shared library loader
# ─────────────────────────────────────────────────────────────────────────────

def load_gasmanAPI() -> ctypes.CDLL:
    """
    Locate and load gasmanAPI.dll / .so / .dylib.

    Search order:
      1. <repo_root>/compiled/   (where CMake places built binaries)
      2. The directory containing this script.
    """
    system = platform.system()
    ext    = {"Windows": ".dll", "Darwin": ".dylib"}.get(system, ".so")
    name   = f"gasmanAPI{ext}"

    # This file lives in examples/, so go one level up to find compiled/.
    repo_root   = Path(__file__).resolve().parent.parent
    candidates  = [
        repo_root / "compiled" / name,
        Path(__file__).resolve().parent / name,
    ]

    for path in candidates:
        if path.is_file():
            lib = ctypes.CDLL(str(path))

            lib.GasManJsonToCsv.argtypes = [
                ctypes.c_char_p,  # jsonStr
                ctypes.c_int,     # len
                ctypes.c_int,     # startSecond
                ctypes.c_int,     # endSecond
                ctypes.c_int,     # everySeconds
            ]
            lib.GasManJsonToCsv.restype = ctypes.c_char_p

            lib.GasManLastError.argtypes = []
            lib.GasManLastError.restype  = ctypes.c_int

            lib.GasManErrorString.argtypes = [ctypes.c_int]
            lib.GasManErrorString.restype  = ctypes.c_char_p

            print(f"Loaded: {path}")
            return lib

    raise FileNotFoundError(
        f"Cannot find {name}. "
        "Build the project first (cmake --build build --config Release)."
    )


# ─────────────────────────────────────────────────────────────────────────────
# Input parsers — all return a dict in the GasMan native JSON schema
# ─────────────────────────────────────────────────────────────────────────────

def _xml_element_to_dict(element: ET.Element) -> dict:
    """
    Recursively convert an XML element to a tagName-keyed dict.
    The GasManJsonToCsv() DLL also accepts this format.
    """
    result = {"tagName": element.tag}
    for key, value in element.attrib.items():
        result[key] = value
    if element.text and element.text.strip():
        result["text"] = element.text.strip()
    for child in element:
        child_dict = _xml_element_to_dict(child)
        if child.tag not in result:
            result[child.tag] = child_dict
        else:
            if not isinstance(result[child.tag], list):
                result[child.tag] = [result[child.tag]]
            result[child.tag].append(child_dict)
    return result


def parse_xml(file_path: str) -> dict:
    """Parse a GasMan XML scenario file into a dict the DLL can consume."""
    root = ET.parse(file_path).getroot()
    return _xml_element_to_dict(root)


def parse_json(file_path: str) -> dict:
    """Load a GasMan JSON scenario file (native or tagName format)."""
    with open(file_path, encoding="utf-8") as f:
        return json.load(f)


def parse_csv(file_path: str) -> dict:
    """
    Parse a GasMan sections-based CSV file into the GasMan native JSON format.

    Sections (case-insensitive, lines starting with # are comments):
      [patient]   — key,value rows (weight_kg)
      [volumes]   — key,value rows (vrg, fat, ven, alv, mus)
      [flows]     — key,value rows (vrg, fat, mus)
      [agent]     — key,value rows; repeat block for each agent
      [settings]  — first row is header, subsequent rows are data
    """
    import csv as csv_mod

    section   = None
    patient   = {}
    volumes   = {}
    flows     = {}
    agents    = []
    cur_agent = None
    settings  = []
    hdr       = None

    with open(file_path, newline="", encoding="utf-8") as fh:
        reader = csv_mod.reader(fh)
        for raw_row in reader:
            # Rejoin the row so we can strip comment lines
            line = ",".join(raw_row).strip()
            if not line or line.startswith("#"):
                continue

            # Section header?
            if line.startswith("["):
                new_sec = line.split("]")[0].lstrip("[").strip().lower()
                if new_sec == "agent":
                    # Save previous agent block if any, start a fresh one
                    if cur_agent is not None:
                        agents.append(cur_agent)
                    cur_agent = {}
                elif new_sec == "settings":
                    hdr = None  # reset so next row is treated as header
                section = new_sec
                continue

            cols = [c.strip() for c in raw_row]

            if section == "patient":
                if len(cols) >= 2:
                    patient[cols[0]] = cols[1]

            elif section == "volumes":
                if len(cols) >= 2:
                    volumes[cols[0]] = cols[1]

            elif section == "flows":
                if len(cols) >= 2:
                    flows[cols[0]] = cols[1]

            elif section == "agent":
                if cur_agent is not None and len(cols) >= 2:
                    cur_agent[cols[0]] = cols[1]

            elif section == "settings":
                if hdr is None:
                    hdr = [c.lower() for c in cols]
                else:
                    row_dict = dict(zip(hdr, cols))
                    settings.append(row_dict)

    # Flush last agent
    if cur_agent is not None:
        agents.append(cur_agent)

    if not agents:
        raise ValueError("No [agent] block found in CSV.")
    if not settings:
        raise ValueError("No [settings] block found in CSV.")

    # ── Build native JSON ─────────────────────────────────────────────────────
    agent_objects = []
    for ag in agents:
        obj = {"name": ag.get("name", "")}
        for k in ("lambdaMus", "lambdaBlood", "lambdaFat",
                  "volatility", "bottleSize", "lambdaVrg", "bottleCost"):
            # CSV keys are lowercase; try exact then lowercase match
            val = ag.get(k) or ag.get(k.lower())
            if val is not None:
                try:
                    obj[k] = float(val)
                except ValueError:
                    obj[k] = val
        agent_objects.append(obj)

    setting_objects = []
    for row in settings:
        # Detect per-agent del/inject columns (del1,inject1,del2,inject2,...)
        agent_settings = []
        for i, ag in enumerate(agent_objects, start=1):
            suffix = str(i) if len(agent_objects) > 1 else ""
            d_key = f"del{suffix}"
            j_key = f"inject{suffix}"
            agent_settings.append({
                "name":   ag["name"],
                "del":    float(row.get(d_key, 0.0)),
                "inject": float(row.get(j_key, 0.0)),
            })

        setting_objects.append({
            "time":           row.get("time", "00:00:00"),
            "va":             float(row.get("va",  4.0)),
            "fgf":            float(row.get("fgf", 5.0)),
            "co":             float(row.get("co",  5.0)),
            "circuit":        row.get("circuit", "Semi"),
            "agent_settings": agent_settings,
        })

    # Emit patient sub-objects ONLY for values the CSV actually supplies.
    # Anything omitted is left out of the JSON entirely, so the DLL falls back
    # to its gasman.ini defaults (the ini wins).  In practice a scenario only
    # needs an [agent] name and a [settings] block; [patient]/[volumes]/[flows]
    # are all optional.  Only recognised compartment keys are forwarded.
    patient_obj = {}
    if "weight_kg" in patient:
        patient_obj["weight"] = {"value": float(patient["weight_kg"]),
                                 "unit": "kilograms"}

    vol_obj = {k: float(v) for k, v in volumes.items()
               if k in ("vrg", "fat", "ven", "alv", "mus")}
    if vol_obj:
        patient_obj["volumes"] = vol_obj

    flow_obj = {k: float(v) for k, v in flows.items()
                if k in ("vrg", "fat", "mus")}
    if flow_obj:
        patient_obj["flows"] = flow_obj

    params = {"agents": agent_objects}
    if patient_obj:
        params["patient"] = patient_obj

    # Optional manual integration time-step (ms), from a [patient] dt_ms row.
    # Overrides the weight-derived allometric DT in the DLL.  Omit to keep the
    # automatic behaviour.
    if "dt_ms" in patient:
        params["dt_ms"] = float(patient["dt_ms"])

    return {
        "description": "",
        "params": params,
        "settings": setting_objects,
    }


def parse_xlsx(file_path: str) -> dict:
    """
    Parse a GasMan Excel workbook (gasman_template.xlsx layout) into the
    GasMan native JSON format.

    Layout expected in the first sheet:
      Row containing "Patient - Weight"
        +1, col C  → weight (kg)
        +3..+7, B  → volumes (vrg, fat, ven, alv, mus)
        +9..+11, B → flows   (vrg, fat, mus)
      Row containing "Agents"
        +1          → column headers
        +2..next    → agent rows
      Row containing "Settings"
        +1          → column headers (va, fgf, circuit, time, co, del, inject)
        +2..end     → setting rows
    """
    import pandas as pd

    df = pd.read_excel(file_path, header=None)

    def find_row(keyword: str) -> int:
        for idx, row in df.iterrows():
            if row.astype(str).str.contains(keyword, case=False).any():
                return int(idx)
        return -1

    p = find_row("Patient - Weight")
    a = find_row("Agents")
    s = find_row("Settings")

    if p < 0 or a < 0 or s < 0:
        raise ValueError(
            "Excel file does not contain the expected 'Patient - Weight', "
            "'Agents', and 'Settings' sections."
        )

    # ── Patient ──────────────────────────────────────────────────────────────
    weight = float(df.iloc[p + 1, 2])
    patient = {
        "weight":  {"value": weight, "unit": "kilograms"},
        "volumes": {
            "vrg": float(df.iloc[p + 3, 1]),
            "fat": float(df.iloc[p + 4, 1]),
            "ven": float(df.iloc[p + 5, 1]),
            "alv": float(df.iloc[p + 6, 1]),
            "mus": float(df.iloc[p + 7, 1]),
        },
        "flows": {
            "vrg": float(df.iloc[p + 9,  1]),
            "fat": float(df.iloc[p + 10, 1]),
            "mus": float(df.iloc[p + 11, 1]),
        },
    }

    # ── Agents ───────────────────────────────────────────────────────────────
    agents_block = df.iloc[a + 1 : s].dropna(how="all").reset_index(drop=True)
    agents_block.columns = agents_block.iloc[0]
    agents_df = agents_block.iloc[1:].reset_index(drop=True)
    agents_df.columns = (
        agents_df.columns.astype(str)
        .str.strip()
        .str.lower()
        .str.replace(" ", "", regex=False)
    )

    agents = []
    for _, row in agents_df.iterrows():
        agent = {}
        for key, val in row.items():
            agent[str(key)] = val if not hasattr(val, "item") else val.item()
        agents.append(agent)

    # ── Settings ─────────────────────────────────────────────────────────────
    settings_block = df.iloc[s + 1:].dropna(how="all").reset_index(drop=True)
    settings_block.columns = settings_block.iloc[0]
    settings_df = settings_block.iloc[1:].reset_index(drop=True)
    settings_df.columns = (
        settings_df.columns.astype(str)
        .str.strip()
        .str.lower()
        .str.replace(" ", "_", regex=False)
    )

    agent_name = agents[0].get("name", "") if agents else ""

    def to_time_str(val) -> str:
        """Convert a time value (datetime.time, string, or numeric fraction) to HH:MM:SS."""
        if isinstance(val, time):
            return val.strftime("%H:%M:%S")
        s = str(val).strip()
        if ":" in s:
            # Normalise to HH:MM:SS — the engine requires exactly 3 components
            if s.count(":") == 1:
                s += ":00"
            return s
        # Excel stores time as a fraction of a day
        try:
            total_sec = round(float(s) * 86400)
            h, rem = divmod(total_sec, 3600)
            m, sec = divmod(rem, 60)
            return f"{h:02d}:{m:02d}:{sec:02d}"
        except ValueError:
            return "00:00:00"

    settings = []
    for _, row in settings_df.iterrows():
        setting = {
            "time":    to_time_str(row.get("time", "00:00:00")),
            "va":      float(row.get("va",  4.0)),
            "fgf":     float(row.get("fgf", 5.0)),
            "co":      float(row.get("co",  5.0)),
            "circuit": str(row.get("circuit", "Semi")),
            "agent_settings": [{
                "name":   agent_name,
                "del":    float(row.get("del", 0.0)),
                "inject": float(row.get("inject", 0.0)),
            }],
        }
        settings.append(setting)

    # ── Assemble native JSON ──────────────────────────────────────────────────
    return {
        "description": "",
        "params": {
            "patient": patient,
            "agents":  agents,
        },
        "settings": settings,
    }


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def _last_setting_sec(data: dict) -> int:
    """Return the last setting's time in seconds, or 0 if unavailable."""
    settings = data.get("settings", [])
    if not settings:
        return 0
    raw = settings[-1].get("time", "00:00:00")
    if isinstance(raw, time):
        return raw.hour * 3600 + raw.minute * 60 + raw.second
    parts = str(raw).strip().split(":")
    try:
        if len(parts) == 3:
            return int(parts[0]) * 3600 + int(parts[1]) * 60 + int(parts[2])
        if len(parts) == 2:
            return int(parts[0]) * 60 + int(parts[1])
    except ValueError:
        pass
    return 0


def _serialize(data):
    """Recursively convert datetime.time objects to HH:MM:SS strings."""
    if isinstance(data, dict):
        return {k: _serialize(v) for k, v in data.items()}
    if isinstance(data, list):
        return [_serialize(v) for v in data]
    if isinstance(data, time):
        return data.strftime("%H:%M:%S")
    return data


def interpolate_csv(csv_text: str, step_sec: int = 1) -> str:
    """First-order (linear) interpolation of the engine's DT-spaced output onto
    a finer, evenly spaced time grid (default 1 s).

    The simulation engine only produces real values at DT ticks (one nominal
    breath, ~6 s) and never returns rows faster than DT.  This routine draws a
    straight line between successive ticks to show second-by-second values.  It
    is a *display* convenience only — it introduces no new physiology beyond the
    linear assumption between ticks.  Numeric columns are interpolated; the
    Agent column is carried; Time is rebuilt at the finer step.  Multi-agent
    output (several rows per timestamp) is handled per agent.
    """
    import bisect
    lines = [ln for ln in csv_text.splitlines() if ln.strip()]
    if len(lines) < 3:
        return csv_text
    header = lines[0].split(",")
    ti, ai = header.index("Time"), header.index("Agent")

    def t2s(t):
        p = t.split(":")
        return int(p[0]) * 3600 + int(p[1]) * 60 + int(p[2])

    def s2t(s):
        # Match the engine's unpadded "H:M:S" time format so a single parser
        # handles both raw and interpolated output.
        h, r = divmod(int(s), 3600)
        m, sec = divmod(r, 60)
        return f"{h}:{m}:{sec}"

    series, order = {}, []
    for ln in lines[1:]:
        c = ln.split(",")
        a = c[ai]
        if a not in series:
            series[a] = []
            order.append(a)
        series[a].append((t2s(c[ti]), c))
    for a in order:
        series[a].sort(key=lambda x: x[0])
    times = {a: [r[0] for r in series[a]] for a in order}

    t0 = min(v[0][0] for v in series.values())
    t1 = max(v[-1][0] for v in series.values())

    out = [",".join(header)]
    for sec in range(t0, t1 + 1, step_sec):
        for a in order:
            ts, rows = times[a], series[a]
            if sec <= ts[0]:
                c = list(rows[0][1])
            elif sec >= ts[-1]:
                c = list(rows[-1][1])
            else:
                j = bisect.bisect_right(ts, sec) - 1
                t_lo, c_lo = ts[j], rows[j][1]
                t_hi, c_hi = ts[j + 1], rows[j + 1][1]
                f = (sec - t_lo) / (t_hi - t_lo)
                c = list(c_lo)
                if f > 0.0:
                    for i in range(len(header)):
                        if i in (ti, ai):
                            continue
                        try:
                            lo, hi = float(c_lo[i]), float(c_hi[i])
                            c[i] = f"{lo + (hi - lo) * f:.6g}"
                        except ValueError:
                            c[i] = c_lo[i]
            c[ti] = s2t(sec)
            out.append(",".join(c))
    return "\n".join(out) + "\n"


def run(input_file: str, output_file: str = "output.csv",
        start_sec: int = 0, end_sec: int = -1, every_sec: int = 10,
        interp: int = 0) -> None:
    """
    Load gasmanAPI, parse *input_file*, run the simulation, write *output_file*.

    end_sec defaults to -1, which means "auto": the simulation runs to at least
    the time of the last settings entry (minimum 300 s).  Pass an explicit value
    to override.

    interp > 0 upsamples the engine output to a *interp*-second grid by linear
    interpolation.  In that mode the engine is asked for its finest real data
    (every_sec=1, which the DLL clamps up to DT), then the gaps are filled here.
    """
    lib = load_gasmanAPI()   # must load before pandas to avoid CRT version conflict

    ext = os.path.splitext(input_file)[1].lower()

    if ext == ".xml":
        data = parse_xml(input_file)
    elif ext == ".json":
        data = parse_json(input_file)
    elif ext == ".csv":
        data = parse_csv(input_file)
    elif ext == ".xlsx":
        data = parse_xlsx(input_file)
    else:
        raise ValueError(f"Unsupported file type: {ext!r}  (use .xml, .json, .csv, or .xlsx)")

    # Remove embedded image data that bloats the payload without adding value.
    if isinstance(data, dict):
        data.pop("imageData", None)

    if end_sec < 0:
        end_sec = max(300, _last_setting_sec(data))

    # When interpolating, ask the engine for its finest real grid (every_sec=1,
    # which the DLL clamps up to DT) so the interpolation starts from DT ticks.
    dll_every = 1 if interp > 0 else every_sec

    payload = json.dumps(_serialize(data)).encode("utf-8")
    result  = lib.GasManJsonToCsv(payload, len(payload), start_sec, end_sec, dll_every)
    err_code = lib.GasManLastError()

    if result is None:
        raise RuntimeError("GasManJsonToCsv returned NULL.")

    csv_text = result.decode("utf-8")
    if err_code != 0:
        err_msg = lib.GasManErrorString(err_code).decode("utf-8")
        raise RuntimeError(f"Simulation failed [code {err_code}: {err_msg}] — {csv_text}")

    if interp > 0:
        csv_text = interpolate_csv(csv_text, interp)

    with open(output_file, "w", encoding="utf-8") as f:
        f.write(csv_text)
    print(f"CSV output written to: {output_file}"
          + (f" (interpolated to {interp}s)" if interp > 0 else ""))


if __name__ == "__main__":
    # Parse positional args + optional --interp[=SEC] flag.
    _interp = 0
    _pos = []
    for _a in sys.argv[1:]:
        if _a.startswith("--interp"):
            _interp = int(_a.split("=", 1)[1]) if "=" in _a else 1
        else:
            _pos.append(_a)

    if len(_pos) not in (1, 2):
        print(__doc__)
        sys.exit(1)

    _input  = _pos[0]
    _output = _pos[1] if len(_pos) == 2 else "output.csv"
    run(_input, _output, interp=_interp)
