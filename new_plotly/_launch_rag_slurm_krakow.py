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

local_script = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\start_rag_slurm.sh")

host = env.get('KRAKOW_HOST') or '10.214.45.45'
remote_root = env['krakow_path']

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
sftp = client.open_sftp()
dst = f"{remote_root}/start_rag_slurm.sh"
sftp.put(str(local_script), dst)
sftp.chmod(dst, 0o750)
print('pushed', dst)
sftp.close()

# Kill any previous tmux session with the same name (idempotent restart).
client.exec_command("tmux kill-session -t hpcc_rag_krakow 2>/dev/null; true", timeout=10)

cmd = f"cd {remote_root} && tmux new-session -d -s hpcc_rag_krakow './start_rag_slurm.sh'"
stdin, stdout, stderr = client.exec_command(f"bash -lc \"{cmd}\"", timeout=20)
print('start rc', stdout.channel.recv_exit_status())
print(stdout.read().decode())
print(stderr.read().decode())

stdin, stdout, stderr = client.exec_command("tmux list-sessions 2>&1", timeout=10)
print('tmux sessions:', stdout.read().decode())
client.close()
