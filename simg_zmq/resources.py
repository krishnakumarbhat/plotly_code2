
"""
Consolidated resource allocation defaults per cluster and tool.
Edit this file to change default resources, partitions, accounts, etc.
"""
import os
from env_utils import get_env, RuntimeEnv

ENV = get_env()
CLUSTER = 'krakow' if ENV.runtime in (RuntimeEnv.KRAKOW, RuntimeEnv.WSL) and ENV.runtime != RuntimeEnv.SOUTHFIELD else 'southfield'
if hasattr(ENV, 'runtime') and ENV.runtime == RuntimeEnv.SOUTHFIELD:
    CLUSTER = 'southfield'

KRAKOW = {
    'host': os.environ.get('KRAKOW_HOST', '10.214.45.45'),
    'partition': os.environ.get('KRAKOW_PARTITION', 'plcyf-com'),
    'account': os.environ.get('KRAKOW_SLURM_ACCOUNT', ''),
    'qos': os.environ.get('KRAKOW_QOS', ''),
    'data_base': os.environ.get('KRAKOW_DATA_BASE', ''),
    'scratch': '/scratch',
    'deploy_root': os.environ.get('KRAKOW_DEPLOY_ROOT', ''),
    'model_dir': os.environ.get('KRAKOW_MODEL_DIR', ''),
    'simg_dir': os.environ.get('KRAKOW_SIMG_DIR', ''),
    'html_db_dir': os.environ.get('KRAKOW_HTML_DB_DIR', ''),
    'cache_dir': os.environ.get('KRAKOW_CACHE_DIR', ''),
    'store_dir': os.environ.get('KRAKOW_STORE_DIR', ''),
    'scripts_dir': os.environ.get('KRAKOW_SCRIPTS_DIR', ''),
    'log_dir': os.environ.get('KRAKOW_LOG_DIR', '/scratch/logs'),
}

SOUTHFIELD = {
    'host': os.environ.get('SOUTHFIELD_HOST', '10.192.224.131'),
    'partition': os.environ.get('SOUTHFIELD_PARTITION', 'defq'),
    'account': os.environ.get('SOUTHFIELD_SLURM_ACCOUNT', ''),
    'qos': os.environ.get('SOUTHFIELD_QOS', ''),
    'data_base': os.environ.get('SOUTHFIELD_DATA_BASE', ''),
    'scratch': '/scratch',
    'deploy_root': os.environ.get('SOUTHFIELD_DEPLOY_ROOT', ''),
    'model_dir': os.environ.get('SOUTHFIELD_MODEL_DIR', ''),
    'simg_dir': os.environ.get('SOUTHFIELD_SIMG_DIR', ''),
    'html_db_dir': os.environ.get('SOUTHFIELD_HTML_DB_DIR', ''),
    'cache_dir': os.environ.get('SOUTHFIELD_CACHE_DIR', ''),
    'store_dir': os.environ.get('SOUTHFIELD_STORE_DIR', ''),
    'scripts_dir': os.environ.get('SOUTHFIELD_SCRIPTS_DIR', ''),
    'log_dir': os.environ.get('SOUTHFIELD_LOG_DIR', '/scratch/logs'),
}

TOOLS = {
    'can_kpi':     {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 8,  'memory': '32G',  'time_limit': '02:00:00', 'gpu': False, 'gres': ''},
    'udp_kpi':     {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 8,  'memory': '32G',  'time_limit': '02:00:00', 'gpu': False, 'gres': ''},
    'interactive_plot': {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 8,  'memory': '64G',  'time_limit': '04:00:00', 'gpu': False, 'gres': ''},
    'rag':         {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 8,  'memory': '72G',  'time_limit': '168:00:00', 'gpu': True, 'gres': 'gpu:1'},
    'main_html':   {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 4,  'memory': '16G',  'time_limit': '168:00:00', 'gpu': False, 'gres': ''},
    'jira':        {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 2,  'memory': '8G',   'time_limit': '00:30:00', 'gpu': False, 'gres': '', 'jira_url': 'https://jira.anomaly.com', 'jira_project': 'FHW'},
    'hyperlink_tool': {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 2,  'memory': '8G',   'time_limit': '00:30:00', 'gpu': False, 'gres': ''},
}


def cluster_config(name: str = '') -> dict:
    """Return cluster config dict for the given cluster name (default: auto-detected)."""
    if not name:
        name = CLUSTER
    return KRAKOW if name == 'krakow' else SOUTHFIELD


def tool_resources(tool: str) -> dict:
    """Return resource dict for a tool (falls back to udp_kpi defaults)."""
    return dict(TOOLS.get(tool, TOOLS['udp_kpi']))


def sbatch_header(tool: str, cluster: str = '') -> list[str]:
    """Build #SBATCH directive list for a tool on a given cluster."""
    cfg = cluster_config(cluster)
    res = tool_resources(tool)
    return [
        f'#SBATCH --partition={cfg["partition"]}',
        f'#SBATCH --account={cfg["account"]}',
        f'#SBATCH --job-name=hpc_{tool}',
        f'#SBATCH --nodes={res["nodes"]}',
        f'#SBATCH --ntasks={res["ntasks"]}',
        f'#SBATCH --cpus-per-task={res["cpus"]}',
        f'#SBATCH --mem={res["memory"]}',
        f'#SBATCH --time={res["time_limit"]}',
    ] + ([f'#SBATCH --gres={res["gres"]}'] if res.get('gres') else []) + [
        f'#SBATCH --output={cfg["log_dir"]}/hpc_{tool}_%j.out',
        f'#SBATCH --error={cfg["log_dir"]}/hpc_{tool}_%j.err',
    ]


def generate_slurm_cmd(tool: str, script_path: str, cluster: str = '', **overrides) -> list[str]:
    """Return full sbatch command array for a tool script with cluster resources."""
    cfg = cluster_config(cluster)
    res = tool_resources(tool)
    return [
        'sbatch',
        f'--partition={overrides.get("partition", cfg["partition"])}',
        f'--account={overrides.get("account", cfg["account"])}',
        f'--job-name=hpc_{tool}',
        f'--nodes={overrides.get("nodes", res["nodes"])}',
        f'--ntasks={overrides.get("ntasks", res["ntasks"])}',
        f'--cpus-per-task={overrides.get("cpus", res["cpus"])}',
        f'--mem={overrides.get("memory", res["memory"])}',
        f'--time={overrides.get("time_limit", res["time_limit"])}',
    ] + ([f'--gres={overrides.get("gres", res["gres"])}'] if res.get('gres') else []) + [
        f'--output={cfg["log_dir"]}/hpc_{tool}_%j.out',
        f'--error={cfg["log_dir"]}/hpc_{tool}_%j.err',
        str(script_path),
    ]


def build_container_cmd(
    singularity_image: str,
    binds: list[str] | None = None,
    tool_args: list[str] | None = None,
) -> list[str]:
    """Build a singularity run command list with bind mounts and tool args."""
    cmd = ['singularity', 'run']
    for b in (binds or ['/net:/net', '/scratch:/scratch', '/tmp:/tmp']):
        cmd += ['--bind', b]
    cmd.append(singularity_image)
    if tool_args:
        cmd.extend(tool_args)
    return cmd


def cluster_ssh_cmd(host: str, user: str, remote_cmd: str, port: int = 22) -> list[str]:
    """Return an SSH command list to execute a remote command on a cluster node."""
    cmd = ['ssh']
    if port and port != 22:
        cmd += ['-p', str(port)]
    cmd += [f'{user}@{host}', remote_cmd]
    return cmd
