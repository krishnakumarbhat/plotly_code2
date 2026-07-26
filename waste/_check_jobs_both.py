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
    cmd = "bash -lc \"find /local /var/tmp /tmp -maxdepth 4 -name hpc_tools_dev.db 2>/dev/null\""
    stdin, stdout, stderr = client.exec_command(cmd, timeout=20)
    dbs = stdout.read().decode().split()
    print('dbs:', dbs)
    for db in dbs:
        cmd2 = f"bash -lc \"sqlite3 {db} 'SELECT id, status, output_log_path, error_message FROM job_history WHERE tool_name=\\\"resim_run\\\" ORDER BY id DESC LIMIT 3;'\""
        stdin, stdout, stderr = client.exec_command(cmd2, timeout=20)
        print(stdout.read().decode())
    client.close()
