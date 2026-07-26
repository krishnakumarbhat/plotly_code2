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
    "source /etc/profile.d/modules.sh; module avail apptainer 2>&1",
    "source /etc/profile.d/modules.sh; module avail singularity 2>&1",
    "source /etc/profile.d/modules.sh; module avail 2>&1 | grep -i -E 'apptainer|singularity'",
]
for command in commands:
    stdin, stdout, stderr = client.exec_command(f"bash -lc '{command}'", timeout=45)
    print(f"=== {command} ===")
    print(stdout.read().decode())
    print(stderr.read().decode())
client.close()
