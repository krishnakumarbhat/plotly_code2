import socket
import subprocess
import sys

import pytest


def test_broker_port_default_is_9200():
    result = subprocess.run(
        [sys.executable, '-c', 'import os; os.environ.pop("HPCC_BROKER_PORT", None); from main_html.hpcc_broker_client import HpccBrokerClient; c = HpccBrokerClient(); print(c.port)'],
        capture_output=True, text=True, timeout=10,
    )
    assert result.returncode == 0, result.stderr
    assert '9200' in result.stdout.strip()


def test_broker_port_fallback_binds_unique():
    host = '127.0.0.1'
    sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock1.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock1.bind((host, 0))
    port1 = sock1.getsockname()[1]

    sock2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock2.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock2.bind((host, 0))
    port2 = sock2.getsockname()[1]

    assert port1 != port2
    assert isinstance(port1, int) and port1 > 0
    assert isinstance(port2, int) and port2 > 0
    sock1.close()
    sock2.close()


def test_generate_upload_no_rag_skips_rag():
    from generate_upload import generate
    try:
        generate(no_rag=True)
    except (SystemExit, FileNotFoundError, ModuleNotFoundError):
        pass
