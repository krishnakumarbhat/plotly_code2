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


def build_restart_script(args: argparse.Namespace, stamp: str) -> str:
    remote_root = args.remote_root
    public_host = args.public_host or args.host
    return f"""
#!/usr/bin/env bash
set -euo pipefail
cd {remote_root}
kill_port_listeners() {{
    local port="$1"
    ss -ltnp "sport = :$port" 2>/dev/null | sed -n 's/.*pid=\\([0-9][0-9]*\\).*/\\1/p' | sort -u | while read -r pid; do
        [[ -n "$pid" ]] || continue
        kill "$pid" >/dev/null 2>&1 || true
    done
}}
kill_port_listeners_force() {{
    local port="$1"
    ss -ltnp "sport = :$port" 2>/dev/null | sed -n 's/.*pid=\\([0-9][0-9]*\\).*/\\1/p' | sort -u | while read -r pid; do
        [[ -n "$pid" ]] || continue
        kill -9 "$pid" >/dev/null 2>&1 || true
    done
}}
tmux kill-server >/dev/null 2>&1 || true
kill_port_listeners {args.port}
kill_port_listeners {args.broker_port}
for pattern in 'run_hpcc_stack.sh' 'main_hpcc.sh' 'hpcc_main.pyz' 'main_html.simg' 'Singularity runtime parent' 'gunicorn'; do
    pkill -u {args.user} -f "$pattern" >/dev/null 2>&1 || true
done
sleep 2
kill_port_listeners_force {args.port}
kill_port_listeners_force {args.broker_port}
for pattern in 'run_hpcc_stack.sh' 'main_hpcc.sh' 'hpcc_main.pyz' 'main_html.simg' 'Singularity runtime parent' 'gunicorn'; do
    pkill -9 -u {args.user} -f "$pattern" >/dev/null 2>&1 || true
done
mkdir -p logs runtime_state/main_html/.cache_html/html runtime_state/main_html/.cache_html/video runtime_state/main_html/.cache_html/vlm_cache
if command -v tmux >/dev/null 2>&1; then
  tmux new-session -d -s hpcc 'cd {remote_root} && env HPCC_PUBLIC_HOST={public_host} PORT={args.port} HPCC_BROKER_PORT={args.broker_port} HPCC_PORT_CONFLICT_POLICY=fail ./run_hpcc_stack.sh'
else
  nohup env HPCC_PUBLIC_HOST={public_host} PORT={args.port} HPCC_BROKER_PORT={args.broker_port} HPCC_PORT_CONFLICT_POLICY=fail ./run_hpcc_stack.sh > logs/restart_{stamp}.log 2>&1 < /dev/null &
fi
sleep 8
printf 'PORTS\n'
ss -ltnp | grep -E ':{args.port}|:{args.broker_port}' || true
printf '\nWRAPPER\n'
tail -n 80 run_hpcc_stack.log 2>/dev/null || true
printf '\nBROKER\n'
tail -n 120 logs/hpcc_broker.log 2>/dev/null || true
printf '\nUI\n'
tail -n 120 logs/main_html.log 2>/dev/null || true
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
    parser.add_argument(
        "--bundle-root",
        default=r"C:\Users\ouymc2\Desktop\simg_zmq\simg_sh_hpcc",
    )
    args = parser.parse_args()

    bundle_root = Path(args.bundle_root).resolve()
    uploads = [
        (bundle_root / "main_hpcc.sh", posixpath.join(args.remote_root, "main_hpcc.sh")),
        (bundle_root / "bundle_common.sh", posixpath.join(args.remote_root, "bundle_common.sh")),
        (bundle_root / "hpcc_main.pyz", posixpath.join(args.remote_root, "hpcc_main.pyz")),
        (
            bundle_root / "kpi" / "int_plot" / "intplot_kpi.simg",
            posixpath.join(args.remote_root, "kpi", "int_plot", "intplot_kpi.simg"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "app.py",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "app.py"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "runtime_store.py",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "runtime_store.py"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "utils.py",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "utils.py"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "static" / "js" / "main.js",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "static", "js", "main.js"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "static" / "css" / "style.css",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "static", "css", "style.css"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "templates" / "history.html",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "templates", "history.html"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "templates" / "dashboard.html",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "templates", "dashboard.html"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "templates" / "job_log.html",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "templates", "job_log.html"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "templates" / "tools" / "interactive_plot.html",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "templates", "tools", "interactive_plot.html"),
        ),
        (
            bundle_root / "bundle_src" / "main_html" / "templates" / "tools" / "dc_html.html",
            posixpath.join(args.remote_root, "bundle_src", "main_html", "templates", "tools", "dc_html.html"),
        ),
    ]
    for local_path, _ in uploads:
        if not local_path.exists():
            raise FileNotFoundError(f"Missing upload artifact: {local_path}")

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

            stamp = time.strftime("%Y%m%d_%H%M%S")
            remote_script_path = posixpath.join(args.remote_root, f".copilot_ui_restart_{stamp}.sh")
            ensure_remote_dir(sftp, posixpath.dirname(remote_script_path))
            with sftp.open(remote_script_path, "w") as handle:
                handle.write(build_restart_script(args, stamp))
            sftp.chmod(remote_script_path, 0o775)
        finally:
            sftp.close()

        stdin, stdout, stderr = client.exec_command(f"bash {remote_script_path}", timeout=300)
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
