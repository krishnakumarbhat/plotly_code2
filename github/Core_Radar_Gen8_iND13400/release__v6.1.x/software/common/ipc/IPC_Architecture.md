# Gen8 iND13400 — R52 ↔ BBE32 IPC Architecture

## Table of Contents

- [1. Overview](#1-overview)
  - [Radar Timing and Latency](#radar-timing-and-latency)
- [2. Key Source Files](#2-key-source-files)
- [3. IPC Memory Architecture](#3-ipc-memory-architecture)
- [4. Shared Memory Layout Detail](#4-shared-memory-layout-detail)
  - [4a. M2D Master to DSP Buffers](#4a-m2d-master-to-dsp-buffers)
  - [4b. D2M DSP to Master Buffers](#4b-d2m-dsp-to-master-buffers)
  - [4c. Notify Ring Buffers](#4c-notify-ring-buffers)
- [5. IPC Commands](#5-ipc-commands)
  - [Error Fields](#error-fields)
- [6. Hardware Interrupt Mechanism](#6-hardware-interrupt-mechanism)
- [7. Ping Pong Buffer Management](#7-ping-pong-buffer-management)
  - [R52 Side M2D](#r52-side-m2d)
  - [BBE Side D2M](#bbe-side-d2m)
- [8. Detailed Message Sequence](#8-detailed-message-sequence)
- [9. Detailed Flow](#9-detailed-flow)
  - [9a. Boot Initialization](#9a-boot-initialization)
  - [9b. Per Frame 50ms Normal Operation](#9b-per-frame-50ms-normal-operation)
- [10. Error Handling and Recovery](#10-error-handling-and-recovery)
  - [R52 DSP Interaction State Machine](#r52-dsp-interaction-state-machine)
  - [Error Detection Counters](#error-detection-counters)
- [11. ISR Context Notes](#11-isr-context-notes)

---


## 1 Overview

The IPC system connects two processor cores on the Chandra SoC:

- **R52 (ARM)**: Runs AUTOSAR application, MMIC control, vehicle communication
- **BBE32 (Xtensa DSP)**: Runs signal processing — range FFT, Doppler FFT, RDD, angle finding, tracker, Radra Capability.

Communication occurs through **shared SRAM** with **hardware interrupt triggers** in each direction. The cycle repeats every **50ms** (one radar "look").


### Radar Timing and Latency

```
Sensor Latency = X/2 (dwell time) + Y (DFFT) + Z (AF) + ETH_UDP time in ms
```

> 📝 **To edit:** Open `Timing_Diagram.drawio.png` in [draw.io](https://app.diagrams.net/). After editing, export via **File → Export as → PNG** and overwrite `Timing_Diagram.drawio.png`.
![IPC Timing Diagram](Timing_Diagram.drawio.png)


Within each 50ms frame rate, the three cores (R52, BBE, RA) pipeline their work:

- **R52** initiates each look with `R52_TRIGGER_RANGE_PROCESSING_CONFIGURATION`, then later receives completion notifications and forwards data over Ethernet/CAN.
- **BBE** runs the signal processing state machine: Range → Doppler/RDD → FuSa Self-Test → Angle Finding → Radar Capability → Alignment → Tracker → Post-Processing.
- **RA** (Radar Accelerator — not covered here) handles data acquisition and Range/Doppler FFT hardware acceleration.

---


## 2 Key Source Files

| File | Core | Role |
|------|------|------|
| `software/r52/autosar/swc/SWC_PLT_CDD_Radar_Control/Source/SWC_PLT_CDD_Radar_Control.c` | R52 | AUTOSAR runnable that kicks off each 50ms cycle |
| `software/r52/dsp_setup/src/dsp_setup.c` | R52 | DSP lifecycle management: boot/init state machine (`Run_DSP_Interaction_State_Machine`), per-look M2D payload build (`DSP_Processing_Per_Look_Init`), BBE/SoC error recovery and reset |
| `software/r52/ipc/src/ipc_mss.c` | R52 | R52-side IPC transport layer: M2D/D2M buffer management, CRC validation (`Check_BBE32_To_R52_Header`), notify ring dispatch (`Update_R52_IPC_Notify`), hardware trigger (`R52_BBE_Trigger`) |
| `software/bbe32/src/ipc_dsp.c` | BBE | All BBE-side IPC logic: init, ISR handler, send functions |
| `software/bbe32/src/main_application.c` | BBE | BBE state machine that drives processing |
| `software/common/ipc/ipc_data.c` | Shared | Ring buffer and notify buffer management (used by both cores) |
| `software/common/ipc/ipc_data.h` | Shared | All IPC structure definitions (M2D, D2M, notify, etc.) |
| `software/common/ipc/ipc_commands.h` | Shared | IPC command enums, error fields, `Ipc_Notify_Structure_T` |

---


## 3 IPC Memory Architecture

The diagram below shows the shared memory layout and data flow between the two cores. Arrows are color-coded by operation:

- **Blue** — R52 **writes** → M2D data buffers + R52\_TO\_BBE notify ring
- **Orange** — BBE **reads** ← M2D data buffers + R52\_TO\_BBE notify ring
- **Red** — BBE **writes** → D2M data buffers + BBE\_TO\_R52 notify ring
- **Green** — R52 **reads** ← D2M data buffers + BBE\_TO\_R52 notify ring

> 📝 **To edit:** Open `IPC_Memory_Architecture.drawio.png` in [draw.io](https://app.diagrams.net/). After editing, export via **File → Export as → PNG** and overwrite `IPC_Memory_Architecture.drawio.png`.

![IPC Memory Architecture](IPC_Memory_Architecture.png)

---


## 4 Shared Memory Layout Detail

Three memory regions are placed in specific linker sections so both cores see the same physical addresses:


### 4a M2D Master to DSP Buffers

Defined in `ipc_mss.c` as `IPC_M2D_Buffers` (type `IPC_M2D_Buffers_T`):

- **`M2D_One_Time_Msg_T`** — One-time init data:
  - SMC calibration SRAM address
  - USC calibration SRAM address
  - Radar mounting position

- **`M2D_Msg_T[2]`** — Ping-pong double buffer, each containing:
  - `IPC_Header_T` (CRC, size, counter)
  - `M2D_Payload_T`:
    - Look info (scan index, look type)
    - XCP instrumentation variables
    - Vehicle data (speed, yaw, steering angle, gear)
    - Alignment data (static + dynamic)
    - Offline mode flag


### 4b D2M DSP to Master Buffers

Defined in `ipc_dsp.c` as `IPC_D2M_Buffer[2]` (type `D2M_Msg_T[2]`):

- Ping-pong double buffer, each containing:
  - `IPC_Header_T`
  - `D2M_Payload_T`:
    - DSP timing data (per-state profiling, cache info)
    - XCP data
    - RDD stream & debug data
    - Diagnostics (self-test, overrun counts)
    - Angle finding detections & debug
    - Alignment streams (static + dynamic)
    - Radar capability data
    - Tracker output (ROT objects, vehicle info, faults)
    - TOI stream


### 4c Notify Ring Buffers

Defined in `ipc_data.c` as `Ipc_Shared_Buffer` (type `IPC_Shared_Buffers_T`):

- **2 ring buffers**, each with **5 slots** of `Ipc_Notify_Structure_T`:
  - `[R52_TO_BBE_CORE]` — commands from R52 to BBE
  - `[BBE_TO_R52_CORE]` — commands from BBE to R52
- Each slot contains:

  | Field | Description |
  |-------|-------------|
  | `command` | IPC command enum (see §5) |
  | `lookIndex` | Scan/look index for this frame |
  | `errorField` | Error status (see `Error_Field_T`) |
  | `pingPongIndex` | Which ping-pong buffer (0 or 1) |
  | `curIpcAddr` | Pointer to the actual M2D/D2M data buffer |
  | `pfm_index` | Program flow monitor index |

- Read/Write indices per direction, FIFO discipline
- Ring buffer size of 5 allows queueing multiple back-to-back messages (e.g., if the receiver is still in its ISR when the next message arrives)
- Also contains: `D2M_Debug_Msg_T` (idle counter, exception info), `Crc_ModuleStateType`

---


## 5 IPC Commands

Defined in `ipc_commands.h`:

| Command | Value | Direction | Meaning |
|---------|-------|-----------|---------|
| `NO_COMMAND` | 0 | — | No operation |
| `CORE_INITIALIZED` | 1 | BBE→R52, then R52→BBE | Boot handshake |
| `R52_TRIGGER_RANGE_PROCESSING_CONFIGURATION` | `0x10000000` | R52→BBE | Start of each 50ms frame |
| `DSP_RANGE_CONFIGURATION_COMPLETE` | `0x20000000` | BBE→R52 | Range configuration finished |
| `DSP_RANGE_PROCESSING_COMPLETE` | `0x20000001` | BBE→R52 | Range FFT finished |
| `DSP_DOPPLER_PROCESSING_COMPLETE` | `0x20000002` | BBE→R52 | Doppler FFT + RDD finished |
| `DSP_SP_POST_PROCESSING_COMPLETE` | `0x20000003` | BBE→R52 | All signal processing done |


### Error Fields

| Error | Meaning |
|-------|---------|
| `ERRORFIELD_NO_ERROR` | Success |
| `ERRORFIELD_RANGE_PROCESS_CONFIG_ERROR` | Range config failed on BBE |
| `ERRORFIELD_RANGE_PROCESS_ERROR` | Range processing error |
| `ERRORFIELD_RANGE_PROCESS_TIMEOUT` | Range processing timed out |
| `ERRORFIELD_DOPPLER_PROCESS_NOT_RUN` | Doppler skipped (range failed) |
| `ERRORFIELD_DOPPLER_PROCESS_TIMEOUT` | Doppler timed out |
| `ERRORFIELD_DOPPLER_PROCESS_ERROR` | Doppler processing error |
| `ERRORFIELD_DOPPLER_PROCESS_MEMPOOL_FAILED` | Memory pool allocation failed |
| `ERRORFIELD_POST_SIG_PROC_ERROR` | Post signal processing error |
| `R52_TO_BBE32_CRC_ERROR` (`0x70A0A0A0`) | CRC validation failure |

---


## 6 Hardware Interrupt Mechanism

| Direction | Trigger Code | Register | ISR Handler |
|-----------|-------------|----------|-------------|
| **R52 → BBE** | `DSP->SP2DSP_INT.f.SP2DSP_LVL1_IRQ = 0x01` | SP2DSP Level 1 IRQ | `SP2DSP_LVL1_Handler()` in `ipc_dsp.c` → calls `Update_DSP_IPC_Notify()` |
| **BBE → R52** | `DSP->IRQ_ENA = 0x01` + `WUR_EXPSTATE(0x0001)` | GPIO0 IRQ | `Update_R52_IPC_Notify()` in `ipc_mss.c` |

The pattern is always:

1. **Write data** to shared M2D or D2M buffer
2. **Write notify entry** to ring buffer slot at `WriteIndx`
3. **Increment `WriteIndx`** (just before the trigger to avoid race conditions)
4. **Fire hardware interrupt** to the other core

---


## 7 Ping Pong Buffer Management

Both M2D and D2M use 2-buffer ping-pong schemes to allow concurrent read/write:


### R52 Side M2D

`R52_M2D_Buffer_Index` toggles (`^= 1`) each frame in `DSP_Processing_Per_Look_Init()`. R52 writes to one buffer while BBE can still read the other.


### BBE Side D2M

Three separate index variables track which ping-pong buffer each processing stage uses:

| Variable | Stage | Inherits From |
|----------|-------|---------------|
| `RP_Buffer_Index` | Range processing | Swapped on new frame trigger (`^= 1`) |
| `DP_Rdd_Buffer_Index` | Doppler/RDD | Copies from `RP_Buffer_Index` after range complete |
| `SP_Post_Proc_Buffer_Index` | Post-processing (AF, tracker, etc.) | Copies from `DP_Rdd_Buffer_Index` after doppler complete |

The buffer index is passed via `pingPongIndex` in the notify structure so each side knows which slot to read.

---


## 8 Detailed Message Sequence

> 📝 **To edit:** Open `IPC_Message_Sequence.drawio.png` in [draw.io](https://app.diagrams.net/). After editing, export via **File → Export as → PNG** and overwrite `IPC_Message_Sequence.drawio.png`.

![IPC Message Sequence](IPC_Message_Sequence.png)

---


## 9 Detailed Flow


### 9a Boot Initialization

```
R52: Run_DSP_Interaction_State_Machine() — DSP_CORE_INITIALIZE state
  │
  ├─ DSP_Processing_Onetime_Init()        [dsp_setup.c]
  │    ├─ IPC_M2D_Buffer_Init()           [ipc_mss.c] — zero out M2D ping-pong
  │    ├─ IPC_M2D_One_Time_Buffer_Init()  [ipc_mss.c] — zero out one-time struct
  │    ├─ IPC_Notify_Buffer_Init()        [ipc_data.c] — zero out all ring buffers
  │    └─ Fill M2D_One_Time_Msg with SMC/USC cal pointers + radar position
  │
  ├─ Release_BBE32_From_Stall()           [dsp_setup.c] — RUNSTALL = 0
  └─ State → DSP_CORE_INITIALIZING

BBE: (released from stall)
  ├─ Init_Handler() → memory pools, ECC, one-time init
  ├─ Appl_DSP_Process_Onetime_Init()
  │    ├─ Register SP2DSP_LVL1_Handler as ISR for interrupt 8
  │    ├─ Send_Core_Initialization()      — CORE_INITIALIZED via notify + HW IRQ
  │    └─ Wait_For_Trigger() — blocks until R52 echoes back
  │
  R52: (ISR fires) → Update_R52_IPC_Notify()  [ipc_mss.c]
  │    ├─ Reads CORE_INITIALIZED
  │    ├─ Echoes CORE_INITIALIZED back with M2D_One_Time address
  │    ├─ DSP_Init = true
  │    └─ R52_BBE_Trigger()                    [ipc_mss.c] — fires SP2DSP interrupt
  │
  BBE: (ISR fires) → reads CORE_INITIALIZED
       ├─ Saves SMC_Cal_Data_Ptr, USC_Cal_Data_Ptr from M2D_One_Time
       ├─ Ready_For_New_Frame = READY_FOR_RANGE_PROCESSING
       └─ IPC_D2M_Buffer_Init() — zero out D2M ping-pong
```


### 9b Per Frame 50ms Normal Operation

The AUTOSAR scheduler calls `RE_Radar_Ctl_Look_Trigger()` → `MMIC_Helper_Function()` every 50ms in `SWC_PLT_CDD_Radar_Control.c`, which calls `Run_DSP_Interaction_State_Machine()`.

**Step 1: R52 sends `R52_TRIGGER_RANGE_PROCESSING_CONFIGURATION`**

In `DSP_Processing_Per_Look_Init()`:

1. Check `Received_Doppler_Done` (ensures previous frame's doppler completed)
2. Check notify buffer not full
3. Fill `M2D_Msg_T[R52_M2D_Buffer_Index]` with current look info, vehicle data, XCP instrumentation, alignment data
4. Fill notify entry: `command = R52_TRIGGER_RANGE_PROCESSING_CONFIGURATION`, `curIpcAddr` pointing to the M2D buffer, `pingPongIndex`, `lookIndex`
5. Toggle `R52_M2D_Buffer_Index ^= 1`
6. `Increment_IPC_Notify_WriteIndx(R52_TO_BBE_CORE)`
7. `R52_BBE_Trigger()` (`ipc_mss.c`) — fires `SP2DSP_LVL1_IRQ`
8. **Busy-wait** up to 2ms for `DSP_Cfg_Init` to be set by BBE's response

**Step 2: BBE receives trigger, configures range processing**

`SP2DSP_LVL1_Handler()` fires → `Update_DSP_IPC_Notify()`:

1. Reads notify entry, gets `M2D_Msg_T*` from `curIpcAddr`
2. Validates header (CRC placeholder)
3. Swaps `RP_Buffer_Index ^= 1` to select fresh D2M buffer
4. Clears D2M RDD/debug/timing buffers
5. Calls `Appl_Range_Process_Trigger()` — configures RA hardware, sets `IPC_R52_To_BBE_Trigger_Received = TRIGGER_RECEIVED`
6. Writes `DSP_RANGE_CONFIGURATION_COMPLETE` to notify[BBE_TO_R52]
7. Fires BBE→R52 interrupt

**Step 3: R52 unblocks, enables MIPI for chirp acquisition**

`Update_R52_IPC_Notify()` reads `DSP_RANGE_CONFIGURATION_COMPLETE`, sets `DSP_Cfg_Init = DSP_CONFIG_NO_ERROR`. The busy-wait exits. R52 configures MIPI and starts chirps.

**Step 4: BBE processes — Range → Doppler → AF → Post-processing**

The BBE state machine in `Main_Application()` loops:

| State | Handler | Sends to R52 |
|-------|---------|-------------|
| IDLE | `Idle_Handler()` — polls `IPC_R52_To_BBE_Trigger_Received` | — |
| RANGE_PROCESSING | `Wait_For_Rfft_Complete_Handler()` — waits for all chirps | `DSP_RANGE_PROCESSING_COMPLETE` |
| DOPPLER_AND_RDD_PROCESSING | `Doppler_Handler()` — DFFT + RDD detection | `DSP_DOPPLER_PROCESSING_COMPLETE` (**+ sets `Ready_For_New_Frame = READY`** enabling overlap) |
| FUSA_BBE_SELF_TEST | `Fusa_BBE_Self_Test_Handler()` — safety self-test | — |
| ANGLE_FINDING | `Anglefinding_Handler()` — beam forming | — |
| RADAR_CAPABILITY | `Radar_Performance_Degradation_Monitor_Handler()` | — |
| TOI_ALIGNMENT | `TOI_Alignment_Handler()` — static + dynamic alignment | — |
| TRACKER | `Tracker_Handler()` — F360 radar tracker | — |
| SIGPROC_POST_PROCESSING | `SP_Post_Proc_Complete_Handler()` | `DSP_SP_POST_PROCESSING_COMPLETE` |
| → IDLE | cycle repeats | — |

> 📝 **To edit:** Open `BBE32_statediagram.drawio.png` in [draw.io](https://app.diagrams.net/). After editing, export via **File → Export as → PNG** and overwrite `BBE32_statediagram.drawio.png`.

![BBE32 State Diagram](BBE32_statediagram.drawio.png)

> **Key overlap**: After sending `DSP_DOPPLER_PROCESSING_COMPLETE`, BBE sets `Ready_For_New_Frame = READY`. This means the **next frame's range processing can be triggered by R52 while BBE is still doing AF/tracker/post-processing** on the current frame. This is the pipelined overlap where "Angle Finding/Tracker(N)" overlaps with "Data Acq & RFFT(N+1)".

**Step 5: R52 processes BBE responses**

On each BBE→R52 interrupt, `Update_R52_IPC_Notify()` drains the ring buffer:

| Command | R52 Action |
|---------|------------|
| `DSP_RANGE_CONFIGURATION_COMPLETE` | Sets DSP_Cfg_Init = DSP_CONFIG_NO_ERROR. The busy-wait exits. R52 configures MIPI and starts chirps |
| `DSP_RANGE_PROCESSING_COMPLETE` | Triggers AUTOSAR look-complete event, populates ADC stream |
| `DSP_DOPPLER_PROCESSING_COMPLETE` | Sets `Received_Doppler_Done = true` (gates next frame trigger), populates RDD stream, triggers CAN Tx |
| `DSP_SP_POST_PROCESSING_COMPLETE` | Populates detection/header/capability streams, triggers final CAN Tx |

---


## 10 Error Handling and Recovery


### R52 DSP Interaction State Machine

The R52 `Run_DSP_Interaction_State_Machine()` implements a 3-state watchdog:

```
DSP_CORE_INITIALIZE → DSP_CORE_INITIALIZING → DSP_CORE_NORMAL_OPERATION
                              ↑                        │
                              └────── on failure ───────┘
```

- If BBE fails to respond within timeout or `DSP_Processing_Per_Look_Init()` returns false, `Handle_Missed_DSP_Frame()` increments `DSP_Processing_Error_Counter`
- After **5 consecutive failures** (`DSP_Processing_Error_Counter_Max`): reset BBE via `Perform_BBE32_Reset()` (stall + CRG reset of core/bank/XOCD), return to `DSP_CORE_INITIALIZE`
- After **10 BBE resets** (`DSP_Reset_Counter_Max`): full `Perform_Chandra_Reset()` (resets entire SoC)
- On any successful frame, both counters reset to 0


### Error Detection Counters

| Counter | Location | Triggers |
|---------|----------|----------|
| `DSP_Range_Config_Counts.error_count` | R52 | BBE response `DSP_RANGE_CONFIGURATION_COMPLETE` received with `errorField = ERRORFIELD_RANGE_PROCESS_CONFIG_ERROR` |
| `DSP_Range_Process_Complete_Counts.error_count` | R52 | BBE response `DSP_RANGE_PROCESSING_COMPLETE` received with non-zero `errorField` |
| `DSP_Doppler_Process_Complete_Counts.error_count` | R52 | BBE response `DSP_DOPPLER_PROCESSING_COMPLETE` received with non-zero `errorField` |
| `DSP_Sp_Post_Complete_Counts.error_count` | R52 | BBE response `DSP_SP_POST_PROCESSING_COMPLETE` received with non-zero `errorField` |
| `IPC_M2D_Crc_Err_Cnt` | R52 | CRC error in BBE→R52 response |
| `M2D_Buffer_Full_Error_Counter` | R52 | Indicates R52→BBE notify ring buffer full, when trying to send message |
| `M2D_DSP_Not_Ready_Counter` | R52 | `Received_Doppler_Done` still false (BBE still processing) |
| `Dsp_Cfg_Error_Counter` | R52 | Config response timeout or error |
| `D2M_Buffer_Full_Error_Counter` | BBE | Indicates BBE→R52 notify ring buffer full, when trying to send message |
| `IPC_M2D_Buffer_Err_Cnt` | BBE | M2D header/CRC validation failure |
| `IPC_R52_Trigger_Failed` | BBE | Range configuartion trigger failed |

---


## 11 ISR Context Notes

### D2M Cache Writeback Before Triggering R52

`IPC_D2M_Buffer` is declared in cacheable SRAM (`.ipc_dsp_to_r52` section). The BBE32 data cache may hold modified lines not yet flushed to physical SRAM. If R52 is triggered **before** a cache writeback, it reads **stale data**.

`xthal_dcache_block_writeback()` before BBE32 to R52 trigger ensures D2M IPC data is flushed to SRAM

### BBE Send SP Post Proc Complete to R52

- Explicitly disables interrupts (`xthal_disable_interrupts()`) while manipulating the notify buffer to prevent race conditions with the ISR that could fire during the AF/tracker overlap period.

**Why this race exists:**

`Send_SP_Post_Proc_Complete_to_R52()` runs on the **BBE main loop** (not in ISR context) during the `SIGPROC_POST_PROCESSING` state. At this same time, because `Ready_For_New_Frame = READY` was set after Doppler, R52 may have already sent the **next frame's** `R52_TRIGGER_RANGE_PROCESSING_CONFIGURATION`. This means `SP2DSP_LVL1_Handler` (the ISR) could fire at any moment.

Both paths write to the **same** `BBE_TO_R52` notify ring buffer:

Without interrupt masking, the ISR could preempt the main loop **between** `Get_IPC_Notify_Buffer(WRITE_BUFFER)` and `Increment_IPC_Notify_WriteIndx()`, causing both to grab the **same write slot** and one message to silently overwrite the other.

> **Note:** `Send_Range_Proc_Complete()` and `Send_Doppler_RDD_Proc_Complete()` do **not** disable interrupts . They are safe by timing — both are called before `Ready_For_New_Frame = READY`, so the next-frame ISR cannot yet fire. `Send_SP_Post_Proc_Complete_to_R52()` is the only one called **after** the overlap window opens.

