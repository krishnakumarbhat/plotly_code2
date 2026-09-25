# LogicModel2 — Design Document

**Aptiv** | STLA Small LM2 | FMI 2.0 Co-Simulation FMU | v5.6

---

## 1. Purpose

LogicModel2 is a **FMI 2.0 Co-Simulation** shared library that wraps the LRCF algorithm for integration into a dSPACE SCALEXIO real-time environment.  
It provides a standard FMI interface over two physical transports:

- **FD-CAN 15** — structured signal exchange (auto-generated from DBC)
- **Ethernet** — raw frame exchange via pointer-triplet (zero-copy)

---

## 2. Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                    dSPACE SCALEXIO (real-time)                       │
│                                                                       │
│  ┌──────────────────────────┐   ┌──────────────────────────────┐    │
│  │   FmuEthBridge CF        │   │   ConfigurationDesk CAN I/O  │    │
│  │   (Custom Function)      │   │   (built-in SCALEXIO blocks) │    │
│  │                          │   │                              │    │
│  │  recvfrom(RX NIC)        │   │  CAN RX signals              │    │
│  │  → rx_buf                │   │  → FMU Integer/Real inputs   │    │
│  │  → lo/hi/size ──────────► │   │  ◄──────────────────────── │    │
│  │                           │   │                              │    │
│  │  lo/hi/size ◄──────────── │   │  FMU Integer/Real outputs    │    │
│  │  → sendto(TX NIC)         │   │  → CAN TX signals            │    │
│  └──────────────────────────┘   └──────────────────────────────┘    │
│                │                               │                      │
│                ▼                               ▼                      │
│  ┌─────────────────────────────────────────────────────────────┐     │
│  │                    LogicModel2.fmu                           │     │
│  │                                                               │     │
│  │  fmi2DoStep()                                                │     │
│  │    ├─ generated_signal_map.cpp  (get/set CAN + ETH vars)    │     │
│  │    └─ LogicModel2.cpp           (algorithm adapter)          │     │
│  │         ├─ eth_ptr_rx()  → Algo_ETH_RX_Frame_t              │     │
│  │         ├─ LRCF_process() / smoke-test stub                 │     │
│  │         └─ eth_ptr_tx()  → updates inst->eth_tx             │     │
│  └─────────────────────────────────────────────────────────────┘     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 3. FMI Variable Layout

### 3.1 Naming Convention

| Group | FMI name pattern | Type | Notes |
|---|---|---|---|
| Parameters | `param_*` | Real | VR 100–115 |
| CAN RX Integer | `VCAN_RX.<MsgName>.<SignalName>` | Integer | Scale=1, offset=0 |
| CAN RX Real | `VCAN_RX.<MsgName>.<SignalName>` | Real | Has physical scaling |
| CAN TX Integer | `VCAN_TX.<MsgName>.<SignalName>` | Integer | LRCF sender |
| CAN TX Real | `VCAN_TX.<MsgName>.<SignalName>` | Real | LRCF sender, physical scaling |
| ETH RX pointer | `EthRxIn.lo` / `.hi` / `.size` | Integer | VR 5000–5002 |
| ETH TX pointer | `EthTxOut.lo` / `.hi` / `.size` | Integer | VR 5003–5005 |

### 3.2 CAN Signal Filtering

- Signals wider than 32 bits and with physical scaling are mapped to `Real` (double, 52-bit mantissa — lossless).
- Signals wider than 32 bits with scale=1/offset=0 that would overflow `int32_t` are **excluded** (e.g. `DIAGNOSTIC_ROE`, `VIN_INFORMATION`).
- Only messages with sender != LRCF are included as RX; only LRCF-sender messages are included as TX.

---

## 4. Ethernet Pointer-Triplet Mechanism

FMI 2.0 has no native byte-array type. Ethernet frames are exchanged via a **64-bit pointer encoded as two `fmi2Integer` (int32_t) values**:

```
  lo   = (uint32_t)(addr & 0xFFFFFFFF)   // lower 32 bits of pointer
  hi   = (uint32_t)(addr >> 32)          // upper 32 bits of pointer
  size = byte length (0 = no frame this step)
```

**RX path (before fmi2DoStep):**
1. `FmuEthBridge` calls `recvfrom()` → fills `rx_buf` (heap, fixed address)
2. Encodes `&rx_buf` as lo/hi/size → writes to FMU Integer inputs VR 5000–5002
3. FMU reconstructs pointer in `eth_ptr_rx()` → returns `Algo_ETH_RX_Frame_t` with a direct pointer (zero-copy)
4. Algorithm reads frame data directly from master's buffer during DoStep

**TX path (after fmi2DoStep):**
1. Algorithm fills `Algo_ETH_TX_Frame_t.data[]` and sets `status=1`
2. `eth_ptr_tx()` copies into static `g_tx_buf`, encodes address as lo/hi/size → writes to FMU Integer outputs VR 5003–5005
3. `FmuEthBridge` reads lo/hi/size after DoStep → reconstructs pointer → `sendto()`

**Buffer lifetime guarantee:** `rx_buf` is heap-allocated in `FmuEthBridge_Create()` — its address never changes. `g_tx_buf` is a static array inside `LogicModel2.so` — also constant. Both pointers are valid for the entire application lifetime and safe to hand across the FMI boundary.

---

## 5. Code Generation

`dbc_to_fmi_generator.py` reads the source DBC and produces three files on every build:

| Output file | Content |
|---|---|
| `generated_structs.h` | C structs mirroring every RX/TX CAN message |
| `generated_signal_map.cpp` | `fmi2GetReal/Integer`, `fmi2SetReal/Integer`, `fmi2DoStep` dispatch |
| `modelDescription.xml` | FMU variable catalogue consumed by ConfigurationDesk |

**These files must not be edited by hand** — regenerate with `build.sh` or CMake after any DBC change.

---

## 6. File Responsibilities

| File 			| Edit? | Purpose |
|------------------------------|-------|---------|
| `LogicModel2.cpp` 		| **YES** | Algorithm adapter — replace smoke-test stub with `LRCF_process()` |
| `LogicModel2.h` 		| No | Frame structs + step function declaration |
| `generated_signal_map.cpp` 	| No | Auto-generated — regenerated on build |
| `generated_structs.h` 	| No | Auto-generated — regenerated on build |
| `dbc_to_fmi_generator.py` 	| Only for interface changes | Source generator |
| `CMakeLists.txt` 		| No | Build system (Linux + Windows) |
| `build.sh` 			| No | Linux convenience wrapper |
| `validate_lrcf_fmu.py` 	| No | 4-step validation suite |

---

## 7. Build System

### 7.1 CMake (cross-platform)

The CMake build auto-detects the platform and sets the correct FMU binary path:

| Platform | Binary folder | Extension |
|---|---|---|
| Linux | `binaries/linux64` | `.so` |
| Windows | `binaries/win64` | `.dll` |

Windows-specific: `WINDOWS_EXPORT_ALL_SYMBOLS ON` ensures all `extern "C"` FMI entry points are exported from the DLL without a `.def` file.

### 7.2 Build steps (all platforms)

```
Step 1 — Code generation
    python3 dbc_to_fmi_generator.py <DBC> <NODE>
    Output: generated_signal_map.cpp, generated_structs.h, modelDescription.xml

Step 2 — Compile shared library
    g++ / MSVC: generated_signal_map.cpp + LogicModel2.cpp → LogicModel2.so/.dll

Step 3 — Package FMU
    Zip: modelDescription.xml + binaries/<platform>/LogicModel2.so → LogicModel2.fmu
```

---

## 8. Validation

`validate_lrcf_fmu.py` runs 4 checks automatically after every build:

| # | Check | What it verifies |
|---|---|---|
| 1 | XML Schema | modelDescription.xml validates against FMI 2.0 XSD |
| 2 | Model Description | Variable counts (551 total), ETH VR range, naming pattern |
| 3 | Co-Sim DoStep | FMU loads, DoStep executes at status=0 |
| 4 | FMI API (ctypes) | fmi2SetReal/Integer → DoStep → fmi2GetReal/Integer round-trip |

Run: `bash build.sh --validate` or `cmake --build build -t validate`

---

## 9. Customer Delivery

The customer receives `Delivery/STLA_LM2_v<X.Y>_<date>_customer.zip` containing:

| File | Description |
|---|---|
| `LogicModel2.fmu` | Pre-built FMU binary (linux64) |
| `FmuEthBridge.xml` | CF descriptor for ConfigurationDesk |
| `FmuEthBridge.h/cpp` | CF implementation |
| `FmuEthBridge_TypeDef.h` | CF instance struct |
| `README.txt` | Step-by-step integration guide |
