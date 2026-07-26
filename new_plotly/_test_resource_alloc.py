import re, requests
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
print('login', r.status_code)

r = s.get(f"{BASE}/api/resource/status")
print('status', r.status_code, r.text)

r = s.post(f"{BASE}/api/resource/allocate", json={'cluster': 'krakow', 'nodes': 1, 'memory_gb': 64})
print('allocate', r.status_code, r.text)

r = s.get(f"{BASE}/api/resource/status")
print('status2', r.status_code, r.text)

r = s.post(f"{BASE}/api/resource/release")
print('release', r.status_code, r.text)
