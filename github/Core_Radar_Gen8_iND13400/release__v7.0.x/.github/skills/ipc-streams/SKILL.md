---
description: "Inter-core communication (IPC) and stream system for Gen8 dual-core radar (R52↔BBE32)"
---

# IPC & Streams Skill — Gen8 Radar

## Architecture Overview

```
R52 (ARM Cortex-R52)          BBE32 (Xtensa DSP)
├── AUTOSAR SWCs              ├── Signal Processing (SPBB)
├── Communication (CAN/ETH)   ├── Angle Finding (AFBB)
├── Diagnostics               ├── RDD Processing
├── Radar Control             ├── Dynamic Alignment
└── dsp_setup (IPC sender)    └── ipc_dsp (IPC receiver)
         │                              │
         └──── M2D (Master→DSP) ────────┘
         ┌──── D2M (DSP→Master) ────────┐
         │                              │
```

## IPC Data Structure

**File:** `software/common/ipc/ipc_data.h`

- `SIT_Data_T` (at ~line 226) — Integration Testing payload
- M2D payloads: R52 sends config/commands to BBE32
- D2M payloads: BBE32 sends results back to R52

## IPC Hook Points (from repo)

| Hook | Source File | Direction |
|------|------------|-----------|
| `SIT_IPC_TRANSFER_M2D_SEND` | `r52/dsp_setup/src/dsp_setup.c` | R52→BBE32 |
| `SIT_IPC_TRANSFER_M2D_RECEIVE` | `bbe32/src/ipc_dsp.c` | BBE32 receives |
| `SIT_IPC_TRANSFER_D2M_SEND` | `bbe32/src/ipc_dsp.c` | BBE32→R52 |
| `SIT_M2D_ONETIME` | `r52/dsp_setup/src/dsp_setup.c` | One-time config |
| `SIT_M2D_PERLOOK` | `r52/dsp_setup/src/dsp_setup.c` | Per-look config |

## Stream System

### Purpose
Streams are structured UDP data packets sent from the sensor to an external PC for logging/debugging. Each stream has a defined header + payload format.

### Stream Header (`software/common/stream_header.h`)
- `Stream_Hdr_T` — common header for all streams
- `LOG_DATA_HDR_LEN` = `sizeof(Stream_Hdr_T)`
- Little-endian structure layout

### Stream Types (from `software/common/`)

| Stream File | Content |
|-------------|---------|
| `bsis_stream.h` | BSIS data |
| `calib_stream.h` | Calibration data |
| `cdc_stream.h` | CDC frames |
| `debug_stream.h` | Debug output |
| `header_stream.h` | Stream header definitions |
| `mmic_stream.h` | MMIC raw data |
| `mois_stream.h` | MOIS data |
| `olp_stream.h` | OLP data |
| `rdd_debug_stream.h` | RDD debug |
| `status_stream.h` | Status info |
| `toi_stream.h` | TOI data |
| `toi_ds_stream.h` | TOI DS data |
| `vid_stream.h` | Video/image data |
| `vse_stream.h` | VSE data |

### Stream Generation
- Controlled by flag: `--enable_stream_generation=True`
- Tool: `@StreamHeader_Gen` (generates stream definitions + Wireshark dissector)
- Output: `outputs/*/streamdefs/` + `wiresharkDissector.lua`
- Bandwidth report: `Stream_Bandwidth_Info` rule in root BUILD

### Stream Bandwidth Analysis
- Rule: `Stream_Bandwidth_Info` in root BUILD
- Input: generated stream definitions
- Output: CSV file (`*_Stream_Bandwidth_Details.csv`)
- Per-variant ID: FLR8=082, SRR8P=083

## Radar Look Types

**File:** `software/common/radar_look_types.h`
- Defines the look structure used across both cores
- Shared between R52 scheduling and BBE32 processing

## Key IPC Files

| File | Purpose |
|------|---------|
| `software/common/ipc/ipc_data.h` | IPC payload structures |
| `software/r52/dsp_setup/src/dsp_setup.c` | R52 side IPC sender |
| `software/bbe32/src/ipc_dsp.c` | BBE32 side IPC receiver/sender |
| `software/common/stream_header.h` | Common stream header |
| `software/common/cdc_frame_interface.h` | CDC frame interface |

## Adding New Stream

1. Create header in `software/common/my_stream.h`
2. Define payload structure after `Stream_Hdr_T`
3. Add source ID constant
4. Register in stream handler (`SWC_PLT_CDD_Logging`)
5. Rebuild with stream generation: `--enable_stream_generation=True`
6. Verify in generated Wireshark dissector

## Common Patterns

### IPC Payload Access (BBE32 side)
```c
/* In ipc_dsp.c — receiving M2D data */
void IPC_Receive_M2D(const M2D_Payload_T* payload) {
    /* Process per-look config from R52 */
}
```

### Stream Logging (R52 side)
```c
/* Typical stream write pattern */
Stream_Hdr_T hdr;
hdr.source_id = MY_STREAM_ID;
hdr.length = sizeof(My_Payload_T);
/* Write to stream handler */
```
