"""
Description:
This module store signal information in 1D format using dictionary
and it exposes get_data() methods so that other class/module can retrieve
data at any point of application

"""
from IPS.DataStore.idatastore import IDataStore
from collections import defaultdict

class PlotDataStore(IDataStore):
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
            cls._instance._stores = {
                'in': {}, 'out': {}, 'special': {},
                'in_scan_index': defaultdict(dict), 'out_scan_index': defaultdict(dict),
                'mismatch_in': defaultdict(dict), 'mismatch_out': defaultdict(dict),
                'mismatch_scan_index_in': defaultdict(dict), 'mismatch_scan_index_out': defaultdict(dict),
                'missing_scan_index': defaultdict(dict), 'missing_value': defaultdict(dict),
                'additional_scan_index': defaultdict(dict), 'additional_value': defaultdict(dict)
            }
            cls._instance.mismatch_signal_list = {}
            cls._instance.additional_missing_signal_list = {}
        return cls._instance

    def update_data(self, signal, data, source=None, mismatch_idx=None, sensor=None):
        s = self._stores
        if source in ('in', 'out', 'special'):
            s[source][signal] = data
        elif source in ('in_scan_index', 'out_scan_index', 'mismatch_in', 'mismatch_out',
                        'missing_scan_index', 'missing_value', 'additional_scan_index', 'additional_value'):
            s[source][sensor][signal] = data
        elif source == 'mismatch_scan_index_in':
            s[source][sensor][signal] = mismatch_idx
        elif source == 'mismatch_scan_index_out':
            s[source][sensor][signal] = mismatch_idx

    def get_data(self, signal, source=None, sensor=None):
        s = self._stores
        if source in ('in', 'out', 'special'):
            return s[source].get(signal)
        elif source in ('in_scan_index', 'out_scan_index', 'mismatch_value_in', 'mismatch_value_out',
                        'mismatch_scan_index_in', 'mismatch_scan_index_out', 'missing_scan_index',
                        'missing_value', 'additional_scan_index', 'additional_value'):
            key = source.replace('_value', '') if 'value' in source else source
            return s.get(key, {}).get(sensor, {}).get(signal) if source.startswith('mismatch_value') else s[source].get(sensor, {}).get(signal)

    def clear_data(self):
        for k in self._stores:
            self._stores[k].clear()
        self.mismatch_signal_list.clear()
        self.additional_missing_signal_list.clear()
