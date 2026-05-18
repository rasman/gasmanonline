"""
python_example.py — Example: calling gasmanAPI from Python via ctypes.

Shows how to:
  1. Load gasmanAPI.dll / .so / .dylib at runtime.
  2. Convert an input file (XML, JSON, CSV, or XLSX) into the GasMan native
     JSON format that GasManJsonToCsv() expects.
  3. Call GasManJsonToCsv() and write the CSV result.

Usage:
    python python_example.py <input_file> [output_file]

    input_file  — .xml, .json, .csv, or .xlsx scenario
    output_file — optional path for the CSV output (default: output.csv)

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

    weight_kg = float(patient.get("weight_kg", 70.0))

    return {
        "description": "",
        "params": {
            "patient": {
                "weight":  {"value": weight_kg, "unit": "kilograms"},
                "volumes": {
                    "vrg": float(volumes.get("vrg", 6.0)),
                    "fat": float(volumes.get("fat", 14.5)),
                    "ven": float(volumes.get("ven", 1.0)),
                    "alv": float(volumes.get("alv", 2.5)),
                    "mus": float(volumes.get("mus", 33.0)),
                },
                "flows": {
                    "vrg": float(flows.get("vrg", 0.76)),
                    "fat": float(flows.get("fat", 0.06)),
                    "mus": float(flows.get("mus", 0.18)),
                },
            },
            "agents": agent_objects,
        },
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

def _serialize(data):
    """Recursively convert datetime.time objects to HH:MM:SS strings."""
    if isinstance(data, dict):
        return {k: _serialize(v) for k, v in data.items()}
    if isinstance(data, list):
        return [_serialize(v) for v in data]
    if isinstance(data, time):
        return data.strftime("%H:%M:%S")
    return data


def run(input_file: str, output_file: str = "output.csv",
        start_sec: int = 0, end_sec: int = 300, every_sec: int = 10) -> None:
    """
    Load gasmanAPI, parse *input_file*, run the simulation, write *output_file*.
    """
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

    lib = load_gasmanAPI()

    payload = json.dumps(_serialize(data)).encode("utf-8")
    result  = lib.GasManJsonToCsv(payload, len(payload), start_sec, end_sec, every_sec)
    err_code = lib.GasManLastError()

    if result is None:
        raise RuntimeError("GasManJsonToCsv returned NULL.")

    csv_text = result.decode("utf-8")
    if err_code != 0:
        err_msg = lib.GasManErrorString(err_code).decode("utf-8")
        raise RuntimeError(f"Simulation failed [code {err_code}: {err_msg}] — {csv_text}")

    with open(output_file, "w", encoding="utf-8") as f:
        f.write(csv_text)
    print(f"CSV output written to: {output_file}")


if __name__ == "__main__":
    if len(sys.argv) not in (2, 3):
        print(__doc__)
        sys.exit(1)

    _input  = sys.argv[1]
    _output = sys.argv[2] if len(sys.argv) == 3 else "output.csv"
    run(_input, _output)
