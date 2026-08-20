"""Sensor schema for CAN radar HDF5 containers.

Purpose: centralize signal classification, sensor aliases, and detection
signal names so that both parser and KPI layers agree on the same vocabulary.
Inputs : none (module-level constants).
Outputs: classification and naming utilities.
"""

from typing import Dict, List, Optional

# AGENT INSTRUCTION: keep detection signal list in sync with producer DBC.
DETECTION_SIGNALS: List[str] = [
    "DET_RANGE",
    "DET_RANGE_VELOCITY",
    "DET_AZIMUTH",
    "DET_ELEVATION",
    "DET_RCS",
    "DET_SNR",
]

HEADER_TIME_KEYS: List[str] = [
    "HED_SENSOR_TIME_STAMP_SEC",
    "HED_SENSOR_TIME_STAMP_NS",
]

SCAN_INDEX_KEYS: List[str] = ["HED_SCAN_INDEX", "HED_LOOK_INDEX"]

SENSOR_ALIASES: Dict[str, str] = {
    "CEER_FC": "CEER_FLR",
    "FC": "CEER_FLR",
    "FLR": "CEER_FLR",
}

SENSOR_NAME_MAP: Dict[str, str] = {
    "CEER_FL": "Front Left (SRR_FL)",
    "CEER_FLR": "Front Long Range (FLR)",
    "CEER_FR": "Front Right (SRR_FR)",
    "CEER_RL": "Rear Left (SRR_RL)",
    "CEER_RR": "Rear Right (SRR_RR)",
}

SENSOR_ORDER: List[str] = ["CEER_FL", "CEER_FLR", "CEER_FR", "CEER_RL", "CEER_RR"]

FRIENDLY: Dict[str, str] = {
    "CEER_FL": "SRR / FL",
    "CEER_FLR": "SRR / FLR",
    "CEER_FR": "SRR / FR",
    "CEER_RL": "SRR / RL",
    "CEER_RR": "SRR / RR",
}

# Subgroup names that may appear under a sensor group, mapped to a category.
_SUBGROUP_CATEGORIES: Dict[str, str] = {
    "detection": "detection",
    "alignment": "alignment",
    "header": "header",
    "status": "status",
    "capability": "capability",
}


class SensorSchema:
    """Static accessor for sensor naming and signal classification."""

    @staticmethod
    def canonical(sensor_id: str) -> str:
        """Purpose: map producer-side sensor ids to canonical ids.
        Inputs : raw sensor id string.
        Outputs: canonical sensor id (aliases resolved)."""
        return SENSOR_ALIASES.get(sensor_id.upper(), sensor_id)

    @staticmethod
    def friendly(sensor_id: str) -> str:
        """Purpose: human-readable sensor label for plots and tables.
        Inputs : canonical sensor id.
        Outputs: friendly display name."""
        return SENSOR_NAME_MAP.get(sensor_id, sensor_id)


def classify_subgroups(subgroup_names: List[str]) -> Dict[str, List[str]]:
    """Purpose: bucket subgroup names into canonical payload categories.
    Inputs : list of subgroup names under a sensor group.
    Outputs: mapping category -> list of subgroup names."""
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
        matched = False
        for token, category in _SUBGROUP_CATEGORIES.items():
            if token.upper() in u:
                cats[category].append(name)
                matched = True
                break
        if not matched:
            cats["other"].append(name)
    return cats


def is_metadata_attr(name: str) -> bool:
    """Purpose: distinguish signal attributes from metadata attrs (id_/timestamp_).
    Inputs : attribute name.
    Outputs: True when the attribute is metadata and must be skipped."""
    return name.startswith("id_") or name.startswith("timestamp_")


def parse_detection_attr(name: str) -> Optional[tuple[str, int]]:
    """Purpose: split a detection attribute name into (signal prefix, detection index).
    Inputs : attribute name, e.g. 'DET_RANGE_017'.
    Outputs: (prefix, index) tuple or None when the name is not a detection attr."""
    import re

    m = re.match(r"^(.+?)_(\d{3})$", name)
    if not m:
        return None
    return m.group(1), int(m.group(2))