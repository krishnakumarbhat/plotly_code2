import ipaddress
import json
import os
from typing import Any, Dict, Optional
from urllib import error, parse, request


class RagClient:
    def __init__(self, runtime_store):
        self.runtime_store = runtime_store

    def ask(self, question: str, session_id: str) -> Dict[str, Any]:
        endpoint = self._resolve_base_url()
        if not endpoint:
            return {
                'ok': False,
                'answer': '',
                'error': 'RAG service URL is not configured. KPI Guide requires rag.simg to be reachable.',
            }

        url = parse.urljoin(endpoint.rstrip('/') + '/', 'ask')
        payload = json.dumps({'question': question, 'session_id': session_id}).encode('utf-8')
        http_request = request.Request(
            url,
            data=payload,
            headers={'Content-Type': 'application/json'},
            method='POST',
        )
        opener = self._build_opener(url)
        last_error = 'RAG service did not return a response.'
        for timeout_seconds in (45, 90):
            try:
                with opener.open(http_request, timeout=timeout_seconds) as response:
                    data = json.loads(response.read().decode('utf-8'))
                return {
                    'ok': True,
                    'answer': data.get('answer', ''),
                    'session_id': data.get('session_id', session_id),
                }
            except error.HTTPError as exc:
                detail = ''
                try:
                    detail = exc.read().decode('utf-8', errors='replace').strip()
                except Exception:
                    detail = ''
                last_error = f'RAG service returned HTTP {exc.code}'
                if detail:
                    last_error = f'{last_error}: {detail}'
                break
            except (error.URLError, json.JSONDecodeError) as exc:
                last_error = f'RAG service unavailable: {exc}'

        return {
            'ok': False,
            'answer': '',
            'error': last_error,
        }

    def _resolve_base_url(self) -> Optional[str]:
        env_value = os.environ.get('RAG_SERVICE_URL', '').strip()
        if env_value:
            return env_value
        rag_tool = self.runtime_store.get_tool('rag')
        if rag_tool and rag_tool.get('service_url'):
            return rag_tool['service_url']
        return None

    def _build_opener(self, url: str):
        if self._should_bypass_proxy(url):
            return request.build_opener(request.ProxyHandler({}))
        return request.build_opener()

    def _should_bypass_proxy(self, url: str) -> bool:
        hostname = (parse.urlparse(url).hostname or '').strip().lower()
        if not hostname:
            return False
        if hostname == 'localhost':
            return True
        try:
            address = ipaddress.ip_address(hostname)
        except ValueError:
            return False
        return address.is_private or address.is_loopback or address.is_link_local