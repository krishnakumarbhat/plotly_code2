"""HDF5 reader for CAN KPI project.

CAN HDF5 files store data as group attributes (not datasets).
"""

import logging
import re
from typing import Any, Dict, List, Optional, Set

import h5py
import numpy as np

logger = logging.getLogger(__name__)


class HdfAttrReader:
    """Loads a CAN-radar HDF5 file into nested dicts."""

    SENSOR_NAME_MAP = {
        "CEER_FL": "Front Left (SRR_FL)",
        "CEER_FLR": "Front Long Range (FLR)",
        "CEER_FR": "Front Right (SRR_FR)",
        "CEER_RL": "Rear Left (SRR_RL)",
        "CEER_RR": "Rear Right (SRR_RR)",
    }

    def read_hdf_attrs(self, path: str) -> Dict[str, Any]:
        result: Dict[str, Any] = {}
        with h5py.File(path, "r") as f:
            for sensor_id in f.keys():
                sensor_grp = f[sensor_id]
                if not isinstance(sensor_grp, h5py.Group):
                    continue

                sensor_data: Dict[str, Any] = {
                    "friendly_name": self.SENSOR_NAME_MAP.get(sensor_id, sensor_id)
                }
                classified = self._classify_subgroups(list(sensor_grp.keys()))
                for category, names in classified.items():
                    sensor_data[category] = {
                        gname: self._read_group_attrs(sensor_grp[gname])
                        for gname in names
                        if isinstance(sensor_grp[gname], h5py.Group)
                        and len(sensor_grp[gname].attrs.keys()) > 0
                    }
                result[sensor_id] = sensor_data
        return result

    def get_scan_index(self, sensor_data: Dict[str, Any]) -> np.ndarray:
        header_groups = sensor_data.get("header", {})
        for attrs in header_groups.values():
            for key in ("HED_SCAN_INDEX", "HED_LOOK_INDEX"):
                if key in attrs:
                    return attrs[key].astype(int)

        det_groups = sensor_data.get("detection", {})
        for attrs in det_groups.values():
            for val in attrs.values():
                if isinstance(val, np.ndarray) and val.ndim == 1:
                    return np.arange(1, len(val) + 1)
        return np.array([])

    def extract_detection_signals(
        self, sensor_data: Dict[str, Any], allowed_prefixes: Optional[Set[str]] = None
    ) -> Dict[str, Dict[int, np.ndarray]]:
        det_groups = sensor_data.get("detection", {})
        signals: Dict[str, Dict[int, np.ndarray]] = {}
        for attrs in det_groups.values():
            real_attrs = {
                k: v
                for k, v in attrs.items()
                if not k.startswith("id_") and not k.startswith("timestamp_")
            }
            if not real_attrs:
                continue
            for attr_name, arr in real_attrs.items():
                m = re.match(r"^(.+?)_(\d{3})$", attr_name)
                if not m:
                    continue
                prefix, det_idx = m.group(1), int(m.group(2))
                if allowed_prefixes is not None and prefix not in allowed_prefixes:
                    continue
                signals.setdefault(prefix, {})[det_idx] = arr
        return signals

    def extract_alignment_signals(
        self, sensor_data: Dict[str, Any]
    ) -> Dict[str, np.ndarray]:
        return self._extract_flat(sensor_data.get("alignment", {}))

    def extract_header_signals(
        self, sensor_data: Dict[str, Any]
    ) -> Dict[str, np.ndarray]:
        return self._extract_flat(sensor_data.get("header", {}))

    def get_absolute_time_ns(
        self,
        sensor_data: Dict[str, Any],
        header_signals: Optional[Dict[str, np.ndarray]] = None,
        scan_count: Optional[int] = None,
    ) -> np.ndarray:
        """Return per-scan absolute sensor time in nanoseconds.

        Preferred source: ``HED_SENSOR_TIME_STAMP_SEC`` + ``HED_SENSOR_TIME_STAMP_NS``.
        Fallbacks use other plausible header time keys when available.
        """
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

    def _extract_storages(self, parsed: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
        """Extract ``storage`` object per sensor from parsed payload map."""
        out: Dict[str, Any] = {}
        for sensor_id, payload in (parsed or {}).items():
            if not isinstance(payload, dict):
                continue
            storage = payload.get("storage")
            if storage is not None:
                out[sensor_id] = storage
        return out

    def _read_group_attrs(self, grp: h5py.Group) -> Dict[str, np.ndarray]:
        out: Dict[str, np.ndarray] = {}
        for k in grp.attrs.keys():
            v = grp.attrs[k]
            out[k] = v if isinstance(v, np.ndarray) else np.array(v)
        return out

    def _extract_flat(
        self, groups: Dict[str, Dict[str, np.ndarray]]
    ) -> Dict[str, np.ndarray]:
        out: Dict[str, np.ndarray] = {}
        for attrs in groups.values():
            for k, v in attrs.items():
                if not k.startswith("id_") and not k.startswith("timestamp_"):
                    out[k] = v
        return out

    def _classify_subgroups(self, subgroup_names: List[str]) -> Dict[str, List[str]]:
        cats: Dict[str, List[str]] = {
            "detection": [],
            "alignment": [],
            "header": [],
            "status": [],
            "capability": [],
            "other": [],
        }
        for name in sorted(subgroup_names):
            u = name.upper()
            if "DETECTION" in u:
                cats["detection"].append(name)
            elif "ALIGNMENT" in u:
                cats["alignment"].append(name)
            elif "HEADER" in u:
                cats["header"].append(name)
            elif "STATUS" in u:
                cats["status"].append(name)
            elif "CAPABILITY" in u:
                cats["capability"].append(name)
            else:
                cats["other"].append(name)
        return cats
