import json

import pytest

from main_html.runtime_store import RuntimeStore
from main_html.temp_dir import hpcc_main
from main_html import app as main_app


class _FakeStore:
    def __init__(self):
        self.request_payload = None

    def get_tool(self, _tool_key):
        return {'tool_key': 'udp_kpi'}

    def create_job(self, **kwargs):
        self.request_payload = kwargs['request_payload']
        return 17

    def append_event(self, *_args, **_kwargs):
        return None


def test_valid_partitions_rejects_sinfo_help_text(monkeypatch):
    result = type('Result', (), {
        'returncode': 0,
        'stdout': 'USAGE: sinfo [args...]\nplcyf-com*\ndefq\ninvalid partition text\n',
    })()
    monkeypatch.setattr(hpcc_main.subprocess, 'run', lambda *_args, **_kwargs: result)

    assert hpcc_main._valid_partitions() == ['plcyf-com', 'defq']


@pytest.mark.parametrize(
    ('raw_value', 'expected'),
    [
        ('  /mnt/project/input.h5  ', '/mnt/project/input.h5'),
        ('"/mnt/project/path with spaces/input.h5"', '/mnt/project/path with spaces/input.h5'),
        ("'/net/project/input.h5'", '/net/project/input.h5'),
        ('"\'/mnt/project/input.h5\'"', '/mnt/project/input.h5'),
    ],
)
def test_flask_path_normalization_removes_only_balanced_outer_quotes(raw_value, expected):
    assert main_app._normalize_submitted_path(raw_value) == expected


def test_submit_does_not_persist_cluster_password(monkeypatch):
    store = _FakeStore()
    broker = hpcc_main.RuntimeBroker.__new__(hpcc_main.RuntimeBroker)
    broker.store = store
    monkeypatch.setattr(
        broker,
        '_build_spec',
        lambda _tool, _payload: {
            'console': {},
            'mode': 'hdf',
            'input_path': '/net/input.h5',
            'output_path': '/net/output',
            'log_path': '/tmp/runtime.log',
            'command': ['true'],
            'resources': {'scheduler': 'slurm'},
        },
    )
    monkeypatch.setattr(broker, '_launch', lambda *_args, **_kwargs: None)

    result = broker._submit({
        'tool_key': 'udp_kpi',
        'user': 'tester',
        'user_password': 'do-not-store',
        'paths': {'input_hdf': '/net/input.h5'},
    })

    assert result['ok'] is True
    assert 'user_password' not in store.request_payload
    assert json.dumps(store.request_payload).find('do-not-store') == -1


def test_runtime_store_redacts_password_from_direct_and_existing_rows(tmp_path):
    db_path = tmp_path / 'runtime.db'
    store = RuntimeStore(str(db_path))
    runtime_job_id = store.create_job(
        tool_key='udp_kpi',
        requested_by='tester',
        session_id='session',
        mode='hdf',
        input_path='/net/input.h5',
        output_path='/net/output',
        log_path=str(tmp_path / 'runtime.log'),
        command=['true'],
        resources={'scheduler': 'slurm'},
        request_payload={'paths': {}, 'user_password': 'do-not-store'},
    )
    with store._connect() as connection:
        stored = connection.execute(
            'SELECT request_json FROM runtime_jobs WHERE id = ?',
            (runtime_job_id,),
        ).fetchone()['request_json']
        connection.execute(
            'UPDATE runtime_jobs SET request_json = ? WHERE id = ?',
            (json.dumps({'paths': {}, 'user_password': 'legacy-secret'}), runtime_job_id),
        )

    assert 'do-not-store' not in stored
    RuntimeStore(str(db_path))
    with store._connect() as connection:
        migrated = connection.execute(
            'SELECT request_json FROM runtime_jobs WHERE id = ?',
            (runtime_job_id,),
        ).fetchone()['request_json']
    assert 'legacy-secret' not in migrated
    assert 'user_password' not in migrated


@pytest.mark.parametrize(
    ('text', 'expected'),
    [
        ('--partition=USAGE: sinfo [args...]', 'partition discovery'),
        ('srun: error: Invalid account or account/partition combination', 'Invalid Slurm'),
        ('Missing file: /net/input.h5', 'missing'),
        ('kernel reported a bad superblock; image is corrupted', 'corrupted'),
        ('bash: module: command not found', 'module'),
        ('Traceback (most recent call last):', 'Python runtime'),
    ],
)
def test_failure_classifier_produces_actionable_messages(text, expected):
    assert expected.lower() in RuntimeStore.classify_failure_text(text, 1).lower()


def test_generic_failure_message_is_still_actionable():
    message = RuntimeStore.classify_failure_text('', 1)

    assert message == 'Runtime launcher exited with code 1. Check launcher and compute logs for the first error.'


def test_failed_terminal_job_is_enriched_from_launcher_log(tmp_path):
    store = RuntimeStore(str(tmp_path / 'runtime.db'))
    run_dir = tmp_path / 'run'
    run_dir.mkdir()
    console = run_dir / 'runtime_console.log'
    console.write_text('[broker] RETURN_CODE: 1\n', encoding='utf-8')
    (run_dir / 'launcher.log').write_text(
        'srun: error: Invalid account or account/partition combination\n[broker] RETURN_CODE: 1\n',
        encoding='utf-8',
    )
    runtime_job_id = store.create_job(
        tool_key='udp_kpi',
        requested_by='tester',
        session_id='session',
        mode='hdf',
        input_path='/net/input.h5',
        output_path=str(tmp_path / 'output'),
        log_path=str(console),
        command=['false'],
        resources={'scheduler': 'slurm'},
        request_payload={'paths': {}},
    )
    store.update_job(runtime_job_id, status='FAILED', return_code=1, error_message='')

    job = store.get_job(runtime_job_id, refresh=False)

    assert job['status'] == 'FAILED'
    assert 'Invalid Slurm' in job['error_message']


def test_broker_server_is_sized_for_bursty_status_and_submit_requests():
    assert hpcc_main.ThreadedBrokerServer.request_queue_size >= 256
    assert hpcc_main.ThreadedBrokerServer.daemon_threads is True


def test_helios_profile_uses_verified_host_srun_wrapper():
    profile = main_app.KRAKOW_RUNTIME_PROFILES['helios']

    assert profile['module'] == 'slurm/helios'
    assert profile['srun'] == '/app/software/slurm/helios/bin/srun'
    assert profile['partition'] == '8k3'