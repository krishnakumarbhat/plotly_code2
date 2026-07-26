"""
Description:
This module converts json to html
"""
import os
import plotly.io as pio
from pathlib import Path
from IPS.DashManager.report_dash import ReportDash
from IPS.DataStore.idatastore import IDataStore
from IPS.EventMan.ievent_mediator import IEventMediator

class JsonToHtmlConvertor:
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
            # Position signals first (x then y)
            'pos_x', 'xpos', 'xposn', 'curvi_pos_x',
            'pos_y', 'ypos', 'yposn', 'curvi_pos_y',
            # Velocity signals (x then y)  
            'vel_x', 'xvel', 'curvi_vel_x',
            'vel_y', 'yvel', 'curvi_vel_y',
            # Acceleration signals (x then y)
            'accel_x', 'xaccel',
            'accel_y', 'yaccel',
            # Range-related
            'ran', 'range',
            'vel', 'range_rate', 'range_velocity',
            # Angular
            'phi', 'azimuth',
            'theta', 'elevation',
            'heading', 'curvi_heading',
            'yaw', 'yaw_rate',
            # Other
            'snr', 'rcs', 'std_rcs',
        ]
        
        def get_sort_key(path):
            name = os.path.basename(path).lower()
            for i, sig in enumerate(signal_order):
                if sig in name:
                    return (i, name)
            return (len(signal_order), name)  # Unknown signals at end
        
        return sorted(paths, key=get_sort_key)

    def consume_event(self, sensor, event):
        if event != "JSON_GEN_DONE": return
        self._plot_ds.clear_data()
        self._convert()

    def _convert(self):
        sensors = ["FC", "FL", "FR", "RR", "RL"]
        for s in sensors:
            scatter = self._json_ds.get_data(s, "scatter")
            hist = self._json_ds.get_data(s, "Histogram")
            mismatch = self._json_ds.get_data(s, "mismatch")
            addmiss = self._json_ds.get_data(s, "additionalmissing")
            for key in ["DETECTION", "VSE", "TOI", "HEADER", "DOWN_SELECTION"]:
                f_sc = self._filter(key, scatter)
                if self._match(key, f_sc): self._gen_html(f_sc, f"{s}_{key.lower()}_scatter.html")
            for key in ["DETECTION", "VSE", "DOWN_SELECTION"]:
                f_h = self._filter(key, hist)
                if self._match(key, f_h): self._gen_html(f_h, f"{s}_{key.lower()}_histogram.html")
            for key in ["DETECTION", "DOWN_SELECTION"]:
                am = set(addmiss) if addmiss else set()
                f_am = set(self._filter(key, am))
                if self._match(key, f_am): self._gen_html(f_am, f"{s}_{key.lower()}_addi_miss.html")
                # Mismatch plots commented out as they are not proper
                # mm = set(mismatch)
                # f_mm = set(self._filter(key, mm))
                # if self._match(key, f_mm): self._gen_html(f_mm, f"{s}_{key.lower()}_mismatch.html")

    def _gen_html(self, paths, out):
        # Sort paths so related signals are adjacent (XPos/YPos, XVel/YVel, etc.)
        sorted_paths = self._sort_paths_by_signal_order(list(paths))
        html = ['<html><head><meta charset="UTF-8"><title>Report</title><script src="https://cdn.plot.ly/plotly-latest.min.js"></script><style>body{font-family:sans-serif;background:#f9f9f9;padding:20px}.grid{display:grid;grid-template-columns:repeat(2,1fr);gap:20px}.card{background:white;padding:10px;border-radius:6px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}.title{font-weight:bold;margin-bottom:8px;text-align:center}@media(max-width:1200px){.grid{grid-template-columns:1fr}}</style></head><body><h1>Report</h1><div class="grid">']
        for p in sorted_paths:
            if not os.path.isfile(p): continue
            try:
                fig = pio.read_json(p)
                fig.update_layout(autosize=True, margin=dict(l=50, r=50, t=50, b=80), legend=dict(orientation='h', y=-0.25), height=500)
                fig_html = pio.to_html(fig, include_plotlyjs=False, full_html=False, config={"responsive": True}, default_width="100%", default_height="500px")
                # Clean the signal name by removing 'scatter' and 'histogram' suffixes
                clean_name = self._clean_signal_name(os.path.splitext(os.path.basename(p))[0])
                html.append(f'<div class="card"><div class="title">{clean_name}</div>{fig_html}</div>')
            except Exception as e:
                html.append(f'<div class="card"><div class="title">{p}</div><p style="color:red;">Error: {e}</p></div>')
        html.append('</div></body></html>')
        folder = os.path.join(ReportDash.report_directory, "reports")
        os.makedirs(folder, exist_ok=True)
        with open(os.path.join(folder, out), 'w') as f: f.write('\n'.join(html))
