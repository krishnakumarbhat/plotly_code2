"""
Collect a True count of all of the UT functions.

Collect all of the *unit_test.cc files and parse each function looking for ASSERT or EXPECT
and calculate a percentage for each UT file.
"""
import os
import json
import argparse
import pandas as pd
from datetime import datetime
from openpyxl import load_workbook
import sys
import subprocess
import shutil


def get_bazel_workspace():
    """Finds the Bazel workspace directory."""
    workspace = os.getenv("BUILD_WORKSPACE_DIRECTORY")

    if workspace:
        print(f"Using BUILD_WORKSPACE_DIRECTORY: {workspace}")
        return workspace
    try:
        result = subprocess.run(
            ["bazel", "info", "workspace"], capture_output=True, text=True, check=True
        )
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        print(f"Error getting Bazel workspace: {e}")
        sys.exit(1)


def process_unit_test(path_name, relative_path, file_analysis):
    """Function to process unit test files and store results."""
    with open(path_name, "r") as file:
        is_present = False
        assertion_dict = {}
        test_name = ""

        print(f"Processing: {relative_path}")  # Print relative path

        for line in file:
            if "TEST_F(" in line:
                if is_present:
                    assertion_dict[test_name] = True
                    is_present = False
                else:
                    assertion_dict[test_name] = False
                test_name = line.strip()

                for line in file:
                    test_name += line.strip()
                    if ")" in test_name:
                        break
                test_name = test_name.rsplit(",", 1)[-1].replace(")", "").replace("{", "").strip()

            if "ASSERT" in line or "EXPECT" in line:
                is_present = True

        assertion_dict[test_name] = is_present
        assertion_dict.pop("", None)

        true_count = sum(assertion_dict.values())
        false_count = len(assertion_dict) - true_count
        total = true_count + false_count

        if total != 0:
            perc = round((true_count / total) * 100, 2)
            file_analysis[relative_path] = {
                "True Count": true_count,
                "False Count": false_count,
                "Total": total,
                "Percentage": perc,
                "Assertions": assertion_dict,  # Store detailed assertion results per test
            }


def find_unit_tests(root_folder, file_analysis):
    """Function to find unit test files in the root folder."""
    count = 0

    for dirpath, _, filenames in os.walk(root_folder):
        for filename in filenames:
            if filename.endswith("unit_test.cc"):
                abs_path = os.path.join(dirpath, filename)
                rel_path = os.path.relpath(abs_path, root_folder)  # Keep relative path
                rel_path = rel_path.replace("\\", "/")  # Convert to forward slashes
                count += 1
                print(f"Processing file {count}: {rel_path}")
                process_unit_test(abs_path, rel_path, file_analysis)


def save_to_json(file_analysis, json_filename, project_folder):
    """Function to save results to JSON."""
    json_path = os.path.abspath(json_filename)
    with open(json_path, "w", encoding="utf-8") as json_file:
        json.dump(file_analysis, json_file, indent=4)

    out_path = os.path.join(project_folder, json_filename)
    shutil.copy(json_path, out_path)

    print(f"JSON report '{json_filename}' created successfully at: {out_path}!")


def save_to_excel(
    file_analysis, excel_filename, report_title, commit_hash, branch, project_folder
):
    """Function to save results to Excel."""
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    excel_path = os.path.abspath(excel_filename)
    with pd.ExcelWriter(excel_path, engine="openpyxl") as writer:
        # Summary Sheet
        summary_data = []
        total_true, total_false, total_tests = 0, 0, 0

        for file, stats in file_analysis.items():
            total_true += stats["True Count"]
            total_false += stats["False Count"]
            total_tests += stats["Total"]
            summary_data.append(
                [
                    file,
                    stats["True Count"],
                    stats["False Count"],
                    stats["Total"],
                    stats["Percentage"],
                ]
            )

            # Save function details in separate sheets
            function_data = [
                [
                    func,
                    1 if result else 0,
                    1 - (1 if result else 0),
                    1,
                    round((1 if result else 0) * 100, 2),
                ]
                for func, result in stats["Assertions"].items()
            ]
            df_function = pd.DataFrame(
                function_data,
                columns=["Function", "True Count", "False Count", "Total", "Coverage (%)"],
            )
            df_function.to_excel(writer, sheet_name=os.path.basename(file)[:30], index=False)

        total_coverage = round((total_true / total_tests) * 100, 2) if total_tests > 0 else 0
        summary_data.append(["TOTAL", total_true, total_false, total_tests, total_coverage])

        df_summary = pd.DataFrame(
            summary_data, columns=["File", "True Count", "False Count", "Total", "Coverage (%)"]
        )
        df_summary.to_excel(writer, sheet_name="Summary", index=False)

        # Metadata Sheet
        metadata = [["Report Title", report_title], ["Generated On", timestamp]]
        if commit_hash:
            metadata.append(["Commit Hash", commit_hash])
        if branch:
            metadata.append(["Branch", branch])

        df_metadata = pd.DataFrame(metadata, columns=["Attribute", "Value"])
        df_metadata.to_excel(writer, sheet_name="Metadata", index=False)

    wb = load_workbook(excel_filename)
    summary_sheet = wb["Summary"]  # copy summary sheet
    wb._sheets.remove(summary_sheet)  # delete the summary sheet
    wb._sheets.insert(0, summary_sheet)  # insert copied summary sheet to the begining

    out_path = os.path.join(project_folder, excel_filename)
    shutil.copy(excel_path, out_path)

    print(f"Excel report '{excel_filename}' created successfully at: {out_path}!")

    # Same summary data
    summary_dict = {
        "total_true": total_true,
        "total_false": total_false,
        "total_tests": total_tests,
        "total_coverage": total_coverage,
    }

    return summary_dict


def main():
    """Main function to parse arguments and process unit test files."""
    parser = argparse.ArgumentParser(
        description="Generate a unit test report (JSON & Excel) from parsed files."
    )

    parser.add_argument(
        "--json_output",
        type=str,
        default="ut_true_count_report.json",
        help="Path to the output JSON file",
    )
    parser.add_argument(
        "--excel_output",
        type=str,
        default="ut_true_count_report.xlsx",
        help="Path to the output Excel file",
    )
    parser.add_argument(
        "--title", type=str, default="Unit Test Tru Count Report", help="Custom report title"
    )
    parser.add_argument("--commit", type=str, default="", help="Optional commit hash")
    parser.add_argument("--branch", type=str, default="", help="Optional branch name")
    args = parser.parse_args()

    project_folder = get_bazel_workspace()

    file_analysis = {}  # Dictionary to store test file results

    find_unit_tests(project_folder, file_analysis)

    # Save results to both JSON and Excel
    (summary) = save_to_excel(
        file_analysis, args.excel_output, args.title, args.commit, args.branch, project_folder
    )
    file_analysis.update(summary)
    save_to_json(file_analysis, args.json_output, project_folder)

    print("\n**************************************")
    print("Summary: ")
    print(f"Total False Count : {summary['total_false']}")
    print(f"Total True Count  : {summary['total_true']}")
    print(f"Total Test Count  : {summary['total_tests']}")
    print(f"Total Coverage    : {summary['total_coverage']}%")
    print("**************************************")


if __name__ == "__main__":
    main()
