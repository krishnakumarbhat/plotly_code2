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
    ('krakow', env.get('KRAKOW_HOST') or '10.214.45.45', '/local/hpc_tools/cache_html/resim_runs/resim_run_f0e51c48.log'),
    ('southfield', env.get('SOUTHFIELD_HOST') or '10.192.224.131', '/mnt/usmidet/projects/GPO-IFV7XX/8-Users/3we243/resim_run_899f93cd.log'),
]

for name, host, logpath in targets:
    print(f'=== {name} {logpath} ===')
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
    cmd = f"bash -lc \"cat '{logpath}' 2>&1 || echo MISSING\""
    stdin, stdout, stderr = client.exec_command(cmd, timeout=20)
    print(stdout.read().decode())
    client.close()
