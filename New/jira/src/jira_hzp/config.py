from __future__ import annotations

import os
from dataclasses import dataclass

from dotenv import load_dotenv


@dataclass
class Settings:
    base_url: str
    pat: str | None
    user: str | None
    api_token: str | None
    default_project: str
    default_board_id: int
    verify_ssl: bool
    timeout_seconds: int

    @classmethod
    def from_env(cls) -> "Settings":
        load_dotenv()

        base_url = os.getenv("JIRA_BASE_URL", "").strip().rstrip("/")
        pat = _empty_to_none(os.getenv("JIRA_PAT"))
        user = _empty_to_none(os.getenv("JIRA_USER"))
        api_token = _empty_to_none(os.getenv("JIRA_API_TOKEN"))
        default_project = os.getenv("JIRA_DEFAULT_PROJECT", "HZP").strip()
        default_board_id = int(os.getenv("JIRA_DEFAULT_BOARD_ID", "14936"))
        verify_ssl = _parse_bool(os.getenv("JIRA_VERIFY_SSL", "true"))
        timeout_seconds = int(os.getenv("JIRA_TIMEOUT_SECONDS", "30"))

        if not base_url:
            raise ValueError("JIRA_BASE_URL is required.")

        has_pat = bool(pat)
        has_basic = bool(user and api_token)

        if not has_pat and not has_basic:
            raise ValueError(
                "Set JIRA_PAT, or set both JIRA_USER and JIRA_API_TOKEN for basic auth."
            )

        return cls(
            base_url=base_url,
            pat=pat,
            user=user,
            api_token=api_token,
            default_project=default_project,
            default_board_id=default_board_id,
            verify_ssl=verify_ssl,
            timeout_seconds=timeout_seconds,
        )


def _parse_bool(value: str) -> bool:
    return value.strip().lower() in {"1", "true", "yes", "on"}


def _empty_to_none(value: str | None) -> str | None:
    if value is None:
        return None
    stripped = value.strip()
    return stripped if stripped else None
