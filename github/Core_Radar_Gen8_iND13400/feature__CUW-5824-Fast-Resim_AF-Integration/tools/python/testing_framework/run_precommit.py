"""Run pre-commit on files changed vs the remote tracking branch (or HEAD fallback)."""
import subprocess
import sys
from os import chdir, path

REPO_ROOT = path.abspath(path.join(path.dirname(path.abspath(__file__)), "..", "..", ".."))
chdir(REPO_ROOT)

# Prefer upstream diff so already-committed-but-unpushed files are also checked.
changed = subprocess.run(
    ["git", "diff", "--name-only", "@{upstream}"], capture_output=True, text=True, check=False
)
if changed.returncode != 0:
    changed = subprocess.run(
        ["git", "diff", "--name-only", "HEAD"], capture_output=True, text=True, check=False
    )

changed_files = [f for f in changed.stdout.splitlines() if f]

if not changed_files:
    print("No changed files, skipping pre-commit.")
    sys.exit(0)

print(f"Running pre-commit on {len(changed_files)} changed file(s)...")
result = subprocess.run(
    [sys.executable, "-m", "pre_commit", "run", "--files"] + changed_files, check=False
)
sys.exit(result.returncode)
