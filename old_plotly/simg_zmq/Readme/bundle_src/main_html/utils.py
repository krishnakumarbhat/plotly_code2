"""
Utility functions for HPC Flask Application
Includes Slurm job submission, file browser, and helper functions
"""
import os
import subprocess
import uuid
import re
import shutil
from datetime import datetime
from pathlib import Path
from typing import Optional, Dict, List, Tuple
import logging

logger = logging.getLogger(__name__)


def slurm_is_available() -> bool:
    """Return True if Slurm client commands appear to be installed on this machine."""
    # Some environments expose only srun (or bind it into containers) without sbatch.
    return (shutil.which('sbatch') is not None) or (shutil.which('srun') is not None)


def cluster_from_path(path: str) -> Optional[str]:
    """Infer cluster name from a Unix-style path prefix."""
    if not path:
        return None
    if path.startswith('/net'):
        return 'krakow'
    if path.startswith('/mnt'):
        return 'southfield'
    return None


def verify_ssh_login(host: str, username: str, password: str, timeout_s: int = 12) -> Tuple[bool, str]:
    """Verify SSH credentials by attempting a short Paramiko connection."""
    try:
        import paramiko

        client = paramiko.SSHClient()
        client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        client.connect(
            hostname=host,
            username=username,
            password=password,
            timeout=timeout_s,
            auth_timeout=timeout_s,
            banner_timeout=timeout_s,
            allow_agent=False,
            look_for_keys=False,
        )
        client.close()
        return True, ''
    except Exception as exc:
        return False, str(exc)


def validate_cluster_credentials(
    username: str,
    password: str,
    clusters: List[Tuple[str, str]],
    require_all: bool = True,
) -> Tuple[bool, Dict[str, Dict[str, str]]]:
    """Validate credentials against one or more clusters.

    Args:
        username: NetID
        password: Cluster password
        clusters: list of (cluster_name, host)
        require_all: if True, all clusters must succeed; if False, any success passes

    Returns:
        (ok, details) where details maps cluster_name -> {"ok": bool, "error": str}
    """
    details: Dict[str, Dict[str, str]] = {}
    any_ok = False
    all_ok = True

    for cluster_name, host in clusters:
        ok, err = verify_ssh_login(host, username, password)
        details[cluster_name] = {'ok': bool(ok), 'error': err}
        any_ok = any_ok or ok
        all_ok = all_ok and ok

    return (all_ok if require_all else any_ok), details


class SlurmManager:
    """Manager class for Slurm job submission and monitoring"""
    
    def __init__(self, partition: str = 'compute', account: str = 'default'):
        self.partition = partition
        self.account = account
    
    def submit_job(self, 
                   script_path: str, 
                   args: List[str] = None,
                   job_name: str = None,
                   time_limit: str = '01:00:00',
                   memory: str = '8G',
                   cpus: int = 4,
                   gpu: bool = False,
                   output_dir: str = '/scratch/logs') -> Tuple[bool, str, str]:
        """
        Submit a job to Slurm using sbatch
        
        Args:
            script_path: Path to the shell script to execute
            args: Additional arguments for the script
            job_name: Name for the Slurm job
            time_limit: Time limit in HH:MM:SS format
            memory: Memory allocation (e.g., '8G')
            cpus: Number of CPUs
            gpu: Whether to request GPU resources
            output_dir: Directory for output logs
        
        Returns:
            Tuple of (success, job_id, error_message)
        """
        if not slurm_is_available():
            return False, None, "Slurm not available: 'sbatch' not found on PATH (use local execution mode on Windows)."

        if job_name is None:
            job_name = f"hpc_tool_{uuid.uuid4().hex[:8]}"
        
        # Ensure output directory exists
        os.makedirs(output_dir, exist_ok=True)
        
        # Build sbatch command
        cmd = [
            'sbatch',
            '--parsable',
            f'--partition={self.partition}',
            f'--account={self.account}',
            f'--job-name={job_name}',
            f'--time={time_limit}',
            f'--mem={memory}',
            f'--cpus-per-task={cpus}',
            f'--output={output_dir}/slurm_{job_name}_%j.out',
            f'--error={output_dir}/slurm_{job_name}_%j.err'
        ]
        
        if gpu:
            cmd.append('--gres=gpu:1')
        
        cmd.append(script_path)
        
        if args:
            cmd.extend(args)
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=30
            )
            
            if result.returncode == 0:
                job_id = result.stdout.strip()
                logger.info(f"Submitted Slurm job: {job_id}")
                return True, job_id, None
            else:
                error_msg = result.stderr.strip()
                logger.error(f"Slurm submission failed: {error_msg}")
                return False, None, error_msg
                
        except subprocess.TimeoutExpired:
            return False, None, "Slurm submission timed out"
        except Exception as e:
            return False, None, str(e)
    
    def check_job_status(self, job_id: str) -> Dict[str, str]:
        """
        Check the status of a Slurm job
        
        Returns:
            Dictionary with job status information
        """
        try:
            result = subprocess.run(
                ['sacct', '-j', job_id, '--format=JobID,State,ExitCode,Elapsed', '--noheader', '--parsable2'],
                capture_output=True,
                text=True,
                timeout=10
            )
            
            if result.returncode == 0 and result.stdout.strip():
                lines = result.stdout.strip().split('\n')
                if lines:
                    parts = lines[0].split('|')
                    status_map = {
                        'PENDING': 'QUEUED',
                        'RUNNING': 'RUNNING',
                        'COMPLETED': 'COMPLETED',
                        'FAILED': 'FAILED',
                        'CANCELLED': 'CANCELLED',
                        'TIMEOUT': 'FAILED'
                    }
                    raw_status = parts[1] if len(parts) > 1 else 'UNKNOWN'
                    return {
                        'job_id': parts[0],
                        'status': status_map.get(raw_status, raw_status),
                        'exit_code': parts[2] if len(parts) > 2 else None,
                        'elapsed': parts[3] if len(parts) > 3 else None
                    }
        except Exception as e:
            logger.error(f"Error checking job status: {e}")
        
        return {'status': 'UNKNOWN'}
    
    def cancel_job(self, job_id: str) -> Tuple[bool, str]:
        """Cancel a Slurm job"""
        try:
            result = subprocess.run(
                ['scancel', job_id],
                capture_output=True,
                text=True,
                timeout=10
            )
            
            if result.returncode == 0:
                return True, f"Job {job_id} cancelled"
            else:
                return False, result.stderr.strip()
                
        except Exception as e:
            return False, str(e)


class FileBrowser:
    """File browser for navigating cluster file systems"""
    
    def __init__(self, allowed_roots: List[str] = None):
        self.allowed_roots = allowed_roots or ['/']
    
    def is_path_allowed(self, path: str) -> bool:
        """Check if path is within allowed roots"""
        abs_path = os.path.abspath(path)
        return any(abs_path.startswith(root) for root in self.allowed_roots)
    
    def list_directory(self, path: str, extensions: List[str] = None) -> Dict:
        """
        List contents of a directory
        
        Args:
            path: Directory path to list
            extensions: Optional list of file extensions to filter
        
        Returns:
            Dictionary with directories and files
        """
        if not self.is_path_allowed(path):
            raise PermissionError(f"Access to {path} is not allowed")
        
        if not os.path.exists(path):
            raise FileNotFoundError(f"Path {path} does not exist")
        
        if not os.path.isdir(path):
            raise ValueError(f"{path} is not a directory")
        
        directories = []
        files = []
        
        try:
            for entry in os.scandir(path):
                if entry.name.startswith('.'):
                    continue
                
                if entry.is_dir():
                    directories.append({
                        'name': entry.name,
                        'path': entry.path,
                        'modified': datetime.fromtimestamp(entry.stat().st_mtime).isoformat()
                    })
                elif entry.is_file():
                    if extensions:
                        ext = entry.name.split('.')[-1].lower() if '.' in entry.name else ''
                        if ext not in extensions:
                            continue
                    
                    files.append({
                        'name': entry.name,
                        'path': entry.path,
                        'size': entry.stat().st_size,
                        'modified': datetime.fromtimestamp(entry.stat().st_mtime).isoformat()
                    })
        except PermissionError:
            raise PermissionError(f"Permission denied accessing {path}")
        
        directories.sort(key=lambda x: x['name'].lower())
        files.sort(key=lambda x: x['name'].lower())
        
        return {
            'current_path': path,
            'directories': directories,
            'files': files
        }
    
    def get_parent_path(self, path: str) -> Optional[str]:
        """Get parent directory path if allowed"""
        parent = os.path.dirname(path)
        if self.is_path_allowed(parent):
            return parent
        return None


def extract_hdf_filename(path: str) -> str:
    """Extract meaningful filename from HDF file path"""
    if not path:
        return ''
    
    filename = os.path.basename(path)
    
    # Try to extract date/ID patterns from filename
    patterns = [
        r'(\d{4}-\d{2}-\d{2})',  # YYYY-MM-DD
        r'(\d{8})',              # YYYYMMDD
        r'(Log_\d+)',            # Log_XXXXX
    ]
    
    for pattern in patterns:
        match = re.search(pattern, filename)
        if match:
            return f"{match.group(1)} - {filename[:30]}..."
    
    # Truncate if too long
    if len(filename) > 40:
        return filename[:37] + '...'
    
    return filename


def generate_slurm_script(tool_name: str, input_path: str, output_path: str, 
                          singularity_image: str, **kwargs) -> str:
    """
    Generate a Slurm batch script for running a tool
    
    Returns:
        Path to generated script
    """
    script_dir = '/tmp/slurm_scripts'
    os.makedirs(script_dir, exist_ok=True)
    
    script_name = f"{tool_name}_{uuid.uuid4().hex[:8]}.sh"
    script_path = os.path.join(script_dir, script_name)
    
    # Tool-specific configurations
    tool_configs = {
        'interactive_plot': {
            'memory': '64G',
            'cpus': 8,
            'time_limit': '04:00:00',
            'partition': 'plcyf-com',
            'module': 'singularity/3.11.4'
        },
        'kpi': {
            'memory': '32G',
            'cpus': 8,
            'time_limit': '02:00:00',
            'partition': 'plcyf-com',
            'module': 'singularity/3.11.4'
        },
        'dc_html': {
            'memory': '16G',
            'cpus': 4,
            'time_limit': '01:00:00',
            'partition': 'plcyf-com',
            'module': 'singularity/3.11.4'
        },
        'hyperlink_tool': {
            'memory': '8G',
            'cpus': 2,
            'time_limit': '00:30:00',
            'partition': 'plcyf-com',
            'module': 'singularity/3.11.4'
        }
    }
    
    config = tool_configs.get(tool_name, {
        'memory': '16G',
        'cpus': 4,
        'time_limit': '01:00:00',
        'partition': 'plcyf-com',
        'module': 'singularity/3.11.4'
    })
    
    # Override with kwargs if provided
    memory = kwargs.get('memory', config['memory'])
    cpus = kwargs.get('cpus', config['cpus'])
    time_limit = kwargs.get('time_limit', config['time_limit'])
    partition = kwargs.get('partition', config['partition'])
    module = kwargs.get('module', config['module'])
    
    # Get additional parameters
    input_mode = kwargs.get('input_mode', 'json')
    config_xml = kwargs.get('config_xml', '')
    input_hdf = kwargs.get('input_hdf', '')
    output_hdf = kwargs.get('output_hdf', '')
    html_dir = kwargs.get('html_dir', '')
    
    # Default html_dir to html_db in simg folder if not provided
    if not html_dir:
        simg_dir = os.path.dirname(singularity_image)
        html_dir = os.path.join(simg_dir, 'html_db')
    
    # Generate tool-specific run command
    if tool_name == 'interactive_plot':
        if input_mode == 'json':
            run_command = f'''singularity run --bind /net:/net --bind /scratch:/scratch \\
    {singularity_image} \\
    "{config_xml}" "{input_path}" "{html_dir}"'''
        else:  # HDF pair mode
            run_command = f'''singularity run --bind /net:/net --bind /scratch:/scratch \\
    {singularity_image} \\
    "{input_hdf}" "{output_hdf}" "{html_dir}"'''
            
    elif tool_name == 'kpi':
        if input_mode == 'json':
            run_command = f'''singularity run --bind /net:/net --bind /scratch:/scratch \\
    {singularity_image} json \\
    "{input_path}" "{html_dir}"'''
        else:  # HDF pair mode
            run_command = f'''singularity run --bind /net:/net --bind /scratch:/scratch \\
    {singularity_image} hdf \\
    "{input_hdf}" "{output_hdf}" "{html_dir}"'''
    else:
        # Default run command for other tools
        run_command = f'''singularity run --bind /net:/net --bind /scratch:/scratch \\
    {singularity_image} \\
    --input "{input_path}" \\
    --output "{output_path}"'''
    
    script_content = f'''#!/bin/bash
#SBATCH --job-name={tool_name}
#SBATCH --partition={partition}
#SBATCH --mem={memory}
#SBATCH --cpus-per-task={cpus}
#SBATCH --time={time_limit}
#SBATCH --output=/scratch/logs/{tool_name}_%j.out
#SBATCH --error=/scratch/logs/{tool_name}_%j.err

echo "========================================"
echo "Starting {tool_name} job at $(date)"
echo "Job ID: $SLURM_JOB_ID"
echo "Node: $SLURM_NODELIST"
echo "========================================"
echo "Input Mode: {input_mode}"
echo "Input: {input_path if input_mode == 'json' else input_hdf}"
echo "Output Dir: {html_dir}"
echo "========================================"

# Load singularity module
module load {module} || echo "Module {module} not found, continuing..."

# Create output directory
mkdir -p "{html_dir}"
mkdir -p /scratch/logs

# Run the tool using singularity with srun for resource allocation
srun --exclusive -N 1 -n 1 \\
    {run_command}

exit_code=$?

echo "========================================"
echo "Job completed at $(date)"
echo "Exit code: $exit_code"
echo "========================================"

exit $exit_code
'''
    
    with open(script_path, 'w') as f:
        f.write(script_content)
    
    os.chmod(script_path, 0o755)
    
    return script_path


def format_file_size(size: int) -> str:
    """Format file size in human-readable format"""
    for unit in ['B', 'KB', 'MB', 'GB', 'TB']:
        if size < 1024:
            return f"{size:.1f} {unit}"
        size /= 1024
    return f"{size:.1f} PB"
