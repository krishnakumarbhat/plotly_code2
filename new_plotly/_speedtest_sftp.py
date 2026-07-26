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

host = '10.192.224.131'
transport = paramiko.Transport((host, 22))
transport.default_window_size = paramiko.common.MAX_WINDOW_SIZE
transport.packetizer.REKEY_BYTES = pow(2, 40)
transport.packetizer.REKEY_PACKETS = pow(2, 40)
transport.connect(username=env['netid'], password=env['netid_password'])
sftp = paramiko.SFTPClient.from_transport(transport, window_size=paramiko.common.MAX_WINDOW_SIZE, max_packet_size=paramiko.common.MAX_WINDOW_SIZE)

local_file = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\rag\model\gemma-4-12b-it-Q5_K_M.gguf")
remote_tmp = f"{env['southfield_path']}/rag/model/gemma-4-12b-it-Q5_K_M.gguf.speedtest"

start = time.time()
CHUNK = 32 * 1024 * 1024
TEST_BYTES = 200 * 1024 * 1024
written = 0
with open(local_file, 'rb') as lf, sftp.open(remote_tmp, 'wb') as rf:
    rf.set_pipelined(True)
    while written < TEST_BYTES:
        data = lf.read(CHUNK)
        if not data:
            break
        rf.write(data)
        written += len(data)
elapsed = time.time() - start
print(f"wrote {written/1e6:.1f} MB in {elapsed:.1f}s => {written/elapsed/1e6:.2f} MB/s")

sftp.remove(remote_tmp)
sftp.close()
transport.close()
