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

targets = [
    ('krakow', env.get('KRAKOW_HOST') or '10.214.45.45', env['krakow_path']),
    ('southfield', env.get('SOUTHFIELD_HOST') or '10.192.224.131', env['southfield_path']),
]

for name, host, remote_root in targets:
    print(f'=== {name} {host} ===')
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
    cmd = f"bash -lc \"ls -la {remote_root}/main_html.simg {remote_root}/bin/xxd {remote_root}/rResim_Gen7.sh 2>&1\""
    stdin, stdout, stderr = client.exec_command(cmd, timeout=20)
    print(stdout.read().decode())
    # Check if ssh binary is present inside the container by running the image directly
    cmd2 = f"bash -lc \"cd {remote_root} && source /etc/profile.d/modules.sh >/dev/null 2>&1; module load singularity >/dev/null 2>&1 || module load apptainer >/dev/null 2>&1; RUNTIME=\\$(command -v apptainer || command -v singularity); \\$RUNTIME exec main_html.simg which ssh 2>&1\""
    stdin, stdout, stderr = client.exec_command(cmd2, timeout=30)
    print('ssh in container:', stdout.read().decode().strip(), stderr.read().decode().strip())
    client.close()
