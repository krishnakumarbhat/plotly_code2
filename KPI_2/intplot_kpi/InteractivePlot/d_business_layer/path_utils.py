import os
import re

_WIN_DRIVE_PATH = re.compile(r"^([a-zA-Z]):[\\/](.*)$")


def set_default_umask() -> None:
    """Use umask 022 so outputs are readable in shared Linux/HPCC contexts."""
    try:
        os.umask(0o022)
    except Exception:
        pass


def normalize_fs_path(path: str) -> str:
    """Normalize file paths for Windows, WSL, and Linux containers."""
    if not path:
        return path

    raw = os.path.expandvars(os.path.expanduser(str(path).strip()))
    normalized = raw.replace("\\", "/")

    if os.path.exists(normalized):
        return os.path.abspath(normalized)

    if os.name != "nt":
        match = _WIN_DRIVE_PATH.match(normalized)
        if match:
            drive = match.group(1).lower()
            remainder = match.group(2)
            return f"/mnt/{drive}/{remainder}"

    return normalized
