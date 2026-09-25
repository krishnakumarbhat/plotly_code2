"""
File Name: can_dataprep.py
Author: A, Sinchana
Email : Sinchana.A@aptiv.com
Description:

"""


import gc
import os
import plotly.graph_objects as go
from IPS.DashManager.report_dash import ReportDash
from IPS.DataPrep.plot_dataprep import PlotDataPreparation
from IPS.DataStore.idatastore import IDataStore
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.Sig_Prep.isig_observer import ISigObserver
from IPS.Sig_Prep.sig_filter import SignalFilter


class CANDataToJSONConvert(ISigObserver):

    SENSOR_MAP = {
        "MCIP_FR": "FR",
        "MCIP_FL": "FL",
        "MCIP_RR": "RR",
        "MCIP_RL": "RL",
        "MCIP_FLR": "FLR",
        "CEER_FR": "FR",
        "CEER_FL": "FL",
        "CEER_RR": "RR",
        "CEER_RL": "RL",
        "CEER_FLR": "FLR"
    }

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore,
                 jsonpath_datastore: IDataStore,
                 plot_data_prep: PlotDataPreparation,
                 sig_filter: SignalFilter):
        self._data_event_mediator_obj = event_mediator
        self._radar_datastore_obj = radar_datastore
        self._plot_datastore_obj = plot_datastore
        self._jsonpath_datastore_obj = jsonpath_datastore
        self._plot_data_prep_obj = plot_data_prep
        self._sig_filter_obj = sig_filter

    def final_poi_sensor_datapath(self, data):
        print(" # final_poi_sig")

    def consume_event(self, sensor_info, event):
        if event == "CAN_DATA_PREP_UPDATED":
            signal_list = ["DET_RANGE", "DET_RCS", "DET_AZIMUTH", "DET_ELEVATION", "DET_RANGE_VELOCITY", "DET_SNR"]
            stream_list = ["SRR_FR_DETECTION", "SRR_FL_DETECTION", "SRR_RR_DETECTION", "SRR_RL_DETECTION",
                           "FLR_DETECTION"]
            for stream in stream_list:
                for signal in signal_list:
                    try:
                        plot_types = self._sig_filter_obj.get_sig_plot_type(stream, signal)
                        plot_unit = self._sig_filter_obj.get_sig_unit(stream, signal)

                        if plot_types and "SC" in plot_types:
                            self.generate_json_scatter_overlay(sensor_info, stream, signal, plot_unit)
                        else:
                            print(f"[Info] Signal '{signal}' does not support scatter plot — skipping.")
                    except Exception as e:
                        print(f"[Error] Failed to process signal '{signal}' for sensor '{sensor_info}': {e}")


    def generate_json_scatter_overlay(self, sensor, stream_base, sig, plot_unit):
        if sig.lower() == "scan_index":
            #print(f"[Info] Skipping unsupported signal: {sig}")
            return

        fig = go.Figure()

        in_timestamp_data = self._plot_datastore_obj.get_data("flattened_timestamp", "in", sensor)
        in_signal_data = self._plot_datastore_obj.get_data(f"flattened_{sig.lower()}", "in", sensor)
        out_timestamp_data = self._plot_datastore_obj.get_data("flattened_timestamp", "out", sensor)
        out_signal_data = self._plot_datastore_obj.get_data(f"flattened_{sig.lower()}", "out", sensor)

        if in_timestamp_data and in_signal_data:
            fig.add_trace(
                go.Scattergl(
                    x=in_timestamp_data,
                    y=in_signal_data,
                    mode='markers',
                    marker=dict(size=3, opacity=0.5, color="blue"),
                    name="input"
                )
            )

        if out_timestamp_data and out_signal_data:
            fig.add_trace(
                go.Scattergl(
                    x=out_timestamp_data,
                    y=out_signal_data,
                    mode='markers',
                    marker=dict(size=3, opacity=0.5, color="red"),
                    name="output"
                )
            )

        fig.update_layout(
            xaxis_title="Timestamp (ms)",
            yaxis_title=str(plot_unit),
            title=f"{sig} vs Timestamp"
        )

        gc.collect()

        # Normalize signal and stream names
        normalized_sig = sig.lower()
        stream = stream_base.split("_")[-1]  # e.g., DETECTION from SRR_FR_DETECTION
        sensor = self.SENSOR_MAP.get(sensor, sensor)

        # Save JSON
        sensor_folder = os.path.abspath(os.path.join(ReportDash.report_directory, "JSON", sensor))
        os.makedirs(sensor_folder, exist_ok=True)

        json_path = f"{sensor}_{stream}_{normalized_sig}_scatter.json"
        file_path = os.path.join(sensor_folder, json_path)

        PlotDataPreparation.list_json_path.append(json_path)

        # Use mapped sensor name for updating JSON path

        self._jsonpath_datastore_obj.update_data(sensor, file_path, "scatter")

        try:
            with open(file_path, "w", encoding="utf-8", newline="\n") as f:
                f.write(fig.to_json())
            #print(f"[Success] JSON file created: {file_path}")
        except Exception as e:
            print(f"[Error] Failed to write JSON file for signal '{sig}': {e}")