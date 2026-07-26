import sys
import time
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

TARGET = sys.argv[1] if len(sys.argv) > 1 else 'southfield'

if TARGET == 'southfield':
    host = '10.192.224.131'
    remote_root = env['southfield_path']
else:
    host = '10.214.45.45'
    remote_root = env['krakow_path']

local_file = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\rag\model\gemma-4-12b-it-Q5_K_M.gguf")
local_size = local_file.stat().st_size
remote_dir = f"{remote_root}/rag/model"
remote_tmp = f"{remote_dir}/gemma-4-12b-it-Q5_K_M.gguf.uploading"
remote_final = f"{remote_dir}/gemma-4-12b-it-Q5_K_M.gguf"

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)

sftp = client.open_sftp()
sftp.get_channel().settimeout(300)

print(f"Uploading {local_size/1e9:.2f} GB to {TARGET} ({host}):{remote_tmp}")
start = time.time()
last_report = [0.0]

def progress(sent, total):
    now = time.time()
    if now - last_report[0] > 15 or sent == total:
        pct = sent / total * 100
        elapsed = now - start
        rate = sent / elapsed / 1e6 if elapsed > 0 else 0
        print(f"  {pct:5.1f}%  {sent/1e9:.2f}/{total/1e9:.2f} GB  {rate:.1f} MB/s  elapsed={elapsed:.0f}s")
        last_report[0] = now

sftp.put(str(local_file), remote_tmp, callback=progress)

remote_size = sftp.stat(remote_tmp).st_size
print(f"Upload finished. remote tmp size={remote_size} local size={local_size}")
if remote_size != local_size:
    print("SIZE MISMATCH - aborting rename, NOT replacing final file")
    sftp.close()
    client.close()
    sys.exit(1)

# atomic rename into place
stdin, stdout, stderr = client.exec_command(f"mv -f '{remote_tmp}' '{remote_final}'", timeout=60)
rc = stdout.channel.recv_exit_status()
print("rename rc:", rc, stderr.read().decode())

stdin, stdout, stderr = client.exec_command(f"ls -la '{remote_final}'", timeout=30)
print(stdout.read().decode())

sftp.close()
client.close()
print("DONE", TARGET)
