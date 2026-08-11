"""Allowlisted configuration file operations for the KPI page."""

from __future__ import annotations

import json
import os
import shutil
import uuid
import xml.etree.ElementTree as ET
from datetime import datetime, timezone
from pathlib import Path
from typing import Any


CONFIGURATION_SPECS = {
    'config_json': {
        'filename': 'config.json',
        'configured_path': r'C:\Users\ouymc2\Desktop\config_1.json',
        'purpose': 'Interactive Plot configuration.',
        'format': 'json',
        'env': 'HPCC_KPI_CONFIG_JSON',
    },
    'input_output_json': {
        'filename': 'input_output.json',
        'configured_path': r'C:\Users\ouymc2\Desktop\plotly_code2\simg_zmq\KPI\intplot_kpi\InputsInteractivePlot.json',
        'purpose': 'Interactive Plot input and output settings.',
        'format': 'json',
        'env': 'HPCC_KPI_INPUT_OUTPUT_JSON',
    },
    'udp_config_xml': {
        'filename': 'ConfigInteractivePlots.xml',
        'configured_path': r'C:\Users\ouymc2\Desktop\plotly_code2\simg_zmq\KPI\intplot_kpi\ConfigInteractivePlots.xml',
        'purpose': 'UDP KPI Interactive Plot configuration.',
        'format': 'xml',
        'env': 'HPCC_KPI_UDP_CONFIG_XML',
    },
    'can_config_xml': {
        'filename': 'ConfigInteractivePlots_bordnet.xml',
        'configured_path': r'C:\Users\ouymc2\Desktop\plotly_code2\simg_zmq\KPI\intplot_kpi\ConfigInteractivePlots_bordnet.xml',
        'purpose': 'CAN KPI Interactive Plot configuration.',
        'format': 'xml',
        'env': 'HPCC_KPI_CAN_CONFIG_XML',
    },
}


def _project_root() -> Path:
    explicit = (os.environ.get('HPCC_PROJECT_ROOT') or '').strip()
    if explicit:
        return Path(explicit)
    return Path(__file__).resolve().parents[1]


def _candidate_paths(config_id: str) -> list[Path]:
    spec = CONFIGURATION_SPECS[config_id]
    configured = os.environ.get(spec['env'], '').strip() or spec['configured_path']
    candidates = [Path(configured)]
    intplot_root = _project_root() / 'KPI' / 'intplot_kpi'
    candidates.append(intplot_root / spec['filename'])
    if config_id == 'config_json':
        candidates.append(_project_root().parent / 'config_1.json')
    unique = []
    for candidate in candidates:
        if candidate not in unique:
            unique.append(candidate)
    return unique


def _path_for(config_id: str, *, require_exists: bool = False) -> Path:
    candidates = _candidate_paths(config_id)
    for candidate in candidates:
        if candidate.is_file():
            return candidate
    if require_exists:
        raise FileNotFoundError(
            f"{CONFIGURATION_SPECS[config_id]['filename']} was not found. Checked: "
            + ', '.join(str(candidate) for candidate in candidates)
        )
    return candidates[0]


def _validate_content(config_id: str, content: str) -> dict[str, Any]:
    spec = CONFIGURATION_SPECS[config_id]
    if not isinstance(content, str) or not content.strip():
        raise ValueError('Configuration content cannot be empty.')
    try:
        if spec['format'] == 'json':
            json.loads(content)
        else:
            ET.fromstring(content)
    except (json.JSONDecodeError, ET.ParseError) as exc:
        raise ValueError(f"Invalid {spec['format'].upper()} syntax: {exc}") from exc
    return {'valid': True, 'format': spec['format'], 'filename': spec['filename']}


def list_configurations() -> list[dict[str, Any]]:
    entries = []
    for config_id, spec in CONFIGURATION_SPECS.items():
        path = _path_for(config_id)
        exists = path.is_file()
        entry = {
            'id': config_id,
            'filename': spec['filename'],
            'purpose': spec['purpose'],
            'format': spec['format'],
            'configured_path': os.environ.get(spec['env'], '').strip() or spec['configured_path'],
            'path': str(path),
            'exists': exists,
            'size': path.stat().st_size if exists else 0,
            'modified': datetime.fromtimestamp(path.stat().st_mtime, timezone.utc).isoformat() if exists else None,
        }
        entries.append(entry)
    return entries


def get_configuration(config_id: str) -> tuple[dict[str, Any], str]:
    if config_id not in CONFIGURATION_SPECS:
        raise KeyError(f'Unknown configuration: {config_id}')
    path = _path_for(config_id, require_exists=True)
    content = path.read_text(encoding='utf-8')
    metadata = next(item for item in list_configurations() if item['id'] == config_id)
    return metadata, content


def validate_configuration(config_id: str, content: str) -> dict[str, Any]:
    if config_id not in CONFIGURATION_SPECS:
        raise KeyError(f'Unknown configuration: {config_id}')
    result = _validate_content(config_id, content)
    result['path'] = str(_path_for(config_id))
    return result


def configuration_path(config_id: str) -> str:
    if config_id not in CONFIGURATION_SPECS:
        raise KeyError(f'Unknown configuration: {config_id}')
    return str(_path_for(config_id))


def save_configuration(config_id: str, content: str) -> dict[str, Any]:
    validate_configuration(config_id, content)
    path = _path_for(config_id)
    if not path.parent.exists():
        raise FileNotFoundError(f'Configuration directory does not exist: {path.parent}')

    backup_path = None
    if path.exists():
        stamp = datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')
        backup_path = path.with_name(f'{path.name}.bak-{stamp}')
        shutil.copy2(path, backup_path)

    temporary_path = path.with_name(f'.{path.name}.{uuid.uuid4().hex}.tmp')
    try:
        temporary_path.write_text(content, encoding='utf-8', newline='\n')
        os.replace(temporary_path, path)
    finally:
        if temporary_path.exists():
            temporary_path.unlink()

    metadata = next(item for item in list_configurations() if item['id'] == config_id)
    metadata['backup_path'] = str(backup_path) if backup_path else None
    return metadata


def save_uploaded_configuration(config_id: str, data: bytes) -> dict[str, Any]:
    try:
        content = data.decode('utf-8')
    except UnicodeDecodeError as exc:
        raise ValueError('Configuration files must be UTF-8 text.') from exc
    return save_configuration(config_id, content)
