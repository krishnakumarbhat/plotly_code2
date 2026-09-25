"""
File Name: Json_datastore.py
Author: Bharanidharan Subramaniam
Email : Bharanidharan.s@aptiv.com
Description:
This module store Jsonpath information  using dictionary
and it exposes get_data() methods so that other class/module can retrieve
data at any point of application

"""

from IPS.DataStore.idatastore import IDataStore
from collections import defaultdict


class JSONDataStore(IDataStore):
    _jsonstore_instance = None

    def __new__(cls):
        if cls._jsonstore_instance is None:
            cls._jsonstore_instance = super(JSONDataStore, cls).__new__(cls)
            cls._jsonstore_instance._json_info = defaultdict(list)  # Initialize json data store by empty values
            cls._jsonstore_instance._json_histogram_info = defaultdict(list)
            cls._jsonstore_instance._json_mismatch_info = defaultdict(list)
            cls._jsonstore_instance._json_additional_missing_info = defaultdict(list)
        return cls._jsonstore_instance

    def update_data(self, sensor, jsonpath, data_source=None):
        #print("JSONDataStore::update_data")
        if data_source == "scatter":
            self._json_info[sensor].append(jsonpath)
        if data_source == "Histogram":
            self._json_histogram_info[sensor].append(jsonpath)
        if data_source == "mismatch":
            self._json_mismatch_info[sensor].append(jsonpath)
        if data_source == "additionalmissing":
            self._json_additional_missing_info[sensor].append(jsonpath)

    def get_data(self, sensor, data_source=None):
        #print("JSONDataStore::get_data")
        if data_source == "scatter":
            return self._json_info[sensor]
        if data_source == "Histogram":
            return self._json_histogram_info[sensor]
        if data_source == "mismatch":
            return self._json_mismatch_info[sensor]
        if data_source == "additionalmissing":
            return self._json_additional_missing_info[sensor]

    def clear_data(self):
        self._json_info.clear()
        self._json_histogram_info.clear()
        self._json_mismatch_info.clear()
        self._json_additional_missing_info.clear()
