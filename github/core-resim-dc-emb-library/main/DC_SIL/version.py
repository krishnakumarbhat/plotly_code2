#!/usr/bin/env python3
import argparse
import re
import sys
from pathlib import Path
from typing import Optional


def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="ignore")
    except Exception as e:
        print(f" Failed to read {path}: {e}", file=sys.stderr)
        return ""


def parse_int(token: str) -> int:
    token = token.strip() #Strips common C suffixes
    token = re.sub(r'[uUlL]+$', '', token)  # remove trailing u/U/l/L/ul/UL
    return int(token) # Convert tokens like '5', '5u', '10U', '3UL', '3ul' to int

# tracker header file parser
def parse_tracker_version(text: str) -> Optional[str]:
    major = None
    minor = None
    patch = None

    major_re = re.search(r'Tracker_Version_Major\s*=\s*([0-9]+)\s*;', text)
    minor_re = re.search(r'Tracker_Version_Minor\s*=\s*([0-9]+)\s*;', text)
    patch_re = re.search(r'Tracker_Version_Patch\s*=\s*([0-9]+)\s*;', text)

    if major_re:
        major = parse_int(major_re.group(1))
    if minor_re:
        minor = parse_int(minor_re.group(1))
    if patch_re:
        patch = parse_int(patch_re.group(1))

    if major is not None and minor is not None and patch is not None:
        return f"{major}.{minor}.{patch}"
    return None

# ocg header file parser
def parse_ocg_version(text: str) -> Optional[str]:
    major = None
    minor = None
    patch = None

    major_re = re.search(r'\bm_major\s*=\s*([0-9]+)\s*;', text)
    minor_re = re.search(r'\bm_minor\s*=\s*([0-9]+)\s*;', text)
    patch_re = re.search(r'\bm_patch\s*=\s*([0-9]+)\s*;', text)

    if major_re:
        major = parse_int(major_re.group(1))
    if minor_re:
        minor = parse_int(minor_re.group(1))
    if patch_re:
        patch = parse_int(patch_re.group(1))

    if major is not None and minor is not None and patch is not None:
        return f"{major}.{minor}.{patch}"
    return None

# olp header file parser
def parse_olp_version(text: str) -> Optional[str]:
    major = None
    minor = None
    patch = None

    major_re = re.search(r'#\s*define\s+OLP_SW_MAJOR_VERSION\s+([0-9]+[uUlL]*)', text)
    minor_re = re.search(r'#\s*define\s+OLP_SW_MINOR_VERSION\s+([0-9]+[uUlL]*)', text)
    patch_re = re.search(r'#\s*define\s+OLP_SW_PATCH_VERSION\s+([0-9]+[uUlL]*)', text)

    if major_re:
        major = parse_int(major_re.group(1))
    if minor_re:
        minor = parse_int(minor_re.group(1))
    if patch_re:
        patch = parse_int(patch_re.group(1))

    if major is not None and minor is not None and patch is not None:
        return f"{major}.{minor}.{patch}"
    return None


def main() -> int:
    ap = argparse.ArgumentParser(description="Generate versions.txt from tracker/ocg/olp headers.")
    ap.add_argument("--tracker", required=True, help="Path to f360_tracker_version.h")
    ap.add_argument("--ocg",     required=True, help="Path to ocg_version.h ")
    ap.add_argument("--olp",     required=True, help="Path to olp_iface.h")
    ap.add_argument("--out",     required=True, help="Path to versions.txt")
    ap.add_argument("--strict",  action="store_true", help="Fail if any version is missing")
    args = ap.parse_args()

    tracker_text = read_text(Path(args.tracker))
    ocg_text     = read_text(Path(args.ocg))
    olp_text     = read_text(Path(args.olp))

    tracker_ver = parse_tracker_version(tracker_text)
    ocg_ver     = parse_ocg_version(ocg_text)
    olp_ver     = parse_olp_version(olp_text)

    missing = []
    if tracker_ver is None:
        tracker_ver = "(not found)"
        missing.append("Tracker_version")
    if ocg_ver is None:
        ocg_ver = "(not found)"
        missing.append("OCG_version")
    if olp_ver is None:
        olp_ver = "(not found)"
        missing.append("OLP_version")

    # Write output
    out_path = Path(args.out)
    try:
        out_path.parent.mkdir(parents=True, exist_ok=True)
        with out_path.open("w", encoding="utf-8") as f:
            f.write(f"Tracker_version: {tracker_ver}\n")
            f.write(f"OCG_version: {ocg_ver}\n")
            f.write(f"OLP_version: {olp_ver}\n")

        print(f"Wrote {out_path}")
    except Exception as e:
        print(f"Failed to write {out_path}: {e}", file=sys.stderr)
        return 1

    if args.strict and missing:
        print(f"Missing versions for: {', '.join(missing)}", file=sys.stderr)
        return 2

    return 0

if __name__ == "__main__":
    sys.exit(main())
