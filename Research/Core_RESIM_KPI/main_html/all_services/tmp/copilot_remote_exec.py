from __future__ import annotations

import argparse
import sys

import paramiko


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--password", required=True)
    parser.add_argument("--command", required=True)
    parser.add_argument("--host", default="10.214.45.45")
    parser.add_argument("--user", default="ouymc2")
    parser.add_argument("--timeout", type=int, default=120)
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
        stdin, stdout, stderr = client.exec_command(args.command, timeout=args.timeout)
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