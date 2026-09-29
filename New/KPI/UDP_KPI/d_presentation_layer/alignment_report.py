alignment_html = """<html>
<head>
    <title>Alignment KPIs - {sensor_id}</title>
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
        .kpi-summary {{
            margin: 15px 0;
            padding: 10px;
            background-color: #f8f9fa;
            border-radius: 4px;
        }}
        details {{
            margin: 10px 0;
            padding: 10px;
            border: 1px solid #e1e4e8;
            border-radius: 4px;
        }}
        summary {{
            font-weight: bold;
            cursor: pointer;
            padding: 5px 0;
        }}
    </style>
    <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
</head>
<body>
    <div class="kpi-section">
        <h1>Alignment KPIs - {sensor_id}</h1>
        
        <div class="kpi-box">
            <div class="kpi-header">Alignment Accuracy</div>
            <div class="kpi-summary">
                <p><strong>Azimuth Accuracy:</strong> <span class="kpi-value">{az_numerator}/{az_denominator} = {az_accuracy}%</span></p>
                <p><strong>Elevation Accuracy:</strong> <span class="kpi-value">{el_numerator}/{el_denominator} = {el_accuracy}%</span></p>
                <p><strong>Total Scans Processed:</strong> <span class="kpi-value">{total_scans}</span></p>
            </div>
        </div>

        <div class="kpi-box">
            <div class="kpi-header">Error Statistics</div>
            <div class="kpi-summary">
                <p><strong>Azimuth (deg):</strong> MAE: <span class="kpi-value">{az_mae}</span>, RMSE: <span class="kpi-value">{az_rmse}</span>, Max |Error|: <span class="kpi-value">{az_max_abs}</span></p>
                <p><strong>Elevation (deg):</strong> MAE: <span class="kpi-value">{el_mae}</span>, RMSE: <span class="kpi-value">{el_rmse}</span>, Max |Error|: <span class="kpi-value">{el_max_abs}</span></p>
            </div>
        </div>

        <div class="thresholds">
            <b>Thresholds Used:</b>
            <ul>
                <li>Azimuth misalignment threshold: {az_threshold:.6f}°</li>
                <li>Elevation misalignment threshold: {el_threshold:.6f}°</li>
            </ul>
        </div>

        <div class="plot-container">
            <div class="plot-title">Misalignment vs Scan Index</div>
            <div id="misalignment-plot"></div>
        </div>

        <div class="plot-container">
            <div class="plot-title">Misalignment Difference vs Scan Index</div>
            <div id="difference-plot"></div>
        </div>

        <details>
            <summary><strong>Analysis Details</strong></summary>
            <p>This KPI measures the accuracy of alignment estimation by comparing the difference between 
            nominal and estimated boresight angles in vehicle data versus simulation data.</p>
            
            <h4>Key Metrics:</h4>
            <ul>
                <li><strong>Azimuth Accuracy:</strong> Percentage of scans where the azimuth misalignment difference between 
                    vehicle and simulation is within the threshold of {az_threshold:.2f}°</li>
                <li><strong>Elevation Accuracy:</strong> Percentage of scans where the elevation misalignment difference between 
                    vehicle and simulation is within the threshold of {el_threshold:.2f}°</li>
            </ul>
        </details>
    </div>

    <script>
        var misalignmentPlot = JSON.parse('{misalignment_plot}');
        Plotly.newPlot('misalignment-plot', misalignmentPlot.data, misalignmentPlot.layout);
        var differencePlot = JSON.parse('{difference_plot}');
        Plotly.newPlot('difference-plot', differencePlot.data, differencePlot.layout);
    </script>
</body>
</html>
"""
