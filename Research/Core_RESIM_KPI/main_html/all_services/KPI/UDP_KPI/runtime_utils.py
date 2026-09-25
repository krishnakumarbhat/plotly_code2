import os
import re
from pathlib import Path

_WIN_DRIVE_PATH = re.compile(r"^([a-zA-Z]):[\\/](.*)$")


def set_default_umask() -> None:
    """Ensure created files/dirs are group/world-readable (umask 022)."""
    try:
        os.umask(0o022)
    except Exception:
        # Non-fatal in restricted runtimes.
        pass


def normalize_fs_path(path: str) -> str:
    """Normalize paths across Windows, WSL, and Linux container environments."""
    if not path:
        return path

    raw = os.path.expandvars(os.path.expanduser(str(path).strip()))
    normalized = raw.replace("\\", "/")

    # If already available as-is, keep it unchanged.
    if os.path.exists(normalized):
        return os.path.abspath(normalized)

    # Convert Windows drive path to WSL mount form when running on POSIX.
    if os.name != "nt":
        match = _WIN_DRIVE_PATH.match(normalized)
        if match:
            drive = match.group(1).lower()
            remainder = match.group(2)
            return f"/mnt/{drive}/{remainder}"

    return normalized


def ensure_dir(path: str) -> str:
    """Normalize, create, and return a directory path."""
    set_default_umask()
    norm = normalize_fs_path(path) or "."
    Path(norm).mkdir(parents=True, exist_ok=True)
    return norm
