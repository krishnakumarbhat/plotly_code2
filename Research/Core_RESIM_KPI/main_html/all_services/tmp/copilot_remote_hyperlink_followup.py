from __future__ import annotations

import posixpath
import time
from pathlib import Path

import paramiko

HOST = "10.214.45.45"
USER = "ouymc2"
PASSWORD = "Zalikapope@202425"
REMOTE_ROOT = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2"
PORT = 5002
BROKER_PORT = 9100
RAG_PORT = 5100
PUBLIC_HOST = "10.214.45.45"

UPLOADS = [
    (
        Path(r"C:\Users\ouymc2\Desktop\simg_zmq\simg_sh_hpcc\bundle_src\main_html\app.py"),
        posixpath.join(REMOTE_ROOT, "bundle_src", "main_html", "app.py"),
    ),
]


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


restart_script = f"""#!/usr/bin/env bash
set -euo pipefail
cd {REMOTE_ROOT}
kill_port_listeners() {{
    local port=\"$1\"
    ss -ltnp \"sport = :$port\" 2>/dev/null | sed -n 's/.*pid=\\([0-9][0-9]*\\).*/\\1/p' | sort -u | while read -r pid; do
        [[ -n \"$pid\" ]] || continue
        kill \"$pid\" >/dev/null 2>&1 || true
    done
}}
kill_port_listeners_force() {{
    local port=\"$1\"
    ss -ltnp \"sport = :$port\" 2>/dev/null | sed -n 's/.*pid=\\([0-9][0-9]*\\).*/\\1/p' | sort -u | while read -r pid; do
        [[ -n \"$pid\" ]] || continue
        kill -9 \"$pid\" >/dev/null 2>&1 || true
    done
}}
tmux kill-server >/dev/null 2>&1 || true
kill_port_listeners {PORT}
kill_port_listeners {BROKER_PORT}
kill_port_listeners {RAG_PORT}
for pattern in 'run_hpcc_stack.sh' 'main_hpcc.sh' 'hpcc_main.pyz' 'main_html.simg' 'run_rag.sh' 'rag.simg' 'Singularity runtime parent' 'gunicorn'; do
    pkill -u {USER} -f \"$pattern\" >/dev/null 2>&1 || true
done
sleep 2
kill_port_listeners_force {PORT}
kill_port_listeners_force {BROKER_PORT}
kill_port_listeners_force {RAG_PORT}
for pattern in 'run_hpcc_stack.sh' 'main_hpcc.sh' 'hpcc_main.pyz' 'main_html.simg' 'run_rag.sh' 'rag.simg' 'Singularity runtime parent' 'gunicorn'; do
    pkill -9 -u {USER} -f \"$pattern\" >/dev/null 2>&1 || true
done
mkdir -p logs runtime_state/main_html/.cache_html/html runtime_state/main_html/.cache_html/video runtime_state/main_html/.cache_html/vlm_cache
find logs -maxdepth 1 -type f \( -name '*.log' -o -name '*.pid' -o -name '*.prev' \) -delete
if command -v tmux >/dev/null 2>&1; then
  tmux new-session -d -s hpcc 'cd {REMOTE_ROOT} && env HPCC_PUBLIC_HOST={PUBLIC_HOST} PORT={PORT} HPCC_BROKER_PORT={BROKER_PORT} HPCC_PORT_CONFLICT_POLICY=fail HPCC_AUTO_START_RAG=1 RAG_PORT={RAG_PORT} ./run_hpcc_stack.sh'
else
  nohup env HPCC_PUBLIC_HOST={PUBLIC_HOST} PORT={PORT} HPCC_BROKER_PORT={BROKER_PORT} HPCC_PORT_CONFLICT_POLICY=fail HPCC_AUTO_START_RAG=1 RAG_PORT={RAG_PORT} ./run_hpcc_stack.sh > logs/restart_hyperlink_auth_followup.log 2>&1 < /dev/null &
fi
sleep 10
printf 'PORTS\n'
ss -ltnp | grep -E ':{PORT}|:{BROKER_PORT}|:{RAG_PORT}' || true
printf '\nUI\n'
tail -n 120 logs/main_html.log 2>/dev/null || true
printf '\nRAG\n'
tail -n 40 logs/rag.log 2>/dev/null || true
"""


client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(HOST, username=USER, password=PASSWORD, look_for_keys=False, allow_agent=False, timeout=20)
try:
    sftp = client.open_sftp()
    try:
        for local_path, remote_path in UPLOADS:
            ensure_remote_dir(sftp, posixpath.dirname(remote_path))
            sftp.put(str(local_path), remote_path)
            print(f"uploaded={local_path.name} -> {remote_path}")
        stamp = time.strftime("%Y%m%d_%H%M%S")
        remote_script_path = posixpath.join(REMOTE_ROOT, f".copilot_hyperlink_auth_followup_{stamp}.sh")
        with sftp.open(remote_script_path, "w") as handle:
            handle.write(restart_script)
        sftp.chmod(remote_script_path, 0o775)
        print(f"restart_script={remote_script_path}")
    finally:
        sftp.close()

    stdin, stdout, stderr = client.exec_command(f"bash {remote_script_path}", timeout=300)
    print(stdout.read().decode("utf-8", errors="replace"))
    err = stderr.read().decode("utf-8", errors="replace")
    if err.strip():
        print("--- STDERR ---")
        print(err)
finally:
    client.close()
