from __future__ import annotations

import argparse
import sys

import paramiko


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--password", required=True)
    parser.add_argument("--host", default="10.214.45.45")
    parser.add_argument("--user", default="ouymc2")
    parser.add_argument(
        "--remote-root",
        default="/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2",
    )
    parser.add_argument("--port", type=int, default=5002)
    parser.add_argument("--broker-port", type=int, default=9100)
    parser.add_argument("--rag-port", type=int, default=5100)
    args = parser.parse_args()

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
        command = f"""
set -euo pipefail
cd {args.remote_root}
printf 'PWD=%s\n' "$(pwd)"
printf '\nRELEVANT FILES\n'
find . -maxdepth 2 \( -name 'InputsInteractivePlot*.json' -o -name 'ConfigInteractivePlots*.xml' -o -name 'kpi.json' \) -type f | sort || true
printf '\nLAUNCHER RUNTIME BLOCK\n'
sed -n '100,150p' main_hpcc.sh || true
printf '\nLAUNCHER UI BLOCK\n'
sed -n '340,350p' main_hpcc.sh || true
printf 'PORTS\n'
ss -ltnp | grep -E ':{args.port}|:{args.broker_port}|:{args.rag_port}' || true
printf '\nRECENT LOGS\n'
ls -1t logs/restart_*.log 2>/dev/null | head -n 3 || true
latest=$(ls -1t logs/restart_*.log 2>/dev/null | head -n 1 || true)
if [[ -n "$latest" ]]; then
  printf '\nLATEST=%s\n' "$latest"
  tail -n 120 "$latest" || true
fi
printf '\nDATABASE STATUS\n'
python3 - <<'PY'
import json
import os
import sqlite3

db_path = 'runtime_state/main_html/.cache_html/hpc_tools_dev.db'
if not os.path.exists(db_path):
    print('missing db: ' + db_path)
    raise SystemExit(0)

conn = sqlite3.connect(db_path)
conn.row_factory = sqlite3.Row
cur = conn.cursor()

print('runtime_jobs:')
for row in cur.execute(
    'SELECT id, tool_key, status, input_path, output_path, log_path, error_message, created_at, completed_at FROM runtime_jobs ORDER BY id DESC LIMIT 5'
):
    print(json.dumps(dict(row), ensure_ascii=False, default=str))

print('job_history:')
for row in cur.execute(
    'SELECT id, tool_name, input_path, output_path, output_log_path, status, error_message, created_at, completed_at FROM job_history ORDER BY id DESC LIMIT 5'
):
    print(json.dumps(dict(row), ensure_ascii=False, default=str))

print('recent execution logs:')
for row in cur.execute(
    'SELECT id, log_path, output_path FROM runtime_jobs WHERE log_path IS NOT NULL AND log_path != "" ORDER BY id DESC LIMIT 3'
):
    print('LOG[' + str(row['id']) + ']=' + row['log_path'])
    if os.path.exists(row['log_path']):
        log_dir = os.path.dirname(row['log_path'])
        print('log_dir=' + log_dir)
        for name in sorted(os.listdir(log_dir)):
            print('  file=' + name)
        print('--- tail start ---')
        with open(row['log_path'], 'r', encoding='utf-8', errors='replace') as fp:
            lines = fp.readlines()[-80:]
        print(''.join(lines))
        print('--- tail end ---')
    else:
        print('missing log file')
PY
printf '\nMAIN HTML LOG\n'
tail -n 120 logs/main_html.log 2>/dev/null || true
printf '\nBROKER LOG\n'
tail -n 120 logs/hpcc_broker.log 2>/dev/null || true
printf '\nRAG LOG\n'
tail -n 120 logs/rag.log 2>/dev/null || true
printf '\nPROCESSES\n'
ps -ef | grep -E 'main_hpcc.sh|hpcc_main.pyz|main_html.simg|run_rag.sh|rag.simg' | grep -v grep || true
"""
        stdin, stdout, stderr = client.exec_command(command, timeout=120)
        out = stdout.read().decode("utf-8", errors="replace")
        err = stderr.read().decode("utf-8", errors="replace")
        sys.stdout.write(out)
        if err.strip():
            sys.stdout.write("\n--- STDERR ---\n")
            sys.stdout.write(err)
    finally:
        client.close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())