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

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=env.get('SOUTHFIELD_HOST') or '10.192.224.131', username=env['netid'], password=env['netid_password'], timeout=30)
setup = 'source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null;'
stdin, stdout, stderr = client.exec_command(f"bash -lc '{setup} sinfo -p defq'", timeout=20)
print(stdout.read().decode())
client.close()
