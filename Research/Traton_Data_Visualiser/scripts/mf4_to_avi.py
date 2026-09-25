"""
Extract video from APTIV-ORCAS MF4 file and write as Motion JPEG AVI.

Usage:
    python scripts/mf4_to_avi.py <input.mf4> [-o OUTPUT] [-c [QUALITY]]

The output filename always matches the input filename stem with a .avi
extension.  Use -o to redirect it to a different directory or file path.
Frames are always re-encoded at JPEG quality 40 by default.
Use -c QUALITY to override the quality level (1–95).

See docs/05_video_extraction.md for a full explanation of the MDF4 block
structure and why asammdf cannot read the VLSD channel directly.
"""
import struct, sys, zlib, io, numpy as np
from pathlib import Path
from PIL import Image
import asammdf


# ── MDF4 block helpers ─────────────────────────────────────────────────────

def _read_header(f, addr):
    f.seek(addr)
    raw = f.read(24)
    block_id = raw[0:4].decode("ascii", errors="replace")
    block_len = struct.unpack_from("<Q", raw, 8)[0]
    links_nr  = struct.unpack_from("<Q", raw, 16)[0]
    return block_id, block_len, links_nr

def _read_links(f, addr, links_nr):
    f.seek(addr + 24)
    return list(struct.unpack(f"<{links_nr}Q", f.read(links_nr * 8)))

def _read_dt(f, addr):
    block_id, block_len, links_nr = _read_header(f, addr)
    data_off = addr + 24 + links_nr * 8
    data_len = block_len - 24 - links_nr * 8
    f.seek(data_off)
    return f.read(data_len)

def _read_dz(f, addr):
    block_id, block_len, links_nr = _read_header(f, addr)
    off = addr + 24 + links_nr * 8
    f.seek(off)
    dz = f.read(block_len - 24 - links_nr * 8)
    # DZ data section layout:
    #   [0:2]  orig_block_type (e.g. "SD")
    #   [2]    zip_type  (0 = zlib, 1 = transposition + zlib)
    #   [3]    reserved
    #   [4:8]  zip_parameter (record size for zip_type=1)
    #   [8:16] orig_data_length
    #   [16:24] data_length
    #   [24:]  compressed payload
    zip_type  = dz[2]
    zip_param = struct.unpack_from("<I", dz, 4)[0]
    orig_len  = struct.unpack_from("<Q", dz, 8)[0]
    comp_len  = struct.unpack_from("<Q", dz, 16)[0]
    payload   = dz[24 : 24 + comp_len]
    if zip_type == 0:
        return zlib.decompress(payload)
    elif zip_type == 1:
        buf  = zlib.decompress(payload)
        cols = zip_param
        rows = orig_len // cols
        return np.frombuffer(buf, dtype=np.uint8).reshape(cols, rows).T.tobytes()
    raise ValueError(f"Unknown DZ zip_type {zip_type}")

def collect_signal_data(filepath, hl_addr):
    """
    Walk the HL -> DL -> ##DT/##DZ block chain and return the concatenated,
    decompressed signal data bytes for a VLSD channel.
    """
    buf = bytearray()
    with open(filepath, "rb") as f:
        _, _, ln = _read_header(f, hl_addr)
        dl_addr = _read_links(f, hl_addr, ln)[0]
        while dl_addr:
            _, _, dl_ln = _read_header(f, dl_addr)
            links = _read_links(f, dl_addr, dl_ln)
            next_dl = links[0]
            for db_addr in (a for a in links[1:] if a):
                bid, _, _ = _read_header(f, db_addr)
                if   bid == "##DT": buf += _read_dt(f, db_addr)
                elif bid == "##DZ": buf += _read_dz(f, db_addr)
            dl_addr = next_dl
    return bytes(buf)

def read_vlsd(signal_data, offset):
    """Read one length-prefixed VLSD entry: [uint32 length][data bytes]."""
    off = int(offset)
    length = struct.unpack_from("<I", signal_data, off)[0]
    return signal_data[off + 4 : off + 4 + length]


# ── AVI / RIFF writer ──────────────────────────────────────────────────────

def _dword(n): return struct.pack("<I", n)
def _chunk(fcc, data): return fcc.encode() + _dword(len(data)) + data
def _list(fcc, data):  return b"LIST" + _dword(4 + len(data)) + fcc.encode() + data

def write_mjpeg_avi(out_path, frames_jpeg, fps, width, height):
    """
    Write a Motion JPEG AVI (RIFF AVI  format).
    frames_jpeg: list of raw JPEG bytes (already JPEG-compressed).
    """
    n = len(frames_jpeg)

    # idx1 legacy index
    idx1 = bytearray()
    offset = 4               # movi content starts after the "movi" FourCC
    for jpeg in frames_jpeg:
        idx1 += b"00dc"      # stream 0, compressed video chunk
        idx1 += _dword(0x10) # AVIIF_KEYFRAME
        idx1 += _dword(offset)
        idx1 += _dword(len(jpeg))
        offset += 8 + len(jpeg)
        if len(jpeg) & 1:
            offset += 1      # word-boundary padding

    # movi LIST
    movi_data = bytearray()
    for jpeg in frames_jpeg:
        movi_data += b"00dc" + _dword(len(jpeg)) + jpeg
        if len(jpeg) & 1:
            movi_data += b"\x00"

    # AVI Main Header (avih) — 56 bytes
    us_per_frame      = int(1_000_000 / fps)
    max_bytes_per_sec = max(len(j) for j in frames_jpeg) * fps
    avih = struct.pack("<IIIIIIIIIIIIII",
        us_per_frame,       # dwMicroSecPerFrame
        max_bytes_per_sec,  # dwMaxBytesPerSec
        0,                  # dwPaddingGranularity
        0x10,               # dwFlags: AVIF_HASINDEX
        n,                  # dwTotalFrames
        0,                  # dwInitialFrames
        1,                  # dwStreams
        0,                  # dwSuggestedBufferSize
        width,              # dwWidth
        height,             # dwHeight
        0, 0, 0, 0,         # dwReserved[4]
    )

    # Stream Header (strh) — 56 bytes
    strh = struct.pack("<4s4sIHHIIIIIIIIhhhh",
        b"vids",            # fccType
        b"MJPG",            # fccHandler
        0,                  # dwFlags
        0,                  # wPriority
        0,                  # wLanguage
        0,                  # dwInitialFrames
        1,                  # dwScale
        fps,                # dwRate
        0,                  # dwStart
        n,                  # dwLength
        max(len(j) for j in frames_jpeg),  # dwSuggestedBufferSize
        0,                  # dwQuality
        0,                  # dwSampleSize
        0, 0, width, height,  # rcFrame: left, top, right, bottom
    )

    # Stream Format (strf) = BITMAPINFOHEADER — 40 bytes
    strf = struct.pack("<IiiHHIIIIii",
        40, width, height, 1, 24,
        0x47504A4D,         # biCompression = MJPG
        width * height * 3,
        0, 0, 0, 0,
    )

    strl      = _list("strl", _chunk("strh", strh) + _chunk("strf", strf))
    hdrl      = _list("hdrl", _chunk("avih", avih) + strl)
    movi      = _list("movi", bytes(movi_data))
    idx       = _chunk("idx1", bytes(idx1))
    riff_data = hdrl + movi + idx
    riff      = b"RIFF" + _dword(4 + len(riff_data)) + b"AVI " + riff_data

    with open(out_path, "wb") as f:
        f.write(riff)
    print(f"Written: {out_path}  ({len(riff):,} bytes, {n} frames @ {fps} fps)")


def _recompress_jpeg(data: bytes, quality: int) -> bytes:
    """Re-encode a JPEG frame at the given Pillow quality level (1-95)."""
    buf = io.BytesIO()
    Image.open(io.BytesIO(data)).save(buf, format="JPEG", quality=quality)
    return buf.getvalue()


# ── Main ───────────────────────────────────────────────────────────────────

def mf4_to_avi(mf4_path: str, out: str = None, compress: int = 40):
    mf4_path = Path(mf4_path)

    # Resolve output path: always keep input filename stem
    if out is None:
        avi_path = mf4_path.with_suffix(".avi")
    else:
        out_p = Path(out)
        if out_p.suffix.lower() == ".avi":
            avi_path = out_p          # explicit full path
        else:
            avi_path = out_p / mf4_path.with_suffix(".avi").name  # directory

    print(f"Input : {mf4_path}")
    print(f"Output: {avi_path}")

    print("Opening MF4...", flush=True)
    mf = asammdf.MDF(str(mf4_path))

    h_sig  = mf.get("MF4Frame.VideoHeight")
    ts_sig = mf.get("TimeStamp")

    video_mask = h_sig.samples == 480
    video_idx  = np.where(video_mask)[0]
    n_frames   = len(video_idx)
    print(f"Video frames : {n_frames}")

    if n_frames == 0:
        print("No video frames found.")
        return

    # Estimate FPS from inter-frame timestamps
    ts = ts_sig.samples[video_idx]
    ts_sorted = np.sort(ts)
    deltas = np.diff(ts_sorted)
    deltas = deltas[deltas > 0]
    fps = max(1, round(1.0 / float(np.median(deltas)))) if len(deltas) else 25
    print(f"Estimated FPS: {fps}")

    # Locate the DataBytes VLSD channel and read the full signal data area
    db_ch = next(ch for ch in mf.groups[0].channels if ch.name == "MF4Frame.DataBytes")
    print("Reading signal data (VLSD HL block)...", flush=True)
    signal_data = collect_signal_data(str(mf4_path), db_ch.data_block_addr)
    print(f"Signal data : {len(signal_data):,} bytes")

    # Extract per-record VLSD offsets from the raw record bytes
    print("Extracting VLSD offsets...", flush=True)
    grp   = mf.groups[0]
    inner = mf._mdf
    data_gen    = inner._load_data(grp, record_offset=0, record_count=None)
    raw_records = bytearray()
    for fragment in data_gen:
        if hasattr(fragment, "data"):
            raw_records += fragment.data
        elif isinstance(fragment, (tuple, list)):
            raw_records += fragment[0]
        else:
            raw_records += bytes(fragment)

    cg       = grp.channel_group
    rec_size = cg.samples_byte_nr + cg.invalidation_bytes_nr
    n_rec    = len(raw_records) // rec_size
    records  = np.frombuffer(raw_records, dtype=np.uint8).reshape(n_rec, rec_size)
    byte_off = db_ch.byte_offset
    offsets  = np.frombuffer(
        records[:, byte_off : byte_off + 8].tobytes(),
        dtype=np.uint64,
    )

    # Decode JPEG frames sorted by timestamp
    print("Decoding JPEG frames...", flush=True)
    order       = np.argsort(ts_sig.samples[video_idx])
    frames_jpeg = []
    width = height = 0
    for rank, i in enumerate(order):
        rec_i = video_idx[i]
        jpeg  = read_vlsd(signal_data, offsets[rec_i])
        if len(jpeg) < 4 or jpeg[:2] != b"\xff\xd8":
            print(f"  Warning: frame {rank} (record {rec_i}) is not JPEG — skipping")
            continue
        frames_jpeg.append(jpeg)
        if width == 0:
            img = Image.open(io.BytesIO(jpeg))
            width, height = img.size
        if (rank + 1) % 100 == 0:
            print(f"  {rank + 1}/{n_frames}", flush=True)

    print(f"Decoded {len(frames_jpeg)} valid JPEG frames ({width}x{height})")

    # Re-compress frames if requested
    if compress is not None:
        print(f"Re-compressing frames at JPEG quality {compress}...", flush=True)
        orig_size = sum(len(j) for j in frames_jpeg)
        frames_jpeg = [_recompress_jpeg(j, compress) for j in frames_jpeg]
        new_size = sum(len(j) for j in frames_jpeg)
        print(f"  Size: {orig_size:,} -> {new_size:,} bytes  "
              f"({100 * new_size // orig_size}% of original)")

    write_mjpeg_avi(str(avi_path), frames_jpeg, fps, width, height)


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
        prog="mf4_to_avi.py",
        description=(
            "Extract the embedded video stream from an APTIV-ORCAS MF4 log file "
            "and write it as a Motion JPEG AVI.\n\n"
            "The video is stored in the 'MF4Frame.DataBytes' VLSD channel "
            "(640x480, ~5 fps, JPEG-compressed frames).\n\n"
            "The output filename always matches the input stem with a .avi extension.\n\n"
            "Frames are always re-encoded at JPEG quality 40 by default.\n\n"
            "Examples:\n"
            "  python mf4_to_avi.py Recording_001.mf4\n"
            "  --> Recording_001.avi  (same directory, quality 40)\n\n"
            "  python mf4_to_avi.py Recording_001.mf4 -o D:/output/\n"
            "  --> D:/output/Recording_001.avi  (quality 40)\n\n"
            "  python mf4_to_avi.py Recording_001.mf4 -c\n"
            "  --> Recording_001.avi  (re-encoded at JPEG quality 70)\n\n"
            "  python mf4_to_avi.py Recording_001.mf4 -o D:/output/ -c 50\n"
            "  --> D:/output/Recording_001.avi  (re-encoded at JPEG quality 50)"
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        "input",
        metavar="INPUT.mf4",
        help="Path to the APTIV-ORCAS MF4 log file to convert.",
    )
    parser.add_argument(
        "-o", "--output",
        metavar="PATH",
        default=None,
        help=(
            "Output path. May be a directory (the AVI filename is derived from "
            "the input stem) or a full .avi file path. "
            "Default: same directory as the input file."
        ),
    )
    parser.add_argument(
        "-c", "--compress",
        metavar="QUALITY",
        nargs="?",
        const=70,
        type=int,
        default=40,
        help=(
            "Re-encode each JPEG frame at the given quality level (1–95). "
            "Lower values produce smaller files at the cost of image quality. "
            "If -c is given without a value, quality defaults to 70. "
            "Without -c, frames are re-encoded at the default quality of 40."
        ),
    )
    args = parser.parse_args()

    if args.compress is not None and not (1 <= args.compress <= 95):
        parser.error("QUALITY must be between 1 and 95.")

    mf4_to_avi(args.input, out=args.output, compress=args.compress)
