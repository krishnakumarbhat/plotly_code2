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

host = env.get('KRAKOW_HOST') or '10.214.45.45'
remote_root = env['krakow_path']
local_app = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\main_html\app.py")

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
sftp = client.open_sftp()
dst = f"{remote_root}/bundle_src/main_html/app.py"
sftp.put(str(local_app), dst)
print('pushed', dst, sftp.stat(dst).st_size)
sftp.close()

cmd = "bash -lc \"pgrep -f 'gunicorn -c' | tr '\\n' ' '\""
stdin, stdout, stderr = client.exec_command(cmd, timeout=20)
pids = stdout.read().decode().split()
masters = []
for pid in pids:
    cmdp = f"bash -lc \"cat /proc/{pid}/status 2>/dev/null | grep -E '^PPid:'; tr '\\0' '\\n' < /proc/{pid}/environ 2>/dev/null | grep -E '^HPCC_BUNDLE_ROOT='\""
    stdin, stdout, stderr = client.exec_command(cmdp, timeout=20)
    out = stdout.read().decode()
    if 'all_services_5' in out:
        ppid = [l for l in out.splitlines() if l.startswith('PPid:')][0].split(':')[1].strip()
        masters.append((pid, ppid))
pid_set = set(pids)
master_pid = next((pid for pid, ppid in masters if ppid not in pid_set), None)
if not master_pid and masters:
    master_pid = min((m[0] for m in masters), key=int)
print('reload target master pid:', master_pid)
if master_pid:
    stdin, stdout, stderr = client.exec_command(f"kill -HUP {master_pid}", timeout=20)
    print('HUP sent to', master_pid, stderr.read().decode())

# Sanity check 'yes' binary exists
stdin, stdout, stderr = client.exec_command("bash -lc 'which yes'", timeout=10)
print('yes:', stdout.read().decode().strip())
client.close()
