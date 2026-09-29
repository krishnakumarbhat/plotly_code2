import urllib.request, urllib.parse, json, sys

def openalex(query, perpage=7):
    url = f"https://api.openalex.org/works?filter=publication_year:2022-2026,type:article&search={urllib.parse.quote(query)}&per-page={perpage}"
    req = urllib.request.Request(url, headers={"User-Agent":"Mozilla/5.0","Accept":"application/json"})
    with urllib.request.urlopen(req, timeout=20) as r:
        return json.load(r)

queries = {
    "GAP-A-tracker": "tracker feedback CFAR adaptive reference window clutter map automotive radar",
    "GAP-B-studentt": "Student-t filter radar tracking non-Gaussian clutter entropy covariance",
    "GAP-C-accel": "acceleration-aware data association PDAF adaptive gate IMM maneuvering automotive radar",
    "GAP-A-trackthresh": "CFAR detector threshold driven by tracker state radar"
}
for idx, (tag, q) in enumerate(queries.items()):
    if idx>0:
        import time; time.sleep(3)
    try:
        data = openalex(q, 5)
    except Exception as e:
        print(f"ERR {tag}: {e}"); continue
    print(f"\n===== {tag} =====")
    for r in data.get("results", [])[:7]:
        doi = r.get("doi") or ""
        url = r.get("open_access",{}).get("oa_url") or (f"https://doi.org/{doi}" if doi else "")
        title = r.get("display_name","")[:220]
        yr = r.get("publication_year","")
        print(f"{yr} | DOI={doi or 'NONE'} | URL={url[:130]}")
        print(f"   T: {title}")
