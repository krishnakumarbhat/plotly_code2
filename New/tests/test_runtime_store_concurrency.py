from pathlib import Path
from concurrent.futures import ThreadPoolExecutor

from main_html.runtime_store import RuntimeStore


def _create_job(store: RuntimeStore, output_path: Path = None) -> int:
    return store.create_job(
        tool_key='dc',
        requested_by='tester',
        session_id='session-1',
        mode='local',
        input_path='/tmp/input.hdf',
        output_path=str(output_path or ''),
        log_path='',
        command=['true'],
        resources={},
        request_payload={},
    )


def test_read_only_job_lookup_skips_refresh(tmp_path, monkeypatch):
    store = RuntimeStore(str(tmp_path / 'runtime.db'))
    runtime_job_id = _create_job(store)

    def fail_refresh(_job):
        raise AssertionError('read-only lookup must not refresh persisted artifacts')

    monkeypatch.setattr(store, '_refresh_persisted_job', fail_refresh)
    job = store.get_job(runtime_job_id, refresh=False)

    assert job['id'] == runtime_job_id


def test_artifact_refresh_indexes_files(tmp_path):
    output_path = tmp_path / 'output'
    output_path.mkdir()
    artifact_path = output_path / 'report.json'
    artifact_path.write_text('{"ok": true}', encoding='utf-8')

    store = RuntimeStore(str(tmp_path / 'runtime.db'))
    runtime_job_id = _create_job(store, output_path)
    job = store.get_job(runtime_job_id, refresh=True)

    assert [artifact['artifact_path'] for artifact in job['artifacts']] == [str(artifact_path)]


def test_one_hundred_read_only_job_lookups_do_not_lock(tmp_path):
    store = RuntimeStore(str(tmp_path / 'runtime.db'))
    runtime_job_id = _create_job(store)

    with ThreadPoolExecutor(max_workers=20) as executor:
        jobs = list(executor.map(
            lambda _: store.get_job(runtime_job_id, refresh=False),
            range(100),
        ))

    assert all(job and job['id'] == runtime_job_id for job in jobs)
