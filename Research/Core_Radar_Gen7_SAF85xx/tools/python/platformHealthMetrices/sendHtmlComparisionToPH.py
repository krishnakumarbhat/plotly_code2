"""

Parse a directory having HTML based Test reports, find latest and previous day reports and compare for the new failures.

Result extracted from these reports is sent to the Platform-Health to populate dashboards.

"""

import os
import re
import time
from datetime import datetime
from bs4 import BeautifulSoup
import argparse
from platformHealthMetrics import platformHealthStats
import uuid
import json


def extract_date_from_filename(filename):
    """------------------------ Extract Date from Filename ------------------------."""
    match = re.search(r"Report_(\w+)_(\d{1,2})_(\d{4})_", filename)
    if not match:
        return None
    month_str, day, year = match.groups()
    try:
        date_str = f"{month_str} {day} {year}"
        return datetime.strptime(date_str, "%b %d %Y")
    except Exception:
        return None


def parse_failed_tests(file_path):
    """------------------------ Parse HTML & Extract Failed Test Cases ------------------------."""
    try:
        content = open(file_path, "r", encoding="utf-8").read()
    except UnicodeDecodeError:
        content = open(file_path, "r", encoding="latin-1", errors="replace").read()
    soup = BeautifulSoup(content, "html.parser")
    failed_rows = soup.find_all("tbody", {"class": "failed results-table-row"})
    results = []

    for row in failed_rows:
        tr = row.find("tr")
        if not tr:
            continue
        cols = tr.find_all("td")
        if len(cols) < 3:
            continue
        test_name = cols[0].get_text(strip=True)
        srs_id = cols[1].get_text(strip=True)
        result = cols[-1].get_text(strip=True)
        results.append((test_name, srs_id, result))
    return results


def find_reports(folder_path):
    """------------------------ Find Latest 2 Reports ------------------------."""
    html_files = [f for f in os.listdir(folder_path) if f.endswith(".html") and "GUI" not in f]
    dated_files = []

    for f in html_files:
        dt = extract_date_from_filename(f)
        if dt:
            dated_files.append((f, dt))

    if len(dated_files) < 2:
        raise Exception("Need at least 2 valid reports")

    dated_files.sort(key=lambda x: x[1], reverse=True)
    return dated_files[0], dated_files[1]


def compare_reports(folder_path):
    """------------------------ Comparision of Reports ------------------------."""
    current_report, previous_report = find_reports(folder_path)
    current_file = os.path.join(folder_path, current_report[0])
    previous_file = os.path.join(folder_path, previous_report[0])
    current_failed = parse_failed_tests(current_file)
    previous_failed = parse_failed_tests(previous_file)
    prev_test_names = {t[0] for t in previous_failed}
    new_failures = [t for t in current_failed if t[0] not in prev_test_names]
    current_date_str = current_report[1].strftime("%d-%b-%Y")
    previous_date_str = previous_report[1].strftime("%d-%b-%Y")

    # ------------------------ Print Output for Current Day ------------------------
    print("\n" + "-" * 110)
    print(f"Current Day (Date: {current_date_str}) -- >>  Failures = {len(current_failed)}")
    print(f"Report File: {current_report[0]}")
    print("-" * 110)
    print("\n" + "-" * 110)

    # ------------------------ Print Output for Previous Day ------------------------
    print(f"Previous Day (Date: {previous_date_str}) -- >>  Failures = {len(previous_failed)}")
    print(f"Report File: {previous_report[0]}")
    print("-" * 110)
    print("\n" + "-" * 110)

    # ------------------------ Print Output for Comparison ------------------------
    print(
        f"Comparison Day (Date: {current_date_str}) -- >> New Failures Found = {len(new_failures)}"
    )
    print("-" * 110)

    if not new_failures:
        print(f"No New Failures. Existing Failures = {len(current_failed)}")
    else:
        for t in new_failures:
            print(f"{t[0]} -- {t[1]} -- {t[2]}")
    return current_report, previous_report, current_failed, previous_failed, new_failures


def send_metric(ph, metric, value, tags):
    """------------------------ Send PH Metrics ------------------------."""
    try:
        ph.sendMetric(metric, value, tags)
        time.sleep(0.05)  # DB stability throttle
    except Exception as e:
        print(f"WARNING: sendMetric failed: {e}")


if __name__ == "__main__":

    run_id = f"run_{uuid.uuid4().hex[:8]}"
    parser = argparse.ArgumentParser(
        description="Compare HTML reports & send Platform Health metrics"
    )
    parser.add_argument("--report_folder", required=True)
    parser.add_argument("--gen_version", required=True)
    parser.add_argument("--job_type", required=True)
    parser.add_argument("--commit_id", required=True)
    parser.add_argument("--variant", required=True)

    args = parser.parse_args()
    folder = args.report_folder

    gen_version = args.gen_version
    job_type = args.job_type
    commit_id = args.commit_id
    variant = args.variant

    metric_name = f"core_radar.{gen_version}.{job_type}.{variant}.comparision_result"

    ph = platformHealthStats(commitid=commit_id, project="Core_Radar_Gen7_SAF85xx", branch="dev")

    # ------------------------ Run Comparison ------------------------
    curr, prev, curr_failed, prev_failed, new_failed = compare_reports(folder)
    curr_date = curr[1].strftime("%d-%b-%Y")
    prev_date = prev[1].strftime("%d-%b-%Y")

    # ------------------------ Send PH Metrics ------------------------
    send_metric(
        ph,
        metric_name,
        len(curr_failed),
        {
            "Type": "current_day_failures",
            "Date": curr_date,
            "ReportFile": curr[0],
            "RunID": run_id,
        },
    )
    send_metric(
        ph,
        metric_name,
        len(prev_failed),
        {
            "Type": "previous_day_failures",
            "Date": prev_date,
            "ReportFile": prev[0],
            "RunID": run_id,
        },
    )
    send_metric(
        ph,
        metric_name,
        len(new_failed),
        {"Type": "new_failures", "ComparisonDate": curr_date, "RunID": run_id},
    )

    # ------------------------ Only Send New Failures in Detail ------------------------
    for t in new_failed:
        send_metric(
            ph,
            metric_name,
            1,
            {
                "Type": "new_failed_testcase",
                "TestCase": t[0],
                "SRS": t[1],
                "ComparisonDate": curr_date,
                "RunID": run_id,
            },
        )

    # ------------------------ Write Comparison Output to JSON ------------------------
    output_data = {
        "new_failures_count": len(new_failed),
        "new_failure_list": [t[0] for t in new_failed],
        "current_html_file": curr[0],
        "previous_html_file": prev[0],
    }

    with open("comparison_output.json", "w") as f:
        json.dump(output_data, f)

    print("\n---- Comparison results sent to Platform Health ----")
