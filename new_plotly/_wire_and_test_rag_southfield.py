import re, requests
from pathlib import Path

BASE = "http://10.192.224.131:5003"
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
payload = {'net_id': env['netid'], 'password': env['netid_password'], 'cluster_target': 'southfield'}
if csrf:
    payload['csrf_token'] = csrf
r = s.post(f"{BASE}/login", data=payload)
print('login', r.status_code)

# Point the runtime map's 'rag' tool at the newly-launched Slurm-hosted RAG service.
r = s.post(f"{BASE}/api/runtime/tools", json={
    'tool_key': 'rag',
    'display_name': 'RAG (Gemma GGUF)',
    'category': 'service',
    'service_url': 'http://usmidet-com-prod-com047.aptiv.com:5100',
    'notes': 'Running via srun on a dedicated 1-node/64GB Slurm allocation (tmux session hpcc_rag_southfield).',
})
print('runtime tool update', r.status_code, r.text[:300])

# Dummy end-to-end prompt through the real /api/chat endpoint (same path the KPI guide chat UI uses).
r = s.post(f"{BASE}/api/chat", json={'message': 'Hello, this is a dummy test prompt. What can you help with?'})
print('chat', r.status_code, r.text[:800])
