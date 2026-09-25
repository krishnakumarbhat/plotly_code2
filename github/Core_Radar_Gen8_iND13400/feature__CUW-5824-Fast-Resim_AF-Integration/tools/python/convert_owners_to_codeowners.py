#!/usr/bin/env python3
"""Convert Gerrit OWNERS files to GitHub CODEOWNERS format.

Walks the repository, reads all OWNERS files, and produces a single
.github/CODEOWNERS file. Requires a mapping of Gerrit owner references
to GitHub teams/usernames.

Usage:
    python convert_owners_to_codeowners.py --repo-root . --output .github/CODEOWNERS --mapping owners_mapping.json
"""

import argparse
import json
import re
from pathlib import Path


def load_mapping(mapping_file: str) -> dict:
    """Load Gerrit-to-GitHub owner mapping.

    Expected JSON format:
    {
        "file:Core_Radar_Permissions:dev:/CI_OWNERS": "@AptivRadar/ci-owners",
        "file:Core_Radar_Permissions:dev:/AFBB_OWNERS": "@AptivRadar/afbb-owners",
        "user@example.com": "@github-username"
    }
    """
    with open(mapping_file, "r") as f:
        return json.load(f)


def parse_owners_file(filepath: Path) -> list:
    """Parse a Gerrit OWNERS file.

    Returns list of dicts:
        {"type": "directory", "owners": ["ref1", ...]}
        {"type": "per-file", "pattern": "glob", "owners": ["ref1", ...]}
        {"type": "include", "ref": "..."}
    """
    rules = []
    with open(filepath, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            # per-file pattern=owner_ref
            m = re.match(r"per-file\s+(.+?)=(.+)", line)
            if m:
                patterns_str = m.group(1).strip()
                owner_ref = m.group(2).strip()
                patterns = [p.strip() for p in patterns_str.split(",")]
                for pattern in patterns:
                    rules.append(
                        {
                            "type": "per-file",
                            "pattern": pattern,
                            "owners": [owner_ref],
                        }
                    )
                continue

            # include directive
            m = re.match(r"include\s+(.+)", line)
            if m:
                rules.append({"type": "include", "ref": m.group(1).strip()})
                continue

            # Plain owner reference (directory-level)
            # Could be: file:Repo:branch:/path, email, or username
            rules.append({"type": "directory", "owners": [line]})

    return rules


def resolve_owner(ref: str, mapping: dict, fallback: str) -> str:
    """Resolve a Gerrit owner reference to GitHub team/username."""
    if ref in mapping:
        return mapping[ref]
    # Try without leading/trailing whitespace variations
    for key, value in mapping.items():
        if ref.strip() == key.strip():
            return value
    return fallback


def owners_to_codeowners_lines(repo_root: Path, mapping: dict, fallback_owner: str) -> list:
    """Walk repo and convert all OWNERS files to CODEOWNERS lines."""
    lines = []
    lines.append("# Auto-generated from Gerrit OWNERS files")
    lines.append("# Do not edit manually - regenerate with convert_owners_to_codeowners.py")
    lines.append("")
    lines.append("# Fallback owner for unmatched files")
    lines.append(f"*    {fallback_owner}")
    lines.append("")

    owners_files = sorted(repo_root.rglob("OWNERS"))

    for owners_file in owners_files:
        rel_dir = owners_file.parent.relative_to(repo_root)
        dir_prefix = "/" + str(rel_dir).replace("\\", "/")
        if dir_prefix == "/.":
            dir_prefix = "/"

        rules = parse_owners_file(owners_file)
        if not rules:
            continue

        lines.append(f"# Source: {rel_dir}/OWNERS")

        for rule in rules:
            if rule["type"] == "directory":
                for owner_ref in rule["owners"]:
                    gh_owner = resolve_owner(owner_ref, mapping, fallback_owner)
                    pattern = dir_prefix if dir_prefix == "/" else dir_prefix + "/"
                    lines.append(f"{pattern}    {gh_owner}")

            elif rule["type"] == "per-file":
                for owner_ref in rule["owners"]:
                    gh_owner = resolve_owner(owner_ref, mapping, fallback_owner)
                    glob = rule["pattern"]
                    if dir_prefix == "/":
                        pattern = f"/{glob}"
                    else:
                        pattern = f"{dir_prefix}/{glob}"
                    lines.append(f"{pattern}    {gh_owner}")

            elif rule["type"] == "include":
                gh_owner = resolve_owner(rule["ref"], mapping, fallback_owner)
                pattern = dir_prefix if dir_prefix == "/" else dir_prefix + "/"
                lines.append(f"{pattern}    {gh_owner}")

        lines.append("")

    return lines


def main():
    """Entry point for OWNERS to CODEOWNERS conversion."""
    parser = argparse.ArgumentParser(description="Convert Gerrit OWNERS to GitHub CODEOWNERS")
    parser.add_argument("--repo-root", default=".", help="Repository root path")
    parser.add_argument("--output", default=".github/CODEOWNERS", help="Output CODEOWNERS path")
    parser.add_argument(
        "--mapping", required=True, help="JSON mapping file (Gerrit ref → GitHub owner)"
    )
    parser.add_argument(
        "--fallback",
        default="@AptivRadar/core-radar-maintainers",
        help="Fallback owner for unmapped references",
    )
    args = parser.parse_args()

    repo_root = Path(args.repo_root).resolve()
    mapping = load_mapping(args.mapping)
    lines = owners_to_codeowners_lines(repo_root, mapping, args.fallback)

    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with open(output_path, "w", newline="\n") as f:
        f.write("\n".join(lines) + "\n")

    print(f"Generated {output_path} with {len(lines)} lines")


if __name__ == "__main__":
    main()
