"""
Description:
This module store signal information in 2D format using dictionary
and it exposes get_data() methods so that other class/module can retrieve
data at any point of application

"""
from IPS.DataStore.idatastore import IDataStore
from collections import defaultdict
import copy

class RadarDataStore(IDataStore):
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
            cls._instance._in_data = defaultdict(dict)
            cls._instance._out_data = defaultdict(dict)
        return cls._instance

    def update_data(self, sensor, signal, data, source):
        if source == 'in':
            self._in_data[sensor][signal] = data
        elif source == 'out':
            self._out_data[sensor][signal] = data

    def get_data(self, sensor, signal, source):
        store = self._in_data if source == 'in' else self._out_data
        return copy.deepcopy(store[sensor][signal])

    def clear_data(self):
        self._in_data.clear()
        self._out_data.clear()
