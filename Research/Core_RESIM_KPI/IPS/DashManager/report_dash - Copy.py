import os
import json
import time
from pathlib import Path
from IPS.EventMan.ievent_mediator import IEventMediator

import os
import time
import json
from pathlib import Path


class ReportDash:
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

    report_directory = None
    input_hdf = None
    output_hdf = None
    report_gen_time = None

    def __init__(self, event_mediator: IEventMediator):
        #print("ReportDash")
        self._data_event_mediator_obj = event_mediator

    def clear_data(self):
        ReportDash.report_directory = None
        ReportDash.input_hdf = None
        ReportDash.output_hdf = None
        ReportDash.report_gen_time = None

    def consume_event(self, sensor, event):
        #print(f"ReportDash # consume_event # event= {event} for {sensor}")
        if event == "HTML_GEN_DONE":
            #print("Consumed Event HTML_GEN_DONE in ReportDash")

            self.generate_dashboard_from_path(
                report_dir=ReportDash.report_directory+"/reports",
                # Update to your actual path
                signal_stats=ReportDash.signal_stats,
                input_hdf=ReportDash.input_hdf,
                output_hdf=ReportDash.output_hdf,
                output_html=ReportDash.report_directory+"/KPI_DashBoard.html"
            )




    def generate_dashboard_from_path(self, report_dir, signal_stats, input_hdf, output_hdf, output_html="dash.html"):
        start_time = time.time()

        report_dir = Path(report_dir)
        if not report_dir.exists():
            raise FileNotFoundError(f"Report path not found: {report_dir}")

        report_files = [f.name for f in report_dir.glob("*.html") if f.is_file()]

        dashboard_data = {}
        for file in report_files:
            parts = file.split("_")
            if len(parts) >= 3:
                sensor = parts[0]
                stream = parts[1]
                dashboard_data.setdefault(sensor, {}).setdefault(stream, []).append(file)

        signal_stats_json = json.dumps(signal_stats)

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
                        <p><strong>Input HDF File:</strong> {input_hdf}</p>
                        <p><strong>Output HDF File:</strong> {output_hdf}</p>
                        <p><strong>Dashboard Generated in:</strong> {round(time.time() - start_time, 2)} seconds</p>
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
                    #print("relative_path", relative_path)
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

        html_content += f"""
    </div>
    <script>
    const signalStats = {signal_stats_json};

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
            const mainKeys = ['ran', 'vel', 'phi', 'theta','snr','rcs'];
            const extraKeys = ['long', 'lat', 'vel', 'acc'];
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

        print(f"HTML dashboard saved as: {output_html}")
