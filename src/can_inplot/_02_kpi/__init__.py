"""Layer 02_kpi — CAN KPI metrics computation.

Purpose: compute per-scan detection match metrics (overall/precision/recall/
F1/accuracy), manage the preprocessed index cache, and expose latency KPIs.
Inputs : parsed KPI_DataModelStorage pairs from 01_ingest.
Outputs: metric arrays and cache-managed index artifacts.
"""

from can_inplot._02_kpi.storage import KPI_DataModelStorage
from can_inplot._02_kpi.align import align_storage_rows_by_scanindex
from can_inplot._02_kpi.match import DetectionMatcher
from can_inplot._02_kpi.kpi_business import KpiBusiness
from can_inplot._02_kpi.index_cache import IndexCache, verify_index_cache

__all__ = [
    "KPI_DataModelStorage",
    "align_storage_rows_by_scanindex",
    "DetectionMatcher",
    "KpiBusiness",
    "IndexCache",
    "verify_index_cache",
]