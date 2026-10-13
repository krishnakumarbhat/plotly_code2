#!/usr/bin/env python3

"""
Pre-commit hook to validate version.yaml.

This script checks that version.yaml (in the root of the repo) contains:
- MajorVersion, MinorVersion, PatchVersion - must all be integers, non-negative, and no leading zeros
- ShortName - Name used in the integration repos, must be a non-empty string (e.g. "AFBB", )

All version lines might include comments, so leading zero detection is done by parsing the
raw line before the '#' character.

Usage:
    Added automatically via pre-commit. Can also be run manually:
        python check-version-yaml.py version.yaml

Notes:
    - Leading zeros (e.g., 01) are flagged even though YAML may parse them as valid integers.
    - PatchVersion is treated as a plain integer, not a SemVer suffix.
"""

import sys
import yaml

REQUIRED_KEYS = {"MajorVersion", "MinorVersion", "PatchVersion", "ShortName"}
VERSION_KEYS = {"MajorVersion", "MinorVersion", "PatchVersion"}


def get_original_line(file_path, key):
    """Get original lines."""
    with open(file_path, "r") as f:
        for line in f:
            if line.strip().startswith(f"{key}:"):
                return line.split(":", 1)[1].strip()
    return None


def has_leading_zero_in_raw(raw_value):
    """Check whether it has leading zeros."""
    if not raw_value:
        return False
    # Strip inline comment and whitespace
    raw_clean = raw_value.split("#", 1)[0].strip()
    # Only check if it's all digits and has leading zero (but not just '0')
    return raw_clean.isdigit() and raw_clean.startswith("0") and raw_clean != "0"


def main(argv):
    """Check all values."""
    success = True

    for file in argv:
        if not file.endswith("version.yaml"):
            continue

        try:
            with open(file, "r") as f:
                data = yaml.safe_load(f)

            if not isinstance(data, dict):
                print(f"{file}: not a valid YAML dictionary")
                success = False
                continue

            missing = REQUIRED_KEYS - data.keys()
            if missing:
                print(f"{file}: missing required keys: {', '.join(sorted(missing))}")
                success = False
                continue

            for key in VERSION_KEYS:
                value = data.get(key)

                if not isinstance(value, int):
                    print(f"{file}: '{key}' must be an integer, found: \"{value}\"")
                    success = False
                    continue

                if value < 0:
                    print(f"{file}: '{key}' must be non-negative, found: {value}")
                    success = False

                raw_value = get_original_line(file, key)
                if has_leading_zero_in_raw(raw_value):
                    print(f"{file}: '{key}' has a leading zero: {raw_value}")
                    success = False

            short_name = data.get("ShortName")
            if not isinstance(short_name, str) or not short_name.strip():
                print(f"{file}: 'ShortName' must be a non-empty string")
                success = False

        except Exception as e:
            print(f"{file}: error reading YAML: {e}")
            success = False

    return 0 if success else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
