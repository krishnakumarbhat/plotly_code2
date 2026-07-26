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
command = "source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null; srun --jobid=43612262 --overlap --ntasks=1 cat /proc/2833926/task/2833926/children /proc/2834295/task/2834295/children 2>&1"
stdin, stdout, stderr = client.exec_command(f"bash -lc \"{command}\"", timeout=60)
print(stdout.read().decode())
print(stderr.read().decode())
client.close()
