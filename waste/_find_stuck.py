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
client.connect(hostname=(env.get('KRAKOW_HOST') or '10.214.45.45'), username=env['netid'], password=env['netid_password'], timeout=30)

# Find stuck ssh/resim processes owned by ouymc2
cmd = "bash -lc \"ps -ef | grep -E 'ssh .*127.0.0.1|rResim_Gen7|resim_main.py' | grep -v grep\""
stdin, stdout, stderr = client.exec_command(cmd, timeout=20)
print(stdout.read().decode())
client.close()
