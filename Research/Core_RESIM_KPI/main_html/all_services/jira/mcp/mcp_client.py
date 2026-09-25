"""Minimal MCP client wrapper.

This client will attempt to call a remote MCP gateway (if `MCP_GATEWAY_URL` is set),
otherwise it falls back to a local connector implementation for quick testing.
"""
import os
from typing import Any, Dict

import requests

MCP_GATEWAY_URL = os.getenv("MCP_GATEWAY_URL")


def call_connector(connector: str, action: str, args: Dict[str, Any]) -> Any:
    """Call a connector action via MCP gateway or local fallback.

    Example usage:
        call_connector("jira", "create_issue", {"project_key": "PROJ", "summary": "..."})
    """
    if MCP_GATEWAY_URL:
        url = f"{MCP_GATEWAY_URL.rstrip('/')}/connectors/{connector}/actions/{action}"
        resp = requests.post(url, json=args, timeout=30)
        resp.raise_for_status()
        return resp.json()

    # Local fallback for development / testing
    if connector == "jira":
        # lazy import so users without MCP can still import this module
        from mcp import jira_connector

        if action == "create_issue":
            return jira_connector.create_issue(**args)
        if action == "update_issue":
            return jira_connector.update_issue(**args)
        if action == "add_comment":
            return jira_connector.add_comment(**args)
        if action == "search_issues":
            return jira_connector.search_issues(**args)

        raise RuntimeError(f"Unsupported action {action} for connector {connector}")

    raise RuntimeError("MCP_GATEWAY_URL not set and no local fallback for connector: %s" % connector)
