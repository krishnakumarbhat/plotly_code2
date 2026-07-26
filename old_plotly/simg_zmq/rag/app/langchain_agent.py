from __future__ import annotations

import json
import logging
import os
from pathlib import Path
from typing import Any

logger = logging.getLogger(__name__)


class RAGLangChainAdapter:
    def __init__(self, rag_engine, jira_agent=None) -> None:
        self.rag = rag_engine
        self.jira = jira_agent
        self.max_tool_calls = 5

    def run(self, question: str, history: list[dict[str, str]] | None = None) -> str:
        q = question.strip().lower()
        if self.jira and self.jira.configured and ('jira' in q or 'ticket' in q or 'create issue' in q):
            return self._handle_jira_request(question, history)
        if self.jira and self.jira.configured and ('kpi' in q and ('accuracy' in q or 'report' in q)):
            return self._handle_kpi_report(question, history)
        return self.rag.answer(question, history)

    def _handle_jira_request(self, question: str, history: list[dict[str, str]] | None = None) -> str:
        rag_ans = self.rag.answer(question, history) if self.rag._documents else ''
        parts = question.split('\n')
        summary = parts[0] if parts else 'Task'
        desc = rag_ans or f"Generated from question: {question}"
        result = self.jira.create_issue(
            project=os.environ.get('JIRA_DEFAULT_PROJECT', 'HZP'),
            summary=summary[:255],
            description=desc,
            assignee=os.environ.get('JIRA_DEFAULT_ASSIGNEE', ''),
        )
        if result.get('ok'):
            key = result['issue'].get('key', '')
            return f"Created Jira ticket {key}: {self.jira._config.get('base_url', '')}/browse/{key}"
        return f"Failed to create Jira ticket: {result.get('error', 'unknown')}"

    def _handle_kpi_report(self, question: str, history: list[dict[str, str]] | None = None) -> str:
        rag_ans = self.rag.answer(question, history) if self.rag._documents else ''
        if not self.jira or not self.jira.configured:
            return rag_ans or 'No indexed KPI data found.'
        import re
        acc_match = re.search(r'accuracy[:\s]*([0-9]*\.?[0-9]+)', question.lower())
        accuracy = float(acc_match.group(1)) if acc_match else 0.0
        result = self.jira.create_issue(
            project=os.environ.get('JIRA_DEFAULT_PROJECT', 'HZP'),
            summary=f"[KPI Auto-Report] {question[:200]}",
            description=rag_ans or f"KPI question: {question}",
            assignee=os.environ.get('JIRA_DEFAULT_ASSIGNEE', ''),
            labels=['kpi', 'auto-generated'],
        )
        if result.get('ok'):
            key = result['issue'].get('key', '')
            return f"Created Jira ticket {key} with KPI report.\n\n{rag_ans}"
        return rag_ans or 'No indexed KPI data found.'
