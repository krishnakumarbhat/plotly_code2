from pathlib import Path
import paramiko

env_path = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\.env")
env = {}
for raw in env_path.read_text().splitlines():
    line = raw.strip()
    if not line or line.startswith('#') or '=' not in line:
        continue
    key, value = line.split('=', 1)
    env[key.strip()] = value.strip().strip('"').strip("'")

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname='10.192.224.131', username=env['netid'], password=env['netid_password'], timeout=30)
commands = [
    "source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null; squeue -j 43612262 -o '%.18i %.9P %.20j %.2t %.10M %.6D %R'",
    "source /etc/profile.d/modules.sh 2>/dev/null; module load slurm 2>/dev/null; scontrol show job 43612262 | grep -E 'JobState|NodeList|Command|WorkDir|Reason|RunTime|TimeLimit'",
    "tail -n 30 /mnt/usmidet/projects/RADARCORE/2-Sim/all_services_5/logs/rag_srun.log",
]
for command in commands:
    stdin, stdout, stderr = client.exec_command(f"bash -lc \"{command}\"", timeout=30)
    print(f"=== {command} ===")
    print(stdout.read().decode())
    print(stderr.read().decode())
client.close()
