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

host = env.get('SOUTHFIELD_HOST') or '10.192.224.131'
remote_root = env['southfield_path']

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)

local_script = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\_direct_rag_ask_test.py")
sftp = client.open_sftp()
dst = f"{remote_root}/_direct_rag_ask_test.py"
sftp.put(str(local_script), dst)
sftp.close()

stdin, stdout, stderr = client.exec_command(f"bash -lc \"cd {remote_root} && python3 _direct_rag_ask_test.py\"", timeout=220)
print(stdout.read().decode())
print(stderr.read().decode())
client.close()
