# Build Guide — LogicModel2 FMU

**Aptiv** | STLA Small LM2 | FMI 2.0 Co-Simulation FMU

---

## Prerequisites

### Linux

| Tool | Version | Install |
|---|---|---|
| GCC / G++ | ≥ 9 | `sudo apt install g++` |
| CMake | ≥ 3.14 | `sudo apt install cmake` |
| Python 3 | ≥ 3.8 | `sudo apt install python3` |
| cantools | any | `pip3 install cantools` |
| fmpy | any (validate only) | `pip3 install fmpy` |

### Windows

| Tool | Version | Notes |
|---|---|---|
| Visual Studio | 2019 or 2022 | Install "Desktop development with C++" workload |
| CMake | ≥ 3.14 | bundled with VS, or [cmake.org](https://cmake.org) |
| Python 3 | ≥ 3.8 | [python.org](https://www.python.org) — add to PATH |
| cantools | any | `pip install cantools` |

---

## Quick Start

Code generation and compilation are **independent steps**. Only regenerate when the DBC changes.

### Linux

**First time, or after a DBC change:**
```bash
cd Development/fmu/Scripts
bash build.sh --generate ../Code/JOB3_LRCF_FD_CAN15.dbc
```

**Subsequent builds (source code changes only):**
```bash
bash build.sh
```

**With validation:** `bash build.sh --validate`

---

### Windows

Open a **Developer Command Prompt for Visual Studio**, then:

**First time, or after a DBC change:**
```cmd
cd Development\fmu\Scripts
build.bat --generate ..\Code\JOB3_LRCF_FD_CAN15.dbc
```

**Subsequent builds (source code changes only):**
```cmd
build.bat
```

**With validation:** `build.bat --validate`

**Clean build outputs only (keep generated sources):** `build.bat --clean`

**Clean everything and regenerate:** `build.bat --clean --generate ..\Code\JOB3_LRCF_FD_CAN15.dbc`

---

## Build Stages in Detail

### Stage 1 — Code Generation (run only when DBC changes)

Generation is an **explicit separate step** — not triggered automatically on every build.

```bash
# Linux
bash Scripts/build.sh --generate Code/JOB3_LRCF_FD_CAN15.dbc

# Windows
Scripts\build.bat --generate Code\JOB3_LRCF_FD_CAN15.dbc
```

The DBC path must be provided explicitly. To override the node name (default `LRCF`):
```bash
bash Scripts/build.sh --generate Code/my.dbc --node MY_NODE
```

**Writes (do not edit by hand):**

| File | Description |
|---|---|
| `generated_structs.h` | C structs for every CAN RX/TX message |
| `generated_signal_map.cpp` | `fmi2Get/Set` + `fmi2DoStep` dispatch table |
| `modelDescription.xml` | FMU variable catalogue (551 variables) |

The generator filters signals by node name:
- Signals **sent by the node** → FMI **outputs** (`VCAN_TX.*`)
- Signals **received by the node** → FMI **inputs** (`VCAN_RX.*`)

If generated files are missing when running without `--generate`, the build fails with a clear error.

---

### Stage 2 — Compile Shared Library

Sources compiled into the shared library:

| Source | Role |
|---|---|
| `generated_signal_map.cpp` | FMI entry points (auto-generated — do not edit) |
| `LogicModel2.cpp` | Algorithm adapter **(edit this file for LRCF integration)** |

**Compiler flags:**
- Linux/GCC: `-Wall -Wno-unused-parameter -O2 -fPIC`
- Windows/MSVC: `/W3 /O2` + `WINDOWS_EXPORT_ALL_SYMBOLS ON`

C++ standard: **C++14**

---

### Stage 3 — Package FMU

The FMU is a ZIP archive with a mandatory directory structure:

```
LogicModel2.fmu
├── modelDescription.xml
└── binaries/
    ├── linux64/
    │   └── LogicModel2.so      ← Linux build
    └── win64/
        └── LogicModel2.dll     ← Windows build
```

Both platform binaries can coexist in the same FMU. To produce a multi-platform FMU, build on Linux and Windows separately and merge the `binaries/` folders before zipping.

---

## CMake

CMake is used internally by `build.bat` on Windows. No CMake options are required for normal use — run `build.bat` instead.

For advanced use (CI pipelines, IDE integration), call CMake directly after generating sources:
```cmd
cmake -S Code -B cmake_build -G "Visual Studio 17 2022" -A x64
cmake --build cmake_build --config Release
```

CMake will fail with a clear `FATAL_ERROR` if the generated files (`generated_signal_map.cpp`, `generated_structs.h`, `modelDescription.xml`) are missing.

---

## Integrating the LRCF Algorithm

`LogicModel2.cpp` contains a smoke-test stub. To replace it with the real algorithm:

1. Add the LRCF static library (`.a` / `.lib`) and its headers to `Development/fmu/`.
2. In `CMakeLists.txt`, add the include path and link the library:
   ```cmake
   target_include_directories(LogicModel2 PRIVATE path/to/lrcf/include)
   target_link_libraries(LogicModel2 PRIVATE path/to/lrcf/liblrcf.a)
   ```
3. In `LogicModel2.cpp`, replace the stub in `LogicModel2_step()` with:
   ```cpp
   LRCF_process(&rx.data[0], rx.size, &tx.data[0], &tx_size, &inst->can_rx, &inst->can_tx);
   tx.size   = tx_size;
   tx.status = (tx_size > 0) ? 1 : 0;
   ```
4. Rebuild:
   ```bash
   # Linux
   bash Scripts/build.sh

   # Windows
   Scripts\build.bat
   ```

The CAN struct fields are in `generated_structs.h` (auto-generated from DBC).

---

## Validation

After a successful build:

```bash
# Linux
bash Scripts/build.sh --validate

# Windows (Developer Command Prompt)
Scripts\build.bat --validate
```

The validation script (`validate_lrcf_fmu.py`) runs 4 checks:

| # | Check | Pass criteria |
|---|---|---|
| 1 | XML Schema | `modelDescription.xml` validates against FMI 2.0 XSD |
| 2 | Variable counts | 551 total, ETH VRs 5000–5005 present |
| 3 | DoStep status | `fmi2DoStep` returns `fmi2OK` |
| 4 | Signal round-trip | `fmi2SetInteger/Real` → `DoStep` → `fmi2GetInteger/Real` |

---

## Build Artifacts (not committed to Git)

| Artifact | Description |
|---|---|
| `generated_signal_map.cpp` | Auto-generated — recreated only when running `--generate` |
| `generated_structs.h` | Auto-generated — recreated only when running `--generate` |
| `modelDescription.xml` | Auto-generated — recreated only when running `--generate` |
| `LogicModel2.fmu` | Build output |
| `LogicModel2.so` / `.dll` | Intermediate shared library |
| `cmake_build/` | CMake build directory (Windows) |
