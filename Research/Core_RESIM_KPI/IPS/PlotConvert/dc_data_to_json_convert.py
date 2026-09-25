"""
File Name: dc_data_to_json_convert.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This module will convert DC processed data to Json file
"""

import gc
import os
import plotly.graph_objects as go

import matplotlib
import matplotlib.pyplot as plt
from math import degrees

from IPS.DashManager.report_dash import ReportDash
from IPS.DataPrep.plot_dataprep import PlotDataPreparation
from IPS.DataStore.idatastore import IDataStore
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.Sig_Prep.isig_observer import ISigObserver
from IPS.Sig_Prep.sig_filter import SignalFilter

# Object class mapping for Y-axis labels
OBJECT_CLASS_MAP = {
    0: 'OBJECT_CLASS_UNKNOWN',
    1: 'OBJECT_CLASS_CAR',
    2: 'OBJECT_CLASS_2WHEEL',
    3: 'OBJECT_CLASS_TRUCK',
    4: 'OBJECT_CLASS_PEDESTRIAN',
}


def convert_signal_values(signal: str, values):
    if values is None:
        return None
    sig = (signal or '').lower()
    try:
        if sig == 'object_class':
            remapped = []
            for x in values:
                cls = int(float(x))
                if cls == 1:
                    remapped.append(1)
                elif cls == 2:
                    remapped.append(2)
                elif cls == 3:
                    remapped.append(3)
                elif 4 <= cls <= 9:
                    remapped.append(4)
                else:
                    # Includes 0 and 10, and any out-of-range values
                    remapped.append(0)
            return remapped
        if sig.endswith('vel') or '_vel_' in sig or sig.startswith('vel_'):
            return [float(x) * 3.6 for x in values]
        return values
    except Exception:
        try:
            return list(values)
        except Exception:
            return values

class DCDataToJSONConvert(ISigObserver):

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore,
                 jsonpath_datastore: IDataStore,
                 plot_data_prep: PlotDataPreparation,
                 sig_filter: SignalFilter,
                 dc_hdf_datacollector: IDataCollect):
        self._data_event_mediator_obj = event_mediator
        self._radar_datastore_obj = radar_datastore
        self._plot_datastore_obj = plot_datastore
        self._jsonpath_datastore_obj = jsonpath_datastore
        self._plot_data_prep_obj = plot_data_prep
        self._sig_filter_obj = sig_filter
        self._dc_hdf_datacollector = dc_hdf_datacollector

    def final_poi_sensor_datapath(self, data):
        pass

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

        unit = self._dc_hdf_datacollector.get_unit(signal)
        self.generate_json_scatter_overlay(sensor, stream, signal, unit)
        self.generate_histogram(sensor, stream, signal)

    def generate_histogram(self, sensor, stream, signal):

        data1 = self._plot_datastore_obj.get_data(signal, "in")
        data2 = self._plot_datastore_obj.get_data(signal, "out")

        if data1 is not None:
            data1 = data1.tolist()
            data1 = convert_signal_values(signal, data1)
        if data2 is not None:
            data2 = data2.tolist()
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

            stream = stream.split('/')[-2] if '/' in stream else stream
            stream = stream.replace("_", "")

            # Save the figure as a JSON file
            json_path = sensor + "_" + stream + "_" + str("histogram") + "_" + signal + ".json"

            file_path = os.path.join(sensor_folder, json_path)

            PlotDataPreparation.list_json_path.append(json_path)
            self._jsonpath_datastore_obj.update_data(sensor, file_path, "Histogram")

            with open(file_path, "w", encoding="utf-8") as f:
                f.write(fig.to_json())

    def generate_json_scatter_overlay(self, sensor, stream, sig, plot_unit):

        # print("generate_json_scatter_overlay")
        input_data = None
        input_scan_index = None
        output_data= None
        output_scan_index= None
        if sig != "scan_index":
            try:
                input_data = self._plot_datastore_obj.get_data(sig, "in")
            except KeyError as e:
                print(f"input Key/signal {e} not found in plot data store")
                input_data = None
                input_scan_index = None

            try:
                output_data = self._plot_datastore_obj.get_data(sig, "out")
            except KeyError as e:
                print(f"output Key/signal {e} not found in plot data store")
                output_data = None
                output_scan_index = None

            if input_data is not None:
                input_data = input_data.tolist()
                input_data = convert_signal_values(sig, input_data)
                # Try to get signal-specific scan_index; if None, fall back to any available signal
                input_scan_index_data = self._plot_datastore_obj.get_data(sig, "in_scan_index", sensor)
                if input_scan_index_data is None:
                    # Try fallback signals in order until we find one with scan_index
                    for fallback_sig in ["range", "range_rate", "snr", "azimuth", "elevation", "amplitude"]:
                        input_scan_index_data = self._plot_datastore_obj.get_data(fallback_sig, "in_scan_index", sensor)
                        if input_scan_index_data is not None:
                            break
                if input_scan_index_data is not None:
                    input_scan_index = input_scan_index_data.tolist()
                else:
                    input_scan_index = None
            if output_data is not None:
                output_data = output_data.tolist()
                output_data = convert_signal_values(sig, output_data)
                # Try to get signal-specific scan_index; if None, fall back to any available signal
                output_scan_index_data = self._plot_datastore_obj.get_data(sig, "out_scan_index", sensor)
                if output_scan_index_data is None:
                    # Try fallback signals in order until we find one with scan_index
                    for fallback_sig in ["range", "range_rate", "snr", "azimuth", "elevation", "amplitude"]:
                        output_scan_index_data = self._plot_datastore_obj.get_data(fallback_sig, "out_scan_index", sensor)
                        if output_scan_index_data is not None:
                            break
                if output_scan_index_data is not None:
                    output_scan_index = output_scan_index_data.tolist()
                else:
                    output_scan_index = None

            # Skip plotting if we don't have scan_index for this signal
            if input_data is not None and input_scan_index is None:
                print(f"Warning: Skipping plot for {sensor} {sig} - no scan_index available")
                return
            if output_data is not None and output_scan_index is None:
                print(f"Warning: Skipping plot for {sensor} {sig} - no scan_index available")
                return

            fig = go.Figure()

            # Add input trace only if input data exists
            if input_data is not None:
                fig.add_trace(
                    go.Scattergl(
                        x=input_scan_index,
                        y=input_data,
                        mode='markers',
                        marker=dict(size=2, opacity=0.5, color="blue"),
                        name="input"
                    )
                )

            # Add output trace only if output data exists
            if output_data is not None:
                fig.add_trace(
                    go.Scattergl(
                        x=output_scan_index,
                        y=output_data,
                        mode='markers',
                        marker=dict(size=2, opacity=0.5, color="red"),
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
                    ),
                    showlegend=True
                )
            else:
                fig.update_layout(
                    xaxis_title="Scan Index",
                    yaxis_title=str(plot_unit),
                    showlegend=True
                )

            gc.collect()

            # Ensure the report directory exists
            sensor_folder = os.path.abspath(os.path.join(ReportDash.report_directory, "JSON", sensor))
            os.makedirs(sensor_folder, exist_ok=True)

            # stream = stream.split('/')[-2]
            stream = stream.split('/')[-2] if '/' in stream else stream
            stream = stream.replace("_", "")
            json_path = f"{sensor}_{stream}_scatter_{sig}.json"
            file_path = os.path.join(sensor_folder, json_path)
            # print("json_path", json_path)
            # print("file_path", file_path)
            PlotDataPreparation.list_json_path.append(json_path)
            self._jsonpath_datastore_obj.update_data(sensor, file_path, "scatter")

            # Write JSON with UTF-8 encoding and Linux-friendly line endings
            with open(file_path, "w", encoding="utf-8", newline="\n") as f:
                f.write(fig.to_json())

            if ReportDash.datasource_type == "udpdc":

                # --- Save PNG in image/<sensor>/ via Matplotlib (no Kaleido) ---
                image_folder = os.path.abspath(os.path.join(ReportDash.report_directory, "image", sensor))
                os.makedirs(image_folder, exist_ok=True)

                # PNG name must match JSON stem
                stem, _ = os.path.splitext(json_path)
                png_path = os.path.join(image_folder, f"{stem}.png")

                # Helper: align x/y; fallback to index if x is None or length mismatch
                def _xy_for_plot(x, y):
                    if y is None or len(y) == 0:
                        return None, None
                    if (x is None) or (len(x) != len(y)):
                        x = list(range(len(y)))
                    return x, y

                x1, y1 = _xy_for_plot(input_scan_index, input_data)
                x2, y2 = _xy_for_plot(output_scan_index, output_data)

                if (y1 is None) and (y2 is None):
                    print(f"[SKIP] No data to plot for {sensor}/{sig}")
                    return

                # Skip PNG export if disabled globally
                if not getattr(ReportDash, 'export_png', True):
                    # print(f"PNG export disabled; skipping {png_path}")
                    return

                # Matplotlib overlay scatter
                try:
                    # A slightly larger figure helps dense clouds; adjust dpi for crisp PNG
                    fig_m, ax = plt.subplots(figsize=(8, 5), dpi=150)

                    # input (blue)
                    if y1 is not None:
                        ax.scatter(x1, y1, s=2, alpha=0.5, c="blue", label="input", rasterized=True)
                    # output (red)
                    if y2 is not None:
                        ax.scatter(x2, y2, s=2, alpha=0.5, c="red", label="output", rasterized=True)

                    ax.set_xlabel("Scan Index")
                    ax.set_ylabel(str(plot_unit))
                    ax.legend(loc="best")
                    ax.grid(True, linestyle="--", alpha=0.4)

                    fig_m.tight_layout()
                    fig_m.savefig(png_path, bbox_inches="tight")
                    plt.close(fig_m)

                    # print(f"[OK] Image saved: {png_path}")
                except Exception as e:
                    print(f"[ERROR] Matplotlib export failed for {png_path}: {e}")

