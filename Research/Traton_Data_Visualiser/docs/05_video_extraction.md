# Video Extraction from APTIV-ORCAS MF4 Files

## Overview

APTIV-ORCAS MF4 log files contain an embedded video stream alongside the radar CAN/SOME-IP data.
The video is stored as a VLSD (Variable Length Signal Data) channel within the single channel group of the file.

---

## MF4 Channel Structure

All data sits in **one channel group** (Group 0) with 22 channels:

| Channel | Description |
|---------|-------------|
| `MF4Frame` | Master frame record |
| `MF4Frame.SOMEIPClientId` … `SOMEIPSessionId` | SOME/IP header fields |
| `MF4Frame.BusChannel`, `ID`, `DLC` … | CAN frame metadata |
| `MF4Frame.DataLength` | Byte length of the payload for this record |
| **`MF4Frame.DataBytes`** | **VLSD channel — contains the actual payload (video or CAN data)** |
| `MF4Frame.ProtocolType`, `Cycle`, `MetaData` | Protocol metadata |
| **`MF4Frame.VideoHeight`** | Frame height in pixels (0 for non-video records, **480** for video) |
| **`MF4Frame.VideoWidth`** | Frame width in pixels (0 for non-video records, **640** for video) |
| **`MF4Frame.VideoDepth`** | Colour depth in bits (0 or **24** for RGB video) |
| `TimeStamp` | Record timestamp |

Video frames are identified by `VideoHeight == 480`.  All other records (CAN/SOME-IP frames) have `VideoHeight == 0`.

---

## Video Format

| Property | Value |
|----------|-------|
| Resolution | 640 × 480 |
| Colour depth | 24-bit RGB |
| Encoding | **Motion JPEG** (each frame is a self-contained JPEG, magic bytes `FF D8 FF`) |
| Typical frame size | ~120 KB (compressed) vs 921 KB uncompressed |
| Frame rate | ~5 fps (estimated from inter-frame timestamps) |
| Frames per ~530 MB file | ~575 |

---

## VLSD Block Structure (why asammdf cannot read it directly)

The `MF4Frame.DataBytes` channel is a **VLSD channel** (`channel_type = 1`).
In standard MDF4, a VLSD channel's `data_block_addr` points to an `##SD` (Signal Data) block.
In these APTIV-ORCAS files it points to an **`##HL` (Header List) block** instead — a fragmented,
zlib-compressed signal data area.  asammdf 8.x raises `MdfException: Wrong signal data block reference`
when it encounters this.

The block chain is:

```
data_block_addr
    └─► ##HL  (Header List, 1 link)
            └─► ##DL  (Data List, up to 2000 links per DL)
                    ├─► ##DZ  (Zipped Data, zip_type=0 = zlib)
                    ├─► ##DZ
                    └─► ...  (~1600–2000 DZ blocks per DL, ~17 DL blocks per file)
```

Each `##DZ` block decompresses to a chunk of the **signal data area**.
The complete concatenated signal data is ~1 GB per file.

**Signal data layout** (standard MDF4 VLSD):
```
[uint32 length][data bytes][uint32 length][data bytes]...
```

Each record in the channel group stores an **8-byte uint64 offset** (at `byte_offset = 61` in the
record) that points into this signal data area.  To read a frame:
1. Read the uint64 offset from the record.
2. Seek to that offset in the decompressed signal data.
3. Read the uint32 length prefix.
4. Read that many bytes → raw JPEG payload.

---

## Extraction Script

`scripts/mf4_to_avi.py` converts a single APTIV-ORCAS MF4 file to a **Motion JPEG AVI**.

### Usage

```
python scripts/mf4_to_avi.py <input.mf4>
```

The output AVI is always written in the same directory as the input file with the
same filename stem and a `.avi` extension (e.g. `Recording_001.mf4` → `Recording_001.avi`).

### Dependencies

All already present in the project venv:

| Package | Purpose |
|---------|---------|
| `asammdf` | Opens the MF4 and reads scalar channels (Height, Width, Timestamps) |
| `numpy` | Record array slicing for fast VLSD offset extraction |
| `Pillow` | Validates JPEG frames and reads their dimensions |
| `zlib` (stdlib) | Decompresses `##DZ` blocks |
| `struct` (stdlib) | Parses MDF4 binary block headers and writes RIFF/AVI |

### Output

A **RIFF AVI** file with:
- Video stream type: `vids`, codec FourCC: `MJPG`
- Frames sorted by timestamp
- `idx1` legacy index for compatibility with older players

### Performance

Processing one ~530 MB file takes approximately 3–4 minutes on a typical workstation:
- ~2 min decompressing the 17 DL chains (~5600 DZ blocks total)
- ~1 min loading raw record data and extracting VLSD offsets
- ~10 s decoding/validating 575 JPEG frames and writing the AVI
