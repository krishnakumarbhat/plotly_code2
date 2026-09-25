from __future__ import annotations

import argparse
import json
import sys
from textwrap import shorten

import requests

from .client import JiraClient
from .config import Settings


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Jira CLI for HZP board")
    sub = parser.add_subparsers(dest="command", required=True)

    sub.add_parser("whoami", help="Check authentication and print current user")

    board = sub.add_parser("board-list", help="List issues from a board")
    board.add_argument("--board-id", type=int, default=None, help="Board id (default from env)")
    board.add_argument("--jql", default=None, help="Optional additional JQL filter")
    board.add_argument("--limit", type=int, default=20, help="Max issues to return")
    board.add_argument("--start-at", type=int, default=0, help="Pagination offset")
    board.add_argument(
        "--fields",
        default="key,summary,status,assignee",
        help="Comma-separated fields",
    )

    search = sub.add_parser("search", help="Search issues using JQL")
    search.add_argument("--jql", required=True, help="JQL query")
    search.add_argument("--limit", type=int, default=20, help="Max issues to return")
    search.add_argument("--start-at", type=int, default=0, help="Pagination offset")
    search.add_argument(
        "--fields",
        default="key,summary,status,assignee",
        help="Comma-separated fields",
    )

    create = sub.add_parser("create", help="Create a Jira issue")
    create.add_argument("--project", default=None, help="Project key (default from env)")
    create.add_argument("--type", default="Task", help="Issue type, for example Task/Bug/Story")
    create.add_argument("--summary", required=True, help="Issue summary")
    create.add_argument("--description", required=True, help="Issue description")
    create.add_argument("--assignee", default=None, help="Assignee username")
    create.add_argument("--labels", default="", help="Comma-separated labels")

    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    try:
        settings = Settings.from_env()
        client = JiraClient(settings)

        if args.command == "whoami":
            data = client.whoami()
            print(json.dumps(data, indent=2))
            return 0

        if args.command == "board-list":
            fields = _parse_csv(args.fields)
            board_id = args.board_id if args.board_id is not None else settings.default_board_id
            data = client.list_board_issues(
                board_id=board_id,
                jql=args.jql,
                fields=fields,
                max_results=args.limit,
                start_at=args.start_at,
            )
            _print_issues(data)
            return 0

        if args.command == "search":
            fields = _parse_csv(args.fields)
            data = client.search_issues(
                jql=args.jql,
                fields=fields,
                max_results=args.limit,
                start_at=args.start_at,
            )
            _print_issues(data)
            return 0

        if args.command == "create":
            labels = _parse_csv(args.labels)
            project = args.project if args.project else settings.default_project
            data = client.create_issue(
                project_key=project,
                issue_type=args.type,
                summary=args.summary,
                description=args.description,
                assignee=args.assignee,
                labels=labels,
            )
            print(json.dumps(data, indent=2))
            return 0

        parser.error("Unknown command")
        return 2

    except requests.HTTPError as exc:
        status = exc.response.status_code if exc.response is not None else "unknown"
        body = exc.response.text if exc.response is not None else str(exc)
        print(f"HTTP error ({status}): {body}", file=sys.stderr)
        return 1
    except Exception as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1


def _parse_csv(value: str) -> list[str]:
    return [item.strip() for item in value.split(",") if item.strip()]


def _print_issues(data: dict) -> None:
    issues = data.get("issues", [])
    total = data.get("total", len(issues))
    print(f"Returned {len(issues)} issue(s), total={total}")

    for issue in issues:
        key = issue.get("key", "?")
        fields = issue.get("fields", {})
        summary = shorten(str(fields.get("summary", "")), width=70, placeholder="...")

        status_obj = fields.get("status") or {}
        status = status_obj.get("name", "") if isinstance(status_obj, dict) else str(status_obj)

        assignee_obj = fields.get("assignee") or {}
        assignee = ""
        if isinstance(assignee_obj, dict):
            assignee = assignee_obj.get("displayName") or assignee_obj.get("name") or ""

        print(f"- {key:<12} [{status:<15}] {assignee:<20} {summary}")


if __name__ == "__main__":
    raise SystemExit(main())
