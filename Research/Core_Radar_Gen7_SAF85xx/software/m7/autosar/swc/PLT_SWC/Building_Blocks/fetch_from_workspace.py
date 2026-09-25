"""Fetch and extract repositories from a workspace.

This script reads a bb.bzl file for dependencies and downloads
the specified repositories, extracting them into a designated output directory.
"""

import os
import re
import tarfile
import zipfile
import requests
import netrc
import argparse
import shutil
from urllib.parse import urlparse

# === CONFIGURATION ===

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Relative paths
BB_BZL_REL_PATH = "../../../../../../bb.MODULE.bazel"
REQ_FILE_REL_PATH = "../Building_Blocks/requested_repos.txt"
OUTPUT_DIR_REL_PATH = "./submodules"

DEP_FILE = os.path.abspath(os.path.join(SCRIPT_DIR, BB_BZL_REL_PATH))
REPO_LIST_FILE = os.path.abspath(os.path.join(SCRIPT_DIR, REQ_FILE_REL_PATH))
OUT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, OUTPUT_DIR_REL_PATH))

os.makedirs(OUT_DIR, exist_ok=True)

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
    """Parse bb.bzl file and return a dict of repo name to URL."""
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


def read_requested_repos(file_path):
    """Read requested_repos.txt and return a list of (name, url) tuples."""
    repos = []
    with open(file_path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if "=" in line:
                name, url = map(str.strip, line.split("=", 1))
                repos.append((name, url))
            else:
                repos.append((line, None))
    return repos


# === DOWNLOAD & EXTRACT ===


def download_and_extract(name, url):
    """Download and extract the archive for the given repo name and URL."""
    extract_path = os.path.join(OUT_DIR, name)
    if os.path.exists(extract_path) and os.listdir(extract_path):
        print(f"⏩ Skipping: {name} (already extracted)")
        return

    print(f"🔽 Downloading: {name} from {url}")
    ext = "tar.gz" if url.endswith(".tar.gz") else "zip"
    archive_path = os.path.join(OUT_DIR, f"{name}.{ext}")

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

    os.makedirs(extract_path, exist_ok=True)
    print(f"📦 Extracting to: {extract_path}")
    try:
        if ext == "tar.gz":
            with tarfile.open(archive_path, "r:gz") as tar:
                tar.extractall(path=extract_path)
        elif ext == "zip":
            with zipfile.ZipFile(archive_path, "r") as zip_ref:
                zip_ref.extractall(extract_path)
        else:
            print(f"⚠️ Unknown archive type for {name}: {url}")
            return
    except Exception as e:
        print(f"❌ Failed to extract {name}: {e}")
        return

    # Cleanup archive
    try:
        os.remove(archive_path)
        print(f"🧹 Removed archive: {archive_path}")
    except Exception as e:
        print(f"⚠️ Could not remove archive {archive_path}: {e}")

    print(f"✅ Done: {name}\n")


# === CLEANUP ===


def cleanup_repos(repo_names):
    """Delete extracted repo directories listed in repo_names."""
    print("🧹 Cleanup mode: removing extracted repos")
    for name in repo_names:
        path = os.path.join(OUT_DIR, name)
        if os.path.exists(path):
            try:
                shutil.rmtree(path)
                print(f"🗑️ Deleted: {path}")
            except Exception as e:
                print(f"⚠️ Failed to delete {path}: {e}")
        else:
            print(f"ℹ️ Not found, skipping: {path}")

    # Delete the entire ext directory
    if os.path.exists(OUT_DIR):
        try:
            shutil.rmtree(OUT_DIR)
            print(f"🗑️ Deleted entire directory: {OUT_DIR}")
        except Exception as e:
            print(f"⚠️ Failed to delete directory {OUT_DIR}: {e}")


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
        cleanup_repos([name for name, _ in requested])
        return

    if not os.path.exists(DEP_FILE):
        print(f"❌ ERROR: bb.MODULE.bzl not found at {DEP_FILE}")
        return

    all_deps = parse_http_archives(DEP_FILE)

    for name, override_url in requested:
        if override_url:
            download_and_extract(name, override_url)
        elif name in all_deps:
            download_and_extract(name, all_deps[name])
        else:
            print(f"❌ Repo '{name}' not found in bb.MODULE.bzl and no URL provided")


if __name__ == "__main__":
    main()
