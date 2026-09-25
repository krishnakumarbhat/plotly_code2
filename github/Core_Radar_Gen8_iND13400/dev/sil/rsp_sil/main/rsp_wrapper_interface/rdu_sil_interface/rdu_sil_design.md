# RDU SIL — Design Document

**Project:** Gen8 iND13400 Radar Software
**Scope:** RDU (Range-Doppler Unfolding) Software-in-Loop simulation
**Entry point:** `sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test/main.cpp`
**RDU wrapper:** `sil/rsp_sil/main/rsp_wrapper_interface/rdu_sil_interface/rdu_sil_interface.cpp`
**RDU API header:** `sil/rsp_sil/main/rsp_wrapper_interface/rdu_sil_interface/rdu_sil_api.hpp`

---

## 1. Overview

The RDU SIL runs the production RDU pipeline (Stage 1 → Stage 2 → Stage 3
Doppler unfolding) off-target on the host (MinGW/GCC), driven by pre-recorded
binary stimulus. The `rdd_sil_wrapper()` function (in
`rdd_sil_interface/test/main.cpp`) is the **single entry point** for the entire
RSP SIL chain. It reads all input `.bin` files for **one scan**, runs the
RDD pipeline, then PSP, then `RDU_SIL_Configuration`, and logs the RDU outputs
to a text file for verification against embedded/reference outputs.

> **Key point:** There is no separate `rdu_sil_wrapper`. RDU is invoked as
> the final processing step inside `rdd_sil_wrapper()`, after RDD and PSP
> execution. The call order is:
> 1. `RDD_to_Detection_Configuration`
> 2. `Psp_Sil_Wrapper`
> 3. `RDU_SIL_Configuration`

---

## 2. Includes & Global State

### 2.1 Headers included in `main.cpp`

| Header                  | Purpose                                          |
|-------------------------|--------------------------------------------------|
| `af_sil_utils.hpp`      | `Print_AF_Det_Outputs`, `Print_AF_DS_Det_Outputs`|
| `binary_header.h`       | Binary file header definitions                   |
| `dyn_align_wrapper.h`   | Dynamic alignment types                          |
| `psp_sil_api.hpp`       | `Psp_Sil_Wrapper` declaration                    |
| `rdd_sil_api.hpp`       | `RDD_to_Detection_Configuration` declaration     |
| `rdu_sil_api.hpp`       | `RDU_SIL_Configuration` declaration              |
| `sil_wrapper_utils.hpp` | All `Read_*_bin` utility functions               |

### 2.2 Global Status Flags

| Flag                       | Type   | Set by                            | Purpose                              |
|----------------------------|--------|-----------------------------------|--------------------------------------|
| `Look_data_Read_Success`   | `bool` | `Read_Look_Data_Bin`              | Guards Look data read.               |
| `Stream_Header_Read_Success`| `bool`| `Stream_Header_Data_Bin`          | Guards stream header read.           |
| `Rdd_Bin_Read_Success`     | `bool` | `Read_Rdd_Data_Bin`               | Guards RDD bin read.                 |
| `Rdu_Input_Bin_Read_Success`| `bool`| `Read_Rdu_Input_bin`              | Guards RDU input bin read.           |
| `Rdd_Execute_Success`      | `bool` | `RDD_to_Detection_Configuration`  | RDD pipeline pass/fail.              |
| `Rdu_Execute_Success`      | `bool` | `RDU_SIL_Configuration`           | RDU pipeline pass/fail.              |
| `Vse_Bin_Read_Success`     | `bool` | `Read_Vse_Stream_Data_Bin`        | Guards VSE bin read.                 |
| `PSP_Input_Bin_Read_Success`| `bool`| `Read_Psp_Input_bin`              | Guards PSP input bin read.           |
| `PSP_Execute_Pass`         | `bool` | `Psp_Sil_Wrapper`                 | PSP pipeline pass/fail.              |

### 2.3 Compile-time Macros

| Macro              | Effect                                                              |
|--------------------|---------------------------------------------------------------------|
| `LOGGING_ENABLE`   | Defined unconditionally — all `Print_*` functions always active.   |
| `ENABLE_PROFILING` | **Not defined by default** — timing instrumentation inactive.       |
| `SRR8P` / `FLR8`  | Selects variant-specific binary input paths (mutually exclusive).  |
| `PATH_LENGTH`      | `200` — fixed size for all path character arrays.                  |

---

## 3. Binary Input Paths

Variant-specific paths (set at compile time via `#ifdef`):

| Path variable        | FLR8                                    | SRR8P                                    |
|----------------------|-----------------------------------------|------------------------------------------|
| `data_bin_path`      | `sil/rsp_sil/data_bin/flr8/rdd_data`   | `sil/rsp_sil/data_bin/srr8p/rdd_data`   |
| `rfft_bin_path`      | `sil/rsp_sil/data_bin/flr8/rfft_data`  | `sil/rsp_sil/data_bin/srr8p/rfft_data`  |
| `psp_data_bin_path`  | `sil/rsp_sil/data_bin/flr8/psp_data`   | `sil/rsp_sil/data_bin/srr8p/psp_data`   |
| `vse_data_bin_path`  | `sil/rsp_sil/data_bin/flr8/vse_data`   | `sil/rsp_sil/data_bin/srr8p/vse_data`   |
| `rdu_data_bin_path`  | `sil/rsp_sil/data_bin/flr8/rdu_data`   | `sil/rsp_sil/data_bin/srr8p/rdu_data`   |

---

## 4. Block Diagram

```text
+-------------------------------------------------------------------------+
|                         rdd_sil_wrapper()                               |
|                         (rdd_sil_interface/test/main.cpp)               |
|                                                                         |
|  +------------------+  +-------------------+  +---------------------+  |
|  | Read_Look_Data   |  | Stream_Header     |  | Read_Rdd_Data_Bin   |  |
|  |     _Bin         |  |   _Data_Bin       |  |                     |  |
|  +--------+---------+  +--------+----------+  +---------+-----------+  |
|           |                    |                        |               |
|           +--------------------+------------------------+               |
|                                |                                        |
|                                v                                        |
|                     p_rdd_stream_in (Rdd_Stream_T)                      |
|                                                                         |
|  +------------------+  +-------------------+  +---------------------+  |
|  | Read_Vse_Stream  |  | Read_Psp_Input    |  | Read_Rdu_Input_bin  |  |
|  |   _Data_Bin      |  |     _bin          |  |                     |  |
|  +--------+---------+  +--------+----------+  +---------+-----------+  |
|           |                    |                        |               |
|           v                    v                        v               |
|      inp_vse_buff         inp_d2m_buff /          out_af_det_buff       |
|      (Vse_Stream_T)       inp_a2m_buff            (Detection_Stream_T)  |
|                                                                         |
|  +------------------------------------------------------------------+   |
|  |  RDD_to_Detection_Configuration(...)          [Rdd_Execute_Success]  |
|  |  Inputs:  look_id, p_rdd_stream_in, inp_vse_buff                |   |
|  |  Outputs: p_rdd_stream_out, out_af_det_buff,                    |   |
|  |           out_af_Det_debug_buff, out_af_ds_det_buff             |   |
|  +------------------------------------------------------------------+   |
|                                                                         |
|  +------------------------------------------------------------------+   |
|  |  Psp_Sil_Wrapper(...)                         [PSP_Execute_Pass] |   |
|  |  Inputs:  inp_vse_buff, p_rdd_stream_out,                        |   |
|  |           out_af_det_buff, out_af_Det_debug_buff,                |   |
|  |           inp_d2m_buff, inp_a2m_buff, inp_m2d_one_time_buff      |   |
|  +------------------------------------------------------------------+   |
|                                                                         |
|  +------------------------------------------------------------------+   |
|  |  RDU_SIL_Configuration(...)                  [Rdu_Execute_Success]  |
|  |  Inputs:  look_id, out_af_det_buff, inp_vse_buff,               |   |
|  |           inp_d2m_buff, inp_a2m_buff, radar_position_data       |   |
|  |  Output:  p_rdu_stream_out (Rdu_Stream_T)                       |   |
|  +------------------------------------------------------------------+   |
|                                                                         |
|  #ifdef LOGGING_ENABLE  (always active — macro defined unconditionally) |
|    Print_Rdd_Outputs(&p_rdd_stream_out->rdd_data)                       |
|    Print_AF_Det_Outputs(out_af_det_buff, out_af_Det_debug_buff)          |
|    Print_RDU_Outputs(p_rdu_stream_out)  → rdu_out_sil.txt               |
|    #if (DOWNSELECTION_ENABLE == 1U)                                      |
|      Print_AF_DS_Det_Outputs(out_af_ds_det_buff)                         |
|    #endif                                                                |
|    Print_RC_Outputs(&inp_d2m_buff->payload.radar_capability_data)        |
|    Print_SA_Outputs(&inp_d2m_buff->payload.alignment_stream_data)        |
|    Print_DA_Outputs(&inp_d2m_buff->payload.dynamic_alignment_log_stream) |
|  #endif                                                                  |
+-------------------------------------------------------------------------+
```

---

## 5. Flow Diagram

```text
        main()
          |
          | getenv("BUILD_WORKING_DIRECTORY")
          | chdir(bazel_cwd)  if env var present
          v
    rdd_sil_wrapper()
          |
   [allocate on heap + memset to 0]
   ┌─────────────────────────────────────────────────┐
   │ Detection_Stream_T        *out_af_det_buff       │
   │ Vse_Stream_T              *inp_vse_buff          │
   │ Detection_Debug_Stream_T  *out_af_Det_debug_buff │
   │ Down_selection_Stream_T   *out_af_ds_det_buff    │
   │ D2M_Msg_T                 *inp_d2m_buff          │
   │ M2D_Msg_T                 *inp_a2m_buff          │
   │ M2D_One_Time_Msg_T        *inp_m2d_one_time_buff │
   │ Rdd_Stream_T              *p_rdd_stream_in       │
   │ Rdd_Stream_T              *p_rdd_stream_out      │
   │ Rdu_Stream_T              *p_rdu_stream_out      │
   └─────────────────────────────────────────────────┘
          |
   [#ifdef ENABLE_PROFILING]
   start_time_ns = timing_helpers_get_time_nsec()
          |
          v
   Read_Look_Data_Bin(data_bin_path,
       &p_rdd_stream_in->look_data)
     → Look_data_Read_Success
          |
          v
   Stream_Header_Data_Bin(data_bin_path,
       &p_rdd_stream_in->stream_hdr)
     → Stream_Header_Read_Success
          |
          v
   Read_Rdd_Data_Bin(data_bin_path,
       &p_rdd_stream_in->rdd_data)
     → Rdd_Bin_Read_Success
          |
          v
   Read_Vse_Stream_Data_Bin(vse_data_bin_path,
       inp_vse_buff)
     → Vse_Bin_Read_Success
          |
          v
   Read_Psp_Input_bin(psp_data_bin_path,
       inp_d2m_buff, inp_a2m_buff)
     → PSP_Input_Bin_Read_Success
          |
          v
   Read_Rdu_Input_bin(rdu_data_bin_path,
       out_af_det_buff, inp_vse_buff,
       inp_d2m_buff, inp_a2m_buff,
       radar_position_data)
     → Rdu_Input_Bin_Read_Success
          |
          v
   RDD_to_Detection_Configuration(
       (Radar_Look_T)p_rdd_stream_in->look_data.look_id,
       p_rdd_stream_in, p_rdd_stream_out,
       out_af_det_buff, out_af_Det_debug_buff,
       out_af_ds_det_buff, inp_vse_buff,
       &radar_position_data)
     → Rdd_Execute_Success
          |
          v
   Psp_Sil_Wrapper(
       inp_vse_buff, p_rdd_stream_out,
       out_af_det_buff, out_af_Det_debug_buff,
       inp_d2m_buff, inp_a2m_buff,
       inp_m2d_one_time_buff)
     → PSP_Execute_Pass
          |
          v
   RDU_SIL_Configuration(
       look_id,               // RADAR_LOOK_B
       out_af_det_buff,       // Detection_Stream_T*
       inp_vse_buff,          // Vse_Stream_T*
       inp_d2m_buff,          // D2M_Msg_T*
       inp_a2m_buff,          // M2D_Msg_T*
       radar_position_data,   // uint8_t
       p_rdu_stream_out)      // Rdu_Stream_T* OUT
     → Rdu_Execute_Success
          |
   [#ifdef ENABLE_PROFILING]
   end_time_ns = timing_helpers_get_time_nsec()
   printf Total_Rdd_Sil_Execution_Time_Ns
          |
   [#ifdef LOGGING_ENABLE]  ← always active
          v
   Print_Rdd_Outputs(&p_rdd_stream_out->rdd_data)
     → PATH_TO_BIN_FILES/rdd_bv_sil.txt
     → PATH_TO_BIN_FILES/rdd_out_sil.txt
     → PATH_TO_BIN_FILES/rdd_thr_sil.txt
          |
   Print_AF_Det_Outputs(out_af_det_buff,
       out_af_Det_debug_buff)
          |
   Print_RDU_Outputs(p_rdu_stream_out)
     → rdu_out_sil.txt  (hardcoded absolute path — see §8)
          |
   [#if DOWNSELECTION_ENABLE == 1U]
   Print_AF_DS_Det_Outputs(out_af_ds_det_buff)
          |
   Print_RC_Outputs(
       &inp_d2m_buff->payload.radar_capability_data)
     → rc_out_sil.txt
          |
   Print_SA_Outputs(
       &inp_d2m_buff->payload.alignment_stream_data)
     → sa_out_sil.txt
          |
   Print_DA_Outputs(
       &inp_d2m_buff->payload.dynamic_alignment_log_stream)
     → da_out_sil.txt
          |
          v
        return 0
```

---

## 6. RDU SIL API

### 6.1 Signature (`rdu_sil_api.hpp`)

```cpp
bool RDU_SIL_Configuration(
    Radar_Look_T        look_id,
    Detection_Stream_T *p_det_in,
    Vse_Stream_T       *p_vse_in,
    D2M_Msg_T          *p_d2m_data,
    M2D_Msg_T          *p_m2d_data,
    uint8_t             radar_position,
    Rdu_Stream_T       *p_rdu_stream_out);
```

### 6.2 Inputs

| Parameter           | Type                   | Source in `rdd_sil_wrapper`                         | Description                                           |
|---------------------|------------------------|-----------------------------------------------------|-------------------------------------------------------|
| `look_id`           | `Radar_Look_T`         | Local (`RADAR_LOOK_B`)                              | Radar look identifier for the current scan.           |
| `p_det_in`          | `Detection_Stream_T *` | `out_af_det_buff` (populated by `Read_Rdu_Input_bin`) | Current-scan detection list.                        |
| `p_vse_in`          | `Vse_Stream_T *`       | `inp_vse_buff` (from `Read_Vse_Stream_Data_Bin`)    | Vehicle speed / ego-motion data.                      |
| `p_d2m_data`        | `D2M_Msg_T *`          | `inp_d2m_buff` (from `Read_Psp_Input_bin`)          | DSP-to-MCU IPC payload (rdd, det, rdu, rc, sa, da).  |
| `p_m2d_data`        | `M2D_Msg_T *`          | `inp_a2m_buff` (from `Read_Psp_Input_bin`)          | MCU-to-DSP IPC payload (contains veh_data).           |
| `radar_position`    | `uint8_t`              | `radar_position_data` (local, initialized to `0`)   | Radar position identifier.                            |
| `p_rdu_stream_out`  | `Rdu_Stream_T *`       | `p_rdu_stream_out` (heap allocated, memset to 0)    | Populated with Stage1/2/3 unfolding results.          |

### 6.3 Internal `RDU_Data_T` Population (inside `RDU_SIL_Configuration`)

| `RDU_Data_T` field      | Source                                           |
|-------------------------|--------------------------------------------------|
| `p_rdd_data`            | `&p_d2m_data->payload.rdd_stream_data.rdd_data`  |
| `p_det_data`            | `&p_d2m_data->payload.det_data`                  |
| `p_vse_data`            | `&p_m2d_data->payload.veh_data`                  |
| `p_rdu_output_data`     | `&p_d2m_data->payload.rdu_output_data`           |
| `radar_position`        | `radar_posn` (passed in)                         |
| `p_prev_det_data`       | `NULL` (first scan) / `&s_prev_d2m_data.payload.det_data` (subsequent) |

### 6.4 Output

| Output              | Type             | Consumer                              | Description                                     |
|---------------------|------------------|---------------------------------------|-------------------------------------------------|
| `p_rdu_stream_out`  | `Rdu_Stream_T *` | `Print_RDU_Outputs(p_rdu_stream_out)` | Stage1/2/3 unfolding results for the scan.      |
| Return value        | `bool`           | Stored in `Rdu_Execute_Success`        | `true` on successful pipeline execution.        |

---

## 7. Heap Allocations in `rdd_sil_wrapper`

All buffers are heap-allocated with `new` and zero-initialised with `memset`:

| Variable               | Type                       | memset |
|------------------------|----------------------------|--------|
| `out_af_det_buff`      | `Detection_Stream_T *`     | ✓      |
| `inp_vse_buff`         | `Vse_Stream_T *`           | ✓      |
| `out_af_Det_debug_buff`| `Detection_Debug_Stream_T *` | ✓    |
| `out_af_ds_det_buff`   | `Down_selection_Stream_T *`| —      |
| `inp_d2m_buff`         | `D2M_Msg_T *`              | —      |
| `inp_a2m_buff`         | `M2D_Msg_T *`              | —      |
| `inp_m2d_one_time_buff`| `M2D_One_Time_Msg_T *`     | —      |
| `p_rdd_stream_in`      | `Rdd_Stream_T *`           | ✓      |
| `p_rdd_stream_out`     | `Rdd_Stream_T *`           | ✓      |
| `p_rdu_stream_out`     | `Rdu_Stream_T *`           | ✓      |

---

## 8. Output File Summary

| Function              | Output file(s)                                            | Path basis                  |
|-----------------------|-----------------------------------------------------------|-----------------------------|
| `Print_Rdd_Outputs`   | `rdd_bv_sil.txt`, `rdd_out_sil.txt`, `rdd_thr_sil.txt`  | `PATH_TO_BIN_FILES` macro    |
| `Print_AF_Det_Outputs`| (defined in `af_sil_utils.hpp`)                          | —                           |
| `Print_RDU_Outputs`   | `rdu_out_sil.txt`                                        | `PATH_TO_BIN_FILES` macro   |
| `Print_RC_Outputs`    | `rc_out_sil.txt`                                         | Relative (cwd)              |
| `Print_SA_Outputs`    | `sa_out_sil.txt`                                         | Relative (cwd)              |
| `Print_DA_Outputs`    | `da_out_sil.txt`                                         | Relative (cwd)              |


## 9. Logged RDU Output Fields (`Print_RDU_Outputs`)

### Stage 1

| Field label                         | Array / Scalar | Loop bound       |
|-------------------------------------|----------------|------------------|
| `stage1_classification_confidence`  | Array          | `AF_MAX_NUM_DET` |
| `stage1_vx_scs`                     | Scalar         | —                |
| `stage1_vy_scs`                     | Scalar         | —                |
| `stage1_yawrate`                    | Scalar         | —                |
| `stage1_longitudinal_host_velocity` | Scalar         | —                |
| `stage1_lateral_host_velocity`      | Scalar         | —                |
| `stage1_scan_idx`                   | Scalar         | —                |
| `stage1_wrapping_k`                 | Array          | `AF_MAX_NUM_DET` |
| `stage1_rrr_motion_status`          | Array          | `AF_MAX_NUM_DET` |
| `stage1_look_id`                    | Scalar         | —                |
| `K_Unused8_1`                       | Scalar         | —                |

### Stage 2

| Field label                         | Array / Scalar | Loop bound                     |
|-------------------------------------|----------------|--------------------------------|
| `stage2_unfolding_confidence`       | Array          | `AF_MAX_NUM_DET`               |
| `stage2_prev_mov_det_idx`           | Array          | `RDU_S2_MAX_POTENTIAL_MATCHES` |
| `stage2_curr_stat_det_idx`          | Array          | `RDU_S2_MAX_POTENTIAL_MATCHES` |
| `stage2_scan_idx_prev_scan`         | Scalar         | —                              |
| `stage2_scan_idx_curr_scan`         | Scalar         | —                              |
| `stage2_rrr_motion_status`          | Array          | `AF_MAX_NUM_DET`               |
| `stage2_prev_mov_det_k_values`      | Array          | `RDU_S2_MAX_POTENTIAL_MATCHES` |
| `stage2_prev_mov_det_type`          | Array          | `RDU_S2_MAX_POTENTIAL_MATCHES` |
| `stage2_potential_matches_count`    | Scalar         | —                              |
| `stage2_look_id_prev_scan`          | Scalar         | —                              |
| `stage2_look_id_curr_scan`          | Scalar         | —                              |
| `K_Unused8_1`                       | Scalar         | —                              |

### Stage 3

| Field label                         | Array / Scalar | Loop bound       |
|-------------------------------------|----------------|------------------|
| `stage3_rdu_unamb_range_rate`       | Array          | `AF_MAX_NUM_DET` |
| `stage3_unfolding_confidence`       | Array          | `AF_MAX_NUM_DET` |
| `stage3_classification_confidence`  | Array          | `AF_MAX_NUM_DET` |
| `stage3_delta_rr`                   | Array          | `AF_MAX_NUM_DET` |
| `stage3_scan_idx_prev_scan`         | Scalar         | —                |
| `stage3_scan_idx_curr_scan`         | Scalar         | —                |
| `K_Unused16_1`                      | Scalar         | —                |
| `stage3_wrapping_k`                 | Array          | `AF_MAX_NUM_DET` |
| `stage3_rrr_motion_status`          | Array          | `AF_MAX_NUM_DET` |
| `stage3_within_interval`            | Array          | `AF_MAX_NUM_DET` |
| `stage3_look_id_prev_scan`          | Scalar         | —                |
| `stage3_look_id_curr_scan`          | Scalar         | —                |

---

## 10. Memory Pool Architecture (`rdu_sil_interface.cpp`)

Two static SRAM1-equivalent memory pools are allocated once per binary execution:

| Pool Instance           | Size                                  | Init function     |
|-------------------------|---------------------------------------|-------------------|
| `Memory_Pool_instance1` | `RDU_MEMORY_POOL1_ALLOCATION_BYTES` (180 KB) | `Mem_Pool_Init()` |
| `Memory_Pool_instance2` | `RDU_MEMORY_POOL2_ALLOCATION_BYTES` (180 KB) | `Mem_Pool_Init()` |

- `SIL_Mempool_Setup()` is called each invocation but
  `RDU_Handler_Memory_Init()` is guarded by `s_mem_pool_init_done` —
  executed **only once**.

---

## 11. Inter-Scan Static State (`rdu_sil_interface.cpp`)

| Static variable        | Purpose                                                           |
|------------------------|-------------------------------------------------------------------|
| `s_prev_rdu_d2m`       | Pointer to previous scan's D2M data (`NULL` on first scan).      |
| `s_prev_d2m_data`      | Copy of the previous scan's full `D2M_Msg_T`.                    |
| `s_first_scan`         | Flag; cleared after first successful `DopplerUnfolding_Process`. |
| `s_mem_pool_init_done` | Guards one-time `RDU_Handler_Memory_Init()` call.                |

---

## 12. Build & Run

```bash
# Build (FLR8 variant)
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --config=flr8

# Run (Bazel sets BUILD_WORKING_DIRECTORY automatically)
bazelisk run //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --config=flr8
```

Output log written to:
```
PATH_TO_BIN_FILES/rdu_out_sil.txt                        ← RDU
<cwd>/rc_out_sil.txt                                     ← RC
<cwd>/sa_out_sil.txt                                     ← SA
<cwd>/da_out_sil.txt                                     ← DA
PATH_TO_BIN_FILES/rdd_bv_sil.txt                         ← RDD BV
PATH_TO_BIN_FILES/rdd_out_sil.txt                        ← RDD outputs
PATH_TO_BIN_FILES/rdd_thr_sil.txt                        ← RDD thresholds
```
