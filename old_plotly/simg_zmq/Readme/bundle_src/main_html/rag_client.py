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
                'error': 'RAG service URL is not configured yet.',
            }

        url = parse.urljoin(endpoint.rstrip('/') + '/', 'ask')
        payload = json.dumps({'question': question, 'session_id': session_id}).encode('utf-8')
        http_request = request.Request(
            url,
            data=payload,
            headers={'Content-Type': 'application/json'},
            method='POST',
        )
        try:
            with request.urlopen(http_request, timeout=120) as response:
                data = json.loads(response.read().decode('utf-8'))
            return {
                'ok': True,
                'answer': data.get('answer', ''),
                'session_id': data.get('session_id', session_id),
            }
        except error.URLError as exc:
            return {
                'ok': False,
                'answer': '',
                'error': f'RAG service unavailable: {exc}',
            }

    def _resolve_base_url(self) -> Optional[str]:
        env_value = os.environ.get('RAG_SERVICE_URL', '').strip()
        if env_value:
            return env_value
        rag_tool = self.runtime_store.get_tool('rag')
        if rag_tool and rag_tool.get('service_url'):
            return rag_tool['service_url']
        return None