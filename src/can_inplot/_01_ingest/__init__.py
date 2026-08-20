"""Layer 01_ingest — HDF5 (edge_hdf) parsing for CAN radar telemetry.

Purpose: decode raw edge_hdf containers into scan-index keyed sensor payloads.
Inputs : path to an HDF5 file produced by the CAN radar toolchain.
Outputs: nested payload dicts per sensor (header/alignment/detection/status/capability).
"""

from can_inplot._01_ingest.schema import (
    SensorSchema,
    SENSOR_ALIASES,
    SENSOR_NAME_MAP,
    DETECTION_SIGNALS,
    HEADER_TIME_KEYS,
    classify_subgroups,
)
from can_inplot._01_ingest.hdf_reader import HdfAttrReader
from can_inplot._01_ingest.hdf_parser import KpiHdfParser

__all__ = [
    "SensorSchema",
    "SENSOR_ALIASES",
    "SENSOR_NAME_MAP",
    "DETECTION_SIGNALS",
    "HEADER_TIME_KEYS",
    "classify_subgroups",
    "HdfAttrReader",
    "KpiHdfParser",
]