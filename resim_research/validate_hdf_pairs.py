"""Run the production UDP-HDF and CAN-HDF KPI implementations on local pairs.

The experimental artifacts are output-only. This utility reads the existing
paired HDF fixtures (source/vehicle first, resim second) and writes reports
under resim_research/kpi_work/; it never edits input HDFs.
"""
import json
import logging
import os
import sys
from html.parser import HTMLParser
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Tables(HTMLParser):
    """Collect HTML table cell text for a compact KPI result summary."""

    def __init__(self):
        super().__init__()
        self.rows, self.row, self.cell = [], [], None

    def handle_starttag(self, tag, attrs):
        if tag == "tr":
            self.row = []
        elif tag in ("td", "th"):
            self.cell = ""

    def handle_data(self, data):
        if self.cell is not None:
            self.cell += data.strip()

    def handle_endtag(self, tag):
        if tag in ("td", "th") and self.cell is not None:
            self.row.append(self.cell)
            self.cell = None
        elif tag == "tr" and self.row:
            self.rows.append(self.row)


def parse_report(path: Path):
    p = Tables()
    p.feed(path.read_text(encoding="utf-8", errors="replace"))
    return p.rows


def run_udp(pairs, outdir):
    kpi = ROOT / "Research/Core_RESIM_KPI/main_html/all_services/KPI/UDP_KPI"
    sys.path.insert(0, str(kpi.parent))
    from UDP_KPI.a_persistence_layer.hdf_wrapper import parse_for_kpi

    rows = []
    for i, (veh, resim) in enumerate(pairs):
        report = Path(parse_for_kpi(
            "SENSOR1", str(veh), str(outdir), f"udp_pair_{i}", "udp", str(resim)))
        rows.append((veh.name, resim.name, str(report), parse_report(report)))
        print(f"UDP_HDF {veh.name} -> {resim.name}: {report}")
    return rows


def run_can(inp, out, outdir):
    script = ROOT / "Research/Core_RESIM_KPI/can_kpi_hdf/kpi_main.py"
    config = outdir / "can_pairs.json"
    config.write_text(json.dumps({
        "INPUT_HDF": [str(p.resolve()).replace("\\", "/") for p in inp],
        "OUTPUT_HDF": [str(p.resolve()).replace("\\", "/") for p in out],
    }, indent=2), encoding="utf-8")
    cmd = [sys.executable, str(script), str(config), str(outdir / "can_html")]
    import subprocess
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, check=False)
    (outdir / "can_kpi_stdout.log").write_text(p.stdout + p.stderr, encoding="utf-8")
    if p.returncode:
        raise RuntimeError(f"CAN HDF KPI failed rc={p.returncode}: {p.stderr[-2000:]}")
    reports = sorted((outdir / "can_html").glob("*.html"))
    print(f"CAN_HDF pairs={len(inp)} reports={len(reports)} stdout={outdir / 'can_kpi_stdout.log'}")
    for r in reports:
        print("CAN_HDF_REPORT", r, parse_report(r))
    return reports


def main():
    outdir = ROOT / "resim_research/kpi_work/final_hdf_validation"
    outdir.mkdir(parents=True, exist_ok=True)
    ud = ROOT / "edge_hdf/udp"
    udp_pairs = [
        (ud / "input/CCA_9010_3100099_DEBUG_20250828_163718_0001_r00070104.h5",
         ud / "output/CCA_9010_3100099_DEBUG_20250828_163718_0001.h5"),
        (ud / "input/CCA_9010_3100099_DEBUG_20260119_120656_0001_r00080104.h5",
         ud / "output/CCA_9010_3100099_DEBUG_20260119_120656_0001.h5"),
        (ud / "input/kpi_dummy_pair1_input.h5", ud / "output/kpi_dummy_pair1_output.h5"),
    ]
    can_in = [
        ROOT / "edge_hdf/can/input/CCA_9010_3100099_BUS_20260119_120641_0000_rR00080104_HDF.h5",
        ROOT / "edge_hdf/can/input/CCA_9010_3100099_BUS_20260119_120711_0002_rR00080104_HDF.h5",
        ROOT / "edge_hdf/can/input/IFV7XX_WBATR95060NC89209_20250919_083635_0000_b04_rR00100012_HDF.h5",
    ]
    can_out = [
        ROOT / "edge_hdf/can/output/CCA_9010_3100099_BUS_20260119_120641_0000_HDF.h5",
        ROOT / "edge_hdf/can/output/CCA_9010_3100099_BUS_20260119_120711_0002_HDF.h5",
        ROOT / "edge_hdf/can/output/IFV7XX_WBATR95060NC89209_20250919_083635_0000_b04_HDF.h5",
    ]
    for a, b in udp_pairs:
        if not a.is_file() or not b.is_file():
            raise FileNotFoundError((a, b))
    run_udp(udp_pairs, outdir / "udp_html")
    run_can(can_in, can_out, outdir)
    print("HDF KPI PAIR VALIDATION COMPLETE")


if __name__ == "__main__":
    main()
