"""

Parse a given text based detailed report for CI test results and SRS results, and generate pie charts for overall test and SRS results.

"""

import argparse
import matplotlib.pyplot as plt


def generate_pie(data, labels, colors, title, filename):
    """Generate a pie chart. Only includes non-zero wedges."""
    # Filter out zero-value entries to avoid misleading empty wedges
    filtered = [(d, l, c) for d, l, c in zip(data, labels, colors) if d > 0]
    if not filtered:
        return False
    data_f, labels_f, colors_f = zip(*filtered)
    plt.figure(figsize=(4, 4))
    plt.pie(
        data_f,
        labels=labels_f,
        colors=colors_f,
        autopct="%1.1f%%",
        startangle=140,
        textprops={"color": "white", "fontsize": 8},
    )
    plt.title(title, color="white")
    plt.gca().set_facecolor("#0f172a")
    plt.gcf().patch.set_facecolor("#0f172a")
    plt.savefig(filename, bbox_inches="tight", dpi=120)
    plt.close()
    return True


def main():
    """Main function for pi-chart generation."""
    parser = argparse.ArgumentParser()

    # Test inputs
    parser.add_argument("--passed-tests", type=int, required=True)
    parser.add_argument("--failed-tests", type=int, required=True)
    parser.add_argument("--skipped-tests", type=int, required=True)
    parser.add_argument("--error-tests", type=int, required=True)

    # SRS inputs
    parser.add_argument("--passed-srs", type=int, required=True)
    parser.add_argument("--failed-srs", type=int, required=True)
    parser.add_argument("--error-srs", type=int, required=True)
    args = parser.parse_args()

    # -------- TEST PIE --------
    test_data = [args.passed_tests, args.failed_tests, args.skipped_tests, args.error_tests]
    test_labels = ["Passed", "Failed", "Skipped", "Error"]
    test_colors = ["#22c55e", "#ef4444", "#f59e0b", "#8b5cf6"]
    if sum(test_data) > 0:
        generate_pie(
            test_data, test_labels, test_colors, "Overall Test Results", "test_results_pie.png"
        )

    # -------- SRS PIE --------
    srs_data = [args.passed_srs, args.failed_srs, args.error_srs]
    srs_labels = ["Passed", "Failed", "Error"]
    srs_colors = ["#22c55e", "#ef4444", "#8b5cf6"]
    if sum(srs_data) > 0:
        generate_pie(
            srs_data, srs_labels, srs_colors, "Overall SRS Results", "srs_results_pie.png"
        )


if __name__ == "__main__":
    main()
