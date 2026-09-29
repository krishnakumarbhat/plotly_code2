# Jira HZP Python Client

Small Python CLI to:
- Read issues from board `14936` (`HZP`)
- Search issues with JQL
- Create Jira issues in project `HZP`

## 1) Setup

```powershell
cd c:\Users\ouymc2\Desktop\jira
py -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install --upgrade pip
python -m pip install -e .
Copy-Item .env.example .env
```

Edit `.env` and set at least:
- `JIRA_BASE_URL`
- `JIRA_PAT` (recommended for Jira Data Center)

## 2) Quick auth check

```powershell
jira-hzp whoami
```

Expected: your Jira username and display name JSON.

## 3) Read issues from HZP board

```powershell
jira-hzp board-list --limit 20
```

Optional filters:

```powershell
jira-hzp board-list --jql "assignee = currentUser() ORDER BY updated DESC" --limit 20
```

## 4) Search issues with JQL

```powershell
jira-hzp search --jql "project = HZP ORDER BY created DESC" --limit 20
```

## 5) Create a ticket in HZP

```powershell
jira-hzp create --type Task --summary "API smoke test" --description "Created from Python CLI"
```

With labels and assignee:

```powershell
jira-hzp create --type Story --summary "Build KPI report" --description "Initial draft" --labels "pi-1,kpi" --assignee "some.user"
```

## Notes

- For Jira Data Center, PAT with `Authorization: Bearer <PAT>` is commonly used.
- If your instance requires basic auth instead, set `JIRA_USER` and `JIRA_API_TOKEN` and leave `JIRA_PAT` empty.
- If board results look empty, verify board filter JQL and status-column mapping in Jira board settings.
