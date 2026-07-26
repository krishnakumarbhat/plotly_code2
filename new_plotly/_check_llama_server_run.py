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

local_script = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\_probe_llama_run.sh")
sftp = client.open_sftp()
dst = f"{remote_root}/_probe_llama_run.sh"
sftp.put(str(local_script), dst)
sftp.chmod(dst, 0o750)
sftp.close()

cmd = (
    "source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null; "
    f"cd {remote_root} && srun --partition=defq --account=radarcore --qos=normal "
    "--nodelist=usmidet-com-prod-com047 --nodes=1 --ntasks=1 --cpus-per-task=4 --mem=8G "
    "--time=00:05:00 --job-name=hpcc_rag_probe2 ./_probe_llama_run.sh"
)
stdin, stdout, stderr = client.exec_command(f"bash -lc '{cmd}'", timeout=100)
print(stdout.read().decode())
print('STDERR:')
print(stderr.read().decode())
client.close()
