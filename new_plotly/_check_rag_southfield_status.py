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

host = env.get('SOUTHFIELD_HOST') or '10.192.224.131'
remote_root = env['southfield_path']
client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
setup = 'source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null;'
stdin, stdout, stderr = client.exec_command(f"bash -lc '{setup} squeue -u ouymc2'", timeout=20)
print(stdout.read().decode())
stdin, stdout, stderr = client.exec_command(f"bash -lc \"tail -n 80 '{remote_root}/logs/rag_srun.log'\"", timeout=20)
print(stdout.read().decode())
client.close()
