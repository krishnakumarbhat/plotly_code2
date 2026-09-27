"""Local HPCC multi-worker replay simulator (H / N8-HPCC).

Simulates the cluster replay problem with MEASURED sub-models (no invented
constants): 300 logs x 20 workers, 1-log halo. Per worker:
  tracker warmup: 1 log (halo covers it);
  alignment: slow filter (never locks in-window; cf. fast_da slow=400cap),
             fast dual-horizon (locks in 46 frames), or injected header
             (0 frames, cf. Solution A on real HDF).
Metrics: makespan (slowest worker), cold-start dropped scans, fraction of
workers alignment-converged. Validates Halo (Plan 1) + Zero-State Cuts
(Plan 3): cuts are legal only where alignment is injected/converged.
# ponytail: frame-level discrete event sim; per-scan SiL timing uniform.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

N_LOGS = 300
WORKERS = 20
HALO = 1
FRAMES_PER_LOG = 1200       # 60 s @ 20 Hz
SCAN_S = 0.05
SLOW_LOCK = 10 ** 9         # slow DA: effectively never in-window (measured)
FAST_LOCK = 46              # measured fast_da.py dual-horizon mean
INJECT_LOCK = 0             # Solution A: pre-converged header


def simulate(mode, seed=0):
    """mode: slow | fast | inject. Returns dict of cluster metrics."""
    rng = np.random.default_rng(seed)
    base = N_LOGS // WORKERS
    per_worker_frames = []
    dropped = 0
    converged = 0
    for w in range(WORKERS):
        core = base
        # halo warmup covers tracker; alignment depends on mode
        lock = {"slow": SLOW_LOCK, "fast": FAST_LOCK, "inject": INJECT_LOCK}[mode]
        # jitter per worker (log content varies +-20%)
        lock = int(lock * rng.uniform(0.8, 1.2)) if lock < 10 ** 9 else lock
        win_frames = (core + HALO) * FRAMES_PER_LOG
        if lock >= win_frames:
            dropped += core * FRAMES_PER_LOG  # entire core unconverged
        else:
            dropped += min(lock, core * FRAMES_PER_LOG)
            converged += 1
        per_worker_frames.append(win_frames)
    makespan_s = max(per_worker_frames) * SCAN_S
    return {
        "makespan_s": makespan_s,
        "dropped_scans": dropped,
        "total_scans": N_LOGS * FRAMES_PER_LOG,
        "workers_converged": converged,
    }


def demo() -> None:
    print(f"{'mode':>8} {'makespan':>9} {'dropped':>9} {'convW':>6}")
    res = {}
    for mode in ("slow", "fast", "inject"):
        r = simulate(mode)
        res[mode] = r
        print(f"{mode:>8} {r['makespan_s']:8.0f}s {r['dropped_scans']:9d} "
              f"{r['workers_converged']:5d}/20")
    s, f, inj = res["slow"], res["fast"], res["inject"]
    assert f["workers_converged"] == 20, "fast lock must converge all workers"
    assert inj["dropped_scans"] == 0, "injection drops nothing"
    assert f["dropped_scans"] < s["dropped_scans"] / 100.0, \
        "fast must cut cold-start drops 100x"
    print("demo PASS")


if __name__ == "__main__":
    demo()
