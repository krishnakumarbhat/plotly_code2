"""Low-level HDF5 reader for CAN radar containers.

Purpose: load a CAN-radar HDF5 file into nested dicts keyed by canonical sensor id.
Inputs : path to an edge_hdf container (group-per-sensor layout).
Outputs: dict {sensor_id: {"friendly_name": str, category: {group: {signal: ndarray}}}}.
"""

import logging
from typing import Any, Dict, Optional, Set

import h5py
import numpy as np

from can_inplot._01_ingest.schema import (
    SensorSchema,
    classify_subgroups,
    is_metadata_attr,
    parse_detection_attr,
    SCAN_INDEX_KEYS,
)

logger = logging.getLogger(__name__)


class HdfAttrReader:
    """Reads raw CAN HDF5 payloads into per-sensor nested dicts.

    Detection signals are stored either as group attributes (``DET_RANGE_001``)
    or as datasets; both layouts are merged into one attribute map.
    """

    def read_hdf_attrs(self, path: str) -> Dict[str, Any]:
        """Purpose: parse the whole HDF5 container into sensor payloads.
        Inputs : HDF5 file path.
        Outputs: dict of canonical sensor id -> payload dict; empty on failure."""
        result: Dict[str, Any] = {}
        with h5py.File(path, "r") as f:
            for sensor_id in f.keys():
                sensor_grp = f[sensor_id]
                if not isinstance(sensor_grp, h5py.Group):
                    continue
                canonical = SensorSchema.canonical(sensor_id)
                sensor_data = result.get(canonical, {})
                sensor_data["friendly_name"] = SensorSchema.friendly(canonical)
                for category, names in classify_subgroups(
                    list(sensor_grp.keys())
                ).items():
                    category_payload = sensor_data.setdefault(category, {})
                    for gname in names:
                        if not isinstance(sensor_grp[gname], h5py.Group):
                            continue
                        payload = self._read_group_payload(sensor_grp[gname])
                        if payload:
                            category_payload[gname] = payload
                result[canonical] = sensor_data
        return result

    def get_scan_index(self, sensor_data: Dict[str, Any]) -> np.ndarray:
        """Purpose: recover the per-scan index array for a sensor payload.
        Inputs : sensor payload dict.
        Outputs: int64 scan index array, or empty when unavailable."""
        for attrs in sensor_data.get("header", {}).values():
            for key in SCAN_INDEX_KEYS:
                if key in attrs:
                    return attrs[key].astype(np.int64)
        for attrs in sensor_data.get("detection", {}).values():
            for key, val in attrs.items():
                if is_metadata_attr(key):
                    continue
                if isinstance(val, np.ndarray) and val.ndim == 1:
                    return np.arange(1, len(val) + 1, dtype=np.int64)
        return np.array([], dtype=np.int64)

    def extract_detection_signals(
        self, sensor_data: Dict[str, Any], allowed_prefixes: Optional[Set[str]] = None
    ) -> Dict[str, Dict[int, np.ndarray]]:
        """Purpose: collect detection signals as {prefix: {det_index: ndarray}}.
        Inputs : sensor payload dict and optional allowed signal prefixes.
        Outputs: detection signal map keyed by prefix then detection index."""
        signals: Dict[str, Dict[int, np.ndarray]] = {}
        for attrs in sensor_data.get("detection", {}).values():
            real_attrs = {
                k: v for k, v in attrs.items() if not is_metadata_attr(k)
            }
            if not real_attrs:
                continue
            for attr_name, arr in real_attrs.items():
                parsed = parse_detection_attr(attr_name)
                if parsed is None:
                    continue
                prefix, det_idx = parsed
                if allowed_prefixes is not None and prefix not in allowed_prefixes:
                    continue
                signals.setdefault(prefix, {})[det_idx] = arr
        return signals

    def extract_alignment_signals(
        self, sensor_data: Dict[str, Any]
    ) -> Dict[str, np.ndarray]:
        """Purpose: flatten alignment group signals into one per-scan map.
        Inputs : sensor payload dict.
        Outputs: dict of alignment signal name -> per-scan ndarray."""
        return self._extract_flat(sensor_data.get("alignment", {}))

    def extract_header_signals(
        self, sensor_data: Dict[str, Any]
    ) -> Dict[str, np.ndarray]:
        """Purpose: flatten header group signals into one per-scan map.
        Inputs : sensor payload dict.
        Outputs: dict of header signal name -> per-scan ndarray."""
        return self._extract_flat(sensor_data.get("header", {}))

    def get_absolute_time_ns(
        self,
        sensor_data: Dict[str, Any],
        header_signals: Optional[Dict[str, np.ndarray]] = None,
        scan_count: Optional[int] = None,
    ) -> np.ndarray:
        """Purpose: compute per-scan absolute sensor time in nanoseconds.
        Inputs : sensor payload dict, optional pre-extracted header signals.
        Outputs: int64 ns timestamp array; empty when unavailable."""
        header = header_signals or self.extract_header_signals(sensor_data)
        sec = header.get("HED_SENSOR_TIME_STAMP_SEC")
        ns = header.get("HED_SENSOR_TIME_STAMP_NS")
        if isinstance(sec, np.ndarray) and isinstance(ns, np.ndarray):
            n = min(len(sec), len(ns))
            if scan_count is not None:
                n = min(n, int(scan_count))
            if n > 0:
                sec_i64 = np.rint(sec[:n]).astype(np.int64)
                ns_i64 = np.rint(ns[:n]).astype(np.int64)
                ns_i64 = np.clip(ns_i64, 0, None)
                return sec_i64 * np.int64(1_000_000_000) + ns_i64
        for key in ("HED_TRIGGER_TIME", "HED_SENSOR_TIME_STAMP"):
            arr = header.get(key)
            if isinstance(arr, np.ndarray) and arr.size > 0:
                n = len(arr) if scan_count is None else min(len(arr), int(scan_count))
                if n > 0:
                    return np.rint(arr[:n]).astype(np.int64)
        return np.array([], dtype=np.int64)

    def _read_group_payload(self, grp: h5py.Group) -> Dict[str, np.ndarray]:
        """Purpose: merge group attributes and datasets into one signal map.
        Inputs : h5py group.
        Outputs: dict of signal name -> ndarray."""
        out: Dict[str, np.ndarray] = {}
        for k in grp.attrs.keys():
            v = grp.attrs[k]
            out[k] = v if isinstance(v, np.ndarray) else np.array(v)
        for key in grp.keys():
            child = grp[key]
            if not isinstance(child, h5py.Dataset):
                continue
            try:
                value = child[()]
            except Exception as exc:
                logger.warning("Failed reading dataset %s: %s", child.name, exc)
                continue
            out[key] = value if isinstance(value, np.ndarray) else np.array(value)
        return out

    def _extract_flat(
        self, groups: Dict[str, Dict[str, np.ndarray]]
    ) -> Dict[str, np.ndarray]:
        """Purpose: flatten grouped signal maps while dropping metadata attrs.
        Inputs : dict of group name -> signal map.
        Outputs: flat dict of signal name -> ndarray."""
        out: Dict[str, np.ndarray] = {}
        for attrs in groups.values():
            for k, v in attrs.items():
                if not is_metadata_attr(k):
                    out[k] = v
        return out