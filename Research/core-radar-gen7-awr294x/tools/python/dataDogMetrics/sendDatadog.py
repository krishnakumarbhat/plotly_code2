"""Author: mrsgdj."""
import re
import os
import argparse
from bs4 import BeautifulSoup
from datadog import initialize, statsd


class dataDogStats:
    """Class definition for sending metrics to Datadog."""

    def __init__(self, statsd_host="172.24.204.248", statsd_port=8125, branch="dev"):
        """Initialize Datadog configuration."""
        self.branch = branch
        self.branchTag = ["branch:" + branch] if branch else []
        self.options = {"statsd_host": statsd_host, "statsd_port": statsd_port}
        initialize(**self.options)

    def sendGaugeMetric(self, metricName, metricValue, tags=None):
        """Send Gauge Metric."""
        if tags is None:
            tags = []
        tags += self.branchTag
        statsd.gauge(metricName, metricValue, tags=tags)


def parse_html_report(file_path):
    """Parse the HTML report and extract overall test results."""
    with open(file_path, "r") as file:
        html_content = file.read()
    passed_tests = 0
    failed_tests = 0
    skipped_tests = 0
    errors_tests = 0

    # Regex to extract test results
    passed_match = re.search(r'<span class="passed">(\d+) passed</span>', html_content)
    failed_match = re.search(r'<span class="failed">(\d+) failed</span>', html_content)
    skipped_match = re.search(r'<span class="skipped">(\d+) skipped</span>', html_content)
    errors_match = re.search(r'<span class="error">(\d+) errors</span>', html_content)

    if passed_match:
        passed_tests = int(passed_match.group(1))
    if failed_match:
        failed_tests = int(failed_match.group(1))
    if skipped_match:
        skipped_tests = int(skipped_match.group(1))
    if errors_match:
        errors_tests = int(errors_match.group(1))
    return passed_tests, failed_tests, skipped_tests, errors_tests


def standardize_module_name(name):
    """Standardize module name by capitalizing and formatting."""
    # Remove "test_" prefix and any characters that aren't letters or underscores
    name = re.sub(r"^test_", "", name, flags=re.IGNORECASE)
    name = re.sub(r"[^a-zA-Z_]", "", name)
    # Split by underscores, capitalize each part, and join back with underscores
    name_parts = name.lower().split("_")
    standardized_name = "_".join(part.capitalize() for part in name_parts if part)
    return standardized_name


def extract_module_results_from_html(file_path, module_names):
    """Extract module-specific test results dynamically based on standardized module names."""
    # Create a standardized module results dictionary
    standardized_module_names = [standardize_module_name(module) for module in module_names]
    module_results = {
        module: {"Passed": 0, "Failed": 0, "Error": 0} for module in standardized_module_names
    }
    with open(file_path, "r", encoding="utf-8") as file:
        html_content = file.read()

    # Parse the HTML using BeautifulSoup
    soup = BeautifulSoup(html_content, "html.parser")

    # Find all rows in the results table
    rows = soup.select("table#results-table tbody tr")

    # Define regex pattern to match module names
    module_pattern = re.compile(r"\btest_([a-zA-Z_]+)\b", re.IGNORECASE)

    # Process each row and extract module name and result
    for row in rows:
        test_case_name_td = row.find("td", {"class": "col-name"})
        result_td = row.find("td", {"class": "col-result"})

        # Skip the row if the necessary columns are not found
        if not test_case_name_td or not result_td:
            continue

        test_case_name = test_case_name_td.text.strip()
        result = result_td.text.strip().lower()  # Convert result to lowercase for uniformity

        # Debug print to verify extracted result and test case name
        print(f"Extracted test case: '{test_case_name}', result: {result}")

        # Apply regex to extract the module name
        match = module_pattern.search(test_case_name)

        if match:
            raw_module_name = match.group(1)  # Extract matched module name
            standardized_module_name = standardize_module_name(
                "test_" + raw_module_name
            )  # Standardize it
            # Check if the standardized name is in the expected module names
            if standardized_module_name in module_results:
                # Increment counters based on the result
                if result == "passed":
                    module_results[standardized_module_name]["Passed"] += 1
                elif result == "failed":
                    module_results[standardized_module_name]["Failed"] += 1
                elif result == "error":
                    module_results[standardized_module_name]["Error"] += 1
    return module_results


if __name__ == "__main__":
    """Argument parser to accept arguments from the command line."""
    parser = argparse.ArgumentParser(description="Send test results to Datadog.")
    parser.add_argument("--report_path", required=True, help="HTML report Path.")
    parser.add_argument("--variant", required=True, help="Variant Type.")
    parser.add_argument("--test_type", required=True, help="Test type SWE6 or SWE5.")

    args = parser.parse_args()

    # Initialize DataDogStats instance
    dataDogInst = dataDogStats()

    # Get the following parameters from command line arguments
    report_path = args.report_path
    variant = args.variant
    test_type = args.test_type

    if os.path.exists(report_path):
        # Extract Summary/overall test results
        passed_tests, failed_tests, skipped_tests, errors_tests = parse_html_report(report_path)

        print(
            f"Test Results: Passed={passed_tests}, Failed={failed_tests}, Skipped={skipped_tests}, Errors={errors_tests}"
        )

        metric_name = f"advradar.gen7.{test_type}.testcases"

        # Send overall test results to Datadog
        dataDogInst.sendGaugeMetric(
            metric_name,
            passed_tests,
            tags=["gen7_env:test", "Type:passed", f"Variant:{variant}"],
        )
        dataDogInst.sendGaugeMetric(
            metric_name,
            failed_tests,
            tags=["gen7_env:test", "Type:failed", f"Variant:{variant}"],
        )
        dataDogInst.sendGaugeMetric(
            metric_name,
            skipped_tests,
            tags=["gen7_env:test", "Type:skipped", f"Variant:{variant}"],
        )
        dataDogInst.sendGaugeMetric(
            metric_name,
            errors_tests,
            tags=["gen7_env:test", "Type:errors", f"Variant:{variant}"],
        )

        # Define module names (case-insensitive)
        module_names = [
            "Fault_Manager",
            "Measurement_Monitor",
            "Mode_Manager_Core",
            "FRONT_END_MANAGEMENT",
            "MCU_Configuration_And_Supervision",
            "SW_Radar_Status",
        ]
        # Extract module-specific test results
        module_results = extract_module_results_from_html(report_path, module_names)

        # Output and send module-specific results to Datadog
        for module, results in module_results.items():
            passed = results["Passed"]
            failed = results["Failed"]
            error = results["Error"]

            print(f"Module: {module} - Passed: {passed}, Failed: {failed}, Error: {error}")

            dataDogInst.sendGaugeMetric(
                metric_name,
                passed,
                tags=[
                    f'{"gen7_env:test"}',
                    f'{"Type:passed"}',
                    f"Variant:{variant}",
                    f"module:{module.lower()}",
                ],
            )
            dataDogInst.sendGaugeMetric(
                metric_name,
                failed,
                tags=[
                    f'{"gen7_env:test"}',
                    f'{"Type:failed"}',
                    f"Variant:{variant}",
                    f"module:{module.lower()}",
                ],
            )
            dataDogInst.sendGaugeMetric(
                metric_name,
                error,
                tags=[
                    "gen7_env:test",
                    "Type:error",
                    f"Variant:{variant}",
                    f"module:{module.lower()}",
                ],
            )
        print("Test Results sent to Datadog for each module.")
    else:
        print(f"HTML file not found: {report_path}")
