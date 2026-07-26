"""JSON config parser for KPI pipeline."""

import json
from pathlib import Path
from typing import Any, Dict, List


class KpiJsonParser:
    """Parse and normalize KPI JSON configuration."""

    def parse(self, json_path: str) -> Dict[str, Any]:
        path = Path(json_path).expanduser().resolve()
        with path.open("r", encoding="utf-8") as f:
            cfg = json.load(f)

        if not isinstance(cfg, dict):
            raise ValueError("KPI JSON must contain a top-level object")

        cfg["INPUT_HDF"] = self._to_list(cfg.get("INPUT_HDF"))
        cfg["OUTPUT_HDF"] = self._to_list(cfg.get("OUTPUT_HDF"))
        return cfg

    def _to_list(self, value: Any) -> List[str]:
        if value is None:
            return []
        if isinstance(value, str):
            return [value]
        if isinstance(value, list):
            return [str(v) for v in value if isinstance(v, (str, Path))]
        raise ValueError("Expected a string or list of strings for HDF path entries")
