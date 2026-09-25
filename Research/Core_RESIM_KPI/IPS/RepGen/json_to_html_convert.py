"""
File Name: json_to_html_convert.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This module converts json to html
"""


from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore

from IPS.DashManager.report_dash import ReportDash
import numpy as np
import plotly.io as pio
from html import escape
import json
import os


class JsonToHtmlConvertor:
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
                 report_dash: ReportDash
                 ):
        self._data_event_mediator_obj = event_mediator
        self._radar_datastore_obj = radar_datastore
        self._plot_datastore_obj = plot_datastore
        self._jsonpath_datastore_obj = jsonpath_datastore
        self._report_dash = report_dash


    def clear_data(self):
        JsonToHtmlConvertor.output_scan_index_scaled = np.array([])
        JsonToHtmlConvertor.input_scan_index_scaled = np.array([])
        JsonToHtmlConvertor.list_json_path.clear()
        JsonToHtmlConvertor.mismatch_input_value = np.array([])
        JsonToHtmlConvertor.mismatch_output_value = np.array([])
        JsonToHtmlConvertor.mismatch_input_scan_index = np.array([])
        JsonToHtmlConvertor.mismatch_output_scan_index = np.array([])

    def is_substring_match_in_longstring(self, substring, long_string_list):
        return any(substring.casefold() in long.casefold() for long in long_string_list)

    def filter_strings_with_substring(self, substring, long_string_list):
        return [s for s in long_string_list if substring.casefold() in s.casefold()]

    def consume_event(self, sensor, event):

        if event == "JSON_GEN_DONE":
            self._plot_datastore_obj.clear_data()  # clearing all datastore information
            # as the collected data are converted to JSON
            self.trigger_json_to_html_conversion()

    def trigger_json_to_html_conversion(self):

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "scatter")
            json_list_filtered_detection = self.filter_strings_with_substring("DETECTION", json_list)
            json_list_filtered_vse = self.filter_strings_with_substring("VSE", json_list)

            if self.is_substring_match_in_longstring("DETECTION", json_list_filtered_detection):
                self.generate_plotly_html_report_new4(json_list_filtered_detection, sensor + "_detection_scatter.html")
            if self.is_substring_match_in_longstring("VSE", json_list_filtered_vse):
                self.generate_plotly_html_report_new4(json_list_filtered_vse, sensor + "_vse_scatter.html")

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "Histogram")
            json_list_filtered_detection = self.filter_strings_with_substring("DETECTION", json_list)
            json_list_filtered_vse = self.filter_strings_with_substring("VSE", json_list)

            if self.is_substring_match_in_longstring("DETECTION", json_list_filtered_detection):
                self.generate_plotly_html_report_new5(json_list_filtered_detection,
                                                      sensor + "_detection_histogram.html")

            if self.is_substring_match_in_longstring("vse", json_list_filtered_vse):
                self.generate_plotly_html_report_new5(json_list_filtered_vse, sensor + "_vse_histogram.html")

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "Histogram")
            json_list_filtered_down_selection = self.filter_strings_with_substring("DOWN_SELECTION", json_list)

            if self.is_substring_match_in_longstring("DOWN_SELECTION", json_list_filtered_down_selection):
                self.generate_plotly_html_report_new5(json_list_filtered_down_selection,
                                                      sensor + "_DOWN_SELECTION_histogram.html")

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "additionalmissing")
            if json_list is not None:
                json_list_set = set(json_list)
                # Use json_list_set as needed
            else:
                print(f"No data found for sensor: {sensor}")
                json_list_set = set()  # or continue/pass based on your logic
            json_list_filtered_detection = self.filter_strings_with_substring("DETECTION", json_list_set)
            json_list_filtered_detection_set = set(json_list_filtered_detection)

            if self.is_substring_match_in_longstring("DETECTION", json_list_filtered_detection_set):
                self.generate_plotly_html_report_new5(json_list_filtered_detection_set,
                                                      sensor + "_detection_addi_miss.html")

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "additionalmissing")
            if json_list is not None:
                json_list_set = set(json_list)
                # Use json_list_set as needed
            else:
                print(f"No data found for sensor: {sensor}")
                json_list_set = set()  # or continue/pass based on your logic

            json_list_filtered_down_selection = self.filter_strings_with_substring("DOWN_SELECTION", json_list_set)
            json_list_filtered_down_selection_set = set(json_list_filtered_down_selection)

            if self.is_substring_match_in_longstring("DOWN_SELECTION", json_list_filtered_down_selection_set):
                self.generate_plotly_html_report_new5(json_list_filtered_down_selection_set,
                                                      sensor + "_down_selection_addi_miss.html")

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "mismatch")
            json_list_set = set(json_list)
            json_list_filtered_detection = self.filter_strings_with_substring("DETECTION", json_list_set)
            json_list_filtered_detection_set = set(json_list_filtered_detection)

            if self.is_substring_match_in_longstring("DETECTION", json_list_filtered_detection_set):
                self.generate_plotly_html_report_new5(json_list_filtered_detection_set,
                                                      sensor + "_detection_mismatch.html")

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "mismatch")
            json_list_set = set(json_list)
            json_list_filtered_down_selection = self.filter_strings_with_substring("DOWN_SELECTION", json_list_set)
            json_list_filtered_down_selection_set = set(json_list_filtered_down_selection)

            if self.is_substring_match_in_longstring("DOWN_SELECTION", json_list_filtered_down_selection_set):
                self.generate_plotly_html_report_new5(json_list_filtered_down_selection_set,
                                                      sensor + "_down_selection_mismatch.html")

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "scatter")
            json_list_filtered_toi = self.filter_strings_with_substring("TOI", json_list)

            if self.is_substring_match_in_longstring("TOI", json_list_filtered_toi):
                self.generate_plotly_html_report_new4(json_list_filtered_toi, sensor + "_toi_scatter.html")

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "scatter")
            json_list_filtered_header = self.filter_strings_with_substring("HEADER", json_list)

            if self.is_substring_match_in_longstring("HEADER", json_list_filtered_header):
                self.generate_plotly_html_report_new4(json_list_filtered_header, sensor + "_header_scatter.html")

        for sensor in ["FC", "FL", "FR", "RR", "RL"]:
            json_list = self._jsonpath_datastore_obj.get_data(sensor, "scatter")
            json_list_filtered_down_selection = self.filter_strings_with_substring("DOWN_SELECTION", json_list)

            if self.is_substring_match_in_longstring("DOWN_SELECTION", json_list_filtered_down_selection):
                self.generate_plotly_html_report_new4(json_list_filtered_down_selection,
                                                      sensor + "_down_selection_scatter.html")

    def generate_plotly_html_report_new4(self, json_paths, output_html_path):
        html_parts = [
            "<html><head><meta charset='UTF-8'>",
            "<title>Plotly Report</title>",
            "<script src='https://cdn.plot.ly/plotly-latest.min.js'></script>",
            "<style>",
            "body { font-family: sans-serif; background: #f9f9f9; padding: 20px; margin: 0; }",
            ".grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; }",
            ".card { background: white; padding: 10px; border-radius: 6px;",
            "box-shadow: 0 2px 4px rgba(0,0,0,0.1); overflow-x: auto; min-width: 0; }",
            ".title { font-weight: bold; margin-bottom: 8px; font-size: 15px; text-align: center; }",
            "@media (max-width: 1200px) { .grid { grid-template-columns: 1fr; } }",
            "</style></head><body>",
            "<h1>Plotly Interactive Chart Report</h1>",
            "<div class='grid'>"
        ]

        for path in json_paths:
            if not os.path.isfile(path):
                html_parts.append(
                    f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>File not found</p></div>")
                continue

            try:
                fig = pio.read_json(path)

                # Move legend below plot to avoid side squeeze
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

                # title = os.path.basename(path)
                title = os.path.splitext(os.path.basename(path))[0]
                html_parts.append(f"<div class='card'><div class='title'>{title}</div>{fig_html}</div>")
            except Exception as e:
                html_parts.append(
                    f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>Error: {e}</p></div>")

        html_parts.append("</div></body></html>")

        report_folder = ReportDash.report_directory + "/reports"
        os.makedirs(report_folder, exist_ok=True)
        output_html_path = os.path.join(report_folder, output_html_path)
        with open(output_html_path, 'w') as f:
            f.write('\n'.join(html_parts))

        # print(f"✅ Final Plotly HTML report created: {output_html_path}")

    def generate_plotly_html_report_new5(self, json_paths, output_html_path):
        html_parts = [
            "<html><head><meta charset='UTF-8'>",
            "<title>Plotly Report</title>",
            "<script src='https://cdn.plot.ly/plotly-latest.min.js'></script>",
            "<style>",
            "body { font-family: sans-serif; background: #f9f9f9; padding: 20px; margin: 0; }",
            ".grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; }",
            ".card { background: white; padding: 10px; border-radius: 6px;",
            "box-shadow: 0 2px 4px rgba(0,0,0,0.1); overflow-x: auto; min-width: 0; }",
            ".title { font-weight: bold; margin-bottom: 8px; font-size: 15px; text-align: center; }",
            "@media (max-width: 1200px) { .grid { grid-template-columns: 1fr; } }",
            "</style></head><body>",
            "<h1>Plotly Interactive Chart Report</h1>",
            "<div class='grid'>"
        ]

        for path in json_paths:
            if not os.path.isfile(path):
                html_parts.append(
                    f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>File not found</p></div>")
                continue

            try:
                fig = pio.read_json(path)

                # Move legend below plot to avoid side squeeze
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

                title = os.path.basename(path)
                html_parts.append(f"<div class='card'><div class='title'>{title}</div>{fig_html}</div>")
            except Exception as e:
                html_parts.append(
                    f"<div class='card'><div class='title'>{path}</div><p style='color:red;'>Error: {e}</p></div>")

        html_parts.append("</div></body></html>")

        report_folder = ReportDash.report_directory + "/reports"
        os.makedirs(report_folder, exist_ok=True)
        output_html_path = os.path.join(report_folder, output_html_path)
        with open(output_html_path, 'w') as f:
            f.write('\n'.join(html_parts))

        # print(f"✅ Final Plotly HTML report created: {output_html_path}")
