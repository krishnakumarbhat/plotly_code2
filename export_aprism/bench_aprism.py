"""A-PRISM export benchmark: run shipped prototypes, report PASS/FAIL + time.

Usage: python bench_aprism.py [--quick]
  --quick: run the 6 new kernels only (default: all 12 shipped files).
Writes bench_aprism_report.txt next to this script.
"""
from __future__ import annotations

import subprocess
import sys
import time
from pathlib import Path

NEW = ["hermite_clothoid.py", "occupancy_blue.py", "yawfree_clock.py",
       "specular_contact.py", "guarded_replay.py", "gram_lift.py",
       "n23_residual_track.py", "n24_rank_gate.py", "n25_ghost_yaw.py",
       "imm_accel_gate.py", "blue_closed_loop.py", "gram_filter_compare.py"]
LEGACY = ["specular_ghost.py", "combined_chain.py", "adaptive_gating.py",
          "dual_loop_cfar.py", "parallel_kf.py", "run_regression.py"]
# run_regression re-runs everything; only include it in full mode.
FULL = NEW + ["specular_ghost.py", "combined_chain.py", "adaptive_gating.py",
              "dual_loop_cfar.py", "doppler_disambiguation.py", "parallel_kf.py"]


def main() -> int:
    root = Path(__file__).resolve().parent
    files = NEW if "--quick" in sys.argv else FULL
    results = []
    for name in files:
        t0 = time.time()
        p = subprocess.run([sys.executable, str(root / "resim_research" / name)],
                           cwd=root, capture_output=True, text=True,
                           timeout=600, check=False)
        dt = time.time() - t0
        ok = p.returncode == 0
        results.append((name, ok, dt, p.stdout.strip().splitlines()[-1] if p.stdout.strip() else ""))
        print(f"{name}: {'PASS' if ok else 'FAIL'} ({dt:.1f}s)")
        if not ok:
            print(p.stdout[-2000:])
            print(p.stderr[-2000:])
    n_ok = sum(1 for _, ok, _, _ in results if ok)
    report = "\n".join(f"{n} {'PASS' if ok else 'FAIL'} {dt:.1f}s :: {tail}"
                       for n, ok, dt, tail in results)
    report += f"\nRESULT {n_ok}/{len(results)} PASS"
    (root / "bench_aprism_report.txt").write_text(report, encoding="utf-8")
    print(report.splitlines()[-1])
    return 0 if n_ok == len(results) else 1


if __name__ == "__main__":
    raise SystemExit(main())
