from __future__ import annotations

import argparse
import json
import sys
from textwrap import dedent

import paramiko


DEFAULT_REMOTE_ROOT = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2"


MODE_CONFIG = {
    "can": {
        "output_dir": "./out/copilot_can_direct",
        "log_path": "/tmp/copilot_can_direct.log",
        "command": [
            "bash",
            "./kpi/can/run_can.sh",
            "./InputsInteractivePlot2.json",
            "./out/copilot_can_direct",
        ],
        "extra_logs": ["can_kpi.log", "interactive_plot.log", "udp_kpi_server.log"],
    },
    "udp": {
        "output_dir": "./out/copilot_udp_direct",
        "log_path": "/tmp/copilot_udp_direct.log",
        "command": [
            "bash",
            "./kpi/udp/run_udp.sh",
            "./InputsInteractivePlot1.json",
            "./out/copilot_udp_direct",
        ],
        "extra_logs": ["udp_kpi_server.log", "can_kpi.log", "interactive_plot.log"],
    },
    "udp-intplot": {
        "output_dir": "./out/copilot_udp_intplot_direct",
        "log_path": "/tmp/copilot_udp_intplot_direct.log",
        "command": [
            "bash",
            "./kpi_runtime_launcher.sh",
            "--target",
            "udp_kpi",
            "--source-target",
            "udp_kpi",
            "--interactive-mode",
            "enabled",
            "--input-mode",
            "json",
            "--json-path",
            "./InputsInteractivePlot1.json",
            "--config-xml",
            "./ConfigInteractivePlots.xml",
            "--output-dir",
            "./out/copilot_udp_intplot_direct",
            "--port",
            "5562",
        ],
        "extra_logs": ["interactive_plot.log", "udp_kpi_server.log", "can_kpi.log"],
    },
}


def build_remote_command(mode: str, remote_root: str, detach: bool) -> str:
    config = MODE_CONFIG[mode]
    payload = {
        "remote_root": remote_root,
        "output_dir": config["output_dir"],
        "log_path": config["log_path"],
        "command": config["command"],
        "extra_logs": config["extra_logs"],
    }
    payload_json = json.dumps(payload)
    return dedent(
        f"""
        python3 - <<'PY'
        import json
        import os
        import pathlib
        import shutil
        import subprocess
        import sys

        payload = json.loads({payload_json!r})
        remote_root = payload['remote_root']
        output_dir = payload['output_dir']
        log_path = payload['log_path']
        command = payload['command']
        extra_logs = payload['extra_logs']

        os.chdir(remote_root)
        shutil.rmtree(output_dir, ignore_errors=True)
        try:
            pathlib.Path(log_path).unlink()
        except FileNotFoundError:
            pass
        pathlib.Path(output_dir).mkdir(parents=True, exist_ok=True)

        with open(log_path, 'w', encoding='utf-8', errors='replace') as fp:
            if {detach!r}:
                process = subprocess.Popen(
                    command,
                    stdout=fp,
                    stderr=subprocess.STDOUT,
                    start_new_session=True,
                )
                print(f'PID={{process.pid}}')
                print(f'LOG={{log_path}}')
                print('COMMAND=' + ' '.join(command))
                sys.exit(0)
            rc = subprocess.run(command, stdout=fp, stderr=subprocess.STDOUT).returncode

        print(f'RC={{rc}}')
        print(f'LOG={{log_path}}')
        print('COMMAND=' + ' '.join(command))

        log_text = pathlib.Path(log_path).read_text(encoding='utf-8', errors='replace')
        print('MAIN_LOG_TAIL_START')
        print(log_text[-16000:])
        print('MAIN_LOG_TAIL_END')

        files = sorted(str(path) for path in pathlib.Path(output_dir).rglob('*') if path.is_file())
        print('OUTPUT_FILES_START')
        for path in files[:80]:
            print(path)
        print('OUTPUT_FILES_END')

        logs_dir = pathlib.Path(output_dir) / 'logs'
        if logs_dir.exists():
            for name in extra_logs:
                candidate = logs_dir / name
                if not candidate.exists():
                    continue
                print(f'EXTRA_LOG_START {{candidate}}')
                print(candidate.read_text(encoding='utf-8', errors='replace')[-16000:])
                print(f'EXTRA_LOG_END {{candidate}}')

        sys.exit(rc)
        PY
        """
    ).strip()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--password", required=True)
    parser.add_argument("--mode", choices=sorted(MODE_CONFIG), required=True)
    parser.add_argument("--host", default="10.214.45.45")
    parser.add_argument("--user", default="ouymc2")
    parser.add_argument("--remote-root", default=DEFAULT_REMOTE_ROOT)
    parser.add_argument("--timeout", type=int, default=3600)
    parser.add_argument("--detach", action="store_true")
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
        stdin, stdout, stderr = client.exec_command(
            build_remote_command(args.mode, args.remote_root, args.detach),
            timeout=args.timeout,
        )
        stdout.channel.settimeout(None)
        stderr.channel.settimeout(None)
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