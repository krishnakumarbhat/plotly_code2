from pathlib import Path
import time
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
local_simg = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\generate_upload\main_html.simg")

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
sftp = client.open_sftp()
dst = f"{remote_root}/main_html.simg"
size = local_simg.stat().st_size
print('uploading', size, 'bytes...')
t0 = time.time()
sftp.put(str(local_simg), dst)
print('done in', round(time.time() - t0, 1), 's, remote size:', sftp.stat(dst).st_size)
sftp.close()
client.close()
