import json
import logging
import os
from typing import Dict, Optional, Tuple

from InteractivePlot.c_data_storage.config_storage import Gen7V1_V2


_ACTIVE_PLOT_CONFIG = Gen7V1_V2
_ACTIVE_PLOT_CONFIG_SOURCE = "InteractivePlot/c_data_storage/config_storage.py"



def _normalize_config(raw_config: dict) -> dict:
    if not isinstance(raw_config, dict):
        raise ValueError("Config content must be a JSON object")

    if "Gen7V1_V2" in raw_config and isinstance(raw_config["Gen7V1_V2"], dict):
        return raw_config["Gen7V1_V2"]

    return raw_config



def _is_valid_plot_config(candidate: dict) -> bool:
    if not isinstance(candidate, dict) or not candidate:
        return False

    has_stream_dict = any(isinstance(value, dict) for value in candidate.values())
    return has_stream_dict



def load_plot_config(config_json_path: Optional[str] = None) -> Tuple[Dict, str]:
    global _ACTIVE_PLOT_CONFIG, _ACTIVE_PLOT_CONFIG_SOURCE

    resolved_path = config_json_path
    if not resolved_path:
        default_path = os.path.join(os.getcwd(), "config.json")
        if os.path.exists(default_path):
            resolved_path = default_path

    if resolved_path:
        try:
            if os.path.getsize(resolved_path) == 0:
                raise ValueError("Config JSON file is empty")

            with open(resolved_path, "r", encoding="utf-8") as cfg_file:
                loaded = json.load(cfg_file)

            normalized = _normalize_config(loaded)
            if not _is_valid_plot_config(normalized):
                raise ValueError("Config JSON is not in expected stream/signal format")

            _ACTIVE_PLOT_CONFIG = normalized
            _ACTIVE_PLOT_CONFIG_SOURCE = resolved_path
            logging.info("Using plot configuration from JSON: %s", resolved_path)
            return _ACTIVE_PLOT_CONFIG, _ACTIVE_PLOT_CONFIG_SOURCE
        except Exception as ex:
            logging.warning(
                "Failed to load plot config from %s (%s). Falling back to config_storage.py",
                resolved_path,
                ex,
            )

    _ACTIVE_PLOT_CONFIG = Gen7V1_V2
    _ACTIVE_PLOT_CONFIG_SOURCE = "InteractivePlot/c_data_storage/config_storage.py"
    return _ACTIVE_PLOT_CONFIG, _ACTIVE_PLOT_CONFIG_SOURCE



def get_plot_config() -> Dict:
    return _ACTIVE_PLOT_CONFIG



def get_plot_config_source() -> str:
    return _ACTIVE_PLOT_CONFIG_SOURCE
