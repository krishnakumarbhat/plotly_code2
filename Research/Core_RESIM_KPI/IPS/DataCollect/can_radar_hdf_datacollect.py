"""
File Name: can_radar_hdf_datacollect.py
Author: A, Sinchana
Email : Sinchana.A@aptiv.com
Description:

"""

from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.Sig_Prep.isig_observer import ISigObserver
import IPS.Metadata.GEN7V2.poi as poi
import h5py
import numpy as np
import os
import re
from collections import defaultdict
import time


class RadarHDFCANDataCollect(IDataCollect, ISigObserver):
    sensor_to_sig = defaultdict(list)
    parse_time = 0


    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore):
        self._data_event_mediator_obj = event_mediator
        self._radar_datastore_obj = radar_datastore
        self._plot_datastore_obj = plot_datastore
        self._final_poi_sensor_dataset_path = defaultdict(list)
        self._in_file = None
        self._out_file = None

    def clear_data(self):
        print(" ")

    def collect_data(self, input_file=None, output_file=None):
        self._in_file = input_file
        self._out_file = output_file
        self.parse_and_collect_data(self._in_file, self._out_file)
        self._data_event_mediator_obj.notify_event(self, "CAN_JSON_GEN_DONE")  # JDS--> JSON Data store

    def parse_and_collect_data(self, infile, outfile):
        for sensor, dataset_paths in self._final_poi_sensor_dataset_path.items():
            print("processing sensor", sensor)
            for path in dataset_paths:
                # Split the full path into components
                path_parts = path.split("/")

                # Extract sensor, stream, and signal with index
                sensor_name = path_parts[0]
                signal_split = path_parts[2]

                # Extract base signal name (e.g., DER_RANGE from DER_RANGE_001)
                signal = "_".join(signal_split.split("_")[:-1])

                # Call parse_hdf_data with full path and extracted components
                # have to pass complete path of range list, azimuth list,

                #start = time.time()
                self.parse_hdf_data(str(infile), "in", dataset_paths, sensor_name)

                self.parse_hdf_data(str(outfile), "out", dataset_paths, sensor_name)
                #end = time.time()
                #RadarHDFCANDataCollect.parse_time += end - start
                #print("RadarHDFCANDataCollect.parse_time:", RadarHDFCANDataCollect.parse_time)

            self._data_event_mediator_obj.notify_event(sensor_name,
                                                       "CAN_DATA_COLLECTION_DONE")  # RDS--> Radar Data store
        #print("FINAL_RadarHDFCANDataCollect.parse_time:", RadarHDFCANDataCollect.parse_time)
   
   

    def sort_final_poi_paths(self):
        """
        Sorts each sensor's dataset paths in self._final_poi_sensor_dataset_path
        based on the numeric suffix in the last part of the path (e.g., DET_RANGE_003).
        """

        def extract_index(path):
            try:
                last_part = path.split("/")[-1]  # e.g., DET_RANGE_003
                index_str = last_part.split("_")[-1]  # '003'
                return int(index_str)
            except (IndexError, ValueError):
                return float('inf')  # Push invalid entries to the end

        for sensor, paths in self._final_poi_sensor_dataset_path.items():
            self._final_poi_sensor_dataset_path[sensor] = sorted(paths, key=extract_index)


    def consume_event(self, sensor, event):
        pass

    def final_poi_sensor_datapath(self, data):
        self._final_poi_sensor_dataset_path = defaultdict(list)

        for sensor, paths in data.items():
            det_range_list = []
            det_rcs_list = []
            det_azimuth_list = []
            det_range_velocity_list = []
            det_elevation_list = []
            det_snr_list = []
            other_list = []

            for path in paths:
                sig_name = os.path.basename(path)  # e.g., DET_RANGE_003
                match = re.match(r"(DET_RANGE|DET_RCS|DET_AZIMUTH|DET_ELEVATION|DET_RANGE_VELOCITY|DET_SNR).*?_\d+$",
                                 sig_name)

                if match:
                    signal_type = match.group(1)
                    if signal_type == "DET_RANGE":
                        det_range_list.append(path)
                    elif signal_type == "DET_RCS":
                        det_rcs_list.append(path)
                    elif signal_type == "DET_AZIMUTH":
                        det_azimuth_list.append(path)
                    elif signal_type == "DET_ELEVATION":
                        det_elevation_list.append(path)
                    elif signal_type == "DET_RANGE_VELOCITY":
                        det_range_velocity_list.append(path)
                    elif signal_type == "DET_SNR":
                        det_snr_list.append(path)
                else:
                    other_list.append(path)

            # Sorting helper
            def extract_index(p):
                try:
                    return int(p.split("_")[-1])
                except:
                    return float('inf')

            # Sort each list
            det_range_list.sort(key=extract_index)
            det_rcs_list.sort(key=extract_index)
            det_azimuth_list.sort(key=extract_index)
            det_elevation_list.sort(key=extract_index)
            det_range_velocity_list.sort(key=extract_index)
            det_snr_list.sort(key=extract_index)
            other_list.sort(key=extract_index)

            # Merge all sorted lists
            merged_sorted_paths = det_range_list + det_rcs_list + det_azimuth_list + det_elevation_list + det_range_velocity_list + det_snr_list + other_list
            self._final_poi_sensor_dataset_path[sensor] = merged_sorted_paths

        # Continue with existing logic
        for sensor, filtered_dataset_paths in self._final_poi_sensor_dataset_path.items():
            for dataset_path in filtered_dataset_paths:
                sig_name = os.path.basename(dataset_path)
                #print(sig_name)
                RadarHDFCANDataCollect.sensor_to_sig[sensor].append(sig_name)

    def parse_hdf_data(self, hdf_file, sourcetype, path, sensor):
        """
        This function traverse to the data path in HDF file and read the dataset
        and store the signal value (2D) in radar data store.
        Before storing it does the following operations
        a. removes duplicate rows in 2D data set
        b. removes zeros from all rows of 2D data set

        """

        signal_groups = defaultdict(list)
        timestamp_dict = {}

        with h5py.File(hdf_file, 'r') as f:
            for dataset_path in path:
                if dataset_path in f:
                    data = f[dataset_path][()]
                    full_signal_name = dataset_path.split("/")[-1]
                    signal_prefix = "_".join(full_signal_name.split("_")[:-1])

                    # Convert to list based on dimensionality
                    if data.ndim == 2:
                        data_list = data.tolist()
                    elif data.ndim == 1:
                        data_list = data.tolist()
                    else:
                        data_list = []

                    signal_groups[signal_prefix].append(data_list)
                    '''
                    parent_group_path = "/".join(dataset_path.split("/")[:-1])
                    if parent_group_path in f:
                        group = f[parent_group_path]
                        for name in group:
                            if "timestamp" in name and isinstance(group[name], h5py.Dataset):
                                ts_data = group[name][()]
                                # Handle FLR separately
                                if "FLR" in name:
                                    match = re.search(r"(FLR_DETECTION_\d{3}_\d{3})", name)
                                    if match:
                                        ts_key = f"timestamp_{match.group(1)}"
                                        timestamp_dict[ts_key] = ts_data.tolist()
                                else:
                                    match = re.search(r"(SRR_[A-Z]+_DETECTION_\d{3}_\d{3})", name)
                                    if match:
                                        ts_key = f"timestamp_{match.group(1)}"
                                        timestamp_dict[ts_key] = ts_data.tolist()
                    '''
                    parent_group_path = "/".join(dataset_path.split("/")[:-1])
                    # group = hdf_file[parent_group_path]
                    stream_name = parent_group_path.split("/")[-1]  # e.g., STREAM1
                    #for name in group:
                    #if "timestamp" in name and isinstance(group[name], h5py.Dataset):

                    final_name = parent_group_path+"/"+f"timestamp_{stream_name}"
                    ts_data = f[final_name][()]
                    timestamp_dict[final_name] = ts_data.tolist()

        for sig_name, grouped_data in signal_groups.items():
            self._radar_datastore_obj.update_data(sensor, sig_name, grouped_data, sourcetype)

            # Update radar datastore with timestamp dictionary
        if timestamp_dict:
            self._radar_datastore_obj.update_data(sensor, "timestamp_data", timestamp_dict, sourcetype)


