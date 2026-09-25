"""

Parse an HTML based Test report, extract its result and send it to the metric reporting tool.

These HTML based report consists of Tests/Modules and SRS based results.
Result extracted from these reports is sent to the Platform-Health to populate dashboards.

"""

import re
import os
import argparse
import uuid
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


def parse_html_report(file_path):
    """Parse the HTML report and extract overall test results."""
    with open(file_path, "r", encoding="utf-8", errors="replace") as file:
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

    with open(file_path, "r", encoding="utf-8", errors="replace") as file:
        html_content = file.read()
    soup = BeautifulSoup(html_content, "html.parser")
    rows = soup.select("table#results-table tbody tr")
    module_pattern = re.compile(r"::test_([a-zA-Z_][a-zA-Z_\d]*)", re.IGNORECASE)
    srs_pattern = re.compile(r"WI-\d+")

    for row in rows:
        test_case_name_td = row.find("td", {"class": "col-name"})
        result_td = row.find("td", {"class": "col-result"})
        srs_td = row.find("td", string=re.compile(r"WI-\d+"))  # Find TD containing WI-xxx
        if test_case_name_td and result_td:
            match = module_pattern.search(test_case_name_td.text.strip())
            result = result_td.text.strip().lower()
            srs_ids = []
            if srs_td:
                srs_ids = srs_pattern.findall(srs_td.text)
            if match:
                module_name = standardize_module_name("::test_" + match.group(1))
                # Initialize module results if not already present
                if module_name not in module_results:
                    module_results[module_name] = {
                        "Passed": 0,
                        "Failed": 0,
                        "Error": 0,
                        "SRS_IDs": set(),
                    }
                # Increment counts based on test result
                if result == "passed":
                    module_results[module_name]["Passed"] += 1
                elif result == "failed":
                    module_results[module_name]["Failed"] += 1
                elif result == "error":
                    module_results[module_name]["Error"] += 1
                # Add SRS IDs
                module_results[module_name]["SRS_IDs"].update(srs_ids)
    return module_results


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Send test results to Platform Health.")
    parser.add_argument("--report_path", required=True, help="HTML report path.")
    parser.add_argument("--variant", required=True, help="Variant Type.")
    parser.add_argument("--job_type", required=True, help="Job type SWE6 or SWE5.")
    parser.add_argument("--gen_version", required=True, help="Gen version Gen7v1 or Gen7v2.")
    parser.add_argument("--commit_id", required=True, help="Latest commit Id")

    args = parser.parse_args()
    commit_id = args.commit_id
    platformHealthInst = PlatformHealthTestJob(commit_id, "Core_Radar_Gen7_SAF85xx")
    report_path = args.report_path
    variant = args.variant
    job_type = args.job_type
    gen_version = args.gen_version

    # Metric names for Tests and SRS
    tests_overall_metric = (
        f"core_radar.{gen_version}.{job_type}.tests_overall_result"  # Overall Test Results
    )
    metric_name = f"core_radar.{gen_version}.{job_type}.testcases"  # Modulewise Test Results
    srs_overall_metric = (
        f"core_radar.{gen_version}.{job_type}.srs_overall_result"  # SRS Overall Results
    )
    srs_metric_name = f"core_radar.{gen_version}.{job_type}.srs"  # SRS individual Results

    if os.path.exists(report_path):

        # Extract overall test results
        passed_tests, failed_tests, skipped_tests, errors_tests = parse_html_report(report_path)

        # Send overall test results
        platformHealthInst.send_metric(
            metric_name,
            passed_tests,
            tags={
                "Type": "passed",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            metric_name,
            failed_tests,
            tags={
                "Type": "failed",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            metric_name,
            skipped_tests,
            tags={
                "Type": "skipped",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            metric_name,
            errors_tests,
            tags={
                "Type": "errors",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )

        # Extract module-wise results
        module_results = extract_module_results_from_html(report_path)

        # Print and send module-wise results
        print("\n-------------------------- Test Results Summary --------------------------")
        for module, results in module_results.items():

            srs_id_str = ",".join(sorted(results["SRS_IDs"])) if results["SRS_IDs"] else "None"

            print(f"Module/Test Case Name: {module}")
            print(f"  No. of Tests Passed: {results['Passed']}")
            print(f"  No. of Tests Failed: {results['Failed']}")
            print(f"  No. of Tests Errors: {results['Error']}")
            print(f"  SRS mapped to Tests: {srs_id_str}")
            print(f"  SRS Count: {len(results['SRS_IDs'])}\n")

            # Send module results to Platform Health
            platformHealthInst.send_metric(
                metric_name,
                results["Passed"],
                tags={
                    "Type": "passed",
                    "Variant": variant,
                    "Module": module,
                    "GenVersion": gen_version,
                    "JobType": job_type,
                    "SRS_Work_IDs": srs_id_str,
                },
            )
            platformHealthInst.send_metric(
                metric_name,
                results["Failed"],
                tags={
                    "Type": "failed",
                    "Variant": variant,
                    "Module": module,
                    "GenVersion": gen_version,
                    "JobType": job_type,
                    "SRS_Work_IDs": srs_id_str,
                },
            )
            platformHealthInst.send_metric(
                metric_name,
                results["Error"],
                tags={
                    "Type": "error",
                    "Variant": variant,
                    "Module": module,
                    "GenVersion": gen_version,
                    "JobType": job_type,
                    "SRS_Work_IDs": srs_id_str,
                },
            )

        overall_test_count = passed_tests + failed_tests + skipped_tests + errors_tests
        total_modules = list(module_results.keys())
        unique_srs_ids = set()

        for result in module_results.values():
            unique_srs_ids.update(result["SRS_IDs"])

        print(
            f"-------------------------- Overall Test Cases: {overall_test_count} --------------------------"
        )
        print(f"Total Tests Passed: {passed_tests}")
        print(f"Total Tests Failed: {failed_tests}")
        print(f"Total Tests Skipped: {skipped_tests}")
        print(f"Total Tests Errors: {errors_tests}")
        print(f"Total Modules Covered: {len(total_modules)}")

        # Send overall Tests/Modules summary to Platform Health
        platformHealthInst.send_metric(
            tests_overall_metric,
            overall_test_count,
            tags={
                "Type": "total_tests",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            tests_overall_metric,
            passed_tests,
            tags={
                "Type": "passed",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            tests_overall_metric,
            failed_tests,
            tags={
                "Type": "failed",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            tests_overall_metric,
            skipped_tests,
            tags={
                "Type": "skipped",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            tests_overall_metric,
            errors_tests,
            tags={
                "Type": "error",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            tests_overall_metric,
            len(total_modules),
            tags={
                "Type": "modules_covered",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )

        # ------------------- Accurate SRS ID-wise Summary -------------------
        print("\n---------------------- SRS based Summary ----------------------")

        from collections import defaultdict

        srs_result_tracker = defaultdict(lambda: {"modules": set(), "results": []})

        with open(report_path, "r", encoding="utf-8", errors="replace") as file:
            html_content = file.read()
        soup = BeautifulSoup(html_content, "html.parser")
        rows = soup.select("table#results-table tbody tr")
        srs_pattern = re.compile(r"WI-\d+")

        for row in rows:
            if row.find("td", class_="col-name") and row.find("td", class_="col-result"):
                raw_name = row.find("td", class_="col-name").text.strip()
                module_match = re.search(
                    r"::test_([a-zA-Z_][a-zA-Z_\d]*)", raw_name, re.IGNORECASE
                )
                module_name = (
                    standardize_module_name("::test_" + module_match.group(1))
                    if module_match
                    else raw_name
                )
                result = row.find("td", class_="col-result").text.strip().lower()
                srs_td = row.find_all("td")[1]  # 2nd column has SRS Work IDs
                srs_ids = srs_pattern.findall(srs_td.text) if srs_td else ["None"]
                for srs_id in srs_ids:
                    srs_result_tracker[srs_id]["modules"].add(module_name)
                    srs_result_tracker[srs_id]["results"].append(result)

        # Initialize counters
        srs_summary_count = {"passed": 0, "failed": 0, "error": 0}
        for srs_id, info in sorted(srs_result_tracker.items()):
            modules_str = ", ".join(sorted(info["modules"]))
            result_list = info["results"]
            # Apply rule logic
            if any(r == "failed" for r in result_list):
                final_result = "failed"
            elif any(r == "passed" for r in result_list):
                final_result = "passed"
            else:
                final_result = "error"
            srs_summary_count[final_result] += 1
            print(f"SRS Work ID: {srs_id}")
            print(f"  Modules: {modules_str}")
            print(f"  All Results: {result_list}")
            print(f"  Final Result: {final_result}\n")
            # Send each SRS result as metric
            platformHealthInst.send_metric(
                srs_metric_name,
                1,
                tags={
                    "Type": final_result,
                    "Variant": variant,
                    "GenVersion": gen_version,
                    "JobType": job_type,
                    "SRS_Work_IDs": srs_id,
                    "Module": modules_str,
                },
            )

        print(
            f"-------------------------- Overall Unique SRS: {len(srs_result_tracker)} --------------------------"
        )
        print(f"No. of SRS Passed: {srs_summary_count['passed']}")
        print(f"No. of SRS Failed: {srs_summary_count['failed']}")
        print(f"No. of SRS Error: {srs_summary_count['error']}")

        # Send overall SRS summary to Platform Health
        platformHealthInst.send_metric(
            srs_overall_metric,
            len(srs_result_tracker),
            tags={
                "Type": "total_unique_srs",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            srs_overall_metric,
            srs_summary_count["passed"],
            tags={
                "Type": "passed",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            srs_overall_metric,
            srs_summary_count["failed"],
            tags={
                "Type": "failed",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )
        platformHealthInst.send_metric(
            srs_overall_metric,
            srs_summary_count["error"],
            tags={
                "Type": "error",
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )

        print("\n-------------------------- Platform Health Details --------------------------")
        print(f"Gen Version: {gen_version}")
        print(f"Job Type: {job_type}")
        print(f"Variant: {variant}")
        print(f"Run Identifier: {run_identifier}")
        print(f"Commit ID: {commit_id}\n")
        print(f"Run Identifier: {run_identifier}")
    else:
        print(f"HTML file not found: {report_path}")
