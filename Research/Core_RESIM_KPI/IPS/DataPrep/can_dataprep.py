
"""
File Name: can_dataprep.py
Author: A, Sinchana
Email : Sinchana.A@aptiv.com
Description:

"""

import re

from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DataCollect.icollectdata import IDataCollect
import numpy as np


class CANPlotDataPreparation:

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore,
                 jsonpath_datastore: IDataStore,
                 sig_filter: SignalFilter,
                 can_hdf_datacollector: IDataCollect):
        self._data_event_mediator = event_mediator
        self._data_event_mediator_obj = event_mediator
        self._plot_datastore_obj = plot_datastore
        self._radar_datastore_obj = radar_datastore
        self._jsonpath_datastore_obj = jsonpath_datastore
        self._sig_filter_obj = sig_filter
        self._can_hdf_datacollector = can_hdf_datacollector

    def clear_data(self):
        print(" ")

    def consume_event(self, sensor, event):
        if event != "CAN_DATA_COLLECTION_DONE":
            return

        signal_list = [
            "DET_RANGE",
            "DET_RCS",
            "DET_AZIMUTH",
            "DET_ELEVATION",
            "DET_RANGE_VELOCITY",
            "DET_SNR"
        ]

        for signal in signal_list:
            try:
                in_timestamp_data = self._radar_datastore_obj.get_data(sensor, "timestamp_data", "in")
                out_timestamp_data = self._radar_datastore_obj.get_data(sensor, "timestamp_data", "out")
                in_signal_data = self._radar_datastore_obj.get_data(sensor, signal, "in")
                out_signal_data = self._radar_datastore_obj.get_data(sensor, signal, "out")

                if not in_timestamp_data and not out_timestamp_data:
                    continue
                if not in_signal_data and not out_signal_data:
                    continue

                # ---------- Input processing ----------
                flattened_timestamp = []
                flattened_signal = []

                for stream_name, ts_values in in_timestamp_data.items():
                    signal_data = []
                    total_signal_values = 0

                    for sublist in in_signal_data:
                        total_signal_values += len(sublist)
                        signal_data.extend(sublist)

                    scaled_ts = []
                    if ts_values and total_signal_values > 0:
                        repeat_factor = total_signal_values // len(ts_values)
                        remainder = total_signal_values % len(ts_values)
                        scaled_ts = ts_values * repeat_factor + ts_values[:remainder]

                    flattened_timestamp.extend(scaled_ts)
                    flattened_signal.extend(signal_data)

                self._plot_datastore_obj.update_data("flattened_timestamp", flattened_timestamp, "in", sensor)
                self._plot_datastore_obj.update_data(f"flattened_{signal.lower()}", flattened_signal, "in", sensor)

                # ---------- Output processing ----------
                flattened_timestamp_out = []
                flattened_signal_out = []

                for stream_name, ts_values in out_timestamp_data.items():
                    signal_data = []
                    total_signal_values = 0

                    for sublist in out_signal_data:
                        total_signal_values += len(sublist)
                        signal_data.extend(sublist)

                    scaled_ts = []
                    if ts_values and total_signal_values > 0:
                        repeat_factor = total_signal_values // len(ts_values)
                        remainder = total_signal_values % len(ts_values)
                        scaled_ts = ts_values * repeat_factor + ts_values[:remainder]

                    flattened_timestamp_out.extend(scaled_ts)
                    flattened_signal_out.extend(signal_data)

                self._plot_datastore_obj.update_data("flattened_timestamp", flattened_timestamp_out, "out", sensor)
                self._plot_datastore_obj.update_data(f"flattened_{signal.lower()}", flattened_signal_out, "out", sensor)

                # Notify for each signal
                self._data_event_mediator.notify_event(sensor, "CAN_DATA_PREP_UPDATED")

            except Exception as e:
                print(f"[Error] Failed to process signal '{signal}' for sensor '{sensor}': {e}")

    def flatten_radar_data(self, sensor, sig):
        print(f"Flattening radar data for sensor: {sensor}, signal: {sig}")