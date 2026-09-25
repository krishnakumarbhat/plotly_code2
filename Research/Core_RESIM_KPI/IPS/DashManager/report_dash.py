import os
import json
import time
from pathlib import Path
from IPS.EventMan.ievent_mediator import IEventMediator

from collections import defaultdict

import os
import time
import json
from pathlib import Path

import shutil
import os
import os
import shutil
import stat
import platform


class ReportDash:
    '''
    signal_stats = {

        "FC": {
            "ran": {"match": 0, "mismatch": 0},
            "vel": {"match": 0, "mismatch": 0},
            "phi": {"match": 0, "mismatch": 0},
            "theta": {"match": 0, "mismatch": 0},
            "snr": {"match": 0, "mismatch": 0},
            "rcs": {"match": 0, "mismatch": 0}
        },

        "FL": {
            "ran": {"match": 0, "mismatch": 0},
            "vel": {"match": 0, "mismatch": 0},
            "phi": {"match": 0, "mismatch": 0},
            "theta": {"match": 0, "mismatch": 0},
            "snr": {"match": 0, "mismatch": 0},
            "rcs": {"match": 0, "mismatch": 0}
        }
        ,
        "FR": {
            "ran": {"match": 0, "mismatch": 0},
            "vel": {"match": 0, "mismatch": 0},
            "phi": {"match": 0, "mismatch": 0},
            "theta": {"match": 0, "mismatch": 0},
            "snr": {"match": 0, "mismatch": 0},
            "rcs": {"match": 0, "mismatch": 0}
        }
        ,
        "RL": {
            "ran": {"match": 0, "mismatch": 0},
            "vel": {"match": 0, "mismatch": 0},
            "phi": {"match": 0, "mismatch": 0},
            "theta": {"match": 0, "mismatch": 0},
            "snr": {"match": 0, "mismatch": 0},
            "rcs": {"match": 0, "mismatch": 0}
        }
        ,
        "RR": {
            "ran": {"match": 0, "mismatch": 0},
            "vel": {"match": 0, "mismatch": 0},
            "phi": {"match": 0, "mismatch": 0},
            "theta": {"match": 0, "mismatch": 0},
            "snr": {"match": 0, "mismatch": 0},
            "rcs": {"match": 0, "mismatch": 0}
        }
    }
    '''

    def create_signal_stats(self):
        return defaultdict(lambda: defaultdict(lambda: {"match": 0, "mismatch": 0}))

    def update_signal_stats(self, sensor, signal, match_count=0, mismatch_count=0):
        ReportDash.signal_stats[sensor][signal]["match"] += match_count
        ReportDash.signal_stats[sensor][signal]["mismatch"] += mismatch_count

    report_directory = None
    input_hdf = None
    output_hdf = None
    report_gen_time = None
    Version_metadata = []
    signal_stats = defaultdict(lambda: defaultdict(lambda: {"match": 0, "mismatch": 0}))

    # Control PNG export across the pipeline. Can be toggled from ResimHTMLReport
    # based on Inputs.json (key: "export_png"). Default True to preserve current behavior.
    export_png = True

    signal_name_set = set()
    datasource_type = None

    def __init__(self, event_mediator: IEventMediator):
        # print("ReportDash")
        self._data_event_mediator_obj = event_mediator
        ReportDash.signal_stats = self.create_signal_stats()

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
        ReportDash.report_directory = None
        ReportDash.input_hdf = None
        ReportDash.output_hdf = None
        # ReportDash.report_gen_time = None

    def safe_delete_folder(self, folder_path):
        # Normalize the path
        folder_path = os.path.abspath(folder_path)
        print(f"[CHECK] Checking folder: {folder_path}")

        if os.path.exists(folder_path):
            if os.path.islink(folder_path):
                print(f"[WARN] '{folder_path}' is a symbolic link. Unlinking it.")
                try:
                    os.unlink(folder_path)
                    print(f"[OK] Symbolic link '{folder_path}' has been removed.")
                except Exception as e:
                    print(f"[ERROR] Failed to remove symbolic link: {e}")
            elif os.path.isdir(folder_path):
                print(f"[DIR] '{folder_path}' is a directory. Preparing to delete.")
                try:
                    # Make all contents writable (especially for Linux/Docker)
                    for root, dirs, files in os.walk(folder_path):
                        for d in dirs:
                            os.chmod(os.path.join(root, d), stat.S_IWUSR | stat.S_IREAD)
                        for f in files:
                            os.chmod(os.path.join(root, f), stat.S_IWUSR | stat.S_IREAD)

                    # shutil.rmtree(folder_path)
                    print(f"[OK] Folder '{folder_path}' and all its contents have been deleted.")
                except PermissionError:
                    print(f"[ERROR] Permission denied while deleting '{folder_path}'. Try running with elevated privileges.")
                except Exception as e:
                    print(f"[ERROR] An error occurred while deleting the folder: {e}")
            else:
                print(f"[WARN] '{folder_path}' exists but is not a directory or symlink.")
        else:
            print(f"[INFO] The folder '{folder_path}' does not exist.")

    def generate_rep(self):
        time.sleep(10)
        self.safe_delete_folder(os.path.join(ReportDash.report_directory, "JSON"))
        '''
        folder_path = os.path.join(ReportDash.report_directory, "JSON")

        # Normalize the path for cross-platform compatibility
        folder_path = os.path.abspath(folder_path)

        if os.path.exists(folder_path) and os.path.isdir(folder_path):
            try:
                shutil.rmtree(folder_path)
                print(f"[OK] Folder '{folder_path}' and all its contents have been deleted.")
            except PermissionError:
                print(f"[ERROR] Permission denied while deleting '{folder_path}'. Try running with elevated privileges.")
            except Exception as e:
                print(f"[ERROR] An error occurred while deleting the folder: {e}")
        else:
            print(f"[INFO] The folder '{folder_path}' does not exist.")

        # print("Generation time in dash module", ReportDash.report_gen_time)
        '''
        signal_name_list = list(ReportDash.signal_name_set)
        signal_name_list.append("range")
        signal_name_list.append("range_rate")
        signal_name_list.append("azimuth")
        signal_name_list.append("elevation")
        signal_name_list.append("snr")

        print("signal_name_list", signal_name_list)
        print("ReportDash.signal_stats", ReportDash.signal_stats)

        self.generate_dashboard_from_path(
            report_dir=ReportDash.report_directory + "/reports",
            signal_stats=ReportDash.signal_stats,
            input_hdf=ReportDash.input_hdf,
            output_hdf=ReportDash.output_hdf,
            rep_time=ReportDash.report_gen_time,
            signames=signal_name_list,
            output_html=ReportDash.report_directory + "/KPI_DashBoard.html",

        )

    def consume_event(self, sensor, event):
        # print(f"ReportDash # consume_event # event= {event} for {sensor}")
        if event == "HTML_GEN_DONE":
            # print("Consumed Event HTML_GEN_DONE in ReportDash")

            folder_path = os.path.join(ReportDash.report_directory, "JSON")

            # Normalize the path for cross-platform compatibility
            folder_path = os.path.abspath(folder_path)

            if os.path.exists(folder_path) and os.path.isdir(folder_path):
                try:
                    # shutil.rmtree(folder_path)
                    print(f"[OK] Folder '{folder_path}' and all its contents have been deleted.")
                except PermissionError:
                    print(f"[ERROR] Permission denied while deleting '{folder_path}'. Try running with elevated privileges.")
                except Exception as e:
                    print(f"[ERROR] An error occurred while deleting the folder: {e}")
            else:
                print(f"[INFO] The folder '{folder_path}' does not exist.")

            print("Generation time in dash module", ReportDash.report_gen_time)

            self.generate_dashboard_from_path(
                report_dir=ReportDash.report_directory + "/reports",
                signal_stats=ReportDash.signal_stats,
                input_hdf=ReportDash.input_hdf,
                output_hdf=ReportDash.output_hdf,
                rep_time=ReportDash.report_gen_time,

                output_html=ReportDash.report_directory + "/KPI_DashBoard.html",

            )

    def generate_dashboard_from_path(self, report_dir, signal_stats, input_hdf, output_hdf, rep_time,
                                     signames,

                                     output_html="dash.html",
                                     ):
        start_time = time.time()
        print("rep_time", rep_time)

        def decode_version(val):
            """Decode bytes to string if needed, otherwise return as-is."""
            if isinstance(val, bytes):
                return val.decode('utf-8')
            return val

        if ReportDash.datasource_type == "udpdc":
            print("version_info", ReportDash.Version_metadata)
            hdf_generate_time = decode_version(ReportDash.Version_metadata[0])
            dc_version = decode_version(ReportDash.Version_metadata[1])
            ocg_version = decode_version(ReportDash.Version_metadata[2])
            olp_version = decode_version(ReportDash.Version_metadata[3])
            sfl_version = decode_version(ReportDash.Version_metadata[4])
            tracker_version = decode_version(ReportDash.Version_metadata[5])

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
                    if part.startswith(ff):
                        ff_variant = part
                        break
                if ff_variant:
                    break
            if ff_variant and file.endswith("lineplot.html"):
                ff_dashboard_data.setdefault(ff_variant, []).append(file)
            elif len(parts) >= 3:
                sensor = parts[0]
                stream = parts[1]
                # Remap DGPS-generated plot groups to 'Detections' so they don't create a separate DGPS tab
                if isinstance(stream, str) and stream.lower().startswith('dgps'):
                    stream = 'Detections'
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
            <style>
                body {{ padding: 2rem; background-color: #f9f9f9; }}
                .btn-sensor {{ margin: 0.2rem; font-size: 0.85rem; }}
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
                            <p><strong>HDF Generation Date:</strong> {hdf_generate_time}</p>
                            <p><strong>Report Generation time:</strong> {str(rep_time)} seconds</p>

                        </div>
                    </div>
                </div>
            </div>
        """

        for idx, (sensor, streams) in enumerate(dashboard_data.items()):
            # Skip sensors that only have empty or zero-data reports
            # Detection sensors (FL, FR, RL, RR, FC) should have data, DC always shows
            if sensor in ['FL', 'FR', 'RL', 'RR', 'FC']:
                # Check if this sensor has any data by looking at signal_stats
                sensor_has_data = False
                for sig, stats in signal_stats.get(sensor, {}).items():
                    if stats.get('match', 0) > 0 or stats.get('mismatch', 0) > 0:
                        sensor_has_data = True
                        break
                if not sensor_has_data:
                    # Skip this sensor - no detection data
                    continue
            
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
                # Show FF lineplot files only
                for file in sorted(files):
                    if file.endswith('lineplot.html'):
                        full_file_path = report_dir / file
                        relative_path = os.path.relpath(full_file_path, Path(output_html).parent)
                        html_content += f"<a href='{relative_path}' target='_blank' class='btn btn-outline-primary btn-sensor'>{file}</a>"
                html_content += "</div>"
            
            html_content += """
                            </div>
                        </div>
                    </div>
                </div>
                """

        html_content += f"""
        </div>
        <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js\"></script>
        </body>
        </html>
        """

        with open(output_html, "w", encoding="utf-8") as f:
            f.write(html_content)

        print(f"[OK] HTML dashboard saved as: {output_html}")
