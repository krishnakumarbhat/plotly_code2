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

targets = [
    ('krakow', env.get('KRAKOW_HOST') or '10.214.45.45', env['krakow_path'], '/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_services_5/rag/model/gemma-4-12b-it-Q5_K_M.gguf'),
    ('southfield', env.get('SOUTHFIELD_HOST') or '10.192.224.131', env['southfield_path'], '/mnt/usmidet/projects/RADARCORE/2-Sim/all_services_5/rag/model/gemma-4-12b-it-Q5_K_M.gguf'),
]

for name, host, remote_root, gguf in targets:
    print(f'=== {name} {host} ===')
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)
    cmd = f"bash -lc \"ls -la '{gguf}' 2>&1; echo ---procs---; pgrep -af 'rag|llama|gguf' 2>&1 | head -20; echo ---ports---; (ss -ltnp 2>/dev/null || netstat -ltnp 2>/dev/null) | grep -E '5100|5003|9100' \""
    stdin, stdout, stderr = client.exec_command(cmd, timeout=25)
    print(stdout.read().decode())
    print(stderr.read().decode())
    client.close()
