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
        <div>Matching Mode: <span class="kpi-value">{matching_mode}</span></div>
        <div>Min Accuracy: <span class="kpi-value">{min_accuracy}%</span></div>
        <div>Max Accuracy: <span class="kpi-value">{max_accuracy}%</span></div>
        <div>Scans (vehicle/simulation): <span class="kpi-value">{veh_si_count}/{sim_si_count}</span></div>
        <div>Scans processed / with matches: <span class="kpi-value">{scans_processed} / {scans_with_matches}</span></div>
    </div>

    <div class="kpi-box">
        <div class="kpi-header">ScanIndex Match</div>
        <!-- Legacy row/unique mixed % keeps original value for backward compat -->
        <div>ScanIndex Match % (legacy rows) : <span class="kpi-value">{scan_match_pct_str}</span></div>
        <div>Common / Input Total (rows): <span class="kpi-value">{common_count} / {input_total}</span></div>
        <div>Input Only: <span class="kpi-value">{input_only_count}</span> | Output Only: <span class="kpi-value">{output_only_count}</span></div>
        <div>Common Scan Count: <span class="kpi-value">{common_scan_count}</span></div>
        <!-- Isolated unbiased diff (separate function, does NOT affect alignment logic) -->
        <div style="margin-top:8px; padding-top:8px; border-top:1px dashed #ddd;">
            <div>ScanIndex Match % (input-unique) : <span class="kpi-value">{input_match_pct_str}</span></div>
            <div>ScanIndex Match % (output-unique): <span class="kpi-value">{output_match_pct_str}</span></div>
            <div>Jaccard (IoU) % : <span class="kpi-value">{jaccard_pct_str}</span></div>
            <div style="font-size:0.85em; color:#6b7280;">Unbiased unique/unique via isolated <code>scan_index_metrics.calculate_scanindex_match_metrics</code> (exclude_zero=True)</div>
        </div>
        <!-- Hidden KPI table for machine extraction (e.g., master index) -->
        <table style="display:none;">
            <tr><td>common_scan_count</td><td>{common_count}</td></tr>
            <tr><td>input_only_scan_count</td><td>{input_only_count}</td></tr>
            <tr><td>output_only_scan_count</td><td>{output_only_count}</td></tr>
            <tr><td>avg_scan_match_pct</td><td>{avg_scan_match_pct_raw}</td></tr>
            <tr><td>matched_scan_index_count</td><td>{common_count}</td></tr>
            <tr><td>input_unique</td><td>{input_unique_raw}</td></tr>
            <tr><td>output_unique</td><td>{output_unique_raw}</td></tr>
            <tr><td>input_match_pct</td><td>{input_match_pct_raw}</td></tr>
            <tr><td>output_match_pct</td><td>{output_match_pct_raw}</td></tr>
            <tr><td>jaccard_pct</td><td>{jaccard_pct_raw}</td></tr>
            <tr><td>scan_match_pct_unique</td><td>{scan_match_pct_unique_raw}</td></tr>
        </table>
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
                    <th>{denominator_label}</th>
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
