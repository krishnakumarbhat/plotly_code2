from __future__ import annotations

import argparse
import posixpath
import stat
import sys
import time
from pathlib import Path

import paramiko


DEFAULT_REMOTE_ROOT = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2"


def ensure_remote_dir(sftp: paramiko.SFTPClient, remote_dir: str) -> None:
    normalized = posixpath.normpath(remote_dir)
    pending = []
    while normalized not in {"/", "."}:
        pending.append(normalized)
        normalized = posixpath.dirname(normalized)
    for directory in reversed(pending):
        try:
            sftp.stat(directory)
        except OSError:
            sftp.mkdir(directory)


def upload_file(sftp: paramiko.SFTPClient, local_path: Path, remote_path: str) -> None:
    ensure_remote_dir(sftp, posixpath.dirname(remote_path))
    sftp.put(str(local_path), remote_path)
    mode = local_path.stat().st_mode
    if mode & stat.S_IXUSR or local_path.suffix in {".sh", ".pyz"}:
        sftp.chmod(remote_path, 0o775)


def upload_tree(sftp: paramiko.SFTPClient, local_dir: Path, remote_dir: str) -> None:
    ensure_remote_dir(sftp, remote_dir)
    for path in sorted(local_dir.rglob("*")):
        relative_path = path.relative_to(local_dir)
        remote_path = posixpath.join(remote_dir, relative_path.as_posix())
        if path.is_dir():
            ensure_remote_dir(sftp, remote_path)
            continue
        upload_file(sftp, path, remote_path)


def build_restart_script(args: argparse.Namespace, stamp: str) -> str:
    remote_root = args.remote_root
    public_host = args.public_host or args.host
    return f"""
#!/usr/bin/env bash
set -euo pipefail
cd {remote_root}
kill_port_listeners() {{
    local port="$1"
    ss -ltnp "sport = :$port" 2>/dev/null | sed -n 's/.*pid=\([0-9][0-9]*\).*/\1/p' | sort -u | while read -r pid; do
        [[ -n "$pid" ]] || continue
        kill "$pid" >/dev/null 2>&1 || true
    done
}}
kill_port_listeners_force() {{
    local port="$1"
    ss -ltnp "sport = :$port" 2>/dev/null | sed -n 's/.*pid=\([0-9][0-9]*\).*/\1/p' | sort -u | while read -r pid; do
        [[ -n "$pid" ]] || continue
        kill -9 "$pid" >/dev/null 2>&1 || true
    done
}}
tmux kill-server >/dev/null 2>&1 || true
kill_port_listeners {args.port}
kill_port_listeners {args.broker_port}
kill_port_listeners {args.rag_port}
for pattern in 'run_hpcc_stack.sh' 'main_hpcc.sh' 'hpcc_main.pyz' 'main_html.simg' 'run_rag.sh' 'rag.simg' 'Singularity runtime parent' 'gunicorn'; do
    pkill -u {args.user} -f "$pattern" >/dev/null 2>&1 || true
done
sleep 2
kill_port_listeners_force {args.port}
kill_port_listeners_force {args.broker_port}
kill_port_listeners_force {args.rag_port}
for pattern in 'run_hpcc_stack.sh' 'main_hpcc.sh' 'hpcc_main.pyz' 'main_html.simg' 'run_rag.sh' 'rag.simg' 'Singularity runtime parent' 'gunicorn'; do
    pkill -9 -u {args.user} -f "$pattern" >/dev/null 2>&1 || true
done
mkdir -p logs runtime_state/main_html/.cache_html/html runtime_state/main_html/.cache_html/video runtime_state/main_html/.cache_html/vlm_cache
find logs -maxdepth 1 -type f \( -name '*.log' -o -name '*.pid' -o -name '*.prev' \) -delete
if command -v tmux >/dev/null 2>&1; then
  tmux new-session -d -s hpcc 'cd {remote_root} && env HPCC_PUBLIC_HOST={public_host} PORT={args.port} HPCC_BROKER_PORT={args.broker_port} HPCC_PORT_CONFLICT_POLICY=fail HPCC_AUTO_START_RAG=1 RAG_PORT={args.rag_port} ./run_hpcc_stack.sh'
else
  nohup env HPCC_PUBLIC_HOST={public_host} PORT={args.port} HPCC_BROKER_PORT={args.broker_port} HPCC_PORT_CONFLICT_POLICY=fail HPCC_AUTO_START_RAG=1 RAG_PORT={args.rag_port} ./run_hpcc_stack.sh > logs/restart_{stamp}.log 2>&1 < /dev/null &
fi
sleep 8
printf 'PORTS\n'
ss -ltnp | grep -E ':{args.port}|:{args.broker_port}|:{args.rag_port}' || true
printf '\nWRAPPER\n'
tail -n 80 run_hpcc_stack.log 2>/dev/null || true
printf '\nBROKER\n'
tail -n 120 logs/hpcc_broker.log 2>/dev/null || true
printf '\nUI\n'
tail -n 120 logs/main_html.log 2>/dev/null || true
printf '\nRAG\n'
tail -n 120 logs/rag.log 2>/dev/null || true
"""


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--password", required=True)
    parser.add_argument("--host", default="10.214.45.45")
    parser.add_argument("--user", default="ouymc2")
    parser.add_argument("--remote-root", default=DEFAULT_REMOTE_ROOT)
    parser.add_argument("--public-host", default="10.214.45.45")
    parser.add_argument("--port", type=int, default=5002)
    parser.add_argument("--broker-port", type=int, default=9100)
    parser.add_argument("--rag-port", type=int, default=5100)
    parser.add_argument(
        "--bundle-root",
        default=r"C:\Users\ouymc2\Desktop\simg_zmq\simg_sh_hpcc",
    )
    args = parser.parse_args()

    bundle_root = Path(args.bundle_root).resolve()
    uploads = [
        (bundle_root / "run_hpcc_stack.sh", posixpath.join(args.remote_root, "run_hpcc_stack.sh")),
        (bundle_root / "main_hpcc.sh", posixpath.join(args.remote_root, "main_hpcc.sh")),
        (bundle_root / "bundle_common.sh", posixpath.join(args.remote_root, "bundle_common.sh")),
        (bundle_root / "hpcc_main.pyz", posixpath.join(args.remote_root, "hpcc_main.pyz")),
        (bundle_root / "main_html.simg", posixpath.join(args.remote_root, "main_html.simg")),
        (bundle_root / "rag" / "rag.simg", posixpath.join(args.remote_root, "rag", "rag.simg")),
        (bundle_root / "rag" / "run_rag.sh", posixpath.join(args.remote_root, "rag", "run_rag.sh")),
    ]
    upload_dirs = [
        (bundle_root / "bundle_src" / "main_html", posixpath.join(args.remote_root, "bundle_src", "main_html")),
        (bundle_root / "bundle_src" / "Hyperlink_tool", posixpath.join(args.remote_root, "bundle_src", "Hyperlink_tool")),
    ]
    for local_path, _ in uploads:
        if not local_path.exists():
            raise FileNotFoundError(f"Missing upload artifact: {local_path}")
    for local_dir, _ in upload_dirs:
        if not local_dir.exists():
            raise FileNotFoundError(f"Missing upload directory: {local_dir}")

    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(
        args.host,
        username=args.user,
        password=args.password,
        look_for_keys=False,
        allow_agent=False,
        timeout=20,
    )
    try:
        sftp = client.open_sftp()
        try:
            for local_path, remote_path in uploads:
                upload_file(sftp, local_path, remote_path)
                print(f"uploaded={local_path.name} -> {remote_path}")
            for local_dir, remote_dir in upload_dirs:
                upload_tree(sftp, local_dir, remote_dir)
                print(f"uploaded_tree={local_dir.name} -> {remote_dir}")

            stamp = time.strftime("%Y%m%d_%H%M%S")
            remote_script_path = posixpath.join(args.remote_root, f".copilot_restart_{stamp}.sh")
            ensure_remote_dir(sftp, posixpath.dirname(remote_script_path))
            with sftp.open(remote_script_path, "w") as handle:
                handle.write(build_restart_script(args, stamp))
            sftp.chmod(remote_script_path, 0o775)
        finally:
            sftp.close()

        command = f"bash {remote_script_path}"
        stdin, stdout, stderr = client.exec_command(command, timeout=300)
        sys.stdout.write(stdout.read().decode("utf-8", errors="replace"))
        err = stderr.read().decode("utf-8", errors="replace")
        if err.strip():
            sys.stdout.write("\n--- STDERR ---\n")
            sys.stdout.write(err)
    finally:
        client.close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())