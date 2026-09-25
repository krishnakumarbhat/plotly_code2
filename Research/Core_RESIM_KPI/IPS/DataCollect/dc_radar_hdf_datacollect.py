"""
File Name: dc_radar_hdf_datacollect.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This module collect/parses DC HDF file using datapath and
dataset mentioned in poi file or external text file and stores
data to radar data store

"""



from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.Sig_Prep.isig_observer import ISigObserver
import IPS.Metadata.GEN7V2.poi as poi
import h5py
import numpy as np
import os
from collections import defaultdict
from IPS.RepGen.dc_json_to_html_convert import DCJsonToHtmlConvertor
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DashManager.report_dash import ReportDash
from IPS.Utilities.hdf_validator import HDFValidator


class RadarHDFDCDataCollect(IDataCollect, ISigObserver):

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore,
                 sig_filter: SignalFilter,
                 report_dash: ReportDash
                 ):
        self._data_event_mediator_obj = event_mediator
        self._radar_datastore_obj = radar_datastore
        self._plot_datastore_obj = plot_datastore
        self._final_poi_sensor_dataset_path = defaultdict(list)
        # self.dc_hdf_datapath = self.extract_datapaths(poi.poi_data_DC)
        self.dc_hdf_datapath = self.extract_datapaths(sig_filter.data)
        self.tracker_datapath = None
        self.detection_datapath = None
        self._in_file = None
        self._out_file = None
        self.sensor_index = {
            "0": "FL",
            "1": "FR",
            "2": "RL",
            "3": "RR",
            "4": "FC"
        }
        self._detection_signal_count = 0
        self._other_signal_count = 0
        self._other_signal_count_out = 0
        self._streamcheck_flag = True
        self._prevstream = None
        self._report_dash = report_dash
        # self._report_dash.Version_metadata = ["NotAvailable"] * 6
        ReportDash.Version_metadata = ["NotAvailable"] * 6

    def clear_data(self):

        self._in_file = None
        self._out_file = None
        self._final_poi_sensor_dataset_path.clear()
        self._detection_signal_count = 0
        self._other_signal_count = 0
        self._other_signal_count_out = 0
        self._streamcheck_flag = True
        self._prevstream = None
        DCJsonToHtmlConvertor.sensor_list.clear()
        DCJsonToHtmlConvertor.stream_list.clear()
        DCJsonToHtmlConvertor.stream_signals.clear()
        DCJsonToHtmlConvertor.sensor_stream.clear()
        # ReportDash.Version_metadata.clear()

    def collect_data(self, input_file=None, output_file=None):

        self._in_file = input_file
        self._out_file = output_file

        self.parse_and_collect_data(self._in_file, self._out_file)
        self._data_event_mediator_obj.notify_event(self, "DC_JSON_GEN_DONE")

    def parse_and_collect_data(self, infile, outfile):
        """

        """
        # print("printing essentials")
        streams = self.get_streams()
        # print("streams", streams)

        for stream in streams:
            signals = self.get_signals(stream)
            # print("signals", signals)

            for sig in signals:
                plottype = self.get_plot_types(sig)
                unit = self.get_unit(sig)
                # print("plottype", plottype)
                # print("unit", unit)

        scan_index_datapath = "01_Scan_Index/scan_index"
        self.parse_scan_index(str(infile), "in", scan_index_datapath)
        self.parse_scan_index(str(outfile), "out", scan_index_datapath)
        # print("self.dc_hdf_datapath",self.dc_hdf_datapath)
        self.parse_hdf_data(str(infile), "in", self.dc_hdf_datapath)
        self.parse_hdf_data(str(outfile), "out", self.dc_hdf_datapath)

        self._data_event_mediator_obj.notify_event("Detections", "DC_DET_RDS_UPDATED")

    def parse_scan_index(self, hdf_file, datasource, datapath):
        # Validate HDF file before processing
        is_valid, error_msg = HDFValidator.validate_hdf_file(hdf_file)
        if not is_valid:
            print(f"ERROR: HDF file validation failed for {hdf_file}: {error_msg}")
            return

        with h5py.File(str(hdf_file), 'r') as f:

            if str(datapath) in f:
                data = f[str(datapath)][()]  # Read entire dataset (default : NumPy array)

                # Don't apply np.unique - preserve all scan_index values even if duplicated
                # The detection data has one entry per scan, so scan_index should too
                # If scan_index has duplicates, they represent distinct scans with same index value

                self._radar_datastore_obj.update_data("DC", "scan_index", data, datasource)

            else:
                print(f"Dataset {datapath} not in file {hdf_file}")

    def consume_event(self, sensor, event):
        pass

    def final_poi_sensor_datapath(self, data):
        pass

    def parse_hdf_featurefunction_data(self, hdf_file, sourcetype, filtered_dataset_paths):
        """
        This function traverse to the data path in HDF file and read the dataset
        and store the signal value (2D) in radar data store.
        Before storing it does the following operations
        a. removes duplicate rows in 2D data set
        b. removes zeros from all rows of 2D data set

        """
        # Validate HDF file before processing
        is_valid, error_msg = HDFValidator.validate_hdf_file(hdf_file)
        if not is_valid:
            print(f"ERROR: HDF file validation failed for {hdf_file}: {error_msg}")
            return

        with h5py.File(str(hdf_file), 'r') as f:
            for dataset_path in filtered_dataset_paths:
                if str(dataset_path) in f:
                    # print(f"Dataset path {dataset_path} found in the file.")
                    sig_name = os.path.basename(dataset_path)
                    sensor = "DCTracks"
                    data = f[str(dataset_path)][()]  # Read entire dataset (default : NumPy array)

                    # Step 1: Remove zeros from each row
                    non_zero_rows = [row[row != 0.0] for row in data]

                    # Step 2: Find the maximum row length after filtering
                    max_row_len = max(len(row) for row in non_zero_rows)

                    # Step 3: Pad rows to make them same length (with 0 or np.nan)
                    filtered_data = np.array([
                        np.pad(row, (0, max_row_len - len(row)), constant_values=0)
                        for row in non_zero_rows
                    ])

                    self._radar_datastore_obj.update_data(sensor, sig_name, filtered_data, sourcetype)

                else:
                    print(f"Dataset {dataset_path} not in file {hdf_file}")

            data = f[str("Scan_Index/scan_index")][()]
            self._radar_datastore_obj.update_data("DCTracks", "scan_index", data, sourcetype)

    def parse_hdf_data(self, hdf_file, sourcetype, filtered_dataset_paths):
        """


        """
        # Validate HDF file before processing
        is_valid, error_msg = HDFValidator.validate_hdf_file(hdf_file)
        if not is_valid:
            print(f"ERROR: HDF file validation failed for {hdf_file}: {error_msg}")
            return

        with h5py.File(str(hdf_file), 'r') as f:
            for dataset_path in filtered_dataset_paths:
                if str(dataset_path) in f:
                    # print(f"Dataset path {dataset_path} found in the file.")
                    if dataset_path.startswith("02_Input_DC_Data/Detections/Detection_Info"):
                        """
                          under Detection_Info group , all sensor data are stacked to same
                          data set(range,rr,azimuth....) with sensor index , hence looping based on sensor index
                          to extract the respective sensor data
                          sensor_index = {"0": "FL", "1": "FR","1": "FR",  "2": "RL","3": "RR", "4": "FC",
                          "5": "RC", "6": "BL", "7" : "BR"
                        """
                        # print(f"Dataset path {dataset_path} found in the file.")
                        if sourcetype == "in":
                            self._detection_signal_count = self._detection_signal_count + 1
                            # print("self._detection_signal_count",self._detection_signal_count)
                        for index in range(0, 5):
                            sig_name = os.path.basename(dataset_path)
                            sensor = self.sensor_index[str(index)]
                            """
                             stream and sensor info are updated to JSONto HTML Convertor class
                             below information are required while report generation.
                             WEBGL plotly library can only render 8 interactive plots per html page.
                             If we have signals more than 8 for plotting we are grouping signals
                             of particular stream as Detections(first 8 signal) and Detections1( next 8 signal)
                             and so....
                            """
                            if sourcetype == "in":
                                suffix_index = (self._detection_signal_count - 1) // 8
                                suffix = "" if suffix_index == 0 else str(suffix_index)
                                stream = "Detections" + suffix
                                DCJsonToHtmlConvertor.stream_list.append(stream)
                                DCJsonToHtmlConvertor.sensor_list.append(sensor)
                                DCJsonToHtmlConvertor.stream_signals[stream].add(sig_name)
                                DCJsonToHtmlConvertor.sensor_stream[sensor].add(stream)

                            detection_signal_list = ['range', 'range_rate', 'snr', 'amplitude', 'azimuth', 'elevation',
                                                     'azimuth_confidence',
                                                     ]
                            if sig_name in detection_signal_list:
                                data_3d = f[str(dataset_path)][()]  # Read entire dataset (default : NumPy array)
                                data_3d_per_sensor = data_3d[:, :, index]  # Extract each sensor's data

                                # read the complete valid detection flag as data set
                                valid_detection_flag_3d = f['02_Input_DC_Data/Detections/Detection_Info/valid'][
                                    ()]
                                # extract the valid flag corresponding to specific sensor
                                valid_detection_flag_per_sensor = valid_detection_flag_3d[:, :, index]
                                # retain only valid flag data
                                if data_3d_per_sensor.shape == valid_detection_flag_per_sensor.shape:
                                    data = np.where(valid_detection_flag_per_sensor == 1, data_3d_per_sensor, 0)
                                else:
                                    print(sig_name, "Shape mismatch between signal array and valid array")

                                # check data is 1D or 2D
                                '''
                                if data.ndim == 1:  # 1D
                                    unique_data, idx = np.unique(data, return_index=True)
                                    data = unique_data[np.argsort(idx)]

                                elif data.ndim == 2:  # 2D
                                    unique_data, idx = np.unique(data, axis=0, return_index=True)
                                    data = unique_data[np.argsort(idx)]
                                '''

                                # Step 1: Remove zeros from each row
                                non_zero_rows = [row[row != 0.0] for row in data]

                                # Step 2: Find the maximum row length after filtering
                                max_row_len = max(len(row) for row in non_zero_rows)

                                # Step 3: Pad rows to make them same length (with 0 or np.nan)
                                filtered_data = np.array([
                                    np.pad(row, (0, max_row_len - len(row)), constant_values=0)
                                    for row in non_zero_rows
                                ])

                                self._radar_datastore_obj.update_data(sensor, sig_name, filtered_data, sourcetype)
                            else:
                                data = f[str(dataset_path)][()]  # Read entire dataset (default : NumPy array)
                                '''
                                # check data is 1D or 2D
                                if data.ndim == 1:  # 1D
                                    unique_data, idx = np.unique(data, return_index=True)
                                    data = unique_data[np.argsort(idx)]

                                elif data.ndim == 2:  # 2D
                                    unique_data, idx = np.unique(data, axis=0, return_index=True)
                                    data = unique_data[np.argsort(idx)]
                                '''
                                # Step 1: Remove zeros from each row
                                non_zero_rows = [row[row != 0.0] for row in data]

                                # Step 2: Find the maximum row length after filtering
                                max_row_len = max(len(row) for row in non_zero_rows)

                                # Step 3: Pad rows to make them same length (with 0 or np.nan)
                                filtered_data = np.array([
                                    np.pad(row, (0, max_row_len - len(row)), constant_values=0)
                                    for row in non_zero_rows
                                ])

                                self._radar_datastore_obj.update_data(sensor, sig_name, filtered_data, sourcetype)
                    elif dataset_path.startswith("02_Input_DC_Data/Detections/Detection_Header"):
                        """
                          Count is reported directly per scan per sensor (2D: scan x sensor index),
                          so it can be read as-is without aggregating a per-detection flag.
                        """
                        if sourcetype == "in":
                            self._detection_signal_count = self._detection_signal_count + 1
                        for index in range(0, 5):
                            sig_name = os.path.basename(dataset_path)
                            sensor = self.sensor_index[str(index)]

                            if sourcetype == "in":
                                suffix_index = (self._detection_signal_count - 1) // 8
                                suffix = "" if suffix_index == 0 else str(suffix_index)
                                stream = "Detections" + suffix
                                DCJsonToHtmlConvertor.stream_list.append(stream)
                                DCJsonToHtmlConvertor.sensor_list.append(sensor)
                                DCJsonToHtmlConvertor.stream_signals[stream].add(sig_name)
                                DCJsonToHtmlConvertor.sensor_stream[sensor].add(stream)

                            data_2d = f[str(dataset_path)][()]  # Read entire dataset (default : NumPy array)
                            count_per_sensor = data_2d[:, index].reshape(-1, 1)
                            self._radar_datastore_obj.update_data(sensor, sig_name, count_per_sensor, sourcetype)
                    elif dataset_path.startswith("00_metadata"):
                        if sourcetype == "in":

                            sig_name = os.path.basename(dataset_path)
                            if sig_name == "Created_datetime":
                                meta_data = f[str(dataset_path)][()]
                                ReportDash.Version_metadata[0] = meta_data
                            if sig_name == "DC_version":
                                meta_data = f[str(dataset_path)][()]
                                ReportDash.Version_metadata[1] = meta_data
                            if sig_name == "OCG_version":
                                meta_data = f[str(dataset_path)][()]
                                ReportDash.Version_metadata[2] = meta_data
                            if sig_name == "OLP_version":
                                meta_data = f[str(dataset_path)][()]
                                ReportDash.Version_metadata[3] = meta_data
                            if sig_name == "SFL_version":
                                meta_data = f[str(dataset_path)][()]
                                ReportDash.Version_metadata[4] = meta_data
                            if sig_name == "Tracker_version":
                                meta_data = f[str(dataset_path)][()]
                                ReportDash.Version_metadata[5] = meta_data
                    else:
                        """
                          below logic works for data group other than detections
                          in other data group (tracks, OLP, vehicle Info, FeatureFunctions) multiple sensor data
                          are not stacked to same dataset, hence looping for sensor index not done here
                        """

                        if dataset_path.startswith("03_TrackerInfo/Tracker_Output/AllObjects"):
                            print(dataset_path)
                            if sig_name != "num_elements":  
                                try:
                                    num_elements_path = "03_TrackerInfo/Tracker_Output/AllObjects/num_elements"
                                    if num_elements_path in f:
                                        num_elements_data = f[num_elements_path][()]
                                        self._radar_datastore_obj.update_data(sensor, "allobjects_num_elements", num_elements_data, sourcetype)
                                except Exception as e:
                                    print(f"Could not extract num_elements for AllObjects: {e}")

                        parts = dataset_path.split('/')
                        if len(parts) > 2:
                            stream = parts[-2]

                        else:
                            stream = parts[0]

                        if self._streamcheck_flag:
                            self._prevstream = stream
                            self._streamcheck_flag = False

                        if self._prevstream != stream:
                            self._other_signal_count = 0
                            self._other_signal_count_out = 0
                            self._streamcheck_flag = True

                        '''
                          we are assigning stream to sensor, as sensor info 
                          not available for other data groups. But we are in need of sensor info
                          while creating KPI Dashboard
                        '''

                        sig_name = os.path.basename(dataset_path)
                        sensor = "DC"

                        if sourcetype == "in":
                            self._other_signal_count = self._other_signal_count + 1
                            # Compute suffix based on blocks of 8
                            '''
                            // operator is integer division (also called floor division)
                            We need whole number blocks (0, 1, 2, 3…) to decide the suffix, hence
                            using //
                             Count 1–8 → (count-1)//8 = 0 → suffix ""
                             Count 9–16 → (count-1)//8 = 1 → suffix "1"
                             Count 17–24 → (count-1)//8 = 2 → suffix "2"
                             Count 25–32 → (count-1)//8 = 3 → suffix "3"
                            '''
                            suffix_index = (self._other_signal_count - 1) // 8
                            suffix = "" if suffix_index == 0 else str(suffix_index)
                            stream = stream + suffix

                            DCJsonToHtmlConvertor.sensor_list.append(sensor)
                            DCJsonToHtmlConvertor.stream_list.append(stream)
                            DCJsonToHtmlConvertor.stream_signals[stream].add(sig_name)
                            DCJsonToHtmlConvertor.sensor_stream[sensor].add(stream)

                        if sourcetype == "out":
                            self._other_signal_count_out = self._other_signal_count_out + 1
                            # Compute suffix based on blocks of 8
                            '''
                            // operator is integer division (also called floor division)
                            We need whole number blocks (0, 1, 2, 3…) to decide the suffix, hence
                            using //
                             Count 1–8 → (count-1)//8 = 0 → suffix ""
                             Count 9–16 → (count-1)//8 = 1 → suffix "1"
                             Count 17–24 → (count-1)//8 = 2 → suffix "2"
                             Count 25–32 → (count-1)//8 = 3 → suffix "3"
                            '''
                            suffix_index = (self._other_signal_count_out - 1) // 8
                            suffix = "" if suffix_index == 0 else str(suffix_index)
                            stream = stream + suffix

                            DCJsonToHtmlConvertor.sensor_list.append(sensor)
                            DCJsonToHtmlConvertor.stream_list.append(stream)
                            DCJsonToHtmlConvertor.stream_signals[stream].add(sig_name)
                            DCJsonToHtmlConvertor.sensor_stream[sensor].add(stream)

                        data = f[str(dataset_path)][()]

                        # Handle different data dimensions
                        if data.ndim == 1:
                            # 1D data - reshape for consistency
                            filtered_data = data.reshape(-1, 1)
                        elif data.ndim == 2:
                            # For object_class, 0 is a valid value (OBJ_CLASS_UNKNOWN), so skip zero removal
                            if sig_name == "object_class":
                                filtered_data = data.astype(int)
                            else:
                                # 2D data - remove zeros and pad
                                non_zero_rows = [row[row != 0.0] for row in data]
                                if non_zero_rows and any(len(row) > 0 for row in non_zero_rows):
                                    max_row_len = max(len(row) for row in non_zero_rows)
                                    if max_row_len == 0:
                                        max_row_len = 1
                                    filtered_data = np.array([
                                        np.pad(row, (0, max_row_len - len(row)), constant_values=0)
                                        for row in non_zero_rows
                                    ])
                                else:
                                    filtered_data = data
                        else:
                            filtered_data = data

                        # Preserve integer type for object_class signal
                        if sig_name == "object_class":
                            filtered_data = filtered_data.astype(int)

                        if sig_name == "f_moveable" or sig_name == "f_stationary":
                            # Below code calculates total no of moving and stationary track objects
                            data_movable_station_tracks = f[str(dataset_path)][()]
                            no_mov_sta_tracks_per_scan_index = np.sum(data_movable_station_tracks == 1, axis=1)
                            data = no_mov_sta_tracks_per_scan_index.reshape(-1, 1)
                            self._radar_datastore_obj.update_data(sensor, sig_name, data, sourcetype)

                        self._radar_datastore_obj.update_data(sensor, sig_name, filtered_data, sourcetype)


                else:
                    print(f"Dataset {dataset_path} not in file {hdf_file}")

    def get_streams(self):
        return poi.poi_data_DC.get("streams", [])

    def get_signals(self, stream):
        return [signal["name"] for signal in poi.poi_data_DC.get(stream, [])]

    def extract_datapaths(self, poi_data: dict) -> list:
        datapaths = []
        for stream in poi_data.get("streams", []):
            for signal in poi_data.get(stream, []):
                datapaths.append(f"{stream}/{signal['name']}")
        return datapaths

    def get_plot_types(self, signal_name: str) -> list:
        for stream in poi.poi_data_DC.get("streams", []):
            for signal in poi.poi_data_DC.get(stream, []):
                if signal["name"] == signal_name:
                    return signal["plots"]
        return []

    def get_unit(self, signal_name: str) -> str:
        for stream in poi.poi_data_DC.get("streams", []):
            for signal in poi.poi_data_DC.get(stream, []):
                if signal.get("name") == signal_name:
                    unit = signal.get("unit")
                    return unit if unit is not None else ""
        return ""
