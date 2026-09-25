"""Generate a CSV inventory of Bazel dependency declarations.

This script parses Bazel module files and extracts dependency entries from
`bazel_dep`, `http_archive`, and `git_repository` declarations.

Output:
    Writes a CSV file (for example `dependency_inventory.csv`) via `--out`
    with columns:
    - include_type
    - building_block
    - repo_name
    - version
    - source_file
"""

import argparse
import csv
import os
import re
from urllib.parse import unquote, urlparse


CALL_RE = re.compile(r"(?s)(bazel_dep|http_archive|git_repository)\((.*?)\)")


def extract_arg(body, key):
    """Extract a quoted argument value from a declaration body.

    Args:
        body: Text inside a Bazel declaration call.
        key: Argument name to extract.

    Returns:
        The argument value if found, otherwise an empty string.
    """
    match = re.search(rf"{re.escape(key)}\s*=\s*\"([^\"]+)\"", body)
    return match.group(1) if match else ""


def extract_first_url(body):
    """Return the first URL value from `urls` or `url` arguments.

    Args:
        body: Text inside a Bazel declaration call.

    Returns:
        The first URL string if found, otherwise an empty string.
    """
    urls_match = re.search(r"urls\s*=\s*\[\s*\"([^\"]+)\"", body)
    if urls_match:
        return urls_match.group(1)
    url_match = re.search(r"url\s*=\s*\"([^\"]+)\"", body)
    if url_match:
        return url_match.group(1)
    return ""


def infer_version_from_url(url):
    """Infer a dependency version from an archive URL.

    The function attempts explicit version patterns in the filename first,
    then falls back to `/vN/` path segments, then finally to the archive stem.

    Args:
        url: Archive URL from Bazel declaration.

    Returns:
        Best-effort version string, or empty string when unavailable.
    """
    if not url:
        return ""

    parsed = urlparse(url)
    decoded_path = unquote(parsed.path)
    filename = os.path.basename(decoded_path)

    patterns = [
        r"_v([0-9]+(?:\.[0-9]+){1,4})\.(?:zip|tar|gz|xz|zst)$",
        r"(?:^|[^A-Za-z0-9])v([0-9]+(?:\.[0-9]+){1,4})(?:[^A-Za-z0-9]|$)",
        r"_([0-9]+(?:\.[0-9]+){1,4})\.(?:zip|tar|gz|xz|zst)$",
    ]
    for pattern in patterns:
        match = re.search(pattern, filename)
        if match:
            return match.group(1)

    path_v = re.search(r"/v([0-9]+(?:\.[0-9]+){0,4})/", decoded_path)
    if path_v:
        return f"v{path_v.group(1)}"

    stem = re.sub(r"\.(zip|tar\.gz|tar\.xz|tar\.zst|tar|gz|xz|zst)$", "", filename)
    return stem


def infer_repo_name_from_url(url):
    """Infer a repo/package name from an Artifactory-style URL path.

    Args:
        url: Archive URL from Bazel declaration.

    Returns:
        Inferred package/repo name, or host name as a fallback.
    """
    if not url:
        return ""
    parsed = urlparse(url)
    parts = [p for p in unquote(parsed.path).split("/") if p]
    if "artifactory" in parts:
        idx = parts.index("artifactory")
        # URL layout is typically:
        # /artifactory/<jfrog_repo>/<package_or_block>/<version>/artifact.zip
        # We want <package_or_block>, not the JFrog repo bucket.
        if idx + 2 < len(parts):
            candidate = parts[idx + 2]
            # Skip generic bucket segments when present.
            if candidate.lower() in {"indie", "builds", "windows", "linux"} and idx + 3 < len(
                parts
            ):
                candidate = parts[idx + 3]
            return candidate
    return parsed.netloc


def infer_repo_name_from_remote(remote):
    """Infer repository name from a git remote URL.

    Args:
        remote: Git remote URL.

    Returns:
        Final path segment of the remote, without `.git` suffix.
    """
    if not remote:
        return ""
    cleaned = remote.rstrip("/")
    basename = cleaned.split("/")[-1]
    if basename.endswith(".git"):
        basename = basename[:-4]
    return basename


def parse_module_file(path):
    """Parse one Bazel module file and extract dependency inventory rows.

    Args:
        path: Absolute or relative path to a module file.

    Returns:
        List of row dictionaries for CSV output.
    """
    with open(path, "r", encoding="utf-8") as f:
        content = f.read()

    rows = []
    for match in CALL_RE.finditer(content):
        include_type = match.group(1)
        body = match.group(2)

        name = extract_arg(body, "name")
        if not name:
            continue

        building_block = name
        repo_name = ""
        version = ""

        if include_type == "bazel_dep":
            # In bazel_dep, repo_name is the workspace repo alias most users think of
            # as the building block name, while name is the module identifier.
            building_block = extract_arg(body, "repo_name") or name
            repo_name = name
            version = extract_arg(body, "version")
        elif include_type == "http_archive":
            url = extract_first_url(body)
            repo_name = infer_repo_name_from_url(url)
            version = infer_version_from_url(url)
        elif include_type == "git_repository":
            remote = extract_arg(body, "remote")
            repo_name = infer_repo_name_from_remote(remote)
            version = (
                extract_arg(body, "tag")
                or extract_arg(body, "commit")
                or extract_arg(body, "branch")
            )

        rows.append(
            {
                "include_type": include_type,
                "building_block": building_block,
                "repo_name": repo_name,
                "version": version,
                "source_file": os.path.basename(path),
            }
        )

    return rows


def write_csv(rows, output_path):
    """Write unique, sorted dependency rows to a CSV file.

    Args:
        rows: Row dictionaries to serialize.
        output_path: Destination path for generated CSV.
    """
    fieldnames = ["include_type", "building_block", "repo_name", "version", "source_file"]
    unique = {
        (r["include_type"], r["building_block"], r["repo_name"], r["version"], r["source_file"]): r
        for r in rows
    }
    sorted_rows = sorted(
        unique.values(),
        key=lambda r: (
            r["include_type"],
            r["building_block"],
            r["source_file"],
            r["version"],
            r["repo_name"],
        ),
    )

    with open(output_path, "w", encoding="utf-8", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(sorted_rows)


def main():
    """CLI entry point for dependency inventory generation."""
    parser = argparse.ArgumentParser(
        description="Generate dependency inventory CSV from Bazel module files."
    )
    parser.add_argument(
        "--input",
        action="append",
        required=True,
        help="Input .bazel module file path. Can be repeated.",
    )
    parser.add_argument("--out", required=True, help="Output CSV path.")
    args = parser.parse_args()

    rows = []
    for input_path in args.input:
        rows.extend(parse_module_file(input_path))

    write_csv(rows, args.out)


if __name__ == "__main__":
    main()
