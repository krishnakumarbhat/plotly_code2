"""Minimal Jira connector for MCP/Agent integration.

This module provides lightweight helpers to create/update Jira issues and comments.
It is intended as a prototype and should be extended with retries, rate-limiting,
and secure credential handling (secrets manager) for production use.
"""
from typing import Any, Dict, Optional
import os
import requests

JIRA_URL = os.getenv("JIRA_URL")
JIRA_USER = os.getenv("JIRA_USER")
JIRA_TOKEN = os.getenv("JIRA_TOKEN")


def _auth() -> Optional[tuple]:
    if JIRA_USER and JIRA_TOKEN:
        return (JIRA_USER, JIRA_TOKEN)
    return None


def create_issue(project_key: str, summary: str, description: str = "", issuetype: str = "Task", extra_fields: Optional[Dict[str, Any]] = None) -> Dict[str, Any]:
    """Create a Jira issue and return the response JSON.

    Args:
        project_key: e.g. "PROJ"
        summary: short summary
        description: long description
        issuetype: Jira issue type name
        extra_fields: additional fields to merge into payload['fields']
    """
    if not JIRA_URL:
        raise RuntimeError("JIRA_URL not configured")

    payload: Dict[str, Any] = {
        "fields": {
            "project": {"key": project_key},
            "summary": summary,
            "description": description,
            "issuetype": {"name": issuetype},
        }
    }

    if extra_fields:
        payload["fields"].update(extra_fields)

    resp = requests.post(f"{JIRA_URL.rstrip('/')}/rest/api/3/issue", auth=_auth(), json=payload, timeout=30)
    resp.raise_for_status()
    return resp.json()


def update_issue(issue_key: str, fields: Dict[str, Any]) -> None:
    """Update fields of an existing issue.

    Raises on HTTP error.
    """
    if not JIRA_URL:
        raise RuntimeError("JIRA_URL not configured")

    resp = requests.put(f"{JIRA_URL.rstrip('/')}/rest/api/3/issue/{issue_key}", auth=_auth(), json={"fields": fields}, timeout=30)
    resp.raise_for_status()


def add_comment(issue_key: str, body: str) -> Dict[str, Any]:
    """Add a comment to an issue and return the created comment JSON."""
    if not JIRA_URL:
        raise RuntimeError("JIRA_URL not configured")

    resp = requests.post(f"{JIRA_URL.rstrip('/')}/rest/api/3/issue/{issue_key}/comment", auth=_auth(), json={"body": body}, timeout=30)
    resp.raise_for_status()
    return resp.json()


def search_issues(jql: str) -> Dict[str, Any]:
    """Run a JQL query and return the search result JSON."""
    if not JIRA_URL:
        raise RuntimeError("JIRA_URL not configured")

    resp = requests.get(f"{JIRA_URL.rstrip('/')}/rest/api/3/search", auth=_auth(), params={"jql": jql}, timeout=30)
    resp.raise_for_status()
    return resp.json()
