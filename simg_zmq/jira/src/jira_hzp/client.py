from __future__ import annotations

from typing import Any

import requests

from .config import Settings


class JiraClient:
    def __init__(self, settings: Settings):
        self.settings = settings
        self.session = requests.Session()
        self.session.headers.update(
            {
                "Accept": "application/json",
                "Content-Type": "application/json",
            }
        )
        self.session.verify = settings.verify_ssl

        self._auth: tuple[str, str] | None = None
        if settings.pat:
            self.session.headers["Authorization"] = f"Bearer {settings.pat}"
        elif settings.user and settings.api_token:
            self._auth = (settings.user, settings.api_token)

    def whoami(self) -> dict[str, Any]:
        return self._request("GET", "/rest/api/2/myself")

    def create_issue(
        self,
        *,
        project_key: str,
        issue_type: str,
        summary: str,
        description: str,
        assignee: str | None = None,
        labels: list[str] | None = None,
    ) -> dict[str, Any]:
        fields: dict[str, Any] = {
            "project": {"key": project_key},
            "issuetype": {"name": issue_type},
            "summary": summary,
            "description": description,
        }
        if assignee:
            fields["assignee"] = {"name": assignee}
        if labels:
            fields["labels"] = labels

        payload = {"fields": fields}
        return self._request("POST", "/rest/api/2/issue", json=payload)

    def search_issues(
        self,
        *,
        jql: str,
        fields: list[str] | None = None,
        max_results: int = 50,
        start_at: int = 0,
    ) -> dict[str, Any]:
        payload = {
            "jql": jql,
            "startAt": start_at,
            "maxResults": max_results,
            "fields": fields or ["key", "summary", "status", "assignee"],
        }
        return self._request("POST", "/rest/api/2/search", json=payload)

    def list_board_issues(
        self,
        *,
        board_id: int,
        jql: str | None = None,
        fields: list[str] | None = None,
        max_results: int = 50,
        start_at: int = 0,
    ) -> dict[str, Any]:
        params: dict[str, Any] = {
            "startAt": start_at,
            "maxResults": max_results,
            "fields": ",".join(fields or ["key", "summary", "status", "assignee"]),
        }
        if jql:
            params["jql"] = jql

        return self._request("GET", f"/rest/agile/1.0/board/{board_id}/issue", params=params)

    def _request(self, method: str, path: str, **kwargs: Any) -> dict[str, Any]:
        url = f"{self.settings.base_url}{path}"
        response = self.session.request(
            method=method,
            url=url,
            auth=self._auth,
            timeout=self.settings.timeout_seconds,
            **kwargs,
        )
        response.raise_for_status()
        return response.json()
