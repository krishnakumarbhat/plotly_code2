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
cmd = "bash -lc \"sqlite3 /local/hpc_tools/db/hpc_tools_dev.db 'SELECT id, status, error_message FROM job_history WHERE id=29;'\""
stdin, stdout, stderr = client.exec_command(cmd, timeout=20)
print(stdout.read().decode())
cmd2 = f"bash -lc \"tail -n 40 {env['krakow_path']}/logs/main_html.log\""
stdin, stdout, stderr = client.exec_command(cmd2, timeout=20)
print(stdout.read().decode())
client.close()
