import re, time, requests
from pathlib import Path

BASE = "http://10.214.45.45:5003"
env_path = Path(r"C:\Users\ouymc2\Desktop\simg\Plotly_code\simg_zmq\.env")
env = {}
for raw in env_path.read_text().splitlines():
    line = raw.strip()
    if not line or line.startswith('#') or '=' not in line:
        continue
    k, v = line.split('=', 1)
    env[k.strip()] = v.strip().strip('"').strip("'")

s = requests.Session()
r = s.get(f"{BASE}/login")
m = re.search(r'name="csrf_token" value="([^"]+)"', r.text)
csrf = m.group(1) if m else None
payload = {'net_id': env['netid'], 'password': env['netid_password'], 'cluster_target': 'krakow'}
if csrf:
    payload['csrf_token'] = csrf
r = s.post(f"{BASE}/login", data=payload)
print('login', r.status_code, r.url)

input_txt = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/2-Sim/USER_DATA/pcmzxl/simg/pt033.txt"
simg_path = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/7-Tools/ReSim/Official_Release/RADAR/GPO-GEN7/R10.0/R10_0_12/docker/resim_v2_r10_platform_010.000.12.simg"

r = s.post(f"{BASE}/api/resim_run_submit", json={'input_txt': input_txt, 'simg_path': simg_path})
print('submit', r.status_code, r.text[:300])
try:
    job_id = r.json().get('job_id')
except Exception:
    job_id = None
print('job_id', job_id)

if job_id:
    for i in range(20):
        time.sleep(3)
        jr = s.get(f"{BASE}/api/job/{job_id}/status")
        try:
            js = jr.json()
        except Exception:
            print('NON-JSON STATUS RESPONSE:', jr.status_code, jr.text[:200])
            break
        print(i, jr.status_code, js.get('status'))
        if js.get('status') in ('COMPLETED', 'FAILED'):
            break

    lr = s.get(f"{BASE}/html/job/{job_id}/log")
    body = lr.text
    idx = body.find('consoleOutput')
    print('--- log ---')
    print(body[idx-20:idx+1800] if idx > 0 else body[:800])
