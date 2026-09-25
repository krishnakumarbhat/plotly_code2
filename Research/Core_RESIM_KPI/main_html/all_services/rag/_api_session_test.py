import json
import urllib.request

def post(payload):
    req = urllib.request.Request(
        'http://127.0.0.1:5000/ask',
        data=json.dumps(payload).encode('utf-8'),
        headers={'Content-Type': 'application/json'},
        method='POST',
    )
    with urllib.request.urlopen(req, timeout=180) as resp:
        return json.loads(resp.read().decode('utf-8'))

r1 = post({'question': 'My project topic is anomaly detection in KPI telemetry.'})
sid = r1['session_id']
r2 = post({'question': 'What is my project topic?', 'session_id': sid})
print('session', sid)
print('q1', bool(r1.get('answer')))
print('q2', r2.get('answer', '')[:220])
