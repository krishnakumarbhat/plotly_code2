from abc import ABC, abstractmethod

class IDataCollect:
    @abstractmethod
    def collect_data(self, input_file, output_file): pass
