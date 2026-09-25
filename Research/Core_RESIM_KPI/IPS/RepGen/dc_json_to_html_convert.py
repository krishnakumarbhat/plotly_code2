"""
File Name: json_to_html_convert.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This module converts DC json to html
"""

from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore

from IPS.DashManager.report_dash import ReportDash
import numpy as np
import plotly.io as pio
from html import escape
import json
import os, string
from collections import defaultdict


class DCJsonToHtmlConvertor:
    output_scan_index_scaled = np.array([])
    input_scan_index_scaled = np.array([])
    list_json_path = []
    sensor_list = []
    stream_list = []
    stream_signals = defaultdict(set)
    sensor_stream = defaultdict(set)
    mismatch_input_value = np.array([])
    mismatch_output_value = np.array([])
    mismatch_input_scan_index = np.array([])
    mismatch_output_scan_index = np.array([])
    _ff_subtypes_cache = {}  

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore,
                 jsonpath_datastore: IDataStore,
                 report_dash: ReportDash,
                 ):
        self._data_event_mediator_obj = event_mediator
        self._radar_datastore_obj = radar_datastore
        self._plot_datastore_obj = plot_datastore
        self._jsonpath_datastore_obj = jsonpath_datastore
        self._report_dash = report_dash


    def clear_data(self):
        pass

    def is_substring_match_in_longstring(self, substring, long_string_list):
        return any(substring.casefold() in long.casefold() for long in long_string_list)

    def filter_strings_with_substring(self, substring, long_string_list):
        return [s for s in long_string_list if substring.casefold() in s.casefold()]

    def _clean_signal_name(self, name):
        """Remove 'scatter' and 'histogram' tokens from display names.

        Works for both prefix/suffix forms like '..._scatter_x', 'histogram_...'
        and standalone tokens anywhere in the filename stem.
        """
        try:
            stem = os.path.splitext(os.path.basename(name or ''))[0]
            tokens = [t for t in stem.split('_') if t]
            tokens = [t for t in tokens if t.lower() not in ('scatter', 'histogram')]
            return '_'.join(tokens) if tokens else stem
        except Exception:
            return name

    def _sort_paths_by_signal_order(self, paths):
        """Sort paths so related signals are adjacent (e.g., pos_x/pos_y, vel_x/vel_y)."""
        # Define signal ordering priority - related signals should be adjacent
        signal_order = [
            'range', 'ran',
            'range_rate', 'rangerate',
            'azimuth', 'phi',
            'elevation', 'theta',
            'pos_x', 'xpos', 'xposn', 'posx', 'x_pos',
            'pos_y', 'ypos', 'yposn', 'posy', 'y_pos',
            # Velocity signals (x then y)
            'vel_x', 'xvel', 'velx', 'x_vel',
            'vel_y', 'yvel', 'vely', 'y_vel',
            # Acceleration signals (x then y)
            'accel_x', 'xaccel', 'accelx', 'x_acc',
            'accel_y', 'yaccel', 'accely', 'y_acc',
            # Generic x then y pattern
            '_x', 'x_',
            '_y', 'y_',
        ]
        
        # Signals that need exact match (no substring matching)
        exact_match_signals = ['azimuth']

        def get_sort_key(path):
            name = os.path.basename(path).lower()
            signal_name = name.replace('.json', '').split('_')[-1]
            
            for i, sig in enumerate(signal_order):
                if sig in exact_match_signals:
                    if signal_name == sig:
                        return (i, name)
                elif sig in name:
                    return (i, name)
            return (len(signal_order), name)  # Unknown signals at end

        return sorted(paths, key=get_sort_key)

    def _get_ff_subtypes_from_metadata(self, ff_name: str):
        """Infer valid subtypes for a FeatureFunction from poi metadata.

        Example (LCDA): signal names include bsw_alert_left/cvw_ttc_s_right/... -> subtypes={bsw,cvw,slc}
        For FFs without subtype prefixes (e.g., CED), this returns an empty set.
        """

        ff_name = (ff_name or '').strip()
        if not ff_name:
            return set()

        # Check cache first
        if ff_name in DCJsonToHtmlConvertor._ff_subtypes_cache:
            return DCJsonToHtmlConvertor._ff_subtypes_cache[ff_name]

        try:
            from IPS.Metadata.GEN7V2 import poi

            key = f'05_FeatureFunctions/{ff_name}'
            signals = getattr(poi, 'poi_data_DC', {}).get(key, [])
            ff_lower = ff_name.lower()
            subtypes = set()

            def _accept(candidate: str) -> bool:
                # Keep LCDA-like short type codes (bsw/cvw/slc). Reject noisy words like object/most/lateral.
                if not candidate or candidate == ff_lower:
                    return False
                if not candidate.isalpha():
                    return False
                return 2 <= len(candidate) <= 4

            for item in signals:
                name = (item or {}).get('name', '')
                if not isinstance(name, str) or not name:
                    continue
                tokens = [t for t in name.lower().split('_') if t]
                if not tokens:
                    continue

                candidate = None
                if 'alert' in tokens:
                    idx = tokens.index('alert')
                    if idx > 0:
                        candidate = tokens[idx - 1]
                elif 'ttc' in tokens:
                    idx = tokens.index('ttc')
                    if idx > 0:
                        candidate = tokens[idx - 1]

                if candidate and _accept(candidate):
                    subtypes.add(candidate)

            # Cache result before returning
            DCJsonToHtmlConvertor._ff_subtypes_cache[ff_name] = subtypes
            return subtypes
        except Exception:
            # Cache empty set on error too
            DCJsonToHtmlConvertor._ff_subtypes_cache[ff_name] = set()
            return set()

    def _infer_sig_type_side_from_filename(self, file_path: str, *, ff_name: str | None = None):
        """Infer (sig_type, sig_side) from filename tokens.

        This aims to preserve the previous grouping behavior where only known subtypes mattered.
        We avoid hardcoding by deriving subtype vocabulary from FeatureFunctions metadata when available.
        """

        base = os.path.splitext(os.path.basename(file_path or ''))[0].lower()
        tokens = [t for t in base.split('_') if t]

        side = ''
        for token in tokens:
            if token in ('left', 'right'):
                side = token
                break

        subtype_vocab = self._get_ff_subtypes_from_metadata(ff_name or '') if ff_name else set()

        sig_type = None
        if subtype_vocab:
            # Pick the first subtype token encountered to mimic prior regex matching.
            for token in tokens:
                if token in subtype_vocab:
                    sig_type = token
                    break

        # Fallback: if we couldn't infer type, match prior default behavior used by lineplot rendering.
        if not sig_type:
            sig_type = 'other'

        return sig_type, side


    

    def _io_legend_and_color(self, trace_name: str):
        trace_name = (trace_name or '').strip()
        lower_name = trace_name.lower()
        if 'input' in lower_name:
            return 'Input', 'blue'
        if 'output' in lower_name:
            return 'Output', 'red'
        return 'Other', 'magenta'

    def _apply_input_output_styling(self, fig, *, prefer_overlay: bool, opacity: float):
        """Normalize Input/Output styling for any figure read from JSON."""

        has_input = False
        has_output = False

        for trace in getattr(fig, 'data', []) or []:
            legend_name, color = self._io_legend_and_color(getattr(trace, 'name', ''))
            if legend_name == 'Other':
                continue
            if legend_name == 'Input':
                has_input = True
            if legend_name == 'Output':
                has_output = True

            trace.update(name=legend_name, showlegend=True)
            try:
                trace.update(opacity=opacity)
            except Exception:
                pass
            try:
                trace.marker = trace.marker or {}
                trace.marker['color'] = color
                trace.marker['opacity'] = opacity
            except Exception:
                pass
            try:
                trace.line = trace.line or {}
                trace.line['color'] = color
            except Exception:
                pass

        if prefer_overlay and has_input and has_output:
            fig.update_layout(barmode='overlay')

    def filter_json_by_word_in_name(self, substring, json_list):
        lowered_substring = substring.casefold()
        result = []
        for json_path in json_list:
            filename = os.path.splitext(os.path.basename(json_path))[0]
            words = filename.casefold().split('_')
            if lowered_substring in words:
                result.append(json_path)
        return result

    def filter_strings_with_exact_match(self, substring, json_list):
        lowered_substring = substring.casefold()
        result = []
        for path in json_list:
            # Extract the file name without extension
            file_name = os.path.splitext(os.path.basename(path))[0]
            # Split into words, strip punctuation
            words = [word.strip(string.punctuation) for word in file_name.casefold().split()]
            if lowered_substring in words:
                result.append(path)
        return result

    def consume_event(self, sensor, event):

        if event == "DC_JSON_GEN_DONE":
            self.trigger_json_to_html_conversion()

    def trigger_json_to_html_conversion(self):
        ff_streams = ['CED', 'CTA', 'ESA', 'LCDA', 'LTB', 'RECW', 'SCW', 'TA']
        for sensor in set(DCJsonToHtmlConvertor.sensor_list):
            json_list_scatter = self._jsonpath_datastore_obj.get_data(sensor, "scatter")
            json_list_hist = self._jsonpath_datastore_obj.get_data(sensor, "Histogram")
            for stream in set(DCJsonToHtmlConvertor.sensor_stream.get(sensor, set())):
                stream = stream.replace("_", "")
                if stream == "Detections" or stream == "Detections1" or stream == "Detections2":
                    filtered_list = [f for f in json_list_scatter if stream in f.split('_')]
                    json_list_filtered_detection = self.filter_json_by_word_in_name(stream, filtered_list)

                    if json_list_filtered_detection:
                        sensor = sensor.replace("_", "")
                        stream = stream.replace("_", "")
                        self.generate_plotly_html_report_new4(json_list_filtered_detection,
                                                              sensor + "_" + stream + "_scatter.html")
                    filtered_list_hist = [f for f in json_list_hist if stream in f.split('_')]
                    json_list_filtered_detection_hist = self.filter_json_by_word_in_name(stream, filtered_list_hist)
                    if json_list_filtered_detection_hist:
                        sensor = sensor.replace("_", "")
                        stream = stream.replace("_", "")
                        self.generate_plotly_html_report_new4(json_list_filtered_detection_hist,
                                                              sensor + "_" + stream + "_histogram.html")
                # FeatureFunctions
                elif stream.startswith("FeatureFunctions") or any(stream.upper().startswith(s.upper()) for s in ff_streams):
                    if stream.startswith("FeatureFunctions"):
                        ff_name = stream.replace("FeatureFunctions_", "").replace("FeatureFunctions", "")
                    else:
                        ff_name = next((s for s in ff_streams if stream.upper().startswith(s.upper())), stream)

                    # Use word boundary matching to avoid TA matching CTA
                    ff_variant_files = sorted([f for f in json_list_scatter if f"_{ff_name.upper()}_" in f"_{os.path.basename(f).upper()}_"])
                    signal_groups = {}
                    for f in ff_variant_files:
                        sig_type, sig_side = self._infer_sig_type_side_from_filename(f, ff_name=ff_name)
                        sig_key = f"{sig_type}_{sig_side}" if sig_side else sig_type
                        if sig_key not in signal_groups:
                            signal_groups[sig_key] = {'alert': [], 'ttc': []}
                        base = os.path.basename(f).lower()
                        if 'alert' in base:
                            signal_groups[sig_key]['alert'].append(f)
                        elif 'ttc' in base:
                            signal_groups[sig_key]['ttc'].append(f)
                    # Sort group keys and files alphabetically (ascending order)
                    json_list_lineplot = []
                    for sig in sorted(signal_groups.keys()):
                        plots = signal_groups[sig]
                        # Sort files within each group consistently
                        json_list_lineplot.extend(sorted(plots['alert']))
                        json_list_lineplot.extend(sorted(plots['ttc']))

                    # Only generate FF page if there is at least one warning in alert signals
                    def _ff_has_warning(groups: dict) -> bool:
                        for g in groups.values():
                            alert_paths = g.get('alert') or []
                            for p in alert_paths:
                                try:
                                    fig = pio.read_json(p)
                                except Exception:
                                    continue
                                for tr in getattr(fig, 'data', []) or []:
                                    y = getattr(tr, 'y', None)
                                    if y is None:
                                        continue
                                    try:
                                        # Treat any value > 0 as a warning (alerts are typically 0/1)
                                        if any((float(v) if v is not None else 0) > 0 for v in y):
                                            return True
                                    except Exception:
                                        # Fallback: stringy values
                                        try:
                                            if any(str(v).strip() in ("1", "true", "True", "WARNING", "Warning") for v in y):
                                                return True
                                        except Exception:
                                            pass
                        return False

                    if json_list_lineplot and _ff_has_warning(signal_groups):
                        outname = f"{ff_name}_lineplot.html"
                        self.generate_plotly_html_report_new4(json_list_lineplot, outname, ff_name, signal_groups)
                
                else:
                    json_list_filtered = self.filter_json_by_word_in_name(stream, json_list_scatter)
                    sensor = "DC"
                    stream = stream.replace("_", "")
                    self.generate_plotly_html_report_new4(json_list_filtered,
                                                          sensor + "_" + stream + "_scatter.html")
                    json_list_filtered_hist = self.filter_json_by_word_in_name(stream, json_list_hist)
                    self.generate_plotly_html_report_new4(json_list_filtered_hist,
                                                          sensor + "_" + stream + "_histogram.html")

    def generate_plotly_html_report_new4(self, json_paths, output_html_path, ff_name=None, signal_groups=None):
        # print("generate_plotly_html_report_new4")
        html_parts = [
            "<html><head><meta charset='UTF-8'>",
            "<title>Plotly Report</title>",
            "<script src='https://cdn.plot.ly/plotly-latest.min.js'></script>",
            "<style>",
            "body { font-family: sans-serif; background: #f9f9f9; padding: 20px; margin: 0; }",
            ".grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; }",
            ".card { background: white; padding: 10px; border-radius: 6px;",
            "box-shadow: 0 2px 4px rgba(0,0,0,0.1); overflow-x: auto; min-width: 0; }",
            ".title { font-weight: bold; margin-bottom: 8px; font-size: 15px; text-align: center; }",
            "@media (max-width: 1200px) { .grid { grid-template-columns: 1fr; } }",
            "</style></head><body>",
            "<h1>Plotly Interactive Chart Report</h1>",
            "<div class='grid'>"
        ]

        # Determine if this is an alert histogram or lineplot tab by output_html_path
        is_alert_histogram_tab = output_html_path.endswith("_Alert_histogram.html")
        is_lineplot_tab = output_html_path.endswith("_lineplot.html")
        import plotly.graph_objs as go

        if is_alert_histogram_tab:
            for path in json_paths:
                if not os.path.isfile(path):
                    html_parts.append(
                        f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>File not found</p></div>")
                    continue
                try:
                    fig = pio.read_json(path)
                    # For alert hist: force overlay look when both exist.
                    self._apply_input_output_styling(fig, prefer_overlay=True, opacity=0.55)
                    legend_added = set()
                    for trace in fig.data:
                        trace_name = getattr(trace, 'name', os.path.splitext(os.path.basename(path))[0])
                        legend_name, color = self._io_legend_and_color(trace_name)
                        # Only add legend once per type
                        show_legend = legend_name not in legend_added
                        legend_added.add(legend_name)
                        # Set color and legend
                        trace.marker = trace.marker or {}
                        trace.marker['color'] = color
                        trace.opacity = 0.55
                        trace.name = legend_name
                        trace.showlegend = show_legend
                        # For bar plots, set width and white border for clarity
                        if hasattr(trace, 'width'):
                            trace.width = 0.5
                        trace.marker['line'] = dict(width=2, color='white')
                        # Keep underlying data as-is; we'll map ticks to user-friendly labels.
                    fig.update_layout(
                        xaxis=dict(
                            title='Alert',
                            tickmode='array',
                            tickvals=[0, 1],
                            ticktext=['No Warning', 'Warning'],
                        ),
                        yaxis=dict(title='Count'),
                        barmode='overlay',
                        bargap=0.4,
                        autosize=True,
                        margin=dict(l=50, r=50, t=50, b=80),
                        legend=dict(orientation='h', y=-0.25),
                        height=500
                    )
                    fig_html = pio.to_html(
                        fig,
                        include_plotlyjs=False,
                        full_html=False,
                        config={"responsive": True},
                        default_width="100%",
                        default_height="500px"
                    )
                    title = os.path.splitext(os.path.basename(path))[0]
                    html_parts.append(f"<div class='card'><div class='title'>{title}</div>{fig_html}</div>")
                except Exception as e:
                    html_parts.append(
                        f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>Error: {e}</p></div>")
        elif is_lineplot_tab:
            # signal_groups must be provided for lineplot rendering
            assert signal_groups, "signal_groups required for lineplot rendering"
            # For each (type, side) group, create a subplot (rowwise: alert, then ttc)
            # Ensure deterministic group ordering: alphabetical (case-insensitive) by key
            for sig in sorted(signal_groups.keys(), key=lambda s: s.lower()):
                plots = signal_groups[sig]
                import plotly.graph_objs as go
                from plotly.subplots import make_subplots
                nrows = 0
                row_titles = []
                figs = []
                if 'alert' in plots and plots['alert']:
                    nrows += 1
                    row_titles.append('Alert')
                    alert_paths = plots['alert'] if isinstance(plots['alert'], list) else [plots['alert']]
                    alert_paths = sorted(alert_paths)
                    figs.append(('alert', alert_paths))
                if 'ttc' in plots and plots['ttc']:
                    nrows += 1
                    row_titles.append('TTC')
                    ttc_paths = plots['ttc'] if isinstance(plots['ttc'], list) else [plots['ttc']]
                    ttc_paths = sorted(ttc_paths)
                    figs.append(('ttc', ttc_paths))
                if nrows == 0:
                    continue
    
                sig_parts = sig.split('_')
                sig_type = sig_parts[0] if len(sig_parts) > 0 else ''
                sig_side = sig_parts[1] if len(sig_parts) > 1 else ''
                if sig_type == 'other' and ff_name:
                    display_ff = self._report_dash.display_ff_name(ff_name)
                    card_title = display_ff
                    if sig_side:
                        card_title += f" {sig_side.capitalize()}"
                    card_title += " Plot"
                else:
                    # Read subtype title from JSON (flat key like "LCDA.bsw"); no hardcoded mapping
                    subtype_title = None
                    if ff_name and sig_type:
                        try:
                            cfg_path = os.path.join(os.path.dirname(__file__), '..', 'Metadata', 'FFDisplayNames.json')
                            cfg_path = os.path.abspath(cfg_path)
                            if os.path.isfile(cfg_path):
                                with open(cfg_path, 'r', encoding='utf-8') as f:
                                    raw = json.load(f) or {}
                                key = f"{ff_name}.{sig_type.lower()}"
                                val = raw.get(key)
                                if isinstance(val, str) and val.strip():
                                    subtype_title = val.strip()
                        except Exception:
                            pass
                    if not subtype_title:
                        subtype_title = sig.replace('_', ' ').upper()
                    card_title = subtype_title
                    if sig_side:
                        card_title += f" {sig_side.capitalize()}"
                    card_title += " Plot"
                subfig = make_subplots(rows=nrows, cols=1, shared_xaxes=True, vertical_spacing=0.08, subplot_titles=row_titles)
                # Use a unified color for both alert and ttc traces
                legend_added = set()
                for idx, (typ, path_list) in enumerate(figs):
                    for path in path_list:
                        try:
                            fig = pio.read_json(path)
                            for trace in fig.data:
                                y = getattr(trace, 'y', None)
                                x = getattr(trace, 'x', None)
                                if y is not None:
                                    if x is not None and hasattr(x, '__len__') and len(x) == len(y):
                                        x_axis = x
                                    else:
                                        x_axis = list(range(len(y)))
                                    trace_name = getattr(trace, 'name', os.path.splitext(os.path.basename(path))[0])
                                    if 'scatter' in trace_name.lower():
                                        trace_name = trace_name.replace('scatter', 'Line Plot').replace('Scatter', 'Line Plot')
                                    legend_name, color = self._io_legend_and_color(trace_name)
                                    # Only add legend once per type
                                    show_legend = legend_name not in legend_added
                                    legend_added.add(legend_name)
                                    subfig.add_trace(
                                        go.Scatter(
                                            x=x_axis,
                                            y=y,
                                            mode='lines',
                                            name=legend_name,
                                            legendgroup=legend_name,
                                            showlegend=show_legend,
                                            opacity=0.85,
                                            line=dict(color=color, width=3),
                                        ),
                                        row=idx + 1,
                                        col=1,
                                    )
                        except Exception as e:
                            subfig.add_annotation(text=f"Error loading {typ}: {e}", xref="paper", yref="paper", showarrow=False, y=1-0.1*idx)
                subfig.update_xaxes(title_text='Scan Index', row=nrows, col=1)
                subfig.update_layout(
                    title="Lineplot",
                    autosize=True,
                    margin=dict(l=50, r=50, t=50, b=80),
                    legend=dict(orientation='h', y=-0.25),
                    height=500
                )
                subfig_html = pio.to_html(
                    subfig,
                    include_plotlyjs=False,
                    full_html=False,
                    config={"responsive": True},
                    default_width="100%",
                    default_height="500px"
                )
                html_parts.append(f"<div class='card'><div class='title'>{card_title}</div>{subfig_html}</div>")
        else:
            # Sort paths so related signals (x/y pairs) are adjacent
            sorted_paths = self._sort_paths_by_signal_order(json_paths)
            for path in sorted_paths:
                if not os.path.isfile(path):
                    html_parts.append(
                        f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>File not found</p></div>")
                    continue
                try:
                    fig = pio.read_json(path)
                    raw_title = os.path.splitext(os.path.basename(path))[0]
                    title = self._clean_signal_name(raw_title)

                    fig.update_layout(
                        autosize=True,
                        margin=dict(l=50, r=50, t=50, b=80),
                        legend=dict(orientation='h', y=-0.25),
                        height=500
                    )
                    fig_html = pio.to_html(
                        fig,
                        include_plotlyjs=False,
                        full_html=False,
                        config={"responsive": True},
                        default_width="100%",
                        default_height="500px"
                    )
                    html_parts.append(f"<div class='card'><div class='title'>{title}</div>{fig_html}</div>")
                except Exception as e:
                    html_parts.append(
                        f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>Error: {e}</p></div>")

        html_parts.append("</div></body></html>")

        report_folder = ReportDash.report_directory + "/reports"
        os.makedirs(report_folder, exist_ok=True)
        output_html_path = os.path.join(report_folder, output_html_path)
        with open(output_html_path, 'w') as f:
            f.write('\n'.join(html_parts))

        print(f"[OK] Final Plotly HTML report created: {output_html_path}")

    def generate_plotly_html_report_new5(self, json_paths, output_html_path):
        html_parts = [
            "<html><head><meta charset='UTF-8'>",
            "<title>Plotly Report</title>",
            "<script src='https://cdn.plot.ly/plotly-latest.min.js'></script>",
            "<style>",
            "body { font-family: sans-serif; background: #f9f9f9; padding: 20px; margin: 0; }",
            ".grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; }",
            ".card { background: white; padding: 10px; border-radius: 6px;",
            "box-shadow: 0 2px 4px rgba(0,0,0,0.1); overflow-x: auto; min-width: 0; }",
            ".title { font-weight: bold; margin-bottom: 8px; font-size: 15px; text-align: center; }",
            "@media (max-width: 1200px) { .grid { grid-template-columns: 1fr; } }",
            "</style></head><body>",
            "<h1>Plotly Interactive Chart Report</h1>",
            "<div class='grid'>"
        ]

        for path in json_paths:
            if not os.path.isfile(path):
                html_parts.append(
                    f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>File not found</p></div>")
                continue

            try:
                fig = pio.read_json(path)

                # Move legend below plot to avoid side squeeze
                fig.update_layout(
                    autosize=True,
                    margin=dict(l=50, r=50, t=50, b=80),
                    legend=dict(orientation='h', y=-0.25),
                    height=500
                )

                fig_html = pio.to_html(
                    fig,
                    include_plotlyjs=False,
                    full_html=False,
                    config={"responsive": True},
                    default_width="100%",
                    default_height="500px"
                )

                title = os.path.basename(path)
                html_parts.append(f"<div class='card'><div class='title'>{title}</div>{fig_html}</div>")
            except Exception as e:
                html_parts.append(
                    f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>Error: {e}</p></div>")

        html_parts.append("</div></body></html>")

        report_folder = ReportDash.report_directory + "/reports"
        os.makedirs(report_folder, exist_ok=True)
        output_html_path = os.path.join(report_folder, output_html_path)
        with open(output_html_path, 'w') as f:
            f.write('\n'.join(html_parts))

        # print(f"✅ Final Plotly HTML report created: {output_html_path}")
