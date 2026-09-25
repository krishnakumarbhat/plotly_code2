#!/usr/bin/env python3

import json
import os
import subprocess
import sys
from pathlib import Path


def main():
    program_files_x86 = os.environ.get("ProgramFiles(x86)")
    if not program_files_x86:
        print("ProgramFiles(x86) is not set.", file=sys.stderr)
        return 1

    vswhere = Path(program_files_x86) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe"
    if not vswhere.is_file():
        print(f"vswhere.exe not found at {vswhere}", file=sys.stderr)
        return 1

    command = [
        str(vswhere),
        "-latest",
        "-version",
        "[16.0,17.0)",
        "-products",
        "*",
        "-requires",
        "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
        "-format",
        "json",
    ]
    result = subprocess.run(command, capture_output=True, text=True, check=False)
    if result.returncode != 0:
        print(result.stderr.strip() or "vswhere failed.", file=sys.stderr)
        return result.returncode

    try:
        installations = json.loads(result.stdout)
        installation_path = Path(installations[0]["installationPath"])
    except (json.JSONDecodeError, KeyError, IndexError, TypeError) as error:
        print(f"No matching VS2019 installation found: {error}", file=sys.stderr)
        return 1

    vcvarsall = installation_path / "VC" / "Auxiliary" / "Build" / "vcvarsall.bat"
    if not vcvarsall.is_file():
        print(f"vcvarsall.bat not found at {vcvarsall}", file=sys.stderr)
        return 1

    print(installation_path)
    return 0


if __name__ == "__main__":
    sys.exit(main())