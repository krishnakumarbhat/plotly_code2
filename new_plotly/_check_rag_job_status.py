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
remote_root = env['krakow_path']

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)

setup = "source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null;"
cmds = [
    f"{setup} squeue -u ouymc2 -o '%.10i %.9P %.20j %.8T %.10M %.6D %R' 2>&1",
    f"tail -n 60 {remote_root}/logs/rag_srun.log 2>&1",
]
for c in cmds:
    print('=== ', c[:60], ' ===')
    stdin, stdout, stderr = client.exec_command(f"bash -lc '{c}'", timeout=20)
    print(stdout.read().decode())
    print(stderr.read().decode())
client.close()
