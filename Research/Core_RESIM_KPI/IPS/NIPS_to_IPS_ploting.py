import os
import json
import threading

import zmq
import plotly.io as pio
from collections import defaultdict
import time
import argparse
import shutil
import plotly.graph_objects as go
import html
import sys
import traceback
import queue
from IPS.DashManager.report_dash import ReportDash

msg_queue = queue.Queue()
stop_event = threading.Event()


class HTMLNIPStoInteractiveTool:

    def __init__(self,port):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PULL)
        self.socket.connect(f"tcp://localhost:{port}")
        self.port = port

    @staticmethod
    def fix_booleans(obj):
        if isinstance(obj, dict):
            for k, v in obj.items():
                if isinstance(v, str):
                    if v.lower() == "true":
                        obj[k] = True
                    elif v.lower() == "false":
                        obj[k] = False
                else:
                    HTMLNIPStoInteractiveTool.fix_booleans(v)
        elif isinstance(obj, list):
            for item in obj:
                HTMLNIPStoInteractiveTool.fix_booleans(item)

    def generate_scatter_json_reports(self, json_root_path):
        report_directory = os.path.join(json_root_path, "reports")
        os.makedirs(report_directory, exist_ok=True)

        radar_json_map = defaultdict(lambda: defaultdict(list))
        for root, dirs, files in os.walk(json_root_path):
            for file in files:
                if file.endswith(".json") and "scatter" in file.lower() and "mismatchscatter" not in file.lower():
                    lower_file = file.lower()
                    radar_pos = os.path.basename(root)
                    full_path = os.path.join(root, file)

                    if "detection" in lower_file:
                        radar_json_map[radar_pos]["detection"].append(full_path)
                    elif "toi" in lower_file:
                        radar_json_map[radar_pos]["toi"].append(full_path)
                    elif "vse" in lower_file:
                        radar_json_map[radar_pos]["vse"].append(full_path)
                    elif "header" in lower_file:
                        radar_json_map[radar_pos]["header"].append(full_path)

        for radar_pos, type_map in radar_json_map.items():
            for json_type, json_paths in type_map.items():
                html_parts = [
                    "<html><head><meta charset='UTF-8'>",
                    f"<title>{radar_pos}_{json_type} Scatter Report</title>",
                    "<script src='https://cdn.plot.ly/plotly-latest.min.js'></script>",
                    "<style>",
                    "body { font-family: sans-serif; background: #f9f9f9; padding: 20px; margin: 0; }",
                    ".grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; }",
                    ".card { background: white; padding: 10px; border-radius: 6px;",
                    "box-shadow: 0 2px 4px rgba(0,0,0,0.1); overflow-x: auto; min-width: 0; }",
                    ".title { font-weight: bold; margin-bottom: 8px; font-size: 15px; text-align: center; }",
                    "@media (max-width: 1200px) { .grid { grid-template-columns: 1fr; } }",
                    "</style></head><body>",
                    f"<h1>{radar_pos} {json_type.upper()} Scatter Plots</h1>",
                    "<div class='grid'>"
                ]

                for path in json_paths:
                    try:
                        with open(path, 'r', encoding='utf-8') as f:
                            raw_data = json.load(f)

                        self.fix_booleans(raw_data)

                        fig = pio.from_json(json.dumps(raw_data))
                        fig.update_layout(
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
                            f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>Error: {e}</p></div>"
                        )

                html_parts.append("</div></body></html>")

                output_html_path = os.path.join(report_directory, f"{radar_pos}_{json_type}_scatter.html")
                with open(output_html_path, 'w', encoding='utf-8') as f:
                    f.write("\n".join(html_parts))

                print(f"✅ Scatter report generated: {output_html_path}")

    def generate_mismatchscatter_json_reports(self, json_root_path):
        report_directory = os.path.join(json_root_path, "reports")
        os.makedirs(report_directory, exist_ok=True)

        radar_json_map = defaultdict(list)
        for root, dirs, files in os.walk(json_root_path):
            for file in files:
                if file.endswith(".json") and "mismatchscatter" in file.lower():
                    radar_pos = os.path.basename(root)
                    full_path = os.path.join(root, file)
                    radar_json_map[radar_pos].append(full_path)

        for radar_pos, json_paths in radar_json_map.items():
            html_parts = [
                "<html><head><meta charset='UTF-8'>",
                f"<title>{html.escape(radar_pos)} Mismatch Scatter Report</title>",
                "<script src='https://cdn.plot.ly/plotly-latest.min.js'></script>",
                "<style>",
                "body { font-family: sans-serif; background: #f9f9f9; padding: 20px; margin: 0; }",
                ".grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; }",
                ".card { background: white; padding: 10px; border-radius: 6px;",
                "box-shadow: 0 2px 4px rgba(0,0,0,0.1); overflow-x: auto; min-width: 0; }",
                ".title { font-weight: bold; margin-bottom: 8px; font-size: 15px; text-align: center; }",
                "@media (max-width: 1200px) { .grid { grid-template-columns: 1fr; } }",
                "</style></head><body>",
                f"<h1>{html.escape(radar_pos)} MISMATCH SCATTER Plots</h1>",
                "<div class='grid'>"
            ]

            for path in json_paths:
                try:
                    with open(path, 'r', encoding='utf-8') as f:
                        raw_data = json.load(f)

                    self.fix_booleans(raw_data)

                    fig = pio.from_json(json.dumps(raw_data))
                    fig.update_layout(
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

                    title = html.escape(os.path.splitext(os.path.basename(path))[0])
                    html_parts.append(f"<div class='card'><div class='title'>{title}</div>{fig_html}</div>")
                except Exception as e:
                    tb = traceback.format_exc()
                    html_parts.append(
                        f"<div class='card'><div class='title'>{html.escape(path)}</div>"
                        f"<p style='color:red;'>Error: {html.escape(str(e))}</p>"
                        f"<pre style='white-space:pre-wrap'>{html.escape(tb)}</pre></div>"
                    )

            html_parts.append("</div></body></html>")

            output_html_path = os.path.join(report_directory, f"{radar_pos}_mismatchscatter.html")
            with open(output_html_path, 'w', encoding='utf-8') as f:
                f.write("\n".join(html_parts))

            print(f"📊 Mismatch Scatter report generated: {output_html_path}")

    def generate_histogram_json_reports(self, json_root_path):
        report_directory = os.path.join(json_root_path, "reports")
        os.makedirs(report_directory, exist_ok=True)

        radar_json_map = defaultdict(lambda: defaultdict(list))
        for root, dirs, files in os.walk(json_root_path):
            for file in files:
                if file.endswith(".json") and "histogram" in file.lower():
                    lower_file = file.lower()
                    radar_pos = os.path.basename(root)
                    full_path = os.path.join(root, file)

                    if "detection" in lower_file:
                        radar_json_map[radar_pos]["detection"].append(full_path)
                    elif "toi" in lower_file:
                        radar_json_map[radar_pos]["toi"].append(full_path)
                    elif "vse" in lower_file:
                        radar_json_map[radar_pos]["vse"].append(full_path)
                    elif "header" in lower_file:
                        radar_json_map[radar_pos]["header"].append(full_path)

        for radar_pos, type_map in radar_json_map.items():
            for json_type, json_paths in type_map.items():
                html_parts = [
                    "<html><head><meta charset='UTF-8'>",
                    f"<title>{radar_pos}_{json_type} Histogram Report</title>",
                    "<script src='https://cdn.plot.ly/plotly-latest.min.js'></script>",
                    "<style>",
                    "body { font-family: sans-serif; background: #f9f9f9; padding: 20px; margin: 0; }",
                    ".grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; }",
                    ".card { background: white; padding: 10px; border-radius: 6px;",
                    "box-shadow: 0 2px 4px rgba(0,0,0,0.1); overflow-x: auto; min-width: 0; }",
                    ".title { font-weight: bold; margin-bottom: 8px; font-size: 15px; text-align: center; }",
                    "@media (max-width: 1200px) { .grid { grid-template-columns: 1fr; } }",
                    "</style></head><body>",
                    f"<h1>{radar_pos} {json_type.upper()} Histogram Plots</h1>",
                    "<div class='grid'>"
                ]

                for path in json_paths:
                    try:
                        with open(path, 'r', encoding='utf-8') as f:
                            raw_data = json.load(f)

                        self.fix_booleans(raw_data)

                        fig = pio.from_json(json.dumps(raw_data))
                        fig.update_layout(
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
                            f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>Error: {e}</p></div>"
                        )

                html_parts.append("</div></body></html>")

                output_html_path = os.path.join(report_directory, f"{radar_pos}_{json_type}_histogram.html")
                with open(output_html_path, 'w', encoding='utf-8') as f:
                    f.write("\n".join(html_parts))

                print(f"📊 Histogram report generated: {output_html_path}")

    def map_match_mismatch_json(self,json_path):
        for sensor_name in os.listdir(json_path):
            sensor_path = os.path.join(json_path, sensor_name)
            json_file_path = os.path.join(sensor_path, "match_mismatch.json")

            if os.path.isdir(sensor_path) and os.path.exists(json_file_path):
                try:
                    with open(json_file_path, 'r') as f:
                        data = json.load(f)

                    for sensor, signals in data.items():
                        for sig, values in signals.items():
                            match_percent, mismatch_percent = values
                            ReportDash.update_signal_stats(sensor, sig, match_percent, mismatch_percent)
                            ReportDash.signal_name_set.add(sig)

                except Exception as e:
                    print(f"❌ Error processing {json_file_path}: {e}")

    def clean_json_folder(self, folder_path):
        print(f"🧹 Cleaning up JSON folder: {folder_path}")
        for item in os.listdir(folder_path):
            item_path = os.path.join(folder_path, item)
            if os.path.isdir(item_path) and item.lower() == "reports":
                print(f" Skipping 'reports' folder: {item_path}")
                continue
            try:
                if os.path.isdir(item_path):
                    shutil.rmtree(item_path)
                    print(f" Deleted folder: {item_path}")
                else:
                    os.remove(item_path)
                    print(f" Deleted file: {item_path}")
            except Exception as e:
                print(f" Failed to delete {item_path}: {e}")

    def worker(self):
        while not stop_event.is_set():
            try:
                task = msg_queue.get(timeout=1)
                if task.get("command") == "stop_service":
                    print("[Worker] Received stop signal.")
                    stop_event.set()
                    break

                while True:
                    message = self.socket.recv_string()
                    print(f"📥 Received path: {message}")
                    try:
                        self.generate_scatter_json_reports(message)
                        self.generate_histogram_json_reports(message)
                        self.generate_mismatchscatter_json_reports(message)
                        self.map_match_mismatch_json(message)
                        print("dict:", ReportDash.signal_stats)
                        self.clean_json_folder(message)
                        self.socket.send_string("Report generation completed.")
                    except Exception as e:
                        self.socket.send_string(f"Error: {str(e)}")
                    break  # Exit after one message

            except queue.Empty:
                continue




    def NIPS_communication(self):
        bind_address = f"tcp://*:{self.port}"
        self.socket.bind(bind_address)
        print(f"[Listener] Bound to {bind_address}, waiting for messages...")
        while not stop_event.is_set():
            try:
                msg = self.socket.recv_json(flags=zmq.NOBLOCK)
                print(f"[Listener] Received: {msg}")
                msg_queue.put(msg)
            except zmq.Again:
                time.sleep(0.1)





