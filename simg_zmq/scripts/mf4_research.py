#!/usr/bin/env python3
"""Inventory and selectively download matched ReSim MF4 input/output pairs."""

from __future__ import annotations

import argparse
import csv
import os
import re
import shlex
import shutil
import stat
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from datetime import date
from pathlib import Path, PurePosixPath

import paramiko


ROOT = Path(__file__).resolve().parents[1]
OUTPUT_ROOT = ROOT.parent / 'Research' / 'Resim_MF4_Sample_20260925'
MAX_PAIRS = 20
MAX_DOWNLOAD_BYTES = 12 * 1024**3
MAX_FIND_DEPTH = 5
MAX_FILES_PER_ROOT = 5000
WORKERS = 2
SFTP_TIMEOUT_SECONDS = 600
SFTP_RETRIES = 3
SFTP_PREFETCH_REQUESTS = 8
SOUTHFIELD_ROOT = '/mnt/usmidet/projects/GPO-IFV7XX/2-Sim'
SOURCE_ROOTS = [
    f'{SOUTHFIELD_ROOT}/REDMKZ/20251028',
    f'{SOUTHFIELD_ROOT}/REDMKZ/20251114',
    f'{SOUTHFIELD_ROOT}/REDMKZ/20251219',
    f'{SOUTHFIELD_ROOT}/s12/CEER_S12_20260701_151002',
    f'{SOUTHFIELD_ROOT}/s12/CEER_S12_20260701_151416',
    f'{SOUTHFIELD_ROOT}/s12/CEER_S12_20260701_151836',
    f'{SOUTHFIELD_ROOT}/MCIP_RESIM/RWUP',
    f'{SOUTHFIELD_ROOT}/Regression_test/a4idj9',
    f'{SOUTHFIELD_ROOT}/Regression_test/hrncac',
    f'{SOUTHFIELD_ROOT}/Regression_test/post_patch_stage_verify',
]
OUTPUT_SUFFIX = re.compile(r'(?i)(?:_rR[^/]*|_r)$')
LOG_INDEX = re.compile(r'_(\d{4})(?:_rR|_r|\.mf4$)', re.IGNORECASE)
DEBUG_INDEX = re.compile(r'_(\d{4})_b05\.mf4$', re.IGNORECASE)
EDGE_TERMS = ('ghost', 'dif', 'bridge')


def _read_local_env() -> dict[str, str]:
    values: dict[str, str] = {}
    env_path = ROOT / '.env'
    if not env_path.is_file():
        raise RuntimeError(f'Local configuration file is missing: {env_path}')
    for raw_line in env_path.read_text(encoding='utf-8').splitlines():
        line = raw_line.strip()
        if not line or line.startswith('#') or '=' not in line:
            continue
        key, value = line.split('=', 1)
        value = value.strip()
        if len(value) >= 2 and value[0] == value[-1] and value[0] in "'\"":
            value = value[1:-1]
        values[key.strip().lower()] = value
    return values


def _connect() -> paramiko.SSHClient:
    values = _read_local_env()
    username = values.get('netid') or os.environ.get('HPCC_NETID', '')
    password = values.get('netid_password') or os.environ.get('HPCC_NETID_PASSWORD', '')
    host = (
        values.get('southfield_host')
        or values.get('southfieldhost')
        or '10.192.224.131'
    )
    port = int(values.get('port', '22'))
    if not username or not password:
        raise RuntimeError('Southfield login settings are incomplete in local configuration.')

    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(
        hostname=host,
        port=port,
        username=username,
        password=password,
        look_for_keys=False,
        allow_agent=False,
        timeout=20,
        banner_timeout=20,
        auth_timeout=20,
    )
    return client


def _inventory_command(search_roots: list[str]) -> str:
    parts = ['set +e']
    for root in search_roots:
        quoted_root = shlex.quote(root)
        parts.append(
            f'if [ -d {quoted_root} ]; then '
            f"find {quoted_root} -maxdepth {MAX_FIND_DEPTH} -type f -iname '*.mf4' "
            f"-printf '%p\\t%s\\t%T@\\n' 2>/dev/null | head -n {MAX_FILES_PER_ROOT}; "
            f'else printf "#MISSING\\t%s\\n" {quoted_root}; fi'
        )
    return '; '.join(parts)


def _output_stem(path: str) -> str | None:
    stem = PurePosixPath(path).stem
    match = OUTPUT_SUFFIX.search(stem)
    if match:
        return stem[:match.start()]
    if any('rR' in part for part in PurePosixPath(path).parent.parts):
        return stem
    return None


def _log_index(path: str) -> int | None:
    match = LOG_INDEX.search(PurePosixPath(path).name)
    return int(match.group(1)) if match else None


def _root_for(path: str, search_roots: list[str]) -> str:
    matching = [root for root in search_roots if path.startswith(root.rstrip('/') + '/') or path == root]
    return max(matching, key=len) if matching else str(PurePosixPath(path).parent)


def _scenario_group(path: str) -> str:
    relative = path.split('/2-Sim/', 1)[-1].split('/')
    depth = 4 if relative[0] == 'USER_DATA' else 2
    return '/'.join(relative[:depth]) if len(relative) > 1 else relative[0]


def _inventory() -> tuple[list[dict], list[dict], list[str], list[str]]:
    search_roots = SOURCE_ROOTS
    client = _connect()
    try:
        _stdin, stdout, stderr = client.exec_command(_inventory_command(search_roots), timeout=180)
        stdout.channel.settimeout(180)
        output = stdout.read().decode('utf-8', errors='replace')
        errors = stderr.read().decode('utf-8', errors='replace').strip()
        status = stdout.channel.recv_exit_status()
        if status not in (0, 1):
            raise RuntimeError(f'Remote inventory command exited with status {status}.')
    finally:
        client.close()

    files: dict[str, dict] = {}
    missing: list[str] = []
    for line in output.splitlines():
        if line.startswith('#MISSING\t'):
            missing.append(line.split('\t', 1)[1])
            continue
        parts = line.split('\t')
        if len(parts) != 3:
            continue
        path, size_text, mtime_text = parts
        try:
            files[path] = {
                'path': path,
                'size': int(size_text),
                'mtime': float(mtime_text),
            }
        except ValueError:
            continue

    grouped: dict[str, dict[str, list[dict]]] = {}
    for entry in files.values():
        path = entry['path']
        output_stem = _output_stem(path)
        if output_stem is not None:
            key = output_stem.casefold()
            grouped.setdefault(key, {'inputs': [], 'outputs': []})['outputs'].append(entry)
        else:
            key = PurePosixPath(path).stem.casefold()
            grouped.setdefault(key, {'inputs': [], 'outputs': []})['inputs'].append(entry)

    pairs = []
    for key, group in grouped.items():
        if not group['inputs'] or not group['outputs']:
            continue
        combinations = []
        for input_entry in group['inputs']:
            for output_entry in group['outputs']:
                common_path = PurePosixPath(os.path.commonpath((input_entry['path'], output_entry['path'])))
                common_depth = len(common_path.parts)
                input_hinted = bool(re.search(r'(?i)(^|/)(input|in|original|source)(/|$)', input_entry['path']))
                output_hinted = bool(re.search(r'(?i)(^|/)(output|out|result|resim)(/|$)', output_entry['path']))
                same_parent = PurePosixPath(input_entry['path']).parent == PurePosixPath(output_entry['path']).parent
                score = common_depth + 2 * same_parent + 2 * input_hinted + 2 * output_hinted
                combinations.append((score, output_entry['mtime'], input_entry, output_entry))
        combinations.sort(key=lambda item: (item[0], item[1], item[2]['mtime']), reverse=True)
        if len(combinations) > 1 and combinations[0][:2] == combinations[1][:2]:
            continue
        _score, _mtime, input_entry, output_entry = combinations[0]
        scenario = _scenario_group(input_entry['path'])
        search_text = f"{scenario}/{input_entry['path']}/{output_entry['path']}".casefold()
        pair = {
            'scenario': scenario,
            'input_path': input_entry['path'],
            'output_path': output_entry['path'],
            'input_bytes': input_entry['size'],
            'output_bytes': output_entry['size'],
            'pair_bytes': input_entry['size'] + output_entry['size'],
            'log_index': _log_index(input_entry['path']),
            'input_mtime': input_entry['mtime'],
            'output_mtime': output_entry['mtime'],
            'edge_tags': ','.join(term for term in EDGE_TERMS if term in search_text),
        }
        pairs.append(pair)

    if errors:
        print('Remote find notes: some inaccessible paths may have been skipped.')
    ordered_pairs = sorted(pairs, key=lambda pair: (pair['scenario'].casefold(), pair['input_path'].casefold()))
    ordered_files = sorted(files.values(), key=lambda entry: entry['path'].casefold())
    return ordered_pairs, ordered_files, missing, search_roots


def _select_pairs(pairs: list[dict], max_pairs: int, byte_budget: int) -> list[dict]:
    groups: dict[str, list[dict]] = {}
    for pair in pairs:
        groups.setdefault(pair['scenario'], []).append(pair)
    for group in groups.values():
        group.sort(key=lambda pair: (pair['log_index'] is None, pair['log_index'] or 0, pair['input_path']))

    selected: list[dict] = []
    seen: set[tuple[str, str]] = set()

    def add(pair: dict) -> bool:
        key = (pair['input_path'], pair['output_path'])
        if key in seen or len(selected) >= max_pairs:
            return False
        if sum(item['pair_bytes'] for item in selected) + pair['pair_bytes'] > byte_budget:
            return False
        seen.add(key)
        selected.append(pair)
        return True

    # Include named edge cases first when they actually occur in the metadata.
    for term in EDGE_TERMS:
        candidate = next((pair for pair in pairs if term in pair['edge_tags']), None)
        if candidate:
            add(candidate)

    # Preserve up to three consecutive log chunks from one scenario when available.
    for group in groups.values():
        indexed = {pair['log_index']: pair for pair in group if pair['log_index'] is not None}
        for index in sorted(indexed):
            if all(index + offset in indexed for offset in range(3)):
                for offset in range(3):
                    add(indexed[index + offset])
                break
        if len(selected) >= max_pairs:
            return selected

    # Add middle and last samples per scenario, then round-robin across scenarios.
    for group in groups.values():
        if group:
            add(group[len(group) // 2])
            add(group[-1])
    cursor = 0
    ordered_groups = list(groups.values())
    while len(selected) < max_pairs and ordered_groups:
        made_progress = False
        for group in ordered_groups:
            if cursor < len(group):
                made_progress = add(group[cursor]) or made_progress
                if len(selected) >= max_pairs:
                    break
        if not made_progress:
            break
        cursor += 1
    return selected


def _select_input_samples(files: list[dict], max_files: int, byte_budget: int, search_roots: list[str]) -> list[dict]:
    groups: dict[str, list[dict]] = {}
    for entry in files:
        path = entry['path']
        if not path.lower().endswith('_b05.mf4'):
            continue
        entry['size'] = int(entry['size'])
        root = _root_for(path, search_roots)
        if root not in SOURCE_ROOTS:
            continue
        match = DEBUG_INDEX.search(PurePosixPath(path).name)
        if not match:
            continue
        entry['log_index'] = int(match.group(1))
        if '/REDMKZ/' in path:
            relative = path.split(root.rstrip('/') + '/', 1)[-1].split('/')
            session = relative[0] if len(relative) > 1 else root
            scenario = root.split('/2-Sim/', 1)[-1]
        else:
            session = PurePosixPath(root).name
            scenario = root.split('/2-Sim/', 1)[-1].split('/', 1)[0]
        entry['scenario'] = scenario
        entry['session'] = session
        groups.setdefault(f'{root}/{session}', []).append(entry)

    for group in groups.values():
        group.sort(key=lambda entry: (entry['log_index'], entry['path'].casefold()))

    selected: list[dict] = []
    seen: set[str] = set()

    def add(entry: dict) -> None:
        if entry['path'] in seen or len(selected) >= max_files:
            return
        if sum(item['size'] for item in selected) + entry['size'] > byte_budget:
            return
        seen.add(entry['path'])
        selected.append(entry)

    ordered_groups = [groups[key] for key in sorted(groups, key=str.casefold)]
    for group in ordered_groups:
        first = next((entry for entry in group if entry['log_index'] == 0), group[0])
        add(first)
    for group in ordered_groups:
        add(group[-1])

    continuation_group = max(ordered_groups, key=lambda group: len(group)) if ordered_groups else []
    continuation_by_index = {entry['log_index']: entry for entry in continuation_group}
    if all(index in continuation_by_index for index in (1, 2, 3)):
        for index in (1, 2, 3):
            add(continuation_by_index[index])

    middle_entries = []
    for group in ordered_groups:
        middle_entries.append(group[len(group) // 2])
    middle_entries.sort(key=lambda entry: (entry['scenario'].casefold(), entry['session'], entry['log_index']))
    for entry in middle_entries:
        add(entry)
        if len(selected) >= max_files:
            break

    cursor = 0
    while len(selected) < max_files:
        made_progress = False
        for group in ordered_groups:
            if cursor < len(group):
                before = len(selected)
                add(group[cursor])
                made_progress = made_progress or len(selected) > before
                if len(selected) >= max_files:
                    break
        if not made_progress:
            break
        cursor += 1
    return selected


def _write_csv(path: Path, rows: list[dict]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    columns = list(dict.fromkeys(key for row in rows for key in row))
    if not columns:
        columns = [
            'scenario', 'input_path', 'output_path', 'input_bytes', 'output_bytes',
            'pair_bytes', 'log_index', 'edge_tags', 'input_mtime', 'output_mtime',
        ]
    with path.open('w', newline='', encoding='utf-8') as output:
        writer = csv.DictWriter(output, fieldnames=columns)
        writer.writeheader()
        writer.writerows(rows)


def inventory(args) -> int:
    pairs, files, missing, search_roots = _inventory()
    free_bytes = shutil.disk_usage(OUTPUT_ROOT.parent).free
    byte_budget = min(args.max_gib * 1024**3, int(free_bytes * 0.60))
    selected = _select_pairs(pairs, min(args.max_pairs, MAX_PAIRS), byte_budget)
    input_samples = []
    if not selected:
        input_samples = _select_input_samples(files, MAX_PAIRS, byte_budget, search_roots)
    OUTPUT_ROOT.mkdir(parents=True, exist_ok=True)
    _write_csv(OUTPUT_ROOT / 'inventory_pairs.csv', pairs)
    _write_csv(OUTPUT_ROOT / 'selected_pairs.csv', selected)
    with (OUTPUT_ROOT / 'selected_input_samples.csv').open('w', newline='', encoding='utf-8') as output:
        columns = ['scenario', 'session', 'log_index', 'path', 'size', 'mtime']
        writer = csv.DictWriter(output, fieldnames=columns)
        writer.writeheader()
        writer.writerows(input_samples)
    with (OUTPUT_ROOT / 'inventory_files.csv').open('w', newline='', encoding='utf-8') as output:
        columns = ['path', 'size', 'mtime', 'root', 'output_like', 'edge_tags']
        writer = csv.DictWriter(output, fieldnames=columns)
        writer.writeheader()
        for entry in files:
            path = entry['path']
            writer.writerow({
                'path': path,
                'size': entry['size'],
                'mtime': entry['mtime'],
                'root': _root_for(path, search_roots),
                'output_like': _output_stem(path) is not None,
                'edge_tags': ','.join(term for term in EDGE_TERMS if term in path.casefold()),
            })
    print(f'Candidate matched pairs: {len(pairs)}')
    print(f'Input-only b05 samples selected: {len(input_samples)}; estimated download: {sum(item["size"] for item in input_samples) / 1024**3:.2f} GiB')
    print(f'MF4 files inventoried: {len(files)}')
    for root in search_roots:
        root_files = [entry for entry in files if _root_for(entry['path'], search_roots) == root]
        outputs = sum(_output_stem(entry['path']) is not None for entry in root_files)
        print(f'  {root.removeprefix(SOUTHFIELD_ROOT + "/")}: {len(root_files)} files, {outputs} output-like')
    print(f'Selected pairs: {len(selected)}; estimated download: {sum(p["pair_bytes"] for p in selected) / 1024**3:.2f} GiB')
    print(f'Local free space before transfer: {free_bytes / 1024**3:.2f} GiB; budget: {byte_budget / 1024**3:.2f} GiB')
    print(f'Missing roots: {len(missing)}')
    for root in missing:
        print(f'  MISSING {root}')
    for pair in selected:
        tags = f' [{pair["edge_tags"]}]' if pair['edge_tags'] else ''
        print(
            f'{pair["scenario"]} log={pair["log_index"] or "?"} '
            f'{pair["pair_bytes"] / 1024**2:.1f} MiB{tags}'
        )
        print(f'  IN  {pair["input_path"]}')
        print(f'  OUT {pair["output_path"]}')
    for sample in input_samples:
        print(f'INPUT {sample["scenario"]}/{sample["session"]} log={sample["log_index"]:04d} {sample["size"] / 1024**2:.1f} MiB')
        print(f'  {sample["path"]}')
    print(f'Manifest: {OUTPUT_ROOT / "selected_input_samples.csv"}')
    return 0 if selected or input_samples else 2


def _download_bucket(files: list[dict]) -> list[str]:
    errors = []
    for item in files:
        destination = OUTPUT_ROOT / item['local_path']
        destination.parent.mkdir(parents=True, exist_ok=True)
        partial = destination.with_name(destination.name + '.part')
        if destination.is_file() and destination.stat().st_size == item['size']:
            continue

        last_error = None
        for attempt in range(SFTP_RETRIES):
            client = None
            sftp = None
            last_error = None
            try:
                client = _connect()
                sftp = client.open_sftp()
                sftp.get_channel().settimeout(SFTP_TIMEOUT_SECONDS)
                remote_size = sftp.stat(item['remote_path']).st_size
                if remote_size != item['size']:
                    raise IOError('remote size no longer matches inventory metadata')
                offset = partial.stat().st_size if partial.is_file() else 0
                if offset > remote_size:
                    partial.unlink()
                    offset = 0
                if offset == 0:
                    sftp.get(
                        item['remote_path'],
                        str(partial),
                        max_concurrent_prefetch_requests=SFTP_PREFETCH_REQUESTS,
                    )
                else:
                    with sftp.open(item['remote_path'], 'rb') as remote:
                        remote.seek(offset)
                        remote.prefetch(
                            file_size=remote_size,
                            max_concurrent_requests=SFTP_PREFETCH_REQUESTS,
                        )
                        with partial.open('ab') as local:
                            shutil.copyfileobj(remote, local, length=1024 * 1024)
                if partial.stat().st_size != item['size']:
                    raise IOError('downloaded size does not match remote metadata')
                os.replace(partial, destination)
            except Exception as exc:
                last_error = exc
                if attempt + 1 < SFTP_RETRIES:
                    time.sleep(2 ** attempt)
            finally:
                if sftp is not None:
                    try:
                        sftp.close()
                    except Exception:
                        pass
                if client is not None:
                    client.close()

            if last_error is None:
                break

        if last_error is not None:
            errors.append(f'{item["remote_path"]}: {type(last_error).__name__}: {last_error}')
    return errors


def download(args) -> int:
    manifest = OUTPUT_ROOT / 'selected_pairs.csv'
    input_manifest = OUTPUT_ROOT / 'selected_input_samples.csv'
    if not manifest.is_file():
        raise RuntimeError('Run inventory first; selected_pairs.csv does not exist.')
    with manifest.open(newline='', encoding='utf-8') as source:
        selected = list(csv.DictReader(source))
    sample_mode = not selected
    if sample_mode:
        if not input_manifest.is_file():
            raise RuntimeError('No matched pairs; run inventory to create the input-only sample manifest.')
        with input_manifest.open(newline='', encoding='utf-8') as source:
            selected = list(csv.DictReader(source))
        if not selected:
            raise RuntimeError('No matched output pairs or primary input samples were selected.')

    free_bytes = shutil.disk_usage(OUTPUT_ROOT).free
    expected_bytes = sum(int(row['size']) for row in selected) if sample_mode else sum(int(row['pair_bytes']) for row in selected)
    if expected_bytes > min(MAX_DOWNLOAD_BYTES, int(free_bytes * 0.60)):
        raise RuntimeError('Selected MF4 files exceed the current transfer disk budget; rerun inventory.')

    download_items = []
    mapping_rows = []
    for pair_number, pair in enumerate(selected, start=1):
        if sample_mode:
            scenario = re.sub(r'[^A-Za-z0-9._-]+', '_', pair['scenario']).strip('._-')[:100] or 'scenario'
            session = re.sub(r'[^A-Za-z0-9._-]+', '_', pair['session']).strip('._-')[:100] or 'session'
            local_path = f'Southfield/{scenario}/{session}/input/{PurePosixPath(pair["path"]).name}'
            download_items.append({
                'remote_path': pair['path'],
                'local_path': local_path,
                'size': int(pair['size']),
            })
            mapping_rows.append({
                'sample': pair_number,
                'scenario': pair['scenario'],
                'session': pair['session'],
                'role': 'input_debug_b05',
                'remote_path': pair['path'],
                'local_path': local_path,
                'bytes': int(pair['size']),
                'log_index': pair['log_index'],
                'edge_tags': '',
            })
            continue
        scenario = re.sub(r'[^A-Za-z0-9._-]+', '_', pair['scenario']).strip('._-')[:100] or 'scenario'
        pair_dir = f'Southfield/{scenario}/pair_{pair_number:02d}'
        for role, key, size_key in (
            ('input', 'input_path', 'input_bytes'),
            ('output', 'output_path', 'output_bytes'),
        ):
            remote_path = pair[key]
            filename = PurePosixPath(remote_path).name
            relative_path = f'{pair_dir}/{role}/{filename}'
            download_items.append({
                'remote_path': remote_path,
                'local_path': relative_path,
                'size': int(pair[size_key]),
            })
            mapping_rows.append({
                'pair': pair_number,
                'scenario': pair['scenario'],
                'role': role,
                'remote_path': remote_path,
                'local_path': relative_path,
                'bytes': int(pair[size_key]),
                'log_index': pair.get('log_index', ''),
                'edge_tags': pair.get('edge_tags', ''),
            })

    buckets = [download_items[index::WORKERS] for index in range(WORKERS)]
    buckets = [bucket for bucket in buckets if bucket]
    errors = []
    with ThreadPoolExecutor(max_workers=len(buckets)) as executor:
        for bucket_errors in executor.map(_download_bucket, buckets):
            errors.extend(bucket_errors)

    _write_csv(OUTPUT_ROOT / 'download_manifest.csv', mapping_rows)
    readme = [
        '# ReSim MF4 research sample',
        '',
        f'- Captured: {date.today().isoformat()}',
        '- Source: Southfield GPO-IFV7XX / 2-Sim',
        '- No generated `rR` output MF4s were present in the three accessible user result runs.',
        '- This is an input-only sample of primary `b05` ReSim debug streams; it is not an input/output pair set.',
        '- Samples span first, middle, last, and available continuation segments across source sessions.',
        '- Only selected MF4 files listed in `download_manifest.csv` were downloaded.',
        '- `download_manifest.csv` records source path, local path, role, and byte size.',
        '',
        f'- Input files selected: {len(selected) if sample_mode else 2 * len(selected)}',
        f'- Expected total: {expected_bytes / 1024**3:.2f} GiB',
        f'- Download errors: {len(errors)}',
    ]
    (OUTPUT_ROOT / 'README.md').write_text('\n'.join(readme) + '\n', encoding='utf-8')
    for error in errors:
        print(f'DOWNLOAD_ERROR {error}')
    completed = sum(
        (OUTPUT_ROOT / item['local_path']).is_file()
        and (OUTPUT_ROOT / item['local_path']).stat().st_size == item['size']
        for item in download_items
    )
    print(f'Files verified: {completed}/{len(download_items)}')
    print(f'Selected records: {len(selected)}; data: {OUTPUT_ROOT}')
    return 1 if errors else 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest='command', required=True)
    inventory_parser = subparsers.add_parser('inventory')
    inventory_parser.add_argument('--max-pairs', type=int, default=MAX_PAIRS)
    inventory_parser.add_argument('--max-gib', type=float, default=12.0)
    subparsers.add_parser('download')
    args = parser.parse_args()
    try:
        return inventory(args) if args.command == 'inventory' else download(args)
    except Exception as exc:
        print(f'ERROR: {type(exc).__name__}: {exc}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())