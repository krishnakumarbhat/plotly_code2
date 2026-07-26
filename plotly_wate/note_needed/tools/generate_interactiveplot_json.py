import os
import json
import argparse

def make_output_path(input_path, suffix="_r00070104"):
    base, ext = os.path.splitext(input_path)
    return f"{base}{suffix}{ext}"

def find_h5_files(root):
    found = []
    for dirpath, _, filenames in os.walk(root):
        for fn in filenames:
            if fn.lower().endswith(".h5"):
                found.append(os.path.join(dirpath, fn))
    found.sort()
    return found

def generate_json(root, out_json, suffix="_r00070104"):
    inputs = find_h5_files(root)
    outputs = [make_output_path(p, suffix) for p in inputs]
    payload = {
        "INPUT_HDF": inputs,
        "OUTPUT_HDF": outputs
    }
    os.makedirs(os.path.dirname(out_json), exist_ok=True)
    with open(out_json, "w", encoding="utf-8") as f:
        json.dump(payload, f, indent=4)
    print(f"Wrote {len(inputs)} pairs to {out_json}")

def main():
    parser = argparse.ArgumentParser(description="Scan folder for .h5 and write InteractivePlot.json")
    parser.add_argument("--root", "-r", default=r"C:\Users\ouymc2\Desktop\plotly_code\hdf_db",
                        help="Root folder to scan (recursive)")
    parser.add_argument("--out", "-o", default=r"c:\git\Core_RESIM_KPI\all_services\tools\InteractivePlot.json",
                        help="Output JSON path")
    parser.add_argument("--suffix", "-s", default="_r00070104", help="Suffix to append before extension for output files")
    args = parser.parse_args()
    generate_json(args.root, args.out, args.suffix)

if __name__ == "__main__":
    main()
