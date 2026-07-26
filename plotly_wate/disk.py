import sqlite3
import tempfile
import os
import time
import statistics

def run_sqlite_test(db_path, commits=50):
    if os.path.exists(db_path):
        os.remove(db_path)

    conn = sqlite3.connect(db_path)
    cur = conn.cursor()

    cur.execute("CREATE TABLE test_data (id INTEGER PRIMARY KEY, value TEXT)")
    conn.commit()

    times = []

    for i in range(commits):
        start = time.perf_counter()
        cur.execute("INSERT INTO test_data (value) VALUES (?)", (f"row-{i}",))
        conn.commit()
        end = time.perf_counter()
        times.append((end - start) * 1000)

    conn.close()

    return {
        "path": db_path,
        "min_ms": min(times),
        "max_ms": max(times),
        "avg_ms": statistics.mean(times),
        "median_ms": statistics.median(times),
        "total_ms": sum(times),
    }

tests = []

project_db = os.path.abspath("sqlite_project_mount_test.db")
tests.append(run_sqlite_test(project_db))

tmp_db = os.path.join(tempfile.gettempdir(), "sqlite_tmp_test.db")
tests.append(run_sqlite_test(tmp_db))

for result in tests:
    print("\nPath:", result["path"])
    print("Min ms:", round(result["min_ms"], 3))
    print("Max ms:", round(result["max_ms"], 3))
    print("Avg ms:", round(result["avg_ms"], 3))
    print("Median ms:", round(result["median_ms"], 3))
    print("Total ms:", round(result["total_ms"], 3))