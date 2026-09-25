from __future__ import annotations

import argparse
import os
import posixpath
import shlex
import stat
import sys
import time
import traceback
from pathlib import Path

import paramiko


DEFAULT_BUNDLE_ROOT = Path(r"C:\Users\ouymc2\Desktop\simg_zmq\simg_sh_hpcc")
SKIP_DIR_NAMES = {'__pycache__', '.pytest_cache', 'logs', 'runtime_state', 'runs'}
SKIP_FILE_SUFFIXES = ('.log', '.pid', '.prev')


def resolve_bundle_root(args: argparse.Namespace) -> Path:
  if args.local_bundle_root:
    return Path(args.local_bundle_root).resolve()
  for legacy_path in (args.local_file, args.local_broker, args.local_ui_image):
    if legacy_path:
      return Path(legacy_path).resolve().parent
  return DEFAULT_BUNDLE_ROOT.resolve()


def ensure_remote_dir(sftp: paramiko.SFTPClient, remote_dir: str) -> None:
  normalized = posixpath.normpath(remote_dir)
  pending = []
  while normalized not in {'/', '.'}:
    pending.append(normalized)
    normalized = posixpath.dirname(normalized)
  for directory in reversed(pending):
    try:
      sftp.stat(directory)
    except OSError:
      sftp.mkdir(directory)


def should_skip(relative_path: Path) -> bool:
  if any(part in SKIP_DIR_NAMES for part in relative_path.parts):
    return True
  if relative_path.name.endswith(SKIP_FILE_SUFFIXES):
    return True
  return False


def iter_bundle_files(bundle_root: Path):
  for path in sorted(bundle_root.rglob('*')):
    if path.is_dir():
      continue
    relative_path = path.relative_to(bundle_root)
    if should_skip(relative_path):
      continue
    yield path, relative_path


def upload_bundle(sftp: paramiko.SFTPClient, bundle_root: Path, remote_stage: str) -> tuple[int, int]:
  ensure_remote_dir(sftp, remote_stage)
  uploaded_count = 0
  uploaded_bytes = 0

  for local_path, relative_path in iter_bundle_files(bundle_root):
    remote_path = posixpath.join(remote_stage, relative_path.as_posix())
    ensure_remote_dir(sftp, posixpath.dirname(remote_path))
    sftp.put(str(local_path), remote_path)
    mode = local_path.stat().st_mode
    if mode & stat.S_IXUSR or local_path.suffix in {'.sh', '.pyz'}:
      sftp.chmod(remote_path, 0o775)
    uploaded_count += 1
    uploaded_bytes += local_path.stat().st_size
  return uploaded_count, uploaded_bytes


def build_remote_restart_script(args: argparse.Namespace, remote_stage: str, stamp: str) -> str:
  remote_root = shlex.quote(args.remote_root)
  remote_stage_q = shlex.quote(remote_stage)
  public_host = shlex.quote(args.host)
  return f"""
set -euo pipefail
cd {remote_root}
mkdir -p logs
if [[ -f main_hpcc.sh ]]; then
  cp -p main_hpcc.sh main_hpcc.sh.bak_{stamp}
fi
cp -a {remote_stage_q}/. {remote_root}/
rm -rf {remote_stage_q}
chmod +x main_hpcc.sh hpcc_main.pyz bundle_common.sh cleanup_memory.sh kpi_runtime_launcher.sh run_hpcc_stack.sh || true
find kpi rag -type f -name '*.sh' -exec chmod +x {{}} + >/dev/null 2>&1 || true
for port in {args.port} {args.broker_port}; do
  if command -v lsof >/dev/null 2>&1; then
  pids=$(lsof -tiTCP:$port -sTCP:LISTEN 2>/dev/null || true)
  if [[ -n "$pids" ]]; then
    kill $pids >/dev/null 2>&1 || true
    sleep 1
    kill -9 $pids >/dev/null 2>&1 || true
  fi
  fi
done
nohup env HPCC_PUBLIC_HOST={public_host} PORT={args.port} HPCC_BROKER_PORT={args.broker_port} ./main_hpcc.sh > logs/restart_{stamp}.log 2>&1 < /dev/null &
echo $! > logs/restart_{stamp}.pid
for _ in $(seq 1 60); do
  if ss -ltn | grep -q ':{args.port} ' && ss -ltn | grep -q ':{args.broker_port} '; then
  break
  fi
  sleep 2
done
ss -ltnp | grep -E ':{args.port}|:{args.broker_port}' || true
printf '\n===== restart log =====\n'
tail -n 120 logs/restart_{stamp}.log || true
"""


def main() -> int:
  parser = argparse.ArgumentParser()
  parser.add_argument('--password', required=True)
  parser.add_argument('--local-bundle-root', default=None)
  parser.add_argument('--local-file', default=None)
  parser.add_argument('--local-broker', default=None)
  parser.add_argument('--local-ui-image', default=None)
  parser.add_argument(
    '--remote-root',
    default='/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2',
  )
  parser.add_argument('--host', default='10.214.45.45')
  parser.add_argument('--user', default='ouymc2')
  parser.add_argument('--port', type=int, default=5002)
  parser.add_argument('--broker-port', type=int, default=9100)
  args = parser.parse_args()

  bundle_root = resolve_bundle_root(args)
  if not bundle_root.is_dir():
    raise FileNotFoundError(f'Local bundle root not found: {bundle_root}')

  stamp = time.strftime('%Y%m%d_%H%M%S')
  remote_stage = posixpath.join(args.remote_root, f'.copilot_bundle_sync_{stamp}')

  print(f'LOCAL_BUNDLE_ROOT={bundle_root}')
  print(f'REMOTE_ROOT={args.remote_root}')
  print(f'REMOTE_STAGE={remote_stage}')
  print(f'STAMP={stamp}')

  client = paramiko.SSHClient()
  client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
  try:
    client.connect(
      args.host,
      username=args.user,
      password=args.password,
      look_for_keys=False,
      allow_agent=False,
      timeout=20,
    )
    print('CONNECTED=1')

    sftp = client.open_sftp()
    try:
      uploaded_count, uploaded_bytes = upload_bundle(sftp, bundle_root, remote_stage)
      print(f'UPLOAD_COUNT={uploaded_count}')
      print(f'UPLOAD_BYTES={uploaded_bytes}')

      remote_script = build_remote_restart_script(args, remote_stage, stamp)
      print('REMOTE_EXEC=1')
      stdin, stdout, stderr = client.exec_command(remote_script, timeout=600)
      out = stdout.read().decode('utf-8', errors='replace')
      err = stderr.read().decode('utf-8', errors='replace')
      print('REMOTE_EXEC_DONE=1')
      sys.stdout.write(out)
      if err.strip():
        sys.stdout.write('\n--- STDERR ---\n')
        sys.stdout.write(err)
    finally:
      sftp.close()
  except Exception:
    traceback.print_exc()
    return 1
  finally:
    client.close()

  return 0


if __name__ == '__main__':
  raise SystemExit(main())