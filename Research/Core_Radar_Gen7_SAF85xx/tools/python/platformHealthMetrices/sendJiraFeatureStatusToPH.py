"""

Aim of this script is to parse a Feature number and then check the status of the linked issues or stories with it.

This data is also sent to platformHealth to populate dashboards.

"""

import requests
from requests.auth import HTTPBasicAuth
import argparse

# === CLI Argument Parsing ===

parser = argparse.ArgumentParser(description="Fetch main issue and its linked issues from JIRA.")
parser.add_argument("--username", required=True, help="JIRA Username")
parser.add_argument("--password", required=True, help="JIRA Password")
parser.add_argument(
    "--issue_key", required=True, help="Main JIRA issue key to fetch (e.g., EMH-22638)"
)

args = parser.parse_args()
username = args.username
password = args.password
main_issue_key = args.issue_key

base_url = "https://jiraprod.aptiv.com"

# === Main Issue Request ===

main_url = f"{base_url}/rest/api/2/issue/{main_issue_key}"
main_params = {"fields": "summary,issuetype,project,issuelinks,status"}
response = requests.get(main_url, auth=HTTPBasicAuth(username, password), params=main_params)

if response.status_code == 200:

    data = response.json()
    fields = data["fields"]

    print("Issue found!\n")
    print("Key:", data["key"])
    print("Summary:", fields["summary"])
    print("Issue Type:", fields["issuetype"]["name"])
    print("Project:", fields["project"]["name"])
    print("Status:", fields["status"]["name"])

    links = fields.get("issuelinks", [])
    print(f"Linked Issues: {len(links)}")

    for i, link in enumerate(links, 1):

        # Handle both inward and outward links
        issue_data = link.get("outwardIssue") or link.get("inwardIssue")

        if issue_data:

            linked_key = issue_data["key"]
            linked_summary = issue_data["fields"].get("summary", "No summary")

            # Fetch status of the linked issue
            issue_url = f"{base_url}/rest/api/2/issue/{linked_key}"
            params = {"fields": "status"}
            issue_resp = requests.get(
                issue_url, auth=HTTPBasicAuth(username, password), params=params
            )

            if issue_resp.status_code == 200:
                issue_status = issue_resp.json()["fields"]["status"]["name"]
                print(f"[{i}] {linked_key} - {linked_summary} | Status: {issue_status}")

            else:
                print(f"[{i}] {linked_key} - {linked_summary} | Status fetch failed")

else:
    print("Failed to fetch main issue.")
    print("Status:", response.status_code)
    print("Response:", response.text)
