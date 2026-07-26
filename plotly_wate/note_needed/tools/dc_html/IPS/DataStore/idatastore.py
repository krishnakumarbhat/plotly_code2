"""
Description:
This is interface file where any class wish to
store  HDF data should inherit this interface class (IDataCollect)
and override the abstract methods
"""
from abc import ABC, abstractmethod

class IDataStore(ABC):
    @abstractmethod
    def get_data(self, signal, data_source): pass
    @abstractmethod
    def update_data(self, signal, data_source, data): pass
