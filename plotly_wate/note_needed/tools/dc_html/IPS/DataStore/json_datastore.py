"""
Description:
This module store Jsonpath information  using dictionary
and it exposes get_data() methods so that other class/module can retrieve
data at any point of application

"""
from IPS.DataStore.idatastore import IDataStore
from collections import defaultdict

class JSONDataStore(IDataStore):
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
            cls._instance._scatter = defaultdict(list)
            cls._instance._histogram = defaultdict(list)
            cls._instance._mismatch = defaultdict(list)
            cls._instance._addmiss = defaultdict(list)
        return cls._instance

    def update_data(self, sensor, jsonpath, source=None):
        store = {'scatter': self._scatter, 'Histogram': self._histogram,
                 'mismatch': self._mismatch, 'additionalmissing': self._addmiss}.get(source)
        if store is not None:
            store[sensor].append(jsonpath)

    def get_data(self, sensor, source=None):
        return {'scatter': self._scatter, 'Histogram': self._histogram,
                'mismatch': self._mismatch, 'additionalmissing': self._addmiss}.get(source, {})[sensor]

    def clear_data(self):
        self._scatter.clear()
        self._histogram.clear()
        self._mismatch.clear()
        self._addmiss.clear()
