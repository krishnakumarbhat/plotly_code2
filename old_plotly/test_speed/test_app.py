#!/usr/bin/env python3
"""
Lightweight Diagnostic Flask Website
-----------------------------------
Deploy this file to both servers (port 5003 or any available port)
and browse to it to run real-time hardware, database, and network diagnostics.
It helps identify if the slowness is caused by:
1. Slow SQLite DB page locked on network shared storage (SMB/NFS)
2. Network timeout from blocking broker sockets
3. Gunicorn/process worker delays or CPU throttles
"""

import os
import sys
import time
import socket
import sqlite3
import shutil
import tempfile
from flask import Flask, jsonify, render_template_string

app = Flask(__name__)

# Basic landing page with UI for running diagnostics
HTML_TEMPLATE = """
<!DOCTYPE html>
<html>
<head>
    <title>HPC Server Performance Diagnostic</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
            line-height: 1.6;
            color: #333;
            max-width: 900px;
            margin: 0 auto;
            padding: 20px;
            background-color: #f8f9fa;
        }
        h1, h2, h3 { color: #2c3e50; }
        .card {
            background: #fff;
            padding: 20px;
            margin-bottom: 20px;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.05);
            border: 1px solid #e9ecef;
        }
        .btn {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 10px 20px;
            font-size: 16px;
            border-radius: 4px;
            cursor: pointer;
            text-decoration: none;
            display: inline-block;
        }
        .btn:hover { background-color: #2980b9; }
        .btn-green { background-color: #2ecc71; }
        .btn-green:hover { background-color: #27ae60; }
        table {
            width: 100%;
            border-collapse: collapse;
            margin: 15px 0;
        }
        th, td {
            text-align: left;
            padding: 12px;
            border-bottom: 1px solid #e9ecef;
        }
        th { background-color: #f1f3f5; }
        .success { color: #2ecc71; font-weight: bold; }
        .warning { color: #f39c12; font-weight: bold; }
        .danger { color: #e74c3c; font-weight: bold; }
        .loader {
            display: none;
            border: 4px solid #f3f3f3;
            border-top: 4px solid #3498db;
            border-radius: 50%;
            width: 30px;
            height: 30px;
            animation: spin 1s linear infinite;
            margin: 10px auto;
        }
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
        pre {
            background-color: #f1f3f5;
            padding: 15px;
            border-radius: 4px;
            overflow-x: auto;
            font-family: "SFMono-Regular", Consolas, "Liberation Mono", Menlo, monospace;
        }
    </style>
</head>
<body>
    <h1>💻 HPC Server Performance Diagnostic</h1>
    <p>Running on: <strong>{{ env_info.get('hostname') }}</strong> | Local Sys Time: {{ env_info.get('time') }}</p>
    
    <div class="card">
        <h2>🔬 Diagnose Performance Bottlenecks</h2>
        <p>This script executes tests on the local filesystem, CPU throughput, and local socket server broker to detect exactly why operations are slow.</p>
        <button class="btn btn-green" onclick="runDiagnostics()">Run Diagnostic Tests Suite</button>
        <div class="loader" id="diag-loader"></div>
        <div id="diag-results" style="margin-top:20px; display:none;">
            <h3>Diagnostics Results</h3>
            <table>
                <tr>
                    <th>Test Component</th>
                    <th>Measured Time / Detail</th>
                    <th>Assessment & Status</th>
                </tr>
                <tr>
                    <td><strong>CPU Loop Speed</strong><br><small>10,000 calculation runs</small></td>
                    <td id="res-cpu-time">-</td>
                    <td id="res-cpu-status">-</td>
                </tr>
                <tr>
                    <td><strong>Local DB Speed (SQLite Writes/Sec)</strong><br><small>50 insert transactions</small></td>
                    <td id="res-db-time">-</td>
                    <td id="res-db-status">-</td>
                </tr>
                <tr>
                    <td><strong>Local Temp DB Speed (SQLite RAM/Tmp)</strong><br><small>Compare local OS disk write speed</small></td>
                    <td id="res-tmpdb-time">-</td>
                    <td id="res-tmpdb-status">-</td>
                </tr>
                <tr>
                    <td><strong>Broker Ping Connection</strong><br><small>Port 9100 response or fail-closed speed</small></td>
                    <td id="res-broker-time">-</td>
                    <td id="res-broker-status">-</td>
                </tr>
                <tr>
                    <td><strong>Filesystem Type Detection</strong><br><small>Base project path inspect</small></td>
                    <td id="res-fs-path">-</td>
                    <td id="res-fs-status">-</td>
                </tr>
            </table>
            
            <h3>Raw Diagnostic JSON</h3>
            <pre id="raw-json"></pre>
        </div>
    </div>

    <div class="card">
        <h2>📂 Environment & Path Diagnostics</h2>
        <table>
            <tr>
                <td>Python Version</td>
                <td>{{ env_info.get('python_version') }}</td>
            </tr>
            <tr>
                <td>CWD (Current Working Dir)</td>
                <td>{{ env_info.get('cwd') }}</td>
            </tr>
            <tr>
                <td>Platform / OS</td>
                <td>{{ env_info.get('os') }}</td>
            </tr>
            <tr>
                <td>CPU Core Count</td>
                <td>{{ env_info.get('cpu_count') }}</td>
            </tr>
            <tr>
                <td>Memory statistics (/proc/meminfo)</td>
                <td><small>{{ env_info.get('mem_info', 'Not available on Windows') }}</small></td>
            </tr>
        </table>
    </div>

    <script>
        function runDiagnostics() {
            document.getElementById('diag-loader').style.display = 'block';
            document.getElementById('diag-results').style.display = 'none';
            
            fetch('/run-diagnostics')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('diag-loader').style.display = 'none';
                    document.getElementById('diag-results').style.display = 'block';
                    
                    // Decode CPU results
                    const cpuSec = data.cpu.duration_seconds.toFixed(4);
                    document.getElementById('res-cpu-time').innerHTML = `${cpuSec}s`;
                    if (data.cpu.duration_seconds < 0.05) {
                        document.getElementById('res-cpu-status').innerHTML = '<span class="success">FAST (Healthy CPU)</span>';
                    } else {
                        document.getElementById('res-cpu-status').innerHTML = '<span class="warning">SLUGGISH (Choked/Shared VM)</span>';
                    }

                    // Decode Shared SQLite Database Results
                    const dbSec = data.db_shared.duration_seconds.toFixed(4);
                    const dbPerSec = data.db_shared.writes_per_second.toFixed(1);
                    document.getElementById('res-db-time').innerHTML = `${dbSec}s (${dbPerSec} writes/sec)`;
                    if (data.db_shared.writes_per_second > 100) {
                        document.getElementById('res-db-status').innerHTML = '<span class="success">EXCELLENT (Local SSD Storage)</span>';
                    } else if (data.db_shared.writes_per_second > 10) {
                        document.getElementById('res-db-status').innerHTML = '<span class="warning">ACCEPTABLE (Lustre/Fast Cluster Share)</span>';
                    } else {
                        document.getElementById('res-db-status').innerHTML = '<span class="danger">EXCEEDINGLY SLOW (NFS/SMB Storage Locking Bottleneck)</span>';
                    }

                    // Decode Tmp/RAM SQLite Database Results
                    const tmpSec = data.db_temp.duration_seconds.toFixed(4);
                    const tmpPerSec = data.db_temp.writes_per_second.toFixed(1);
                    document.getElementById('res-tmpdb-time').innerHTML = `${tmpSec}s (${tmpPerSec} writes/sec)`;
                    if (data.db_temp.writes_per_second > 100) {
                        document.getElementById('res-tmpdb-status').innerHTML = '<span class="success">HEALED using local /tmp block disk</span>';
                    } else {
                        document.getElementById('res-tmpdb-status').innerHTML = '<span class="warning">Also slow (OS/VM layer IO bottle)</span>';
                    }

                    // Decode Broker connection results
                    const brokerSec = data.broker.connect_duration_seconds.toFixed(4);
                    const bStatus = data.broker.success ? '<span class="success">CONNECTED</span>' : 
                                   (data.broker.error.includes('Connection refused') ? '<span class="warning">FAILED FAST (Offline - Refused)</span>' : '<span class="danger">TIMEOUT (Firewall Drop Blackhole)</span>');
                    document.getElementById('res-broker-time').innerHTML = `${brokerSec}s (Port ${data.broker.port})`;
                    document.getElementById('res-broker-status').innerHTML = `${bStatus}<br><small>${data.broker.error || 'Connected successfully'}</small>`;

                    // Decode filesystem / path results
                    document.getElementById('res-fs-path').innerText = data.fs.path;
                    if (data.fs.is_network_share) {
                        document.getElementById('res-fs-status').innerHTML = '<span class="warning">NETWORK SHARE detected.<br>SQLite databases of the main app will be very slow on this path! Use local /tmp instead.</span>';
                    } else {
                        document.getElementById('res-fs-status').innerHTML = '<span class="success">LOCAL FS or fast local mount detected.</span>';
                    }

                    // Display Raw Json
                    document.getElementById('raw-json').textContent = JSON.stringify(data, null, 4);
                })
                .catch(error => {
                    document.getElementById('diag-loader').style.display = 'none';
                    alert("Error running diagnostic: " + error);
                });
        }
    </script>
</body>
</html>
"""

@app.route('/')
def index():
    # Gather environment metadata
    mem_info = "N/A"
    if os.path.exists('/proc/meminfo'):
        try:
            with open('/proc/meminfo', 'r') as f:
                mem_info = "".join([f.readline() for _ in range(5)]) # Get first 5 lines
        except Exception:
            pass

    env_info = {
        'hostname': socket.gethostname(),
        'cwd': os.getcwd(),
        'os': sys.platform,
        'python_version': sys.version,
        'cpu_count': os.cpu_count(),
        'time': time.strftime("%Y-%m-%d %H:%M:%S %Z"),
        'mem_info': mem_info
    }
    return render_template_string(HTML_TEMPLATE, env_info=env_info)


@app.route('/run-diagnostics')
def run_diagnostics():
    results = {}

    # 1. CPU Benchmark Check
    t0 = time.perf_counter()
    x = 0
    for i in range(10000):
        x += (i ** 2) % 325
    t1 = time.perf_counter()
    results['cpu'] = {
        'duration_seconds': t1 - t0,
        'result_hash': x
    }

    # 2. SQLite latency in the project path (shared or local filesystem)
    shared_db_path = os.path.join(os.getcwd(), 'diagnostic_bench_shared.db')
    if os.path.exists(shared_db_path):
        try: os.remove(shared_db_path)
        except Exception: pass

    try:
        t0 = time.perf_counter()
        conn = sqlite3.connect(shared_db_path)
        conn.execute('CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)')
        for i in range(50):
            conn.execute('INSERT INTO test (value) VALUES (?)', (f'value_string_{i}',))
            conn.commit()  # Forces file sync/write transaction
        conn.close()
        t1 = time.perf_counter()
        dur = t1 - t0
        results['db_shared'] = {
            'duration_seconds': dur,
            'writes_per_second': 50.0 / dur if dur > 0 else 0,
            'success': True,
            'error': None
        }
    except Exception as e:
        results['db_shared'] = {
            'duration_seconds': 0,
            'writes_per_second': 0,
            'success': False,
            'error': str(e)
        }
    finally:
        try: os.remove(shared_db_path)
        except Exception: pass

    # 3. SQLite latency in /tmp (guaranteed fast local OS mount if on Linux)
    temp_dir = tempfile.gettempdir()
    temp_db_path = os.path.join(temp_dir, 'diagnostic_bench_temp.db')
    if os.path.exists(temp_db_path):
        try: os.remove(temp_db_path)
        except Exception: pass

    try:
        t0 = time.perf_counter()
        conn = sqlite3.connect(temp_db_path)
        conn.execute('CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)')
        for i in range(50):
            conn.execute('INSERT INTO test (value) VALUES (?)', (f'value_string_{i}',))
            conn.commit()
        conn.close()
        t1 = time.perf_counter()
        dur = t1 - t0
        results['db_temp'] = {
            'duration_seconds': dur,
            'writes_per_second': 50.0 / dur if dur > 0 else 0,
            'success': True,
            'error': None
        }
    except Exception as e:
        results['db_temp'] = {
            'duration_seconds': 0,
            'writes_per_second': 0,
            'success': False,
            'error': str(e)
        }
    finally:
        try: os.remove(temp_db_path)
        except Exception: pass

    # 4. Broker connection speed check
    # Pings port 9100 on localhost
    broker_port = int(os.environ.get('HPCC_BROKER_PORT', '9100'))
    broker_host = os.environ.get('HPCC_BROKER_HOST', '127.0.0.1')
    broker_results = {
        'port': broker_port,
        'host': broker_host,
        'success': False,
        'connect_duration_seconds': 0.0,
        'error': None
    }
    
    t0 = time.perf_counter()
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(2.0) # Set timeout short to test fast fail or socket connection speed
        s.connect((broker_host, broker_port))
        s.close()
        broker_results['success'] = True
    except Exception as e:
        broker_results['error'] = str(e)
    finally:
        t1 = time.perf_counter()
        broker_results['connect_duration_seconds'] = t1 - t0
        
    results['broker'] = broker_results

    # 5. Filesystem inspection
    path = os.getcwd()
    is_network = False
    # Check common pathways on cluster to flag network share
    if any(network_indicator in path.lower() for network_indicator in ['/net/', '/mnt/', 'plkra', 'usmidet']):
        is_network = True

    results['fs'] = {
        'path': path,
        'is_network_share': is_network
    }

    return jsonify(results)


if __name__ == '__main__':
    # Run on default port 5003 (so it doesn't conflict with main app running on 5002)
    port = int(os.environ.get('PORT', 5003))
    print(f"Starting Diagnostic Server on http://0.0.0.0:{port}...")
    app.run(host='0.0.0.0', port=port, debug=False)
