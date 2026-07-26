from pathlib import Path
import paramiko

env_path = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\.env")
env = {}
for raw in env_path.read_text().splitlines():
    line = raw.strip()
    if not line or line.startswith('#') or '=' not in line:
        continue
    key, value = line.split('=', 1)
    env[key.strip()] = value.strip().strip('"').strip("'")

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname='10.192.224.131', username=env['netid'], password=env['netid_password'], timeout=30)
commands = [
    "source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null; sstat -j 43612262.batch --format=JobID,MaxRSS,AveCPU,MaxDiskRead,MaxDiskWrite",
    "source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null; scontrol listpids 43612262 2>&1",
    "source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null; srun --jobid=43612262 --overlap --ntasks=1 hostname 2>&1",
]
for command in commands:
    stdin, stdout, stderr = client.exec_command(f"bash -lc \"{command}\"", timeout=45)
    print(f"=== {command} ===")
    print(stdout.read().decode())
    print(stderr.read().decode())
client.close()
