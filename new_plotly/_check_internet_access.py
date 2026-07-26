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

for name, host in [('Krakow', '10.214.45.45'), ('Southfield', '10.192.224.131')]:
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
    cmd = "curl -sS -m 15 -o /dev/null -w 'http_code=%{http_code} time=%{time_total}s speed=%{speed_download}B/s\\n' https://huggingface.co 2>&1"
    stdin, stdout, stderr = client.exec_command(f"bash -lc \"{cmd}\"", timeout=25)
    out = stdout.read().decode()
    err = stderr.read().decode()
    print(f"=== {name} ===")
    print(out, err)
    client.close()
