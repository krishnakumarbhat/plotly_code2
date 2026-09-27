"""Probe the bundled MDF decoder on one copied SiL ORCAS output.

Each test uses a fresh private input directory/config, captures stdout/stderr,
and lists created artifacts. Nothing in Research/ or the primary SiL output
directory is modified.
"""
import json
import shutil
import subprocess
from pathlib import Path
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
RUNTIME = ROOT / "resim_research/sil_runtime"
DECODER = RUNTIME / "decoder"
ENGINE = RUNTIME / "gen7/output/ORCAS"


def probe(key: str, enable_canoe: bool) -> dict:
    """Run one decoder route, return exit/output evidence."""
    tag = f"{key}_{'canoe' if enable_canoe else 'no_output'}"
    base = DECODER / "probes" / tag
    srcdir, outdir = base / "in", base / "out"
    srcdir.mkdir(parents=True, exist_ok=True)
    outdir.mkdir(parents=True, exist_ok=True)
    src = next(ENGINE.glob("CEER_S12*MF4"))
    inp = srcdir / src.name
    shutil.copy2(src, inp)
    master = DECODER / "config" / "converter_master.xml"
    tree = ET.parse(master)
    root = tree.getroot()
    for node in root.iter():
        if node.tag.endswith("_OUTPUT_ENABLE"):
            node.text = "0"
    if enable_canoe:
        node = root.find("CANoe_MDF4_OUTPUT_ENABLE")
        if node is not None:
            node.text = "1"
    node = root.find("CSV_ENABLE")
    if node is not None:
        node.text = "1"
    node = root.find("CONVERTOR_OUTPUT_PATH")
    if node is not None:
        node.text = "OUTPUT_INSIDE_CONVERTED_FOLDER"
    cfg = base / "master.xml"
    tree.write(cfg, encoding="utf-8", xml_declaration=True)
    json_path = base / "files.json"
    json_path.write_text(json.dumps({"reprocessingInputFileStreams": [
        {"key": key, "files": [inp.resolve().as_posix()]}
    ]}, indent=2), encoding="utf-8")
    exe = DECODER / "mdf_udpData_Proc.exe"
    env = __import__("os").environ.copy()
    env["PATH"] = str(DECODER) + __import__("os").pathsep + env.get("PATH", "")
    p = subprocess.run([str(exe), str(cfg), str(json_path)], cwd=DECODER,
                       env=env, capture_output=True, text=False, timeout=300,
                       check=False)
    log = base / "decoder.log"
    stdout = p.stdout.decode("cp1252", errors="replace")
    stderr = p.stderr.decode("cp1252", errors="replace")
    log.write_text(stdout + "\n--- STDERR ---\n" + stderr, encoding="utf-8")
    files = [(str(x.relative_to(base)), x.stat().st_size)
             for x in base.rglob("*") if x.is_file() and x not in (cfg, json_path, log)]
    result = {"key": key, "canoe": enable_canoe, "exit": p.returncode,
              "stdout": stdout, "stderr": stderr, "files": files,
              "log": str(log)}
    print(f"probe key={key} canoe={enable_canoe} exit={p.returncode} files={files}")
    print(stdout[-900:])
    return result


def main() -> None:
    """Run no-op route and conversion route; assert reported evidence saved."""
    results = [probe("DEBG", False), probe("SRR_DEBUG", True)]
    out = DECODER / "probes" / "summary.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(results, indent=2), encoding="utf-8")
    print("summary", out)


if __name__ == "__main__":
    main()
