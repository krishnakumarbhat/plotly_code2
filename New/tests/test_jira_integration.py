import json
import importlib.util
from pathlib import Path

from jira_integration import JiraIntegration


def _load_deployer():
    script_path = Path(__file__).resolve().parents[1] / 'generate_upload.py'
    spec = importlib.util.spec_from_file_location('hpcc_generate_upload', script_path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def test_jira_integration_loads_mounted_runtime_config(tmp_path, monkeypatch):
    secret_path = tmp_path / 'jira.json'
    secret_path.write_text(
        json.dumps({
            'JIRA_BASE_URL': 'https://jira.example.invalid',
            'JIRA_PAT': 'test-pat',
            'JIRA_DEFAULT_PROJECT': 'FHW',
        }),
        encoding='utf-8',
    )
    monkeypatch.setenv('HPCC_JIRA_CONFIG_FILE', str(secret_path))
    for key in (
        'JIRA_BASE_URL', 'JIRA_PAT', 'JIRA_USER', 'JIRA_API_TOKEN',
        'JIRA_DEFAULT_PROJECT', 'JIRA_DEFAULT_BOARD',
    ):
        monkeypatch.delenv(key, raising=False)

    jira = JiraIntegration()

    assert jira._enabled is True
    assert jira.base_url == 'https://jira.example.invalid'
    assert jira.pat == 'test-pat'
    assert jira.default_project == 'FHW'


def test_jira_pat_uses_bearer_auth(monkeypatch):
    monkeypatch.setenv('JIRA_BASE_URL', 'https://jira.example.invalid')
    monkeypatch.setenv('JIRA_PAT', 'test-pat')
    monkeypatch.delenv('HPCC_JIRA_CONFIG_FILE', raising=False)

    jira = JiraIntegration()

    assert jira._headers()['Authorization'] == 'Bearer test-pat'


def test_resim_ticket_uses_fhw_as_project_key(monkeypatch):
    monkeypatch.setenv('JIRA_BASE_URL', 'https://jira.example.invalid')
    monkeypatch.setenv('JIRA_PAT', 'test-pat')
    monkeypatch.setenv('JIRA_DEFAULT_PROJECT', 'OTHER')
    monkeypatch.delenv('HPCC_JIRA_CONFIG_FILE', raising=False)
    jira = JiraIntegration()
    captured = {}
    jira._build_resim_description = lambda *_args: 'test description'

    def capture_post(url, payload):
        captured['url'] = url
        captured['payload'] = payload
        return {'key': 'FHW-123'}

    jira._post = capture_post
    result = jira.create_resim_ticket(
        input_txt='/net/project/input.txt',
        simg_path='/net/project/resim.simg',
        job_id=28,
        board='FHW',
    )

    assert result == 'FHW-123'
    assert captured['payload']['fields']['project']['key'] == 'FHW'


def test_upload_jira_config_requires_complete_auth_and_drops_empty_keys(monkeypatch):
    deployer = _load_deployer()
    monkeypatch.delenv('JIRA_BASE_URL', raising=False)
    monkeypatch.delenv('JIRA_PAT', raising=False)

    assert deployer._jira_config_payload({}) is None

    payload = deployer._jira_config_payload({
        'JIRA_BASE_URL': 'https://jira.example.invalid',
        'JIRA_PAT': 'test-pat',
        'JIRA_DEFAULT_PROJECT': 'FHW',
    })

    assert json.loads(payload) == {
        'JIRA_BASE_URL': 'https://jira.example.invalid',
        'JIRA_PAT': 'test-pat',
        'JIRA_DEFAULT_PROJECT': 'FHW',
    }


def test_port_5006_upload_targets_isolated_all_services_roots():
    deployer = _load_deployer()

    krakow_root, southfield_root = deployer._runtime_5006_roots()

    assert krakow_root.endswith('/RNA-SDV-SRR7/4-Checkout/all_services_7')
    assert southfield_root == '/mnt/usmidet/projects/RADARCORE/2-Sim/all_services_7'
