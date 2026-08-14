from pathlib import Path
from types import SimpleNamespace

import pytest
from flask import render_template

from main_html import app as main


def _render(template_name, **context):
    with main.app.test_request_context('/html/runtime-map', headers={'Host': '10.214.45.45:5005'}):
        return render_template(template_name, **context)


def _kpi_context(customer_accounts, default_account='radarcore'):
    defaults = {
        key: dict(value)
        for key, value in main.RUNTIME_TOOL_DEFAULTS.items()
    }
    defaults['udp_kpi']['account'] = default_account
    return {
        'tool_name': 'KPI Analysis',
        'recent_jobs': [],
        'runtime_tools': [],
        'defaults': defaults,
        'customer_accounts': customer_accounts,
        'configuration_files': [],
        'detected_cluster': 'krakow',
        'krakow_runtime_profiles': main.KRAKOW_RUNTIME_PROFILES,
        'allow_local_scheduler': False,
    }


def _runtime_context(cluster='krakow'):
    return {
        'runtime_graph': main.runtime_store.graph_payload(),
        'broker_defaults': main.RUNTIME_TOOL_DEFAULTS,
        'detected_cluster': cluster,
        'krakow_runtime_profiles': main.KRAKOW_RUNTIME_PROFILES,
    }


def _response_parts(result):
    if isinstance(result, tuple):
        response, status = result
    else:
        response, status = result, result.status_code
    return status, response.get_json()


class _FakeSession:
    def __init__(self):
        self.jobs = []
        self.commits = 0

    def add(self, job):
        self.jobs.append(job)

    def commit(self):
        self.commits += 1
        if self.jobs:
            self.jobs[-1].id = 42


class _FakeThread:
    instances = []

    def __init__(self, *args, **kwargs):
        self.args = args
        self.kwargs = kwargs
        self.started = False
        self.instances.append(self)

    def start(self):
        self.started = True


@pytest.fixture
def resim_boundary_mocks(tmp_path, monkeypatch):
    script_path = tmp_path / 'rResim_Gen7.sh'
    script_path.write_text('#!/usr/bin/env bash\n', encoding='utf-8')
    session = _FakeSession()
    _FakeThread.instances = []

    monkeypatch.setattr(main, 'current_user', SimpleNamespace(id=7, net_id='tester'))
    monkeypatch.setattr(main, 'db', SimpleNamespace(session=session))
    monkeypatch.setattr(main.threading, 'Thread', _FakeThread)
    monkeypatch.setattr(main, '_resim_script_source_path', lambda: str(script_path))
    monkeypatch.setattr(main, '_stored_cluster_password_for_current_user', lambda _net_id: 'secret')
    monkeypatch.setattr(main, '_write_askpass_script', lambda _password, path: Path(path).write_text('askpass', encoding='utf-8'))
    monkeypatch.setattr(main, '_first_writable_dir', lambda *_paths: str(tmp_path))
    monkeypatch.setattr(main, 'get_cache_dir', lambda: str(tmp_path))

    return session, _FakeThread, tmp_path


def _submit_resim(payload):
    with main.app.test_request_context('/api/resim_run_submit', method='POST', json=payload):
        return main.api_resim_run_submit.__wrapped__()


def test_customer_accounts_are_unique_and_ignore_empty_values(monkeypatch):
    monkeypatch.setitem(main.app.config, 'SLURM_ACCOUNT', 'app-account')
    monkeypatch.setattr(main, '_SLURM_DEFAULTS', {'account': 'global-account'})
    monkeypatch.setattr(
        main,
        'get_cluster_slurm_defaults',
        lambda cluster: {'account': 'krakow-account' if cluster == 'krakow' else 'southfield-account'},
    )
    monkeypatch.setattr(
        main,
        'KRAKOW_RUNTIME_PROFILES',
        {'helios': {'account': 'profile-account'}, 'empty': {'account': ''}},
    )
    monkeypatch.setattr(
        main,
        'RUNTIME_TOOL_DEFAULTS',
        {'udp_kpi': {'account': 'tool-account'}, 'duplicate': {'account': 'app-account'}},
    )

    assert main._runtime_customer_accounts() == [
        'app-account',
        'global-account',
        'krakow-account',
        'southfield-account',
        'profile-account',
        'tool-account',
    ]


def test_kpi_template_places_bundle_before_execution_note_and_renders_known_accounts():
    html = _render('tools/kpi.html', **_kpi_context(['radarcore', 'rna-sdv-srr7', '8k3p89']))

    assert html.index('Prepare the bundle') < html.index('Execution note')
    assert html.count('id="deployBundleButton"') == 1
    assert '<label class="form-label">Customer Name</label>' in html
    assert '<option value="radarcore" selected>radarcore</option>' in html
    assert 'value="rna-sdv-srr7"' in html and '>rna-sdv-srr7</option>' in html
    assert 'value="8k3p89"' in html and '>8k3p89</option>' in html
    assert 'id="account"' in html and 'name="account"' in html


def test_kpi_template_selects_other_for_an_unknown_default_account():
    html = _render('tools/kpi.html', **_kpi_context(['radarcore'], default_account='new-customer'))

    assert '<option value="__other__" selected>Other</option>' in html
    assert 'id="customAccountField"' in html
    assert 'value="new-customer"' in html


def test_runtime_map_template_exposes_helios_and_updates_profile_metadata():
    html = _render('runtime_map.html', **_runtime_context())

    assert 'id="resimProfile"' in html
    assert 'value="krakow" data-partition="highPrio" data-module="slurm" selected' in html
    assert 'value="helios" data-partition="8k3" data-module="slurm/helios"' in html
    assert 'value="athena"' in html
    assert 'id="detectedPartition"' in html
    assert 'profile: resimProfile ? resimProfile.value : \'krakow\'' in html


def test_runtime_map_template_keeps_highprio_for_southfield():
    html = _render('runtime_map.html', **_runtime_context(cluster='southfield'))

    assert 'let detectedResimCluster = "southfield";' in html
    assert " : 'highPrio';" in html


def test_runtime_map_route_supplies_profile_context(monkeypatch):
    captured = {}

    def capture_template(template_name, **context):
        captured['template_name'] = template_name
        captured['context'] = context
        return 'rendered'

    monkeypatch.setattr(main, 'render_template', capture_template)
    with main.app.test_request_context('/html/runtime-map', headers={'Host': '10.214.45.45:5005'}):
        result = main.runtime_map.__wrapped__()

    assert result == 'rendered'
    assert captured['template_name'] == 'runtime_map.html'
    assert captured['context']['detected_cluster'] == 'krakow'
    assert captured['context']['krakow_runtime_profiles'] is main.KRAKOW_RUNTIME_PROFILES


def test_resim_defaults_to_normal_krakow_runtime(resim_boundary_mocks):
    session, thread_class, _tmp_path = resim_boundary_mocks

    result = _submit_resim({
        'input_txt': '/net/8k3/project/input.txt',
        'simg_path': '/net/8k3/project/resim.simg',
    })
    status, data = _response_parts(result)

    assert status == 200
    assert data == {'ok': True, 'message': 'Submitted', 'job_id': 42}
    assert session.commits == 1
    assert session.jobs[0].parameters['profile'] == 'krakow'
    assert session.jobs[0].parameters['profile_label'] == 'Krakow Default'
    assert len(thread_class.instances) == 1
    command = thread_class.instances[0].kwargs['args'][1][-1]
    assert 'RESIM_SLURM_MODULE=slurm' in command
    assert 'srun -A' not in command
    assert '/net/8k3/project/input.txt /net/8k3/project/resim.simg highPrio' in command
    assert thread_class.instances[0].started is True


def test_resim_accepts_case_insensitive_profile_and_uses_athena(resim_boundary_mocks):
    _session, thread_class, _tmp_path = resim_boundary_mocks

    result = _submit_resim({
        'input_txt': '/net/8k3/project/input.txt',
        'simg_path': '/net/8k3/project/resim.simg',
        'profile': 'ATHENA',
    })
    status, _data = _response_parts(result)

    assert status == 200
    ssh_command = thread_class.instances[0].kwargs['args'][1]
    command = ssh_command[-1]
    assert command.startswith('bash -lc ')
    assert 'bash' not in ssh_command[-3:-1]
    assert 'module load slurm/athena' in command
    assert '/app/software/slurm/athena/bin/srun -A 8k3p89 -p athena' in command


@pytest.mark.parametrize('profile_id', tuple(main.KRAKOW_RUNTIME_PROFILES))
def test_resim_supports_every_krakow_profile(profile_id, resim_boundary_mocks):
    _session, thread_class, _tmp_path = resim_boundary_mocks

    result = _submit_resim({
        'input_txt': '/net/8k3/project/input.txt',
        'simg_path': '/net/8k3/project/resim.simg',
        'profile': profile_id,
    })
    status, _data = _response_parts(result)

    assert status == 200
    profile = main.KRAKOW_RUNTIME_PROFILES[profile_id]
    command = thread_class.instances[0].kwargs['args'][1][-1]
    assert f"RESIM_SLURM_MODULE={profile['module']}" in command
    if profile_id == 'krakow':
        assert 'srun -A' not in command
    else:
        assert f"module load {profile['module']}" in command
        assert f"{profile['srun']} -A {profile['account']} -p {profile['partition']}" in command
        assert f"--mem={profile['memory']} --cpus-per-task={profile['cpus']}" in command
        assert f"--time={profile['time_limit']}" in command


def test_resim_keeps_southfield_execution_unchanged(resim_boundary_mocks):
    session, thread_class, _tmp_path = resim_boundary_mocks

    result = _submit_resim({
        'input_txt': '/mnt/usmidet/project/input.txt',
        'simg_path': '/mnt/usmidet/project/resim.simg',
        'profile': 'cyfronet',
    })
    status, _data = _response_parts(result)

    assert status == 200
    assert session.jobs[0].parameters['profile'] == 'krakow'
    command = thread_class.instances[0].kwargs['args'][1][-1]
    assert 'module load slurm/' not in command
    assert 'srun -A' not in command
    assert '/mnt/usmidet/project/input.txt /mnt/usmidet/project/resim.simg highPrio' in command


def test_resim_rejects_missing_cluster_password_without_starting_a_job(resim_boundary_mocks, monkeypatch):
    session, thread_class, _tmp_path = resim_boundary_mocks
    monkeypatch.setattr(main, '_stored_cluster_password_for_current_user', lambda _net_id: '')

    result = _submit_resim({
        'input_txt': '/net/8k3/project/input.txt',
        'simg_path': '/net/8k3/project/resim.simg',
    })
    status, data = _response_parts(result)

    assert status == 400
    assert 'No cluster password saved' in data['error']
    assert session.jobs == []
    assert thread_class.instances == []


def test_resim_rejects_missing_script_without_starting_a_job(resim_boundary_mocks, monkeypatch):
    session, thread_class, tmp_path = resim_boundary_mocks
    monkeypatch.setattr(main, '_resim_script_source_path', lambda: str(tmp_path / 'missing-rResim_Gen7.sh'))

    result = _submit_resim({
        'input_txt': '/net/8k3/project/input.txt',
        'simg_path': '/net/8k3/project/resim.simg',
    })
    status, data = _response_parts(result)

    assert status == 500
    assert 'rResim_Gen7.sh not found' in data['error']
    assert session.jobs == []
    assert thread_class.instances == []


def test_generated_dashboard_copies_include_the_runtime_map_changes():
    root = Path(__file__).resolve().parent.parent
    pairs = [
        ('main_html/app.py', 'generate_upload/bundle_src/main_html/app.py'),
        ('main_html/templates/tools/kpi.html', 'generate_upload/bundle_src/main_html/templates/tools/kpi.html'),
        ('main_html/templates/runtime_map.html', 'generate_upload/bundle_src/main_html/templates/runtime_map.html'),
    ]
    markers = {
        'main_html/app.py': ('_runtime_customer_accounts', 'Unknown Krakow Resim runtime profile.'),
        'main_html/templates/tools/kpi.html': ('Prepare the bundle', 'Customer Name', 'Other'),
        'main_html/templates/runtime_map.html': ('resimProfile', 'Helios', 'profile:'),
    }

    for source_name, generated_name in pairs:
        source = (root / source_name).read_text(encoding='utf-8')
        generated = (root / generated_name).read_text(encoding='utf-8')
        assert all(marker in source and marker in generated for marker in markers[source_name])


@pytest.mark.parametrize(
    ('payload', 'expected_error'),
    [
        ({}, 'Input file (input.txt) path is required.'),
        ({'input_txt': '/net/8k3/project/input.txt'}, 'Simg file path is required.'),
        ({'input_txt': 'C:/project/input.txt', 'simg_path': 'C:/project/resim.simg'}, 'Input file path must start'),
        ({'input_txt': '/net/8k3/project/input.txt', 'simg_path': '/mnt/usmidet/project/resim.simg'}, 'Both files must be in the same partition'),
        ({'input_txt': '/net/8k3/project/input.txt', 'simg_path': '/net/8k3/project/resim.simg', 'profile': 'unknown'}, 'Unknown Krakow Resim runtime profile.'),
    ],
)
def test_resim_rejects_invalid_inputs_without_starting_a_job(payload, expected_error, resim_boundary_mocks):
    session, thread_class, _tmp_path = resim_boundary_mocks

    result = _submit_resim(payload)
    status, data = _response_parts(result)

    assert status == 400
    assert expected_error in data['error']
    assert session.jobs == []
    assert thread_class.instances == []
