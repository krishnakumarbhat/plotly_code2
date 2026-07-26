#!/usr/bin/env python3
"""
Sync all_services project to remote cluster via SSH/SFTP.

Usage:
  python sync_to_cluster.py [--host HOST] [--user USER] [--dest DEST] [--dry-run]

Defaults:
  host: 10.214.45.45 (Krakow)
  user: from USERNAME env or current user
    dest: /net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2
"""
import os
import sys
import argparse
import getpass
import stat
from pathlib import Path
from typing import List, Tuple, Optional

try:
    import paramiko
except ImportError:
    print("ERROR: paramiko not installed. Run: pip install paramiko")
    sys.exit(1)

KRAKOW_REMOTE_ROOT = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2"

# Cluster settings
CLUSTERS = {
    "krakow": {
        "host": "10.214.45.45",
        "remote_root": KRAKOW_REMOTE_ROOT,
    },
    "southfield": {
        "host": "10.192.224.131",
        "base_path": "/mnt/usmidet/projects/RADARCORE/2-Sim/USER_DATA/ouymc/all_services",
    },
}

DEFAULT_CLUSTER = "krakow"

# Files/folders to exclude from sync
EXCLUDE_PATTERNS = {
    "__pycache__",
    ".git",
    ".venv",
    "venv",
    "node_modules",
    "*.pyc",
    "*.pyo",
    ".env",
    "*.sif",
    "*.simg",
    "hpc_tools_dev.db",
    ".cache_html",
    "chromadb_data",
    "*.egg-info",
    ".pytest_cache",
    ".mypy_cache",
    "out",
    "llm_model",  # Large model files - sync separately if needed
    "simg",  # Synced separately via build_and_sync.py
}


def should_exclude(name: str) -> bool:
    """Check if a file/folder should be excluded."""
    if name in EXCLUDE_PATTERNS:
        return True
    for pattern in EXCLUDE_PATTERNS:
        if pattern.startswith("*") and name.endswith(pattern[1:]):
            return True
    return False


def get_local_files(base_path: Path) -> List[Tuple[Path, str]]:
    """Get list of (local_path, relative_path) for all files to sync."""
    files = []
    for root, dirs, filenames in os.walk(base_path):
        # Filter out excluded directories
        dirs[:] = [d for d in dirs if not should_exclude(d)]
        
        rel_root = Path(root).relative_to(base_path)
        
        for fname in filenames:
            if should_exclude(fname):
                continue
            local_path = Path(root) / fname
            rel_path = str(rel_root / fname).replace("\\", "/")
            if rel_path.startswith("./"):
                rel_path = rel_path[2:]
            files.append((local_path, rel_path))
    
    return files


def ensure_remote_dir(sftp: paramiko.SFTPClient, remote_path: str) -> None:
    """Recursively create remote directory if it doesn't exist."""
    parts = remote_path.strip("/").split("/")
    current = ""
    for part in parts:
        current = f"{current}/{part}"
        try:
            sftp.stat(current)
        except FileNotFoundError:
            try:
                sftp.mkdir(current)
                print(f"  Created dir: {current}")
            except Exception as e:
                # Might already exist due to race condition
                pass


def sync_files(
    host: str,
    username: str,
    password: str,
    local_base: Path,
    remote_base: str,
    dry_run: bool = False
) -> Tuple[int, int, List[str]]:
    """
    Sync local files to remote cluster.
    Returns (synced_count, skipped_count, errors).
    """
    files = get_local_files(local_base)
    synced = 0
    skipped = 0
    errors = []
    
    print(f"\nConnecting to {host} as {username}...")
    
    if dry_run:
        print("\n[DRY RUN] Would sync the following files:")
        for local_path, rel_path in files:
            print(f"  {rel_path}")
        print(f"\nTotal: {len(files)} files")
        return len(files), 0, []
    
    try:
        transport = paramiko.Transport((host, 22))
        transport.connect(username=username, password=password)
        sftp = paramiko.SFTPClient.from_transport(transport)
        
        print(f"Connected. Syncing {len(files)} files to {remote_base}...\n")
        
        # Ensure base directory exists
        ensure_remote_dir(sftp, remote_base)
        
        for local_path, rel_path in files:
            remote_path = f"{remote_base}/{rel_path}"
            remote_dir = os.path.dirname(remote_path)
            
            try:
                # Ensure parent directory exists
                ensure_remote_dir(sftp, remote_dir)
                
                # Check if file needs update (compare size/mtime)
                need_upload = True
                try:
                    remote_stat = sftp.stat(remote_path)
                    local_stat = local_path.stat()
                    # Simple check: if sizes match and remote is newer, skip
                    if remote_stat.st_size == local_stat.st_size:
                        if remote_stat.st_mtime >= local_stat.st_mtime:
                            skipped += 1
                            need_upload = False
                except FileNotFoundError:
                    pass  # File doesn't exist remotely
                
                if need_upload:
                    print(f"  Uploading: {rel_path}")
                    sftp.put(str(local_path), remote_path)
                    synced += 1
                    
            except Exception as e:
                error_msg = f"{rel_path}: {e}"
                errors.append(error_msg)
                print(f"  ERROR: {error_msg}")
        
        sftp.close()
        transport.close()
        
    except Exception as e:
        errors.append(f"Connection failed: {e}")
        print(f"\nERROR: {e}")
    
    return synced, skipped, errors


def main():
    parser = argparse.ArgumentParser(description="Sync all_services to remote cluster")
    parser.add_argument("--cluster", choices=list(CLUSTERS.keys()), default=DEFAULT_CLUSTER,
                        help=f"Target cluster (default: {DEFAULT_CLUSTER})")
    parser.add_argument("--host", default=None, help="Remote host (overrides cluster default)")
    parser.add_argument("--user", default=os.environ.get("USERNAME", getpass.getuser()), help="SSH username")
    parser.add_argument("--dest", default=None, help="Remote destination path (default: auto from cluster)")
    parser.add_argument("--password", default=None, help="SSH password (will prompt if not given)")
    parser.add_argument("--dry-run", action="store_true", help="Show what would be synced without uploading")
    args = parser.parse_args()
    
    # Determine local base (this script's parent's parent directory - project root)
    local_base = Path(__file__).parent.parent.resolve()
    
    # Determine cluster settings
    cluster = CLUSTERS[args.cluster]
    host = args.host or cluster["host"]
    
    # Determine remote destination
    if args.dest:
        remote_base = args.dest
    else:
        if args.cluster == "krakow":
            remote_base = cluster["remote_root"]
        else:
            remote_base = cluster["base_path"]  # Southfield already includes full path
    
    print("=" * 60)
    print("all_services Cluster Sync")
    print("=" * 60)
    print(f"Cluster: {args.cluster.upper()}")
    print(f"Local:   {local_base}")
    print(f"Remote:  {args.user}@{host}:{remote_base}")
    print("=" * 60)
    
    # Get password
    if args.dry_run:
        password = ""
    elif args.password:
        password = args.password
    else:
        password = getpass.getpass(f"Password for {args.user}@{host}: ")
    
    # Sync
    synced, skipped, errors = sync_files(
        host=host,
        username=args.user,
        password=password,
        local_base=local_base,
        remote_base=remote_base,
        dry_run=args.dry_run
    )
    
    print("\n" + "=" * 60)
    print(f"Synced: {synced} | Skipped (up-to-date): {skipped} | Errors: {len(errors)}")
    if errors:
        print("\nErrors:")
        for e in errors[:10]:
            print(f"  - {e}")
        if len(errors) > 10:
            print(f"  ... and {len(errors) - 10} more")
    print("=" * 60)
    
    return 0 if not errors else 1


if __name__ == "__main__":
    sys.exit(main())
