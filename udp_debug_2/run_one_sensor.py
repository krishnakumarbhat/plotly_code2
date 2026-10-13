"""Run KPI for ONE sensor in a fresh process with peak-RSS monitoring.

Why: full multi-sensor runs grew to 13-31GB RSS and OOM-killed Windows
(tiny pagefile). One sensor per process bounds peak memory and lets the OS
reclaim everything on exit. Prints peak RSS so growth is visible.
A hard RSS cap aborts the process instead of threatening Windows.

Usage:
    python run_one_sensor.py SENSOR IN_H5 OUT_H5 OUT_DIR BASE_NAME
Env:
    KPI_RSS_CAP_MB (default 3000), KPI_EXECUTOR_MODE (default serial),
    KPI_LOG_LEVEL (set outside).
"""
import gc
import os
import sys
import threading
import time

import psutil

REPO = r"C:\Users\ouymc2\Desktop\udp_debug_2"
if REPO not in sys.path:
    sys.path.insert(0, REPO)

os.environ.setdefault("KPI_EXECUTOR_MODE", "serial")

_peak_mb = 0.0
_stop = False
# Hard self-cap: abort the process instead of threatening Windows.
# (A corrupt-header bug once drove RSS to 32GB and BSOD'd the machine.)
RSS_CAP_MB = float(os.environ.get("KPI_RSS_CAP_MB", "3000"))


def _sampler():
    global _peak_mb
    proc = psutil.Process(os.getpid())
    while not _stop:
        try:
            rss = proc.memory_info().rss / 1e6
            if rss > _peak_mb:
                _peak_mb = rss
            if rss > RSS_CAP_MB:
                print(
                    f"ABORT: RSS {rss:.0f} MB exceeded cap {RSS_CAP_MB:.0f} MB",
                    flush=True,
                )
                os._exit(99)
        except Exception:
            pass
        time.sleep(0.2)


def _rss_now():
    try:
        return psutil.Process(os.getpid()).memory_info().rss / 1e6
    except Exception:
        return -1.0


def main():
    global _stop
    sensor, in_h5, out_h5, out_dir, base = sys.argv[1:6]
    t = threading.Thread(target=_sampler, daemon=True)
    t.start()
    print(f"[mem] start RSS={_rss_now():.0f} MB", flush=True)

    t0 = time.time()
    from UDP_KPI.a_persistence_layer.hdf_wrapper import parse_for_kpi

    print(f"[mem] imports done RSS={_rss_now():.0f} MB", flush=True)
    html = parse_for_kpi(sensor, in_h5, out_dir, base, "KPI", out_h5)
    print(f"[mem] parse done RSS={_rss_now():.0f} MB peak={_peak_mb:.0f} MB", flush=True)
    del html
    gc.collect()
    _stop = True
    t.join(timeout=2)
    print(
        f"DONE sensor={sensor} elapsed={time.time()-t0:.1f}s "
        f"peak_RSS_MB={_peak_mb:.0f} final_RSS_MB={_rss_now():.0f}",
        flush=True,
    )


if __name__ == "__main__":
    main()
