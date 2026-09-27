"""Run every self-check prototype; save complete stdout and return codes."""
from __future__ import annotations

import subprocess
import sys
from pathlib import Path


def main() -> int:
    """Run prototype demos sequentially and write a machine-readable log."""
    root = Path(__file__).resolve().parents[1]
    files = [
        "adaptive_gating.py", "async_motion_compensation.py",
        "bend_conditioned.py", "combined_chain.py", "conformal_fusion.py",
        "doppler_disambiguation.py", "doppler_radarsplat_mvp.py",
        "dual_loop_cfar.py", "ego_ghost_alignment.py", "fast_da.py",
        "ghost_augmentation.py", "hybrid_consensus.py", "parallel_kf.py",
        "sector_clutter_map.py", "spectral_covariance_scaling.py",
        "specular_ghost.py", "synthetic_generator.py", "track_conditioned.py",
    ]
    logs, failures = [], []
    for name in files:
        p = subprocess.run(
            [sys.executable, str(root / "resim_research" / name)],
            cwd=root, capture_output=True, text=True, timeout=600, check=False,
        )
        logs.append(f"=== {name} rc={p.returncode} ===\n{p.stdout}\n{p.stderr}")
        print(f"{name}: {'PASS' if p.returncode == 0 else 'FAIL'}")
        if p.returncode:
            failures.append(name)
    out = root / "resim_research" / "kpi_work" / "final_prototype_regression.log"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(logs), encoding="utf-8")
    print(f"RESULT {len(files)-len(failures)}/{len(files)} PASS; failed={failures}; log={out}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
