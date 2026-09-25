"""
File Name: plot_datastore.py
Author: Bharanidharan Subramaniam
Email : Bharanidharan.s@aptiv.com
Description:
This module store signal information in 1D format using dictionary
and it exposes get_data() methods so that other class/module can retrieve
data at any point of application

"""


from IPS.DataStore.idatastore import IDataStore
from collections import defaultdict
import copy


class PlotDataStore(IDataStore):
    _plot_store_instance = None

    def __new__(cls):
        if cls._plot_store_instance is None:
            cls._plot_store_instance = super(PlotDataStore, cls).__new__(cls)
            cls._plot_store_instance._plot_in_data = {}
            cls._plot_store_instance._plot_out_data = {}
            cls._plot_store_instance._in_scan_index = defaultdict(dict)
            cls._plot_store_instance._out_scan_index = defaultdict(dict)
            cls._plot_store_instance._mismatch_plot_in_data = defaultdict(dict)
            cls._plot_store_instance._mismatch_plot_out_data = defaultdict(dict)
            cls._plot_store_instance._mismatch_plot_in_scan_index = defaultdict(dict)
            cls._plot_store_instance._mismatch_plot_out_scan_index = defaultdict(dict)
            cls._plot_store_instance._missing_plot_data = defaultdict(dict)
            cls._plot_store_instance._missing_plot_scan_index = defaultdict(dict)
            cls._plot_store_instance._additional_plot_data = defaultdict(dict)
            cls._plot_store_instance._additional_plot_scan_index = defaultdict(dict)
            cls._plot_store_instance.mismatch_signal_list = {}
            cls._plot_store_instance.additional_missing_signal_list = {}

            cls._plot_store_instance._plot_special_data = {}
        return cls._plot_store_instance

    def update_data(self, signal, signal_numpy_data, data_source=None, mismatch_scan_index=None, sensor=None):
        # print(f"PlotDataStore::update_data {signal}")

        if data_source == "in":
            self._plot_in_data[signal] = signal_numpy_data
        elif data_source == "out":
            self._plot_out_data[signal] = signal_numpy_data

        elif data_source == "in_scan_index":
            self._in_scan_index[sensor][signal] = signal_numpy_data

        elif data_source == "out_scan_index":
            self._out_scan_index[sensor][signal] = signal_numpy_data

        elif data_source == "special":
            self._plot_special_data[signal] = signal_numpy_data

        elif data_source == "mismatch_in":
            self._mismatch_plot_in_data[sensor][signal] = signal_numpy_data

        elif data_source == "mismatch_out":
            self._mismatch_plot_out_data[sensor][signal] = signal_numpy_data

        elif data_source == "mismatch_scan_index_in":
            self._mismatch_plot_in_scan_index[sensor][signal] = mismatch_scan_index

        elif data_source == "mismatch_scan_index_out":
            self._mismatch_plot_out_scan_index[sensor][signal] = mismatch_scan_index

        elif data_source == "missing_scan_index":  #
            self._missing_plot_scan_index[sensor][signal] = signal_numpy_data

        elif data_source == "missing_value":  #
            self._missing_plot_data[sensor][signal] = signal_numpy_data

        elif data_source == "additional_scan_index":  #
            self._additional_plot_scan_index[sensor][signal] = signal_numpy_data

        elif data_source == "additional_value":  #
            self._additional_plot_data[sensor][signal] = signal_numpy_data

        # print(self._plot_in_data[signal])

    def get_data(self, signal, data_source=None, sensor=None):
        # print("PlotDataStore::get_data")
        if data_source == "in":
            # print(self._plot_in_data[signal])
            if signal in self._plot_in_data:
               return self._plot_in_data[signal]
            else:
                return None
        elif data_source == "out":
            # print(self._plot_in_data[signal])
            if signal in self._plot_out_data:
               return self._plot_out_data[signal]
            else:
                return None

        elif data_source == "in_scan_index":

            if signal in self._in_scan_index.get(sensor, {}):
                return self._in_scan_index[sensor][signal]
            else:
                return None

            # return self._in_scan_index[sensor][signal]

        elif data_source == "out_scan_index":
            if signal in self._out_scan_index.get(sensor, {}):
                return self._out_scan_index[sensor][signal]
            else:
                return None

        elif data_source == "special":
            return self._plot_special_data[signal]

        elif data_source == "mismatch_value_in":
            if signal in self._mismatch_plot_in_data.get(sensor, {}):
                return self._mismatch_plot_in_data[sensor][signal]
            else:
                return None

        elif data_source == "mismatch_value_out":
            if signal in self._mismatch_plot_out_data.get(sensor, {}):
                return self._mismatch_plot_out_data[sensor][signal]
            else:
                return None

        elif data_source == "mismatch_scan_index_in":
            if signal in self._mismatch_plot_in_scan_index.get(sensor, {}):
                return self._mismatch_plot_in_scan_index[sensor][signal]
            else:
                return None

        elif data_source == "mismatch_scan_index_out":
            if signal in self._mismatch_plot_out_scan_index.get(sensor, {}):
                return self._mismatch_plot_out_scan_index[sensor][signal]
            else:
                return None

        elif data_source == "missing_scan_index":  #
            if signal in self._missing_plot_scan_index.get(sensor, {}):
                return self._missing_plot_scan_index[sensor][signal]
            else:
                return None

        elif data_source == "missing_value":  #
            if signal in self._missing_plot_data.get(sensor, {}):
                return self._missing_plot_data[sensor][signal]
            else:
                return None

        elif data_source == "additional_scan_index":  #
            if signal in self._additional_plot_scan_index.get(sensor, {}):
                return self._additional_plot_scan_index[sensor][signal]
            else:
                return None

        elif data_source == "additional_value":  #
            if signal in self._additional_plot_data.get(sensor, {}):
                return self._additional_plot_data[sensor][signal]
            else:
                return None

    def clear_data(self):
        self._plot_in_data.clear()
        self._plot_out_data.clear()
        self._mismatch_plot_in_data.clear()
        self._mismatch_plot_in_scan_index.clear()
        self._mismatch_plot_out_data.clear()
        self._mismatch_plot_out_scan_index.clear()
        self._plot_special_data.clear()
        self._missing_plot_data.clear()
        self._missing_plot_scan_index.clear()
        self._additional_plot_data.clear()
        self._additional_plot_scan_index.clear()
        self.mismatch_signal_list.clear()
        self.additional_missing_signal_list.clear()
