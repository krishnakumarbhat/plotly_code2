import json
import os
import socket
from typing import Any, Dict


class HpccBrokerClient:
    def __init__(self, host: str = '', port: int = 0, timeout: float = 60.0):
        self.host = host or os.environ.get('HPCC_BROKER_HOST', '127.0.0.1')
        self.port = port or int(os.environ.get('HPCC_BROKER_PORT', '9100'))
        self.timeout = timeout

    def ping(self) -> Dict[str, Any]:
        return self.request({'action': 'ping'})

    def submit_job(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        payload = dict(payload)
        payload['action'] = 'submit'
        return self.request(payload)

    def get_status(self, runtime_job_id: int) -> Dict[str, Any]:
        return self.request({'action': 'status', 'runtime_job_id': runtime_job_id})

    def cancel_job(self, runtime_job_id: int) -> Dict[str, Any]:
        return self.request({'action': 'cancel', 'runtime_job_id': runtime_job_id})

    def request(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        encoded = (json.dumps(payload) + '\n').encode('utf-8')
        with socket.create_connection((self.host, self.port), timeout=self.timeout) as connection:
            connection.sendall(encoded)
            buffer = b''
            while not buffer.endswith(b'\n'):
                chunk = connection.recv(65536)
                if not chunk:
                    break
                buffer += chunk
        if not buffer:
            raise RuntimeError('Broker returned no response')
        response = json.loads(buffer.decode('utf-8').strip())
        if not response.get('ok', False):
            raise RuntimeError(response.get('error', 'Unknown broker error'))
        return response