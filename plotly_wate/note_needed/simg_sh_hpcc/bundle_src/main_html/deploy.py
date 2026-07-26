#!/usr/bin/env python3
"""
Full deployment script for all_services:
1. Build .simg in WSL (if on Windows)
2. Sync project files to cluster
3. Optionally run the container on cluster

Usage:
  python deploy.py --build --sync --run
  python deploy.py --sync-only
  python deploy.py --run-only

Environment variables:
  CLUSTER_HOST: Remote host (default: 10.214.45.45)
  CLUSTER_USER: SSH username
  CLUSTER_DEST: Remote destination path
"""
import os
import sys
import argparse
import subprocess
import getpass
import platform
from pathlib import Path

KRAKOW_REMOTE_ROOT = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_service2"

# Cluster configurations
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
IMAGE_NAME = "all_services.simg"

# Get project root (parent of main_html)
PROJECT_ROOT = Path(__file__).parent.parent.resolve()
SIMG_PATH = PROJECT_ROOT / "simg"


def is_windows() -> bool:
    return platform.system().lower() == "windows"


def run_cmd(cmd: list, cwd: str = None, check: bool = True) -> subprocess.CompletedProcess:
    """Run a command and return the result."""
    print(f"\n$ {' '.join(cmd)}")
    result = subprocess.run(cmd, cwd=cwd, capture_output=False)
    if check and result.returncode != 0:
        print(f"Command failed with exit code {result.returncode}")
        sys.exit(result.returncode)
    return result


def build_simg() -> bool:
    """Build .simg using WSL on Windows or directly on Linux."""
    print("\n" + "=" * 60)
    print("Building Singularity/Apptainer image")
    print("=" * 60)
    
    # Ensure simg folder exists
    SIMG_PATH.mkdir(exist_ok=True)
    image_path = SIMG_PATH / IMAGE_NAME
    
    if is_windows():
        # Convert Windows path to WSL path
        wsl_project_path = str(PROJECT_ROOT).replace("C:", "/mnt/c").replace("D:", "/mnt/d").replace("\\", "/")
        wsl_image_path = f"{wsl_project_path}/simg/{IMAGE_NAME}"
        
        print(f"Building in WSL: {wsl_image_path}")
        result = subprocess.run(
            ["wsl", "bash", "-c", f"cd '{wsl_project_path}' && sudo apptainer build --force '{wsl_image_path}' Singularity.def"],
            cwd=str(PROJECT_ROOT)
        )
        return result.returncode == 0
    else:
        # On Linux, run directly
        for tool in ["apptainer", "singularity"]:
            if subprocess.run(["which", tool], capture_output=True).returncode == 0:
                result = run_cmd(
                    ["sudo", tool, "build", "--force", str(image_path), "Singularity.def"],
                    cwd=str(PROJECT_ROOT),
                    check=False
                )
                return result.returncode == 0
        print("ERROR: Neither apptainer nor singularity found.")
        return False


def sync_to_cluster(cluster: str, user: str, password: str, dest: str = None) -> bool:
    """Sync project files to cluster using sync_to_cluster.py."""
    print("\n" + "=" * 60)
    print(f"Syncing files to {cluster.upper()}")
    print("=" * 60)
    
    sync_script = Path(__file__).parent / "sync_to_cluster.py"
    
    cmd = [
        sys.executable, str(sync_script),
        "--cluster", cluster,
        "--user", user,
        "--password", password
    ]
    
    if dest:
        cmd.extend(["--dest", dest])
    
    result = subprocess.run(cmd, cwd=str(PROJECT_ROOT))
    return result.returncode == 0


def copy_simg_to_cluster(cluster: str, user: str, password: str, dest: str) -> bool:
    """Copy the built .simg file to cluster via scp."""
    print("\n" + "=" * 60)
    print(f"Copying container image to {cluster.upper()}")
    print("=" * 60)
    
    local_image = SIMG_PATH / IMAGE_NAME
    if not local_image.exists():
        print(f"ERROR: Image not found: {local_image}")
        return False
    
    cluster_info = CLUSTERS[cluster]
    host = cluster_info["host"]
    
    # Determine remote path
    if dest:
        remote_dest = f"{dest}/simg"
    elif cluster == "krakow":
        remote_dest = f"{cluster_info['remote_root']}/simg"
    else:
        remote_dest = f"{cluster_info['base_path']}/simg"
    
    remote_path = f"{user}@{host}:{remote_dest}/{IMAGE_NAME}"
    print(f"Copying {local_image} -> {remote_path}")
    
    # Try with sshpass if available
    try:
        result = subprocess.run(
            ["sshpass", "-p", password, "scp", str(local_image), remote_path],
            capture_output=True
        )
        if result.returncode == 0:
            print("Image copied successfully!")
            return True
    except FileNotFoundError:
        pass
    
    # Fall back to plain scp
    print("Using scp (you may be prompted for password)...")
    result = subprocess.run(["scp", str(local_image), remote_path])
    return result.returncode == 0


def run_on_cluster(cluster: str, user: str, password: str, dest: str, port: int = 5002) -> bool:
    """SSH to cluster and run the container."""
    print("\n" + "=" * 60)
    print(f"Starting container on {cluster.upper()}")
    print("=" * 60)
    
    cluster_info = CLUSTERS[cluster]
    host = cluster_info["host"]
    
    # Determine remote path
    if dest:
        simg_dir = f"{dest}/simg"
    elif cluster == "krakow":
        simg_dir = f"{cluster_info['remote_root']}/simg"
    else:
        simg_dir = f"{cluster_info['base_path']}/simg"
    
    run_cmd_remote = f"""
cd {simg_dir}
chmod +x run_all_services.sh 2>/dev/null || true
export DATABASE_URL="sqlite:///{simg_dir}/../.cache_html/hpc_tools.db"
export PORT={port}
nohup ./run_all_services.sh {port} > server.log 2>&1 &
echo "Server starting on port {port}..."
sleep 2
ps aux | grep -E 'gunicorn|flask' | grep -v grep | head -1
echo ""
echo "Access the app at: http://{host}:{port}/html"
"""
    
    print(f"Executing on {user}@{host}...")
    print(f"SIMG path: {simg_dir}")
    
    try:
        result = subprocess.run(
            ["sshpass", "-p", password, "ssh", f"{user}@{host}", run_cmd_remote],
            capture_output=False
        )
        return result.returncode == 0
    except FileNotFoundError:
        print("sshpass not found. Please run manually:")
        print(f"  ssh {user}@{host}")
        print(f"  cd {simg_dir}")
        print(f"  ./run_all_services.sh {port}")
        return False


def main():
    parser = argparse.ArgumentParser(description="Deploy all_services to cluster")
    parser.add_argument("--build", action="store_true", help="Build .simg container")
    parser.add_argument("--sync", action="store_true", help="Sync files to cluster")
    parser.add_argument("--copy-image", action="store_true", help="Copy .simg to cluster")
    parser.add_argument("--run", action="store_true", help="Run container on cluster")
    parser.add_argument("--all", action="store_true", help="Do everything: build, sync, copy-image, run")
    
    parser.add_argument("--cluster", choices=list(CLUSTERS.keys()), default=DEFAULT_CLUSTER,
                        help=f"Target cluster (default: {DEFAULT_CLUSTER})")
    parser.add_argument("--user", default=os.environ.get("CLUSTER_USER", os.environ.get("USERNAME", getpass.getuser())))
    parser.add_argument("--dest", default=None)
    parser.add_argument("--port", type=int, default=5002)
    parser.add_argument("--password", default=os.environ.get("CLUSTER_PASSWORD"))
    
    args = parser.parse_args()
    
    # If --all, enable everything
    if args.all:
        args.build = args.sync = args.copy_image = args.run = True
    
    # If nothing specified, show help
    if not any([args.build, args.sync, args.copy_image, args.run]):
        parser.print_help()
        return 1
    
    cluster_info = CLUSTERS[args.cluster]
    
    print("=" * 60)
    print("all_services Deployment")
    print("=" * 60)
    print(f"Cluster: {args.cluster.upper()}")
    print(f"Host:    {cluster_info['host']}")
    print(f"User:    {args.user}")
    print(f"Port:    {args.port}")
    print("=" * 60)
    
    # Get password if needed for remote operations
    password = args.password
    if not password and (args.sync or args.copy_image or args.run):
        password = getpass.getpass(f"Password for {args.user}@{cluster_info['host']}: ")
    
    success = True
    
    if args.build:
        if not build_simg():
            print("\nBuild failed!")
            success = False
    
    if args.sync and success:
        if not sync_to_cluster(args.cluster, args.user, password, args.dest):
            print("\nSync failed!")
            success = False
    
    if args.copy_image and success:
        if not copy_simg_to_cluster(args.cluster, args.user, password, args.dest):
            print("\nImage copy failed!")
            success = False
    
    if args.run and success:
        if not run_on_cluster(args.cluster, args.user, password, args.dest, args.port):
            print("\nRun failed!")
            success = False
    
    print("\n" + "=" * 60)
    if success:
        print("Deployment completed successfully!")
        if args.run:
            print(f"\nAccess the app at: http://{cluster_info['host']}:{args.port}/html")
    else:
        print("Deployment completed with errors.")
    print("=" * 60)
    
    return 0 if success else 1


if __name__ == "__main__":
    sys.exit(main())
