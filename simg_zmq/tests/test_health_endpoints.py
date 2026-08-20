import json
import sys
from pathlib import Path

import pytest


@pytest.fixture
def rag_app():
    root = Path(__file__).resolve().parent.parent
    rag_path = str(root / 'rag')
    if rag_path not in sys.path:
        sys.path.insert(0, rag_path)

    class DummyIngestor:
        pass

    class DummyEngine:
        def __init__(self, last_error):
            self._last_llm_error = last_error

    class DummyGraph:
        def __init__(self, engine):
            self.rag_engine = engine

    class DummyStore:
        def stats(self):
            return {'session_count': 0}

    return DummyIngestor, DummyEngine, DummyGraph, DummyStore


def test_app_has_health_endpoint():
    from main_html.app import app
    with app.test_client() as client:
        resp = client.get('/health')
        assert resp.status_code == 200
        data = json.loads(resp.data)
        assert data['status'] == 'ok'


def test_rag_api_health_serializes_exception(rag_app):
    DummyIngestor, DummyEngine, DummyGraph, DummyStore = rag_app
    from app.api import RagApi

    api = RagApi(
        DummyIngestor(),
        DummyGraph(DummyEngine(ValueError('test error'))),
        DummyStore(),
        max_session_messages=10,
    )
    with api.app.test_client() as client:
        resp = client.get('/health')
        assert resp.status_code == 200
        data = json.loads(resp.data)
        assert data['status'] == 'ok'
        assert data['llm_last_error'] == 'test error'


def test_rag_api_health_no_error(rag_app):
    DummyIngestor, DummyEngine, DummyGraph, DummyStore = rag_app
    from app.api import RagApi

    api = RagApi(
        DummyIngestor(),
        DummyGraph(DummyEngine(None)),
        DummyStore(),
        max_session_messages=10,
    )
    with api.app.test_client() as client:
        resp = client.get('/health')
        assert resp.status_code == 200
        data = json.loads(resp.data)
        assert data['llm_last_error'] is None
