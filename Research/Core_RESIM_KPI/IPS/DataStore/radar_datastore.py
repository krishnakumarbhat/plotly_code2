"""
File Name: radar_datastore.py
Author: Bharanidharan Subramaniam
Email : Bharanidharan.s@aptiv.com
Description:
This module store signal information in 2D format using dictionary
and it exposes get_data() methods so that other class/module can retrieve
data at any point of application

"""



from IPS.DataStore.idatastore import IDataStore
from collections import defaultdict
import copy


class RadarDataStore(IDataStore):
    _radar_store_instance = None

    def __new__(cls):
        if cls._radar_store_instance is None:
            cls._radar_store_instance = super(RadarDataStore, cls).__new__(cls)
            cls._radar_store_instance._input_radar_data = defaultdict(dict)
            cls._radar_store_instance._output_radar_data = defaultdict(dict)
        return cls._radar_store_instance

    def update_data(self, sensor, signal, signal_data, data_source):

        # print(f"RadarDataStore::update_data for signal {signal}")
        # print("-------------------")

        if data_source == "in":
            self._input_radar_data[sensor][signal] = signal_data
        elif data_source == "out":
            self._output_radar_data[sensor][signal] = signal_data

    def get_data(self, sensor, signal, data_source):
        # print("RadarDataStore::get_data")
        # print("sensor",sensor)
        # print("signal",signal)
        if data_source == "in":
            return copy.deepcopy(self._input_radar_data[sensor][signal])
        elif data_source == "out":
            return copy.deepcopy(self._output_radar_data[sensor][signal])

    def clear_data(self):
        self._input_radar_data.clear()
        self._output_radar_data.clear()
