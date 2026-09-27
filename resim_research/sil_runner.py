"""Prepare an isolated Gen7 APT_SRR_RESIM runtime from checked-in binaries.

Vendor sources under Research/ remain read-only. Runtime copy, configs,
logs, and SiL outputs all live in ignored resim_research/sil_runtime/.

Usage:
  python resim_research/sil_runner.py prepare <vehicle_b05.mf4>
  python resim_research/sil_runner.py run [timeout_seconds]
"""
import os
import shutil
import subprocess
import sys
import time
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RUNTIME = ROOT / "resim_research" / "sil_runtime" / "gen7"
VENDOR = ROOT / "Research" / "Core_RESIM_DC_Emb_Library" / "DC_SIL"
BINARY_DIR = VENDOR / "sil_executables" / "fw_dlls"
CONFIG_DIR = VENDOR / "sil_executables" / "dc_config" / "Gen7"


def prepare(input_mf4: Path) -> None:
    """Clone the vendor bundle/config into sandbox and rewrite paths there."""
    input_mf4 = input_mf4.resolve(strict=True)
    if not input_mf4.is_file() or input_mf4.suffix.lower() != ".mf4":
        raise ValueError(f"Expected a real vehicle .mf4: {input_mf4}")
    if RUNTIME.exists():
        shutil.rmtree(RUNTIME)
    RUNTIME.mkdir(parents=True)
    shutil.copytree(BINARY_DIR, RUNTIME, dirs_exist_ok=True)
    cfgdir = RUNTIME / "config"
    shutil.copytree(CONFIG_DIR, cfgdir)
    dgps_src = VENDOR / "sil_executables" / "dgps_config"
    shutil.copytree(dgps_src, RUNTIME / "dgps_config")
    outdir = RUNTIME / "output"
    outdir.mkdir()
    in_copy = RUNTIME / "input" / input_mf4.name
    in_copy.parent.mkdir()
    shutil.copy2(input_mf4, in_copy)
    (cfgdir / "SIL_Input.txt").write_text(str(in_copy) + "\n", encoding="utf-8")

    engine = cfgdir / "SIL_Engine_Config.xml"
    tree = ET.parse(engine)
    xml = tree.getroot()
    def set_text(parent: str, child: str, value: str) -> None:
        node = xml.find(f"./{parent}/{child}")
        if node is not None:
            node.text = value
    set_text("RESIM_OUTPUT_PATH", "Output_Path_Options", "SAME_AS_OUTPUT")
    set_text("RECU_LIB_CONFIG_PATH", "RECU_CONFIG_00", str(cfgdir / "SRR_DC_Lib_Control.xml"))
    set_text("RECU_LIB_CONFIG_PATH", "RECU_CONFIG_01", str(cfgdir / "MRR_DC_Lib_Control.xml"))
    set_text("OUTPUT_FILE_FOLDER", "CREATE_VIDEO_FILE_ENABLE", "0")
    node = xml.find("./CREATE_VIDEO_FILE_ENABLE")
    if node is not None:
        node.text = "0"
    node = xml.find("./GENERATE_VIDEO_FILE")
    if node is not None:
        node.text = "DISABLE"
    node = xml.find("./RESIM_VIDEO_FRAME_TRANSMISSION")
    if node is not None:
        node.text = "DISABLE"
    node = xml.find("./RESIM_LOG_INFO/LogTracingPath")
    if node is not None:
        node.text = "SAME_AS_OUTPUT"
    tree.write(engine, encoding="utf-8", xml_declaration=True)

    dc = cfgdir / "SRR_DC_Lib_Control.xml"
    dt = ET.parse(dc)
    root = dt.getroot()
    node = root.find("DGPS_SM_Config")
    if node is not None:
        node.text = str(RUNTIME / "dgps_config")
    dt.write(dc, encoding="utf-8", xml_declaration=True)
    print(f"prepared runtime={RUNTIME}")
    print(f"input={in_copy} bytes={in_copy.stat().st_size}")
    print(f"output={outdir}")
    print(f"exe={RUNTIME / 'APT_SRR_RESIM.exe'}")


def run(timeout_s: int = 3600) -> int:
    """Run the supplied engine on the sandbox input and capture complete logs."""
    exe = RUNTIME / "APT_SRR_RESIM.exe"
    cfg = RUNTIME / "config" / "SIL_Engine_Config.xml"
    inp = RUNTIME / "config" / "SIL_Input.txt"
    out = RUNTIME / "output"
    if not (exe.exists() and cfg.exists() and inp.exists()):
        raise FileNotFoundError("Run prepare <input.mf4> first")
    stamp = time.strftime("%Y%m%d_%H%M%S")
    log = RUNTIME / f"run_{stamp}.log"
    env = os.environ.copy()
    env["PATH"] = str(RUNTIME) + os.pathsep + env.get("PATH", "")
    cmd = [str(exe), "-c", str(cfg), "-p", str(inp), "-o", str(out)]
    print("CMD:", subprocess.list2cmdline(cmd))
    print("LOG:", log)
    with log.open("w", encoding="utf-8", errors="replace") as fp:
        try:
            p = subprocess.run(cmd, cwd=RUNTIME, env=env, stdout=fp,
                               stderr=subprocess.STDOUT, timeout=timeout_s,
                               check=False)
            print(f"exit={p.returncode}")
            return p.returncode
        except subprocess.TimeoutExpired:
            fp.write(f"\nTIMEOUT after {timeout_s}s\n")
            print(f"timeout after {timeout_s}s")
            return 124


if __name__ == "__main__":
    if len(sys.argv) < 2:
        raise SystemExit("usage: sil_runner.py prepare <input.mf4> | run [timeout]")
    if sys.argv[1] == "prepare" and len(sys.argv) == 3:
        prepare(Path(sys.argv[2]))
    elif sys.argv[1] == "run":
        raise SystemExit(run(int(sys.argv[2]) if len(sys.argv) > 2 else 3600))
    else:
        raise SystemExit("usage: sil_runner.py prepare <input.mf4> | run [timeout]")
