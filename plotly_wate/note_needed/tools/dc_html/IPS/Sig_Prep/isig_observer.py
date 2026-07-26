from abc import ABC, abstractmethod

class ISigObserver:
    @abstractmethod
    def final_poi_sensor_datapath(self, data): pass
