"""
File Name: radar_hdf_datacollect.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This module collect/parses HDF file using datapath and
dataset mentioned in poi file (metadata->GEN7v2-->poi.py) and stores
data to radar data store
"""



from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.Sig_Prep.isig_observer import ISigObserver
import h5py
import numpy as np
import os
from collections import defaultdict


class RadarHDFDataCollect(IDataCollect, ISigObserver):
    sensor_to_sig = defaultdict(list)

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore,
                 ):
        self._data_event_mediator_obj = event_mediator
        self._radar_datastore_obj = radar_datastore
        self._plot_datastore_obj = plot_datastore

        self._in_file = None
        self._out_file = None
        self._final_poi_sensor_dataset_path = defaultdict(list)

    def clear_data(self):
        RadarHDFDataCollect.sensor_to_sig.clear()
        self._in_file = None
        self._out_file = None
        self._final_poi_sensor_dataset_path.clear()

    def collect_data(self, input_file=None, output_file=None):

        self._in_file = input_file
        self._out_file = output_file

        self.parse_and_collect_data(self._in_file, self._out_file)
        self._data_event_mediator_obj.notify_event(self, "JSON_GEN_DONE")  # JDS--> JSON Data store

    def parse_and_collect_data(self, infile, outfile):
        """
        This function calls hdf parser function for input and output hdf
        after parsing of  input & output hdf, data sets are updated to radar data store.
        once in & out data set updated to radar data store, an event (RDS_UPDATES) is notified.
        radar data preparation module (radar_dataprep.py) will consume the event (RDS_UPDATES)
        parsing will be done sensor by sensor ( in & out)
        example : parse RL (in and out) --> Generate RL Json (Histogram,scatter......)
                  * to do 1* After RL Json Generation done clear data store memory
        """

        for sensor, filtered_dataset_paths in self._final_poi_sensor_dataset_path.items():
            print("----------------------------")
            print("processing sensor", sensor)
            # print("filtered_dataset_paths", filtered_dataset_paths)
            self.parse_hdf_data(str(infile), "in", filtered_dataset_paths)
            self.parse_hdf_data(str(outfile), "out", filtered_dataset_paths)
            self._data_event_mediator_obj.notify_event(sensor, "RDS_UPDATED")  # RDS--> Radar Data store

    def final_poi_sensor_datapath(self, data):
        # print("RadarHDFDataCollect # final_poi_sig ")
        # print(data)
        self._final_poi_sensor_dataset_path = data

        for sensor, filtered_dataset_paths in self._final_poi_sensor_dataset_path.items():
            for dataset_path in filtered_dataset_paths:
                # print("dataset_path", dataset_path)
                sig_name = os.path.basename(dataset_path)
                RadarHDFDataCollect.sensor_to_sig[sensor].append(sig_name)

    def parse_hdf_data(self, hdf_file, sourcetype, filtered_dataset_paths):
        """
        This function traverse to the data path in HDF file and read the dataset
        and store the signal value (2D) in radar data store.
        Before storing it does the following operations
        a. removes duplicate rows in 2D data set
        b. removes zeros from all rows of 2D data set

        """

        with h5py.File(str(hdf_file), 'r') as f:
            for dataset_path in filtered_dataset_paths:
                if str(dataset_path) in f:
                    # print(f"Dataset path {dataset_path} found in the file.")
                    sig_name = os.path.basename(dataset_path)
                    sensor = dataset_path.split('/')[0]
                    data = f[str(dataset_path)][()]  # Read entire dataset (default : NumPy array)

                    # Vectorized values using lambda and scientific formatting
                    mask = np.vectorize(lambda x: 'e' in format(x, '.15e') and (abs(x) < 1e-3 or abs(x) >= 1e4))(
                        data)

                    # Replace exponential values with zero
                    data[mask] = 0

                    # Step 1: Remove zeros from each row
                    non_zero_rows = [row[row != 0.0] for row in data]

                    # Step 2: Find the maximum row length after filtering
                    max_row_len = max(len(row) for row in non_zero_rows)

                    # Step 3: Pad rows to make them same length (with 0 or np.nan)
                    filtered_data = np.array([
                        np.pad(row, (0, max_row_len - len(row)), constant_values=0)
                        for row in non_zero_rows
                    ])

                    # if sig_name != "scan_index":
                    # filtered_data = (np.asarray(filtered_data) * 100).astype(int) / 100.0

                    self._radar_datastore_obj.update_data(sensor, sig_name, filtered_data, sourcetype)

                else:
                    print(f"Dataset {dataset_path} not in file {hdf_file}")

    def consume_event(self, sensor, event):
        pass
