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
    ('Krakow', env.get('KRAKOW_HOST') or '10.214.45.45', env.get('krakow_path')),
    ('Southfield', env.get('SOUTHFIELD_HOST') or '10.192.224.131', env.get('southfield_path')),
]

for name, host, remote_root in targets:
    if not remote_root:
        print(name, 'no remote_root configured, skipping')
        continue
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
    cmd = f"ls -la {remote_root}/rag/model/*.gguf 2>&1"
    stdin, stdout, stderr = client.exec_command(f"bash -lc '{cmd}'", timeout=30)
    print(f"=== {name} ({host}) ===")
    print(stdout.read().decode())
    print(stderr.read().decode())
    client.close()
