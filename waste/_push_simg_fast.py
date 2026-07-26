from pathlib import Path
import socket
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

sock = socket.create_connection((host, 22), timeout=60)
transport = paramiko.Transport(sock)
transport.connect(username=env['netid'], password=env['netid_password'])
transport.set_keepalive(30)
sftp = paramiko.SFTPClient.from_transport(
    transport,
    window_size=64 * 1024 * 1024,
    max_packet_size=4 * 1024 * 1024,
)
sftp.get_channel().settimeout(300)

dst_tmp = f"{remote_root}/main_html.simg.new"
dst = f"{remote_root}/main_html.simg"
size = local_simg.stat().st_size
print('uploading', size, 'bytes to', dst_tmp)
t0 = time.time()

last = [0, t0]
def cb(sent, total):
    now = time.time()
    if now - last[1] > 10:
        print(f'{sent}/{total} bytes ({sent/total*100:.1f}%) elapsed={now-t0:.0f}s')
        last[1] = now

sftp.put(str(local_simg), dst_tmp, callback=cb, confirm=True)
print('upload done in', round(time.time() - t0, 1), 's')
sftp.rename(dst_tmp, dst)
print('renamed into place:', dst, sftp.stat(dst).st_size)
sftp.close()
transport.close()
