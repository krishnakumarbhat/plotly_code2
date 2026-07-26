from pathlib import Path
import paramiko

env_path = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\.env")
env = {}
for raw in env_path.read_text().splitlines():
    line = raw.strip()
    if not line or line.startswith('#') or '=' not in line:
        continue
    k, v = line.split('=', 1)
    env[k.strip()] = v.strip().strip('"').strip("'")

host = env.get('KRAKOW_HOST') or '10.214.45.45'
client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)

cmds = [
    "source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>&1; sinfo -o '%P %a %l %D %N' 2>&1 | head -20",
    "squeue -u ouymc2 2>&1",
    "which tmux srun sbatch 2>&1",
    "sacctmgr show assoc user=ouymc2 format=account,partition,qos 2>&1 | head -10",
]
for c in cmds:
    print('=== ', c, ' ===')
    stdin, stdout, stderr = client.exec_command(f"bash -lc \"{c}\"", timeout=30)
    print(stdout.read().decode())
    print(stderr.read().decode())
client.close()
