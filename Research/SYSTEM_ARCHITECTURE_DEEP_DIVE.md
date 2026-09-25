# SYSTEM ARCHITECTURE DEEP DIVE — ADAS Radar Re-Simulation (Resim) & KPI Framework

> Workspace: `C:\Users\ouymc2\Desktop\Research` — 11 git repos audited (9 populated + 2 empty placeholders; 2 repos added 2026-09-22: `core-radar-gen7-awr294x`, `core-resim-logic-model`).
> Author: Senior Principal ADAS Radar Systems Architect. Dates: 2026-09-20 (main body) + 2026-09-23 (Appendix A multi-branch + new-repo update).
> Scope: end-to-end data flow from 77 GHz FMCW physics to CAN/UDP KPI regression verdicts,
> mapped to exact source files, structs, functions, build targets, and config schemas found in the workspace.
> All paths below are REAL (verified by direct directory reads on 2026-09-20).

---

## TABLE OF CONTENTS

1. [Executive Summary & Repository Ecosystem Map](#1-executive-summary--repository-ecosystem-map)
2. [Physical & Hardware Ingestion Layer](#2-physical--hardware-ingestion-layer)
3. [Binary Formats, Bus Protocols, and Signal Decoding](#3-binary-formats-bus-protocols-and-signal-decoding)
4. [Inside the Embedded Radar Algorithm Pipeline (EmLib)](#4-inside-the-embedded-radar-algorithm-pipeline-emlib)
5. [The Re-Simulation (Resim) Harness — SiL & HiL](#5-the-re-simulation-resim-harness-sil--hil)
6. [The KPI Verification Suite & Mathematical Verification Engine](#6-the-kpi-verification-suite--mathematical-verification-engine)
7. [Build System Reference](#7-build-system-reference)
8. [Failure-Mode & Traceability Index](#8-failure-mode--traceability-index)
9. [Glossary Pointer](#9-glossary-pointer)
10. [Appendix A — Multi-Branch Snapshots & New Repos](appendix-a--multi-branch-snapshot-manifest--new-repo-deep-dives-audited-2026-09-23)
11. [Appendix C — File-Level Atlas](appendix-c--file-level-atlas-audited-2026-09-24-all-refs-verified-via-git-showls-tree)

> Note: Appendix B (multi-branch investigation update) lives in the companion file RESEARCH_INVENTIONS_AND_ROADMAP.md.

---

## 1. Executive Summary & Repository Ecosystem Map

### 1.1 What this system is

A complete closed-loop radar regression facility for Aptiv Gen7 (NXP SAF85xx) and Gen8 (indie iND13400)
corner/front radars. A vehicle records baseline bus traffic (CAN-FD + UDP Ethernet) into `.mf4` (ASAM MDF4).
Offline, the **Resim** facility replays the recording through a *candidate* embedded build
(signal processing + tracker + domain-controller fusion + feature functions) inside a Software-in-the-Loop
(SiL) or Hardware-in-the-Loop (HiL) harness, producing an **output `.mf4`**. The **KPI suite** then
pairwise-compares input `.mf4` vs output `.mf4` at two observability layers — **UDP KPI** (pre-tracking
detections: RDD bins + AF floats) and **CAN KPI** (post-tracking objects/tracks) — and emits HTML regression
reports. Decoder libraries bridge every byte: `.mf4` bus frames → per-stream structs → `.h5`/CSV → KPI math.

### 1.2 ASCII dependency / data-flow diagram

```
                        ┌──────────────────────────────────────────────────────────────┐
                        │                     VEHICLE / BENCH                          │
                        │  5-radar satellite net (FC,FL,FR,RL,RR) 77GHz FMCW +         │
                        │  Domain Controller (DCU, F360 tracker + FF) + Vehicle CAN-FD │
                        └───────┬──────────────────────────────────┬───────────────────┘
                                │ UDP Ethernet (bodnet, 1472B)     │ CAN-FD (64B frames)
                                ▼                                  ▼
                    ┌───────────────────────┐          ┌───────────────────────┐
                    │ ViGEm high-speed      │          │ ViGEm / dSPACE HiL    │
                    │ Ethernet logger       │          │ CAN-FD tap + XCP      │
                    └───────┬───────────────┘          └───────┬───────────────┘
                            │  ETH_Frame                   │  CAN_DataFrame
                            ▼                              ▼
                    ┌──────────────────────────────────────────────────┐
                    │  INPUT .mf4  (ASAM MDF4, sequential bus log)      │
                    │  channels: ETH_Frame / CAN_DataFrame / RawData    │
                    └───────┬──────────────────────────────────────────┘
                            │
              ┌─────────────┼─────────────────────────────────┐
              │             │ MF4 → structured decode          │
              ▼             ▼                                  ▼
 ┌────────────────────┐ ┌──────────────────────┐ ┌────────────────────────┐
 │ UDP Decoder Library│ │ Bordnet Tool         │ │ HIL Engine decoders    │
 │ radar_stream_lib   │ │ (PCAN/SOMEIP/CAN-FD) │ │ mudp_decoder, mdf_log  │
 │ GEN7/ / GEN8/      │ │ Auto_Gen_Files (DBC) │ │ apt_mdf_log.{h,cpp}    │
 └────────┬───────────┘ └──────────┬───────────┘ └───────────┬────────────┘
          │ Stream_Hdr_T +          │ Physical=Raw*Factor     │ DVSU/ORCAS/MDF4
          │ Detection/RDD/TOI/...   │ +Offset → CSV/XML/HDF5  │
          └─────────────┬───────────┴─────────────┬───────────┘
                        ▼                         ▼
              ┌──────────────────┐     ┌──────────────────┐
              │ .h5 / HDF5       │     │ CSV per stream   │
              │ tensor slices    │     │ *_UDP_GEN7_*.csv │
              └────────┬─────────┘     └────────┬─────────┘
                       │                        │
                       └────────────┬───────────┘
                                    ▼
              ┌──────────────────────────────────────────────────┐
              │  RESIM HARNESS (re-simulate candidate build)      │
              │                                                   │
              │  SiL-DCU:  APT_SRR_RESIM[.exe] (closed FW)        │
              │    + radar DLLs  SRR7/8_SiL_*.dll, FLR7/8_SIL_FC  │
              │    + DC libs SRR_DC_SIL_LIB / MRR_DC_SIL_LIB      │
              │      (sil_wrapper.cpp RECU_SiL_Execute loop)      │
              │    config: SIL_Engine_Config.xml + Input.json     │
              │                                                   │
              │  SiL-RSP:  Bazel rdd_sil / cdc_sil / af_sil libs  │
              │    Rdd_To_Detection_Configuration()               │
              │    CDC_To_Detection_Configuration()               │
              │    + PSP chain ID→RC→DA→SA, driven by .bin vectors│
              │                                                   │
              │  HiL: SRR_HiL_Resim(.exe) + SRR_HIL_Exec plugin   │
              │    + srr_comm (UDP inject to bench) + XCP/FDX     │
              │    config: Hil_Configuration.xml + log_path.txt   │
              └───────────────────────┬──────────────────────────┘
                                      │ per-scan outputs
                                      ▼
              ┌──────────────────────────────────────────────────┐
              │  OUTPUT .mf4 (re-simulated candidate recording)   │
              │  same channel layout as input (ETH/CAN frames)    │
              └───────┬──────────────────────────────────────────┘
                      │
                      ▼
              ┌──────────────────────────────────────────────────┐
              │  KPI VERIFICATION SUITE                           │
              │  UDP KPI: gen7v2_resim_kpi_scripts/ + IPS/udp     │
              │    (R_index,D_index) pairing + eps tolerances     │
              │  CAN KPI: can_kpi_hdf/ + can_kpi_scripts/ +       │
              │    IPS/mcip_can,ceer_can (scan-index continuity,  │
              │    quantized-hash detection match, track gating)  │
              │  Reports: HTML (ResimHTMLReport.py, kpi_html_gen, │
              │    json_to_html_convert), timing_plotter.py       │
              └──────────────────────────────────────────────────┘
```

### 1.3 Repository summary table

| # | Repository (path under `Research/`) | Lang | Architectural role | Inputs | Outputs |
|---|--------------------------------------|------|--------------------|--------|---------|
| 1 | `Core_Radar_Gen7_SAF85xx` | C/C++, Python/MATLAB, Bazel | Gen7 embedded radar FW + RSP SiL (EmLib equivalent). MSS=`software/m7` (Cortex-M7 AUTOSAR), DSS=`software/bbe32` (Xtensa BBE32 SPT) + `software/a53` (tracker host). `sil/emb_lib` + `sil/rsp_sil` host models. | `.bin` vectors (`sil/rsp_sil/data_bin/.../rdd_input.bin`, `CDC_stream.bin`), `Emb_Lib_Config.xml`, SMC/USC cal blobs | `Detection_Stream_T`, `Rdd_Stream_T`, `rdd_sil`/`cdc_sil` test binaries + `//:rdd_sil_lib` DLL for ITF/MATLAB |
| 2 | `Core_Radar_Gen8_iND13400` | C/C++, Python/MATLAB, Bazel | Gen8 embedded radar FW + RSP SiL. MSS=`software/r52` (Cortex-R52 AUTOSAR), DSS=`software/bbe32` (+RA accelerator, MIPI/CSI2). Adds `rdd_proc` (CFAR) + `rdu` (unfolding/classifier) on DSP. | Same pattern, variant `flr8/srr8p`; `rdu_dev_can` configs | Same + `rdu_sil` outputs, `//:gen8` images, PHY 1Gb/100Mb variants |
| 3 | `Core_RESIM_UDP_Decoder_Library` | C/C++ | Bodnet/UDP decoder static lib (`radar_stream_lib`): ETH→UDPRecord→per-stream structs→HDF5. GEN7/GEN8 versioned stream headers (`V5..V200`). | Input `.mf4` ETH frames, `Decoder_DLL_Release_Notes.xml` version map | Decoded structs (`Detection_Stream_T`, `Rdd_Stream_T`, TOI/VSE/…) + `.h5` datasets via `PrintHDFData`/`Hdf5WriteUtils.h` |
| 4 | `Core_RESIM_Bordnet_Tool` | C/C++ (autogen), XML | Bordnet (CAN-FD + SOME/IP) decoder: DBC-autogen `Physical=Raw*Factor+Offset`, SOME/IP reassembly, MF4 read/write (`mdf_log`). Per-position decoders (FL/FR/FLR/RL/RR). | `.mf4` `CAN_DataFrame`/`ETH_Frame`, `bordnet_config.xml`, `Gen7_BORDNET_fList.json` | Decoded `double64_t DET_*`/`HED_*`/`STS_*` → XML/CSV/HDF5 (`ALPcanHDF_*.h`) |
| 5 | `Core_RESIM_DC_Emb_Library` | C++17, CMake, Python, Shell | Domain-controller SiL: F360 tracker + OLP + VSE + Feature Functions (CTA/CED/ESA/LCDA/LTB/PT/RECW/SCW/TA) as `SRR_DC_SIL_LIB`/`MRR_DC_SIL_LIB`, driven by FW `APT_SRR_RESIM`. | `SIL_Engine_Config.xml`, `SRR/MRR_DC_Lib_Control.xml`, `SIL_Input.txt`/`Input.json` (list of `.mf4`), DGPS configs | Output `.mf4` variants (ORCAS/CANAPE/AUTERA/VIGEM/PCAP…), HDF/XTRK/BIN debug, `versions.txt` |
| 6 | `Core_RESIM_HIL_Engine` | C/C++ | HiL resimulator + loggers: `SRR_HiL_Resim` harness, `SRR_HIL_Exec` plugin, `srr3_comm` UDP bench inject, XCP/FDX/CAN support, MDF4 decoders. | `Hil_Configuration.xml`, `log_path.txt`, `HiLApp.ini`, `Channel.txt`, live DSPACE or `.mf4` | Bench stimulation (UDP/CAN/XCP), `SRR3_HiL_Trace*.txt`, `timeDiff.csv`, KPI inputs |
| 7 | `Core_RESIM_KPI` | Python | KPI verification + HTML reporting: UDP KPI (`gen7v2_resim_kpi_scripts`, `IPS`, `main_html/.../UDP_KPI`), CAN KPI (`can_kpi_hdf`, `can_kpi_scripts`, `IPS/...can...`), timing plotter. | Paired CSV/HDF5 (`veh` baseline + `sim` resim), `meta_data.json`, `kpi.json`, `HTMLConfig.xml`, `Inputs.json` | Match % KPIs, diff lists, HTML reports (`IRep*`), PNG plots, `ResimHTMLReport.simg` |
| 8 | `Core_RESIM_HPCC` | — | EMPTY placeholder in this workspace snapshot (0 files). Role per naming: HPCC offload/cluster runner for KPI/portal (`main_html/all_services/hpcc_main.py` references HPCC runtime). | — | — |
| 9 | `Core_RESIM_USS_Sensor_Model` | — | EMPTY placeholder in this workspace snapshot (0 files). Role per naming: ultrasonic-sensor model / DGPS-adjacent simulation input (DC_SIL DGPS `SensorModel2`/`SMValidation` libs are the only sensor-model code present). | — | — |

Build order (cold start): 1–2 (radar FW/SiL libs, Bazel) → 3–4 (decoders, CMake/VS) → 5–6 (DC_SIL `BuildAll`, HIL `Build_Project.sh`)
→ 7 (KPI, `pip install -r requirements.txt`, `build_simg.sh`). Data order at runtime is the diagram above.

---

## 2. Physical & Hardware Ingestion Layer

### 2.1 Sensor modality — 77 GHz FMCW, chirps, virtual MIMO

Both Gen7 and Gen8 are 77 GHz Frequency-Modulated Continuous-Wave (FMCW) imaging radars. Each *look*
(a scheduled chirp burst) transmits a sequence of linear-frequency ramps (fast-time samples within a chirp,
slow-time chirps within a dwell). The on-chip front end (Gen7: NXP SAF85xx RFE/MMIC + SPT RFFT engine;
Gen8: indie `Chandra` + `Mars` with RA Radar Accelerator and MIPI/CSI2 ingest in
`Core_Radar_Gen8_iND13400/software/bbe32/src/mipi_ifc.c`) digitizes the de-chirped beat signal (ADC stream,
`software/common/adc_stream.h`).

Range FFT (fast-time FFT, `Appl_Range_Process_Execute()` in
`Core_Radar_Gen7_SAF85xx/software/bbe32/src/range_proc.c`, `range_process_ifc.c` in Gen8) maps beat
frequency to range bins:

```
r_m = m * c / (2 * S * T_s * N_R)  ≈  m * rbin_res,   m = 0..N_R-1
```

where `S` = chirp slope (Hz/s), `rbin_res` is logged per look in `Look_Data_T.rbin_res` (u16p16 fixed point,
`software/common/rdd_stream.h`). Doppler FFT (slow-time FFT across chirps,
`Appl_Doppler_Rdd_Processing()` / `doppler_process_ifc.c`, vectorized on Xtensa BBE32 +
`RADAR_RECIP/POW2` helpers in `software/bbe32/rdu/inc/rdu_math.h`) maps phase progression to Doppler bins:

```
v_k = k * λ / (2 * N_D * T_c)  ≈  k * dbin_res,   λ = c / f_c, f_c ≈ 77 GHz
```

(`dbin_res`/`vua` s10p21 per look in the same `Look_Data_T`.) The complex Range-Doppler power map is the
Compressed Data Cube (CDC, `software/common/cdc_stream.h`, `cdc_frame_interface.h`); CFAR detection on it
produces the RDD list (`rdd_stream.h`).

**Virtual MIMO.** `NUM_TX_CHANNELS × NUM_RX_ANTENNAS` physical paths synthesize
`N_tx × N_rx` virtual elements (Gen8 SRR8 `radar_sw_config.h`: `NUM_TX_CHANNELS=4, NUM_RX_ANTENNAS=8`,
`ANGLE_BINS_USED=8`; per-detection beamvector `rdd1_bv[MAX_DETS_FIRST_PASS][NUM_TX][NUM_RX]`).
Angular resolution follows the aperture limit `Δθ ≈ λ / (N_virt · d · cosθ)`; the AF stage converts
inter-channel phase deltas to azimuth/elevation (Section 4.4). Sidelobe energy outside the main lobe is
pruned by calibration/sidelobe tables (`rcemlib` role, `calib_stream.h`, SMC/USC blobs).

**Look modes.** The scheduler (`software/radar_processing/bb_radar_ctl/bb_radar_ctl_imp/radar_look.c`,
Gen8 `software/r52/bb_radar_ctl/...`) cycles looks; `Look_Data_T` + `AF_Det_List_Property_T` carry
`look_id / look_index / scan_type / dwell_type`:

| Look Index | Conventional use | Signal meaning in code |
|---|---|---|
| 0 | LRR (long-range, narrow FOV, high Doppler resolution) | Long dwell, fine `dbin_res`, far `range_coverage` (u9p7) |
| 1 | MRR (mid-range) | Medium dwell/coverage |
| 2 / 3 | SRR (short-range, wide FOV) | Short dwell, wide azimuth span, near-range bias |

`look_index` selects CFAR thresholds (`bwdep_*_thold[MAX_RANGE_BINS]`), calibration tables
(`calibration_cfg.h`), and tracker process-noise presets per look.

### 2.2 Constellation topology — 5-radar satellite network

Standard fitment is five heads: **FC** (front-center long-range, `FLR7_SIL_FC.dll` / `libFLR*_SIL_FC.so`),
**FL/FR** (front corners), **RL/RR** (rear corners). Variants: standalone (single-sensor tracking,
`--tracker_variant=..._standalone`), 2-sensor fusion (`platform_srr7p_2_sensor_fusion`,
`platform_srr8p_2_sensor_fusion`), partner-sensor and AL (application-library) modes
(Gen8 `--tracker_variant=disabled|partner_sensor|platform_flr8_standalone|platform_srr8p_standalone|
platform_srr8p_2_sensor_fusion|AL`).

Each satellite runs its own DSS clock (BBE32) + MSS clock (M7/R52); the DC fuses them. Consequences:

- **Timer decoupling / clock drift.** Detection timestamps (`detection_timestamp_ns/sec`,
  `AF_Str_Det_List_Property_T` in `GEN8/GPO/SRR8/DETECTION/V200/detection_stream.h`) are chirp-reflection
  epochs in the *sensor* time base; header/serialization timestamps are bus epochs. Inter-head drift of even
  tens of microseconds smears fusion during yaw (see IDF-3 in the companion R&D dossier).
- **Scan cadence.** `RADAR_CYCLE_S = 0.05` s (20 Hz) in
  `Core_RESIM_KPI/gen7v2_resim_kpi_scripts/config.py:40`; `scan_index` increments per cycle and is the
  primary join key of every KPI merge (`pd.merge(..., on='scan_index', how='inner')`).
- **Position-indexed decoding.** Every decoder and KPI fans out by radar position:
  `Radar_Position` enum (`RL=71 RR=72 FR=73 FL=74 …` in `CommonFiles/udp_headers/Udp_record.h`),
  `SENSOR_00..10`/`ECU_00..03` in `SIL_Engine_Config.xml`, per-position CAN IDs (`SRR_FL_DETECTION_*=0x280…`
  in `AL_PCAN_FL_Common_V1_1.h`), per-position SOME/IP source IPs (`CEER_SRR7P_SOURCE_FL=0xC0A80149…` in
  `Common_Structures/SOMEIPCommonHeader.h`), per-position SiL DLLs (`SRR7_SiL_FL/FR/RL/RR`, `SRR8_SiL_*`).

### 2.3 In-vehicle data acquisition — ViGEm and dSPACE

- **ViGEm high-speed loggers** capture raw Ethernet (`ETH_Frame`: `Eth_Header_T` + `IPV4_Header_T` +
  `UDP_Header_T` in `CommonFiles/udp_headers/Eth_ViGEMHeader.h`) and CAN-FD (`CAN_DataFrame`) into `.mf4`.
  Reader: `mdf_log/src/apt_mdf_log.cpp:274 ProcessETH_Frame()` / `ProcessViGEMCAN_Frame()` /
  `ProcessViGEMETH_Frame()`, channels `ETH_Frame` + `.Source/.Destination/.EtherType/.ReceivedDataByteCount`.
- **dSPACE HiL racks** (`DPH_RR_ADAS_HIL`, `HiL_IFace_Controller/HiLExecutiveBMW.cpp`, FDX description
  `FDX_Protocol/ConfigFiles/FDXDescription_HiL_V1.0.xml`) stimulate the bench over UDP/XCP/CAN and record
  the same channel set, plus `DSPACE_Common_Input_Structure.h` vectors (vendored in both the UDP decoder
  `GEN6P_DSPACE/` and DC_SIL `sil_udp_streams/RECU/`).
- **XCP / seed-key** (`CoreLibraryProjects/XCP_SeedKey*`, `HiLApp.ini [XCP_*]`, `HiLController::
  GetStreamAndSWVersionUsingXCP / EnableHILMode`) puts ECUs into HiL mode and versions the streams under test.

---

## 3. Binary Formats, Bus Protocols, and Signal Decoding

### 3.1 The file-format bridge — `.mf4` vs `.h5`

| Aspect | `.mf4` (ASAM MDF4) | `.h5` / HDF5 |
|---|---|---|
| Layout | Sequential time-ordered bus frames (`dvlCanMessageFD`, `DVSU_RECORD_T`, `CCA_UDP_RECORD_T`) | Contiguous per-signal tensors (`/detection/ran`, `/rdd/rindx`, …) |
| Access | Stream-replay; good for injection, bad for random access | Slice by `scan_index`/sensor; good for vectorized KPI math |
| Writers | `apt_mdfFile::write_MDF()` (`mdf_log/inc/apt_mdf_log.h`), `AS_BIN_WRITER` (`DC_SIL/sil_ext_libs/AS_BIN_WRITER`) | `IRadarStream::PrintHDFData(H5::H5File*)`, `Hdf5WriteUtils.h::writeHDFVector()`, `HDF5Optimization` helpers, per-version `*_stream_hdf_V*.cpp` |
| Readers | `apt_mdfFile::read_MDF()`, `MdfUse.h` (`VMdf4Lib`, `IFilePtr`/`IChannelPtr`), `radar_hdf_datacollect.py` vs `radar_mdf_datacollect.py` (IPS `DataCollect/`) | `h5py` in KPI (`radar_hdf_datacollect.py:52 parse_hdf_data`), `hdf_parser.py`/`hdf_wrapper.py` (`can_kpi_hdf/a_persistence_layer/`) |
| Converter role | Raw logger output; HiL/SiL replay source | KPI working format; `mf4 → h5` is the first Resim pipeline stage |

Converter scripts/code (exact): `mdf_log/src/apt_mdf_log.cpp` (ETH/CAN frame extraction),
`Common/Hdf5WriteUtils.h` + `CommonFiles/Utility/hdf5_write_optimization.h` (HDF5 writes),
`BordNetDecoder/Decoder.{h,cpp}` (`ReadMF4FileAndDecode`, `ReadCANMF4`, `ReadVIGEMMF4`, `ReadORCASMF4`,
`ReadSomeIpInputMF4/ReadSomeIpOutputMF4`), per-stream `detection_stream_hdf_V200.cpp`-style writers,
IPS `DataCollect/radar_hdf_datacollect.py` + `dc_radar_hdf_datacollect.py` + `can_radar_hdf_datacollect.py`.

### 3.2 UDP ("Bodnet" / Ethernet) — payload layout

Per-frame stack (offsets from `SOMEIPCommonHeader.h`: `ETH_PAYLOAD_SOMEIP_OFFSET=28`, `SOMEIP_OFFSET_POS=16`;
UDP payload cap `UDP_PAYLOAD_SIZE=1472` / `RECU_UDP_FRAME_LENGTH=1468`):

```
ETH (DestMAC6+SrcMAC6+VLAN4) → IPv4 (20B) → UDP (8B, ports 5555/5556/3490/6001/4410)
 → UDPRecord_Header / GEN7_UDPRecord_Header_T (pack(2)):
     versionInfo (0xA218 LE / 0x18A2 BE / 0xA318 A3_LE), sourceTxCnt, sourceTxTime,
     Platform (UDP_PLATFORM_T: SRR5=41…FLR7=73…FLR8=82,SRR8_PLUS=83), Radar_Position,
     streamRefIndex (= scan_index domain), streamDataLen, streamTxCnt,
     streamNumber (= logging source, gen7_Udp_source_Tag e_DETECTION_STREAM=1 … e_BSIS_STREAM=160),
     streamVersion, streamChunksPerCycle/streamChunks/streamChunkIdx (fragment reassembly),
     customerId, sensorId
 → Rec_Hdr_T (REC_VER 0xA5 variant, Stream_Hdr_T sibling):
     size/version/checksum/scan_index/error_info/module_time_ms (+ Dyn_Hdr_T{scan_index,records_cnt})
 → versioned stream body, e.g. Detection_Stream_T (V200, SIZE_OF_RESIM_DETECTION_STREAM=240104):
     det_list_property{detection_timestamp_ns/sec, lookindex, scanindex, look_type,
                       range_coverage, doppler_coverage, num_fp_detections, num_sp_detections,
                       slope_correction_status, timestamp_status},
     af_diagnostics_info, ch_ch_degradation_data, num_af_det,
     af_dets[6000]{ran,vel,pow,snr,theta,phi,rcs,spec_res,rdd_idx,az_conf,el_conf,
                   f_single_target,f_superres_target,f_bistatic,…}   (40 B each)
   or Rdd_Stream_T (V117, SIZE_OF_RESIM_RDD_STREAM=189900):
     look_data{start/mid/end_dwell nanosec/sec + status, look_id, scan_type, look_index, scan_index},
     rdd_data{cfar_corr_coeff, rdd1_{rbinest,dbinest,abinest,rindx,dindx,aindx,rdop_amp,
              bv[NUM_TX][NUM_RX],num_detect}, rdd2_{thold_2nd_pass,range,range_rate,snr,num_detect},
              bwdep_*{cr_resp,sensitivity,af_sensitivity,hvc,mb}_thold[MAX_RANGE_BINS],
              cfar_{nf_est,thold}, nci_bexp, mixing_strength, neighbor/sp/zdb/hvc flags}
```

Key files: `Common/stream_header.h` (`Rec_Hdr_T`, `Stream_Hdr_T`), `CommonFiles/udp_headers/Udp_record.h`,
`Gen7_Udp_record.h` (`GEN7_Radar_Logging_Data_Source_Tag`, `gen7_Udp_source_Tag`), `udp_sources.h`
(`UDP_PLATFORM_T`), `Eth_ViGEMHeader.h`, `inc/radar_unpack_struct.h` (`Proc_Info_T` latch state machine:
`f_first_chunk_rcvd → f_complete`, `latched_block_count`, `crc_status/xsum_status/udpPack_status`),
`GEN8/GPO/SRR8/{DETECTION/V200, RDD/V117, TOI/V5,V106, …}/*_stream.h` + `radar_sw_config.h`
(`MAX_RANGE_BINS=420, MAX_RANGE_FFT_SIZE=1024, MAX_DOPPLER_FFT_SIZE=512, MAX_DETS_FIRST_PASS=1024,
AF_MAX_NUM_DET=2048, ANGLE_BINS_USED=8 …`), `Common/IRadarStream.{h,cpp}`
(`FillStreamData/GEN5FillStreamData/GEN7FillStreamData`, `getData()`, `ComputeChecksum()`),
`Common/IRadarStream_Lib_Loader.h` (C ABI: `RadarSilGetData`, `RadarSilFillStreamData`, `Hil_get_data`, …).

Pre-tracking vs post-tracking on this bus: RDD + Detection (+ Detection_Debug, Down_Selection, CDC, VSE,
ID/RC/DA/SA) are *pre-tracking* DSP outputs; TOI/F360 objects are *post-tracking* (tracker) outputs.
UDP KPI compares the former; CAN KPI the latter.

### 3.3 CAN / CAN-FD — frame formats and DBC unpacking

CAN-FD transport struct (`Common_Headers/CAN_FD.h`):

```c
dvlCanMessageFD { dvlMessageInfo{timestamp,messageSize,messageType,reserved};
                  id, extFrame, channel, length, data[64], flags, fd_brs; }  // 64 B payload
```

Per-sensor CAN-ID maps (example `AL_PCAN_FL_Decoder/AL_PCAN_FL_Common_V1_1.h`):
`AL_PCAN_FL_BUS_SPEC_ID=2103`, `SRR_FL_DETECTION_001_004=0x280 … SRR_FL_DETECTION_125_128=0x29F`
(4 detections per 64-B CAN-FD frame, 32 frames for 128 detections),
`SRR_FL_HEADER_001=0x98, STATUS_001=0x9B/002=0x9C, ALIGNMENT_STATUS_001=0x9D/002=0x9E, CAPABILITY=0x9F`.
Decoded mid struct (`Common_Structures/Common_Structures.h`, `pack(4)`): `Pcan_DetectionDataMID_T`
(`ScanIndex, LookIndex, Look_ID, TargetReportCnt, range[8], range_rate_raw[8], azimuth_raw[8],
elevation[8], amplitude[8], rcs_std_dev[8], f_valid_level, valid_level[8], det_id, …`, plus
`_390/_410/_480/_430_SOP3` variants and VCAN `BPILDecoderDetectionList_430T` with
`DETECTION_OBJS_PER_FD_FRAME=5, TOTAL_VCAN_HIGH_DETECTION_FRAMES=30`).

DBC codegen (no raw `.dbc` is checked in; the DBC is a *generator input*):
`Auto_Gen_Files/Include/AL_V1_1/autogen_AL_PCAN_Decoder_DBC_signal_macros.h` —
`Factor_of_signal_DET_RANGE_001_in_message_SRR_FL_DETECTION_001_004 (0.015625)`,
`Engg_val_…(x)` bit-pack expression, `Phys_Val_…(x) = Engg*Factor + Offset`;
`Auto_Gen_Files/Source/AL_V1_1/autogen_AL_PCAN_Decoder_DBC_message_decoding.cpp:7594` —
`d_ptr->DET_RANGE_001 = Phys_Val_…(e_ptr);` (encode inverse in `…_encode_messages.cpp`).
Other factors in the same header: `STS_VEHICLE_SPEED_CALC=0.01`, `STS_STEERING_ANGLE/HED_FOV=0.0174533`,
`DET_EXISTENCE_PROB=1.588`. Decoded C++ types:
`Auto_Gen_Files/Include/CEER_V1/autogen_CEER_PCAN_FL_Decoder_DBC_message_decode_types.h`
(`double64_t DET_RANGE_001, DET_RANGE_VELOCITY_001, DET_AZIMUTH_001, DET_RCS_001, HED_*, STS_*`).

SOME/IP (Ethernet-carried detections): `Common_Structures/SOMEIPCommonHeader.h`
(`CEER_SRR7P_SOURCE_FL=0xC0A80149…`, `DET_LIST_MSG=0x00108001`, `FRONT_RADAR_DET_LIST_MSG=0x30848001`),
`CEER_SRR7P_SOMEIP_HeaderV2.h` (`MessageInformationCEERSRR7PV2`, `SOMEIPPayLoadV2_T` 87-B stride × 16 packets
→ 400 detections, `OutputSOMEIPDataV2_T`), `CEER_SOMEIP_FL_Decoder/CEER_SOMEIP_FL_Decoder.h`
(`IPV4HEADER=28`, `SOMIPHEADER_MULTIPLE_PACKETS=20/SINGLE_PACKET=16`, `NUMBRE_OF_DTETECTION_PACKETS=31`,
67-KB latch `payloadBuff_det[67000]`, `DecodeFLR7FLDetection/…`, `bswap<float32_t>(HED_SCAN_INDEX)`).

**Why the brief says "78-byte":** no literal 78-B CAN payload struct exists in the audited tree; the number
arises as *detection-index shorthand* (e.g. `SRR_FL_DETECTION_125_128` tail frames, 78-B SOME/IP fragment
strides, or 64-B CAN-FD + 14-B ETH/IP/UDP framing). Treat "78-byte" as the message-assembly unit in the
portal simulator, with the 64-B `data[64]` array as the ground-truth payload.

### 3.4 Timestamp analysis — detection vs header epoch

- **Detection Timestamp** (`detection_timestamp_ns/sec`, `AF_Str_Det_List_Property_T`; `Look_Data_T`
  `start/mid/end_dwell_nanosec/sec`): the *chirp-reflection epoch* — when the photons returned. Lives in the
  sensor DSP time base; comparable across looks only after dwell-centering (`mid_dwell_*`).
- **Header Timestamp** (`Stream_Hdr_T.module_time_ms`, `Rec_Hdr_T.sourceTxTime`, `dvlMessageInfo.timestamp`,
  `mdfCANchannel_T` stamps): the *bus-serialization epoch* — when the frame hit the wire.
- **Latency metric:** `ΔT = T_header − T_detection`. KPI-adjacent uses: `can_xml_kpi_scripts.py:587
  compare_cycles()` timestamp-diff gating (skip `Δ>0.040 s` input / `Δ<0` output, the 40-ms rule),
  `can_kpi_hdf` `TIME_MATCH_TOL_NS=2_000_000` (2 ms) alignment window, `profile_timing_plotter/
  timing_plotter.py` distributions, DC_SIL `PLP_TIMESTAMP/TIMING_PROFILE` traces, HIL `timeDiff.csv`
  (`sourceTxTime, CurrentPC-time, Scan Index`).

---

## 4. Inside the Embedded Radar Algorithm Pipeline (EmLib)

> Naming: the brief's `EmLib/Embleep + rpemlib/rcemlib/trackercemlib` are *conceptual* partitions. In this
> workspace they materialize as: Gen7/8 `software/bbe32` (DSS signal chain ≈ rpemlib), `software/m7 or r52`
> + `calib_cfg`/`dyn_alignment`/`static_alignment` (≈ rcemlib), `emb_tracker` + external `@spbb/@afbb/@idbb/
> @rcbb/@dabb/@sabb` + DC_SIL `F360TrackerLib` (≈ trackercemlib/XTRK). No file is literally named
> `rpemlib`/`rcemlib`/`MESR_RCE_BIT` except a BIT flag in `software/bbe32/ecc_test/ecc_test.c`.

### 4.1 Signal-flow architecture (code path, raw ADC → confirmed tracks)

```
MMIC/Mars ADC → MIPI/CSI2 → Range-FFT → Doppler-FFT → 1st-pass CFAR → CDC pack →
2nd-pass CFAR/RDD prune → Angle Finding (beamform + down-select + fascia comp) →
ID → RC → DA → SA (PSP chain) → RDU (Gen8) → Emb Tracker F360 (XTRK) →
D2M/A2M IPC → MSS AUTOSAR (dsp_setup + SWC_PLT_CDD_Radar_Control, 50 ms look trigger) →
CAN / SOMEIP / UDP-CDC out
```

**Step 1 — Range & Doppler FFTs, CDC generation.**
Gen7: `software/bbe32/src/range_proc.c` (`Appl_Range_Process_One_Time_Init()`,
`Appl_Range_Process_Execute(Radar_Look_T, M2D_Msg_T*)`), `software/bbe32/src/doppler_proc.c`
(`Appl_Doppler_Rdd_Processing(Radar_Look_T)`, `Get_NF_est_look_ptr()`, `Update_Look_Data()`,
`Rdd_Memory_Init()`, `dfft_bv_output_buffer`), `software/bbe32/src/cdc_if.c` /
`cdc_packing.c` (`CDC_OutputDataCube_T[SPBB_MAX_DOPPLER_FFT_SIZE][NUM_CDM_CHANNELS]`).
Gen8: `software/bbe32/src/range_process_ifc.c`, `doppler_process_ifc.c`, `cdc_packing.c`,
`software/bbe32/inc/cdc_packing.h`. CDC wire type: `CDC_Stream_T{Dyn_Hdr_T{crc_check,records_cnt,
scan_index}; CDC_Record_T[35]{CDC_RngDopp_T{rng_idx,dopp_idx,nci_data};
cdc_out_bv[8]}}` (`software/common/cdc_stream.h`, v12, 1408 B). Build flags:
`--enable_cdc/--enable_cdc_crc`, `CDC_2K_BINS/CDC_5K_BINS` (`software/bbe32/bb_cfg/build`).
Memory discipline: `Mem_Pool_T` + `Memory_Pool_Return_T` allocators, `xthal_dcache_block_writeback/
invalidate()` around IPC handoff, ping-pong `M2D_Msg_T[2]`/`D2M_Msg_T[2]`.

**Step 2 — RDD module (Range-Doppler Detection), 1st & 2nd pass CFAR.**
Gen8 entry: `software/bbe32/rdd_proc/api/appl_cfar_ifc.h` + `imp/src/appl_cfar_ifc.c` +
`imp/inc/appl_rdd_prv.h`: `Appl_Cfar_Init(Radar_Look_T)`, `Appl_Cfar_Execute(uint16_t ridx, RDD_Data_T*)`,
`Appl_CFAR_Memory_Init()`; test `rdd_proc/test/appl_cfar_ifc_unit_test.cc`. Gen7 CFAR: `software/bbe32/
test/cfar_if_unit_test.cc`, `cdc_if_unit_test.cc`, `cdc_packing_unit_test.cc`, `test/mocks/cdc_*_fff.*`.
Math: cell-averaging CFAR per range bin against `bwdep_*_thold[MAX_RANGE_BINS]` with guard/reference windows,
noise-floor estimate `cfar_nf_est` (`Get_NF_est_look_ptr()`), correlation correction `cfar_corr_coeff`,
then second-pass confirmation (`rdd2_thold_2nd_pass`, `rdd2_sp_fail_flag`, `below_sp/zdb/hvc_thold` booleans,
`min_chirp_scaling`, `mixing_strength`). **`(R_index, D_index)` coordinates** are integer
`(rindx, dindx)` bin addresses into the Range-Doppler map (`uint16_t rdd1_rindx/dindx/aindx
[MAX_DETS_FIRST_PASS]`, `rdd1_rdop_amp`, `rdd1_bv`), with float refinements `rdd1_rbinest/dbinest/abinest`
(s10p21) and second-pass `rdd2_range/range_rate` (s10p21) + `rdd2_snr` (s8p23). KPI Tier-1 matches the integer
pair; Tier-2 validates the floats (Section 6).

**Step 3 — Angle Finding / beamforming (DoA).**
`software/building_block/anglefinding/angle_finding_project_interface.h`,
`anglefinding_project_interface.c`, `test/anglefinding_project_interface_unit_test.cc` (Gen7);
`software/bbe32/src/anglefinding_project_interface.c` + `inc/…` (Gen8).
Gen7 signature: `AF_Appl_Input_T{RDD_Data_T*, Look_Data_T*, XCP_Data_T*, XCP_Info_T*, Vse_Stream_T*,
radar_position*, AF/DS_Bypass_Flag*, Plt_Mounting_Data_T*} → AF_Appl_Output_T{Detection_Stream_T*,
Detection_Debug_Stream_T*, Angle_Data_XCP_T*, Down_selection_Stream_T*}` via
`Appl_Angle_Finding_Process()`, `Execute_Fascia_Compensation()`, `Anglefinding_Memory_Init()`.
Gen8 adds `FS_Executed_Flag_T`, `Angle_Timing_Data_T*`, `DownSelection_Memory_Init()`,
`Appl_Down_Selection_Process()`, `Appl_Execute_Fascia_Compensation(Fascia_Comp_Data_T*,…)`,
`Angle_Finding_Update_Metrics()`, `AF_Retval_T`. Phase monopulse core:
`Δφ = 2π·d·sinθ/λ  ⇒  θ = asin(λ·Δφ/(2π·d))`, with elevation from the orthogonal baseline pair;
outputs `AF_Det_T{ran,vel,pow,snr,theta,phi,rcs,spec_res,rdd_idx,az/el_conf,
f_single/superres/bistatic/ci,spacingtype/stage/az_af_type}` (`software/common/detection_stream.h`, v21,
38508 B; Gen8 V200 `AF_Str_Detection_T`, 40 B, `AF_MAX_NUM_DET=6000/2048` by version).

**Step 4 — Calibration (rcemlib role).**
`software/common/calib_stream.h`, `software/m7/calib_cfg/calibration_cfg.h`,
`calibration_sect_cfg.h`, Gen8 `software/common/calib_cfg/`, `software/bbe32/{dyn_alignment,
static_alignment, TOI_char_quality_determination}`, SiL wrappers `psp_sil_interface/{da,sa}_…`
(`da_sil_api.hpp`, `sa_sil_wrapper.hpp`), DC_SIL `sil_smc/` (`SMCParameters.{cpp,h}`, `sil_autogen/
AutoGen*SMCParameter.cpp`, `DCCustomer.h/DCEnums.h/DCMacros.h/DCSensorType.h`). Static bore-sight +
fascia compensation (`Execute_Fascia_Compensation`, `Plt_Mounting_Data_T`), dynamic alignment tracking road
clutter (`da_sil`, `Dynamic_Alignment_Log_Stream_T`, KPI `alignment_matching_kpi_script.py`), sidelobe
pruning via SMC/USC tables (`smc_cal_Satellite_46_79_0_5`, `usc_cal_default_6_79_0_0`,
`--override_repository=smc_/usc_…`, `af_smc_mismatch_flag/af_usc_mismatch_flag` in `AF_Diagnostics_T`).

**Step 5 — Tracking subsystem (trackercemlib / XTRK).**
On-sensor: `software/a53/emb_tracker/` (Gen7) / `software/bbe32/emb_tracker/` (Gen8):
`emb_tracker_wrapper.{h,cpp}`, `emb_tracker_internal.h`, `emb_tracker_project_parameters.h`,
`emb_tracker_stubs.{h,cpp}`. Gen7: `Emb_Tracker_Mem_Init(A2M_Msg_T*)`, `Init_Emb_Tracker(D2M_Msg_T*)`,
`Run_Emb_Tracker(D2M_Msg_T*, A2M_Msg_T*)`. Gen8: `Emb_Tracker_Mem_Init(Mem_Pool_T*, D2M_Msg_T*)`,
`Init_Emb_Tracker(M2D_One_Time_Msg_T*, M2D_Msg_T*)`, `Run_Emb_Tracker(D2M_Msg_T*, M2D_Msg_T*,
M2D_One_Time_Msg_T*)` + `Map_Ref_Point_To_Len_Wid_Segments()`; both expose
`Open/Close_F360_Tracker_Xtrk_File()` (XTRK logging). Domain-controller tracker: DC_SIL `Application/
F360Tracker/F360TrackerLib/` (`Fusion360/`, `StateManager/`, `Timing/`, `SharedTrackerAPI/`,
`safety_handler/`, `CMakeLists.txt`) + `OLP_Core` (`olp_iface.h`) + `VSE_Core` + `rspp` + `ocg` +
`sg_stationary_geometry`. State space `x = [x, y, vx, vy]^T` (VCS frame; KPI tracker cols
`vcs_xposn/yposn/xvel/yvel`, `trkID_i`, `f_moving` × 64 in `tracker_matching_kpi_script.py:390`);
constant-velocity propagation `x_{k+1} = F·x_k`, `F = [[I, dt·I],[0, I]]`, `dt = 0.05 s`;
process noise `Q`, measurement noise `R`; Mahalanobis gate `d² = νᵀS⁻¹ν ≤ γ`; greedy nearest-neighbor
association with distance-scaled thresholds (`threshold_mul_factor = |x|/10` for `|x| ≥ 10 m`,
`tracker_matching_kpi_script.py:92-93`). SiL twin: `sil/emb_lib/tracker_emb_lib/
tracker_sil_wrapper.{h,cpp}`, `f360_wrapper/f360_radar_tracker.{h,cpp}`.

**Supporting DSP (Gen8 RDU):** `software/bbe32/rdu/{inc,src,test}/` —
`rdu_math.h` (BBE vector `rdu_finv/exfp/sqrtf/floorf/ceilf/roundf/absf/fmaxf/acosf` via
`xt_bben_scalarfp.h`/`RADAR_RECIP/POW2`), `doppler_unfolding.h`, `two_cycle_unfolding.h`,
`stationary_moving_classifier.h`, `moving_special_cases.h`; interface `rdu_stream.h`
(`ENABLE_RDU_DEV`), SiL `rdu_sil_interface/test:rdu_sil_Test`.

### 4.2 Multi-core partitioning — MSS / DSS / IOC

No `Ioc_*` AUTOSAR-IOC calls exist; inter-core transport is custom shared-SRAM + HW IRQ, with `Rte_*`
used only for MSS software-component plumbing (`Rte_SWC_PLT_CDD_Radar_Control.h`,
`radar_look_unit_test.cc`, `dsp_setup_unit_test.cc`).

| Core | Gen7 | Gen8 | Runs |
|---|---|---|---|
| MSS (master, ARM) | `software/m7` (Cortex-M7, AUTOSAR: `autosar/{config/Appl/GenData,sip,swc/{PLT_SWC,PRJ_SWC,TKEY_SWC,COMMON}}`, `mcal`, `drivers/{serializer,pmic,gpt,board_temperature}`, `mmic/drivers/ppe_csi2`, `dsp_setup`, `ipc/{inc,src}`, `calib_cfg`, `program_flow_monitor`) | `software/r52` (Cortex-R52, same AUTOSAR shape + `drivers/mcal/{Mipi_wrapper,gpt,mcu_reset_wrapper,power_supply_clock}`, `mmic/drivers/mipi_csi2`, `bb_radar_ctl`, `r52_stack`, `startup`) | 50-ms look trigger `RE_Radar_Ctl_Look_Trigger() → MMIC_Helper_Function()` (`SWC_PLT_CDD_Radar_Control.c`), `Run_DSP_Interaction_State_Machine()`, `DSP_Processing_Onetime_Init/Per_Look_Init()` (`software/r52/dsp_setup/src/dsp_setup.c`), NvM/SecOC/MacSec/NM/UDS (`SWC_TKEY_*`), TimeSync/Logging/IPC SWCs |
| DSS (DSP) | `software/bbe32` (Xtensa BBE32 SPT, `src/{cdc_if,cdc_packing,doppler_proc,range_proc,fp_if,sp_if,sweep_bw_if,ipc_dsp,main_application,mpu,interrupts,exceptions}.c`) | `software/bbe32` (BBE32 Luna RJ-2025.5, same + `range_process_ifc`, `doppler_process_ifc`, `anglefinding_project_interface`, `mipi_ifc`, `performance_monitor_ifc`, `get_r52_time_from_bbe32`, `ci_stv_misalignment`, `rdd_proc`, `rdu`) | Range/Doppler/CFAR/AF/RDU + `main_application.c` state machine |
| Tracker host | `software/a53` (Cortex-A53: `emb_tracker`, `dyn/static_alignment`, `capability`, `ipc`, `TOI_char_quality_determination`) | folded into BBE32 (`emb_tracker/`) | `Run_Emb_Tracker()` → objects/TOI |

**Shared-memory ring (authoritative: `software/common/ipc/IPC_Architecture.md`, 409 lines, Gen8):**
`software/common/ipc/{ipc_data.h,ipc_data.c,ipc_commands.h}`,
`software/r52/ipc/src/ipc_mss.c` (`IPC_M2D_Buffers`, `R52_BBE_Trigger()`, `Update_R52_IPC_Notify()`,
`Check_BBE32_To_R52_Header()`), `software/bbe32/src/ipc_dsp.c` (`IPC_D2M_Buffer[2]`,
`SP2DSP_LVL1_Handler() → Update_DSP_IPC_Notify()`, `Send_Range/Doppler/SP_Post_Proc_Complete_to_R52()`).
Types: `IPC_Header_T{crc,size,counter}`, `Ipc_Notify_Structure_T{command,lookIndex,errorField,
pingPongIndex,curIpcAddr,pfm_index}`, `Core_To_Core_ID_T{R52_TO_BBE_CORE=0,BBE_TO_R52_CORE=1}`,
commands `R52_TRIGGER_RANGE_PROCESSING_CONFIGURATION=0x10000000`,
`DSP_RANGE_CONFIGURATION_COMPLETE=0x20000000`, `DSP_RANGE/DOPPLER/SP_POST_PROCESSING_COMPLETE`,
`M2D_Buffers_T{M2D_One_Time_Msg_T{usc/smc_sram_address,Calib_Data_T,radar_position,fascia_comp},
M2D_Msg_T[2]{M2D_Payload_T{xcp_info,look_Info,vse,alignment_Info,hil_ipc_info,partners_data}}}`,
`D2M_Msg_T[2]{D2M_Payload_T{dsp_timing_data,xcp_data,rdd_stream_data,rdd_debug,det_data,det_debug,
alignment,interference,radar_capability,tracker streams,toi,rdu_stream}}`,
`IPC_Shared_Buffers_T{Ipc_Notify_Buffers[2][5],Read/WriteIndx,D2M_Debug_Msg,UserFrame,Crc_ModuleState}`.
Linker: `software/common/linker/bbe32.ld` (`.ipc_sections_d2m: *(.ipc_dsp_to_r52),*(.ipc_metadata_section)`),
`r52.ld` (`.ipc_sections_m2d`, `.ipc_sections`), `ipc_data.c: Ipc_Shared_Buffer
__attribute__((aligned(32),section(".ipc_metadata_section")))`. IRQs: R52→BBE
`DSP->SP2DSP_INT.f.SP2DSP_LVL1_IRQ=0x01`; BBE→R52 `DSP->IRQ_ENA=0x01+WUR_EXPSTATE` (GPIO0).
Sync: ping-pong `R52_M2D_Buffer_Index`, `RP/DP_Rdd/SP_Post_Proc_Buffer_Index`, CRC sentinels
(`M7_TO_BBE32_CRC_ERROR=0xA0A0A0A0` Gen7), `xthal_dcache` maintenance pre-trigger.

---

## 5. The Re-Simulation (Resim) Harness — SiL & HiL

### 5.1 SiL-DCU execution flow (the `.mf4`-in / `.mf4`-out loop)

Canonical configs (REAL): `Core_RESIM_DC_Emb_Library/DC_SIL/sil_executables/dc_config/Gen8/
SIL_Engine_Config.xml` (376 lines, v5.6, `Customer_Name=PLATFORM_GEN8`, `SIL_Entrypoint=DSPACE_MODE`)
and `…/Gen7/SIL_Engine_Config.xml` (`SIL_Entrypoint=DETECTIONS_UDP`); per-ECU
`{SRR,MRR}_DC_Lib_Control.xml` + `Emb_Lib_Config.xml`; input lists `{SIL_Input.txt, Input.json}`
(Gen8 `SIL_Input.txt` = one absolute `.mf4`; `Input.json` keys `BN_CALIFR/BN_FASETH/SRR_DEBUG/
SRR_REFERENCE` → `C:\FMU\…CEER_S13_…MF4`).

`SIL_Engine_Config.xml` schema (`<SIL_ENGINE_Configuration>`): `SIL_ENGINE_Config_XML_Version`,
`Customer_Name`, `IStep (I400)`, `Vector_Approach_Selection`, `DC_STATUS{DC_MRR,DC_SRR,DC_MRR_SRR}`,
`BAD_LOG_CHECK`, `RESIM_OUTPUT_PATH{Output_Path_Options}`, `EMBEDDED_LIB_CONFIG_PATH{SENSOR_CONFIG}`,
`RECU_LIB_CONFIG_PATH{RECU_CONFIG_00,RECU_CONFIG_01}`, `RESIM_OUTPUT_FILE_FORMAT{ORCAS_MDF4,CANAPE_MDF4,
AUTERA_MDF4,VIGEM_VPCAP,VIGEM_CCA_MDF4,X2E_VECTOR_MDF4,CANOE_VECTOR_MF4,CANoe_BLF,PCAP_OUTPUT,UPU_OUTPUT}`,
`SENSOR_STATUS{SENSOR_00..10,SENSOR_15}`, `ECU_STATUS{ECU_00..03}`,
`OUTPUT_FILE_FOLDER{CUSTOMER_CAN_OUTPUT_ENABLE,APTIV_INTERNAL_OUTPUT_ENABLE,CAN_OVER_UDP_ENABLE}`,
`CDC_WRITE_OUTPUT_ENABLE`, `SRR_MRR_ORCAS_OUTPUT_FILE_ENABLE`,
`TRACKER_INTERNAL_PERIODIC_INJECTION{GDSR_F360_INIT/PERIODIC_INJECTION_ENABLE}`,
`SIL_INJECTION_MODE (VEHICLE_EXPEDITION_FILE|VIRTUAL_SIMULATION_FILE|LIVE|VEHICLE_EXPEDITION_LIVE)`,
`LOG_REPLAY_MODE (Sequential|Continuous_FILE_Input)`,
`SIL_Entrypoint (CDC|DETECTIONS_UDP|AF_MODE|DSPACE_MODE|DETECTIONS_SOMEIP|RDD_MODE|IGNORE_…)`,
`RESIM_UDP_FRAME_TRANSMISSION{…DESTINATION_IP/PORTS}`,
`SIL_LIVE_MODE_CONFIGURATION{SIL_LIVE_MODE_TX_CONFIGURATION (AS_DSPACE_TX|BENCH), DESTINATION_IP,
MULTICAST_IP (239.255.42.99), SENSOR_PORT (5555), ECU_PORT (3490)}`,
`CALIBRATION_SOURCE/EXTRACTION`, `RESIM_LOG_INFO{LogLevel,…,PLP_TIMESTAMP,TIMING_PROFILE(_PATH),
STATISTIC_REPORT_PATH,SIGNAL_CREATION,TIMESTAMP_DIAGNOSTICS_PATH}`.

Binaries (REAL, vendored): FW `DC_SIL/sil_executables/fw_dlls[_gen8]/APT_SRR_RESIM[.exe]` —
`APT_SRR_RESIM.exe SIL_Engine_Config.xml SIL_Input_Logs.txt` (`fw_dlls_gen8/Run_Resim.bat:1`; Docker
`RUN_RESIM.sh: ./APT_SRR_RESIM "$3" $1 $2`); radar DLLs `sil_executables/radar_dlls/
{SRR7_SiL_FL/FR/RL/RR.dll, SRR8_SiL_FL/FR/RL/RR.dll, FLR7_SIL_FC.dll, FLR8_SIL_FC.dll}` (+
`libSRR7/8_SiL_*.so`, `libFLR7/8_SIL_FC.so`); DC libs built by `DC_SIL/Build{,.sh,All.bat,All.sh}`
(`cmake .. -DCMAKE_BUILD_TYPE=… -DBUILD_MODE=srr_dc|mrr_dc`, `project(DC_RESIM)` C++17,
`PROJECTNAME=SRR_DC_SIL_LIB|MRR_DC_SIL_LIB`, `CMakeLists.txt` 423 lines) into `SRR_DC_SIL_LIB.{dll,lib,so}`
+ `MRR_DC_SIL_LIB.*`; shipped FW sidecars `DPH_RR_ADAS_SIL/LOGGER`, `radar_stream_decoder`,
`srr3_decoder_dph`, `SRR3_MUDP_Log`, `Timing_Profile` (+ HDF5/ffmpeg/MDF deps).

Per-frame ABI (`DC_SIL/sil_wrappers/sil_wrapper.{cpp,h}` + `Tracker_VariantA_Wrapper`,
`olp_wrapper`, `sfl_wrapper`): `RECU_SIL_API extern C RECU_SiL_Init(void*)/Execute(const void*,const void*)/
Reset()/GetVersion(DC_Version_Info_T**)/SetFolderPath(const char*)/Exit(void*,bool)`,
`sil_status_e{E_SIL_RUN_OK,ERR,INIT_ERR,PENDING_STATE}`.
`sil_wrapper.cpp:27 RECU_SiL_Init`: `ReadConfigFromXML(SRR/MRR_DC_Lib_Control.xml)` →
`getConfigParameters().{cust,senstype,XTRK_Files}` → `InitTracker/InitOLP/InitMUDPStreams/
InitFeatureFunction/InitDebugFiles`. `sil_wrapper.cpp:51 RECU_SiL_Execute()` per scan:
`SetOutputDataPtr/SetRecuOutputHeaderdata → ProcessInputData (SIL_DC_Input_Data_T{Input_Data_Hdr_T,
UDP_CUST_FRAME_HEADER_T,Radar_Stream[40],Symbol_Record}, header Sec_Size/Checksum CalcSum16_Z2/version
SIL_ECU_DEBUG_INPUT_STRUCTURE_VERSION, boresight deg2rad fixup, DC_ValidChecks_T DataQualityChk per-sensor
ScanId/LookId/LookType/NoStale/FOV/Vua/ZeroDetect/Period/Overrun/Elev/Azimuth/MntPos/VehInfo in
sil_input.{cpp,h} + sil_input.h:21-175) → updateRunConfig (DGPS) → RunTracker (F360/rspp/OCG/SG/VSE,
Tracker_VariantA_Wrapper.{cpp,h}:412,456 InitTracker/RunTracker/TrackerReset/GetF360TrackerObject/…) →
RunOLP (olp_wrapper.{cpp,h}:355,358) → RunFeatureFunction (sfl_wrapper.{cpp,h}:792,807 SFLFillVehicleData/
ObjectData, CTA/TA/SCW/RECW/LTB/LCDA/ESA/CED/PT getters) → TransmitMUDPStreams (A5 Gen7 + RECU
CORE0/1/3/CAL/VRU/OSI/DSPACE muxed UDP, RECU_UDP_FRAME_LENGTH=1468, Radar_Logging_Data_Source_T{CORE0=90,
CORE1=91,CORE3=92,OG=93,CAL=94,ECU_INTERNAL=95,VRU=96,OSI=67,DSPACE=10}, Rec_Hdr_T REC_VER=0xA5,
SENSOR_ID_{RL=1,RR=2,FL=3,FR=4,SRR_DC=128,MRR_DC=129}) → WriteDebugFiles (HDF/XTRK/BIN/statistic/timing)`.
Scan plumbing: `recu_stream_log.cpp:239 GetScanIndex()/115-118 streamRefIndex/sourceTxCnt`,
`sil_input.cpp:98-119 SetRECUMUDPScanIndex()`, `stream_handler.cpp:145 TriggerStreamHandler{Init,Run}`,
`sil_output.cpp:12-13 CopyTrackerOutToROT{ISO,Det}Stream()`.
`SRR_DC_Lib_Control.xml` (`<Embedded_Library_Output_Control>`): `DC_Library_XML_Version, Customer_Name
(GPO), Sensor_Type (SRR8Plus…), HDF_FILES{INPUT,OUTPUT}, DC_IP_DQ_Chk_Print, XTRK_FILES, BIN_FILES,
UDP_Stream_to_use, DGPS_Decode_Status, DGPS_SM_Config`; parsed by `sil_config_read/dc_read_config.cpp:
ReadConfigFromXML()` into `Input_Config_T` with `customer_map` (BMW/FORD/CHANGAN/RNA/…/CEER/GPO) and
`sensor_map` (SRR5Plus/MRR3/SRR5/SRR3/FLR4/SRR6Plus/SRR6/FLR4Plus/SRR7Plus/FLR7/FLR8/SRR8Plus/SRR7PlusUWB)`.
DGPS isolation: `sil_dgps/DGPS_Decoder.cpp:47,68-72 SMValidation_LoadLibrary` (`dlopen RTLD_NOW|RTLD_LOCAL|
RTLD_DEEPBIND` / `LoadLibrary`), `patchelf --set-soname libSMValidation_MRR.so` (`CMakeLists.txt:406-422`).
Pre-build source patch: `update.py` rewrites `#ifdef _DEBUG → #if 1` in `f360_detection_hist.h,
f360_object_track_initialization.cpp, f360_tracker.{cpp,h}, f360_xtrk_logging.{h,cpp}`; `version.py`
(`--tracker/--ocg/--olp/--out/--strict`) stamps `versions.txt`; `update_veh_sing_xml.py` forces
`LOG_REPLAY_MODE=Continuous_FILE_Input`, `SIL_INJECTION_MODE=VEHICLE_EXPEDITION_FILE` per customer/variant;
`Generate_Veh_Singularity.sh` (217 lines) + `Create_Image.sh` (218) + `fetch_jfrog_binaries.sh` (127) package
Docker/Singularity (`RUN_RESIM.sh`, `Run_Singularity.sh`, `resim_v2_platform_*_dc_*.tar`, `*.simg`).
`.vscode/launch.json`: `program=…/build/{Debug/APT_SRR_RESIM.exe | APT_SRR_RESIM}`,
`args=[…/dc_config/SIL_Engine_Config.xml, …/SIL_Input.txt, …/output/]`,
`LD_LIBRARY_PATH=…/build`, `HDF5_DISABLE_VERSION_CHECK=2`. **No `.oz` scenario files, no named pipes,
no `IOC_` emulation** exist in the DC tree (verified by glob/grep); live transport is UDP/multicast +
file replay. `Run_Mode_T{NONE_ECU,DETECTIONS_UDP,OBJECT_MODE_ECU,DETECTIONS_PCAN,ECU_DSPACE,
ECU_DETECTIONS_SOMEIP,RUN_BIN}` (`sil_engine_headers/dc_config.h:10-62`).

### 5.2 SiL-RSP execution flow (DSP-algorithm SiL, Bazel)

Design docs: `Core_Radar_Gen7_SAF85xx/Design_Doc_CDC_SIL.md` (364 lines) + `Design_Doc_RDD_SIL.md`
(431 lines); `Core_Radar_Gen8_iND13400/Design_Doc_RDD_SIL.md` (97 lines).
APIs: `sil/rsp_sil/main/rsp_wrapper_interface/{rdd_sil_interface/rdd_sil_interface.cpp
(Rdd_To_Detection_Configuration(Radar_Look_T, Rdd_Stream_T* in/out, Detection_Stream_T*,
Down_selection_Stream_T*, Detection_Debug_Stream_T*, Vse_Stream_T*, uint8_t* radar_pos)),
cdc_sil_interface/cdc_sil_interface.cpp (CDC_To_Detection_Configuration), af_sil_interface/af_sil_api.hpp,
psp_sil_interface/{id,rc,da,sa}_…, rdu_sil_interface (Gen8), common/}`.
I/O structs: `CDC_SIL_Input_Tag{look_type; SIL_CDC_Stream_T sil_cdc_stream; Rdd_Stream_T rdd_sil_in;
Vse_Stream_T vse_sil_in; PSP_Input_Sil_T psp_sil_in; Radar_Capability_Stream_T rc_sil_in}`,
`CDC_SIL_Output_T{rdd_sil_out; af_det_sil_out; af_ds_det_output; p_af_det_debug_output; id/da/sa/rc_sil_out}`;
RDD variant with `Rdd_Data_T/RDD_Data_T` instead of `CDC_Data_T`.
Vectors: `sil/rsp_sil/data_bin/{srr7p,srr7e,flr7,flr8,srr8p,psp_data/rdd_input.bin}/
{rdd_data/CDC_stream.bin, cdc_data/rdd_input.bin}`; MATLAB generators
`sil/rsp_sil/parser_script/{rfft_bin_file_generation.m, cdc_rdd_bin_file_generation.m,
af_input_rdd_bin_file_generation.m, print_rdd_out.m, PSP_parser_script/write_rdd_data.m}` +
`tools/ITF/{Mex_Executable_Specs/Mex_Scripts/{RangeFFT,Doppler_FFT_spt*}.m,
Integrated_Testing/ResimAutoFrameWork/{t32_cfardata_collect,specmex_cfardata_collect,
Dfft_decomp_embed_data}.m, …/t32_rfft_datacube_injection.m, decompress_rfft_data.m, decomp_dfft_data.m}`.
Chain per call: copy `rdd1_num_detect, bwdep_*_thold, cfar_nf_est, rdd1_{rbinest,dbinest,abinest,rindx,
dindx,bv}` → second-pass (`second_pass_pcresim_lib`, `building_block/src/rdd_proc_pcresim.{hpp,cpp}`,
`angle_finding_pcresim.cpp`) → AF (`af_sil_lib`, down-select, fascia comp) → PSP ID→RC→DA→SA →
`Detection_Stream_T/AF_Det_T`. External algorithm bodies live outside this workspace:
`@spbb` (SPBB 3.12), `@afbb` (AFBB 1.6.01), `@idbb` (2.0.0_08), `@rcbb` (1.0.12), `@dabb` (1.0.9),
`@sabb` (1.0.12), `smc_cal_Satellite_46_79_0_5`, `usc_cal_default_6_79_0_0`.
Builds: `bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/{cdc,rdd}_sil_interface/test:…_test
--variant=srr7p|flr7|srr7e|flr8|srr8p --@spbb//common:use_bbe_cstub_simulator=… --@afbb//module/_common:
sil_config_enable=True [--psp_sil_config=true …]`; ITF DLL `//:rdd_sil_lib` (`cc_binary linkshared`,
`DLL_EXPORT`, MEX vs LOG input modes, §6 of the RDD doc); Gen8 `//sil/…/rdu_sil_interface/test:rdu_sil_Test
[--config=rdu_dev_can]`.

### 5.3 HiL execution flow

Harness: `Core_RESIM_HIL_Engine/ApplicationProjects/SRR3_Resimulator/` —
`SRR3_HiL_Resim/srr3_hil_resimulator.cpp` (`main`), `SRR3_HiL_Exec/{srr3_hil_exec,HiLExecutive,
HiLController,HiLOfflineStream,HiLOnlineStream,SyncManager}.{cpp,h}` (+ `HiLApp.ini`),
`SRR3_Comm/{asio_socket,UdpIpConn,UdpIpConn_Raw_Sock,srr3_comm,ThBase}.{cpp,h}`,
`SRR3_HiL_Resim/{ThMudpGrab,CallBackProcs_*,DataQ,Xml_Read}.cpp`,
`VehicleTransmitter/VEH_XMIT/`, `EthernetGrabber/`, `mudp_grabber/`, `tinyLogger/`, `Dessector/`,
`FDX_Protocol/ConfigFiles/FDXDescription_HiL_V1.0.xml`, `HiLInputs/{Hil_Configuration.xml (v9.5),
log_path.txt, Channel.txt, HiLApp.ini, Error_Codes.txt, SRR_HiL_Resim.{bat,sh}, SRR_HiL_Resim_Live.bat}`,
`output/`; `CoreLibraryProjects/{CCA_ViGEM,CCA_Vpcap,CrossPlatform,dvlFile,dvlMessage,excel_utils,mdf_log,
mdf_log_convertor,mudp_decoder(_calib),mudp_log,mudp_receiver,MUDP_Serializer,ptp_to_xml_generator,
radar_stream_decoder,SRR3_Internal_Data_Logger,UDP_Transmitter,XCP_SeedKey(_Lib)}`; `Common/IRadarStream*.h`.
`Hil_Configuration.xml` (`<HIL_SRR_Configuration>` v9.5): `CUSTOMER_NAME (PLATFORM_GEN5)`,
`SENSOR_CONNECTION_STATUS{REAR_LEFT/RIGHT,FRONT_RIGHT/LEFT,FRONT_CENTER,BPR_RIGHT/LEFT,RADAR_ECU}`,
`INPUT_OPTIONS (SIX/FOUR/SINGLE_DVSU_INPUT|MDF4_INPUT)`, `CONTINUOUS_RUN_MODE`, `LOG_REPEAT_COUNT`,
`RADAR_RADIATION_MODE`, `HIL_MODE_ENABLE{VEHICLE_DYNAMIC_SOURCE,TIMESTAMP_SOURCE,FUSION_DETECTION_SOURCE
(UDP|CAN),RADAR_FUSION_ENABLED,INPUT_DATA_TYPE (LOG_SIMULATION|LIVE_SIMULATION_*)}`,
`AUTOCLOSE_COMMAND_WIN`, `FDX_SYNCH`, `KPI_OUTPUT`, `DEBUGGING_HIL`,
`UDP_PORT_{DATA (6001),XCP (4410),LOGGING (5555)}`, `INTERFACE_HIL_IP/JSON_INTERFACE_IP/INTERFACE_PORT
(5544)`, `TOBJECT_INJECTION_TYPE (DSPACE|INTERNAL)`, `HIL_ENGINE_IP/SCALEXIO_IP/PORT (5556)`,
`LIVE_ZERO_DATA_SOURCE`, `DETECTION/TRACKER/RDD_COMPONENT`, `SPECIAL_MODE_SENSORS_OFF`,
`FAULT_INJECTION{SET_FAULT_FROM/TILL,INJECT_FAULT_SENSOR_RL/RR}`.
Run: `SRR_HiL_Resim.exe Hil_Configuration.xml log_path.txt` → `PluginLoader`
(`CommonFiles/plugin/PluginLoader.{h,cpp}`: `Load_SRR3_Log/Comm/Decoder/dph_sil_iface/Stream_Decoder/
AUDI_SRR3_Decoder`, call sites `srr3_hil_resimulator.cpp:2280,2282,2674,2960,3515,3535`,
`ThMudpGrab.cpp:439,445`; ABI `Common/IRadarStream_Lib_Loader.h:31-62`, base
`Common/IRadarStream.h:370-494`) loads `SRR_HIL_Exec/srr_comm/srr_decoder_dph/radar_stream_decoder` →
`CHiLController` spawns per-sensor `CHiLExecutive` (`RL/RR/FR/FL/FC/BPR/BPL`) → XCP handshake →
`HiLExecutive.cpp:613 Execute() → 704 ExecuteOnline() / offline; 3538 ExecuteOnline() → 3825
RequestNextFrame()/3233 GEN5_RequestNextFrame() → 3838 ProcessOnline(m_CurrentScanIndex)/3212 →
3228 HiL_Injection(m_pLiveStream)/2380 or 2528 HiL_Injection_Offline → 2166 Process()` →
UDP bench inject (`asio_socket::asio_send_to/receive_from/handle_timeout`, `UdpIpConn::{Init,
Udp_Receive,Udp_Send,Close}`, multicast `239.255.42.99`) with `SyncManager::Sync/DoWait` barrier
(`Sync_Info_T{timestamp,sourceIdx,hWaitEvnt}`, `m_MaxTm/m_bSyncComplete`), FDX sync
(`ReadFDXData/MDFsychwithReceivedtimestamp/SetFilePosition`), XCP sequencing
(`Enable/Disable/ReEnableHILMode`, `Activate_HIL_GEN7Mode`), traces
(`output/x64/{RELEASE,DEBUG}/Hil_Configuration.xml`, `SRR3_HiL_Trace*.txt` with
`Scan Index Drop Count / Total Sent Scan_Count`, `DEBUG/timeDiff.csv`).
Build: `Build_Project.sh` / `SRR3_Resimulator/Build_Project.{sh,bat}` (`cmake
-DCMAKE_BUILD_TYPE={RELEASE|DEBUG} -DBUILDCONFIGTYPE=HIL`), `.vscode/{launch.json
(program=…/output/SRR_HiL_Resim), tasks.json (g++-9), settings.json, c_cpp_properties.json}`.

### 5.4 Why input `.mf4` → output `.mf4`

The input `.mf4` is the *recorded ground truth* (vehicle sensors + vehicle motion + environment). The harness
substitutes the *candidate* radar/DC software for the recorded ECUs while replaying identical bus inputs, so
any byte difference in the output `.mf4` is attributable to the software change — the definition of a
regression experiment. KPI then diffs the pair (Section 6), and the loop closes: fail → fix → rebuild
(`BuildAll` / `bazelisk`) → re-resim → re-KPI.

---

## 6. The KPI Verification Suite & Mathematical Verification Engine

### 6.1 Suite layout (REAL)

```
Core_RESIM_KPI/
  build_simg.sh  ResimHTMLReport.def  requirements.txt  HTML_docker_Doc.docx
  gen7v2_resim_kpi_scripts/   # UDP KPI (CSV-level, GEN7v2): THE reference implementation
    run_kpi_script.{py,bat,sh}  config.py  constants.py  variables.py  meta_data.{py,json}
    file_handling.py  logger.py  README.md  requirements.txt  log_path.txt  .vscode/
    detection_matching_kpi_script.py  downSelection_matching_kpi_script.py
    alignment_matching_kpi_script.py  interferenceDetection_matching_kpi_script.py
    radarCapability_matching_kpi_script.py  tracker_matching_kpi_script.py
    tracker_info_kpi_script.py  tracker_processed_det_kpi_script.py
    tracker_vehicle_info_kpi_script.py
  can_kpi_hdf/                # CAN KPI (HDF-level): layered a_/b_/c_/d_ + kpi/
    kpi_main.py  kpi.json  kpi_smoke.json  can_kpi.spec  can_singularity_KPI.def
    a_persistence_layer/{hdf_parser.py,hdf_wrapper.py,json_parser.py}
    b_data_storage/{can_kpi_data_model_storage.py,kpi_config_storage.py}
    c_business_layer/kpi_business.py
    d_presentation_layer/kpi_html_gen.py
    kpi/{sil_radar_validation.py,sil_log_narrative.py}
  can_kpi_scripts/can_xml_kpi_scripts.py   # CAN KPI (XML/CSV-level): cycle-compare reference
  IPS/                        # HTML report pipeline: DataCollect→DataPrep→DataStore→PlotConvert→RepGen
    ResimHTMLReport.py  plots_gen.py  NIPS_to_IPS_ploting.py  connect_to_server.py
    HTMLConfig.xml  Inputs.json  input_gt_vs_tracker.xml  input_tracker_vs_tracker.xml
    DashManager/report_dash.py  DataCollect/{radar_hdf_datacollect.py,radar_mdf_datacollect.py,
      dc_radar_hdf_datacollect.py,can_radar_hdf_datacollect.py,parsercontext.py,icollectdata.py}
    DataPrep/{radar_dataprep.py,plot_dataprep.py,dc_dataprep.py,can_dataprep.py}
    DataStore/{radar_datastore.py,plot_datastore.py,json_datastore.py,idatastore.py}
    PlotConvert/{data_to_json_convert.py,dc_data_to_json_convert.py,can_data_to_json_convert.py}
    RepGen/{json_to_html_convert.py,dc_json_to_html_convert.py,can_json_to_html_convert.py}
    Sig_Prep/{sig_filter.py,isig_observer.py}  EventMan/{data_event_mediator.py,ievent_mediator.py}
    InputParsing/input_parsing.py (HDFConfigSingleton)  Metadata/GEN7V2/poi.py
    DGPSFilter/dgps_filter.py  hdf_temp_manager.py  Utilities/hdf_validator.py
  main_html/                  # offline/online portal + HPCC services + mirrored KPI libs
    code/{main.py,html_offline/{main.py,html_build.py,html_build_backup.py},
      html_online/{main.py,cluster_connect.py,vlm.py}}
    all_services/{app.py,hpcc_main.py,build_static.py,generate_upload.py,resources.py,
      progress_tracker.py,hpcc_runtime_store.py, KPI/UDP_KPI/c_business_layer/{detection_matching_kpi.py,
      tracker_matching_kpi.py,alignment_matching_kpi.py,kpi_factory.py}, KPI/can_kpi/…,
      KPI/can_interactive_plot/,intplot_kpi/,mcp/,rag/,jira/,scripts/,tests/}
  profile_timing_plotter/timing_plotter.py
```

`requirements.txt` (35 lines): `flask,flask-cors,paramiko,requests,urllib3,numpy,pandas,plotly,h5py,
opencv-python,torch,transformers,qwen-vl-utils,pyyaml,cryptography,bcrypt,pynacl,cffi,pycparser,zmq,
psutil,scipy,matplotlib,scikit-learn,scikit-image,opencv-python-headless,protobuf,tqdm,pyinstaller`
(+ `pytorch-lightning,torchaudio,torchvision`). Container: `ResimHTMLReport.def` (91 lines,
`FROM python:3.11-slim`, `libhdf5-dev`, `numpy,pandas,h5py,plotly,matplotlib,pyzmq`,
`python -m IPS.ResimHTMLReport config input report`, `IRep{N}_{basename}` outputs),
`build_simg.sh` (33 lines, `apptainer|singularity build ResimHTMLReport.simg`).

### 6.2 UDP KPI matching engine (exact two-tier algorithm)

Config (`gen7v2_resim_kpi_scripts/config.py:41-57`, `constants.py:4-5`): `Constants.EPSILON=0.0000001`,
`SCALE_P21_TO_FLOAT=4.768371582e-07`, `RAN_THRESHOLD=0.01+EPSILON` m, `VEL_THRESHOLD=0.015+EPSILON` m/s,
`THETA_THRESHOLD=0.00873+EPSILON` rad (≈0.5°), `PHI_THRESHOLD=0.00873+EPSILON` rad,
`AZ/EL_MISALIGNMENT_THRESHOLD=0.01+EPSILON` deg, `ID_THRESHOLD=0`, `RC_STATUS_THRESHOLD=0`,
`DEFAULT_ACC_THRESHOLD=99.0`, `MAX_NUM_OF_RDD_DETS=512`,
`MAX_NUM_OF_AF_DETS=768` front / `680` corner (`MAX_NUM_OF_AF_DETS_FRONT/CORNER_RADAR`, selected by sensor
position), `MAX_NUM_OF_DS_DETS=128/64`, `RANGE_SATURATION_THRESHOLD=135.0` (overwritten from
`meta_data.json Range_Saturation_Thresh_*`), `MAX_CDC_RECORDS=5016`, `RADAR_CYCLE_S=0.05`,
`MAX_NUM_OF_SI_TO_PROCESS=0` (unlimited). File suffixes: `DET=_UDP_GEN7_DET_CORE.csv`,
`RDD=_UDP_GEN7_RDD_CORE.csv`, `CDC=_UDP_CDC.csv`, `VSE=_UDP_GEN7_VSE_CORE.csv`,
`ALIGN=_UDP_GEN7_DYNAMIC_ALIGNMENT_STREAM.csv`. State: `variables.py:Log_Vars`
(`num_of_SI_in_veh_af/sim_af/veh_and_sim_af/_rdd/_merged`, `ran/vel/theta/phi_diff_list`,
`accuracy_list`, `overall_accuracy`, `overall_accuracy_excluding_cdc_saturation`,
`perc_of_scans_with_cdc_saturation/range_saturation`, `dist_travelled_by_veh/sim`).
Pairing: `file_handling.py:24 find_file_pairs` (regex `_rR00\d+_`), `find_det/ds/align/id/rc_related_data_files`.

Entry: `detection_matching_kpi_script.py:35 process_one_log(veh_csv,sim_csv,veh_rdd_csv,sim_rdd_csv,
veh_cdc_csv,sim_cdc_csv,veh_vse_csv,sim_vse_csv)->bool`, `:655 plot_stats()`, `:872 plot_data_across_logs()`.

**Scan-index hygiene (59-82, 169-192):** `veh_det_df[scan_index!=0 & num_af_det!=0].
drop_duplicates(scan_index)`; `merged_det_df=pd.merge(veh,sim,on='scan_index',how='inner')` (same for RDD
with `rdd1_num_detect!=0`); KPI `result1=% SI same num dets`.

**Tier 1 — integer `(R_index, D_index)` pairing (`count_rindx_dindx_matches`, 243-258, 287-290):**

```python
veh_pairs = list(zip(row[rindx_cols_veh[:num_detect_veh]], row[dindx_cols_veh[:...]]))
sim_pairs = list(zip(row[rindx_cols_sim...],      row[dindx_cols_sim...]))
for veh_pair in veh_pairs:
    if veh_pair in sim_pairs: match_count += 1; sim_pairs.remove(veh_pair)  # 1:1 without replacement
merged_rdd_df['matched_rindx_dindx_pairs'] = ...
matched_pct = matched / rdd1_num_detect_veh          # :287
# SI with matched_pct >= 0.99 counted                 # :290
```

RDD range/rate cross-check (261-285): `sim_range*SCALE_P21_TO_FLOAT` rounded 3-dec vs `veh_range`,
`|sim-veh| ≤ RAN_THRESHOLD and VEL_THRESHOLD`.

**Tier 2 — bounded float validation (`count_det_params_matches`, 452-511, 526):**

```python
# anchor on exact (rdd1_rindx, rdd1_dindx) equality, then greedy matched_sim_indices set:
if ran_diff_abs <= RAN_THRESHOLD and vel_diff_abs <= VEL_THRESHOLD: subset += 1
    if theta_diff_abs <= THETA_THRESHOLD and phi_diff_abs <= PHI_THRESHOLD: all += 1
else: log_vars.ran/vel/theta/phi_diff_list.append((veh_ran,veh_vel,veh_theta,veh_phi,
                                                   f_single,superres,bistatic,diff))
matching_pct_det_all_params = all / num_af_det_veh
overall_accuracy = sum(all) / sum(veh) * 100          # :526
```

i.e. the brief's contract exactly:

```
|r_in − r_out| ≤ ε_r  (0.01 m),  |ṙ_in − ṙ_out| ≤ ε_v  (0.015 m/s),
|θ_in − θ_out| ≤ ε_θ  (0.00873 rad),  |φ_in − φ_out| ≤ ε_φ  (0.00873 rad).
```

**Edge cases / regression flags:** unmatched input detections (in `veh_pairs` but never consumed from
`sim_pairs` → miss), ghost output detections (leftover `sim_pairs` → false positive), CDC saturation
(`num_cdc_records==MAX_CDC_RECORDS`, `:146`, excluded via `overall_accuracy_excluding_cdc_saturation`),
range saturation (`max(ran)≥135 and sim<135`, `:438`), VSE mileage (`dist=(veh_speed*0.05).sum()`, `:572,577`;
`mileage_yield=sim/veh*100`), `DEFAULT_ACC_THRESHOLD=99.0` gate per SI.
Siblings: `downSelection_matching_kpi_script.py:35 process_one_log` (cols `rdd_idx_i,ran,vel,theta,phi` ×
`MAX_NUM_OF_DS_DETS`, `:129 count_det_params_matches`, exact `veh_rdd_idx==sim_rdd_idx` then the same four
epsilons `:163-164`); `alignment_matching_kpi_script.py:82 main` (`alignment_mode`,
`vacs_boresight_{az,el}_{nominal,kf_internal,estimated}`, `az_misalign_est=(nominal-estimated)*180/pi
:138-145`, **`scan_index_diff=diff()!=1` continuity plot `:148`**, `|az_diff|<AZ_MISALIGNMENT_THRESHOLD
:187-188`); `interferenceDetection_matching_kpi_script.py:53 main`
(`interference_detected_diff=real-sim :93`, `|diff|≤ID_THRESHOLD(=0) :103` exact);
`radarCapability_matching_kpi_script.py:86 main` (`blockage/dm/re/sll_status` vs `RC_STATUS_THRESHOLD=0` +
`probability/value/current_dB/max_det_range_*` plots); tracker-file trio
(`tracker_info_kpi_script.py:8` latency/info `elapsed_time_s,reduced_num_active_objs,
num_active_clusters,number_of_historic_detections`; `tracker_processed_det_kpi_script.py`
`vcs_long/lat_posn/vel_*`; `tracker_vehicle_info_kpi_script.py` `world_x/y,speed,curvature` — all
merge-on-`scan_index` plot pattern, no tolerances).

### 6.3 CAN KPI verification engine

**Reference cycle-compare** (`can_kpi_scripts/can_xml_kpi_scripts.py`):
`compute_match_stats(map1..4,tol):317` with REAL tolerances `:326-330` —
`range_tolerance=0.016`, `range_rate_tolerance=0.016`, `azimuth_tolerance=0.00873`,
`elevation_tolerance=0.00873`, `rcs_tolerance=1` (unused); note order `det_A=[range,range_rate,
elevation,azimuth]`; greedy `|dA-dB|≤tol` + `not selected_flag :343`, `%=matched/number_of_detections1*100
:349`. `compare_cycles(cyclesA,cyclesB):587` — `TIMESTAMP` float diff `b-a`; junk `1.84467E+10` skip
`:633-637`; `diff>0.040` skip input `:642`, `diff<0` skip output `:647` (the 40-ms rule);
`delta=(ts_b-ts_a) :765`; **scan continuity** `b.scanindex-a.scanindex==0 → PASSED else FAILED :772-778**;
`%=total_matched/max_idx*100 :820`. Helpers: `UpdataDetectionMarkerBasedOnRadarPosition:357`
(`FLR/SRR_FL/FR/RL/RR_DETECTION_001_004`), `collect_messages_between_markers:490`
(`DETECTION_*` + `HEADER_* → HED_SCAN_INDEX/HED_NUM_OF_VALID_DETECTIONS`), `create_multi_set_plot:183`.
Cadence: fixed 50-ms scan-index cycles (20 Hz), same `RADAR_CYCLE_S`.

**HDF production path** (`can_kpi_hdf/kpi_main.py:31 KpiMain.run/run_pair:102`,
`c_business_layer/kpi_business.py:16 KpiBusiness`): `MATCH_SIGNALS=[DET_RANGE,DET_RANGE_VELOCITY,
DET_AZIMUTH,DET_ELEVATION] :17`, `MATCH_EPSILON=10.0 :18` (quantization step — see below),
`TIME_MATCH_TOL_NS=2_000_000 :19` (2 ms), `SENSOR_ORDER=[CEER_FL,CEER_FLR,CEER_FR,CEER_RL,CEER_RR]`;
`_quantize(v)=round(v/10.0) :183-184`, `_match_detections_hashmap:211` (4-D quantized hash + 81 neighbor
offsets `product([-1,0,1],repeat=4) :39`, Manhattan-sorted), `_match_1d_hashmap:186`, `_compute_match_pct:44`
→ `align_storage_rows_by_scanindex:63`, denom `max(in,out)`, `overall=tp/denom, precision=tp/(tp+fp),
recall=tp/(tp+fn), f1, accuracy=tp/(tp+fp+fn) :148-160`.
Persistence: `a_persistence_layer/hdf_parser.py:159 align_storage_rows(time_tolerance_ns=2M)` two-pointer
`|in_time-out_time|≤tol :193` else advance smaller; `:210 align_storage_rows_by_scanindex` →
`_align_scan_only:224` exact-int scan dict. Geometric validator: `kpi/sil_radar_validation.py:651
match_points(…,gate_threshold)` + `linear_sum_assignment/cdist` (CLI `--gate` default `1.0 :1073`).
**Track continuity / ID retention / drops / latency:** tracker matcher
(`tracker_matching_kpi_script.py:8 main`, `:71 match_tracks`): cols `trkID_i,vcs_xposn/yposn/xvel/yvel,
f_moving` × `max_number_of_data=64 :390`, `max_valid_distance=160 :391`, `epsilon=1e-7 :392`,
`vcs_xposn_threshold=0.01+eps :393`, `yposn 0.01`, `xvel/yvel 0.02 :394-396`; valid `trkID>0 &
|x|,|y|≤160 :74-75`; **track-ID NOT matched** — greedy nearest with
`threshold_mul_factor=int(max(|x|,|y|)/10)` if `≥10 :92-93` then
`|dx|≤mul*xposn_thr and |dy|≤mul*yposn_thr and |dvx|≤mul*xvel_thr and |dvy|≤mul*yvel_thr :110-113`
(+ `f_moving==1` both → `mov+=1`); `matching_pct_all=all/num_valid_veh :128`, `100%`-SI KPIs
`result2/3 :145-146`. Latency: header-vs-detection `ΔT` analysis (Section 3.4), `tracker_info`
`elapsed_time_s`, IPS `TIMESTAMP_DIAGNOSTICS_PATH`.

**HDF twin of UDP matchers** (`main_html/all_services/KPI/UDP_KPI/c_business_layer/`):
`detection_matching_kpi.py:117 process_rdd_matching` (`veh_hash[(rindx,dindx)]`),
`tracker_matching_kpi.py:89 process_tracker_matching`, `alignment_matching_kpi.py`, `kpi_factory.py`,
`kpi_server.py`, `runtime_utils.py`.

### 6.4 Report generation pipeline

`IPS/ResimHTMLReport.py:67 main` — `args config,input,report_path`; `ET.parse(config).text→source_type`;
`poi.datasource=source_type`; branches `udp → RadarHDFDataCollect+PlotDataPreparation+DataToJSONConvert+
JsonToHtmlConvertor`; `udpdc/dcdgps → …DC… (+DGPSFilter+create_filtered_temp_hdfs if dcdgps)`;
`mcip_can/ceer_can → …CAN…`; mediation `DataEventMediator.register(…)`,
`ParserContext(radar_data_collect).execute(in_use,out_use)` per `HDFConfigSingleton.all_pairs()` →
`IRep{N}_{basename}`; `HDFValidator.validate_hdf_pair` gate; `ReportDash.report_gen_time, generate_rep()`.
Collect: `DataCollect/radar_hdf_datacollect.py:52 parse_and_collect_data` per sensor
(`h5py`, dedup/zero-strip → `RDS_UPDATED`); filter: `Sig_Prep/sig_filter.py`
(`get_poi_ref/parse_group_signals/final_poi_signals/notify`); POIs: `Metadata/GEN7V2/poi.py:11 poi_data`
(`ran,vel,phi,theta,snr,rcs → SC/HIS/SC_MM/Bar_MM`, `scan_index`, `04_OLP`, `DOWN_SELECTION`) + `poi_data_DC`;
render: `RepGen/json_to_html_convert.py:65 trigger_json_to_html_conversion` per `FC,FL,FR,RR,RL` →
`*_detection_scatter/histogram.html`; dash: `DashManager/report_dash.py:22 ReportDash`
(`signal_stats{match,mismatch}`, `report_directory/input_hdf/output_hdf`, `export_png`, `generate_rep/
clear_data`); plots: `plots_gen.py`, `NIPS_to_IPS_ploting.py`, `profile_timing_plotter/timing_plotter.py`.
Portal/services: `main_html/code/main.py:20 main()` router (`--serve/--vlm → html_online.main` else
`html_offline.main`), `html_offline/html_build.py`, `html_online/{main.py,cluster_connect.py,vlm.py}`,
`all_services/{app.py,hpcc_main.py,build_static.py,generate_upload.py,resources.py,progress_tracker.py,
hpcc_runtime_store.py}` (+ `rag/`, `mcp/`, `jira/`, `tests/test_check_hdf_pairs.py`).

---

## 7. Build System Reference

| Repo | System | Key files | Canonical commands |
|---|---|---|---|
| Gen7 radar | Bazel/Bazelisk | `bazelisk(.exe)`, `.bazelrc`, `.bazeliskrc`, `MODULE.bazel`, `bb.MODULE.bazel`, `extensions.bzl`, ~100 `BUILD` files | `bazelisk build //:gen7 --variant=srr7e\|flr7\|srr7p\|flr7v3 --board=A1 --micro_revision=ES2` (+ `--tracker_variant=…`, `--enable_cdc[_crc]`, `CDC_2K/5K_BINS`, `--stub_tracker`, `--enable_stream_generation`, `--enable_id_testing/--enable_bench_testing/--align_det_test/--enable_sra_doppler`); `bazelisk test //:all_unit_tests`, `//:tests_bbe --variant=…`; `//coverage:report\|bbe_report`; `//software/{m7:windriver_v7_cov,a53:windriver_v7_cov,bbe32:xtensa_bbe32_cov}`; `//:rdd_sil_lib` (ITF DLL); `//:fuzz`, `//:valgrind_all_unit_tests` |
| Gen8 radar | Bazel/Bazelisk | same + `catalog-info.yaml`, `mkdocs.yml`, `.mcp/` | `bazelisk build //:gen8 --config=flr8\|srr8p --veh_com=can\|someip\|can_standalone\|al_can_standalone\|none --tracker_variant=disabled\|partner_sensor\|platform_flr8_standalone\|platform_srr8p_standalone\|platform_srr8p_2_sensor_fusion\|AL --chandra_hw_rev=B0 --mars_hw_rev=B0` (+ `--enable_phy_100Mb`, `--Integration_Testing --AF_IT --RC_IT --FP_IT --TOI_IT`); `//sil/…/rdd_sil_interface/test:rdd_sil_Test [--config=rdu_dev_can]`, `//sil/…/rdu_sil_interface/test:rdu_sil_Test`, `//:rdu_tests`; `//:gen8` outputs `phy_100mb_compat_guard`, `save_build_config`, `generate_memory_stats/stack_analysis_bbe32/flash_memory_stats` |
| UDP decoder | CMake + VS | `CMake/CMakeLists.txt` (`HDF5_INC=HDF/include`, link `hdf5/hdf5_cpp/hdf5_hl` `.lib`/`.so`), `HDF/{bin,highfive,include,lib}` | VS `radar_stream_lib.vcxproj` / CMake build; version map `Decoder_DLL_Release_Notes.xml` (head `26.09.36`, 2026-09-03) |
| Bordnet tool | CMake + `.bat/.sh` | `Build.{bat,sh}`, `CMake/`, per-decoder `CMake/` | `Build.bat/sh` per variant (AL/CEER/MCIP × PCAN/VCAN/SOMEIP × FL/FR/FLR/RL/RR) |
| DC_SIL | CMake 3.5+ | `DC_SIL/CMakeLists.txt` (423), `Build{,All}.{bat,sh}`, `version.py`, `update.py`, `update_veh_sing_xml.py`, `Generate_Veh_Singularity.sh`, `Create_Image.sh`, `fetch_jfrog_binaries.sh`, `input_path.xml` | Single: `Build.bat/sh` (prompt Release/Debug × srr_dc/mrr_dc); Both: `BuildAll.bat/sh`; Docker/Singularity via Generate/Create scripts; debug via `.vscode/launch.json` (`APT_SRR_RESIM[.exe] SIL_Engine_Config.xml SIL_Input.txt output/`) |
| HIL engine | CMake + VS sln | `Build_Project.sh`, `SRR3_Resimulator/Build_Project.{sh,bat}`, `SRR3_Resimulator/CMake/CMakeLists.txt`, `DPH_RR_ADAS_HIL/*.sln` | `cmake -DCMAKE_BUILD_TYPE={RELEASE\|DEBUG} -DBUILDCONFIGTYPE=HIL .. && make`; run `SRR_HiL_Resim.exe Hil_Configuration.xml log_path.txt` |
| KPI | pip + Apptainer | `requirements.txt` (×3: root, `can_kpi_scripts`, `main_html/code`, `profile_timing_plotter`), `ResimHTMLReport.def`, `build_simg.sh`, `run_kpi_script.{py,bat,sh}`, `can_kpi.spec`, `ResimHTMLReport.spec`, `log_viewer_*.spec` | `pip install -r requirements.txt`; `python run_kpi_script.py` (UDP CSV KPI); `python -m IPS.ResimHTMLReport config input report` or `sudo apptainer build ResimHTMLReport.simg ResimHTMLReport.def`; `python timing_plotter.py` |

Toolchains: `repo_init.py` (Python 3.7+, 3.10 recommended, `.netrc` API key, git submodules incl.
`software/r52/autosar/sip`, `Core_Radar_Python_Framework`, `ADVRADAR_Gen7_Exec_Spec`,
`Core_Radar_Gen8_iND13400_Matlab`), Trace32/Lauterbach (`tools/lauterbach`), Coverity, ASan/UBSan
(`asan.toml/ubsan.toml`), Valgrind (`valgrind.toml`), fuzz (`:fuzz`), Grafana (`grafana_config/`),
pre-commit (`.pre-commit-config.yaml`), CI (`.github/workflows/quality-checks.yml`), `miss_hit.cfg`.

---

## 8. Failure-Mode & Traceability Index

| # | Failure mode | Current implementation & location | Root cause (equations/logic) | KPI tripwire | Portal / IDF pointer |
|---|---|---|---|---|---|
| F1 | Rain-spray / wet-surface track dropout | `emb_tracker` fixed `R0`; `Run_Emb_Tracker()` (`software/*/emb_tracker/emb_tracker_wrapper.cpp`); DC `F360TrackerLib/StateManager` | Fixed measurement covariance; clutter variance unmodeled | UDP `overall_accuracy` dip on wet logs; CAN recall drop | IDF-1 (dynamic `R(t)` scaling) |
| F2 | Multipath ghosts (overpasses, guardrails) | AF `f_single/superres/bistatic` flags (`detection_stream.h`); 2nd-pass `below_sp/zdb/hvc_thold` (`rdd_stream.h`) | Single-slope geometry admits virtual intersections | Ghost `sim_pairs` leftovers; false-positive braking | IDF-2 (multi-look slope disambiguation) |
| F3 | Inter-satellite split during high yaw | Per-head DSP clocks; DC fusion without odometry extrapolation (`sil_wrapper.cpp:51` pre-`RunTracker`) | Missing `Δt` motion compensation `p(t+Δt)=p+v·Δt+½a·Δt²`, yaw rotation `R(ωΔt)` | CAN `scanindex` FAILED + ID fragmentation across heads | IDF-3 (async temporal extrapolation) |
| F4 | UDP-hit → CAN-track handoff drop (maneuvers) | Fixed Mahalanobis gate `γ` in tracker gating; overwrite in `f360_tracker.cpp` / `StateManager` | Static `d²=νᵀS⁻¹ν≤γ` rejects high-innovation valid returns | UDP matched but CAN unmatched (handoff yield) | IDF-4 (adaptive `γ_adaptive`) |
| F5 | CDC saturation truncation | `MAX_CDC_RECORDS=5016` (`config.py:51`); `CDC_2K/5K_BINS`; `num_cdc_records==MAX_CDC_RECORDS` (`:146`) | Fixed CDC record budget vs dense scenes | `perc_of_scans_with_cdc_saturation`, excluded-accuracy delta | Capacity planning; portal simulator |
| F6 | Range saturation asymmetry | `RANGE_SATURATION_THRESHOLD=135.0` (`config.py:52-53,57`); `:438` check | Far-target clipping differs veh vs sim | Range-sat SI fraction | Config `meta_data.json` override |
| F7 | Bumper/fascia mis-alignment bias | `Execute_Fascia_Compensation`, `Plt_Mounting_Data_T`, SMC/USC tables; `af_smc/usc_mismatch_flag` | Stale mounting/cal tables | Alignment `\|az_diff\|` KPI; `AF_Diagnostics_T` flags | Alignment KPI script |
| F8 | Dynamic-align drift (road-clutter lock loss) | `dyn_alignment/`, `da_sil_interface`, `Dynamic_Alignment_Log_Stream_T` | Clutter-model mismatch | `alignment_matching_kpi_script` result1/2 | Same |
| F9 | Interference false detections | `IDBB` (`interference_detection`), `ID_Stream_T`, `interferenceDetection_matching_kpi_script.py` | Jammer energy passes CFAR | `interference_detected_diff≠0` | ID KPI |
| F10 | Scan-index discontinuity / drops | `scan_index_diff=diff()!=1` (`alignment:148`); `b.scanindex-a.scanindex==0` (`can_xml:772-778`); `SRR3_HiL_Trace*.txt` drop counts | Dropped frames, replay overruns, sync loss | Continuity FAILED rate; HiL trace counters | CAN KPI + HiL logs |
| F11 | Timestamp/latency regression | `ΔT=T_header−T_detection`; 40-ms rule (`can_xml:642,647`); 2-ms HDF window (`kpi_business:19`); `timing_plotter.py`; `timeDiff.csv` | Serialization/scheduling jitter | Latency histograms, `TIMESTAMP_DIAGNOSTICS_PATH` | Timing plotter |
| F12 | Variant/version mismatch (stream V-shears) | `Decoder_DLL_Release_Notes.xml` (e.g. `26.09.36`), `radar_sw_config.h` per-V constants, `ST_S_*_DBC_BASE_VER_*` | Veh log cut with different stream/DBC version than sim | Mass mismatch across all KPIs | Release-notes gate |

---

## 9. Glossary Pointer

Full searchable glossary (CDC, RDD, AF, XTRK, MSS, DSS, IOC, SiL, HiL, ViGEm, DBC, Scan Index, Look Index,
RDU, PSP, SMC/USC, TOI, VSE, F360, OLP, BBE32, MIPI, XCP, FDX, MDF4, HDF5, bodnet, SOME/IP, CFAR, DoA,
Mahalanobis, EKF, AUTOSAR, …) plus a 10-question self-assessment are implemented in the companion portal
`radar_resim_learning_portal.html` (Section 6 equivalent UI). Patent blueprints are in
`RESEARCH_INVENTIONS_AND_ROADMAP.md` (IDF-1…IDF-4 + bottleneck matrix + validation plans).

---

*End of SYSTEM_ARCHITECTURE_DEEP_DIVE.md — generated from workspace audit 2026-09-20. Pair with
`radar_resim_learning_portal.html` (interactive teaching portal) and `RESEARCH_INVENTIONS_AND_ROADMAP.md`
(R&D / patent dossier).*

---

# APPENDIX A — MULTI-BRANCH SNAPSHOT MANIFEST & NEW-REPO DEEP DIVES (audited 2026-09-23)

> Update to §1.2 workspace map: the workspace now contains **11 git repositories** (was 9).
> Two new repos were added: `core-radar-gen7-awr294x` (Gen7v1 AWR294x satellite FW, ~240 remote branches)
> and `core-resim-logic-model` (LM2 logic-model / FMU, 11 remote branches).
> Per-branch full source snapshots are materialized under `_branch_snapshots/<repo>/<branch_path_with__/>/`
> so every main branch is browsable side-by-side without `git checkout`. Each snapshot dir has a sibling
> `_BRANCH_CATALOG.txt` listing **every** remote branch as `ref @@ sha @@ date @@ subject`.

## A.1 Snapshot inventory (50 branch copies total, ~61 GB)

| Repo | Remote branches | Snapshot folders materialized | Snapshot size |
|------|----------------:|-------------------------------:|--------------:|
| Core_Radar_Gen7_SAF85xx | 280 | 10: `dev`, `release__v12.0.x`, `release__v11.0.x`, `release__v10.0.x`, `release__v9.0.x`, `release__v8.1.x`, `feature__RSP_SIL_Integration`, `feature__CDC_SIL`, `feature__gen7v2_rdd_sil_dev`, `feature__EmbLibv11.0.x` | 11.4 GB |
| Core_Radar_Gen8_iND13400 | 553 | 10: `dev`, `release__v7.0.x`, `release__v6.1.x`, `release__v6.0.x`, `release__v5.1.x`, `feature__RSP_SIL`, `feature__RESI_Support`, `feature__EmbLibv7.0.x`, `story__HLR-1436-fusion`, `feature__CUW-5824-Fast-Resim_AF-Integration` | 7.1 GB |
| core-radar-gen7-awr294x | ~240 | 10: `dev`, `release__v16.0.x`, `release__v14.0.x`, `release__v12.0.x`, `feature__emb-lib-linux`, `feature__emb-lib-hdf5`, `feature__emblib_tracker`, `feature__HIL_development_branch`, `feature__variable_cfar_initial_changes`, `feature__Gen7_Downselection` | (see disk) |
| core-resim-logic-model | 11 | 10: `main`, `feature__adcam_main_dev`, `feature__adcam_releases`, `feature__STLA_Small_LM2`, `feature__USS_Aggregator`, `feature__JBC-93__Update_Dspace_Structure_And_Chunk`, `feature__JBC-238`, `feature__JBC-191__Heap_overflow_fix`, `feature__JBC-186__Transmit_mudp_frames_test`, `feature__JBC-138__Enable_Windows_FMU_build` | 26.1 GB |
| Core_RESIM_DC_Emb_Library | 5 | 4: `master`, `feature__DC_FF_Testing`, `feature__DGPS`, `feature__keg_writing` | 14.2 GB |
| Core_RESIM_KPI | 2 | `master` | 1.1 GB |
| Core_RESIM_Bordnet_Tool | 2 | `master` | 0.8 GB |
| Core_RESIM_UDP_Decoder_Library | 2 | `master` | 0.6 GB |
| Core_RESIM_HIL_Engine | 2 | `master` | (LFS-heavy tree) |
| Core_RESIM_HPCC | 2 | `master` (empty repo placeholder) | 0 |
| Core_RESIM_USS_Sensor_Model | 2 | `develop` (empty repo placeholder) | 0 |

Snapshots were produced with `git -c filter.lfs.smudge= … archive` — **LFS pointer files** are preserved as
pointer stubs for a handful of objects (e.g. Gen7 `software/m7/mcal/app/config/SystemModel2.tdb`) because the
JFrog LFS host `jfrog.asux.aptiv.com` is not resolvable from this machine. Everything else is full content.

## A.2 Core_Radar_Gen7_SAF85xx — branch anatomy (280 remote refs)

**Branch-family census:** `feature/*` 255 · `release/*` 19 · `feature-review/*` 3 · `dev` 1 · `temp_branch` 1.

**Materialized branches (head commit / date / subject):**

| Snapshot folder | Ref | Head | Date | Subject |
|---|---|---|---|---|
| `dev` | origin/dev | `851d0856` | 2026-09-16 | EOM-2105 Fix memory and stream population for partner sensor stream |
| `release__v12.0.x` | origin/release/v12.0.x | `b48d1b3a` | 2026-09-10 | HYX-3150 UT coverage and improve CCM for Tkey |
| `release__v11.0.x` | origin/release/v11.0.x | `7b321f35` | 2026-08-28 | EUR-2948, Map fascia compensation flag with NVM variable |
| `release__v10.0.x` | origin/release/v10.0.x | `78c1ad58` | 2026-07-08 | HYX-3000 Baseline number update for R10.0.14 |
| `release__v9.0.x` | origin/release/v9.0.x | `f9205ca3` | 2026-04-15 | EEG-6903 Part Number update for R9.0.6 |
| `release__v8.1.x` | origin/release/v8.1.x | `9f2be4ca` | 2026-01-29 | HYX-2190 Part No Update for F188 for 815 release |
| `feature__RSP_SIL_Integration` | origin/feature/RSP_SIL_Integration | `5014f60a` | 2025-02-10 | CUW-4259: path fix for radar_sw_cfg_h |
| `feature__CDC_SIL` | origin/feature/CDC_SIL | `31a68554` | 2025-03-01 | [CUX-5821] CDC SIL all modules integrated |
| `feature__gen7v2_rdd_sil_dev` | origin/feature/gen7v2_rdd_sil_dev | `7a5e030c` | 2024-07-17 | CUX-4946 RDD SIL Folder Stuct update |
| `feature__EmbLibv11.0.x` | origin/feature/EmbLibv11.0.x | `dbd76d05` | 2026-09-17 | [HLR-1620]:SiL version update |

**What each branch is for (from commit subjects + diff vs `dev`):**

- **`dev`** — integration trunk. Top-level: `software/{m7,bbe32,a53,building_block,common,radar_processing}`, `sil/{emb_lib,rsp_sil}`, `tools/{CI,ITF,lauterbach,coverity,mayhem,…}`, Bazel (`MODULE.bazel`, `bb.MODULE.bazel`), design docs `Design_Doc_RDD_SIL.md` + `Design_Doc_CDC_SIL.md`. Drift vs `release/v12.0.x` = only **90 files / +2538 −651 lines** — release branches stay tight to dev; changed files concentrate on `software/a53/emb_tracker/emb_tracker_wrapper.*`, `software/bbe32/src/ipc_dsp.c`, AUTOSAR `GenData` (Os/Rte/NvM), `software/common/versions/versions.c`.
- **Release lineage v8.1 → v12.0** — five point-in-time production baselines (F188/815, R9.0.x, R10.0.x, R11.0.x, R12.0.x). Each is the SW part-numbered drop for a program; v12 is newest (hygiene commits: UT coverage, Tkey CCM, XCP fault-injection macro default-off, BIST No_run revert, FCCU functional-reset fix).
- **`feature/RSP_SIL_Integration`** — RSP (signal-processing) SiL interface bring-up: `rdd_sil_interface.cpp` / `cdc_sil_interface.cpp` paths fixed for `radar_sw_cfg_h` (CUW-3961/4259), AF SIL updates for R5.0, "Push RDD SIL to dev" (CUX-5593). This is the branch that landed the RDD/CDC SIL APIs referenced throughout §5.
- **`feature/CDC_SIL`** — CUX-5821 "CDC SIL all modules integrated": down-merged `release/v5.2.x`, brought `sil_wrapper` + `sweep_bw` API (GHW-1931), CEER DBC for Gen7v2 (HLR-368), XML-output emblib parity with Gen7v1 (HLR-396), UDP mapping fix (EPB-3588), CAN Rx overrun reporting (DND-3375). Diff vs dev ≈ 4833 files — a full-stack integration branch.
- **`feature/gen7v2_rdd_sil_dev`** — Gen7v2 RDD SIL folder restructure (CUX-4946), IPC component integration (EEG-4162), stream-handler UTs (DNP-5068), RFE look-complete trigger moved RFFT-done→bbe (EUR-735), stream-def codegen script (DNP-5148). The structural ancestor of today's `sil/rsp_sil` layout.
- **`feature/EmbLibv11.0.x`** — live EmLib release line for R11: SiL version bump + CRC/E2E update (HLR-1620), CAN-over-Ethernet DBC v0.91 for STLA_Small (HLR-1525), Ceer DBC 4.0.4v (HLR-1568), CAN non-PLP order (HLR-1558), fascia-comp flag ↔ NVM (EUR-2948). Diff vs dev = **981 files / +2.57M lines** — emblib binaries/generated artifacts dominate.

**High-value branches NOT snapshotted (available on demand — full list in `_BRANCH_CATALOG.txt`):**
`feature/AF_SIL_Integration`, `feature/PSP_Sil_integration`, `feature/CDC4…`/`feature/CDC_*` (`DA_resolution`),
`feature/EmbLibv5.2.x-tracker-fix` … `feature/EmbLibv10.0.x` (full EmbLib version matrix),
`feature/RSP_SIL_Integration` siblings `feature/Adcam_Resim`, `feature/Alignment_*`, `feature/radar_capability`,
all 19 `release/v*.x` (only 5 snapshotted), `feature/SecOC_*`, `feature/XCP_*`, `feature/Tt_*`/`feature/TT_*` (RDU, 3D-FFT, IDM/SIDM), `feature/DND-4228_Fusion_changes` (ROT fusion), `feature/rot_fusion_ref_design`.

## A.3 Core_Radar_Gen8_iND13400 — branch anatomy (553 remote refs)

**Branch-family census:** `feature/*` 265 · `gerrit/*` 148 · `story/*` 56 · `feat/*` 17 · `release/*` 10 · `task/*` 7 · `gh-readonly-queue/*` 5 · `fix/*` 4 · assorted singles (`dev`, PR stubs, `prc8`, cherry-picks) ~31.

**Materialized branches:**

| Snapshot folder | Ref | Head | Date | Subject |
|---|---|---|---|---|
| `dev` | origin/dev | `e6869b04` | 2026-09-17 | EEG-7407 Pcan E2E fault implementation (#875) |
| `release__v7.0.x` | origin/release/v7.0.x | `8e679e45` | 2026-09-17 | Merge PR #1072 IUC-3532-OLP-wrapper-curvature-map |
| `release__v6.1.x` | origin/release/v6.1.x | `fbd6a132` | 2026-09-17 | HLR-1607 - Final fusion changes, CAN, VCAN (#1035) |
| `release__v6.0.x` | origin/release/v6.0.x | `59c4c228` | 2026-09-13 | EAH-8604: sync .github from dev, release smoke test runner var (#1015) |
| `release__v5.1.x` | origin/release/v5.1.x | `bcecfab0` | 2026-06-30 | EPB-4634 High Temperature Fault Changes (SWE_3 R6.1) |
| `feature__RSP_SIL` | origin/feature/RSP_SIL | `5fefd90c` | 2026-04-01 | GHW-3060 - RDD_SIL: Update spbb_cfg.h file |
| `feature__RESI_Support` | origin/feature/RESI_Support | `bb689928` | 2024-10-01 | [GHW-1715] Integrate the SPBB updates for XCP signal outputs |
| `feature__EmbLibv7.0.x` | origin/feature/EmbLibv7.0.x | `983de1ac` | 2026-09-12 | HLR-1615: Symbol update in emblib |
| `story__HLR-1436-fusion` | origin/story/HLR-1436-fusion | `06d65784` | 2026-09-14 | HLR-1436:Fusion changes |
| `feature__CUW-5824-Fast-Resim_AF-Integration` | origin/feature/CUW-5824-Fast-Resim_AF-Integration | `d01138d7` | 2026-09-08 | CUW-5824, AF_FastResim_Integration |

**What each branch is for:**

- **`dev`** — trunk; top-level adds Gen8-only `software/r52` (replaces Gen7 `m7`+`a53`), `sil/{emb_lib,rsp_sil}`, `.mcp/` (MCP server), `docs/`+`mkdocs.yml`, `catalog-info.yaml` (Backstage). Drift vs `release/v7.0.x` = **652 files / +83k −65k** — heavier than Gen7's dev↔release gap because v7 lags active feature merges.
- **Release lineage v5.1 → v7.0** — production drops; v7 head is OLP curvature-mapping merge (IUC-3532) plus GHW-3502 range chirp count check — both **directly touch resim-relevant code**: `software/bbe32/Cust/OLP_Wrapper/`, `sil/rsp_sil/main/rsp_wrapper_interface/{rdd,rdu}_sil_interface/`, MATLAB parser scripts `sil/rsp_sil/parser_script/*.m` (`cdc_rdd_bin_file_generation.m`, `rdu_bin_file_generation.m`, `rdu_create_input_data_SIL.m`), and `sil/emb_lib/sil_source/sil_engine_headers/sil_{input,output}_symbol.h`.
- **`feature/RSP_SIL`** — the Gen8 RSP-SiL feature branch: `PC_RESIL_SIL` macro + `spbb_cfg.h` gating (GHW-3060), "Support SIL activity for RSP_SIL feature branch" (GHW-2901), **Dynamic Alignment commented out in RDD SIL mode** (EUR-2031 — a known SiL simplification with F8 implications), AF Fascia Compensation integration (ICH-119), SMC v25 (CUW-5103), FLR8 V2.2 "Tiger" integration. Note: `git diff origin/dev origin/feature/RSP_SIL` shows large deletions because the branch predates several dev restructures — treat as historical + patch source, not a drop-in tree.
- **`feature/RESI_Support`** — RESI (Radar External Sensor Interface?) / SPBB XCP signal-output integration (GHW-1711/1715), test DBC integration across can/canif/com/comM/Rte (DND-2468), SYSMEM0_DSP_CODE split (CUW-3077). Diff vs dev = 3168 files — heavy generated-code surface.
- **`feature/EmbLibv7.0.x`** — live Gen8 EmLib line tied to `release/v7.0.x`: symbol update (HLR-1615), ETH_PHYRSTB pin (GNZ-2438), bootloader/quickflash update (DDR-5231), host buffer FC disable + blockage/alignment in partner stream (EOM-2066). Diff vs dev = 256 files / +20k −67k — a *downsizing* diff (branch trimmed dev-only scaffolding).
- **`story/HLR-1436-fusion`** — fusion workstream on top of `release/v6.1.x`: tracker scan-index increment fix (HLR-1606 — **directly relevant to F10 scan-continuity KPI**), header-stream dependency removal, RTE update for Alignment M2D port (HYX-3156). Changed dirs include `sil/emb_lib/{fusion,tracker_emb_lib,someip,timer}` — the SiL-visible fusion surface.
- **`feature/CUW-5824-Fast-Resim_AF-Integration`** — **fast-resim**: adds `sil/fast_resim/{angle_finding_fast_resim_interface.cpp/.hpp, fast_resim_af_data.c/.h, BUILD, deps, inc, src, utils}` — a specialized AF path that skips full pipeline stages for quicker KPI loops. Ten-patch series "creating-BUILD-options-for-fast-resim" p2→p6 + build command mode. Highest-leverage resim branch on Gen8; pairs with §5 harness.

**High-value branches NOT snapshotted:** `feature/RSP_SIL_3.1.119`, `feature/HLR-1423-sil-wrapper-virtual-vehicle-streams`, `feature/HLR-1424`, `feature/EmbLib_Tiger2`/`EmbLib_FF`, `feature/F360_Integration`/`F360_Tracker_v1`, `feature/TT_RDU`/`TT_RDU_R6`, `feature/AF_ML_downselection`, `feature/AL_FeatureFunction`, `feature/AL_small_classical`, `story/HLR-1626` (HIL fixes for VSE and RDD), `story/DNP-7666-Align-timestamp-for-tracker` (**F3/F11 timestamp alignment**), `story/CUX-7452_RDU_UT`, `story/CUX-7460-rdu_buf_mem_opt`, all 148 `gerrit/change-*` refs, 10 `release/*` (4 snapshotted), `feat/DDR-*`, `feature/GOC-3712-my-change2` (PSP SIL update).

## A.4 core-radar-gen7-awr294x — NEW REPO (Gen7v1 AWR294x satellite, ~240 remote refs)

**What it is:** the **TI AWR294x (Gen7v1)** satellite/front-radar firmware — predecessor/parallel line to SAF85xx Gen7v2. Bazel `WORKSPACE`-era layout (not the newer `MODULE.bazel` style). Top-level: `software/{FBL,app,boot,build}` (bootloader + flash loader + application — no `sil/` dir on dev), `coreradar_itv` (ITV framework), `instrumentation`, `documentation`, `toolchains`, `tools`, `repo_init.py`, `RULESETS.md`. Head of `dev`: `86e007b9` 2026-08-28 *EPB-4907 data qualifier is not setting as expected*.

**Materialized branches:**

| Snapshot folder | Ref | Head | Date | Subject | Resim relevance |
|---|---|---|---|---|---|
| `dev` | origin/dev | `86e007b9` | 2026-08-28 | EPB-4907 data qualifier is not setting as expected | trunk |
| `release__v16.0.x` | origin/release/v16.0.x | `7b656aae` | 2026-06-09 | DDR-4865 : Fix dss.ld issue for standalone build | newest standalone pkg |
| `release__v14.0.x` | origin/release/v14.0.x | `cfc9abf2` | 2025-12-09 | DDR-4446: Release update standalone package R14.0.150 | R14 standalone |
| `release__v12.0.x` | origin/release/v12.0.x | `01c2b746` | 2025-08-28 | DND-3748 Merge someip fixes and CPU optimizations | SOME/IP fix line |
| `feature__emb-lib-linux` | origin/feature/emb-lib-linux | `d2226734` | 2024-09-16 | [HLR-172] gcc glibc toolchain | **linux build of emblib → SiL enabler** (make build on linux, gcc toolchain, bazeliskrc cleanup) |
| `feature__emb-lib-hdf5` | origin/feature/emb-lib-hdf5 | `8070d2dd` | 2024-10-22 | [HLR-212] wip | **emblib→HDF5 output path** (ancestor of KPI h5 pipeline) |
| `feature__emblib_tracker` | origin/feature/emblib_tracker | `20afd206` | 2024-08-27 | [HLR-134] fix dynamic exe | tracker-in-emblib integration (wip tracker integration, building_block BUILD fix) |
| `feature__HIL_development_branch` | origin/feature/HIL_development_branch | `85fe7e9b` | 2025-01-10 | EUR-999: sync latest tracker changes && HIL stream bypass | **HIL stream bypass + stop command**, IPC memory fix, standalone build |
| `feature__variable_cfar_initial_changes` | origin/feature/variable_cfar_initial_changes | `853c0150` | 2023-02-28 | [GHW-94] Update the CFAR BB to implement variable CFAR LUT | **variable-CFAR LUT — direct prior art / baseline for IDF-1** |
| `feature__Gen7_Downselection` | origin/feature/Gen7_Downselection | `3ab1d21e` | 2023-10-09 | CUW-2894 : Merging dev updates to feature branch | down-selection building block (AF PULSE demo lineage) |

**Why it matters to Resim:** (1) `variable_cfar_initial_changes` is the in-house precedent for CFAR threshold adaptation — cite it as prior art in IDF-1 §Prior-art gap and distinguish (LUT precompute vs runtime slope-mapped R0). (2) `emb-lib-linux` + `emb-lib-hdf5` are the historical path that made EmLib buildable/runnable off-target with HDF5 capture — i.e. the *origin* of the SiL+KPI loop now living in Gen7v2 `sil/emb_lib/hdf5` and `Core_RESIM_KPI`. (3) `HIL_development_branch` "HIL stream bypass" is a known bypass mode — check F10 interaction (bypassed streams vs KPI continuity).

**Branch census highlights (not snapshotted):** `release/v*.x` 31 lines (v2.0.x_ECO … v16.0.x + DRA_19.9.2 + R1.0 + v9.1.x etc.), `feature/*` ~140 (BIST/PBIST/LPBIST, HSM, STC, MNR_Blockage, DRA integration, Inter-PRP-TimeSynch v2–v6, gen7-pcresim-flag, emb-lib-strip-calib, variable_cfar, CDC packing/stream, SMC v20/v23/v48, BYD v9.1.x lines, `feature-review/*` 6 incl. `smc_v23_integration`, `spbb_integration`, `v9.1.x_ddma`, `CUX-3104_interference_detection` — **interference detection prior art for F9**).

## A.5 core-resim-logic-model — NEW REPO (LM2 logic model / FMU, 11 remote refs)

**What it is:** the **Logic Model 2 (LM2)** — a dSPACE/FMU-packaged vehicle-side logic model that sits *upstream* of the radar SiL: it converts customer logs (BMW High/Low/MID, Scania, TML SRR5, Motional SRR3, STLA Small IFV600, DSPACE common) into the interfaces the Resim harness consumes (MDF4, SOME/IP, MUDP, OSI), or stubs/feeds radar ECU outputs back for closed-loop dSPACE runs. Ships as `.fmu` + `.dll` under `LM2_Packaging/Deliverables/` (e.g. `SRR_Master_LogicModel2_GPO_Gen8.fmu`, `SRR_Master_LogicModel2.dll`, `LM2_USS_AGG_FMU_CEER.fmu`, `Interface_Main.dll`, `Mdf4Lib_x64.dll`).

**Top-level tree (`main`):** `Code/{Common_Headers,Customer,Generic,libs,UDP_logging_headers,Interface_Output_Control.xml}`, `Documentation/` (Logic Model Guide, Dspace.docx, AS_DSPACE Block Diagram, Doxygen), `LM2_Packaging/{Build.bat,Build.sh,Build_All.*,Deliverables,LM2_FMU,LM2_Interface}`, `Tools/{Simpler_Code_For_MUDP_TX,TestInterface,XML_Comparator}`, `RULESETS.md`, `ADMIN_AUDIT_LOG.md` (7 GitHub rulesets incl. Codex Validation requiring `.github/workflows/codex-validation.yml`).

**Key code:**

- `Code/Generic/LM2_Interface.{h,cpp}` — core interface singleton; `Generic_Interface.{h,cpp}` glue; `MUDP_Serializer.{h,cpp}`; `CAN_INTF.{hpp,cpp}`; `UdpSocket.{hpp,cpp}`; `XML_Reader.{h,cpp}`; `TimingInfo.{h,cpp}`; `Version.{hpp,cpp}`; `Resim_Config.h`; headers `sil_ecu_input_ext.h`, `radar_ecu_CORE3.h`, `dvl_message.h`, `tracker_output_interface.h`, `some_ip_{input_RX,output}.h`, `RR_ADAS_SYMBOL_{Data,Enum}.h`, `SRR3_SIL_Output_Data.h`.
- `Code/Generic/Interface_Output_Control.xml` — runtime mode switch (XML_Version 2.0): `Customer_Name` ∈ {BMW_LOW, BMW_MID, BMW_HIGH}; `RUN_MODE` ∈ {`E_LIME_SIL_FRAMEWORK`, `E_LIME_HIL_FRAMEWORK`, `E_READ_VEHICLE_LOG_N_TRANSMIT`, `E_READ_DSPACE_LOG_N_TRANSMIT`}; `DEBUG_MODE` ∈ {`E_DEBUG_NONE`, `E_DEBUG_XML_WRITER`, `E_DEBUG_ORCAS_SCENE_WRITER`, `E_DEBUG_DSPACE_WRITER`}; `Debug_File_Type` ∈ {`E_DEBUG_DVSU`, `E_DEBUG_MDF4`}.
- `Code/Customer/*_Log_Convertor.{cpp,h}` — per-customer decoders: `BMW_{High,Low,MID}_Log_Convertor`, `Scania_Log_Convertor`, `TML_SRR5_Log_Convertor`, `Motional_Srr3_SIL_Log_Convertor`, `DSPACE_Common_Log_Converter`, `Checksum`, plus (on feature branches) `STLA_Small_IFV600_Converter.{cpp,h}` + `Stla_Small_IFV600_Struct.h`.
- `LM2_Packaging/LM2_FMU/` — `Build.py`, `BuildForAllCustomer.py`, `CMakeLists.txt`, `prepare_fmu.py`, `UpdateModelDescription.py`, `modelDescription.{xml,in.xml}` (FMI standard), `index.txt`.
- `Code/Common_Headers/` — `SIL_Library.h`, `customer_ID_config.h`, `MDF_Include/`, `helper/`, `udp_headers/`.

**Materialized branches (10 of 11):**

| Snapshot folder | Ref | Head | Date | Subject | What it changes (diff vs main) |
|---|---|---|---|---|---|
| `main` | origin/main | `7715600a` | 2026-09-22 | docs(audit): update log and access summary for manage_team_access | trunk |
| `feature__adcam_main_dev` | origin/feature/adcam_main_dev | `c611013b` | 2026-09-02 | Merge branch 'feature/JBC-138/Enable_Windows_FMU_build' into feature/adcam_main_dev | **323 files / +15k −10k**: ADCAM dev line — STLA_Small IFV600 converter, Windows FMU build merge, `build_and_deploy.ps1/.sh`, `Build_FMUs.sh`, `find_vs.py`, deliverable FMU/DLL refresh |
| `feature__adcam_releases` | origin/feature/adcam_releases | `a2e2446` | (recent) | Prepare Windows FMU for v1p2 release | release cut of adcam_main_dev (v1p2 FMU packaging) |
| `feature__STLA_Small_LM2` | origin/feature/STLA_Small_LM2 | `d04b8120` | 2026-09-02 | JBC-212: Updated the autogen files using the latest autogen tool | **191 files / +40k −5k**: adds `STLA_Small_LM2/` subtree (CustomFunctions/FmuEthBridge, Development/fmu, Documentation/{BUILD,DESIGN}.md, README); FMU v1.0.0 w/ emblib v3.2.6.23 (JBC-116); CanTx (JBC-118/119); Windows debug build + SIL lib loading (JBC_172) |
| `feature__USS_Aggregator` | origin/feature/USS_Aggregator | `273dd951` | 2026-02-13 | Updated the Aggregator logic | **456 files / +21.5k −1.9k**: USS (ultrasonic) **Aggregator** LM2 — `LM2_USS_AGG_FMU_CEER.fmu`, `LM2_USS_AGG_FMU.dll`, `USS_AGG_FMU_CEER.fmu` deliverables; touches Generic_Interface/LM2_Interface/XML_Reader + Deliverables. Connects the empty `Core_RESIM_USS_Sensor_Model` placeholder to a real implementation branch here |
| `feature__JBC-93__Update_Dspace_Structure_And_Chunk` | origin/feature/JBC-93/… | `9d1a6ea`+ | (2025) | Chunking implementation for dSPACE STLA structures; Core Protocol input for camera LM2 data | dSPACE structure chunking, CP stream wiring, VCS→detection transform fix, vehicle dimension fix, polarity/rear-axle calc (JBC-139), OSI conversions for other customers |
| `feature__JBC-238` | origin/feature/JBC-238 | `2712f3d` | (2025) | Added the tool to convert input vcs co-ordinates to osi | `Tools/` VCS→OSI coordinate converter + STLA_Small stack (same lineage as JBC-93) |
| `feature__JBC-191__Heap_overflow_fix` | origin/feature/JBC-191/Heap_overflow_fix | `ca67083` | (2025) | Fixes related to Xml reader, build scripts w/ asan flags, execution passes initial XML reader issue | **heap overflow hardening** + ASan build flags — security/robustness of XML_Reader path |
| `feature__JBC-186__Transmit_mudp_frames_test` | origin/feature/JBC-186/… | `493eac9` | (2025) | changes to adapt output transmission through OSMP pointer | MUDP TX over dSPACE **OSMP** pointer (dSPACE real-time IPC) |
| `feature__JBC-138__Enable_Windows_FMU_build` | origin/feature/JBC-138/… | `07aaf69` | (2025) | Added logic to handle missing Linux Emb sil while building Windows FMU | Windows FMU build: LoadLibraryExA DLL loading, MSVC path detection (`find_vs.py`), std namespace fix, FMU includes emblib + SIL engine, generic Sil engine config paths, stack-size/polarity fixes (JBC-139) |

Only non-materialized ref: the 11th (`feature/adcam_releases` was snapshotted; if a ref was added between census and run, see `_BRANCH_CATALOG.txt`).

## A.6 Core_RESIM_DC_Emb_Library — branch anatomy (5 refs)

Top-level: `Application/{F360Tracker/{F360TrackerLib,olp,OLP_Core,rspp,VSE_Core,ocg,sg_stationary_geometry,Wrappers}, FeatureFunctions/{CTA,CED,ESA,LCDA,PT,Calibration_Tool,Math_Library,Feature_Building_Kit,SFL_Interface,Mock_Files}, UDP_Logging, utilities/cmake}`, `DC_SIL/…`.

| Snapshot | Ref | Head | Date | Subject | Diff vs master |
|---|---|---|---|---|---|
| `master` | origin/master | `a06efdb2` | 2026-09-09 | DET-2211: BuildAll script and other changes | trunk (DET-2198 jFrog binary fetch for Docker, DET-2194 max_dets SRR/MRR, DET-2184 R10 emblibs) |
| `feature__DC_FF_Testing` | origin/feature/DC_FF_Testing | `292fe03b` | 2025-09-15 | MRR changes for SG | **4503 files / +1.40M −319k**: feature-function test line — MRR stationary-geometry (SG), singularity XTRK gen fix, tracker 10.28.1, HDF scan-index update, tracker-input timestamp change, HLR-930 DC input data-quality check (feeds `DataQualityChk` in §5), terminal-print disable config |
| `feature__DGPS` | origin/feature/DGPS | `d1871ad9` | 2026-04-13 | DGPS initial integration | **763 files / +42k −106k**: adds `DC_SIL/sil_dgps/{DGPS_Decoder.cpp,.h}`, `DC_SIL/sil_ext_libs/DGPS`, Gen8 fw_dlls dir, Event_Logger/HDF_Trace updates — **ground-truth DGPS input path for KPI** (IPS has `DGPSFilter/`) |
| `feature__keg_writing` | origin/feature/keg_writing | `2e5dfb1e` | 2026-01-19 | dgps osi changes | **4302 files / +969k −132k**: DGPS→**OSI** text-file export (deterministic open-simulation interface for external scenario tools), DGPS-Ethernet init, DET-2023 Keg writing |

**Unmaterialized:** none (5th ref is `origin/HEAD` same as master).

## A.7 Single-branch repos (master/develop = only meaningful ref)

| Repo | Head | Date | Subject | Tree facts worth citing |
|---|---|---|---|---|
| `Core_RESIM_KPI` | `0fce967e` | 2026-09-17 | DET-2218 : Added HDF Validation checks in DC HTML | Full UDP KPI script set confirmed on `master`: `gen7v2_resim_kpi_scripts/{detection,tracker,tracker_processed_det,tracker_info,tracker_vehicle_info,alignment,interferenceDetection,downSelection,radarCapability}_matching_kpi_script.py`, `config.py`, `constants.py`, `file_handling.py`, `logger.py`, `meta_data.{py,json}`, `run_kpi_script.{py,bat,sh}`, `log_path.txt`; `can_kpi_hdf/{a_persistence_layer,b_data_storage,c_business_layer,d_presentation_layer,kpi_main.py,kpi.json,kpi_smoke.json,can_kpi.spec,can_singularity_KPI.def}`; `can_kpi_scripts/`; `main_html/`; `profile_timing_plotter/`; `IPS/` report engine (`ResimHTMLReport.py`, `DGPSFilter/`, `DashManager/`, `DataCollect/`, `DataPrep/`, `DataStore/`, `EventMan/`, `HTMLConfig.xml`, `Inputs.json`, `input_{gt_vs_tracker,tracker_vs_tracker}.xml`, `plots_gen.py`, `RepGen/`, `PlotConvert/`, `Sig_Prep/`, `Metadata/`, `Utilities/`); root `requirements.txt`, `ResimHTMLReport.def`, `build_simg.sh`, `HTML_docker_Doc.docx`. |
| `Core_RESIM_Bordnet_Tool` | `ad1cb4a0` | 2026-09-08 | AL PCAN FLR HDF CHANGES | 40+ decoder/extractor projects on `master`: `AL_PCAN_{Extractor,FLR,FL,RL}_Decoder`, `AL_VCAN_*`, `CEER_PCAN_{Extractor,FLR,FL,FR,RL,RR}_Decoder`, `CEER_SOMEIP_{FL,FLR,FR,RL,RR,SRR7P}_Decoder` + `CEER_SOMEIP_{Extractor,Wrapper}`, `MCIP_PCAN_*`, `BordNetDecoder/`, `Auto_Gen_Files/{CMake,Include,Source}`, `Common_Headers/`, `Common_Structures/`, `Cross_Platform/`, `BOOST/`, `mdf_log/`, `Utility/`, `Document/` (BordnetSDD, Design-RESIM-BordnetDll, AUTOSAR SOME/IP PRS/SWS PDFs, ThunderGateway.pptx), `Build.{bat,sh}`, `CMake/`. |
| `Core_RESIM_UDP_Decoder_Library` | `d67985b4` | 2026-09-16 | Static Alignment Stream Addition | `GEN7/{GPO_V1,GPO_V2,RNA_GEN7}`, `GEN8/GPO`, `GEN6P_DSPACE`, `BMW_SP25`, `HDF/`, `Common/`, `CommonFiles/`, `Utils/`, `Decoder_DLL_Release_Notes.xml`, `ReadMe.txt`, `GEN7_RADAR_UDP_DESIGN_DOCUMENT.xlsx`. Latest commit adds **static-alignment stream** decode — new `_DYNAMIC_ALIGNMENT_STREAM`-adjacent channel for F8 KPI. |
| `Core_RESIM_HIL_Engine` | `8364689a` | 2026-08-06 | Build Script changes. | `ApplicationProjects/{SRR3_Resimulator,DPH_RR_ADAS_{Config,HIL,LOGGER},CAN_MF4_Logger,MDF4_Decoder,BOOST,BOOST_VS15,BOOST_VS19,output}`, `CoreLibraryProjects/{CCA_ViGEM,CCA_Vpcap,MUDP_Serializer,SRR3_Internal_Data_Logger,UDP_Transmitter,XCP_SeedKey{,_Lib},dvlFile,dvlMessage,mdf_log,mdf_log_convertor,mudp_{decoder,decoder_calib,log,receiver},ptp_to_xml_generator,radar_stream_decoder,excel_utils,CrossPlatform}`, `Common/{IRadarStream.h,IRadarStream_Lib_Loader.h,Z_Logging.h}`, `Build_Project.sh`, `HILEngine_BUILD_README.doc`. Confirms PTP→XML generator (time-sync artifacts for F11) and dual ViGEm capture libs (CCA_ViGEM/CCA_Vpcap). |
| `Core_RESIM_HPCC` | `1b06ee03` | 2025-03-03 | Initial empty repository | Placeholder for High-Performance Compute Cluster orchestration — no code; roadmap risk/opportunity (§B of roadmap). |
| `Core_RESIM_USS_Sensor_Model` | `7094b1ae` | 2025-10-15 | Initial empty repository (branch `develop`) | Placeholder for ultrasonic sensor model — actual USS LM2 work lives on `core-resim-logic-model/feature/USS_Aggregator` (A.5). |

## A.8 Cross-branch architectural facts (what changes *between* branches that Resim cares about)

1. **SiL interface stability:** Gen7 `rdd_sil_interface.cpp` / `cdc_sil_interface.cpp` and Gen8 `rdd_sil_interface` / `rdu_sil_interface` / `cdc_*` exist on dev **and** every snapshotted release/SiL branch — the `Rdd_To_Detection_Configuration` / `CDC_To_Detection_Configuration` / `Rdu_To_Detection_Configuration` contract is release-stable; MATLAB parser scripts (`sil/rsp_sil/parser_script/*.m`) regenerate bin fixtures per release.
2. **Fascia compensation + NVM mapping** first appears on Gen7 `release/v11.0.x` (EUR-2948) and Gen8 `feature/RSP_SIL` (ICH-119) — F7 calibration surface differs by release; KPI alignment scripts must be version-aware.
3. **Dynamic-alignment-disabled mode** exists only on Gen8 `feature/RSP_SIL` (EUR-2031) — explains any alignment-KPI "N/A" results on RSP_SIL builds.
4. **Scan-index semantics** fixed on `story/HLR-1436-fusion` (HLR-1606 tracker scan index increment + header-stream deps removed) — CAN KPI continuity results are only comparable across builds **after** this fix merges to the branch under test.
5. **Fast-resim** exists only on Gen8 `feature/CUW-5824…` — `sil/fast_resim/` is opt-in; baseline full-pipeline resim remains `sil/rsp_sil` + `sil/emb_lib`.
6. **DGPS ground truth** only on `Core_RESIM_DC_Emb_Library/feature/DGPS` (+ OSI export on `feature/keg_writing`) — KPI "DGPS" mode in `IPS/DGPSFilter` requires this branch's binaries.
7. **Windows-vs-Linux FMU split** on `core-resim-logic-model` (`feature/JBC-138…` Windows, `feature/adcam_main_dev` merges both, `feature/JBC-191…` ASan) — LM2 packaging choice per customer.
8. **LFS note:** Gen7 release branches contain at least one LFS-hosted artifact (`SystemModel2.tdb`); snapshots store the pointer. Resolve with `git lfs pull` when `jfrog.asux.aptiv.com` is reachable.
9. **Branch-count as process signal:** Gen8's 148 `gerrit/change-*` + 5 `gh-readonly-queue` refs are transient review refs — they inflate the 553 count; durable development surface ≈ 265 feature + 56 story + 10 release.
10. **Empty repos are intentional:** HPCC (cluster scheduling) and USS_Sensor_Model (parked in favor of LM2 USS_Aggregator) mark the two planned-but-unfunded platform expansions — tracked in the roadmap §B.

---

*Appendix A generated from git metadata + tree/diff inspection 2026-09-23. Full branch lists: `_branch_snapshots/<repo>/_BRANCH_CATALOG.txt`.*

---

# APPENDIX C — FILE-LEVEL ATLAS (audited 2026-09-24, all refs verified via `git show`/`ls-tree`)

> Companion to Appendix A (branch manifest). Every path below was read from the named ref — no guessing.
> Notation: `G7@dev:path` = file in `Core_Radar_Gen7_SAF85xx@origin/dev`, `G8@`, `AWR@`, `LM2@`, `KPI@`, `DC@`, `HIL@`, `BN@` (Bordnet), `UD@` (UDP decoder) likewise.

## C.1 Build & dependency skeleton (what pulls what)

**Git submodules (external repos — resim needs these for a from-scratch build):**

| Owner | Submodule path | URL | Branch |
|---|---|---|---|
| G7@dev | `software/m7/autosar/sip` | `GPO/core-radar-gen7-awr294x-autosar-sip.git` | (default) |
| G7@dev | `tools/ITF/ExecutableSpecs/ADVRADAR_Gen7_Exec_Spec` | `GPO/core-radar-gen7-exec-spec.git` | (default) |
| G7@dev | `tools/python/Core_Radar_Gen7_SAF85xx_Python_Framework` | `GPO/core-radar-gen7-saf85xx-python-framework.git` | `dev`, ignore=all |
| G8@dev | `software/r52/autosar/sip` | `GPO/core-radar-gen8-ind13400-sip.git` | (default) |
| G8@dev | `tools/python/testing_framework/Core_Radar_Python_Framework` | `GPO/core-radar-python-framework.git` | `dev`, ignore=all |
| G8@dev | `tools/ITF/ExecutableSpecs/ADVRADAR_Gen7_Exec_Spec` | `GPO/core-radar-gen7-exec-spec.git` | `dev` |
| G8@dev | `tools/ITF/Mex_Executable_Specs/Core_Radar_Gen8_iND13400_Matlab` | `GPO/core-radar-gen8-ind13400-matlab.git` | `dev` |

**Bazel (G7@dev `MODULE.bazel`, module `gen7_saf85xx` version `dev`):** `platforms 1.0.0`, `bazel_features 1.33.0`, `rules_pkg 1.0.1`, `com_google_googletest 1.11.0`, `rules_cc 0.2.16`, `bazel_skylib 1.9.0`, `rules_fuzzing 0.6.0`, `rules_mayhem 0.8.4` (+mayhem/yq CLI linux+windows), `fff 7e09f07`, `rapid_build_platform 2.1.1`, `rules_python 1.4.1` (single_version_override), Coverity 2023 extension (`//:extensions.bzl`). Includes `//:bb.MODULE.bazel`. AWR294x@dev is older-style: `WORKSPACE` + `bb.bzl`, no `MODULE.bazel` at root.
**Gen8 extras:** `.mcp/` (MCP server + config), `mkdocs.yml` + `docs/`, `catalog-info.yaml` (Backstage), `module_filename_map.json` under `software/`.

## C.2 Embedded source map — compilation units that matter to Resim

**Gen7 DSS kernels (`G7@dev:software/bbe32/src/`):** `range_proc.c` (Range FFT), `doppler_proc.c` (Doppler FFT + RDD), `cdc_packing.c` + `cdc_if.c` (CDC 2K/5K pack), `ipc_dsp.c` (DSS side of M2D/D2M), `sweep_bw_if.c`, `sp_if.c`, `fp_if.c` (fixed-point interfaces), `mpu.c`, `interrupts.c`, `exceptions.c`, `merr_uncorrectable.S`, `fusa_bbe_self_test.c`, `ecc_test`, `main_application.c`. Support dirs: `bb_cfg/`, `bb_include/`, `bbe_dispatcher/`, `bbe_fault_injector/`, `static_register_validation/`, `inc/`, `stack/`, `test/`, `integration_test/`.
**Gen8 DSS (`G8@dev:software/bbe32/src/`):** `range_process_ifc.c`, `doppler_process_ifc.c`, `anglefinding_project_interface.c` (moved in-tree vs G7 building_block), `cdc_packing.c`, `ipc_dsp.c`, `mipi_ifc.c` (front-end ingest), `performance_monitor_ifc.c`, `get_r52_time_from_bbe32.c` (cross-core time — F11 evidence), `main_application.c`, `test/`. Plus first-class dirs absent on Gen7: `rdd_proc/`, `rdu/{inc,src,test}/`, `dyn_alignment/{dyn_align_wrapper.c,.h,DA.md}`, `interference_detection/{interference_detection_algo_interface.c,.h}` (F9 owner), `static_alignment/`, `emb_tracker/{emb_tracker_wrapper.cpp,.h,emb_tracker_internal.h,emb_tracker_project_parameters.h,emb_tracker_stubs.cpp,.h,emb_tracker_build_flags.bzl,emb_tracker_project_profile.bzl}`, `capability/`, `adc_logging/`, `TOI_char_quality_determination/`, `Ra_fault_injection/`, `dss_ecc_parity/`, `mpu/`, `Cust/` (customer wrappers incl. OLP/SFL/MOIS/BSIS).
**Gen7 MSS (`software/m7/`):** `main.c`, `autosar/` (+`sip` submodule), `calib_cfg/`, `drivers/`, `dsp_setup/`, `integration_test/`, `ipc/{inc,src,test}/`, `mcal/`, `mmic/`, `program_flow_monitor/`, `stack/`, `test/`, `Hse_fw/`.
**Gen8 MSS (`software/r52/`):** `main.c`, `startup/`, `autosar/` (+`sip`), `bb_radar_ctl/`, `drivers/`, `dsp_setup/`, `integration_test/`, `ipc/{inc,src,test}/`, `mmic/`, `program_flow_monitor/`, `r52_stack/`.
**Gen7 A53 (`software/a53/`):** `main.c`, `emb_tracker/`, `dyn_alignment/`, `capability/`, `TOI_char_quality_determination/`, `id/`, `ipc/`, `inc/`, `src/`, `stack/`, `test/`, `integration_test/`, `a53_fault_injector/`, `a53_scst/`. (Gen8 folds tracker into `software/bbe32/emb_tracker/` — A53 island disappears; SiL impact: tracker runs in different core model.)
**Shared contracts (`G7@dev:software/common/`):** `rdd_stream.h`, `detection_stream.h`, `detection_debug_stream.h`, `down_selection_stream.h`, `cdc_stream.h`, `cdc_frame_interface.h`, `header_stream.h`, `status_stream.h`, `debug_stream.h`, `calib_stream.h`, `mmic_stream.h`, `adc_stream.h`, `toi_stream.h`, `vse_stream.h`, `partner_sensor_stream.h`, `stream_header.h`, `radar_look_types.h`, `ipc/{ipc_commands.h,ipc_data.c,ipc_data.h,IPC_UT/}`, `versions/{versions.c,versions.h,linkstamp}`, `linker/`, `calibrations/`, `crc_calc/`, `app_chksum/`, `mmic/`, `utilities/`.
**Gen7 AF building block (`software/building_block/anglefinding/`):** `anglefinding_project_interface.c`, `angle_finding_project_interface.h`, `angle_finding_module_cfg.h`, `test/`. (`hil_process/` + `common/` siblings; Gen8 has `sh_cfg/` instead — stream-handler config split.)
**AWR294x Gen7v1 (`AWR@dev:software/app/`):** `mss/{autosar,bb_radar_ctl,drivers,kernel,mcal,mcal_awr2x44p_eco,mmic,mss_stack,program_flow_monitor,sensor_pos,src,ti_startup}`, `dss/{drivers,src,sys_config}`, `rss/`, `emb_lib/` (full SiL-capable subtree: `Emb_Lib_Config.xml`, `building_block/`, `ff_emb_lib/`, `hdf5/`, `kpi/`, `can/`, `boost_pfr/`, `build/`), `building_block/`, `common/`. `coreradar_itv/{R3.0,R4.0}/`.

**Key struct evidence (real fields):**
- `G7:software/common/rdd_stream.h` — `Look_Data_T` (28 B: `start/end/mid_dwell_{nanosec,sec}`, `look_id`, `scan_type`, `dwell_type`, `scan_index`, `look_index`, `rest_count`); `RDD_Data_T` (96 B + per-bin arrays: `bwdep_cr_resp/sensitivity_thold/hvc_thold/mb_thold[MAX_RANGE_BINS]`, `rdd2_thold_{2nd_pass,1p5_pass,idm_artifact_mask}`, `cfar_corr_coeff[]`, `cfar_nf_est[]`, `cfar_thold[]`, `rdd1_max_per_range_bin[]`, `rdd1_signal_thold_first_pass[]`, `rdd1_max_detectable_range`, `rdd_time/rdd_time_max`); fp-targets saturation struct; `Exec_Spec_Version_T` (`sha1_id`, `major_version`).
- `UD:GEN8/GPO/SRR8/DETECTION/V200/detection_stream.h` — `Stream_Hdr_T` (`version`, `checksum`, `scan_index`, `error_info`); `AF_Str_Detection_T` (100 B: `ran,vel,pow,snr,theta,phi,rcs,spec_res` float32 + `rdd_idx` int16); `AF_Str_Det_List_Property_T` (`lookindex,scanindex,look_type,range_coverage,doppler_coverage,num_fp_detections,num_sp_detections,slope_correction_status,timestamp_status`); `AF_Diagnostics_T` (16 B: `af_timeout_count`, `af_saturation_flag`, `af_timeout_flag`, `af_smc_mismatch_flag`, `af_usc_mismatch_flag`); `Ch_Ch_Degradation_T` (48 B per-Tx/Rx conf). Empty `Detection_Stream_T` tag (versioned union pattern).

## C.3 SiL harness file map (what you actually build/run)

**`sil/emb_lib/` (both gens; G8 listing):** `Emb_Lib_Config.xml` (v7.5), `BUILD`, `emblib.MODULE.bazel`, `README.md`, `sil_source/`, `rsp_emb_lib/`, `tracker_emb_lib/{tracker_sil_wrapper.cpp,.h}`, `ff_emb_lib/{customer,olp,sfl}/`, `vse/{vse_wrapper.cpp,.h}`, `streams/{cdc_stream_combined.h}`, `udp/`, `someip/`, `can/`, `timer/`, `plp_sync/`, `hdf5/{highfive.archive.BUILD,hdf5.archive.BUILD,linux/,mingw/,test.cpp}` (HighFive header-only HDF5), `kpi/{Detection_Matching.ipynb}` (notebook KPI prototype!), `mdf_lib/`, `mdf` output, `docs/`, `profile_timing/`, `boost_pfr/`, `build_bin/`, `toolchains/`.
**`Emb_Lib_Config.xml` (G8@dev, XML v7.5) switch inventory:** `STATISTICAL_REPORT=0`, `CDC_SATURATION_CHECK=0`, `CAN_TRANSMISSION=CAN_TX_DISABLED`, `Veh_Data_CSV_Path=NONE`, `TRACKER_INTERNALS=ENABLED`, `ALIGNMENT_INTERNALS=ENABLED`, `PSP_MODULES=ENABLED`, `UDP_TRANSMISSION=UDP_TX_DISABLED`, `XML_Trace_Files=DISABLED`, `LOAD_TRACKER_INTERNALS=ENABLE_TRACKER_MODE_INIT`, `LOAD_ANERMA_ERROR=DISABLED`, `USC_XML_Control=USC_NORMAL` + `Suppress_Usc_{Csv,Xml}_log=DISABLE`, `SMC_XML_Control=SMC_NORMAL` + `Suppress_Smc_{Csv,Xml}_log=DISABLE`, `USC_Calibration_Source=LOG`, `SMC/PSC_Calibration_Source=DEFAULT`, `PLP_Resim/{plp_algorithms=0,plp_analysis=0}`, `Enable_Console_Prints=1`, `Output_Symbol_Source=1`, `XML_Prints/*` (Vehicle_Info, Mounting_Info, MNR/TD_blockage, Self/Opp_Dets, scan_prints, Cal_Error_Files, Active_Faults, UDP_Datas, Curvi_Tracks, Alignment_Info, PATH_Tracking_Info, LCDA/CTA/RECW/ASW/ABA_Info, CDC_Errors_Info, IPC_Error_Infor, PCAN_Error_Info, ENET_Frame_Error_Info). **Every one of these is a legitimate experiment knob** — e.g. `Veh_Data_CSV_Path` enables DET-only resim; `LOAD_TRACKER_INTERNALS` seeds tracker state (IDF-2/IDF-4 A/B control).
**`sil/rsp_sil/` (both gens):** `main/{building_block,rsp_wrapper_interface,sil_wrapper_interface}/`, `parser_script/` (MATLAB `.m` bin-fixture generators incl. `rdu_*`, `cdc_rdd_bin_file_generation.m`, `manual_parser_script.m`), `data_bin/`. Gen8 RSP_SIL branch adds `rdu_sil_interface/{rdu_sil_api.hpp,rdu_sil_interface.cpp,rdu_sil_design.md,test/}` + `parser_script` RDU scripts.
**`sil/fast_resim/` (G8 `feature/CUW-5824…` only):** `angle_finding_fast_resim_interface.{cpp,hpp}`, `fast_resim_af_data.{c,h}`, `BUILD`, `deps/SRR8p_streamline_AF/` (~15+ files: `SRR8p_2D_gridsearch{,_fast}.c`, `NLS_2T_GNK.c`, `SRR8p_{add_AF_det_S1,add_AF_det_S5,bistatic_detection,calculate_error_mag,convert_angle_data_format,downselect_for_S4,elevation_fft,initialize_angle_data_S1,initialize_angle_data_S5}.c`, `FFTImplementationCallback.c`), `inc/`, `src/`, `utils/`. Ten-commit BUILD-options series (p2→p6 + command mode) — the branch is a self-contained AF fast path.
**Gen7 `tools/` arsenal:** `CI/`, `Generate_Signals_Properties/`, `ITF/` (exec specs), `bazel/`, `canape/`, `coverity/`, `lauterbach/`, `mayhem/`, `preCommit/`, `python/`, `sensorFlashTool/`, `tasking/`, `tools_tkey/`, `util_scripts/`, `vector_safety/`, `readmeImages/`.

## C.4 KPI suite file map (executables + entry points)

**UDP scripts (`KPI@master:gen7v2_resim_kpi_scripts/`, 23 files):** entry `run_kpi_script.py` (+`.bat`,`.sh`), `detection_matching_kpi_script.py` (`process_one_log(veh_csv, sim_csv, veh_rdd_csv, sim_rdd_csv, veh_cdc_csv, sim_cdc_csv, veh_vse_csv, sim_vse_csv) -> bool`, `process_logs(data_files)`, `plot_stats()`, `plot_data_across_logs()`, `func_bar/func_scatter` plot helpers; CLI `log_path.txt meta_data.json <outdir>`; `Metadata.from_file()`; per-run `UNIQUE_KEY=uuid4`, `time.time_ns()` split; debug/report filenames from `Config`), `tracker_matching_kpi_script.py`, `tracker_processed_det_kpi_script.py`, `tracker_info_kpi_script.py`, `tracker_vehicle_info_kpi_script.py`, `alignment_matching_kpi_script.py`, `interferenceDetection_matching_kpi_script.py`, `downSelection_matching_kpi_script.py`, `radarCapability_matching_kpi_script.py`, `config.py`, `constants.py`, `variables.py`, `file_handling.py`, `logger.py`, `meta_data.{py,json}`, `README.md`, `requirements.txt`, `log_path.txt`, `.vscode/`.
**`config.py` (class `Config`, FILE_VERSION `v1.5.0`, singleton `config`):** report titles (DET/DS/ALIGN/ID/RC *"KPIs and Plots"*), CSV suffixes (`_UDP_GEN7_DET_CORE.csv`, `_UDP_GEN7_DOWN_SELECTION_STREAM.csv`, `_UDP_GEN7_RDD_CORE.csv`, `_UDP_CDC.csv`, `_UDP_GEN7_VSE_CORE.csv`, `_UDP_GEN7_DYNAMIC_ALIGNMENT_STREAM.csv`, `_UDP_GEN7_ID_STREAM.csv`, `_UDP_GEN7_RADAR_CAPABILITY_STREAM.csv`), `DEFAULT_ACC_THRESHOLD=99.0`, `MAX_LOGS_IN_ONE_REPORT=20`, AF caps (front 768 / corner 680), DS caps (front 128 / corner 64), `MAX_NUM_OF_RDD_DETS=512`, `RADAR_CYCLE_S=0.05`, Tier-2 epsilons (`RAN 0.01`, `VEL 0.015`, `THETA/PHI 0.00873` + `Constants.EPSILON`), misalignment epsilons (AZ/EL 0.01), `ID_THRESHOLD=0`, `RC_STATUS_THRESHOLD=0`, `RESIM_MODE="CDC"` (meta-overridable), `MAX_CDC_RECORDS=5016`, range-saturation 135.0 (front/corner/unified).
**CAN KPI (`can_kpi_hdf/`, clean 4-layer):** `a_persistence_layer/{hdf_parser.py,hdf_wrapper.py,json_parser.py}`, `b_data_storage/{can_kpi_data_model_storage.py,kpi_config_storage.py}`, `c_business_layer/kpi_business.py`, `d_presentation_layer/kpi_html_gen.py`, `kpi/{sil_log_narrative.py,sil_radar_validation.py}`, `kpi_main.py`, `kpi.json`, `kpi_smoke.json`, `can_kpi.spec`, `can_singularity_KPI.def`. Plus `can_kpi_scripts/`, `main_html/`, `profile_timing_plotter/`.
**Report engine (`IPS/`):** `ResimHTMLReport.py` (+`ResimHTMLReport.spec`, `ResimHTML_linux.spec`, `launch.sh`), `InputParsing/`, `Inputs.json`, `input_{gt_vs_tracker,tracker_vs_tracker}.xml`, `HTMLConfig.xml`, `DataCollect/`, `DataPrep/`, `DataStore/`, `Sig_Prep/`, `Metadata/`, `EventMan/`, `DashManager/`, `RepGen/`, `PlotConvert/`, `plots_gen.py` (+`run_plots_gen.sh`), `NIPS_to_IPS_ploting.py`, `DGPSFilter/`, `Utilities/`, `connect_to_server.py`. Root: `requirements.txt`, `ResimHTMLReport.def`, `build_simg.sh` (Apptainer), `HTML_docker_Doc.docx`.
**KPI branch note:** single `master` (head `0fce967e` DET-2218 HDF-validation-in-DC-HTML) — KPI code is monoline; versioning happens in `config.FILE_VERSION` + `meta_data.json`, not branches.

## C.5 Decoder & bus-tool file map

**UDP decoder (`UD@master`, DLL 26.09.36 dated 2026-09-03):** per-platform/version matrix proves **F12 is structural** — SRR8P: DETECTION 200 / DS 200 / RDD 21 / HEADER 11 / TOI 106 / TOI_DS 105 / DYNAMIC_ALIGNMENT 16 / VSE 4 / MMIC 25 / RADAR_CAPABILITY 53 / OLP 1 / MOIS 2 / BSIS 3 / F360_* (DETECTION_LOG 4, OBJECTS_LOG 12, INTERNAL_{OBJECT,CLUSTER,CWD,DETECTION_HISTORY,REFLECTION_BUFFER}, HOST_PROPS_LOG, SENSOR_CALIB_LOG, FUNCTIONAL_SAFETY_FAULTS 13, HOST_CALIBS_LOG 3) / TIMING_INFO_LOG 22 / SYNC_INFO_LOG 3 / STATUS 9 / ID 5 / ALIGNMENT 10 / PARTNER_SENSOR 2 / CV_TRAILER 4 / STATIC_ENV_POLYS_LOG 3 (+SATELLITE_HIGH_DET: DET 115 / DS 101 / RDD 120); FLR8 mirrors SRR8P minus F360; SRR7P-V1/V2 + FLR7P-V1/V2 + RNA_SRR7P (ADC 3, CDC 8, RFFT 2, ROT_* 4/5/6/12/113/114, DRA_INTERNALS 1, CUST_ALGO 5, CTA/CED/LCDA_CUST 2, BLOCKAGE 1, DEBUG 11–13, DYNAMIC_ALIGNMENT 14/16, TOI_DS 3/8/105) + SRR6 (CDC 4, C0/C1/C2 cores). Stream dirs: `GEN8/GPO/{FLR8,SRR8}/{DETECTION/{V5,V115,V200 + Logging},RDD/{V13,V18,V21,V113,V117,V120 + Logging},ALIGNMENT,BSIS,…,TOI,…}`, `GEN7/{GPO_V1,GPO_V2/{FLR7,SRR7},RNA_GEN7}`, `GEN6P_DSPACE/`, `BMW_SP25/`, `HDF/`, `Common/`, `CommonFiles/`, `Utils/`, `GEN7_RADAR_UDP_DESIGN_DOCUMENT.xlsx`, `ReadMe.txt`. Latest commit: **Static Alignment Stream Addition**.
**Bordnet (`BN@master`, head `ad1cb4a0` "AL PCAN FLR HDF CHANGES"):** 40+ projects — `AL_PCAN_{Extractor,FLR_Decoder,FL_Decoder,RL_Decoder}` (+`AL_VCAN_*`), `CEER_{PCAN_{Extractor,FLR,FL,FR,RL,RR},SOMEIP_{Extractor,Wrapper,FL,FLR,FR,RL,RR,SRR7P}}`, `MCIP_PCAN_{Extractor,FLR,FL,FR,RR}}`, `BordNetDecoder/`, `mdf_log/`, `BOOST/`, `Cross_Platform/`, `Common_Headers/`, `Common_Structures/`, `Utility/`, `CMake/`, `Build.{bat,sh}`, `Inputs/`. Autogen matrix: `Auto_Gen_Files/Include/{AL_V1_1,AL_VCAN_V1_1,CEER_V1,V2,V3,V4,V4_0_2,V4_0_3,MCIP_V2,MCIP_V4,lib_types.h}` (+`Source/`, `CMake/`). Per-decoder anatomy (AL_PCAN_FL): `ALPCANFLDecoderBase_V1_1.h`, `AL_PCAN_FL_{Common_V1_1,Decoder_V1_1.{h,cpp},DecoderAPI.cpp}`, `IAL_PCAN_FL_Decoder{,API,_V1_1}.h`, `ReleaseNotes_AL_PCAN_FL.txt`, `Version.h`, `Resource.rc`, `CMake`. Docs: `Document/{BordnetSDD.DOCX,Design - RESIM BordnetDll.docx,AUTOSAR_PRS_SOMEIPProtocol.pdf,AUTOSAR_SWS_SOMEIPTransportProtocol.pdf,SOME-IP basics.pptx,SOMIP_PPT.PPTX,ThunderGateway.pptx}`.
**HIL (`HIL@master`):** `ApplicationProjects/{SRR3_Resimulator,DPH_RR_ADAS_{Config,HIL,LOGGER},CAN_MF4_Logger,MDF4_Decoder,BOOST{,_VS15,_VS19},output/}`, `CoreLibraryProjects/{CCA_ViGEM,CCA_Vpcap,MUDP_Serializer,SRR3_Internal_Data_Logger,UDP_Transmitter,XCP_SeedKey{,_Lib},dvl{File,Message},excel_utils,mdf_log,mdf_log_convertor,mudp_{decoder,decoder_calib,log,receiver},mudp_log,ptp_to_xml_generator,radar_stream_decoder,CrossPlatform}`, `Common/{IRadarStream.h,IRadarStream_Lib_Loader.h,Z_Logging.h}`, `CommonFiles/{CommonHeaders,MDF_Include,Raw_headers,Stream_headers,Utility,cca_vigem_inc,helper,inc,libfort,plugin,sym,thread,udp_headers}`, `Build_Project.sh`, `HILEngine_BUILD_README.doc`, `logs/`, `.vscode/`, `.gitingore`. SRR3_Resimulator contains dual CMake+VS builds, `EthernetGrabber`, `Dessector/` (Wireshark dissector `wireshark_dessector.cpp` + File_list), `IniConfig`, `MDFLog`, `SRR3_{Comm,HiL_Exec,Resim}`, `Mudp_{decoder,log}`, `libfort` — i.e. the HiL box is a full second decode stack (ViGEm→MDF→UDP-inject).

## C.6 Domain-controller & logic-model file map

**DC (`DC@master`):** `DC_SIL/{Build{,.sh},BuildAll.{bat,sh},CMakeLists.txt,Create_Image.sh,Generate_Veh_Singularity.sh,fetch_jfrog_binaries.sh,input_path.xml,update.py,update_veh_sing_xml.py,version.py,sil_wrappers/{sil_wrapper.{cpp,h},olp_wrapper.{cpp,h},sfl_wrapper.{cpp,h},Tracker_VariantA_Wrapper.{cpp,h},tracker_out_iface.h,dc_{app_version,config,version}.h,rr_cal_log.h},sil_config_read/{dc_read_config.cpp,.h},sil_input/,sil_output/,sil_engine_headers/,sil_component_streams/,sil_udp_streams/,sil_debug_file/,sil_dgps/(DGPS branch),sil_smc/,sil_ext_libs/,sil_executables/{dc_config/,dgps_config/(DGPS),fw_dlls/,fw_dlls_gen8/,radar_dlls/}}`, `Application/{F360Tracker/{F360TrackerLib,OLP_Core,VSE_Core,ocg,rspp,sg_stationary_geometry},FeatureFunctions/{CED,CTA,ESA,LCDA,PT,RECW,SCW,TA,Calibration_Tool,Feature_Building_Kit,Math_Library,Mock_Files,LTB},UDP_Logging/,utilities/cmake}`.
**LM2 (`LM2@main` + branches):** `Code/{Generic/{LM2_Interface,Generic_Interface,MUDP_Serializer,CAN_INTF,UdpSocket,XML_Reader,TimingInfo,Version}.{h,cpp}+{DFT_Sfl,Dspace_Lime_Interface,Interface_Enums,Interface_Input_Struct,Resim_Config,MemMap,RR_ADAS_SYMBOL_{Data,Enum},Rte_Type_SomeIP_RX,SRR3_SIL_Output_Data,sil_ecu_input_ext,radar_ecu_CORE3,dvl_message,some_ip_{input_RX,output},tracker_output_interface}.h,Customer/{BMW_{High,MID,Low},DSPACE_Common,Motional_Srr3_SIL,Scania,TML_SRR5}_Log_Convertor{,.cpp/.h}+Checksum.cpp(+STLA_Small_IFV600_Converter on ADCAM branches),Common_Headers/{SIL_Library.h,customer_ID_config.h,fixmac.h,MDF_Include/,helper/,udp_headers/},UDP_logging_headers/,libs/,Interface_Output_Control.xml}`, `LM2_Packaging/{Build{,_All}.{bat,sh},build_and_deploy.{ps1,sh},Build_FMUs.sh,find_vs.py,Deliverables/*.fmu+*.dll,LM2_FMU/{Build.py,BuildForAllCustomer.py,CMakeLists.txt,prepare_fmu.py,UpdateModelDescription.py,modelDescription.{xml,in.xml},index.txt,Code/{FMI2/,FMI_interface.cpp,LiME_{Tools,Windows}.h,LogicModel{,Base}2.{h,cpp}},Data/},LM2_Interface/,Common/Code/}`, `Tools/{Simpler_Code_For_MUDP_TX,TestInterface,XML_Comparator}` (+`Tools/` VCS→OSI converter on JBC-238), `STLA_Small_LM2/{CustomFunctions/FmuEthBridge,Development/fmu,Documentation/{BUILD,DESIGN}.md,README.md}` (STLA branch), `Documentation/{Logic Model Guide.docx,Dspace.docx,AS_DSPACE Block Diagram.pptx,Doxygen/,Doxyzen_Doocument.docx}`, `RULESETS.md` (7 rulesets, Codex Validation workflow gate), `ADMIN_AUDIT_LOG.md`, `Version.{hpp,cpp}` (Major/Minor/Patch class, author `abhishek.uh@aptiv.com`).

## C.7 Snapshot→question lookup (which folder answers which question)

| Question | Open this snapshot first | Then compare |
|---|---|---|
| How did CFAR thresholds evolve? | `awr294x/feature__variable_cfar_initial_changes` (GHW-94 LUT) | G7 `dev:software/bbe32/src/doppler_proc.c` + `bwdep_*_thold` |
| What does RSP-SIL change vs trunk? | G8 `feature__RSP_SIL` (`rdu_sil_interface/`, EUR-2031 DA comment) | G8 `dev:sil/rsp_sil/` |
| Fast-resim vs full pipeline? | G8 `feature__CUW-5824-Fast-Resim_AF-Integration:sil/fast_resim/` | G8 `dev:sil/rsp_sil/` + `software/bbe32/src/anglefinding_project_interface.c` |
| Scan-index fix scope? | G8 `story__HLR-1436-fusion` (tracker scan-index, header deps) | `release__v6.1.x` (its base) |
| DGPS ground-truth path? | DC `feature__DGPS` (`sil_dgps/`, `dgps_config/`) | KPI `IPS/DGPSFilter/` |
| Windows FMU packaging? | LM2 `feature__JBC-138__Enable_Windows_FMU_build` | LM2 `main:LM2_Packaging/` |
| USS aggregator? | LM2 `feature__USS_Aggregator` (deliverable FMUs) | empty `Core_RESIM_USS_Sensor_Model` |
| Release-to-release drift? | G7 `release__v8.1.x` → `release__v12.0.x` chain; G8 `release__v5.1.x` → `release__v7.0.x` | `dev` (integration delta) |
| EmLib version matrix? | G7 `feature__EmbLibv11.0.x`; G8 `feature__EmbLibv7.0.x`; AWR `feature__emb-lib-{linux,hdf5}` + `emblib_tracker` | trunk `emb_tracker/` dirs |

---

*Appendix C generated 2026-09-24 from `git show`/`ls-tree` against pinned refs. Refresh: re-run the same commands after `git fetch`.*
