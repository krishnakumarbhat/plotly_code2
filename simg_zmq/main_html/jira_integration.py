import json
import logging
import os
from pathlib import Path
from typing import Any, Dict, List, Optional
from datetime import datetime

logger = logging.getLogger(__name__)

LLAMA_SERVER_URL = os.environ.get(
    'LLAMA_SERVER_BASE_URL',
    os.environ.get('RAG_LLAMA_URL', 'http://127.0.0.1:8081'),
).rstrip('/')


class JiraIntegration:
    def __init__(self):
        self.base_url = os.environ.get('JIRA_BASE_URL', '').rstrip('/')
        self.pat = os.environ.get('JIRA_PAT', '')
        self.user = os.environ.get('JIRA_USER', '')
        self.api_token = os.environ.get('JIRA_API_TOKEN', '')
        self.default_project = os.environ.get('JIRA_DEFAULT_PROJECT', '')
        self.default_board = os.environ.get('JIRA_DEFAULT_BOARD', 'FHW')
        self._enabled = bool(self.base_url and (self.pat or (self.user and self.api_token)))
        if not self._enabled:
            logger.warning('Jira not configured: set JIRA_BASE_URL and JIRA_PAT (or JIRA_USER + JIRA_API_TOKEN)')

    def _headers(self) -> Dict[str, str]:
        if self.pat:
            auth = self.pat
        else:
            import base64
            auth = base64.b64encode(f'{self.user}:{self.api_token}'.encode()).decode()
        return {
            'Authorization': f'Basic {auth}',
            'Content-Type': 'application/json',
        }

    def _get(self, url: str) -> Optional[Dict[str, Any]]:
        if not self._enabled:
            return None
        try:
            import requests
            resp = requests.get(url, headers=self._headers(), timeout=30)
            resp.raise_for_status()
            return resp.json()
        except Exception as e:
            logger.error(f'Jira API GET error: {e}')
            return None

    def _post(self, url: str, data: dict) -> Optional[Dict[str, Any]]:
        if not self._enabled:
            return None
        try:
            import requests
            resp = requests.post(url, headers=self._headers(), json=data, timeout=30)
            resp.raise_for_status()
            return resp.json()
        except Exception as e:
            logger.error(f'Jira API error: {e}')
            return None

    def find_users(self, query: str = '') -> List[Dict[str, str]]:
        """Search Jira users by query string."""
        if not self._enabled:
            return []
        search_url = f'{self.base_url}/rest/api/2/user/search?username={query}&maxResults=20'
        result = self._get(search_url)
        if isinstance(result, list):
            return [{'name': u.get('name', ''), 'displayName': u.get('displayName', ''), 'emailAddress': u.get('emailAddress', '')} for u in result]
        return []

    def find_nearest_user(self, name: str) -> Optional[str]:
        """Find the nearest matching Jira user by name or return None."""
        if not name or not self._enabled:
            return None
        users = self.find_users(name)
        if users:
            return users[0].get('name') or users[0].get('displayName', '')
        # Try broader search
        users = self.find_users('')
        if users:
            lower_name = name.lower()
            for u in users:
                if lower_name in u.get('name', '').lower() or lower_name in u.get('displayName', '').lower():
                    return u.get('name') or u.get('displayName', '')
        return None

    def create_kpi_ticket(
        self,
        accuracy_score: float,
        log_path: str,
        hdf_path: str,
        html_path: str,
        details_dict: Dict[str, Any],
        sensor_id: str = '',
    ) -> Optional[str]:
        if accuracy_score >= 60:
            return None
        debug_analysis = self._debug_hdf5_analysis(accuracy_score, hdf_path, log_path)
        description = self._build_ticket_description(accuracy_score, log_path, hdf_path, debug_analysis)
        name = details_dict.get('name', sensor_id or 'unknown')
        summary = f'[KPI Alert] Low accuracy: {accuracy_score:.1f}% - {name}'
        return self._create_ticket(summary, description, story_points=1, board=self.default_board)

    def create_rag_description(self, html_path: str, rag_answer: str) -> Optional[str]:
        summary = f'[RAG Report] Analysis from {os.path.basename(html_path)}'
        return self._create_ticket(summary, rag_answer, story_points=1, board=self.default_board)

    def _create_ticket(self, summary: str, description: str, story_points: int = 1, board: str = 'FHW') -> Optional[str]:
        if not self._enabled:
            return None
        url = f'{self.base_url}/rest/api/2/issue'
        fields = {
            'project': {'key': self.default_project},
            'summary': summary,
            'description': description,
            'issuetype': {'name': 'Task'},
        }
        if story_points is not None:
            fields['customfield_10016'] = story_points
        data = {'fields': fields}
        result = self._post(url, data)
        if result:
            key = result.get('key', '')
            logger.info(f'Created Jira ticket {key}')
            return key
        return None

    def create_resim_ticket(
        self,
        input_txt: str,
        simg_path: str,
        log_path: str = '',
        notes: str = '',
        assignee: str = '',
        job_id: int = 0,
        board: str = 'FHW',
        story_points: int = 1,
    ) -> Optional[str]:
        """Create a JIRA ticket for a resim run result."""
        desc = self._build_resim_description(input_txt, simg_path, log_path, notes, job_id)
        summary = f'[Resim Run] {Path(input_txt).name} - Job #{job_id}'
        ticket_key = self._create_ticket(summary, desc, story_points=story_points, board=board)

        if ticket_key and assignee:
            resolved = self.find_nearest_user(assignee)
            if resolved:
                self._assign_ticket(ticket_key, resolved)
        return ticket_key

    def _build_resim_description(self, input_txt: str, simg_path: str,
                                  log_path: str, notes: str, job_id: int) -> str:
        now = datetime.now().isoformat()
        parts = [
            'Resimulation Run Report',
            '',
            f'Job ID: {job_id}',
            f'Generated: {now}',
            f'Input file: {input_txt}',
            f'SIMG image: {simg_path}',
            f'Log file: {log_path or "N/A"}',
            '',
        ]
        if notes:
            parts.append(f'Additional notes from user:')
            parts.append(f'{notes}')
            parts.append('')

        gemma_analysis = self._call_gemma_for_resim(input_txt, simg_path, log_path)
        if gemma_analysis:
            parts.append('Gemma AI Analysis:')
            parts.append('')
            parts.append(gemma_analysis)
            parts.append('')

        parts.append('This ticket was auto-generated from the HPCC Runtime Console.')
        parts.append('Review the analysis above and the logs for details.')
        return '\n'.join(parts)

    def _call_gemma_for_resim(self, input_txt: str, simg_path: str, log_path: str) -> str:
        """Call gemma_4 model via llama.cpp server for resim analysis."""
        try:
            import requests
        except ImportError:
            return ''

        url = f'{LLAMA_SERVER_URL}/v1/chat/completions'
        payload = {
            'model': 'local',
            'messages': [
                {
                    'role': 'system',
                    'content': (
                        'You are a resimulation analysis assistant. You will receive '
                        'details about a resimulation run. Summarize what happened, '
                        'identify any potential issues or anomalies, and suggest next steps. '
                        'Be concise and technical. Focus on: input data quality, '
                        'simulation parameters, output expectations, and performance characteristics.'
                    ),
                },
                {
                    'role': 'user',
                    'content': (
                        f'Resimulation run completed.\n'
                        f'Input file: {input_txt}\n'
                        f'SIMG image: {simg_path}\n'
                        f'Log path: {log_path}\n\n'
                        f'Provide a brief technical summary of what this resimulation run '
                        f'entails and what to check in the results.'
                    ),
                },
            ],
            'temperature': 0.1,
            'max_tokens': 512,
            'stream': False,
        }

        try:
            resp = requests.post(url, json=payload, timeout=120)
            resp.raise_for_status()
            body = resp.json()
            choices = body.get('choices') or []
            if choices:
                msg = choices[0].get('message') or {}
                content = msg.get('content') or ''
                return content.strip()
        except Exception as e:
            logger.warning('Gemma resim analysis call failed: %s', e)
        return ''

    def _assign_ticket(self, ticket_key: str, assignee: str) -> bool:
        """Assign a JIRA ticket to a user."""
        url = f'{self.base_url}/rest/api/2/issue/{ticket_key}/assignee'
        data = {'name': assignee}
        try:
            resp = requests.put(url, headers=self._headers(), json=data, timeout=30)
            resp.raise_for_status()
            logger.info('Assigned %s to %s', ticket_key, assignee)
            return True
        except Exception as e:
            logger.error('Failed to assign ticket %s: %s', ticket_key, e)
            return False

    def find_low_accuracy_logs(self, accuracy_dict: Dict[str, float]) -> List[str]:
        return [k for k, v in accuracy_dict.items() if v < 60]

    def _build_ticket_description(self, accuracy_score: float, log_path: str, hdf_path: str, debug_analysis: str = '') -> str:
        now = datetime.now().isoformat()
        parts = [
            f'KPI Accuracy Alert',
            f'',
            f'Accuracy: {accuracy_score:.1f}%',
            f'Generated: {now}',
            f'Log file: {log_path}',
            f'HDF file: {hdf_path}',
            f'',
        ]
        if debug_analysis:
            parts.append(f'AI Debug Analysis:')
            parts.append(f'')
            parts.append(debug_analysis)
            parts.append(f'')
        parts.append(f'Accuracy is below the 60% threshold. Review the AI analysis above and the logs for root cause.')
        return '\n'.join(parts)

    def _summarize_hdf5(self, hdf_path: str) -> str:
        try:
            import h5py
            import numpy as np
        except ImportError:
            return ''

        path = Path(hdf_path)
        if not path.exists():
            return f'HDF5 file not found: {hdf_path}'

        try:
            f = h5py.File(str(path), 'r')
        except Exception as e:
            return f'Cannot open HDF5: {e}'

        lines = [f'HDF5 file: {path.name}', f'File size: {path.stat().st_size / 1024 / 1024:.1f} MB', '']

        def _walk_group(group, depth=0):
            indent = '  ' * depth
            for key in group:
                try:
                    item = group[key]
                except Exception:
                    continue
                if isinstance(item, h5py.Group):
                    lines.append(f'{indent}[{key}]')
                    _walk_group(item, depth + 1)
                elif isinstance(item, h5py.Dataset):
                    shape = str(item.shape) if item.shape is not None else 'scalar'
                    dtype = str(item.dtype) if item.dtype is not None else 'unknown'
                    lines.append(f'{indent}{key}: shape={shape}, dtype={dtype}')
                    if item.ndim == 1 and item.size > 0 and item.size <= 20:
                        try:
                            values = item[:]
                            if np.issubdtype(item.dtype, np.floating) or np.issubdtype(item.dtype, np.integer):
                                lines.append(f'{indent}  values={values.tolist()}')
                        except Exception:
                            pass
                    elif item.ndim == 1 and item.size > 20:
                        try:
                            arr = item[:]
                            if np.issubdtype(item.dtype, np.floating):
                                lines.append(f'{indent}  range=[{float(arr.min()):.4f}, {float(arr.max()):.4f}], mean={float(arr.mean()):.4f}')
                            elif np.issubdtype(item.dtype, np.integer):
                                lines.append(f'{indent}  range=[{int(arr.min())}, {int(arr.max())}], mean={float(arr.mean()):.2f}')
                        except Exception:
                            pass

        try:
            _walk_group(f)
        except Exception as e:
            lines.append(f'(error walking HDF: {e})')
        f.close()
        return '\n'.join(lines[:120])

    def _call_llm_for_debug(self, prompt: str) -> str:
        try:
            import requests
        except ImportError:
            return ''

        url = f'{LLAMA_SERVER_URL}/v1/chat/completions'
        payload = {
            'model': 'local',
            'messages': [
                {
                    'role': 'system',
                    'content': (
                        'You are a radar/KPI debug analyst. You will receive HDF5 data summaries '
                        'from radar validation runs. Analyze the data, identify potential causes '
                        'for low accuracy, and suggest specific things to investigate. '
                        'Be concise and technical. Focus on: scan count mismatches, '
                        'missing detections, signal range anomalies, or empty groups.'
                    ),
                },
                {'role': 'user', 'content': prompt},
            ],
            'temperature': 0.1,
            'max_tokens': 512,
            'stream': False,
        }

        try:
            resp = requests.post(url, json=payload, timeout=120)
            resp.raise_for_status()
            body = resp.json()
            choices = body.get('choices') or []
            if choices:
                msg = choices[0].get('message') or {}
                content = msg.get('content') or ''
                return content.strip()
        except Exception as e:
            logger.warning('LLM debug call failed: %s', e)
        return ''

    def _debug_hdf5_analysis(self, accuracy: float, hdf_path: str, log_path: str) -> str:
        summary = self._summarize_hdf5(hdf_path)
        if not summary:
            return ''

        prompt_lines = [
            f'A KPI validation run achieved only {accuracy:.1f}% accuracy.',
            f'Here is the HDF5 input data summary:',
            f'',
            summary,
            f'',
            f'Log path: {log_path}',
            f'',
            f'Analyze why accuracy might be low. Consider:',
            f'- Are there empty groups or zero-size datasets?',
            f'- Is the scan count very low?',
            f'- Are signal ranges abnormal?',
            f'- Could there be a timestamp/alignment mismatch?',
            f'- Are expected sensors/groups missing?',
            f'Provide specific debugging suggestions.',
        ]

        return self._call_llm_for_debug('\n'.join(prompt_lines))
