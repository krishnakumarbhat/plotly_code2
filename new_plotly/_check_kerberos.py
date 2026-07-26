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

host = env.get('KRAKOW_HOST') or '10.214.45.45'
target_file = '/net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Support/Resim/resim_main.py'

client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
client.connect(hostname=host, username=env['netid'], password=env['netid_password'], timeout=30)

# Reproduce EXACTLY what our app does: password-auth loopback SSH to 127.0.0.1 as the same user.
cmd = (
    "ssh -tt -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o ConnectTimeout=20 "
    f"{env['netid']}@127.0.0.1 bash -lc "
    f"'echo ---id---; id; echo ---groups---; groups; echo ---klist---; klist 2>&1; "
    f"echo ---aklog---; which aklog tokens 2>&1; tokens 2>&1; "
    f"echo ---stat---; stat {target_file} 2>&1; "
    f"echo ---ls---; ls -la {target_file} 2>&1'"
)

# This SSH-to-loopback also needs an askpass; instead just run it directly over
# our EXISTING already-authenticated session (which used the real login path),
# to compare env/groups/klist from a genuine top-level SSH login vs the nested loopback hop.
print('=== outer (real) session id/groups/klist ===')
for c in ['id', 'groups', 'klist 2>&1', 'tokens 2>&1', f'ls -la {target_file} 2>&1']:
    stdin, stdout, stderr = client.exec_command(f"bash -lc '{c}'", timeout=20)
    print(f'--- {c} ---')
    print(stdout.read().decode().strip())
    print(stderr.read().decode().strip())

client.close()
