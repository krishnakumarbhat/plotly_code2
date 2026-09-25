"""

Parse a given HTML based DIAG Test report and save the data for use in metric reporting tools.

Data extracted from these reports is sent to platformHealth to populate dashboards.

"""

import re
import os
import argparse
import time
import uuid
from datetime import datetime
from bs4 import BeautifulSoup
from platformHealthMetrics import platformHealthStats

# Generate a consistent run identifier, it will help tracking No. of results sending in a particular execution
run_identifier = f"run_{uuid.uuid4().hex[:8]}"


class PlatformHealthTestJob:
    """Class for sending test results to Platform Health."""

    def __init__(self, commit_id, project="test_job", branch="dev"):
        """Initialize Platform Health configuration."""
        self.branch = branch
        self.platformHealthMetrics = platformHealthStats(
            commitid=commit_id, project=project, branch=branch
        )

    def send_metric(self, metric_name, metric_value, tags=None):
        """Send metric to Platform Health."""
        if tags is None:
            tags = {}
        tags["RunID"] = run_identifier  # Add RunID tag to all metrics
        self.platformHealthMetrics.sendMetric(metric_name, metric_value, tags)


def extract_overall_test_results(file_path):
    """Parse the HTML report and extract overall test results and execution date."""
    with open(file_path, "r", encoding="utf-8") as file:
        soup = BeautifulSoup(file, "html.parser")
        text = soup.get_text()

    # Dictionary to store extracted results
    overall_test_results = {
        "Overall number of test cases": None,
        "Executed test cases": None,
        "Not executed test cases": None,
        "Test cases passed": None,
        "Test cases with warning": None,
        "Test cases failed": None,
        "Date of Execution": None,
    }

    # Regex patterns for extracting values
    patterns = {
        "Overall number of test cases": r"Overall number of test cases\s+(\d+)",
        "Executed test cases": r"Executed test cases\s+(\d+)",
        "Not executed test cases": r"Not executed test cases\s+(\d+)",
        "Test cases passed": r"Test cases passed\s+(\d+)",
        "Test cases with warning": r"Test cases with warning\s+(\d+)",
        "Test cases failed": r"Test cases failed\s+(\d+)",
        "Date of Execution": r"Test begin:\s+(\d{4}-\d{2}-\d{2})",  # Extracts YYYY-MM-DD format
    }

    for key, pattern in patterns.items():
        match = re.search(pattern, text)
        if match:
            overall_test_results[key] = (
                match.group(1) if key == "Date of Execution" else int(match.group(1))
            )
        else:
            overall_test_results[key] = "Not Found" if key == "Date of Execution" else 0

    # Convert "Date of Execution" to Unix timestamp (if found)
    if overall_test_results["Date of Execution"] != "Not Found":
        overall_test_results["Date of Execution"] = int(
            time.mktime(
                datetime.strptime(
                    overall_test_results["Date of Execution"], "%Y-%m-%d"
                ).timetuple()
            )
        )

    return overall_test_results


def send_results_to_grafana(overall_test_results, commit_id, variant, job_type):
    """Send extracted test results to Platform Health."""
    platformHealthInst = PlatformHealthTestJob(commit_id, "Core_Radar_Gen7_SAF85xx")
    metric_name = f"core_radar.{gen_version}.{job_type}.tests_overall_result"

    passed = overall_test_results["Test cases passed"]
    failed = overall_test_results["Test cases failed"]
    warnings = overall_test_results["Test cases with warning"]
    skipped = overall_test_results["Not executed test cases"]
    error = warnings
    diagnostics_error = warnings + skipped

    # --- Send "Overall" module results ---
    print(f"\n--- Sending 'Overall' Module Results for commit id: {commit_id}---")
    platformHealthInst.send_metric(
        metric_name,
        passed,
        tags={
            "Type": "passed",
            "Variant": variant,
            "GenVersion": gen_version,
            "JobType": job_type,
        },
    )
    platformHealthInst.send_metric(
        metric_name,
        failed,
        tags={
            "Type": "failed",
            "Variant": variant,
            "GenVersion": gen_version,
            "JobType": job_type,
        },
    )
    platformHealthInst.send_metric(
        metric_name,
        error,
        tags={
            "Type": "errors",
            "Variant": variant,
            "GenVersion": gen_version,
            "JobType": job_type,
        },
    )
    platformHealthInst.send_metric(
        metric_name,
        skipped,
        tags={
            "Type": "skipped",
            "Variant": variant,
            "GenVersion": gen_version,
            "JobType": job_type,
        },
    )
    print("Overall SWE6 DIAG Results has been sent to Platform Health:")
    print(f"Passed: {passed}, Failed: {failed}, Errors: {error}, Skipped: {skipped}")

    # --- Send "Diagnostics" module results ---
    modulewise_metric_name = f"core_radar.{gen_version}.{job_type}.testcases"
    print(f"\n--- Sending 'Diagnostics' Module Results for commit id: {commit_id} ---")
    platformHealthInst.send_metric(
        modulewise_metric_name,
        passed,
        tags={
            "Type": "passed",
            "Variant": variant,
            "GenVersion": gen_version,
            "JobType": job_type,
            "Module": "Diagnostics",
        },
    )
    platformHealthInst.send_metric(
        modulewise_metric_name,
        failed,
        tags={
            "Type": "failed",
            "Variant": variant,
            "GenVersion": gen_version,
            "JobType": job_type,
            "Module": "Diagnostics",
        },
    )
    platformHealthInst.send_metric(
        modulewise_metric_name,
        diagnostics_error,
        tags={
            "Type": "error",
            "Variant": variant,
            "GenVersion": gen_version,
            "JobType": job_type,
            "Module": "Diagnostics",
        },
    )
    print("SWE6 DIAG Module 'Diagnostics' Result has been sent to Platform Health:")
    print(f"Passed: {passed}, Failed: {failed}, Error: {diagnostics_error}")
    print(f"Run Identifier: {run_identifier}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Send test results to Platform Health.")
    parser.add_argument("--report_path", required=True, help="HTML report path.")
    parser.add_argument("--variant", required=True, help="Variant Type.")
    parser.add_argument("--job_type", required=True, help="Test type SWE6 or SWE5.")
    parser.add_argument("--commit_id", required=True, help="Latest commit Id")
    parser.add_argument("--gen_version", required=True, help="Generation version V1 or V2")
    args = parser.parse_args()

    report_path = args.report_path
    variant = args.variant
    job_type = args.job_type
    commit_id = args.commit_id
    gen_version = args.gen_version

    if os.path.exists(report_path):
        # Extract test results
        overall_test_results = extract_overall_test_results(report_path)

        # Send Overall-results to Platform Health (Grafana)
        send_results_to_grafana(overall_test_results, commit_id, variant, job_type)
    else:
        print(f"HTML file not found or Invalid path: {report_path}")
