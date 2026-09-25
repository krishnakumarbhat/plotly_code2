"""
video_widget.py
===============
Detachable "Video" tab that displays the APTIV-ORCAS camera stream frame
synchronised to the current ScanIndex_L navigation position.

Synchronisation key
-------------------
Every video frame (MF4Frame.ProtocolType == 5) carries an ASCII string in
the MF4Frame.MetaData VLSD channel:

    FL:XXXX;FR:XXXX;RL:XXXX;RR:XXXX;CAN_TS:XXXX

RL is the rear-left radar scan counter, which equals ScanIndex_L exactly.
RR equals ScanIndex_R.  The widget uses RL as the primary key.

Loading strategy
----------------
1.  ``set_mf4_path(path)`` is called by MainWindow whenever a new MF4 is
    loaded.  The widget resets to "ready" state (Load Video button enabled).
2.  The user clicks "Load Video".
    - If ``{mf4_stem}_video_cache.pkl`` already exists beside the MF4, the
      cache is read into memory in < 1 s (typically ~60–70 MB JPEG data).
    - Otherwise a background thread runs the full VLSD extraction (~3-4 min
      for a 2-minute APTIV file), then saves the cache for next time.
3.  ``show_frame(scan_rl)`` is called on every navigation step.  It looks up
    the nearest RL key in the in-memory dict and displays the JPEG.  The
    lookup is O(1) (dict get) after a single np.searchsorted for "nearest".
"""

from __future__ import annotations

import io
import pickle
import re
import struct
import zlib
from pathlib import Path
from typing import Optional

import numpy as np
from PyQt6.QtCore import Qt, QObject, QThread, pyqtSignal
from PyQt6.QtGui import QImage, QPixmap
from PyQt6.QtWidgets import (
    QComboBox,
    QHBoxLayout,
    QLabel,
    QMessageBox,
    QProgressDialog,
    QPushButton,
    QScrollArea,
    QSizePolicy,
    QStackedWidget,
    QVBoxLayout,
    QWidget,
)

try:
    import asammdf as _asammdf
except ImportError:
    _asammdf = None

try:
    from PIL import Image as _PIL_Image
except ImportError:
    _PIL_Image = None


# ---------------------------------------------------------------------------
# MDF4 low-level VLSD helpers
# (self-contained copy so the GUI works without importing from scripts/)
# ---------------------------------------------------------------------------

def _mf4_header(f, addr):
    f.seek(addr)
    raw  = f.read(24)
    bid  = raw[:4].decode("ascii", errors="replace")
    blen = struct.unpack_from("<Q", raw, 8)[0]
    lnr  = struct.unpack_from("<Q", raw, 16)[0]
    return bid, blen, lnr


def _mf4_links(f, addr, lnr):
    f.seek(addr + 24)
    return list(struct.unpack(f"<{lnr}Q", f.read(lnr * 8)))


def _read_dt_block(f, addr):
    _, blen, lnr = _mf4_header(f, addr)
    f.seek(addr + 24 + lnr * 8)
    return f.read(blen - 24 - lnr * 8)


def _read_dz_block(f, addr):
    _, blen, lnr = _mf4_header(f, addr)
    f.seek(addr + 24 + lnr * 8)
    d = f.read(blen - 24 - lnr * 8)
    # DZ data section layout (after the 24-byte block header + links):
    #   [0:2]   orig_block_type  (e.g. "SD")
    #   [2]     zip_type  (0=zlib, 1=transposition+zlib)
    #   [3]     reserved
    #   [4:8]   zip_parameter (record size for zip_type 1)
    #   [8:16]  orig_data_length
    #   [16:24] data_length (compressed payload size)
    #   [24:]   compressed payload
    zip_type  = d[2]
    zip_param = struct.unpack_from("<I", d, 4)[0]
    comp_len  = struct.unpack_from("<Q", d, 16)[0]
    payload   = d[24 : 24 + comp_len]
    if zip_type == 0:
        return zlib.decompress(payload)
    if zip_type == 1:
        orig_len = struct.unpack_from("<Q", d, 8)[0]
        buf  = zlib.decompress(payload)
        cols = zip_param
        rows = orig_len // cols
        return np.frombuffer(buf, dtype=np.uint8).reshape(cols, rows).T.tobytes()
    raise ValueError(f"Unsupported DZ zip_type {zip_type}")


def _collect_vlsd(filepath: str, hl_addr: int,
                  progress_cb=None, p_start: int = 0, p_end: int = 50) -> bytes:
    """Walk HL → DL → DT/DZ chain; return concatenated decompressed bytes.

    Optional *progress_cb(pct, msg)* is called with rough progress in the
    range [p_start, p_end].
    """
    buf = bytearray()

    # Count DL blocks first so we can emit meaningful progress.
    block_addrs: list[int] = []
    with open(filepath, "rb") as f:
        _, _, ln = _mf4_header(f, hl_addr)
        dl_addr = _mf4_links(f, hl_addr, ln)[0]
        while dl_addr:
            _, _, dl_ln = _mf4_header(f, dl_addr)
            ls = _mf4_links(f, dl_addr, dl_ln)
            block_addrs.append(dl_addr)
            dl_addr = ls[0]

    n_blocks = len(block_addrs) or 1

    with open(filepath, "rb") as f:
        for bi, dl_addr in enumerate(block_addrs):
            _, _, dl_ln = _mf4_header(f, dl_addr)
            ls = _mf4_links(f, dl_addr, dl_ln)
            for db_addr in (a for a in ls[1:] if a):
                bid, _, _ = _mf4_header(f, db_addr)
                if   bid == "##DT":
                    buf += _read_dt_block(f, db_addr)
                elif bid == "##DZ":
                    buf += _read_dz_block(f, db_addr)
            if progress_cb is not None:
                pct = p_start + int((p_end - p_start) * (bi + 1) / n_blocks)
                progress_cb(pct, f"Reading signal data block {bi + 1}/{n_blocks}…")

    return bytes(buf)


def _read_vlsd_entry(sd: bytes, offset: int) -> bytes:
    """Read one length-prefixed VLSD entry: [uint32 length][data bytes]."""
    off = int(offset)
    n   = struct.unpack_from("<I", sd, off)[0]
    return sd[off + 4 : off + 4 + n]


def _load_raw_records(mf, group_index: int = 0) -> bytes:
    """Load all raw record bytes for group *group_index* via asammdf._load_data."""
    grp   = mf.groups[group_index]
    inner = mf._mdf
    raw   = bytearray()
    for frag in inner._load_data(grp, record_offset=0, record_count=None):
        if hasattr(frag, "data"):
            raw += frag.data
        elif isinstance(frag, (tuple, list)):
            raw += frag[0]
        else:
            raw += bytes(frag)
    return bytes(raw)


def _vlsd_offsets_from_records(raw: bytes, rec_size: int,
                                byte_offset: int) -> np.ndarray:
    """Extract uint64 VLSD byte offsets for one channel from the raw record array."""
    n    = len(raw) // rec_size
    recs = np.frombuffer(raw, dtype=np.uint8).reshape(n, rec_size)
    bo   = byte_offset
    return np.frombuffer(recs[:, bo : bo + 8].tobytes(), dtype=np.uint64)


# ---------------------------------------------------------------------------
# MetaData parser
# ---------------------------------------------------------------------------

_META_RE = re.compile(r"RL:(\d+).*?RR:(\d+)", re.ASCII)


def _parse_meta(raw: bytes) -> tuple[int, int] | None:
    """Parse MetaData payload → (rl, rr) or None on failure."""
    try:
        m = _META_RE.search(raw.decode("ascii", errors="ignore"))
        if m:
            return int(m.group(1)), int(m.group(2))
    except Exception:
        pass
    return None


def _recompress_jpeg(data: bytes, quality: int) -> bytes:
    """Re-encode JPEG at *quality* (1-95) using Pillow; pass-through if unavailable."""
    if _PIL_Image is None:
        return data
    buf = io.BytesIO()
    _PIL_Image.open(io.BytesIO(data)).save(buf, format="JPEG", quality=quality)
    return buf.getvalue()


# ---------------------------------------------------------------------------
# Background extraction worker
# ---------------------------------------------------------------------------

class _ExtractionWorker(QObject):
    """
    Background QObject that extracts all video frames from an MF4 file and
    saves them as a scan-index-keyed cache.

    Emits:
      progress(int, str)  – (0-100, status message)
      finished(dict)      – {"rl_sorted": np.ndarray, "frames": {rl: bytes}}
      error(str)          – traceback on failure
    """

    progress = pyqtSignal(int, str)
    finished = pyqtSignal(dict)
    error    = pyqtSignal(str)

    def __init__(self, mf4_path: str, cache_path: str, compress: int = 40):
        super().__init__()
        self.mf4_path   = mf4_path
        self.cache_path = cache_path
        self.compress   = compress

    def run(self) -> None:
        try:
            result = self._extract()
            self.finished.emit(result)
        except Exception:
            import traceback
            self.error.emit(traceback.format_exc())

    def _emit(self, pct: int, msg: str) -> None:
        self.progress.emit(pct, msg)

    def _extract(self) -> dict:
        p = self._emit

        p(2, "Opening MF4…")
        mf = _asammdf.MDF(self.mf4_path)

        p(4, "Scanning for video frames…")
        h_sig      = mf.get("MF4Frame.VideoHeight")
        video_mask = h_sig.samples == 480
        video_idx  = np.where(video_mask)[0]
        n_frames   = len(video_idx)
        if n_frames == 0:
            raise RuntimeError(
                "No video frames found (MF4Frame.VideoHeight == 480) in this file.\n"
                "This file may not contain an APTIV-ORCAS camera stream."
            )

        # Locate the two VLSD channels
        chans   = {ch.name: ch for ch in mf.groups[0].channels}
        db_ch   = chans["MF4Frame.DataBytes"]
        meta_ch = chans["MF4Frame.MetaData"]

        p(5, f"Found {n_frames} video frames.\n"
              "Reading DataBytes signal data (VLSD HL chain)…\n"
              "This takes a few minutes for a 2-minute recording.")

        # DataBytes VLSD: the large one (~1 GB) – progress from 5 → 50
        data_sd = _collect_vlsd(
            self.mf4_path, db_ch.data_block_addr,
            progress_cb=lambda pct, msg: p(pct, msg),
            p_start=5, p_end=50,
        )

        p(52, "Reading MetaData signal data (VLSD HL chain)…")
        meta_sd = _collect_vlsd(self.mf4_path, meta_ch.data_block_addr,
                                 p_start=52, p_end=58)

        p(60, "Loading fixed-size record array…")
        raw      = _load_raw_records(mf)
        cg       = mf.groups[0].channel_group
        rec_size = cg.samples_byte_nr + cg.invalidation_bytes_nr

        p(64, "Extracting VLSD offsets…")
        data_offsets = _vlsd_offsets_from_records(raw, rec_size, db_ch.byte_offset)
        meta_offsets = _vlsd_offsets_from_records(raw, rec_size, meta_ch.byte_offset)

        p(68, f"Decoding {n_frames} JPEG frames…")
        frames_by_rl: dict[int, bytes] = {}
        skipped = 0

        for k, rec_i in enumerate(video_idx):
            # Parse MetaData → RL scan index
            meta_raw = _read_vlsd_entry(meta_sd, meta_offsets[rec_i])
            parsed   = _parse_meta(meta_raw)
            if parsed is None:
                skipped += 1
                continue
            rl, _rr = parsed

            # Extract JPEG bytes
            jpeg = _read_vlsd_entry(data_sd, data_offsets[rec_i])
            if len(jpeg) < 4 or jpeg[:2] != b"\xff\xd8":
                skipped += 1
                continue

            if self.compress is not None:
                jpeg = _recompress_jpeg(jpeg, self.compress)

            frames_by_rl[rl] = jpeg

            if (k + 1) % 50 == 0:
                pct = 68 + int(25 * (k + 1) / n_frames)
                p(pct, f"Decoded {k + 1}/{n_frames} frames…")

        if not frames_by_rl:
            raise RuntimeError(
                "All video frames were skipped — could not decode any JPEG data."
            )

        p(94, "Saving cache…")
        rl_sorted = np.array(sorted(frames_by_rl.keys()), dtype=np.int64)
        cache_data = {"rl_sorted": rl_sorted, "frames": frames_by_rl}
        with open(self.cache_path, "wb") as fout:
            pickle.dump(cache_data, fout, protocol=pickle.HIGHEST_PROTOCOL)

        valid = len(frames_by_rl)
        p(100,
          f"Done.  {valid} frames cached"
          + (f"  ({skipped} skipped)" if skipped else "")
          + f"\n→ {self.cache_path}")
        return cache_data


# ---------------------------------------------------------------------------
# VideoWidget
# ---------------------------------------------------------------------------

class VideoWidget(QWidget):
    """
    Detachable tab that displays the APTIV-ORCAS video stream frame nearest
    to the current ScanIndex_L position.

    Public API (called by MainWindow):

      set_mf4_path(path)      – reset widget when a new MF4 file is loaded
      show_frame(scan_rl)     – display nearest frame for given ScanIndex_L
    """

    def __init__(self, parent=None):
        super().__init__(parent)
        self._mf4_path:     Optional[str]         = None
        self._frames_by_rl: dict[int, bytes]      = {}
        self._rl_sorted:    np.ndarray             = np.array([], dtype=np.int64)
        self._current_rl:   int                    = -1
        self._current_jpeg: Optional[bytes]        = None
        self._thread:       Optional[QThread]      = None
        self._worker:       Optional[_ExtractionWorker] = None
        self._build_ui()

    # ------------------------------------------------------------------
    # UI
    # ------------------------------------------------------------------

    def _build_ui(self) -> None:
        root = QVBoxLayout(self)
        root.setContentsMargins(6, 6, 6, 6)
        root.setSpacing(4)

        # ── Top bar: button + status text ─────────────────────────────
        top = QWidget()
        tlay = QHBoxLayout(top)
        tlay.setContentsMargins(0, 0, 0, 0)
        tlay.setSpacing(8)

        self._load_btn = QPushButton("Load Video")
        self._load_btn.setFixedWidth(120)
        self._load_btn.setEnabled(False)
        self._load_btn.setToolTip(
            "Extract video frames from the loaded MF4 file and build a\n"
            "scan-index look-up cache beside the source file.\n"
            "On subsequent uses the cache loads in < 1 s."
        )
        self._load_btn.clicked.connect(self._on_load_clicked)
        tlay.addWidget(self._load_btn)

        self._zoom_combo = QComboBox()
        for _z in ("50%", "100%", "200%", "400%"):
            self._zoom_combo.addItem(_z)
        self._zoom_combo.setCurrentText("100%")
        self._zoom_combo.setFixedWidth(70)
        self._zoom_combo.setToolTip("Display size relative to original (640×480)")
        self._zoom_combo.currentTextChanged.connect(self._on_zoom_changed)
        tlay.addWidget(self._zoom_combo)

        self._status_lbl = QLabel("No MF4 file loaded.")
        self._status_lbl.setStyleSheet("color: #8888aa; font-size: 11px;")
        tlay.addWidget(self._status_lbl, stretch=1)
        root.addWidget(top)

        # ── Stacked area: placeholder vs. image viewer ─────────────────
        self._stk = QStackedWidget()

        # Page 0 – placeholder
        ph = QLabel(
            "No video loaded.\n\n"
            "Click  'Load Video'  to extract frames from the MF4 file.\n"
            "The result is cached on disk for instant re-use."
        )
        ph.setAlignment(Qt.AlignmentFlag.AlignCenter)
        ph.setStyleSheet("color: #555577; font-size: 13px;")
        self._stk.addWidget(ph)

        # Page 1 – image viewer inside a scroll area
        # The label is sized to exactly the scaled image; the scroll area
        # provides panning when the image is larger than the panel.
        # This prevents the infinite-growth feedback loop that occurs when
        # scaling a pixmap to the label's own size triggers a resize event.
        self._scroll = QScrollArea()
        self._scroll.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self._scroll.setWidgetResizable(False)
        self._scroll.setStyleSheet("QScrollArea { background: #000020; border: none; }")
        self._img_lbl = QLabel()
        self._img_lbl.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self._img_lbl.setStyleSheet("background: #000020;")
        self._scroll.setWidget(self._img_lbl)
        self._stk.addWidget(self._scroll)

        root.addWidget(self._stk, stretch=1)

        # ── Sync info bar ─────────────────────────────────────────────
        self._sync_lbl = QLabel("")
        self._sync_lbl.setStyleSheet(
            "color: #8888aa; font-size: 10px; padding: 1px 4px;")
        root.addWidget(self._sync_lbl)

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------

    def set_mf4_path(self, path: Optional[str]) -> None:
        """Reset widget whenever a new original MF4 file is loaded."""
        self._mf4_path     = path
        self._frames_by_rl = {}
        self._rl_sorted    = np.array([], dtype=np.int64)
        self._current_rl   = -1
        self._current_jpeg = None
        self._stk.setCurrentIndex(0)
        self._img_lbl.clear()
        self._sync_lbl.setText("")
        self._load_btn.setText("Load Video")

        if path:
            self._load_btn.setEnabled(True)
            cache = self._cache_path()
            if cache and cache.exists():
                size_mb = cache.stat().st_size / 1_048_576
                self._status_lbl.setText(
                    f"Cache found: {cache.name}  ({size_mb:.0f} MB) — "
                    "click 'Load Video' to use it."
                )
            else:
                self._status_lbl.setText(
                    "Click 'Load Video' to extract frames "
                    "(first run: ~3–4 min  |  subsequent: < 1 s from cache)."
                )
        else:
            self._load_btn.setEnabled(False)
            self._status_lbl.setText("No MF4 file loaded.")

    def show_frame(self, scan_rl: int) -> None:
        """Display the video frame nearest to *scan_rl* (= ScanIndex_L).
        No-op if video has not been loaded yet."""
        if self._rl_sorted.size == 0:
            return

        self._current_rl = scan_rl
        idx = int(np.searchsorted(self._rl_sorted, scan_rl))
        idx = min(idx, len(self._rl_sorted) - 1)

        # Prefer the left neighbour when it is closer
        if idx > 0:
            left_dist  = abs(int(self._rl_sorted[idx - 1]) - scan_rl)
            right_dist = abs(int(self._rl_sorted[idx])     - scan_rl)
            if left_dist <= right_dist:
                idx -= 1

        rl   = int(self._rl_sorted[idx])
        jpeg = self._frames_by_rl.get(rl)
        if jpeg:
            self._current_jpeg = jpeg
            self._display_jpeg(jpeg)
            delta = scan_rl - rl
            self._sync_lbl.setText(
                f"Video RL = {rl}   │   Current ScanIndex_L = {scan_rl}"
                + (f"   │   Δ = {delta:+d} scans" if delta != 0 else "")
            )

    # ------------------------------------------------------------------
    # Internal
    # ------------------------------------------------------------------

    def _cache_path(self) -> Optional[Path]:
        if not self._mf4_path:
            return None
        p = Path(self._mf4_path)
        return p.parent / (p.stem + "_video_cache.pkl")

    def _on_load_clicked(self) -> None:
        if not self._mf4_path:
            return
        if _asammdf is None:
            QMessageBox.critical(
                self, "Missing dependency",
                "asammdf is not installed.\nCannot read MF4 video frames."
            )
            return

        cache = self._cache_path()

        # ── Fast path: load existing cache ────────────────────────────
        if cache and cache.exists():
            self._status_lbl.setText("Loading from cache…")
            try:
                with open(cache, "rb") as f:
                    data = pickle.load(f)
                self._on_extraction_done(data)
                size_mb = cache.stat().st_size / 1_048_576
                self._status_lbl.setText(
                    f"{len(self._frames_by_rl)} frames  │  "
                    f"RL {int(self._rl_sorted[0])} – {int(self._rl_sorted[-1])}  │  "
                    f"cache: {cache.name}  ({size_mb:.0f} MB)"
                )
                return
            except Exception as exc:
                self._status_lbl.setText(
                    f"Cache load failed ({exc}) — re-extracting from MF4…")

        # ── Slow path: full background extraction ─────────────────────
        self._load_btn.setEnabled(False)
        self._status_lbl.setText("Starting video extraction…")

        self._progress_dlg = QProgressDialog(
            "Extracting video frames from MF4…", None, 0, 100, self)
        self._progress_dlg.setWindowModality(Qt.WindowModality.WindowModal)
        self._progress_dlg.setWindowTitle("Load Video")
        self._progress_dlg.setMinimumDuration(0)
        self._progress_dlg.setValue(0)
        self._progress_dlg.show()

        self._thread = QThread()
        self._worker = _ExtractionWorker(
            self._mf4_path, str(cache), compress=40)
        self._worker.moveToThread(self._thread)
        self._thread.started.connect(self._worker.run)
        self._worker.progress.connect(self._on_progress)
        self._worker.finished.connect(self._on_extraction_done)
        self._worker.error.connect(self._on_extraction_error)
        self._worker.finished.connect(self._thread.quit)
        self._worker.error.connect(self._thread.quit)
        self._thread.start()

    def _on_progress(self, pct: int, msg: str) -> None:
        self._progress_dlg.setValue(pct)
        self._progress_dlg.setLabelText(msg)

    def _on_extraction_done(self, data: dict) -> None:
        if hasattr(self, "_progress_dlg"):
            self._progress_dlg.close()

        self._frames_by_rl = data["frames"]
        self._rl_sorted    = data["rl_sorted"]
        self._load_btn.setEnabled(True)
        self._load_btn.setText("Reload Video")
        self._stk.setCurrentIndex(1)

        # If the user was already browsing, show the current frame now.
        if self._current_rl >= 0:
            self.show_frame(self._current_rl)
        else:
            # Show the first frame as a preview
            first_rl = int(self._rl_sorted[0])
            jpeg     = self._frames_by_rl.get(first_rl)
            if jpeg:
                self._current_jpeg = jpeg
                self._display_jpeg(jpeg)

    def _on_extraction_error(self, msg: str) -> None:
        if hasattr(self, "_progress_dlg"):
            self._progress_dlg.close()
        self._load_btn.setEnabled(True)
        self._status_lbl.setText("Extraction failed — see error dialog.")
        QMessageBox.critical(self, "Video extraction failed", msg)

    def _get_zoom_factor(self) -> float:
        return int(self._zoom_combo.currentText().rstrip("%")) / 100.0

    def _on_zoom_changed(self) -> None:
        if self._current_jpeg is not None:
            self._display_jpeg(self._current_jpeg)

    def _display_jpeg(self, jpeg_bytes: bytes) -> None:
        img = QImage.fromData(jpeg_bytes)
        if img.isNull():
            return
        orig_w = img.width()
        orig_h = img.height()
        zoom   = self._get_zoom_factor()
        target_w = max(1, int(orig_w * zoom))
        target_h = max(1, int(orig_h * zoom))
        pix = QPixmap.fromImage(img).scaled(
            target_w, target_h,
            Qt.AspectRatioMode.KeepAspectRatio,
            Qt.TransformationMode.SmoothTransformation,
        )
        # Fix the label to exactly the pixmap size — no feedback loop.
        self._img_lbl.setFixedSize(pix.width(), pix.height())
        self._img_lbl.setPixmap(pix)
