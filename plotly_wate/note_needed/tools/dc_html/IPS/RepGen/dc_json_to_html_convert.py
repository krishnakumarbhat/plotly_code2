"""
Description:
This module converts DC json to html
"""
import os
import math
import json
import numpy as np
import plotly.io as pio
import plotly.graph_objects as go
from plotly.subplots import make_subplots
from collections import defaultdict
from IPS.DashManager.report_dash import ReportDash
from IPS.DataStore.idatastore import IDataStore
from IPS.EventMan.ievent_mediator import IEventMediator

class DCJsonToHtmlConvertor:
    sensor_list, stream_list = [], []
    stream_signals = defaultdict(set)
    sensor_stream = defaultdict(set)

    # User request (Jan-2026): exclude these streams from HTML generation.
    # Keep only 04OLP, ROT, VSE, and AllObjects in DC Reports section.
    # Also exclude LCDA from DC reports as requested.
    EXCLUDED_STREAMS = {"04OLP1", "04OLP2", "VSE1", "00metadata", "ROT1", "ROT2", "LCDA", "Lcda1", "Lcda"}

    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore, json_ds: IDataStore, report_dash: ReportDash):
        self._mediator, self._radar_ds, self._plot_ds, self._json_ds, self._report_dash = mediator, radar_ds, plot_ds, json_ds, report_dash

    def clear_data(self): pass
    def _filter_word(self, sub, lst): return [p for p in lst if sub.casefold() in os.path.splitext(os.path.basename(p))[0].casefold().split('_')]
    
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

    def _is_figure_empty(self, fig) -> bool:
        """Return True if figure has no plottable data points."""
        def is_valid_value(v) -> bool:
            if v is None:
                return False
            if isinstance(v, float) and math.isnan(v):
                return False
            return True

        def any_valid_in_seq(seq) -> bool:
            try:
                for item in seq:
                    if isinstance(item, (list, tuple)):
                        if any_valid_in_seq(item):
                            return True
                    elif is_valid_value(item):
                        return True
            except TypeError:
                return is_valid_value(seq)
            return False

        try:
            traces = list(getattr(fig, "data", []) or [])
        except Exception:
            traces = []
        if not traces:
            return True

        for t in traces:
            # Common Plotly trace fields carrying data
            for attr in ("x", "y", "z", "values"):
                if hasattr(t, attr):
                    v = getattr(t, attr)
                    if v is None:
                        continue
                    try:
                        if len(v) == 0:
                            continue
                    except TypeError:
                        # Scalar value
                        if is_valid_value(v):
                            return False
                        continue
                    if any_valid_in_seq(v):
                        return False
        return True

    def _is_plotly_json_dict_empty(self, fig_dict: dict) -> bool:
        """Return True if a plotly JSON dict has no plottable data points."""
        def is_valid_value(v) -> bool:
            if v is None:
                return False
            if isinstance(v, float) and math.isnan(v):
                return False
            return True

        def any_valid_in_seq(seq) -> bool:
            try:
                for item in seq:
                    if isinstance(item, list):
                        if any_valid_in_seq(item):
                            return True
                    elif is_valid_value(item):
                        return True
            except TypeError:
                return is_valid_value(seq)
            return False

        data = fig_dict.get("data") or []
        if not isinstance(data, list) or len(data) == 0:
            return True

        def has_any_valid_list(v) -> bool:
            return isinstance(v, list) and len(v) > 0 and any_valid_in_seq(v)

        for t in data:
            if not isinstance(t, dict):
                continue

            trace_type = str(t.get("type") or "scatter").lower()

            # For scatter-like traces, x without y (or y without x) draws an empty plot.
            if trace_type in ("scatter", "scattergl"):
                if has_any_valid_list(t.get("x")) and has_any_valid_list(t.get("y")):
                    return False
                continue

            # For 3D scatter, need x, y, and z.
            if trace_type == "scatter3d":
                if has_any_valid_list(t.get("x")) and has_any_valid_list(t.get("y")) and has_any_valid_list(t.get("z")):
                    return False
                continue

            # Histogram is driven by x (or y for horizontal), but in our data it's x.
            if trace_type == "histogram":
                if has_any_valid_list(t.get("x")) or has_any_valid_list(t.get("y")):
                    return False
                continue

            # Default: consider any data-bearing field
            for attr in ("x", "y", "z", "values"):
                if attr in t:
                    v = t.get(attr)
                    if v is None:
                        continue
                    if isinstance(v, list):
                        if len(v) == 0:
                            continue
                        if any_valid_in_seq(v):
                            return False
                    else:
                        if is_valid_value(v):
                            return False
        return True

    def consume_event(self, sensor, event):
        if event != "DC_JSON_GEN_DONE":
            return
        self._convert()
        self._gen_ff_lineplots()  # Generate FF lineplot reports

    def _safe_get_radar_data(self, sensor, signal, source):
        """Safely get data from radar datastore, returns None if key doesn't exist."""
        try:
            return self._radar_ds.get_data(sensor, signal, source)
        except (KeyError, Exception):
            return None

    def _gen_ff_lineplots(self):
        """Generate Feature Function lineplot reports for ESA, LCDA, SCW."""
        # Define FF lineplot configurations - each signal gets its own row
        # Format: {'signal_label': {'left': signal_name, 'right': signal_name}}
        ff_lineplot_config = {
            'ESA': {
                'title': 'Emergency Steering Assist',
                'signals': {
                    'ESA Alert': {'left': 'f_esa_alert_left', 'right': 'f_esa_alert_right'},
                    'ESA TTC': {'left': 'esa_object_ttc_s_left', 'right': 'esa_object_ttc_s_right'}
                }
            },
            'LCDA': {
                'title': 'Lane Change Decision Aid',
                'signals': {
                    'BSW Alert': {'left': 'bsw_alert_left', 'right': 'bsw_alert_right'},
                    'CVW Alert': {'left': 'cvw_alert_left', 'right': 'cvw_alert_right'},
                    'CVW TTC': {'left': 'cvw_ttc_s_left', 'right': 'cvw_ttc_s_right'},
                    'SLC Alert': {'left': 'slc_alert_left', 'right': 'slc_alert_right'}
                }
            },
            'SCW': {
                'title': 'Side Collision Warning',
                'signals': {
                    'SCW Alert': {'left': 'scw_object_alert_level_left', 'right': 'scw_object_alert_level_right'},
                    'SCW TTC': {'left': 'scw_object_lateral_ttc_s_left', 'right': 'scw_object_lateral_ttc_s_right'}
                }
            }
        }
        
        for ff_name, config in ff_lineplot_config.items():
            try:
                self._gen_multi_signal_ff_lineplot(ff_name, config)
            except Exception as e:
                print(f"Warning: Could not generate {ff_name} lineplot: {e}")

    def _gen_multi_signal_ff_lineplot(self, ff_name, config):
        """Generate a multi-signal FF lineplot HTML file with left and right columns.
        
        Each signal gets its own row, with left and right columns.
        """
        title = config['title']
        signals = config['signals']  # Dict: {'Signal Label': {'left': name, 'right': name}}
        
        num_signals = len(signals)
        if num_signals == 0:
            print(f"Warning: No signals defined for {ff_name} lineplot")
            return
        
        # Create subplot titles - each row has a left and right plot
        subplot_titles = []
        for signal_label in signals.keys():
            subplot_titles.extend([f'{signal_label} (Left)', f'{signal_label} (Right)'])
        
        # Create figure with subplots - 2 columns, N rows (one per signal type)
        fig = make_subplots(
            rows=num_signals, cols=2,
            subplot_titles=subplot_titles,
            vertical_spacing=0.08,
            horizontal_spacing=0.1
        )
        
        has_any_trace = False
        row_idx = 1
        
        for signal_label, signal_config in signals.items():
            left_sig = signal_config.get('left')
            right_sig = signal_config.get('right')
            
            # Get left signal data
            if left_sig:
                left_in = self._safe_get_radar_data('DC', left_sig, 'in')
                left_out = self._safe_get_radar_data('DC', left_sig, 'out')
                
                if left_in is not None:
                    y_data = left_in.ravel() if hasattr(left_in, 'ravel') else np.array(left_in).ravel()
                    scan_idx = list(range(len(y_data)))  # Use list, not numpy array
                    y_list = y_data.tolist()  # Convert to list for proper JSON serialization
                    show_legend = row_idx == 1  # Only show legend for first row
                    fig.add_trace(go.Scatter(x=scan_idx, y=y_list, mode='lines',
                                            name='Input', line=dict(color='blue', width=2),
                                            showlegend=show_legend), row=row_idx, col=1)
                    has_any_trace = True
                    
                if left_out is not None:
                    y_data = left_out.ravel() if hasattr(left_out, 'ravel') else np.array(left_out).ravel()
                    scan_idx = list(range(len(y_data)))
                    y_list = y_data.tolist()
                    show_legend = row_idx == 1
                    fig.add_trace(go.Scatter(x=scan_idx, y=y_list, mode='lines',
                                            name='Output', line=dict(color='red', width=2),
                                            showlegend=show_legend), row=row_idx, col=1)
                    has_any_trace = True
            
            # Get right signal data
            if right_sig:
                right_in = self._safe_get_radar_data('DC', right_sig, 'in')
                right_out = self._safe_get_radar_data('DC', right_sig, 'out')
                
                if right_in is not None:
                    y_data = right_in.ravel() if hasattr(right_in, 'ravel') else np.array(right_in).ravel()
                    scan_idx = list(range(len(y_data)))
                    y_list = y_data.tolist()
                    fig.add_trace(go.Scatter(x=scan_idx, y=y_list, mode='lines',
                                            name='Input', line=dict(color='blue', width=2),
                                            showlegend=False), row=row_idx, col=2)
                    has_any_trace = True
                    
                if right_out is not None:
                    y_data = right_out.ravel() if hasattr(right_out, 'ravel') else np.array(right_out).ravel()
                    scan_idx = list(range(len(y_data)))
                    y_list = y_data.tolist()
                    fig.add_trace(go.Scatter(x=scan_idx, y=y_list, mode='lines',
                                            name='Output', line=dict(color='red', width=2),
                                            showlegend=False), row=row_idx, col=2)
                    has_any_trace = True
            
            row_idx += 1
        
        if not has_any_trace:
            print(f"Warning: No valid trace data for {ff_name} lineplot")
            return
        
        # Calculate height based on number of signals (350px per row, min 600px)
        height = max(600, 350 * num_signals)
        
        # Update layout
        fig.update_layout(
            title=dict(text=f'<b>{title} - Lineplot Report</b>', font=dict(size=20)),
            height=height,
            showlegend=True,
            legend=dict(orientation='h', yanchor='bottom', y=-0.05, xanchor='center', x=0.5),
            plot_bgcolor='rgba(230, 236, 245, 1)',
            paper_bgcolor='white'
        )
        
        # Update x-axis labels only on bottom row
        fig.update_xaxes(title_text='Scan Index', row=num_signals, col=1)
        fig.update_xaxes(title_text='Scan Index', row=num_signals, col=2)
        
        # Save the file using write_html which embeds full Plotly.js (more reliable)
        folder = os.path.join(ReportDash.report_directory, "reports")
        os.makedirs(folder, exist_ok=True)
        out_path = os.path.join(folder, f"{ff_name}_lineplot.html")
        fig.write_html(out_path, include_plotlyjs=True, full_html=True)
        print(f"Report: {ff_name}_lineplot.html")

    def _convert(self):
        # FF stream names for special handling
        ff_names = ['CED', 'CTA', 'ESA', 'LCDA', 'LTB', 'RECW', 'SCW', 'TA']
        for sensor in set(DCJsonToHtmlConvertor.sensor_list):
            scatter = self._json_ds.get_data(sensor, "scatter")
            sensor_out = sensor.replace('_', '')
            # NOTE: FR = Front Right sensor, FF = Feature Function (not a sensor)
            hist = self._json_ds.get_data(sensor, "Histogram")
            for stream in set(DCJsonToHtmlConvertor.sensor_stream.get(sensor, set())):
                stream_clean = stream.replace("_", "")
                # Explicitly skip any LCDA-related streams (case-insensitive)
                # This ensures variants like 'Lcda1', 'LCDA1', or 'lcda' are not shown in DC Reports.
                if 'lcda' in stream_clean.lower():
                    continue
                if stream_clean in DCJsonToHtmlConvertor.EXCLUDED_STREAMS:
                    continue
                
                # Check if this is a Feature Function stream (e.g., 05_FeatureFunctions/CED)
                is_ff_stream = False
                ff_name = None
                for ff in ff_names:
                    if stream == ff:  # Exact match to avoid 'CTA' matching 'TA'
                        is_ff_stream = True
                        ff_name = ff
                        break
                
                if is_ff_stream and ff_name:
                    # Feature Function scatter reports are intentionally disabled per request.
                    # Keep only the FF lineplots generated by `_gen_ff_lineplots`.
                    # If later needed, uncomment the code below to re-enable FF scatter HTML generation.
                    # f_sc = self._filter_word(ff_name, scatter)
                    # if f_sc:
                    #     self._gen_html(f_sc, f"FF_{ff_name}_scatter.html")
                    pass
                elif stream_clean in ["Detections", "Detections1", "Detections2"]:
                    f_sc = self._filter_word(stream_clean, [f for f in scatter if stream_clean in f.split('_')])
                    if f_sc:
                        self._gen_html(f_sc, f"{sensor_out}_{stream_clean}_scatter.html")
                    f_h = self._filter_word(stream_clean, [f for f in hist if stream_clean in f.split('_')])
                    if f_h:
                        self._gen_html(f_h, f"{sensor_out}_{stream_clean}_histogram.html")
                else:
                    f_sc = self._filter_word(stream_clean, scatter)
                    if f_sc:
                        self._gen_html(f_sc, f"DC_{stream_clean}_scatter.html")
                    f_h = self._filter_word(stream_clean, hist)
                    if f_h:
                        self._gen_html(f_h, f"DC_{stream_clean}_histogram.html")

    def _gen_html(self, paths, out):
        # Sort paths so related signals are adjacent (XPos/YPos, XVel/YVel, etc.)
        sorted_paths = self._sort_paths_by_signal_order(list(paths))
        html = ['<html><head><meta charset="UTF-8"><title>Report</title><script src="https://cdn.plot.ly/plotly-latest.min.js"></script><style>body{font-family:sans-serif;background:#f9f9f9;padding:20px}.grid{display:grid;grid-template-columns:repeat(2,1fr);gap:20px}.card{background:white;padding:10px;border-radius:6px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}.title{font-weight:bold;margin-bottom:8px;text-align:center}@media(max-width:1200px){.grid{grid-template-columns:1fr}}</style></head><body><h1>Report</h1><div class="grid">']
        rendered_cards = 0
        for p in sorted_paths:
            if not os.path.isfile(p):
                continue
            try:
                with open(p, "r", encoding="utf-8") as fp:
                    fig_dict = json.load(fp)
                if not isinstance(fig_dict, dict) or self._is_plotly_json_dict_empty(fig_dict):
                    continue

                # Apply a consistent layout without constructing a full plotly Figure (faster).
                layout = fig_dict.setdefault("layout", {}) if isinstance(fig_dict, dict) else {}
                if isinstance(layout, dict):
                    layout["autosize"] = True
                    layout["margin"] = {"l": 50, "r": 50, "t": 50, "b": 80}
                    layout["legend"] = {"orientation": "h", "y": -0.25}
                    layout["height"] = 500

                fig_html = pio.to_html(fig_dict, include_plotlyjs=False, full_html=False, config={"responsive": True}, default_width="100%", default_height="500px")
                # Clean the signal name by removing 'scatter' and 'histogram' suffixes
                clean_name = self._clean_signal_name(os.path.splitext(os.path.basename(p))[0])
                html.append(f'<div class="card"><div class="title">{clean_name}</div>{fig_html}</div>')
                rendered_cards += 1
            except Exception as e:
                html.append(f'<div class="card"><div class="title">{p}</div><p style="color:red;">Error: {e}</p></div>')
                rendered_cards += 1
        html.append('</div></body></html>')
        folder = os.path.join(ReportDash.report_directory, "reports")
        os.makedirs(folder, exist_ok=True)
        out_path = os.path.join(folder, out)
        if rendered_cards == 0:
            # If everything is empty, don't generate the section at all.
            # Also delete an older file if it exists.
            try:
                if os.path.isfile(out_path):
                    os.remove(out_path)
            except Exception:
                pass
            return
        with open(out_path, 'w', encoding='utf-8') as f:
            f.write('\n'.join(html))
        print(f"Report: {out}")
