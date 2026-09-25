"""

Aim of this script is to parse a PI number and then check the status of the linked issues or stories with it.

This data is also sent to platformHealth to populate dashboards.

"""

import requests
from requests.auth import HTTPBasicAuth
from platformHealthMetrics import platformHealthStats
import uuid
import argparse

# === CLI Argument Parsing ===

parser = argparse.ArgumentParser(
    description="Send JIRA PI feature status metrics to Platform Health."
)
parser.add_argument("--username", required=True, help="JIRA Username")
parser.add_argument("--password", required=True, help="JIRA Password")
parser.add_argument(
    "--target_pi_value", default="25 PI3", help="Target PI value (default: 25 PI3)"
)

args = parser.parse_args()
username = args.username
password = args.password
target_pi_value = args.target_pi_value

# === PI and Field Config ===

target_pi_field_id = "customfield_11021"

# === JQL Search ===

jql = (
    f'project = EMH AND issuetype = features AND text ~ "{target_pi_value}" ORDER BY created DESC'
)
url = "https://jiraprod.aptiv.com/rest/api/2/search"
params = {"jql": jql, "fields": f"key,summary,status,{target_pi_field_id}", "maxResults": 50}

# === Platform Health Init ===

run_identifier = f"run_{uuid.uuid4().hex[:8]}"
ph = platformHealthStats(
    commitid="JIRA-PI-STATUS", project="Core_Radar_Gen7_SAF85xx", branch="dev"
)
metric_name = "core_radar.jira.pi.status"

# === Execute JIRA API Request ===

response = requests.get(url, params=params, auth=HTTPBasicAuth(username, password))

if response.status_code == 200:

    issues = response.json().get("issues", [])

    if not issues:

        print(f"No features found for Target PI = {target_pi_value}")

    else:

        print(f"\nFound {len(issues)} features in Target PI '{target_pi_value}':\n")

        for idx, issue in enumerate(issues, 1):

            fields = issue["fields"]

            issue_key = issue["key"]

            summary = fields["summary"]

            status = fields["status"]["name"]

            target_pi = fields.get(target_pi_field_id, "N/A")

            print(f"{idx}. {issue_key} | {summary}")
            print(f"   Status: {status}")
            print(f"   Target PI: {target_pi}\n")

            # === Send to Platform Health ===

            try:

                ph.sendMetric(
                    metric_name=metric_name,
                    metric_value=1,
                    tags={
                        "RunID": run_identifier,
                        "Target_PI": target_pi_value,
                        "Feature_Key": issue_key,
                        "Feature_Status": status,
                    },
                )

                print(
                    f"Metric sent to Platform Health: {metric_name} | Feature: {issue_key} | Status: {status}"
                )

            except Exception as e:

                print(f"Failed to send metric for {issue_key}: {e}")

        print(
            f"\nAll {len(issues)} feature metrics sent to Platform Health with RunID: {run_identifier}"
        )

        print(f"Check wrsd.perception_metrics WHERE metric_name = '{metric_name}'")

else:

    print("Failed to fetch features.")
    print("Status Code:", response.status_code)
    print("Response:", response.text)
