"""
File Name: json_to_html_convert.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This module converts CAN json to html
"""

import os
import numpy as np
import plotly.io as pio
from html import escape
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.DashManager.report_dash import ReportDash


class CANJsonToHtmlConvertor:
    output_scan_index_scaled = np.array([])
    input_scan_index_scaled = np.array([])
    list_json_path = []
    mismatch_input_value = np.array([])
    mismatch_output_value = np.array([])
    mismatch_input_scan_index = np.array([])
    mismatch_output_scan_index = np.array([])

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore,
                 jsonpath_datastore: IDataStore,
                 report_dash: ReportDash):
        #print("Initializing CANJsonToHtmlConvertor")
        self._data_event_mediator_obj = event_mediator
        self._radar_datastore_obj = radar_datastore
        self._plot_datastore_obj = plot_datastore
        self._jsonpath_datastore_obj = jsonpath_datastore
        self._report_dash = report_dash

    def clear_data(self):
        CANJsonToHtmlConvertor.output_scan_index_scaled = np.array([])
        CANJsonToHtmlConvertor.input_scan_index_scaled = np.array([])
        CANJsonToHtmlConvertor.list_json_path.clear()
        CANJsonToHtmlConvertor.mismatch_input_value = np.array([])
        CANJsonToHtmlConvertor.mismatch_output_value = np.array([])
        CANJsonToHtmlConvertor.mismatch_input_scan_index = np.array([])
        CANJsonToHtmlConvertor.mismatch_output_scan_index = np.array([])

    def is_substring_match_in_longstring(self, substring, long_string_list):
        return any(substring.casefold() in long.casefold() for long in long_string_list)

    def filter_strings_with_substring(self, substring, long_string_list):
        return [s for s in long_string_list if substring.casefold() in s.casefold()]

    def consume_event(self, sensor, event):
        if event == "CAN_JSON_GEN_DONE":
            self._plot_datastore_obj.clear_data()
            self.trigger_json_to_html_conversion()

    def trigger_json_to_html_conversion(self):
        for sensor in ["FR", "FL", "RR", "RL", "FLR"]:  # Extend this list if needed
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "scatter")
            json_list_filtered_detection = self.filter_strings_with_substring("DETECTION", json_list)

            if self.is_substring_match_in_longstring("DETECTION", json_list_filtered_detection):
                self.generate_plotly_html_report(json_list_filtered_detection, f"{sensor}_detection_scatter.html")


    def generate_plotly_html_report(self, json_paths, output_html_filename):
        """
        Generate a Plotly HTML report from multiple JSON chart files.
        One JSON = One plot (no duplicates, no empty plots).
        """

        # ✅ Remove duplicate paths
        json_paths = list(set(json_paths))

        report_folder = os.path.join(self._report_dash.report_directory, "reports")
        os.makedirs(report_folder, exist_ok=True)

        html_parts = [
            "<html><head><meta charset='UTF-8'>",
            f"<title>Plotly Report – {output_html_filename}</title>",
            "<script src='https://cdn.plot.ly/plotly-latest.min.js'></script>",
            "<style>",
            "body { font-family: sans-serif; background: #f9f9f9; padding: 20px; margin: 0; }",
            ".grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; }",
            ".card { background: white; padding: 10px; border-radius: 6px;",
            "box-shadow: 0 2px 4px rgba(0,0,0,0.1); overflow-x: auto; min-width: 0; }",
            ".title { font-weight: bold; margin-bottom: 8px; font-size: 15px; text-align: center; }",
            "@media (max-width: 1200px) { .grid { grid-template-columns: 1fr; } }",
            "</style></head><body>",
            f"<h1>Plotly Interactive Report – {output_html_filename}</h1>",
            "<div class='grid'>"
        ]

        for path in json_paths:
            if not os.path.isfile(path):
                continue  # ❌ skip missing files completely

            try:
                fig = pio.read_json(path)

                # ❌ Skip if figure has no data
                if not fig.data or len(fig.data) == 0:
                    #print(f"[Skipping] Empty figure: {os.path.basename(path)}")
                    continue

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

                title = os.path.splitext(os.path.basename(path))[0]
                html_parts.append(f"<div class='card'><div class='title'>{title}</div>{fig_html}</div>")

            except Exception as e:
                print(f"[Error] Failed to render {path}: {e}")
                continue

        html_parts.append("</div></body></html>")

        output_file = os.path.join(report_folder, output_html_filename)

        with open(output_file, 'w', encoding='utf-8') as f:
            f.write('\n'.join(html_parts))

        print(f"✅ Final HTML report created: {output_file}")







