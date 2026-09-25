"""
File Name: icollectdata.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This is interface file where any class wish to
collect HDF data should inherit this interface class (IDataCollect)
and override the abstract methods
"""

from abc import ABC, abstractmethod


class IDataCollect:
    @abstractmethod
    def collect_data(self, input_file, output_file): pass
