"""

Aim of this script is to fetch the latest 10 HTML reports from JFrog and send them to Platform Health.

"""


import os
import re
import argparse
import requests
from collections import defaultdict
from platformHealthMetrics import platformHealthStats

parser = argparse.ArgumentParser()
parser.add_argument("--variant", required=True)
parser.add_argument("--gen_version", required=True)
args = parser.parse_args()

JFROG_BASE = "https://jfrog.asux.aptiv.com"
REPO = "core_radar-aptiv-00000000-gen8_ind13400-local"
REPORT_PATH = f"origin/dev/SWE6/Reports/{args.variant}"
METRIC_NAME = f"core_radar.{args.gen_version}.SWE6.swe6_html_reports"
VARIANT = args.variant
GEN_VERSION = args.gen_version

# Credentials injected as environment variables (via Jenkins withCredentials or set locally)
jfrog_user = os.environ.get("JFROG_USER")
jfrog_api_key = os.environ.get("JFROG_API_KEY")

if not jfrog_user or not jfrog_api_key:
    print("ERROR: JFROG_USER and JFROG_API_KEY environment variables must be set.")
    exit(1)

# Fetch HTML reports from JFrog
aql_query = f'items.find({{"repo": "{REPO}", "path": "{REPORT_PATH}", "name": {{"$match": "*.html"}}}}).include("repo", "path", "name", "created").sort({{"$desc": ["created"]}}).limit(300)'

resp = requests.post(
    f"{JFROG_BASE}/artifactory/api/search/aql",
    data=aql_query,
    headers={"Content-Type": "text/plain"},
    auth=(jfrog_user, jfrog_api_key),
)

if resp.status_code != 200:
    print(f"JFrog API error {resp.status_code}: {resp.text}")
    exit(1)

body = resp.json()
if "results" not in body:
    print(f"Unexpected response: {body}")
    exit(1)

results = body["results"]
print(f"Total HTML files found in JFrog: {len(results)}")

# Pick the latest file per day, then take the top 10 days
# Extract the date from the filename (e.g. "...Apr_12_2026...") to avoid UTC offset
# issues where JFrog's 'created' timestamp shifts the date back by one day for
# reports uploaded in early IST morning hours.
_MONTH_MAP = {
    "Jan": "01",
    "Feb": "02",
    "Mar": "03",
    "Apr": "04",
    "May": "05",
    "Jun": "06",
    "Jul": "07",
    "Aug": "08",
    "Sep": "09",
    "Oct": "10",
    "Nov": "11",
    "Dec": "12",
}
_DATE_PATTERN = re.compile(r"(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)_(\d{2})_(\d{4})")


def _date_from_name(name: str) -> str | None:
    """Return 'YYYY-MM-DD' extracted from a filename, or None if not found."""
    m = _DATE_PATTERN.search(name)
    if m:
        month, day, year = m.group(1), m.group(2), m.group(3)
        return f"{year}-{_MONTH_MAP[month]}-{day}"
    return None


by_date = defaultdict(list)
for item in results:
    date = _date_from_name(item["name"]) or item["created"][:10]  # fallback to UTC created
    by_date[date].append(item)

top10_dates = sorted(by_date.keys(), reverse=True)[:10]
top10_reports = []
for date in top10_dates:
    # JFrog already returns sorted by created desc, so first item = latest that day
    latest = by_date[date][0]
    url = f"{JFROG_BASE}/ui/native/{REPO}/{latest['path']}/{latest['name']}"
    top10_reports.append((date, latest["name"], url))

# Send to Platform Health using platformHealthStats
ph = platformHealthStats(
    commitid="jfrog_reports_job", project="Core_Radar_Gen8_IND13400", branch="dev"
)

print("\n========== SENDING TOP 10 HTML REPORTS TO PLATFORM HEALTH ==========\n")
for date, report_name, url in top10_reports:
    ph.sendMetric(
        metric_name=METRIC_NAME,
        metric_value=1,
        tags={
            "Variant": VARIANT,
            "GenVersion": GEN_VERSION,
            "ReportDate": date,
            "ReportName": report_name,
            "ReportURL": url,
        },
    )
    print(f"  [{date}]  {report_name}")
    print(f"           {url}\n")

print(
    f"Done. {len(top10_reports)} report(s) sent to Platform Health under metric '{METRIC_NAME}'."
)
