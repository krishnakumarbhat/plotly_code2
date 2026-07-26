from __future__ import annotations

import os
import json
import logging
from typing import Any
from pathlib import Path

logger = logging.getLogger(__name__)


class JiraAgent:
    def __init__(self, config_dir: str = '') -> None:
        self._config = self._load_config(config_dir)
        self._client = None

    @staticmethod
    def _load_config(config_dir: str) -> dict:
        paths = []
        if config_dir:
            paths.append(Path(config_dir) / 'jira_config.json')
        paths.extend([
            Path(os.getcwd()) / 'jira_config.json',
            Path(__file__).parent.parent / 'jira_config.json',
            Path(os.environ.get('HPCC_BUNDLE_ROOT', '')) / 'bundle_src' / 'jira' / 'jira_config.json',
        ])
        for p in paths:
            if p.exists():
                return json.loads(p.read_text())
        return {}

    @property
    def configured(self) -> bool:
        return bool(self._config.get('base_url'))

    def create_issue(self, project: str, summary: str, description: str, assignee: str = '', labels: list[str] | None = None) -> dict:
        if not self.configured:
            return {'ok': False, 'error': 'Jira not configured'}
        try:
            from requests import Session
            s = Session()
            s.headers.update({'Accept': 'application/json', 'Content-Type': 'application/json'})
            auth = None
            if self._config.get('pat'):
                s.headers['Authorization'] = f"Bearer {self._config['pat']}"
            elif self._config.get('user') and self._config.get('api_token'):
                auth = (self._config['user'], self._config['api_token'])
            fields = {'project': {'key': project or self._config.get('default_project', 'HZP')}, 'issuetype': {'name': 'Task'}, 'summary': summary, 'description': description}
            if assignee:
                fields['assignee'] = {'name': assignee}
            if labels:
                fields['labels'] = labels
            url = f"{self._config['base_url'].rstrip('/')}/rest/api/2/issue"
            resp = s.post(url, json={'fields': fields}, auth=auth, timeout=30)
            resp.raise_for_status()
            return {'ok': True, 'issue': resp.json()}
        except Exception as e:
            return {'ok': False, 'error': str(e)}

    def search(self, jql: str, max_results: int = 10) -> list[dict]:
        if not self.configured:
            return []
        try:
            from requests import Session
            s = Session()
            s.headers.update({'Accept': 'application/json', 'Content-Type': 'application/json'})
            auth = None
            if self._config.get('pat'):
                s.headers['Authorization'] = f"Bearer {self._config['pat']}"
            elif self._config.get('user') and self._config.get('api_token'):
                auth = (self._config['user'], self._config['api_token'])
            url = f"{self._config['base_url'].rstrip('/')}/rest/api/2/search"
            resp = s.post(url, json={'jql': jql, 'maxResults': max_results, 'fields': ['key', 'summary', 'status', 'assignee', 'description']}, auth=auth, timeout=30)
            resp.raise_for_status()
            return resp.json().get('issues', [])
        except Exception as e:
            logger.warning('Jira search failed: %s', e)
            return []

    def kpi_report_issue(self, job_id: str, tool: str, accuracy: float, log_path: str, summary: str, details: str) -> dict:
        project = self._config.get('default_project', 'HZP')
        assignee = os.environ.get('JIRA_DEFAULT_ASSIGNEE', '')
        summary_text = f"[KPI] {tool} job {job_id} - {summary}"
        desc = f"*Job:* {job_id}\n*Tool:* {tool}\n*Accuracy:* {accuracy:.2%}\n*Log:* {log_path}\n\n{details}"
        return self.create_issue(project=project, summary=summary_text, description=desc, assignee=assignee, labels=['kpi', 'auto-generated'])
