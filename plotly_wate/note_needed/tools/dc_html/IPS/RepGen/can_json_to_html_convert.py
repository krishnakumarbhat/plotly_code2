"""
Description:
This module converts CAN json to html
"""
import os
import plotly.io as pio
from IPS.DashManager.report_dash import ReportDash
from IPS.DataStore.idatastore import IDataStore
from IPS.EventMan.ievent_mediator import IEventMediator

class CANJsonToHtmlConvertor:
    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore, json_ds: IDataStore, report_dash: ReportDash):
        self._mediator, self._radar_ds, self._plot_ds, self._json_ds, self._report_dash = mediator, radar_ds, plot_ds, json_ds, report_dash

    def clear_data(self): pass
    def _match(self, sub, lst): return any(sub.casefold() in s.casefold() for s in lst)
    def _filter(self, sub, lst): return [s for s in lst if sub.casefold() in s.casefold()]
    
    def _clean_signal_name(self, name):
        """Remove 'scatter' and 'histogram' from signal names for cleaner display."""
        # Remove common suffixes from the signal name
        cleaned = name.replace('_scatter', '').replace('_histogram', '')
        cleaned = cleaned.replace('scatter_', '').replace('histogram_', '')
        return cleaned

    def _sort_paths_by_signal_order(self, paths):
        """Sort paths so related signals are adjacent (e.g., pos_x/pos_y, vel_x/vel_y)."""
        # Define signal ordering priority - related signals should be adjacent
        signal_order = [
            # Range-related
            'ran', 'range', 'det_range',
            'vel', 'range_rate', 'range_velocity', 'det_range_velocity',
            # Angular
            'phi', 'azimuth', 'det_azimuth',
            'theta', 'elevation', 'det_elevation',
            # Other
            'snr', 'det_snr', 'rcs', 'det_rcs',
        ]
        
        def get_sort_key(path):
            name = os.path.basename(path).lower()
            for i, sig in enumerate(signal_order):
                if sig in name:
                    return (i, name)
            return (len(signal_order), name)  # Unknown signals at end
        
        return sorted(paths, key=get_sort_key)

    def consume_event(self, sensor, event):
        if event != "CAN_JSON_GEN_DONE": return
        self._plot_ds.clear_data()
        self._convert()

    def _convert(self):
        for s in ["FR", "FL", "RR", "RL", "FLR"]:
            scatter = self._json_ds.get_data(s, "scatter")
            f = self._filter("DETECTION", scatter)
            if self._match("DETECTION", f): self._gen_html(f, f"{s}_detection_scatter.html")

    def _gen_html(self, paths, out):
        # Sort paths so related signals are adjacent
        sorted_paths = self._sort_paths_by_signal_order(list(set(paths)))
        folder = os.path.join(self._report_dash.report_directory, "reports")
        os.makedirs(folder, exist_ok=True)
        html = ['<html><head><meta charset="UTF-8"><title>Report</title><script src="https://cdn.plot.ly/plotly-latest.min.js"></script><style>body{font-family:sans-serif;background:#f9f9f9;padding:20px}.grid{display:grid;grid-template-columns:repeat(2,1fr);gap:20px}.card{background:white;padding:10px;border-radius:6px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}.title{font-weight:bold;margin-bottom:8px;text-align:center}@media(max-width:1200px){.grid{grid-template-columns:1fr}}</style></head><body><h1>Report</h1><div class="grid">']
        for p in sorted_paths:
            if not os.path.isfile(p): continue
            try:
                fig = pio.read_json(p)
                if not fig.data: continue
                fig.update_layout(autosize=True, margin=dict(l=50, r=50, t=50, b=80), legend=dict(orientation='h', y=-0.25), height=500)
                fig_html = pio.to_html(fig, include_plotlyjs=False, full_html=False, config={"responsive": True}, default_width="100%", default_height="500px")
                # Clean the signal name by removing 'scatter' and 'histogram' suffixes
                clean_name = self._clean_signal_name(os.path.splitext(os.path.basename(p))[0])
                html.append(f'<div class="card"><div class="title">{clean_name}</div>{fig_html}</div>')
            except Exception as e:
                print(f"[Error] {p}: {e}")
        html.append('</div></body></html>')
        with open(os.path.join(folder, out), 'w', encoding='utf-8') as f: f.write('\n'.join(html))
        print(f"Report: {out}")
