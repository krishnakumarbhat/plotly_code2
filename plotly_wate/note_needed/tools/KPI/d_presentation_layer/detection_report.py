detection_html = """<html>
<head>
    <title>Detection KPIs - {sensor_id}</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            margin: 20px;
            line-height: 1.6;
        }}
        .kpi-box {{
            border: 1px solid #ddd;
            padding: 15px;
            margin: 10px 0;
            border-radius: 5px;
            background-color: #f9f9f9;
        }}
        .kpi-header {{
            font-weight: bold;
            color: #2c3e50;
            margin-bottom: 10px;
            font-size: 1.1em;
        }}
        .kpi-value {{
            font-weight: bold;
            color: #2980b9;
        }}
        .thresholds {{
            background-color: #f0f7ff;
            padding: 10px;
            border-left: 4px solid #3498db;
            margin: 10px 0;
        }}
        .plot-container {{
            margin: 30px 0;
            border: 1px solid #eee;
            border-radius: 5px;
            padding: 15px;
        }}
        .plot-title {{
            font-size: 1.2em;
            margin-bottom: 15px;
            color: #2c3e50;
            font-weight: bold;
        }}
        table {{
            border-collapse: collapse;
            width: 100%;
            margin-top: 10px;
        }}
        th, td {{
            border: 1px solid #ddd;
            padding: 8px;
            text-align: left;
        }}
        th {{
            background-color: #f2f2f2;
        }}
        .btn-link {{
            display: inline-block;
            padding: 8px 12px;
            background: #3498db;
            color: #fff;
            text-decoration: none;
            border-radius: 4px;
            transition: background 0.2s ease;
        }}
        .btn-link:hover {{ background: #2c80b7; }}
    </style>
    <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
</head>
<body>
    <h1>Detection KPIs - {sensor_id}</h1>

    <div class="kpi-box">
        <div class="kpi-header">Matching Accuracy</div>
        <div>Matched Detections: <span class="kpi-value">{matches}/{total_detections}</span></div>
        <div>Accuracy: <span class="kpi-value">{accuracy}%</span></div>
    </div>



    <div class="kpi-box">
        <div class="kpi-header">Summary</div>
        <div>Min Accuracy: <span class="kpi-value">{min_accuracy}%</span></div>
        <div>Max Accuracy: <span class="kpi-value">{max_accuracy}%</span></div>
        <div>Scans (vehicle/simulation): <span class="kpi-value">{veh_si_count}/{sim_si_count}</span></div>
        <div>Scans processed / with matches: <span class="kpi-value">{scans_processed} / {scans_with_matches}</span></div>
    </div>

    <div class="thresholds">
        <b>Thresholds Used:</b>
        <ul>
            <li>Range threshold: {ran_th} m</li>
            <li>Velocity threshold: {vel_th} m/s</li>
            <li>Azimuth threshold: {theta_th} rad</li>
            <li>Elevation threshold: {phi_th} rad</li>
        </ul>
    </div>

    <div class="plot-container">
        <div class="plot-title">Accuracy vs Scan Index</div>
        <div id="accuracy-plot"></div>
    </div>

    <div class="plot-container">
        <div class="plot-title">Number of AF Detections vs Scan Index</div>
        <div id="af-det-plot"></div>
    </div>

    <div class="kpi-box">
        <div class="kpi-header">Per-Scan Accuracy</div>
        <table>
            <thead>
                <tr>
                    <th>Scan Index</th>
                    <th>Matches</th>
                    <th>Detections (Vehicle)</th>
                    <th>Accuracy (%)</th>
                </tr>
            </thead>
            <tbody>
                {per_scan_rows}
            </tbody>
        </table>
    </div>

    <script>
        var accuracyPlot = JSON.parse('{accuracy_plot}');
        Plotly.newPlot('accuracy-plot', accuracyPlot.data, accuracyPlot.layout);
        var afDetPlot = JSON.parse('{af_det_plot}');
        Plotly.newPlot('af-det-plot', afDetPlot.data, afDetPlot.layout);
    </script>
</body>
</html>
"""
