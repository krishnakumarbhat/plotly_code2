"""Fetch and extract repositories from a workspace.

This script reads a bb.MODULE.bazel file for dependencies and downloads
the specified repositories, extracting them into a designated output directory.

Supported requested_repos.txt formats (one per line):
1) <repo_name>
2) <repo_name>=<override_url>
3) <repo_name>|<destination_path>
4) <repo_name>|<destination_path>|<override_url>

Notes:
- destination_path must be a relative path under PLT_SWC.

Extraction behavior:
- Each repo is extracted under PLT_SWC/<destination_path>/bb
- The first-level extracted directory is normalized to "bb" by default
- No BUILD file modifications are performed
"""

import os
import re
import tarfile
import zipfile
import requests
import netrc
import argparse
import shutil
import tempfile
from urllib.parse import urlparse

# === CONFIGURATION ===

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Relative paths from software/r52/autosar/swc/PLT_SWC/
BB_BZL_REL_PATH = "../../../../../bb.MODULE.bazel"
REQ_FILE_REL_PATH = "requested_repos.txt"
SWC_ROOT_REL_PATH = "."
LEGACY_SWC_ROOT_REL_PATH = "SWC"

DEP_FILE = os.path.abspath(os.path.join(SCRIPT_DIR, BB_BZL_REL_PATH))
REPO_LIST_FILE = os.path.abspath(os.path.join(SCRIPT_DIR, REQ_FILE_REL_PATH))
SWC_ROOT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, SWC_ROOT_REL_PATH))
LEGACY_SWC_ROOT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, LEGACY_SWC_ROOT_REL_PATH))

# === AUTH ===


def get_auth_from_netrc(gerrit_url):
    """Retrieve username and password for the given URL from .netrc file."""
    try:
        hostname = urlparse(gerrit_url).hostname
        authenticator = netrc.netrc()
        username, _, password = authenticator.authenticators(hostname)
        return username, password
    except (netrc.NetrcParseError, TypeError):
        print(f"❌ Error: No credentials for {gerrit_url} in .netrc")
        return None, None


# === PARSING ===


def parse_http_archives(file_path):
    """Parse bb.MODULE.bazel and return a dict of repo name to URL."""
    with open(file_path, "r") as f:
        content = f.read()

    pattern = re.compile(
        r"http_archive\(\s*name\s*=\s*\"(?P<name>[^\"]+)\".*?urls\s*=\s*\[(?P<urls>.*?)\]",
        re.DOTALL,
    )

    deps = {}
    for match in pattern.finditer(content):
        name = match.group("name")
        url_block = match.group("urls")
        urls = re.findall(r"\"(http.*?)\"", url_block)
        if urls:
            deps[name] = urls[0]
    return deps


def _default_destination_for_repo(name):
    """Return default destination folder for a repo name."""
    destination_aliases = {
        "SWC_PLT_Appl_TimeSync": "SWC_PLT_CDD_TimeSync",
    }
    return destination_aliases.get(name, name)


def _normalize_relative_destination(destination):
    """Normalize destination and ensure it stays a relative path."""
    normalized = os.path.normpath(destination.strip())
    if os.path.isabs(normalized):
        raise ValueError(f"Destination must be a relative path, got: {destination}")
    return normalized


def read_requested_repos(file_path):
    """Read requested_repos.txt and return repo metadata entries.

    Each entry is a dict with keys:
    - name: repo name from bb.MODULE.bazel
    - destination: relative destination path under SWC root
    - url: optional override URL
    """
    repos = []
    with open(file_path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            if "|" in line:
                parts = [p.strip() for p in line.split("|")]
                if len(parts) == 2:
                    name = parts[0]
                    if parts[1].startswith("http"):
                        repos.append(
                            {
                                "name": name,
                                "destination": _normalize_relative_destination(
                                    _default_destination_for_repo(name)
                                ),
                                "url": parts[1],
                            }
                        )
                    else:
                        repos.append(
                            {
                                "name": name,
                                "destination": _normalize_relative_destination(parts[1]),
                                "url": None,
                            }
                        )
                elif len(parts) >= 3:
                    name = parts[0]
                    destination = (
                        _normalize_relative_destination(parts[1])
                        if parts[1]
                        else _normalize_relative_destination(_default_destination_for_repo(name))
                    )
                    url = parts[2] if parts[2] else None
                    repos.append({"name": name, "destination": destination, "url": url})
                else:
                    repos.append(
                        {
                            "name": line,
                            "destination": _normalize_relative_destination(
                                _default_destination_for_repo(line)
                            ),
                            "url": None,
                        }
                    )
            elif "=" in line:
                name, url = map(str.strip, line.split("=", 1))
                repos.append(
                    {
                        "name": name,
                        "destination": _normalize_relative_destination(
                            _default_destination_for_repo(name)
                        ),
                        "url": url,
                    }
                )
            else:
                repos.append(
                    {
                        "name": line,
                        "destination": _normalize_relative_destination(
                            _default_destination_for_repo(line)
                        ),
                        "url": None,
                    }
                )
    return repos


# === DOWNLOAD & EXTRACT ===


def _normalize_root_dir(extract_tmp_path, final_repo_path, root_dir_name):
    """Normalize extracted content so that first-level folder name is root_dir_name."""
    os.makedirs(final_repo_path, exist_ok=True)
    normalized_root = os.path.join(final_repo_path, root_dir_name)

    if os.path.exists(normalized_root):
        shutil.rmtree(normalized_root)

    entries = [item for item in os.listdir(extract_tmp_path) if item not in ("__MACOSX",)]

    if len(entries) == 1 and os.path.isdir(os.path.join(extract_tmp_path, entries[0])):
        shutil.move(os.path.join(extract_tmp_path, entries[0]), normalized_root)
    else:
        os.makedirs(normalized_root, exist_ok=True)
        for item in entries:
            shutil.move(os.path.join(extract_tmp_path, item), os.path.join(normalized_root, item))


def _resolve_repo_parent_path(destination):
    """Resolve destination path to an absolute path under PLT_SWC by default.

    Falls back to legacy PLT_SWC/SWC layout if the destination exists only there.
    """
    destination = _normalize_relative_destination(destination)

    new_path = os.path.abspath(os.path.join(SWC_ROOT_DIR, destination))
    legacy_path = os.path.abspath(os.path.join(LEGACY_SWC_ROOT_DIR, destination))

    if os.path.isdir(new_path):
        return new_path
    if os.path.isdir(legacy_path):
        return legacy_path

    return new_path


def download_and_extract(name, url, destination, root_dir_name="bb"):
    """Download and extract the archive for the given repo name and URL."""
    repo_path = _resolve_repo_parent_path(destination)
    normalized_root = os.path.join(repo_path, root_dir_name)
    if os.path.exists(normalized_root) and os.listdir(normalized_root):
        print(f"⏩ Skipping: {name} (already extracted at {normalized_root})")
        return

    print(f"🔽 Downloading: {name} from {url}")
    ext = "tar.gz" if url.endswith(".tar.gz") else "zip"
    tmp_work_dir = tempfile.mkdtemp(prefix=f"{name}_")
    archive_path = os.path.join(tmp_work_dir, f"{name}.{ext}")

    username, password = get_auth_from_netrc(url)
    auth = (username, password) if username and password else None

    try:
        response = requests.get(url, auth=auth)
        if response.status_code == 200:
            with open(archive_path, "wb") as f:
                f.write(response.content)
        else:
            print(f"❌ Failed to download {name} (status code {response.status_code})")
            return
    except Exception as e:
        print(f"❌ Exception during download of {name}: {e}")
        return

    tmp_extract_path = os.path.join(tmp_work_dir, "extract")
    os.makedirs(tmp_extract_path, exist_ok=True)
    print(f"📦 Extracting to temporary path: {tmp_extract_path}")
    try:
        if ext == "tar.gz":
            with tarfile.open(archive_path, "r:gz") as tar:
                tar.extractall(path=tmp_extract_path)
        elif ext == "zip":
            with zipfile.ZipFile(archive_path, "r") as zip_ref:
                zip_ref.extractall(tmp_extract_path)
        else:
            print(f"⚠️ Unknown archive type for {name}: {url}")
            return

        _normalize_root_dir(tmp_extract_path, repo_path, root_dir_name)
        print(f"📁 Normalized repo path: {os.path.join(repo_path, root_dir_name)}")
    except Exception as e:
        print(f"❌ Failed to extract {name}: {e}")
        return
    finally:
        try:
            shutil.rmtree(tmp_work_dir)
        except Exception:
            pass

    print(f"✅ Done: {name}\n")


# === CLEANUP ===


def cleanup_repos(repo_entries):
    """Delete extracted bb directories listed in requested_repos.txt."""
    print("🧹 Cleanup mode: removing extracted repos")
    for entry in repo_entries:
        repo_path = _resolve_repo_parent_path(entry["destination"])
        path = os.path.join(repo_path, "bb")
        if os.path.exists(path):
            try:
                shutil.rmtree(path)
                print(f"🗑️ Deleted: {path}")
            except Exception as e:
                print(f"⚠️ Failed to delete {path}: {e}")
        else:
            print(f"ℹ️ Not found, skipping: {path}")


# === MAIN ===


def main():
    """Main entry point for the script."""
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--cleanup",
        action="store_true",
        help="Delete extracted repos listed in requested_repos.txt",
    )
    args = parser.parse_args()

    if not os.path.exists(REPO_LIST_FILE):
        print(f"❌ ERROR: requested_repos.txt not found at {REPO_LIST_FILE}")
        return

    requested = read_requested_repos(REPO_LIST_FILE)

    if args.cleanup:
        cleanup_repos(requested)
        return

    if not os.path.exists(DEP_FILE):
        print(f"❌ ERROR: bb.MODULE.bazel not found at {DEP_FILE}")
        return

    all_deps = parse_http_archives(DEP_FILE)

    for entry in requested:
        name = entry["name"]
        destination = entry["destination"]
        override_url = entry["url"]

        if override_url:
            download_and_extract(name, override_url, destination)
        elif name in all_deps:
            download_and_extract(name, all_deps[name], destination)
        else:
            print(f"❌ Repo '{name}' not found in bb.MODULE.bazel and no URL provided")


if __name__ == "__main__":
    main()
