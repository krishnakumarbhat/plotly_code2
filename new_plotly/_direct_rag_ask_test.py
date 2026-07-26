import json, time, urllib.request

url = "http://usmidet-com-prod-com047.aptiv.com:5100/ask"
payload = json.dumps({'question': 'Summarize in one sentence what the ingestor.py file does.', 'session_id': 'diag-test'}).encode('utf-8')
req = urllib.request.Request(url, data=payload, headers={'Content-Type': 'application/json'}, method='POST')
start = time.time()
with urllib.request.urlopen(req, timeout=200) as resp:
    body = resp.read().decode('utf-8')
elapsed = time.time() - start
print('elapsed seconds:', elapsed)
print(body[:1500])
