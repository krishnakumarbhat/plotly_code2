#!/usr/bin/env python3
import argparse
import json
import sys
import xml.etree.ElementTree as ET
from pathlib import Path
import subprocess

def list_hdf_files(folder: Path):
    return sorted(folder.rglob("*.h5"))

def merge_no_duplicates(existing, new):
    seen = set(existing)
    result = list(existing)
    for item in new:
        if item not in seen:
            result.append(item)
            seen.add(item)
    return result

def build_paired_paths(hin_dir: Path, hout_dir: Path):
    in_files = list_hdf_files(hin_dir)
    out_files = list_hdf_files(hout_dir)
    out_index = {f.name: f for f in out_files}

    input_paths = []
    output_paths = []
    for in_f in in_files:
        out_f = out_index.get(in_f.name)
        if out_f:
            input_paths.append(str(in_f.resolve()))
            output_paths.append(str(out_f.resolve()))
    return input_paths, output_paths

def discover_hdf_pairs(root: Path):
    """Walk all subdirectories of `root` at any depth looking for sibling
    HDF_Input / HDF_Output directories.

    Expected layout (any nesting depth):
        root/
          .../subfolder_A/
                HDF_Input/   *.h5
                HDF_Output/  *.h5
          .../subfolder_B/
                HDF_Input/   *.h5
                HDF_Output/  *.h5
    """
    all_input = []
    all_output = []

    for hin in sorted(root.rglob("HDF_Input")):
        if not hin.is_dir():
            continue
        hout = hin.parent / "HDF_Output"
        if hout.is_dir():
            inp, out = build_paired_paths(hin, hout)
            if inp:
                print(f"  Found {len(inp)} pair(s) in {hin.parent.relative_to(root)}/")
                all_input.extend(inp)
                all_output.extend(out)

    return all_input, all_output

def discover_tracker_vs_tracker_pairs(root1: Path, root2: Path):
    """Walk root1 for all HDF_Output dirs and find matching HDF_Output at the
    same relative path under root2.

    Expected layout (both resim runs share the same folder structure):
        root1/.../subfolder/HDF_Output/  <-->  root2/.../subfolder/HDF_Output/
    Files are paired by filename across the two runs.
    """
    all_input = []
    all_output = []

    for hout1 in sorted(root1.rglob("HDF_Output")):
        if not hout1.is_dir():
            continue
        rel = hout1.relative_to(root1)
        hout2 = root2 / rel
        if hout2.is_dir():
            inp, out = build_paired_paths(hout1, hout2)
            if inp:
                print(f"  Found {len(inp)} pair(s) in {rel}/")
                all_input.extend(inp)
                all_output.extend(out)
        else:
            print(f"  SKIP: no matching HDF_Output in resim2 for {rel}/")

    return all_input, all_output

def read_config_from_xml(xml_path: Path):
    try:
        tree = ET.parse(xml_path)
        root = tree.getroot()
    except Exception as e:
        print(f"ERROR: Failed to read XML config: {e}", file=sys.stderr)
        sys.exit(1)

    def get(tag):
        elem = root.find(tag)
        if elem is None or not elem.text:
            print(f"ERROR: Missing or empty <{tag}> in {xml_path.name}", file=sys.stderr)
            sys.exit(1)
        return elem.text.strip()

    return get

def read_mode1_config(xml_path: Path):
    get = read_config_from_xml(xml_path)
    return {
        "hdf_root": Path(get("HDF_Root_Folder")),
        "input_json": Path(get("InputJSON_Path")),
        "html_config": Path(get("HTML_Config_Path")),
        "singularity_image": Path(get("Singularity_Image_Path")),
        "output_path": Path(get("Output_Path")),
    }

def read_mode2_config(xml_path: Path):
    get = read_config_from_xml(xml_path)
    return {
        "resim1_root": Path(get("ResimRun1_Root")),
        "resim2_root": Path(get("ResimRun2_Root")),
        "input_json": Path(get("InputJSON_Path")),
        "html_config": Path(get("HTML_Config_Path")),
        "singularity_image": Path(get("Singularity_Image_Path")),
        "output_path": Path(get("Output_Path")),
    }

def run_with_module_load(module_name: str, command: str):
    chained = f"module load {module_name} && {command}"
    try:
        subprocess.run(["bash", "-lc", chained], check=True)
    except subprocess.CalledProcessError as e:
        print("\n Command failed.", file=sys.stderr)
        print(f"Exit code: {e.returncode}", file=sys.stderr)
        sys.exit(e.returncode)
    except FileNotFoundError:
        print("ERROR: 'bash' not found. This script requires Bash to load modules.", file=sys.stderr)
        sys.exit(1)

def main():
    parser = argparse.ArgumentParser(description="HTML Report Builder")
    parser.add_argument(
        "--mode", type=int, choices=[1, 2], required=True,
        help="1 = GroundTruth vs TrackerOutput  |  2 = TrackerOutput vs TrackerOutput"
    )
    args = parser.parse_args()

    script_dir = Path(__file__).resolve().parent

    # ------------------------------------------------------------------ #
    # Mode 1 – GroundTruth vs TrackerOutput
    #   Reads: input.xml  (HDF_Root_Folder contains both HDF_Input & HDF_Output)
    # ------------------------------------------------------------------ #
    if args.mode == 1:
        xml_path = script_dir / "input.xml"
        if not xml_path.exists():
            print(f"ERROR: {xml_path} not found", file=sys.stderr)
            sys.exit(1)

        cfg = read_mode1_config(xml_path)
        hdf_root      = cfg["hdf_root"]
        json_path     = cfg["input_json"]
        html_config   = cfg["html_config"]
        sif_image     = cfg["singularity_image"]
        output_dir    = cfg["output_path"]

        print("=== HTML Report Builder  [Mode 1: GroundTruth vs TrackerOutput] ===")
        print(f"Config : {xml_path}")

        if not hdf_root.is_dir():
            print(f"ERROR: HDF root folder not found: {hdf_root}", file=sys.stderr)
            sys.exit(1)

        _validate_common(json_path, html_config, sif_image, output_dir)

        data = _load_json(json_path)
        print(f"\nScanning {hdf_root} for HDF_Input / HDF_Output pairs...")
        new_in, new_out = discover_hdf_pairs(hdf_root)

    # ------------------------------------------------------------------ #
    # Mode 2 – TrackerOutput vs TrackerOutput
    #   Reads: input_tracker_vs_tracker.xml  (ResimRun1_Root & ResimRun2_Root)
    #   Pairs HDF_Output from resim1 with same-path HDF_Output from resim2.
    # ------------------------------------------------------------------ #
    else:
        xml_path = script_dir / "input_tracker_vs_tracker.xml"
        if not xml_path.exists():
            print(f"ERROR: {xml_path} not found", file=sys.stderr)
            sys.exit(1)

        cfg = read_mode2_config(xml_path)
        resim1_root   = cfg["resim1_root"]
        resim2_root   = cfg["resim2_root"]
        json_path     = cfg["input_json"]
        html_config   = cfg["html_config"]
        sif_image     = cfg["singularity_image"]
        output_dir    = cfg["output_path"]

        print("=== HTML Report Builder  [Mode 2: TrackerOutput vs TrackerOutput] ===")
        print(f"Config  : {xml_path}")
        print(f"Resim 1 : {resim1_root}")
        print(f"Resim 2 : {resim2_root}")

        for root_path, label in [(resim1_root, "ResimRun1_Root"), (resim2_root, "ResimRun2_Root")]:
            if not root_path.is_dir():
                print(f"ERROR: {label} folder not found: {root_path}", file=sys.stderr)
                sys.exit(1)

        _validate_common(json_path, html_config, sif_image, output_dir)

        data = _load_json(json_path)
        print(f"\nMatching HDF_Output pairs across both resim runs...")
        new_in, new_out = discover_tracker_vs_tracker_pairs(resim1_root, resim2_root)

    # ------------------------------------------------------------------ #
    # Common: clear, populate, write JSON, run singularity
    # ------------------------------------------------------------------ #
    if not new_in:
        print("WARNING: No paired HDF files found. JSON will be empty.", file=sys.stderr)

    data["InputHDF"]  = new_in
    data["OutputHDF"] = new_out

    json_path.write_text(json.dumps(data, indent=2), encoding="utf-8")
    print(f"\n   InputHDF entries : {len(data['InputHDF'])}")
    print(f"   OutputHDF entries: {len(data['OutputHDF'])}")
    print("   JSON updated successfully")

    cmd = f'singularity run "{sif_image}" "{html_config}" "{json_path}" "{output_dir}"'
    run_with_module_load("singularity/3.8.0", cmd)
    print("\n Singularity execution completed successfully.")


def _validate_common(json_path, html_config, sif_image, output_dir):
    if not json_path.exists():
        print(f"ERROR: JSON file not found: {json_path}", file=sys.stderr)
        sys.exit(1)
    if not html_config.exists():
        print(f"ERROR: HTML config not found: {html_config}", file=sys.stderr)
        sys.exit(1)
    if not sif_image.exists():
        print(f"ERROR: Singularity image not found: {sif_image}", file=sys.stderr)
        sys.exit(1)
    output_dir.mkdir(parents=True, exist_ok=True)


def _load_json(json_path: Path) -> dict:
    try:
        data = json.loads(json_path.read_text(encoding="utf-8"))
    except Exception as e:
        print(f"ERROR: Could not parse JSON: {e}", file=sys.stderr)
        sys.exit(1)
    if "InputHDF" not in data or "OutputHDF" not in data:
        print("ERROR: JSON must contain 'InputHDF' and 'OutputHDF' arrays.", file=sys.stderr)
        sys.exit(1)
    if not isinstance(data["InputHDF"], list) or not isinstance(data["OutputHDF"], list):
        print("ERROR: 'InputHDF' and 'OutputHDF' must be arrays.", file=sys.stderr)
        sys.exit(1)
    return data


if __name__ == "__main__":
    main()