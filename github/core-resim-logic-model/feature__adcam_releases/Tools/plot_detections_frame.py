#!/usr/bin/env python3
"""
plot_detections_frame.py

Visualises detections_frame_log.csv (written once by
STLA_Small_IFV600_Converter::log_detections_and_mounting_frame) for a single
simulation frame.

Two side-by-side scatter plots:
  1. OSI sensor-local cartesian (osi_x/osi_y) - one color per sensor, origin
     at each sensor's own mounting point (no offset applied).
  2. VCS cartesian (vcs_x/vcs_y) - all sensors combined in the vehicle frame
     (origin = front bumper center, x forward, y left), plus mounting-point
     markers.

Usage:
    python3 plot_detections_frame.py [path/to/detections_frame_log.csv]
"""
import sys
import csv
from collections import defaultdict

import matplotlib.pyplot as plt

SENSOR_COLORS = {
    "FR": "tab:orange",
    "RR": "tab:purple",
    "RL": "tab:blue",
    "FC": "tab:green",
}


def load_csv(path):
    rows = defaultdict(list)
    mounts = {}
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            sensor = row["sensor"]
            rows[sensor].append(row)
            mounts[sensor] = (
                float(row["mount_lon_mm"]) / 1000.0,
                float(row["mount_lat_mm"]) / 1000.0,
            )
    return rows, mounts


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "detections_frame_log.csv"
    rows, mounts = load_csv(path)

    if not rows:
        print(f"No data found in {path}")
        return

    fig, (ax_osi, ax_vcs) = plt.subplots(1, 2, figsize=(14, 8))

    for sensor, recs in rows.items():
        color = SENSOR_COLORS.get(sensor, "gray")

        osi_x = [float(r["osi_x_m"]) for r in recs]
        osi_y = [float(r["osi_y_m"]) for r in recs]
        ax_osi.scatter(osi_y, osi_x, s=12, color=color, label=sensor)

        vcs_x = [float(r["vcs_x_m"]) for r in recs]
        vcs_y = [float(r["vcs_y_m"]) for r in recs]
        ax_vcs.scatter(vcs_y, vcs_x, s=12, color=color, label=sensor)

    # Mounting points on VCS plot
    for sensor, (mx, my) in mounts.items():
        ax_vcs.scatter([my], [mx], marker="^", s=120,
                       color=SENSOR_COLORS.get(sensor, "gray"),
                       edgecolor="black", zorder=5)
    ax_vcs.scatter([0], [0], marker="s", s=140, color="red",
                   edgecolor="black", zorder=5, label="origin (bumper)")

    for ax, title in ((ax_osi, "OSI sensor-local (x fwd, y left)"),
                      (ax_vcs, "VCS vehicle frame (x fwd, y left)")):
        ax.set_title(title)
        ax.set_xlabel("y (left) [m]  -- plotted on X axis")
        ax.set_ylabel("x (forward) [m] -- plotted on Y axis")
        ax.invert_xaxis()  # left positive appears on the left side visually
        ax.axhline(0, color="lightgray", linewidth=0.8)
        ax.axvline(0, color="lightgray", linewidth=0.8)
        ax.set_aspect("equal", adjustable="datalim")
        ax.legend(loc="best", fontsize=8)
        ax.grid(True, linestyle=":", linewidth=0.5)

    fig.suptitle(f"Detections frame visualisation ({path})")
    fig.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
