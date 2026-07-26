from __future__ import annotations

from app.langchain_agent import RAGLangChainAdapter
from app.rag_engine import RagEngine
from app.sqlite_store import SQLiteLogStore


class QueryGraph:
    def __init__(self, rag_engine: RagEngine, sqlite_store: SQLiteLogStore, jira_agent=None) -> None:
        self.rag_engine = rag_engine
        self.sqlite_store = sqlite_store
        self.adapter = RAGLangChainAdapter(rag_engine, jira_agent=jira_agent)

    def run(self, question: str, history: list[dict[str, str]] | None = None) -> str:
        answer = self.adapter.run(question, history=history)
        self.sqlite_store.log_query(question=question, answer=answer)
        return answer
