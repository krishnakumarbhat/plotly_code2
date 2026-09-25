"""
Post-generation script: Discard date-only changes in DaVinci GenData files.

For each modified file in the GenData folder, checks if the only changes are
to date/timestamp lines. If so, reverts the file to its committed state.
If there are any other changes, ALL changes for that file are kept as-is.

Date/timestamp line patterns detected:
  - Lines containing '@date' or '@Date' (Doxygen date tag)
  - Lines containing 'Generation Time:' (DaVinci generation timestamp)
  - Lines containing 'Date created:' (indie Semiconductor MCAL date tag)
"""

import os
import re
import subprocess
import sys


# Matches any line that contains a date/timestamp marker used by DaVinci or MCAL generators
DATE_LINE_PATTERN = re.compile(
    r"@[Dd]ate\b"  # Doxygen @date / @Date tag
    r"|Generation\s+Time\s*:"  # DaVinci generation timestamp
    r"|Date\s+created\s*:"  # indie Semiconductor MCAL date tag
)


def get_repo_root():
    """Return the absolute path to the root of the git repository."""
    result = subprocess.run(
        ["git", "rev-parse", "--show-toplevel"], capture_output=True, text=True, check=True
    )
    return result.stdout.strip()


def get_changed_files_in_gendata(repo_root, gendata_rel_prefix):
    """
    Return repo-relative paths of changed files inside the GenData folder.

    Queries git diff HEAD and filters results to files that reside under
    gendata_rel_prefix (inclusive of subdirectories).
    """
    result = subprocess.run(
        ["git", "diff", "HEAD", "--name-only"],
        capture_output=True,
        text=True,
        check=True,
        cwd=repo_root,
    )
    prefix = gendata_rel_prefix.replace("\\", "/").rstrip("/")
    changed = []
    for line in result.stdout.splitlines():
        normalized = line.strip().replace("\\", "/")
        if normalized == prefix or normalized.startswith(prefix + "/"):
            changed.append(normalized)
    return changed


def get_diff_changed_lines(repo_root, rel_path):
    """
    Return the content of every added/removed line in the diff for rel_path.

    Compares rel_path against HEAD; the leading +/- diff prefix is stripped
    from each returned line. Diff header and hunk lines are excluded.
    """
    result = subprocess.run(
        ["git", "diff", "HEAD", "--", rel_path],
        capture_output=True,
        text=True,
        check=True,
        cwd=repo_root,
    )
    changed_lines = []
    for line in result.stdout.splitlines():
        # Skip diff header lines (+++/---) and hunk headers (@@)
        if line.startswith("+++") or line.startswith("---") or line.startswith("@@"):
            continue
        if line.startswith("+") or line.startswith("-"):
            changed_lines.append(line[1:])  # Strip the +/- prefix
    return changed_lines


def is_date_only_change(changed_lines):
    """
    Return True when every changed line matches a date/timestamp pattern.

    Returns False when changed_lines is empty or when at least one line does
    not match any of the known DaVinci/MCAL date patterns.
    """
    if not changed_lines:
        return False
    return all(DATE_LINE_PATTERN.search(line) for line in changed_lines)


def revert_file(repo_root, rel_path):
    """Restore a file to its HEAD state using git checkout."""
    subprocess.run(["git", "checkout", "HEAD", "--", rel_path], check=True, cwd=repo_root)


def main():
    """Entry point: revert date-only changes in GenData and keep all other changes."""
    script_dir = os.path.dirname(os.path.abspath(__file__))

    try:
        repo_root = get_repo_root()
    except subprocess.CalledProcessError:
        print("ERROR: This script must be run inside a git repository.", file=sys.stderr)
        sys.exit(1)

    gendata_abs = os.path.normpath(os.path.join(script_dir, "..", "Appl", "GenData"))
    gendata_rel = os.path.relpath(gendata_abs, repo_root).replace("\\", "/")

    print("=" * 70)
    print("GenData date-only change cleanup")
    print("=" * 70)
    print(f"Repository root : {repo_root}")
    print(f"GenData folder  : {gendata_abs}")
    print()

    try:
        changed_files = get_changed_files_in_gendata(repo_root, gendata_rel)
    except subprocess.CalledProcessError as exc:
        print(f"ERROR: Failed to query git status: {exc}", file=sys.stderr)
        sys.exit(1)

    if not changed_files:
        print("No changed files found in GenData folder. Nothing to do.")
        return

    print(f"Found {len(changed_files)} changed file(s) in GenData:\n")

    reverted = []
    kept = []
    errors = []

    for rel_path in sorted(changed_files):
        try:
            changed_lines = get_diff_changed_lines(repo_root, rel_path)
        except subprocess.CalledProcessError as exc:
            print(f"  [ERROR]    {rel_path}  (could not read diff: {exc})", file=sys.stderr)
            errors.append(rel_path)
            continue

        if is_date_only_change(changed_lines):
            try:
                revert_file(repo_root, rel_path)
                print(f"  [REVERTED] {rel_path}")
                reverted.append(rel_path)
            except subprocess.CalledProcessError as exc:
                print(f"  [ERROR]    {rel_path}  (revert failed: {exc})", file=sys.stderr)
                errors.append(rel_path)
        else:
            print(f"  [KEPT]     {rel_path}")
            kept.append(rel_path)

    print()
    print("=" * 70)
    print("Summary:")
    print(f"  Reverted (date-only) : {len(reverted)}")
    print(f"  Kept (other changes) : {len(kept)}")
    if errors:
        print(f"  Errors               : {len(errors)}")
    print("=" * 70)

    if errors:
        sys.exit(1)


if __name__ == "__main__":
    main()
