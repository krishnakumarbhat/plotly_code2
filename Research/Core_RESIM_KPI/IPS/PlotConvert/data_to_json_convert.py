"""
File Name: data_to_json_convert.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This module will convert processed data to Json file
"""


import gc
import os
import plotly.graph_objects as go
from IPS.PlotConvert.dc_data_to_json_convert import OBJECT_CLASS_MAP
from IPS.DashManager.report_dash import ReportDash
from IPS.DataPrep.plot_dataprep import PlotDataPreparation
from IPS.DataStore.idatastore import IDataStore
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.Sig_Prep.isig_observer import ISigObserver
from IPS.Sig_Prep.sig_filter import SignalFilter
from math import degrees


def convert_signal_values(signal: str, values):
    if values is None:
        return None
    sig = (signal or '').lower()
    try:
        if 'curvi_heading' in sig:
            return [degrees(float(x)) for x in values]
        if 'vel' in sig:
            return [float(x) * 3.6 for x in values]
        return values
    except Exception:
        try:
            return list(values)
        except Exception:
            return values


class DataToJSONConvert(ISigObserver):

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
        print("RadarHDFDataCollect # final_poi_sig ")
        # print(data)
        # self._final_poi_sensor_dataset_path = data

    def consume_event(self, sensor_info, event):
        """
        This function Consumes an event JDS_UPDATED
        event carry info (sensor#stream#signal)
        index 0 : sensor
        index 1 : stream
        index 2 : signal
        """

        if event == "JDS_UPDATED":
            sensor_info = sensor_info.split('#')
            if sensor_info[1] != 'scan_index':
                self.generate_json_plot(sensor_info[0], sensor_info[1], sensor_info[2])

    def generate_json_plot(self, sensor, stream, signal):

        plot_types = self._sig_filter_obj.get_sig_plot_type(stream, signal)
        plot_unit = self._sig_filter_obj.get_sig_unit(stream, signal)

        if '/' in stream:
            stream_first = stream.split('/')[-2]
        else:
            stream_first = stream

        if "SC" in plot_types:
            self.generate_json_scatter_overlay(sensor, stream_first, signal, plot_unit)

        if "HIS" in plot_types:
            self.generate_histogram(sensor, stream_first, signal)

        if "SC_MM" in plot_types:
            if signal != "scan_index":
                for sensor, mismatch_signal in self._plot_datastore_obj.mismatch_signal_list.items():
                    self.generate_json_scatter_mismatch(sensor, stream_first, mismatch_signal, plot_unit)

                for sensor, additional_miss_signal in self._plot_datastore_obj.additional_missing_signal_list.items():
                    self.generate_json_scatter_additional_missing(sensor, stream_first, additional_miss_signal,
                                                                  plot_unit)

    def generate_histogram(self, sensor, stream, signal):

        data1 = self._plot_datastore_obj.get_data(signal, "in")
        data2 = self._plot_datastore_obj.get_data(signal, "out")

        if data1 is not None:
            data1 = self._plot_datastore_obj.get_data(signal, "in").tolist()
            data1 = convert_signal_values(signal, data1)
        if data2 is not None:
            data2 = self._plot_datastore_obj.get_data(signal, "out").tolist()
            data2 = convert_signal_values(signal, data2)

        if data1 is not None or data2 is not None:
            # Create histogram traces with transparency
            trace1 = go.Histogram(
                x=data1,
                opacity=0.5,
                name='input',
                marker_color='blue'
            )

            trace2 = go.Histogram(
                x=data2,
                opacity=0.5,
                name='output',
                marker_color='red'
            )

            # Combine traces
            fig = go.Figure(data=[trace1, trace2])

            # Set overlay mode for transparency
            fig.update_layout(
                barmode='overlay',
                title='Histogram ',
                xaxis_title='Value',
                yaxis_title='Count'
            )

            # Ensure the report directory exists
            sensor_folder = os.path.join(ReportDash.report_directory + "/JSON", sensor)
            os.makedirs(sensor_folder, exist_ok=True)

            # Save the figure as a JSON file
            json_path = sensor + "_" + stream + "_" + str("histogram") + "_" + signal + ".json"

            file_path = os.path.join(sensor_folder, json_path)

            PlotDataPreparation.list_json_path.append(json_path)
            self._jsonpath_datastore_obj.update_data(sensor, file_path, "Histogram")

            with open(file_path, "w", encoding="utf-8") as f:
                f.write(fig.to_json())

    def generate_json_scatter_overlay(self, sensor, stream, sig, plot_unit):
        if sig != "scan_index":
            input_data = self._plot_datastore_obj.get_data(sig, "in")
            output_data = self._plot_datastore_obj.get_data(sig, "out")
            if input_data is not None:
                input_data = self._plot_datastore_obj.get_data(sig, "in").tolist()
                input_data = convert_signal_values(sig, input_data)
                input_scan_index = self._plot_datastore_obj.get_data(sig, "in_scan_index", sensor).tolist()

            if output_data is not None:
                output_data = self._plot_datastore_obj.get_data(sig, "out").tolist()
                output_data = convert_signal_values(sig, output_data)
                output_scan_index = self._plot_datastore_obj.get_data(sig, "out_scan_index", sensor).tolist()

            if input_data is not None or output_data is not None:
                fig = go.Figure()

                fig.add_trace(
                    go.Scattergl(
                        x=input_scan_index,
                        y=input_data,
                        mode='markers',
                        marker=dict(size=3, opacity=0.5, color="blue"),
                        name="input"
                    )
                )
                fig.add_trace(
                    go.Scattergl(
                        x=output_scan_index,
                        y=output_data,
                        mode='markers',
                        marker=dict(size=3, opacity=0.5, color="red"),
                        name="output"
                    )
                )

                # Add custom Y-axis tick labels for object_class
                if sig.lower() == 'object_class':
                    fig.update_layout(
                        xaxis_title="Scan Index",
                        yaxis=dict(
                            title=str(plot_unit),
                            tickvals=list(OBJECT_CLASS_MAP.keys()),
                            ticktext=list(OBJECT_CLASS_MAP.values())
                        )
                    )
                else:
                    fig.update_layout(
                        xaxis_title="Scan Index",
                        yaxis_title=str(plot_unit)
                    )

                gc.collect()

                # Ensure the report directory exists
                sensor_folder = os.path.abspath(os.path.join(ReportDash.report_directory, "JSON", sensor))
                os.makedirs(sensor_folder, exist_ok=True)

                json_path = f"{sensor}_{stream}_scatter_{sig}.json"
                file_path = os.path.join(sensor_folder, json_path)

                PlotDataPreparation.list_json_path.append(json_path)
                self._jsonpath_datastore_obj.update_data(sensor, file_path, "scatter")

                # Write JSON with UTF-8 encoding and Linux-friendly line endings
                with open(file_path, "w", encoding="utf-8", newline="\n") as f:
                    f.write(fig.to_json())

    def generate_json_scatter_additional_missing(self, sensor, stream, sig, plot_unit):

        missing_data = self._plot_datastore_obj.get_data(sig, "missing_value", sensor)
        missing_scan_index = self._plot_datastore_obj.get_data(sig, "missing_scan_index", sensor)
        additional_data = self._plot_datastore_obj.get_data(sig, "additional_value", sensor)
        additional_scan_index = self._plot_datastore_obj.get_data(sig, "additional_scan_index", sensor)

        if len(missing_data) != 0 or len(additional_data) != 0:
            fig = go.Figure()

            fig.add_trace(
                go.Scattergl(x=missing_scan_index,
                             y=missing_data, mode='markers',
                             marker=dict(size=3, opacity=0.6, color="blue"),
                             name='missing'))

            fig.add_trace(
                go.Scattergl(x=additional_scan_index,
                             y=additional_data, mode='markers',
                             marker=dict(size=3, opacity=0.6, color="red"),
                             name='additional'))

            # Add custom Y-axis tick labels for object_class
            if sig.lower() == 'object_class':
                fig.update_layout(
                    xaxis_title="Scan Index",
                    yaxis=dict(
                        title=str(plot_unit),
                        tickvals=list(OBJECT_CLASS_MAP.keys()),
                        ticktext=list(OBJECT_CLASS_MAP.values())
                    )
                )
            else:
                fig.update_layout(
                    xaxis_title="Scan Index",
                    yaxis_title=str(plot_unit)
                )

            gc.collect()
            # Save figure to temporary JSON file

            # Ensure the report directory exists
            sensor_folder = os.path.join(ReportDash.report_directory + "/JSON", sensor)
            os.makedirs(sensor_folder, exist_ok=True)

            # Full path to save the JSON file
            json_path = sensor + "_" + stream + "_" + str("additionalmissing") + "_" + sig + ".json"
            file_path = os.path.join(sensor_folder, json_path)
            PlotDataPreparation.list_json_path.append(json_path)
            self._jsonpath_datastore_obj.update_data(sensor, file_path, "additionalmissing")

            with open(file_path, "w", encoding="utf-8") as f:
                f.write(fig.to_json())

            PlotDataPreparation.input_scan_index_scaled = None
            PlotDataPreparation.output_scan_index_scaled = None
            input_data = None
            output_data = None

    def generate_json_scatter_mismatch(self, sensor, stream, sig, plot_unit):

        input_data = self._plot_datastore_obj.get_data(sig, "mismatch_value_in", sensor)
        input_scan_index = self._plot_datastore_obj.get_data(sig, "mismatch_scan_index_in", sensor)
        # output_data = self._plot_datastore_obj.get_data(sig, "mismatch_value_out", sensor)
        # output_scan_index = self._plot_datastore_obj.get_data(sig, "mismatch_scan_index_out", sensor)

        if len(input_data) != 0:
            fig = go.Figure()

            fig.add_trace(
                go.Scattergl(x=input_scan_index,
                             y=input_data, mode='markers',
                             marker=dict(size=3, opacity=0.5, color="blue"),
                             name='input'))
            '''

            fig.add_trace(
                go.Scattergl(x=output_scan_index,
                             y=output_data, mode='markers',
                             marker=dict(size=3, opacity=0.5, color="red"),
                             name='output'))
                             
            '''

            fig.update_layout(
                # title=str(sensor) + "_" + str(stream) + "_" + "scatter" + "_" + str(sig),
                xaxis_title="Scan Index",
                yaxis_title=str(plot_unit)
            )

            gc.collect()
            # Save figure to temporary JSON file

            # Ensure the report directory exists
            sensor_folder = os.path.join(ReportDash.report_directory + "/JSON", sensor)
            os.makedirs(sensor_folder, exist_ok=True)
            json_path = sensor + "_" + stream + "_" + str("mismatch") + "_" + sig + ".json"
            file_path = os.path.join(sensor_folder, json_path)

            PlotDataPreparation.list_json_path.append(json_path)
            self._jsonpath_datastore_obj.update_data(sensor, file_path, "mismatch")

            with open(file_path, "w", encoding="utf-8") as f:
                f.write(fig.to_json())

            PlotDataPreparation.input_scan_index_scaled = None
            PlotDataPreparation.output_scan_index_scaled = None
            input_data = None
            output_data = None
