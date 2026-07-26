import os, json, time, html, queue, threading, shutil
import zmq, plotly.io as pio
from collections import defaultdict
from IPS.DashManager.report_dash import ReportDash

msg_queue, stop_event = queue.Queue(), threading.Event()

class HTMLNIPStoInteractiveTool:
    def __init__(self, port):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PULL)
        self.socket.connect(f"tcp://localhost:{port}")
        self.port = port

    @staticmethod
    def fix_booleans(obj):
        if isinstance(obj, dict):
            for k, v in obj.items():
                if isinstance(v, str) and v.lower() in ["true", "false"]: obj[k] = v.lower() == "true"
                else: HTMLNIPStoInteractiveTool.fix_booleans(v)
        elif isinstance(obj, list):
            for item in obj: HTMLNIPStoInteractiveTool.fix_booleans(item)

    def _gen_report(self, json_root, radar_map, prefix, exclude_mismatch=True):
        report_dir = os.path.join(json_root, "reports")
        os.makedirs(report_dir, exist_ok=True)
        for radar, type_map in radar_map.items():
            for jtype, paths in (type_map.items() if isinstance(type_map, dict) else [("", type_map)]):
                html_parts = [f'<html><head><meta charset="UTF-8"><title>{radar}_{jtype} {prefix}</title><script src="https://cdn.plot.ly/plotly-latest.min.js"></script><style>body{{font-family:sans-serif;background:#f9f9f9;padding:20px}}.grid{{display:grid;grid-template-columns:repeat(2,1fr);gap:20px}}.card{{background:white;padding:10px;border-radius:6px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}}.title{{font-weight:bold;margin-bottom:8px;text-align:center}}@media(max-width:1200px){{.grid{{grid-template-columns:1fr}}}}</style></head><body><h1>{radar} {jtype.upper() if jtype else ""} {prefix}</h1><div class="grid">']
                for p in paths:
                    try:
                        with open(p, 'r', encoding='utf-8') as f: raw = json.load(f)
                        self.fix_booleans(raw)
                        fig = pio.from_json(json.dumps(raw))
                        fig.update_layout(margin=dict(l=50, r=50, t=50, b=80), legend=dict(orientation='h', y=-0.25), height=500)
                        fig_html = pio.to_html(fig, include_plotlyjs=False, full_html=False, config={"responsive": True}, default_width="100%", default_height="500px")
                        html_parts.append(f'<div class="card"><div class="title">{os.path.splitext(os.path.basename(p))[0]}</div>{fig_html}</div>')
                    except Exception as e:
                        html_parts.append(f'<div class="card"><div class="title">{html.escape(p)}</div><p style="color:red;">Error: {html.escape(str(e))}</p></div>')
                html_parts.append('</div></body></html>')
                out = os.path.join(report_dir, f"{radar}_{jtype}_{prefix.lower()}.html" if jtype else f"{radar}_{prefix.lower()}.html")
                with open(out, 'w', encoding='utf-8') as f: f.write('\n'.join(html_parts))
                print(f"Report: {out}")

    def _collect_json(self, root, pattern, exclude=None):
        radar_map = defaultdict(lambda: defaultdict(list))
        for r, _, files in os.walk(root):
            for f in files:
                if f.endswith(".json") and pattern in f.lower() and (exclude is None or exclude not in f.lower()):
                    radar, path = os.path.basename(r), os.path.join(r, f)
                    for t in ["detection", "toi", "vse", "header"]:
                        if t in f.lower(): radar_map[radar][t].append(path); break
        return radar_map

    def generate_scatter_json_reports(self, root): self._gen_report(root, self._collect_json(root, "scatter", "mismatchscatter"), "Scatter")
    def generate_histogram_json_reports(self, root): self._gen_report(root, self._collect_json(root, "histogram"), "Histogram")

    def generate_mismatchscatter_json_reports(self, root):
        radar_map = defaultdict(list)
        for r, _, files in os.walk(root):
            for f in files:
                if f.endswith(".json") and "mismatchscatter" in f.lower():
                    radar_map[os.path.basename(r)].append(os.path.join(r, f))
        self._gen_report(root, {k: {"mismatch": v} for k, v in radar_map.items()}, "Mismatch")

    def map_match_mismatch_json(self, root):
        for sensor in os.listdir(root):
            path = os.path.join(root, sensor, "match_mismatch.json")
            if os.path.isdir(os.path.join(root, sensor)) and os.path.exists(path):
                try:
                    with open(path, 'r') as f: data = json.load(f)
                    for s, sigs in data.items():
                        for sig, (m, mm) in sigs.items():
                            ReportDash.update_signal_stats(ReportDash, s, sig, m, mm)
                            ReportDash.signal_name_set.add(sig)
                except Exception as e: print(f"Error: {path}: {e}")

    def clean_json_folder(self, path):
        for item in os.listdir(path):
            p = os.path.join(path, item)
            if item.lower() == "reports": continue
            try:
                if os.path.isdir(p): shutil.rmtree(p)
                else: os.remove(p)
            except: pass

    def worker(self):
        while not stop_event.is_set():
            try:
                task = msg_queue.get(timeout=1)
                if task.get("command") == "stop_service": stop_event.set(); break
                msg = self.socket.recv_string()
                try:
                    self.generate_scatter_json_reports(msg)
                    self.generate_histogram_json_reports(msg)
                    self.generate_mismatchscatter_json_reports(msg)
                    self.map_match_mismatch_json(msg)
                    self.clean_json_folder(msg)
                    self.socket.send_string("Done")
                except Exception as e: self.socket.send_string(f"Error: {e}")
            except queue.Empty: continue

    def NIPS_communication(self):
        self.socket.bind(f"tcp://*:{self.port}")
        while not stop_event.is_set():
            try:
                msg = self.socket.recv_json(flags=zmq.NOBLOCK)
                msg_queue.put(msg)
            except zmq.Again: time.sleep(0.1)
