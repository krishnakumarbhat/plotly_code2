# LogicModel2 — STLA Small LM2 FMU

**Aptiv** | STLA Small Platform | FMI 2.0 Co-Simulation

---

## Overview

LogicModel2 is an FMI 2.0 Co-Simulation FMU that implements the **LRCF (Long-Range Camera Fusion)** algorithm interface for the STLA Small vehicle platform.  
It runs on a **dSPACE SCALEXIO** real-time processing unit and exchanges data via:

- **CAN (FD15)** — 539 signals across RX and TX messages, auto-generated from DBC
- **Ethernet** — single raw frame per step using a 64-bit pointer-triplet mechanism

The FMU is built for **linux64** (SCALEXIO target) and optionally for **win64** (desktop simulation).

---

## Repository Structure

```
STLA_Small_LM2/
├── Development/
│   ├── fmu/                        # FMU source, build scripts, tooling
│   │   ├── LogicModel2.cpp         # Algorithm adapter (smoke-test / replace with LRCF lib)
│   │   ├── LogicModel2.h           # ETH frame structs + step function declaration
│   │   ├── generated_signal_map.cpp  # Auto-generated FMI get/set + DoStep dispatcher
│   │   ├── generated_structs.h       # Auto-generated CAN RX/TX C structs
│   │   ├── modelDescription.xml      # Auto-generated FMU variable catalogue
│   │   ├── fmi2Functions.h         # FMI 2.0 standard header (unmodified)
│   │   ├── dbc_to_fmi_generator.py # Generator: DBC → generated_*.cpp/h + modelDescription.xml
│   │   ├── validate_lrcf_fmu.py    # 4-step FMU validation suite
│   │   ├── fmu_viewer.py           # Inspect FMU variable list
│   │   ├── CMakeLists.txt          # Cross-platform CMake build (Linux + Windows)
│   │   ├── build.sh                # Convenience shell build script (Linux)
│   │   ├── JOB3_LRCF_FD_CAN15.dbc # Source DBC file
│   │   └── LogicModel2.fmu         # Built FMU (do not commit — generated artefact)
│   └── config/
│       └── transmission_config.json
├── CustomFunctions/
│   ├── FmuEthBridge/               # dSPACE SCALEXIO Custom Function (Ethernet bridge)
│   │   ├── FmuEthBridge.xml
│   │   ├── FmuEthBridge.h
│   │   ├── FmuEthBridge.cpp
│   │   ├── FmuEthBridge_TypeDef.h
│   │   └── README.txt
│   ├── ZMQ.*                       # ZeroMQ Custom Function (reference, not used by FMU)
│   └── CustomEthSetup.*            # dSPACE Custom Ethernet Setup CF (reference)
└── Delivery/                       # Customer delivery zips (do not commit large binaries)
```

---

## Quick Start — Build

### Prerequisites

| Tool | Version | Purpose |
|---|---|---|
| g++ / MSVC | C++14 | Compile FMU shared library |
| CMake | ≥ 3.14 | Cross-platform build |
| Python 3 | ≥ 3.8 | Code generation + validation |
| cantools | `pip install cantools` | DBC parsing |
| fmpy | `pip install fmpy` | FMU validation |

### Linux (SCALEXIO target)

```bash
cd Development/fmu

# One-step build + validate
bash build.sh --validate

# OR via CMake
cmake -S . -B build
cmake --build build
cmake --build build -t validate
```

### Windows

```cmd
cd Development\fmu

cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Output is `Development/fmu/LogicModel2.fmu` in both cases.

### Build Options

| CMake cache variable | Default | Description |
|---|---|---|
| `DBC_FILE` | `JOB3_LRCF_FD_CAN15.dbc` | Source DBC file |
| `FMU_NODE` | `LRCF` | DBC node name for CAN TX signal filtering |

---

## FMU Variable Summary

| Group | Count | Variable references |
|---|---|---|
| Parameters | 10 | VR 100–115 |
| CAN RX (Integer) | 139 | VR 1000–1138 |
| CAN RX (Real) | 66 | VR 1139–1204 |
| CAN TX (Integer) | 246 | VR 1205–1450 |
| CAN TX (Real) | 84 | VR 1451–1534 |
| ETH RX pointer | 3 | VR 5000–5002  (`EthRxIn.lo/hi/size`) |
| ETH TX pointer | 3 | VR 5003–5005  (`EthTxOut.lo/hi/size`) |
| **Total** | **551** | |

---

## SCALEXIO Integration

See `CustomFunctions/FmuEthBridge/README.txt` for the complete ConfigurationDesk setup guide.

**One-block setup:**
1. Add `FmuEthBridge` Custom Function to the CD project
2. Assign RX/TX Ethernet hardware ports under **Electrical Interface → Hardware Assignment**
3. Set 8 parameters (ports, IPs, subnet masks)
4. Wire 6 scalar Int32 DataPorts to FMU ETH Integer variables (VR 5000–5005)
5. Wire CAN RX/TX signals to FMU variables via auto-wiring (VCAN_RX / VCAN_TX prefix)

---

## Algorithm Integration

`Development/fmu/LogicModel2.cpp` currently contains a **smoke-test stub**:

- **ETH loopback** — incoming frame echoed back on TX (proves pointer chain)
- **CAN passthrough** — `BSM_DATA_4` dynamics piped to `LRCF_DATA_2` outputs
- **Moving patterns** — sawtooth distance, sine acceleration, rolling E2E counters

Replace the `SMOKE TEST` block in `algo_adapter_step()` with the real `LRCF_process()` call when the algorithm library is available.
