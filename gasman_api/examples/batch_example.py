"""
batch_example.py — Parameter sweep / batch runner for gasmanAPI.

Build a GasMan scenario from parameters, sweep a range of patient weights, run
each simulation into a pandas DataFrame, and print the end-of-run values for
each weight (plus a side-by-side comparison).

Edit the CONFIGURATION block below — that is all you normally need to touch.

Usage:
    python batch_example.py

Requires:
    pip install pandas
    (gasmanAPI.dll / .so / .dylib must be built — see README.md)
"""

import ctypes
import io
import json
import platform
import sys
from pathlib import Path


# ═════════════════════════════════════════════════════════════════════════════
# CONFIGURATION — edit this section
# ═════════════════════════════════════════════════════════════════════════════

# ── Agent ────────────────────────────────────────────────────────────────────
# Must match a section in gasman.ini.  Options:
#   "Sevoflurane", "Desflurane", "Isoflurane", "Enflurane", "Halothane",
#   "Ether", "Nitrogen", "Nitrous Oxide", "Xenon"
AGENT = "Sevoflurane"

# ── Sweep range ──────────────────────────────────────────────────────────────
# Patient weight (kg).  The engine scales its default compartment sizes by
# weight automatically (a weight/70 factor applied during integration), so you
# only set weight here and the compartment sizes follow.
# Use None for an entry to fall back to the gasman.ini default weight (~70 kg).
WEIGHTS = [40.0, 70.0, 100.0, 150.0]

# ── Simulation timing ────────────────────────────────────────────────────────
DURATION_MIN = 10*60+15                  # total simulated minutes
EVERY_SEC    = 10                  # seconds between output rows

# Optional manual integration step, in milliseconds (valid 100–60000).
#   None   -> engine default, allometric based calculation
#   6000   -> force 6-second integration steps
DT_MS = 6000

# ── Time-course config ───────────────────────────────────────────────────────
# Each entry sets the ventilator/vaporiser state from its `time` onward, until
# the next entry overrides it.  Columns:
#   time     HH:MM:SS
#   va       alveolar ventilation (L/min)
#   fgf      fresh gas flow (L/min)
#   co       cardiac output (L/min)
#   circuit  "Semi" | "Closed" | "Open" | "Ideal"
#   del      vaporiser dial setting (%)
#   inject   liquid bolus (mL); 0 = none
# Optional per-entry flags (omit to use whatever the circuit type implies):
#   flush           True -> high-flow flush: the breathing-circuit gas is driven
#                   to the fresh-gas value for this step (FGF shown as 30 L/min)
#                   and any pending liquid injection is cleared.
#   return_enabled  venous blood recirculates to the lungs (default True);
#                   False models a single-pass / no-recirculation patient.
#   uptake_enabled  tissues take up agent from blood (default True); False
#                   freezes uptake so the alveolar level tracks delivery.
#   vapor_enabled   injected liquid agent vaporises into the circuit, enriching
#                   FGF by the dial fraction (default False; liquid agents only).
SETTINGS = [
    {"time": "00:00:00", "va": 4, "fgf": 8, "co": 5, "circuit": "Semi", "del": 3.0},
    {"time": "00:05:00", "va": 4, "fgf": 2, "co": 5, "circuit": "Semi", "del": 1.5},
    {"time": "00:10:00", "va": 4, "fgf": 2, "co": 5, "circuit": "Semi", "del": 1.5, "flush": True},
    {"time": "10:0:00", "va": 3, "fgf": 8, "co": 5, "circuit": "Semi", "del": 0},
]

# ── Advanced overrides — leave as None to use the gasman.ini defaults ──────────
# Compartment volumes (litres).  None -> ini [Volumes], then scaled by weight.
# Provide any subset; unspecified compartments keep their default.
VOLUMES = None
# e.g. VOLUMES = {"vrg": 6.0, "mus": 33.0, "fat": 14.5, "alv": 2.5, "ven": 1.0}

# Fractional blood flows (fraction of cardiac output).  None -> ini [Ratio].
# Keys: vrg, mus, fat  (should sum to ~1.0).
FLOWS = None
# e.g. FLOWS = {"vrg": 0.76, "mus": 0.18, "fat": 0.06}

# Agent pharmacology overrides.  None -> the agent's values from gasman.ini.
# Any subset of:
#   lambdaBlood  blood/gas partition coefficient
#   lambdaVrg    vessel-rich-group / blood solubility
#   lambdaFat    fat / blood solubility
#   lambdaMus    muscle / blood solubility
#   volatility   saturated vapour pressure (mmHg)
#   bottleSize   bottle volume (mL)
#   bottleCost   cost per bottle
AGENT_PARAMS = False
# e.g. AGENT_PARAMS = {"lambdaBlood": 0.65, "lambdaFat": 34}

# Request the engine to add UptakeCost / DeliveredCost columns (off by default).
# Cost = litres of vapour × 1000 * BottleCost / (BottleSize * Volatility).
INCLUDE_COST = False

# Request a MAC column: alveolar concentration in MAC multiples (ALV / agent MAC,
# from gasman.ini).  1.0 = one MAC.  Off by default.
INCLUDE_MAC = True

# Request a DeliveredConc column: the realized delivered concentration (vol %)
# actually entering the circuit each step — a computed result, not the dial
# setpoint you pass as `del`.  Off by default.
INCLUDE_DELCONC = False

# ═════════════════════════════════════════════════════════════════════════════
# Machinery — usually no need to edit below here
# ═════════════════════════════════════════════════════════════════════════════

def load_gasmanAPI():
    """Load gasmanAPI.dll / .so / .dylib and set up its function signatures.

    Assumes the shared library sits in the same folder as this script — copy the
    built gasmanAPI library here before running.
    """
    ext  = {"Windows": ".dll", "Darwin": ".dylib"}.get(platform.system(), ".so")
    name = f"gasmanAPI{ext}"
    path = Path(__file__).resolve().parent / name
    if path.is_file():
        lib = ctypes.CDLL(str(path))
        lib.GasManJsonToCsv.argtypes = [ctypes.c_char_p, ctypes.c_int,
                                        ctypes.c_int, ctypes.c_int, ctypes.c_int]
        lib.GasManJsonToCsv.restype  = ctypes.c_char_p
        lib.GasManLastError.restype  = ctypes.c_int
        lib.GasManErrorString.argtypes = [ctypes.c_int]
        lib.GasManErrorString.restype  = ctypes.c_char_p
        print(f"Loaded: {path}")
        return lib
    raise FileNotFoundError(
        f"Cannot find {name} next to this script. Get the project from Github releases or"
        "build the project first (cmake --build build --config Release).")


def build_scenario(agent=AGENT, weight_kg=None, settings=None,
                   volumes=None, flows=None, agent_params=None,
                   dt_ms=None, include_cost=False, include_mac=False,
                   include_delconc=False, description=""):
    """
    Assemble a GasMan native-format scenario dict from plain parameters.

    Everything is optional — anything left as None falls back to gasman.ini
    (weight_kg=None uses the ini default weight, ~70 kg). 
    """
    if settings is None:
        settings = SETTINGS

    flag_keys = ("flush", "return_enabled", "uptake_enabled", "vapor_enabled")
    native_settings = []
    for s in settings:
        entry = {
            "time":    s.get("time", "00:00:00"),
            "va":      float(s.get("va",  4)),
            "fgf":     float(s.get("fgf", 8)),
            "co":      float(s.get("co",  5)),
            "circuit": s.get("circuit", "Semi"),
            "agent_settings": [{
                "name":   agent,
                "del":    float(s.get("del",    0)),
                "inject": float(s.get("inject", 0)),
            }],
        }
        flags = {k: bool(s[k]) for k in flag_keys if k in s}
        if flags:
            entry["flags"] = flags          # optional; else inferred from circuit
        native_settings.append(entry)

    patient = {}
    if weight_kg is not None:
        patient["weight"] = {"value": float(weight_kg), "unit": "kilograms"}
    if volumes:
        patient["volumes"] = dict(volumes)    # optional per-compartment override
    if flows:
        patient["flows"] = dict(flows)

    agent_obj = {"name": agent}
    if agent_params:
        agent_obj.update(agent_params)         # optional pharmacology override

    params = {"patient": patient, "agents": [agent_obj]}
    if dt_ms is not None:
        params["dt_ms"] = int(dt_ms)          # manual integration step (ms)

    scenario = {"description": description, "params": params,
                "settings": native_settings}
    if include_cost:
        scenario["include_cost"] = True       # ask engine for the cost columns
    if include_mac:
        scenario["include_mac"] = True        # ask engine for the MAC column
    if include_delconc:
        scenario["include_delconc"] = True    # ask engine for the DeliveredConc column
    return scenario


def simulate_df(lib, scenario, start_sec=0, end_sec=600, every_sec=EVERY_SEC):
    """Run one scenario through the engine and return a pandas DataFrame.

    The DLL returns a CSV string (its only output format); pandas parses it
    directly — no temp file or intermediate helper needed.
    """
    import pandas as pd
    payload = json.dumps(scenario).encode("utf-8")
    result  = lib.GasManJsonToCsv(payload, len(payload), start_sec, end_sec, every_sec)
    if result is None:
        raise RuntimeError("GasManJsonToCsv returned NULL.")
    csv_text = result.decode("utf-8")
    if lib.GasManLastError() != 0:
        err = lib.GasManErrorString(lib.GasManLastError()).decode("utf-8")
        raise RuntimeError(f"Simulation failed: {err} — {csv_text}")
    return pd.read_csv(io.StringIO(csv_text))


def _hms_to_min(t: str) -> float:
    h, m, s = (str(t).split(":") + ["0", "0", "0"])[:3]
    return int(h) * 60 + int(m) + int(s) / 60.0


def run_sweep():
    """Run the WEIGHTS sweep; print end-of-run values; return the DataFrames."""
    lib = load_gasmanAPI()      # load the DLL before importing pandas
    import pandas as pd

    end_sec = DURATION_MIN * 60
    print(f"Sweeping {len(WEIGHTS)} weights of {AGENT} over {DURATION_MIN} min\n")

    frames, finals = [], []
    for weight in WEIGHTS:
        scenario = build_scenario(agent=AGENT, weight_kg=weight, dt_ms=DT_MS,
                                  volumes=VOLUMES, flows=FLOWS,
                                  agent_params=AGENT_PARAMS,
                                  include_cost=INCLUDE_COST,
                                  include_mac=INCLUDE_MAC,
                                  include_delconc=INCLUDE_DELCONC)
        df = simulate_df(lib, scenario, end_sec=end_sec, every_sec=EVERY_SEC)

        # Tag every row with the parameters + a numeric time axis.
        df.insert(0, "run_agent",     AGENT)
        df.insert(1, "run_weight_kg", weight)
        df["t_min"] = df["Time"].map(_hms_to_min)

        frames.append(df)
        last = df.iloc[-1]            # final timestep of this run
        finals.append(last)

        wlabel = "ini default" if weight is None else f"{weight:g} kg"
        print(f"── {wlabel} : {len(df)} rows, ends @ {last['Time']} ──")
        print(last.to_string())
        print()

    # Side-by-side comparison of the final state across all weights.  Columns
    # come straight from the DataFrame, so this tracks whatever the engine emits;
    # drop the identifier columns for readability.
    summary = pd.DataFrame(finals).reset_index(drop=True)
    summary = summary.drop(columns=["run_agent", "Agent"], errors="ignore")
    print("Final values across weights:")
    with pd.option_context("display.max_columns", None, "display.width", 240):
        print(summary.to_string(index=False))

    return frames


def main():
    run_sweep()


if __name__ == "__main__":
    main()
