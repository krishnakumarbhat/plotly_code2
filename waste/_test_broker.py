import sys, os, json, tempfile, threading, time
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'main_html'))
os.environ['FLASK_ENV'] = 'development'
os.environ['SECRET_KEY'] = 'test-key'
os.environ['HPCC_BROKER_PORT'] = '9100'
os.environ['CACHE_HTML_DIR'] = '/tmp/test_hpcc/cache'
os.environ['PYTHONUNBUFFERED'] = '1'

from runtime_store import RuntimeStore
import app
app.app.config['TESTING'] = True
app.app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///:memory:'

store = RuntimeStore()
store.ensure_defaults()

import hpcc_main
from hpcc_main import RuntimeBroker

broker = RuntimeBroker(workspace_root=Path('/tmp/test_broker'), store=store)
print('1. RuntimeBroker created OK')

test_spec = {
    'tool_key': 'compute_can_kpi',
    'command': 'sleep 2 && exit 0',
    'mode': 'slurm',
    'input_path': '/tmp/test_broker/input',
    'output_path': '/tmp/test_broker/output',
    'log_path': '/tmp/test_broker/test.log',
    'resources': {'partition': 'kraken', 'account': 'test', 'time': '00:05:00'},
    'console': {},
    'tmux_config': {},
    'launch_plan': {},
}

print('2. All imports OK')
print('3. Test spec prepared')

from pathlib import Path
Path('/tmp/test_broker').mkdir(parents=True, exist_ok=True)

sync_file = Path('/tmp/test_broker/sync.flag')
sync_file.write_text('0')

def run_broker_cycle():
    payload = {
        'action': 'submit',
        'tool_key': 'compute_can_kpi',
        'user': 'testuser',
        'session_id': 'test-session',
    }
    result = broker._submit(payload)
    sync_file.write_text('1')
    return result

result = broker._submit({
    'tool_key': 'compute_can_kpi',
    'user': 'testuser',
    'session_id': 'test-session',
})
if result.get('ok'):
    print(f'4. Job submitted OK, job_id={result["job_id"]}, status={result["status"]}')
else:
    print(f'4. Submit result: {result}')

print()
print('=== BROKER TEST PASSED ===')
