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

BASE = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq")
files = {
    'bundle_src/main_html/app.py': BASE / 'main_html' / 'app.py',
    'bundle_src/main_html/models.py': BASE / 'main_html' / 'models.py',
    'bundle_src/main_html/templates/dashboard.html': BASE / 'main_html' / 'templates' / 'dashboard.html',
    'main_hpcc.sh': BASE / 'main_hpcc.sh',
}

targets = [
    ('krakow', env.get('KRAKOW_HOST') or '10.214.45.45', env['krakow_path']),
    ('southfield', env.get('SOUTHFIELD_HOST') or '10.192.224.131', env['southfield_path']),
]

for name, host, remote_root in targets:
    print(f'=== {name} {host} ===')
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
    sftp = client.open_sftp()
    for rel, local in files.items():
        dst = f"{remote_root}/{rel}"
        sftp.put(str(local), dst)
        print('pushed', dst, sftp.stat(dst).st_size)
    sftp.close()

    # Normalize main_hpcc.sh to LF on the remote side too (Windows edit tool
    # may have re-saved with CRLF); harmless no-op if already LF.
    client.exec_command(f"sed -i 's/\\r$//' '{remote_root}/main_hpcc.sh'", timeout=15)

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
    client.close()
