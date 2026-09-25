"""

Parse a given HTML based Test report and save the data for use in metric reporting tools.

These reports are all HTML based files having overall and module wise results.
Data extracted from these reports is sent to platformHealth to populate dashboards.

"""

import re
import os
import argparse
from bs4 import BeautifulSoup
from platformHealthMetrics import platformHealthStats


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
        self.platformHealthMetrics.sendMetric(metric_name, metric_value, tags)


def parse_html_report(file_path):
    """Parse the HTML report and extract overall test results."""
    with open(file_path, "r", encoding="utf-8") as file:
        html_content = file.read()

    passed_tests = int(
        re.search(r'<span class="passed">(\d+) passed</span>', html_content).group(1) or 0
    )
    failed_tests = int(
        re.search(r'<span class="failed">(\d+) failed</span>', html_content).group(1) or 0
    )
    skipped_tests = int(
        re.search(r'<span class="skipped">(\d+) skipped</span>', html_content).group(1) or 0
    )
    errors_tests = int(
        re.search(r'<span class="error">(\d+) errors</span>', html_content).group(1) or 0
    )

    return passed_tests, failed_tests, skipped_tests, errors_tests


def standardize_module_name(name):
    """Standardize module names (case-insensitive) for aggregation."""
    name = re.sub(r"^::test_", "", name, flags=re.IGNORECASE)
    name = re.sub(r"_\d+|\d+|_$", "", name)
    name = name.split()[0]
    return "_".join(
        part.capitalize() for part in name.split("_") if part
    ).lower()  # Convert to lowercase for case-insensitive matching


def extract_module_results_from_html(file_path):
    """Extract and aggregate module-wise test results."""
    module_results = {}

    with open(file_path, "r", encoding="utf-8") as file:
        html_content = file.read()
    soup = BeautifulSoup(html_content, "html.parser")
    rows = soup.select("table#results-table tbody tr")

    module_pattern = re.compile(r"::test_([a-zA-Z_][a-zA-Z_\d]*)", re.IGNORECASE)

    for row in rows:
        test_case_name_td = row.find("td", {"class": "col-name"})
        result_td = row.find("td", {"class": "col-result"})

        if test_case_name_td and result_td:
            match = module_pattern.search(test_case_name_td.text.strip())
            result = result_td.text.strip().lower()
            if match:
                module_name = standardize_module_name("::test_" + match.group(1))

                # Initialize module results if not already present
                if module_name not in module_results:
                    module_results[module_name] = {"Passed": 0, "Failed": 0, "Error": 0}

                # Increment counts based on test result
                if result == "passed":
                    module_results[module_name]["Passed"] += 1
                elif result == "failed":
                    module_results[module_name]["Failed"] += 1
                elif result == "error":
                    module_results[module_name]["Error"] += 1

    return module_results


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Send test results to Platform Health.")
    parser.add_argument("--report_path", required=True, help="HTML report path.")
    parser.add_argument("--variant", required=True, help="Variant Type.")
    parser.add_argument("--test_type", required=True, help="Test type SWE6 or SWE5.")
    parser.add_argument("--commit_id", required=True, help="Latest commit Id")
    args = parser.parse_args()

    commit_id = args.commit_id

    platformHealthInst = PlatformHealthTestJob(commit_id, "Core_Radar_Gen7_SAF85xx")

    report_path = args.report_path
    variant = args.variant
    test_type = args.test_type

    if os.path.exists(report_path):
        # Extract overall test results
        passed_tests, failed_tests, skipped_tests, errors_tests = parse_html_report(report_path)

        metric_name = f"core_radar.gen7_v2.{test_type}.testcases"

        # Send overall test results
        platformHealthInst.send_metric(
            metric_name,
            passed_tests,
            tags={"Type": "passed", "Variant": variant, "GenVersion": "Gen7v2"},
        )
        platformHealthInst.send_metric(
            metric_name,
            failed_tests,
            tags={"Type": "failed", "Variant": variant, "GenVersion": "Gen7v2"},
        )
        platformHealthInst.send_metric(
            metric_name,
            skipped_tests,
            tags={"Type": "skipped", "Variant": variant, "GenVersion": "Gen7v2"},
        )
        platformHealthInst.send_metric(
            metric_name,
            errors_tests,
            tags={"Type": "errors", "Variant": variant, "GenVersion": "Gen7v2"},
        )

        # Extract module-wise results
        module_results = extract_module_results_from_html(report_path)

        # Print and send module-wise results
        print("\n--- Module-wise Test Results ---")
        for module, results in module_results.items():
            print(f"Module: {module}")
            print(f"  Passed: {results['Passed']}")
            print(f"  Failed: {results['Failed']}")
            print(f"  Errors: {results['Error']}\n")

            # Send module results to Platform Health
            platformHealthInst.send_metric(
                metric_name,
                results["Passed"],
                tags={
                    "Type": "passed",
                    "Variant": variant,
                    "Module": module,
                    "GenVersion": "Gen7v2",
                },
            )
            platformHealthInst.send_metric(
                metric_name,
                results["Failed"],
                tags={
                    "Type": "failed",
                    "Variant": variant,
                    "Module": module,
                    "GenVersion": "Gen7v2",
                },
            )
            platformHealthInst.send_metric(
                metric_name,
                results["Error"],
                tags={
                    "Type": "error",
                    "Variant": variant,
                    "Module": module,
                    "GenVersion": "Gen7v2",
                },
            )

        print(f"Total Passed: {passed_tests}")
        print(f"Total Failed: {failed_tests}")
        print(f"Total Skipped: {skipped_tests}")
        print(f"Total Errors: {errors_tests}")

        print(
            f"\n--- Overall/Modulewise Data sent to Platform Health for Gen7v2 Commit: {commit_id} ---\n"
        )
    else:
        print(f"HTML file not found: {report_path}")
