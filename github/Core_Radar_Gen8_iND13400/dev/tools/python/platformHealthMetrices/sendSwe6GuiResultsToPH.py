"""

Parse an HTML based Test execution report and extract the overall/modulewise results for Test-Cases and SRS.

Data extracted from Test report is sent to platformHealth to populate dashboards.

"""

import argparse
from bs4 import BeautifulSoup
from platformHealthMetrics import platformHealthStats
import uuid

# Run ID (run identifier will help tracking No. of results sending in a particular execution)
run_identifier = f"run_{uuid.uuid4().hex[:8]}"


class PlatformHealthTestJob:
    """Class for sending test results to Platform Health."""

    def __init__(self, commit_id, project="Core_Radar_Gen7_SAF85xx", branch="dev"):
        """Initialize Platform Health configuration."""
        self.platformHealthMetrics = platformHealthStats(
            commitid=commit_id, project=project, branch=branch
        )

    def send_metric(self, metric_name, metric_value, tags=None):
        """Send metric to Platform Health."""
        if tags is None:
            tags = {}
        tags["RunID"] = run_identifier  # Add RunID tag to all metrics
        self.platformHealthMetrics.sendMetric(metric_name, metric_value, tags)


def extract_failure_details(soup, ph, variant, job_type, gen_version):
    """Parse the HTML report and extract failed test-cases and srs results."""
    failed_test_metric = f"core_radar.{gen_version}.{job_type}.failed_testresults"
    failed_srs_metric = f"core_radar.{gen_version}.{job_type}.failed_srsresults"

    print("\n========== FAILED TEST CASE DETAILS ==========\n")

    failure_cards = soup.find_all("details", class_="card-details")
    for card in failure_cards:
        summary = card.find("summary")
        if not summary:
            continue
        # Process both FAIL/ERROR entries
        summary_text = summary.get_text().upper()
        if "FAIL" not in summary_text and "ERROR" not in summary_text:
            continue

        detail_content = card.find("div", class_="detail-content")
        if not detail_content:
            continue
        # Defaults
        module = "UNKNOWN"
        test_name = "UNKNOWN"
        srs_ids = "UNKNOWN"
        meta_items = detail_content.find_all("div", class_="failure-meta-item")
        for item in meta_items:
            label = item.find("span", class_="failure-meta-label")
            value = item.find("span", class_="failure-meta-value")
            if not label or not value:
                continue
            label_text = label.get_text(strip=True)
            value_text = value.get_text(strip=True)
            if "File Name" in label_text:
                test_name = value_text.replace(".py", "")
            elif "Module" in label_text:
                module = value_text
            elif "SRS" in label_text:
                srs_ids = value_text

        # Print (existing behavior)
        print(f"Module: {module}")
        print(f"SRS IDs: {srs_ids}")
        print(f"Test Name: {test_name}")
        print("-" * 60)

        # Send to Platform Health
        ph.send_metric(
            failed_test_metric,
            1,
            tags={
                "Module": module,
                "TestName": test_name,
                "SRS_IDs": srs_ids,
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )

    print("\n========== FAILED SRS DETAILS ==========\n")

    srs_rows = soup.find_all("tr", class_="srs-data-row", attrs={"data-srs-status": "failed"})
    for row in srs_rows:
        srs_id = row.find("td", class_="srs-id").text.strip()
        module_tags = row.find_all("span", class_="srs-mod-chip")
        modules = [m.text.strip() for m in module_tags]
        print(f"\nSRS ID: {srs_id}")
        print(f"Modules: {', '.join(modules)}")
        drill_row = row.find_next_sibling("tr")
        test_names = []
        if drill_row and "srs-drill-row" in drill_row.get("class", []):
            test_rows = drill_row.find_all("tr")
            print("Associated Failed Test Cases:")
            for tr in test_rows:
                cols = tr.find_all("td")
                if len(cols) >= 2:
                    test_name = cols[1].text.strip()
                    test_names.append(test_name)
                    print(f"  - {test_name}")
        ph.send_metric(
            failed_srs_metric,
            len(test_names),
            tags={
                "SRS_ID": srs_id,
                "Module": ",".join(modules),
                "TestNames": ",".join(test_names),
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )


def parse_html_report(file_path, ph, variant, job_type, gen_version):
    """Parse the HTML report and extract overall/modulewise results for test and srs."""
    with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
        soup = BeautifulSoup(f, "html.parser")

    extract_failure_details(soup, ph, variant, job_type, gen_version)

    # Metric names
    overall_test_metric = f"core_radar.{gen_version}.{job_type}.overall_testresults"
    module_test_metric = f"core_radar.{gen_version}.{job_type}.modulewise_testresults"
    overall_srs_metric = f"core_radar.{gen_version}.{job_type}.overall_srsresults"
    module_srs_metric = f"core_radar.{gen_version}.{job_type}.modulewise_srsresults"

    # Overall: Test Results
    total_tests = int(soup.find("div", class_="dash-count c-total").text.strip())
    passed = int(soup.find("div", class_="dash-count c-pass").text.strip())
    failed = int(soup.find("div", class_="dash-count c-fail").text.strip())
    skipped = int(soup.find("div", class_="dash-count c-skip").text.strip())
    errors = int(soup.find("div", class_="dash-count c-error").text.strip())

    print("\n========== TEST RESULTS ==========")
    print(
        f"TOTAL: {total_tests}, PASSED: {passed}, FAILED: {failed}, SKIPPED: {skipped}, ERRORS: {errors}"
    )
    for t, v in {
        "total": total_tests,
        "passed": passed,
        "failed": failed,
        "skipped": skipped,
        "error": errors,
    }.items():
        ph.send_metric(
            overall_test_metric,
            v,
            tags={
                "Type": t,
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )

    # Modulewise: Test Results
    module_table = soup.find("table", id="moduleSummaryTable")
    rows = module_table.find("tbody").find_all("tr")
    for row in rows:
        cols = row.find_all("td")
        module_name = cols[0].text.strip()
        total = int(cols[1].text.strip())
        passed = int(cols[2].text.strip())
        failed = int(cols[3].text.strip())
        skipped = int(cols[4].text.strip())
        errors = int(cols[5].text.strip())
        for t, v in {
            "total": total,
            "passed": passed,
            "failed": failed,
            "skipped": skipped,
            "error": errors,
        }.items():
            ph.send_metric(
                module_test_metric,
                v,
                tags={
                    "Type": t,
                    "Module": module_name,
                    "Variant": variant,
                    "GenVersion": gen_version,
                    "JobType": job_type,
                },
            )

    # Overall: SRS Results
    total_srs = int(soup.find("div", class_="dash-count c-total srs-dash-count").text.strip())
    srs_passed = int(soup.find("div", class_="dash-count c-pass srs-dash-count").text.strip())
    srs_failed = int(soup.find("div", class_="dash-count c-fail srs-dash-count").text.strip())
    srs_errors = total_srs - (srs_passed + srs_failed)

    print("\n========== SRS RESULTS ==========")
    print(f"TOTAL: {total_srs}, PASSED: {srs_passed}, FAILED: {srs_failed}, ERRORS: {srs_errors}")
    for t, v in {
        "total": total_srs,
        "passed": srs_passed,
        "failed": srs_failed,
        "error": srs_errors,
    }.items():
        ph.send_metric(
            overall_srs_metric,
            v,
            tags={
                "Type": t,
                "Variant": variant,
                "GenVersion": gen_version,
                "JobType": job_type,
            },
        )

    # Modulewise: SRS Results
    srs_table = soup.find("table", class_="srs-table")
    rows = srs_table.find("tbody").find_all("tr")
    for row in rows:
        cols = row.find_all("td")
        module_name = cols[0].text.strip()
        srs_count = int(cols[1].text.strip())
        passed = int(cols[2].text.strip())
        failed = int(cols[3].text.strip())
        errors = srs_count - (passed + failed)
        for t, v in {
            "total": srs_count,
            "passed": passed,
            "failed": failed,
            "error": errors,
        }.items():
            ph.send_metric(
                module_srs_metric,
                v,
                tags={
                    "Type": t,
                    "Module": module_name,
                    "Variant": variant,
                    "GenVersion": gen_version,
                    "JobType": job_type,
                },
            )


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--htmlfile", required=True)
    parser.add_argument("--commit_id", required=True)
    parser.add_argument("--variant", required=True)
    parser.add_argument("--job_type", required=True)
    parser.add_argument("--gen_version", required=True)
    args = parser.parse_args()
    ph = PlatformHealthTestJob(args.commit_id)
    parse_html_report(
        args.htmlfile,
        ph,
        args.variant,
        args.job_type,
        args.gen_version,
    )
