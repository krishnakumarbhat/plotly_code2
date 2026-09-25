
"""
Consolidated resource allocation defaults per cluster and tool.
Edit this file to change default resources, partitions, accounts, etc.
"""
from env_utils import get_env, RuntimeEnv

ENV = get_env()
CLUSTER = 'krakow' if ENV.runtime in (RuntimeEnv.KRAKOW, RuntimeEnv.WSL) and ENV.runtime != RuntimeEnv.SOUTHFIELD else 'southfield'
if hasattr(ENV, 'runtime') and ENV.runtime == RuntimeEnv.SOUTHFIELD:
    CLUSTER = 'southfield'

KRAKOW = {
    'host': '10.214.45.45',
    'partition': 'plcyf-com',
    'account': 'RNA-SDV-SRR7',
    'qos': '',
    'data_base': '/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/2-Sim/USER_DATA',
    'scratch': '/scratch',
    'deploy_root': '/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_services_3',
    'model_dir': '/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/rag/model',
}

SOUTHFIELD = {
    'host': '10.192.224.131',
    'partition': 'defq',
    'account': 'radarcore',
    'qos': '',
    'data_base': '/mnt/usmidet/projects/RADARCORE/2-Sim/USER_DATA/ouymc',
    'scratch': '/scratch',
    'deploy_root': '/mnt/usmidet/projects/RADARCORE/2-Sim/all_services_3',
    'model_dir': '/mnt/usmidet/projects/RADARCORE/rag/model',
}

TOOLS = {
    'can_kpi':     {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 8,  'memory': '32G',  'time_limit': '02:00:00', 'gpu': False, 'gres': ''},
    'udp_kpi':     {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 8,  'memory': '32G',  'time_limit': '02:00:00', 'gpu': False, 'gres': ''},
    'interactive_plot': {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 8,  'memory': '64G',  'time_limit': '04:00:00', 'gpu': False, 'gres': ''},
    'rag':         {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 8,  'memory': '72G',  'time_limit': '168:00:00', 'gpu': True, 'gres': 'gpu:1'},
    'main_html':   {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 4,  'memory': '16G',  'time_limit': '168:00:00', 'gpu': False, 'gres': ''},
    'jira':        {'scheduler': 'slurm', 'nodes': 1, 'ntasks': 1, 'cpus': 4,  'memory': '16G',  'time_limit': '168:00:00', 'gpu': False, 'gres': ''},
}

def cluster_config(name: str = '') -> dict:
    if not name:
        name = CLUSTER
    return KRAKOW if name == 'krakow' else SOUTHFIELD

def tool_resources(tool: str) -> dict:
    return dict(TOOLS.get(tool, TOOLS['udp_kpi']))

def sbatch_header(tool: str, cluster: str = '') -> list[str]:
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
        f'#SBATCH --output=/scratch/logs/hpc_{tool}_%j.out',
        f'#SBATCH --error=/scratch/logs/hpc_{tool}_%j.err',
    ]
