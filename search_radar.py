import urllib.request, urllib.parse, xml.etree.ElementTree as ET, sys

def fetch_arxiv(query, max_results=12):
    url = "http://export.arxiv.org/search/query?search_query=all:%22" + urllib.parse.quote(query) + "%22&start=0&max_results=" + str(max_results) + "&sortBy=relevance&sortOrder=descending"
    req = urllib.request.Request(url, headers={"User-Agent":"Mozilla/5.0", "Accept":"application/atom+xml"})
    with urllib.request.urlopen(req, timeout=15) as r:
        return r.read()

def parse(xmlb):
    root = ET.fromstring(xmlb)
    ns = {'atom':'http://www.w3.org/2005/Atom'}
    entries = []
    for entry in root.iter('{http://www.w3.org/2005/Atom}entry'):
        title = entry.find('{http://www.w3.org/2005/Atom}title')
        id_ = entry.find('{http://www.w3.org/2005/Atom}id')
        summary = entry.find('{http://www.w3.org/2005/Atom}summary')
        entries.append({
            'title': (title.text or '').strip()[:250],
            'id': (id_.text or '').strip(),
            'summary': (summary.text or '').strip()[:350]
        })
    return entries

queries = [
    "CFAR clutter map tracker feedback adaptive windows radar",
    "Student-t filter non-Gaussian clutter tracking radar variational Bayes",
    "IMM PDAF adaptive gate maneuvering target association automotive radar"
]
for q in queries:
    xml = fetch_arxiv(q)
    entries = parse(xml)
    print("\n===== ARXIV QUERY: "+q[:80]+" =====")
    for e in entries[:6]:
        print("ID:", e['id'])
        print("T :", e['title'])
        print("S :", e['summary'][:300])
