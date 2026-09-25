import sys
import json
import pandas as pd
import numpy as np
import re
from pathlib import Path
from typing import Dict, Optional, List, Tuple


class TimingPlotter:
    """Enhanced timing plotter with improved legend placement and max time analysis"""

    # Add function names to plot
    FUNCTIONS_TO_PLOT = [
        "populate_ipc_stream_data()",
        "rsp_run()",
        "tracker_run()",
        "ff_run()",
        "transmit_udp_data()",
        "transmit_can_data()",
        "populate_radar_output_symbol()",
        "SRR7_SiL_Execute()",
    ]

    # Colors for each function
    FUNCTION_COLORS = {
        "populate_ipc_stream_data()": "#696969",
        "rsp_run()": "#FF0000",
        "tracker_run()": "#008000",
        "ff_run()": "#0000FF",
        "transmit_udp_data()": "#A0522D",
        "transmit_can_data()": "#800080",
        "populate_radar_output_symbol()": "#FFA500",
        "SRR7_SiL_Execute()": "#9900FF",
    }

    # Sensor colors for max time plot
    SENSOR_COLORS = {
        "FL": "#FF0000",
        "FR": "#0000FF",
        "RL": "#008000",
        "RR": "#FFA500",
        "FC": "#800080",
    }

    SENSOR_ABBREVS = {
        "FRONT_LEFT": "FL",
        "FRONT_RIGHT": "FR",
        "REAR_LEFT": "RL",
        "REAR_RIGHT": "RR",
        "FRONT_CENTER": "FC",
    }

    SENSORS = ["FRONT_LEFT", "FRONT_RIGHT", "REAR_LEFT", "REAR_RIGHT", "FRONT_CENTER"]

    # Time per scan index in milliseconds
    SCAN_INDEX_TIME_MS = 50.0

    def __init__(self, sr_path: str, output_path: str = None):
        """Initialize the timing plotter.

        Args:
            sr_path: Path to the statistical reports directory
            output_path: Output directory (defaults to sr_path)
        """
        self.sr_path = Path(sr_path)
        self.output_path = Path(output_path) if output_path else self.sr_path
        self.log_files = {}
        self.plot_data = {}
        self.max_time_data = {}
        self.log_name_mapping = {}  # Maps log index to full log name

        # For execution ratio calculation
        self.total_scan_indices = 0
        self.total_execution_time = 0.0

        # Validate paths
        if not self.sr_path.exists():
            raise FileNotFoundError(f"Path {sr_path} does not exist")

        self.output_path.mkdir(parents=True, exist_ok=True)

    def parse_log_file(self, file_path: Path) -> Optional[pd.DataFrame]:
        """Parse log file for function timing data.

        Args:
            file_path: Path to the log file

        Returns:
            DataFrame with timing data or None if parsing fails
        """
        data = {"SCANINDEX": [], "Function": [], "Time": []}

        try:
            with open(file_path, "r", encoding="utf-8") as f:
                for line in f:
                    line = line.strip()
                    match = re.match(
                        r"SCAN_INDEX\s*:\s*(\d+)\s*[;]?\s*MSG\s*:\s*([^\s()]+)\(\)\s*(\d+)",
                        line,
                        re.IGNORECASE,
                    )
                    if match:
                        scan_index, func_name, time = match.groups()
                        full_func_name = func_name + "()"
                        if full_func_name in self.FUNCTIONS_TO_PLOT:
                            data["SCANINDEX"].append(int(scan_index))
                            data["Function"].append(full_func_name)
                            data["Time"].append(float(time) / 1000.0)

            if not data["SCANINDEX"]:
                print(f"Warning: No valid data in {file_path}")
                return None

            return pd.DataFrame(data)

        except FileNotFoundError:
            print(f"Error: File {file_path} not found")
            return None
        except UnicodeDecodeError:
            print(f"Error: File {file_path} has unsupported encoding (expected UTF-8)")
            return None
        except Exception as e:
            print(f"Error parsing {file_path}: {e}")
            return None

    def discover_log_files(self) -> Dict[str, Dict[str, Path]]:
        """Find _STATISTICAL_REPORT.txt files, grouped by prefix.

        Returns:
            Dictionary mapping log prefixes to sensor file paths
        """
        log_files = {}

        for sensor in self.SENSORS:
            sensor_path = self.sr_path / sensor
            if not sensor_path.exists():
                continue

            for file in sensor_path.iterdir():
                if file.name.endswith("_STATISTICAL_REPORT.txt"):
                    prefix = file.name.split(f"_{sensor}_STATISTICAL_REPORT.txt")[0]
                    if prefix not in log_files:
                        log_files[prefix] = {}
                    log_files[prefix][sensor] = file

        if not log_files:
            raise ValueError("No valid data files found")

        # Create log name mapping (index -> full name)
        for i, log_name in enumerate(sorted(log_files.keys())):
            self.log_name_mapping[str(i)] = (
                log_name  # Use string keys for JSON compatibility
            )

        print(f"Found logs: \n{sorted(log_files.keys())}")
        return log_files

    def process_sensor_data(
        self, log_prefix: str, sensor: str, file_path: Path
    ) -> Optional[Dict]:
        """Process data for a single sensor.

        Args:
            log_prefix: Log file prefix
            sensor: Sensor name
            file_path: Path to sensor data file

        Returns:
            Processed sensor data dictionary or None
        """
        df = self.parse_log_file(file_path)
        if df is None:
            return None

        sensor_abbrev = self.SENSOR_ABBREVS.get(sensor, sensor)
        plot_id = f"{log_prefix}_{sensor_abbrev}"

        log_index = next(
            (i for i, name in self.log_name_mapping.items() if name == log_prefix), "-1"
        )

        # Prepare plot data for each function
        traces_data = []
        max_time_value = df["Time"].max() if not df.empty else 1
        y_scale = "log" if max_time_value > 10 else "linear"

        # Extract data for SRR7_SiL_Execute()
        sil_execute_func = "SRR7_SiL_Execute()"
        if sil_execute_func in df["Function"].values:
            sil_df = df[df["Function"] == sil_execute_func]
            if not sil_df.empty:
                # Count the number of scan indices for this sensor
                scan_index_count = len(sil_df)
                total_execution_time = sil_df["Time"].sum()
                max_scan_index = int(sil_df["SCANINDEX"].max())
                max_time = float(sil_df["Time"].max())

                if log_index not in self.max_time_data:
                    self.max_time_data[log_index] = {}

                self.max_time_data[log_index][sensor_abbrev] = {
                    "scan_index_count": scan_index_count,
                    "total_execution_time": total_execution_time,
                    "max_scan_index": max_scan_index,
                    "max_time": max_time,
                    "sensor": sensor_abbrev,
                    "log_name": log_prefix,
                }

        for func in df["Function"].unique():
            df_func = df[df["Function"] == func]
            # Convert NumPy arrays to Python lists with native types
            x_values = [int(x) for x in df_func["SCANINDEX"].tolist()]
            y_values = [float(y) for y in df_func["Time"].tolist()]

            trace_data = {
                "x": x_values,
                "y": y_values,
                "name": func,
                "mode": "markers",
                "type": "scatter",
                "marker": {
                    "size": 6,
                    "opacity": 0.8,
                    "color": self.FUNCTION_COLORS.get(func, "#7f7f7f"),
                },
                "hovertemplate": "SCANINDEX: %{x}<br>Time (ms): %{y:.3f}<br>Function: "
                + func
                + "<extra></extra>",
            }
            traces_data.append(trace_data)

        return {
            "id": plot_id,
            "title": f"{sensor_abbrev}",
            "traces": traces_data,
            "y_scale": y_scale,
            "sensor": sensor,
            "sensor_abbrev": sensor_abbrev,
            "log_prefix": log_prefix,
        }

    def calculate_execution_ratio(self) -> Tuple[float, str]:
        """Calculate the execution ratio of SRR7_SiL_Execute() function.

        For each log, find the sensor with the maximum total execution time,
        then calculate the ratio of total execution time to total log duration.

        Returns:
            Tuple containing:
            - The calculated ratio as a float
            - A formatted string describing the ratio
        """
        total_log_duration = 0.0
        total_execution_time = 0.0
        log_details = []

        # For each log, find the sensor with the maximum total execution time
        for log_index in self.max_time_data:
            # First find the sensor with maximum scan index count for log duration
            max_count = 0
            max_count_sensor = None

            # Find sensor with maximum scan index count for log duration
            for sensor, data in self.max_time_data[log_index].items():
                if data["scan_index_count"] > max_count:
                    max_count = data["scan_index_count"]
                    max_count_sensor = sensor

            # Now find the sensor with maximum total execution time
            max_execution_time = 0.0
            max_execution_sensor = None

            for sensor, data in self.max_time_data[log_index].items():
                if data["total_execution_time"] > max_execution_time:
                    max_execution_time = data["total_execution_time"]
                    max_execution_sensor = sensor

            if max_count_sensor and max_execution_sensor:
                # Add to total log duration based on max scan count
                log_duration = max_count * self.SCAN_INDEX_TIME_MS
                total_log_duration += log_duration

                # Add to total execution time based on max execution time
                sensor_execution_time = self.max_time_data[log_index][
                    max_execution_sensor
                ]["total_execution_time"]
                total_execution_time += sensor_execution_time

                # Store details for display
                log_details.append(
                    {
                        "log_index": log_index,
                        "log_name": self.log_name_mapping.get(
                            log_index, f"Log {log_index}"
                        ),
                        "max_count_sensor": max_count_sensor,
                        "max_execution_sensor": max_execution_sensor,
                        "scan_count": max_count,
                        "duration_ms": log_duration,
                        "execution_time_ms": sensor_execution_time,
                    }
                )

        if total_log_duration == 0:
            return 0.0, "No SRR7_SiL_Execute() data found"

        # Calculate ratio
        ratio = total_execution_time / total_log_duration

        # Format description
        if ratio < 1.0:
            # If less than 1, show as percentage
            description = f"Execution Ratio : {ratio:.4f}"
        else:
            # If greater than 1, show as "X times slower"
            description = f"SRR7_SiL_Execute() is {ratio:.2f} times slower"

        # Store values for display
        self.total_scan_indices = int(total_log_duration / self.SCAN_INDEX_TIME_MS)
        self.total_execution_time = total_execution_time
        self.log_details = log_details

        return ratio, description

    def generate_plot_data(self) -> Dict:
        """Generate all plot data.

        Returns:
            Dictionary containing all plot configurations
        """
        self.log_files = self.discover_log_files()
        plot_data = {}

        for log_prefix in sorted(self.log_files.keys()):
            plot_data[log_prefix] = {}
            for sensor in sorted(self.log_files[log_prefix].keys()):
                sensor_data = self.process_sensor_data(
                    log_prefix, sensor, self.log_files[log_prefix][sensor]
                )
                if sensor_data:
                    sensor_abbrev = self.SENSOR_ABBREVS.get(sensor, sensor)
                    plot_data[log_prefix][sensor_abbrev] = sensor_data

        return plot_data

    def prepare_max_time_plot_data(self) -> Tuple[List[Dict], Dict, Dict]:
        """Prepare data for the maximum execution time plot.

        Returns:
            Tuple containing:
            - List of trace data for the max time plot
            - Dictionary of annotations for the plot
            - Dictionary with execution ratio information
        """
        max_time_traces = []

        # Create a trace for each sensor
        for sensor_abbrev, color in self.SENSOR_COLORS.items():
            x_values = []
            y_values = []
            scan_indices = []
            text_values = []

            # Collect data points for this sensor across all logs
            for log_index in sorted(self.max_time_data.keys()):
                if sensor_abbrev in self.max_time_data[log_index]:
                    x_values.append(int(log_index))  # Convert to native Python int
                    y_values.append(
                        float(self.max_time_data[log_index][sensor_abbrev]["max_time"])
                    )
                    scan_indices.append(
                        int(
                            self.max_time_data[log_index][sensor_abbrev][
                                "max_scan_index"
                            ]
                        )
                    )
                    log_name = self.max_time_data[log_index][sensor_abbrev]["log_name"]
                    text_values.append(f"{log_name}_{sensor_abbrev}")

            if x_values:  # Only add trace if we have data points
                trace = {
                    "x": x_values,
                    "y": y_values,
                    "text": text_values,
                    "customdata": scan_indices,  # Store scan indices for hover
                    "name": sensor_abbrev,
                    "mode": "markers",
                    "type": "scatter",
                    "marker": {"size": 10, "opacity": 0.8, "color": color},
                    "hovertemplate": "Time (ms): %{y:.3f}<br>SCANINDEX: %{customdata}<br>Log: %{text}<extra></extra>",
                }
                max_time_traces.append(trace)

        # Create annotations for x-axis ticks - convert keys to integers for proper sorting
        tick_vals = [int(k) for k in self.log_name_mapping.keys()]
        tick_text = [str(k) for k in tick_vals]  # Use string representation for display

        annotations_data = {"xaxis": {"tickvals": tick_vals, "ticktext": tick_text}}

        # Calculate execution ratio
        ratio, ratio_description = self.calculate_execution_ratio()

        # Prepare detailed ratio data for display
        ratio_data = {
            "ratio": ratio,
            "description": ratio_description,
            "total_scan_indices": self.total_scan_indices,
            "total_execution_time": self.total_execution_time,
            "total_available_time": self.total_scan_indices * self.SCAN_INDEX_TIME_MS,
            "log_details": getattr(self, "log_details", []),
        }

        return max_time_traces, annotations_data, ratio_data

    def create_html_output(self, plot_data: Dict) -> str:
        """Create HTML output.

        Args:
            plot_data: Dictionary containing plot configurations

        Returns:
            Complete HTML string
        """
        # Generate navigation
        nav_html = self._generate_navigation(plot_data)

        # Prepare max time plot data
        max_time_traces, annotations_data, ratio_data = (
            self.prepare_max_time_plot_data()
        )

        # Custom JSON encoder to handle NumPy types
        class NumpyEncoder(json.JSONEncoder):
            def default(self, obj):
                if isinstance(obj, (np.integer, np.int64, np.int32)):
                    return int(obj)
                elif isinstance(obj, (np.floating, np.float64, np.float32)):
                    return float(obj)
                elif isinstance(obj, np.ndarray):
                    return obj.tolist()
                return super(NumpyEncoder, self).default(obj)

        # Convert data to JSON with custom encoder
        max_time_data_json = json.dumps(max_time_traces, indent=2, cls=NumpyEncoder)
        annotations_json = json.dumps(annotations_data, indent=2, cls=NumpyEncoder)
        ratio_data_json = json.dumps(ratio_data, indent=2, cls=NumpyEncoder)
        log_mapping_json = json.dumps(self.log_name_mapping, indent=2, cls=NumpyEncoder)
        plot_data_json = json.dumps(plot_data, indent=2, cls=NumpyEncoder)

        html_template = f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Profile Timing Plots</title>
    <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
    <style>
        {self._get_css_styles()}
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>Profile Timing Plots</h1>
            <div class="tab-container">
                <button id="tab-plots" class="tab-button active">Timing Plots</button>
                <button id="tab-max-time" class="tab-button">SRR7_SiL_Execute() Plot</button>
            </div>
        </header>
        
        <div class="main-content">
            <nav class="sidebar" id="sidebar">
                <div class="sidebar-header">
                    <h3>Navigation</h3>
                    <button id="toggle-sidebar" title="Toggle sidebar width">⇄</button>
                </div>
                <div class="nav-container">
                    {nav_html}
                </div>
            </nav>
            
            <main class="plot-area">
                <!-- Function Timing Plots Tab -->
                <div id="plots-tab" class="tab-content active">
                    <div id="loading-indicator" class="loading">
                        <div class="spinner"></div>
                        <p>Select a log from the navigation</p>
                    </div>
                    <div id="plot-container" class="plot-container" style="display: none;">
                        <div id="current-plot-title" class="plot-title"></div>
                        <div id="plots-wrapper"></div>
                    </div>
                </div>
                
                <!-- Max Time Plot Tab -->
                <div id="max-time-tab" class="tab-content">
                    <div class="plot-title">Maximum SRR7_SiL_Execute() Execution Time</div>
                    <div id="execution-ratio" class="execution-ratio"></div>
                    <div id="max-time-plot" class="plot" style="height: 600px;"></div>
                    <div id="ratio-details" class="ratio-details"></div>
                </div>
            </main>
        </div>
    </div>

    <script>
        {self._get_javascript_code(plot_data_json, max_time_data_json, annotations_json, log_mapping_json, ratio_data_json)}
    </script>
</body>
</html>
        """

        return html_template

    def _generate_navigation(self, plot_data: Dict) -> str:
        """Generate navigation HTML structure."""
        nav_html = ""

        for log_prefix in sorted(plot_data.keys()):
            nav_html += f"""
            <button class="log-btn" onclick="loadAllPlotsForLog('{log_prefix}')" 
                    id="nav-{log_prefix}" data-log-id="{log_prefix}">
                {log_prefix}
            </button>
            """

        return nav_html

    def _get_css_styles(self) -> str:
        """Return CSS styles for the HTML output."""
        return """
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
            background: #f5f7fa;
            min-height: 100vh;
            color: #2d3748;
            font-size: 14px;
        }

        .container {
            max-width: 1600px;
            margin: 0 auto;
            background: white;
            min-height: 100vh;
            box-shadow: 0 0 20px rgba(0,0,0,0.05);
            display: flex;
            flex-direction: column;
        }

        header {
            background: linear-gradient(135deg, #3182ce 0%, #2c5282 100%);
            color: white;
            padding: 1rem 1.5rem;
            text-align: center;
        }

        header h1 {
            font-size: 1.8rem;
            margin-bottom: 0.5rem;
            font-weight: 600;
        }

        .tab-container {
            display: flex;
            justify-content: center;
            gap: 1rem;
            margin-top: 0.5rem;
        }

        .tab-button {
            background: rgba(255, 255, 255, 0.2);
            border: none;
            border-radius: 4px;
            color: white;
            padding: 0.5rem 1rem;
            cursor: pointer;
            font-size: 0.9rem;
            transition: all 0.2s ease;
        }

        .tab-button:hover {
            background: rgba(255, 255, 255, 0.3);
        }

        .tab-button.active {
            background: rgba(255, 255, 255, 0.4);
            font-weight: 600;
        }

        .tab-content {
            display: none;
            width: 100%;
            height: 100%;
        }

        .tab-content.active {
            display: block;
        }

        .main-content {
            display: flex;
            flex: 1;
        }

        .sidebar {
            width: 300px;
            background: #f8fafc;
            border-right: 1px solid #e2e8f0;
            transition: width 0.3s ease;
            display: flex;
            flex-direction: column;
        }

        .sidebar.expanded {
            width: 450px;
        }

        .sidebar-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 1rem;
            border-bottom: 1px solid #e2e8f0;
        }

        .sidebar-header h3 {
            color: #4a5568;
            font-size: 1rem;
            font-weight: 600;
        }

        #toggle-sidebar {
            background: #edf2f7;
            border: none;
            width: 24px;
            height: 24px;
            border-radius: 4px;
            cursor: pointer;
            color: #4a5568;
            font-weight: bold;
        }

        #toggle-sidebar:hover {
            background: #e2e8f0;
        }

        .nav-container {
            overflow-y: auto;
            padding: 1rem;
            flex: 1;
        }

        .log-btn {
            width: 100%;
            padding: 0.7rem 1rem;
            background: #edf2f7;
            color: #2d3748;
            border: 1px solid #e2e8f0;
            border-radius: 4px;
            cursor: pointer;
            font-size: 0.75rem;
            font-weight: 500;
            text-align: left;
            margin-bottom: 0.5rem;
            transition: all 0.2s ease;
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
        }

        .log-btn:hover {
            background: #e2e8f0;
            transform: translateY(-1px);
        }

        .log-btn.active {
            background: #ebf4ff;
            color: #3182ce;
            border-color: #bee3f8;
            font-weight: 600;
        }

        .plot-area {
            flex: 1;
            padding: 1rem;
            background: white;
            overflow-y: auto;
        }

        .loading {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 300px;
            color: #718096;
        }

        .spinner {
            width: 30px;
            height: 30px;
            border: 3px solid #e2e8f0;
            border-top: 3px solid #3182ce;
            border-radius: 50%;
            animation: spin 1s linear infinite;
            margin-bottom: 1rem;
        }

        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }

        .plot-container {
            width: 100%;
        }

        .plot-title {
            font-size: 1.2rem;
            font-weight: 600;
            color: #2d3748;
            margin-bottom: 1rem;
            padding: 0.75rem 1rem;
            background: #f7fafc;
            border-radius: 4px;
            border-left: 3px solid #3182ce;
        }

        .plot-wrapper {
            margin-bottom: 2rem;
            border: 1px solid #e2e8f0;
            border-radius: 4px;
            overflow: hidden;
        }

        .plot-header {
            padding: 0.75rem 1rem;
            background: #f7fafc;
            border-bottom: 1px solid #e2e8f0;
            font-weight: 600;
            color: #4a5568;
            font-size: 0.9rem;
        }

        .plot {
            height: 400px;
        }

        .error-message {
            background: #fff5f5;
            color: #c53030;
            padding: 1rem;
            border-radius: 4px;
            border-left: 3px solid #f56565;
            margin: 1rem 0;
            font-size: 0.9rem;
        }

        .execution-ratio {
            background: #ebf8ff;
            color: #2c5282;
            padding: 1rem;
            border-radius: 4px;
            border-left: 3px solid #4299e1;
            margin-bottom: 1rem;
            font-size: 1.1rem;
            font-weight: 500;
            text-align: center;
        }

        .ratio-details {
            margin-top: 1.5rem;
            padding: 1rem;
            background: #f7fafc;
            border-radius: 4px;
            border: 1px solid #e2e8f0;
        }

        .ratio-details h3 {
            font-size: 1rem;
            margin-bottom: 0.5rem;
            color: #4a5568;
        }

        .ratio-details table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 0.5rem;
        }

        .ratio-details th, .ratio-details td {
            padding: 0.5rem;
            text-align: left;
            border-bottom: 1px solid #e2e8f0;
        }

        .ratio-details th {
            background: #f1f5f9;
            font-weight: 600;
        }

        .log-index-note {
            color: #718096;
            font-style: italic;
            margin-bottom: 1rem;
            padding: 0 1rem;
        }

        .mapping-table {
            width: 100%;
            border-collapse: collapse;
        }

        .mapping-table th, .mapping-table td {
            padding: 0.5rem;
            text-align: left;
            border-bottom: 1px solid #e2e8f0;
        }

        .mapping-table th {
            background: #f1f5f9;
            font-weight: 600;
        }

        /* Responsive design */
        @media (max-width: 768px) {
            .main-content {
                flex-direction: column;
            }
            
            .sidebar {
                width: 100%;
                max-height: 200px;
            }
            
            header h1 {
                font-size: 1.5rem;
            }
            
            .plot-area {
                padding: 0.75rem;
            }
        }
        """

    def _get_javascript_code(
        self,
        plot_data_json: str,
        max_time_data_json: str,
        annotations_json: str,
        log_mapping_json: str,
        ratio_data_json: str,
    ) -> str:
        """Return JavaScript code for plot functionality."""
        return f"""
        // Plot data
        const plotData = {plot_data_json};
        const maxTimeData = {max_time_data_json};
        const annotationsData = {annotations_json};
        const logMapping = {log_mapping_json};
        const ratioData = {ratio_data_json};
        let currentLog = null;
        const loadedPlots = new Set();

        // Tab switching functionality
        document.getElementById('tab-plots').addEventListener('click', () => {{
            switchTab('plots');
        }});
        
        document.getElementById('tab-max-time').addEventListener('click', () => {{
            switchTab('max-time');
            if (!document.getElementById('max-time-plot').hasChildNodes()) {{
                createMaxTimePlot();
                displayExecutionRatio();
            }}
        }});
        
        function switchTab(tabId) {{
            // Update tab buttons
            document.querySelectorAll('.tab-button').forEach(btn => {{
                btn.classList.remove('active');
            }});
            document.getElementById('tab-' + tabId).classList.add('active');
            
            // Update tab content
            document.querySelectorAll('.tab-content').forEach(content => {{
                content.classList.remove('active');
            }});
            document.getElementById(tabId + '-tab').classList.add('active');
        }}

        // Toggle sidebar width
        document.getElementById('toggle-sidebar').addEventListener('click', () => {{
            const sidebar = document.getElementById('sidebar');
            sidebar.classList.toggle('expanded');
        }});

        // Display execution ratio information
        function displayExecutionRatio() {{
            const executionRatioDiv = document.getElementById('execution-ratio');
            executionRatioDiv.textContent = ratioData.description;
            
            // Create ratio details table
            const ratioDetailsDiv = document.getElementById('ratio-details');
            
            let logDetailsHtml = '';
            if (ratioData.log_details && ratioData.log_details.length > 0) {{
                logDetailsHtml = `
                    <h3>Log Details</h3>
                    <table class="mapping-table">
                        <tr>
                            <th>Log</th>
                            <th>Max Scan Sensor</th>
                            <th>Scan Count</th>
                            <th>Max Scan Duration (ms)</th>
                            <th>Max Execution time Sensor</th>
                            <th>Max Execution Time (ms)</th>
                            <th>Ratio</th>
                        </tr>
                `;
                
                ratioData.log_details.forEach(detail => {{
                    const logRatio = detail.execution_time_ms / detail.duration_ms;
                    logDetailsHtml += `
                        <tr>
                            <td>${{detail.log_name}}</td>
                            <td>${{detail.max_count_sensor}}</td>
                            <td>${{detail.scan_count.toLocaleString()}}</td>
                            <td>${{detail.duration_ms.toFixed(2)}}</td>
                            <td>${{detail.max_execution_sensor}}</td>
                            <td>${{detail.execution_time_ms.toFixed(2)}}</td>
                            <td>${{logRatio.toFixed(4)}}</td>
                        </tr>
                    `;
                }});
                
                logDetailsHtml += '</table>';
            }}
            
            ratioDetailsDiv.innerHTML = `
                <h3>Execution Ratio</h3>
                <table>
                    <tr>
                        <th>Metric</th>
                        <th>Value</th>
                    </tr>
                    <tr>
                        <td>Total Execution Time</td>
                        <td>${{ratioData.total_execution_time.toFixed(2)}} ms</td>
                    </tr>
                    <tr>
                        <td>Total Log duration</td>
                        <td>${{ratioData.total_available_time.toFixed(2)}} ms</td>
                    </tr>
                </table>
                <br>
                <p style="margin-top: 1rem; color: #4a5568; font-size: 1rem; text-align: center;">
                    Execution Time / Log duration = ${{ratioData.ratio.toFixed(4)}}
                </p>
                <br>
                ${{logDetailsHtml}}
            `;
        }}


        // Create the max time plot
        function createMaxTimePlot() {{
            const layout = {{
                xaxis: {{
                    title: 'Log Index',
                    gridcolor: '#e2e8f0',
                    showgrid: true,
                    tickmode: 'array',
                    tickvals: annotationsData.xaxis.tickvals,
                    ticktext: annotationsData.xaxis.ticktext
                }},
                yaxis: {{
                    title: 'Time (ms)',
                    gridcolor: '#e2e8f0',
                    showgrid: true,
                    tickformat: '.3f'
                }},
                hovermode: 'closest',
                plot_bgcolor: 'white',
                paper_bgcolor: 'white',
                margin: {{ t: 50, b: 50, l: 60, r: 20 }},
                showlegend: true,
                legend: {{
                    x: 1.05,
                    y: 1,
                    xanchor: 'left',
                    yanchor: 'top',
                    bgcolor: 'rgba(255, 255, 255, 0.9)',
                    bordercolor: '#e2e8f0',
                    borderwidth: 1
                }},
                height: 600,
                autosize: true
            }};
            
            const config = {{
                responsive: true,
                displayModeBar: true,
                modeBarButtonsToRemove: ['pan2d', 'lasso2d', 'select2d'],
                displaylogo: false
            }};
            
            Plotly.newPlot('max-time-plot', maxTimeData, layout, config);
        }}

        // Load and display all plots for a log
        async function loadAllPlotsForLog(logPrefix) {{
            // Update navigation state
            document.querySelectorAll('.log-btn').forEach(btn => {{
                btn.classList.remove('active');
            }});
            document.getElementById('nav-' + logPrefix).classList.add('active');
            
            // Switch to plots tab if not already there
            switchTab('plots');
            
            // Show loading state
            showLoading();
            
            try {{
                // Create container for plots
                const plotsWrapper = document.getElementById('plots-wrapper');
                plotsWrapper.innerHTML = '';
                
                // Set the title
                document.getElementById('current-plot-title').textContent = logPrefix;
                
                // Get all sensors for this log
                const sensorKeys = Object.keys(plotData[logPrefix]);
                
                // Create a plot for each sensor
                for (let i = 0; i < sensorKeys.length; i++) {{
                    const sensorKey = sensorKeys[i];
                    const plotInfo = plotData[logPrefix][sensorKey];
                    
                    // Create plot wrapper
                    const plotWrapper = document.createElement('div');
                    plotWrapper.className = 'plot-wrapper';
                    
                    // Create plot header
                    const plotHeader = document.createElement('div');
                    plotHeader.className = 'plot-header';
                    plotHeader.textContent = sensorKey;
                    plotWrapper.appendChild(plotHeader);
                    
                    // Create plot div
                    const plotDiv = document.createElement('div');
                    plotDiv.id = 'plot-' + plotInfo.id;
                    plotDiv.className = 'plot';
                    plotWrapper.appendChild(plotDiv);
                    
                    // Add to document
                    plotsWrapper.appendChild(plotWrapper);
                    
                    // Create plot
                    const traces = plotInfo.traces;
                    
                    const layout = {{
                        xaxis: {{
                            title: 'SCAN_INDEX',
                            gridcolor: '#e2e8f0',
                            showgrid: true
                        }},
                        yaxis: {{
                            title: 'Time (ms)',
                            type: plotInfo.y_scale,
                            gridcolor: '#e2e8f0',
                            showgrid: true,
                            tickformat: '.3f'
                        }},
                        hovermode: 'closest',
                        plot_bgcolor: 'white',
                        paper_bgcolor: 'white',
                        margin: {{ t: 20, b: 40, l: 60, r: 20 }},
                        showlegend: true,
                        legend: {{
                            x: 1.05,
                            y: 1,
                            xanchor: 'left',
                            yanchor: 'top',
                            bgcolor: 'rgba(255, 255, 255, 0.9)',
                            bordercolor: '#e2e8f0',
                            borderwidth: 1
                        }},
                        height: 400,
                        autosize: true
                    }};
                    
                    const config = {{
                        responsive: true,
                        displayModeBar: true,
                        modeBarButtonsToRemove: ['pan2d', 'lasso2d', 'select2d'],
                        displaylogo: false
                    }};
                    
                    // Create plot
                    await Plotly.newPlot('plot-' + plotInfo.id, traces, layout, config);
                }}
                
                // Show plots
                showPlot();
                
                currentLog = logPrefix;
                
            }} catch (error) {{
                console.error('Error loading plots:', error);
                showError('Failed to load plots: ' + error.message);
            }}
        }}

        // Utility functions
        function showLoading() {{
            document.getElementById('loading-indicator').style.display = 'flex';
            document.getElementById('plot-container').style.display = 'none';
        }}

        function showPlot() {{
            document.getElementById('loading-indicator').style.display = 'none';
            document.getElementById('plot-container').style.display = 'block';
        }}

        function showError(message) {{
            const loadingDiv = document.getElementById('loading-indicator');
            loadingDiv.innerHTML = `
                <div class="error-message">
                    <strong>Error:</strong> ${{message}}
                </div>
            `;
        }}

        // Handle window resize
        window.addEventListener('resize', () => {{
            if (currentLog) {{
                const sensorKeys = Object.keys(plotData[currentLog]);
                for (let i = 0; i < sensorKeys.length; i++) {{
                    const sensorKey = sensorKeys[i];
                    const plotInfo = plotData[currentLog][sensorKey];
                    const plotElement = document.getElementById('plot-' + plotInfo.id);
                    if (plotElement) {{
                        Plotly.Plots.resize(plotElement);
                    }}
                }}
            }}
            
            // Resize max time plot if it exists
            const maxTimePlotElement = document.getElementById('max-time-plot');
            if (maxTimePlotElement && maxTimePlotElement.hasChildNodes()) {{
                Plotly.Plots.resize(maxTimePlotElement);
            }}
        }});

        // Initialize
        document.addEventListener('DOMContentLoaded', () => {{
            console.log('Statistical Report Plotter initialized');
            console.log('Available logs:', Object.keys(plotData).length);
            
            // Adjust sidebar width based on longest log name
            let maxLength = 0;
            Object.keys(plotData).forEach(logName => {{
                if (logName.length > maxLength) {{
                    maxLength = logName.length;
                }}
            }});
            
            // Set minimum width based on longest log name
            if (maxLength > 30) {{
                document.getElementById('sidebar').classList.add('expanded');
            }}
        }});
        """

    def generate_output(self) -> str:
        """Generate the complete HTML output file.

        Returns:
            Path to the generated HTML file
        """
        print(f"\nProcessing logs from: {self.sr_path}\n\n")
        print(f"Output will be saved to: {self.output_path}\n\n")

        # Generate plot data
        plot_data = self.generate_plot_data()

        if not plot_data:
            raise ValueError("No valid data to plot")

        # Create HTML output
        html_content = self.create_html_output(plot_data)

        # Save to file
        output_file = self.output_path / "profile_timing_plots.html"
        with open(output_file, "w", encoding="utf-8") as f:
            f.write(html_content)

        print("\nProcessing complete!")

        return str(output_file)


def main():
    """Main entry point for the timing plotter."""
    if len(sys.argv) < 2:
        print("Error: Please provide folder_path.txt")
        print("Usage: python timing_plotter.py folder_path.txt [output_path]")
        sys.exit(1)

    # Read folder path
    try:
        with open(sys.argv[1], "r", encoding="utf-8") as f:
            sr_path = f.read().strip()
    except FileNotFoundError:
        print(f"Error: {sys.argv[1]} not found")
        sys.exit(1)

    # Set output path
    output_path = sys.argv[2] if len(sys.argv) > 2 else sr_path

    try:
        # Create plotter and generate output
        plotter = TimingPlotter(sr_path, output_path)
        output_file = plotter.generate_output()
        print(f"\n✅ Success! Open {output_file} in your browser to view the plots.")

    except Exception as e:
        print(f"❌ Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
