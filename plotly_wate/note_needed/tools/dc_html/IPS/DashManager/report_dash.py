import os, time, json, stat, shutil
from pathlib import Path
from collections import defaultdict
from IPS.EventMan.ievent_mediator import IEventMediator

class ReportDash:
    report_directory, input_hdf, output_hdf, report_gen_time = None, None, None, None
    Version_metadata, signal_name_set, datasource_type = [], set(), None
    signal_stats = defaultdict(lambda: defaultdict(lambda: {"match": 0, "mismatch": 0}))

    # NOTE:
    # User request (Jan-2026): When generating HTML, do not show/generate these groups.
    # Keep the code as-is, but exclude them from the dashboard.
    EXCLUDED_DASH_STREAMS = {"00metadata", "04OLP1", "VSE1"}

    def __init__(self, mediator: IEventMediator):
        self._mediator = mediator
        ReportDash.signal_stats = defaultdict(lambda: defaultdict(lambda: {"match": 0, "mismatch": 0}))

    def update_signal_stats(self, sensor, signal, match=0, mismatch=0):
        ReportDash.signal_stats[sensor][signal]["match"] += match
        ReportDash.signal_stats[sensor][signal]["mismatch"] += mismatch

    def display_ff_name(self, ff_abbr: str) -> str:
        """Map FF abbreviation to display name using JSON; fallback to abbreviation.

        Uses IPS/Metadata/FFDisplayNames.json. Only base FF keys (without subtype suffix)
        are considered here.
        """
        key = (ff_abbr or '').strip().upper()
        if not key:
            return ''
        try:
            cfg_path = os.path.join(os.path.dirname(__file__), '..', 'Metadata', 'FFDisplayNames.json')
            cfg_path = os.path.abspath(cfg_path)
            if os.path.isfile(cfg_path):
                with open(cfg_path, 'r', encoding='utf-8') as f:
                    data = json.load(f) or {}
                    base_map = {str(k).upper(): str(v) for k, v in data.items() if '.' not in str(k)}
                    return base_map.get(key, key)
        except Exception:
            pass
        return key

    def clear_data(self):
        ReportDash.report_directory = ReportDash.input_hdf = ReportDash.output_hdf = None

    def _safe_delete(self, path):
        path = os.path.abspath(path)
        if os.path.islink(path): os.unlink(path)
        elif os.path.isdir(path):
            for r, ds, fs in os.walk(path):
                for d in ds: os.chmod(os.path.join(r, d), stat.S_IWUSR | stat.S_IREAD)
                for f in fs: os.chmod(os.path.join(r, f), stat.S_IWUSR | stat.S_IREAD)

    def generate_rep(self):
        time.sleep(10)
        self._safe_delete(os.path.join(ReportDash.report_directory, "JSON"))
        sigs = list(ReportDash.signal_name_set) + ["range", "range_rate", "azimuth", "elevation", "snr"]
        self._gen_dashboard(ReportDash.report_directory + "/reports", ReportDash.signal_stats, ReportDash.input_hdf, ReportDash.output_hdf, ReportDash.report_gen_time, sigs, ReportDash.report_directory + "/KPI_DashBoard.html")

    def consume_event(self, sensor, event):
        if event != "HTML_GEN_DONE": return
        self._gen_dashboard(ReportDash.report_directory + "/reports", ReportDash.signal_stats, ReportDash.input_hdf, ReportDash.output_hdf, ReportDash.report_gen_time, [], ReportDash.report_directory + "/KPI_DashBoard.html")

    def _gen_dashboard(self, report_dir, stats, in_hdf, out_hdf, rep_time, sigs, out_html):
        meta = ReportDash.Version_metadata if ReportDash.datasource_type == "udpdc" else ["NA"]*6
        hdf_time, dc_ver, ocg_ver, olp_ver, sfl_ver, trk_ver = (meta + ["NA"]*6)[:6]
        main_keys = ['range', 'range_rate', 'azimuth', 'elevation', 'snr', 'std_rcs'] if ReportDash.datasource_type == "udpdc" else ['ran', 'vel', 'phi', 'theta', 'snr', 'rcs']
        extra_keys = ['vcs_pos_x', 'vcs_pos_y', 'vcs_vel_x', 'vcs_vel_y', 'vcs_accel_x', 'vcs_accel_y'] if ReportDash.datasource_type == "udpdc" else []
        report_path = Path(report_dir)
        if not report_path.exists(): return
        files = [f.name for f in report_path.glob("*.html") if f.is_file()]
        dashboard = {}
        ff_dashboard = {}  # Feature Function reports
        ff_names = ['CED', 'CTA', 'ESA', 'LCDA', 'LTB', 'RECW', 'SCW', 'TA']
        for f in files:
            parts = f.split("_")
            # Check if this is a Feature Function file (e.g., FF_CED_lineplot.html, DC_CED_scatter.html)
            ff_variant = None
            for part in parts:
                for ff in ff_names:
                    if part.upper().startswith(ff):
                        ff_variant = part
                        break
                if ff_variant:
                    break
            # Include both lineplot and scatter files for FF reports
            if ff_variant and ('lineplot' in f.lower() or 'scatter' in f.lower()):
                ff_dashboard.setdefault(ff_variant, []).append(f)
                continue
            if len(parts) >= 3:
                # Example filenames: DC_04OLP_histogram.html, DC_04OLP1_scatter.html, DC_00metadata_scatter.html
                if parts[1].casefold() in {s.casefold() for s in ReportDash.EXCLUDED_DASH_STREAMS}:
                    continue
                sensor_key = parts[0]
                # NOTE: FR = Front Right sensor, FF = Feature Function (not a sensor)
                dashboard.setdefault(sensor_key, {}).setdefault(parts[1], []).append(f)

        # NOTE: original dashboard header had a "Metadata" accordion.
        # User request (Jan-2026): no need metadata section.
        # html = f'''<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Dashboard</title><link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet"><script src="https://cdn.jsdelivr.net/npm/chart.js"></script><style>body{{padding:2rem;background:#f9f9f9}}.chart-box{{width:100%;max-width:250px;height:200px}}</style></head><body><div class="container"><h2 class="text-center mb-4">Sensor Report Dashboard</h2><div class="accordion mb-4" id="hA"><div class="accordion-item"><h2 class="accordion-header"><button class="accordion-button collapsed" type="button" data-bs-toggle="collapse" data-bs-target="#cM">Metadata</button></h2><div id="cM" class="accordion-collapse collapse"><div class="accordion-body"><p><b>Tool:</b> V2.0</p><p><b>Input:</b> {in_hdf}</p><p><b>Output:</b> {out_hdf}</p><p><b>DC:</b> {dc_ver}</p><p><b>OLP:</b> {olp_ver}</p><p><b>OCG:</b> {ocg_ver}</p><p><b>SFL:</b> {sfl_ver}</p><p><b>Tracker:</b> {trk_ver}</p><p><b>HDF Time:</b> {hdf_time}</p><p><b>Report Time:</b> {rep_time}s</p></div></div></div></div>'''

        # NOTE: Original header included Chart.js + chart-box CSS for side plots.
        # Keeping the original code commented as requested.
        # html = f'''<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Dashboard</title><link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet"><script src="https://cdn.jsdelivr.net/npm/chart.js"></script><style>body{{padding:2rem;background:#f9f9f9}}.chart-box{{width:100%;max-width:250px;height:200px}}</style></head><body><div class="container"><h2 class="text-center mb-4">Sensor Report Dashboard</h2>'''

        html = f'''<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Dashboard</title><link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet"><style>body{{padding:2rem;background:#f9f9f9}}</style></head><body><div class="container-fluid px-4"><h2 class="text-center mb-4">📊 Sensor Report Dashboard</h2><div class="accordion mb-4" id="hA"><div class="accordion-item"><h2 class="accordion-header"><button class="accordion-button collapsed" type="button" data-bs-toggle="collapse" data-bs-target="#cM">Metadata</button></h2><div id="cM" class="accordion-collapse collapse"><div class="accordion-body"><p><b>Tool:</b> V3.0</p><p><b>Input:</b> {in_hdf}</p><p><b>Output:</b> {out_hdf}</p><p><b>DC:</b> {dc_ver}</p><p><b>OLP:</b> {olp_ver}</p><p><b>OCG:</b> {ocg_ver}</p><p><b>SFL:</b> {sfl_ver}</p><p><b>Tracker:</b> {trk_ver}</p><p><b>HDF Time:</b> {hdf_time}</p><p><b>Report Time:</b> {rep_time}s</p></div></div></div></div>'''
        for i, (sensor, streams) in enumerate(dashboard.items()):
            html += f'<div class="accordion mb-3" id="sA-{i}"><div class="accordion-item"><h2 class="accordion-header"><button class="accordion-button collapsed" type="button" data-bs-toggle="collapse" data-bs-target="#s-{i}">{sensor} Reports</button></h2><div id="s-{i}" class="accordion-collapse collapse"><div class="accordion-body">'
            for stream, fs in streams.items():
                html += f"<h6>{stream.capitalize()}</h6>"
                for f in fs:
                    rel = os.path.relpath(report_path / f, Path(out_html).parent)
                    display_name = f
                    html += f"<a href='{rel}' target='_blank' class='btn btn-outline-primary btn-sm m-1'>{display_name}</a>"
            # NOTE: Original layout included two side plots (canvas) per sensor.
            # Keeping it commented out as requested.
            # html += f'</div><div class="col-md-3 d-flex justify-content-center"><div class="chart-box"><canvas id="c-{sensor}"></canvas></div></div><div class="col-md-3 d-flex justify-content-center"><div class="chart-box"><canvas id="c2-{sensor}"></canvas></div></div></div></div></div></div>'
            html += f'</div></div></div></div>'

        # Add Feature Function Reports section if there are FF reports
        if ff_dashboard:
            html += f'<div class="accordion mb-3" id="ffA"><div class="accordion-item"><h2 class="accordion-header"><button class="accordion-button collapsed" type="button" data-bs-toggle="collapse" data-bs-target="#ff-reports">FF Reports</button></h2><div id="ff-reports" class="accordion-collapse collapse"><div class="accordion-body">'
            for ff_name, ff_files in sorted(ff_dashboard.items()):
                display_ff = self.display_ff_name(ff_name)
                html += f"<h6><strong>{display_ff}</strong></h6><div class='mb-3'>"
                for f in sorted(ff_files):
                    rel = os.path.relpath(report_path / f, Path(out_html).parent)
                    html += f"<a href='{rel}' target='_blank' class='btn btn-outline-success btn-sm m-1'>{f}</a>"
                html += "</div>"
            html += '</div></div></div></div>'

        # NOTE: Original dashboard appended Chart.js script to render the side plots.
        # Keeping it commented out as requested.
        # html += f'''</div><script>const S={json.dumps(stats)},M={json.dumps(main_keys)},E={json.dumps(extra_keys)};function C(c,d,l1,l2){{const L=Object.keys(d),m=L.map(k=>d[k].match),x=L.map(k=>d[k].mismatch);new Chart(c,{{type:'bar',data:{{labels:L,datasets:[{{label:l1,data:m,backgroundColor:'rgba(40,167,69,0.7)'}},{{label:l2,data:x,backgroundColor:'rgba(220,53,69,0.7)'}}]}},options:{{responsive:true,maintainAspectRatio:false,scales:{{y:{{beginAtZero:true,max:100}}}}}}}})}}document.addEventListener('DOMContentLoaded',()=>{{Object.entries(S).forEach(([s,d])=>{{const mD=Object.fromEntries(Object.entries(d).filter(([k])=>M.includes(k))),eD=Object.fromEntries(Object.entries(d).filter(([k])=>E.includes(k)));C(document.getElementById(`c-${{s}}`).getContext('2d'),mD,'Match %','Mismatch %');if(Object.keys(eD).length>0)C(document.getElementById(`c2-${{s}}`).getContext('2d'),eD,'Match %','Mismatch %')}})}})</script><script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script></body></html>'''

        html += f'''</div><script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script></body></html>'''
        with open(out_html, "w", encoding="utf-8") as f: f.write(html)
        print(f"Dashboard: {out_html}")

    def generate_dashboard_from_path(self, report_dir, signal_stats, input_hdf, output_hdf, rep_time,
                                     signames,

                                     output_html="dash.html",
                                     ):
        start_time = time.time()
        print("rep_time", rep_time)

        if ReportDash.datasource_type == "udpdc":
            print("version_info", ReportDash.Version_metadata)
            hdf_generate_time = ReportDash.Version_metadata[0]
            dc_version = ReportDash.Version_metadata[1]
            ocg_version = ReportDash.Version_metadata[2]
            olp_version = ReportDash.Version_metadata[3]
            sfl_version = ReportDash.Version_metadata[4]
            tracker_version = ReportDash.Version_metadata[5]

            main_keys = ['range', 'range_rate', 'azimuth', 'elevation', 'snr', 'std_rcs']
            extra_keys = ['vcs_pos_x', 'vcs_pos_y', 'vcs_vel_x', 'vcs_vel_y', 'vcs_accel_x', 'vcs_accel_y']




        else:
            dc_version = "NA"
            ocg_version = "NA"
            olp_version = "NA"
            sfl_version = "NA"
            tracker_version = "NA"
            hdf_generate_time = "NA"
            main_keys = ['ran', 'vel', 'phi', 'theta', 'snr', 'rcs']
            extra_keys = []

        report_dir = Path(report_dir)
        if not report_dir.exists():
            raise FileNotFoundError(f"Report path not found: {report_dir}")

        report_files = [f.name for f in report_dir.glob("*.html") if f.is_file()]

        dashboard_data = {}
        ff_dashboard_data = {}  # Separate dictionary for Feature Function reports
        
        for file in report_files:
            parts = file.split("_")
            ff_names = ['CED', 'CTA', 'ESA', 'LCDA', 'LTB', 'RECW', 'SCW', 'TA']
            ff_variant = None
            for part in parts:
                for ff in ff_names:
                    if part.upper().startswith(ff):
                        ff_variant = part
                        break
                if ff_variant:
                    break
            # Include both lineplot and scatter files for FF reports
            if ff_variant and ('lineplot' in file.lower() or 'scatter' in file.lower()):
                ff_dashboard_data.setdefault(ff_variant, []).append(file)
            elif len(parts) >= 3:
                sensor = parts[0]
                stream = parts[1]
                dashboard_data.setdefault(sensor, {}).setdefault(stream, []).append(file)

        signal_stats_json = json.dumps(signal_stats)
        js_signal_array = json.dumps(signames)

        html_content = f"""
        <!DOCTYPE html>
        <html lang=\"en\">
        <head>
            <meta charset=\"UTF-8\">
            <title>Sensor Report Dashboard</title>
            <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css\" rel=\"stylesheet\">
            <script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>
            <style>
                body {{ padding: 2rem; background-color: #f9f9f9; }}
                .btn-sensor {{ margin: 0.2rem; font-size: 0.85rem; }}
                .chart-box {{ width: 100%; max-width: 250px; height: 200px; }}
            </style>
        </head>
        <body>
        <div class=\"container\">
            <h2 class=\"text-center mb-4\">📊 Sensor Report Dashboard</h2>

            <div class=\"accordion mb-4\" id=\"headerAccordion\">
                <div class=\"accordion-item\">
                    <h2 class=\"accordion-header\" id=\"headingMeta\">
                        <button class=\"accordion-button collapsed\" type=\"button\" data-bs-toggle=\"collapse\" data-bs-target=\"#collapseMeta\">
                            🔧 Dashboard Metadata
                        </button>
                    </h2>
                    <div id=\"collapseMeta\" class=\"accordion-collapse collapse\">
                        <div class=\"accordion-body\">
                            <p><strong>Tool version:</strong> {"V2.0"}</p>
                            <p><strong>Input HDF File:</strong> {input_hdf}</p>
                            <p><strong>Output HDF File:</strong> {output_hdf}</p>
                            <p><strong>DC Version:</strong> {dc_version}</p>
                            <p><strong>OLP Version:</strong> {olp_version}</p>
                            <p><strong>OCG Version:</strong> {ocg_version}</p>
                            <p><strong>SFL Version:</strong> {sfl_version}</p>
                            <p><strong>Tracker Version:</strong> {tracker_version}</p>
                            <p><strong>HDF Geneartion Date:</strong> {hdf_generate_time}</p>
                            <p><strong>Report Generation time:</strong> {str(rep_time)} seconds</p>

                        </div>
                    </div>
                </div>
            </div>
        """

        for idx, (sensor, streams) in enumerate(dashboard_data.items()):
            html_content += f"""
                <div class=\"accordion mb-3\" id=\"sensorAccordion-{idx}\">
                    <div class=\"accordion-item\">
                        <h2 class=\"accordion-header\">
                            <button class=\"accordion-button collapsed\" type=\"button\" data-bs-toggle=\"collapse\" data-bs-target=\"#sensor-{idx}\">
                                {sensor} Reports
                            </button>
                        </h2>
                        <div id=\"sensor-{idx}\" class=\"accordion-collapse collapse\">
                            <div class=\"accordion-body row\">
                                <div class=\"col-md-6\">
                """
            for stream, files in streams.items():
                html_content += f"<h6>{stream.capitalize()} Stream</h6>"
                for file in files:
                    full_file_path = report_dir / file
                    relative_path = os.path.relpath(full_file_path, Path(output_html).parent)
                    # print("relative_path", relative_path)
                    html_content += f"<a href='{relative_path}' target='_blank' class='btn btn-outline-primary btn-sensor'>{file}</a>"

            html_content += f"""
                                </div>
                                <div class=\"col-md-3 d-flex justify-content-center\">
                                    <div class=\"chart-box\">
                                        <canvas id=\"chart-{sensor}\"></canvas>
                                    </div>
                                </div>
                                <div class=\"col-md-3 d-flex justify-content-center\">
                                    <div class=\"chart-box\">
                                        <canvas id=\"chart2-{sensor}\"></canvas>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                """

        # Add FF Reports section if there are FF reports
        if ff_dashboard_data:
            ff_idx = len(dashboard_data)  
            html_content += f"""
                <div class=\"accordion mb-3\" id=\"ffAccordion\">
                    <div class=\"accordion-item\">
                        <h2 class=\"accordion-header\">
                            <button class=\"accordion-button collapsed\" type=\"button\" data-bs-toggle=\"collapse\" data-bs-target=\"#ff-reports\">
                                FF Reports
                            </button>
                        </h2>
                        <div id=\"ff-reports\" class=\"accordion-collapse collapse\">
                            <div class=\"accordion-body\">
                """
            
            # Add each Feature Function as a sub-section
            for ff_name, files in sorted(ff_dashboard_data.items()):
                display_ff = self.display_ff_name(ff_name)
                html_content += f"<h6><strong>{display_ff}</strong></h6><div class=\"mb-3\">"
                # Show FF lineplot and scatter files
                for file in sorted(files):
                    if 'lineplot' in file.lower() or 'scatter' in file.lower():
                        full_file_path = report_dir / file
                        relative_path = os.path.relpath(full_file_path, Path(output_html).parent)
                        html_content += f"<a href='{relative_path}' target='_blank' class='btn btn-outline-success btn-sensor'>{file}</a>"
                html_content += "</div>"
            
            html_content += """
                            </div>
                        </div>
                    </div>
                </div>
                """

        html_content += f"""
        </div>
        <script>
        const signalStats = {signal_stats_json};
        const signalarray= {js_signal_array};

        function createChart(ctx, data, label1, label2) {{
            const labels = Object.keys(data);
            const matchData = labels.map(k => data[k].match);
            const mismatchData = labels.map(k => data[k].mismatch);

            new Chart(ctx, {{
                type: 'bar',
                data: {{
                    labels: labels,
                    datasets: [
                        {{ label: label1, data: matchData, backgroundColor: 'rgba(40, 167, 69, 0.7)' }},
                        {{ label: label2, data: mismatchData, backgroundColor: 'rgba(220, 53, 69, 0.7)' }}
                    ]
                }},
                options: {{
                    responsive: true,
                    maintainAspectRatio: false,
                    scales: {{ y: {{ beginAtZero: true, max: 100 }} }}
                }}
            }});
        }}

        document.addEventListener('DOMContentLoaded', () => {{
            Object.entries(signalStats).forEach(([sensor, data]) => {{
                
                
                const mainKeys = {json.dumps(main_keys)};
                const extraKeys = {json.dumps(extra_keys)};

                const mainData = Object.fromEntries(Object.entries(data).filter(([k, _]) => mainKeys.includes(k)));
                const extraData = Object.fromEntries(Object.entries(data).filter(([k, _]) => extraKeys.includes(k)));

                const ctx1 = document.getElementById(`chart-${{sensor}}`).getContext('2d');
                createChart(ctx1, mainData, 'Match %', 'Mismatch %');

                if (Object.keys(extraData).length > 0) {{
                    const ctx2 = document.getElementById(`chart2-${{sensor}}`).getContext('2d');
                    createChart(ctx2, extraData, 'Match %', 'Mismatch %');
                }}
            }});
        }});
        </script>
        <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js\"></script>
        </body>
        </html>
        """

        with open(output_html, "w", encoding="utf-8") as f:
            f.write(html_content)

        print(f"✅ HTML dashboard saved as: {output_html}")
