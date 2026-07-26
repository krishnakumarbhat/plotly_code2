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

# Run a quick check INSIDE the same rag.simg image (a lightweight one-off
# exec via srun on the SAME node the --talk server is already running on,
# so it does not disturb it) to see if the llama-server binary was actually
# baked into the image (apptainer/singularity is only in PATH on compute
# nodes, not the login node).
local_probe = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\_probe_llama.sh")
sftp = client.open_sftp()
dst_probe = f"{remote_root}/_probe_llama.sh"
sftp.put(str(local_probe), dst_probe)
sftp.chmod(dst_probe, 0o750)
sftp.close()

cmd = (
    "source /etc/profile.d/modules.sh 2>/dev/null; "
    f"cd {remote_root} && "
    "srun --partition=defq --account=radarcore --qos=normal --nodelist=usmidet-com-prod-com047 "
    "--nodes=1 --ntasks=1 --cpus-per-task=1 --mem=2G --time=00:05:00 --job-name=hpcc_rag_probe "
    "./_probe_llama.sh"
)
stdin, stdout, stderr = client.exec_command(f"bash -lc '{cmd}'", timeout=40)
print(stdout.read().decode())
print(stderr.read().decode())
client.close()
