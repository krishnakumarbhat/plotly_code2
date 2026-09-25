"""

Parse a given text based detailed comparison report for CI test results and SRS results, and send the new failures to platformHealth.

"""

import argparse
import pandas as pd
from platformHealthMetrics import platformHealthStats


parser = argparse.ArgumentParser()
parser.add_argument("--variant", required=True)
parser.add_argument("--gen_version", required=True)
parser.add_argument("--job_type", required=True)
args = parser.parse_args()
variant = args.variant
gen_version = args.gen_version
job_type = args.job_type


# Metric names
failed_test_metric = f"core_radar.{gen_version}.{job_type}.failed_testresults"
failed_srs_metric = f"core_radar.{gen_version}.{job_type}.failed_srsresults"

# INIT DB CONNECTION
ph = platformHealthStats(commitid="compare_job", project="comparison", branch="dev")
conn = ph.connection
cursor = conn.cursor()

# STEP 1: Get last 2 Run IDs.
run_query = """
SELECT
    metric_tags->>'RunID' as run_id,
    MAX(date) as latest_time
FROM wrsd."perception_metrics"
WHERE metric_name = %s
AND LOWER(metric_tags->>'Variant') = LOWER(%s)
GROUP BY run_id
ORDER BY latest_time DESC
LIMIT 2;
"""

cursor.execute(run_query, (failed_test_metric, variant))
runs = cursor.fetchall()

if len(runs) < 2:
    print("Not enough runs to compare")
    exit(1)
latest_run = runs[0][0]
prev_run = runs[1][0]

print("\n========== RUN INFO ==========")
print(f"Latest Run   : {latest_run}")
print(f"Previous Run : {prev_run}")

# STEP 2: Fetch Failed Test Data.
test_query = """
SELECT
    metric_tags->>'TestName' as test_name,
    metric_tags->>'RunID' as run_id
FROM wrsd."perception_metrics"
WHERE metric_name = %s
AND LOWER(metric_tags->>'Variant') = LOWER(%s)
AND metric_tags->>'RunID' IN (%s, %s)
"""

cursor.execute(test_query, (failed_test_metric, variant, latest_run, prev_run))
rows = cursor.fetchall()
df_test = pd.DataFrame(rows, columns=["test_name", "run_id"])

# STEP 3: Test Comparison.
latest_tests = set(df_test[df_test["run_id"] == latest_run]["test_name"])
prev_tests = set(df_test[df_test["run_id"] == prev_run]["test_name"])
new_failed_tests = latest_tests - prev_tests

# STEP 4: Fetch Failed SRS Data.
srs_query = """
SELECT
    metric_tags->>'SRS_ID' as srs_id,
    metric_tags->>'RunID' as run_id
FROM wrsd."perception_metrics"
WHERE metric_name = %s
AND LOWER(metric_tags->>'Variant') = LOWER(%s)
AND metric_tags->>'RunID' IN (%s, %s)
"""
cursor.execute(srs_query, (failed_srs_metric, variant, latest_run, prev_run))
rows = cursor.fetchall()
df_srs = pd.DataFrame(rows, columns=["srs_id", "run_id"])

# STEP 5: SRS Comparison.
latest_srs = set(df_srs[df_srs["run_id"] == latest_run]["srs_id"])
prev_srs = set(df_srs[df_srs["run_id"] == prev_run]["srs_id"])
new_failed_srs = latest_srs - prev_srs

# STEP 6: Print Results.
print("\n========== FAILED TEST COMPARISON ==========\n")
print(f"New Failed Test Count: {len(new_failed_tests)}\n")
if new_failed_tests:
    print("New Failed Tests:")
    for t in sorted(new_failed_tests):
        print(f" - {t}")
else:
    print("No new failed tests")

print("\n========== FAILED SRS COMPARISON ==========\n")
print(f"New Failed SRS Count: {len(new_failed_srs)}\n")
if new_failed_srs:
    print("New Failed SRS IDs:")
    for s in sorted(new_failed_srs):
        print(f" - {s}")
else:
    print("No new failed SRS")

# Cleanup
cursor.close()
conn.close()
