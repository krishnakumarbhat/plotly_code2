"""
File Name: dc_dataprep.py
Author: Bharanidharan Subramaniam
Email : Bharanidharan.s@aptiv.com
Description:
This module uses Radar Data Store(RDS) and prepare the data for
plotting. RDS have signal data in 2D format.
In this module convert 2D to 1D.
It also scales up scan index as per dimension of 2D data
Example : Range Signal 2D ( rows:200, column : 250)
Each row    ---> corresponds to Scan index
Each Column ---> corresponds to data

"""

from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.DashManager.report_dash import ReportDash
from IPS.RepGen.dc_json_to_html_convert import DCJsonToHtmlConvertor
import numpy as np


class DCPlotDataPreparation:

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore,
                 jsonpath_datastore: IDataStore,
                 sig_filter: SignalFilter,
                 dc_hdf_datacollector: IDataCollect):
        self._data_event_mediator_obj = event_mediator
        self._plot_datastore_obj = plot_datastore
        self._radar_datastore_obj = radar_datastore
        self._jsonpath_datastore_obj = jsonpath_datastore
        self._sig_filter_obj = sig_filter
        self._dc_hdf_datacollector = dc_hdf_datacollector

    def clear_data(self):
        pass

    def consume_event(self, sensor, event):

        if event == "DC_DET_RDS_UPDATED":
            # dc_stream = self._dc_hdf_datacollector.get_streams()
            dc_stream = set(DCJsonToHtmlConvertor.stream_list)
            for stream in dc_stream:

                if stream == "Detections" or stream == "Detections1" or stream == "Detections2":

                    dc_detection_signal_list = DCJsonToHtmlConvertor.stream_signals.get(stream, set())
                    for sig in dc_detection_signal_list:

                        for sensor in ["FL", "FR", "RR", "RL", "FC"]:
                            self.flatten_radar_data(sensor, sig)
                            if sig in ["range", "range_rate", "azimuth", "elevation", "snr"]:
                                self.compare_vehicle_resim_datapoints(sig, sensor)

                            self._data_event_mediator_obj.notify_event(sensor + "#" + str(stream) + "#" + sig,
                                                                       "JDS_UPDATED")

                else:
                    # For other stream sensor is stream name

                    dc_signal_list = DCJsonToHtmlConvertor.stream_signals.get(stream, set())
                    for sig in dc_signal_list:
                        # for sensor in stream:
                        sensor = "DC"

                        self.flatten_radar_data(sensor, sig)
                        if sig in ["vcs_pos_x", "vcs_pos_y", "vcs_vel_x", "vcs_vel_y", "vcs_accel_x", "vcs_accel_y"]:
                            self.compare_vehicle_resim_datapoints(sig, sensor)
                        self._data_event_mediator_obj.notify_event(sensor + "#" + str(stream) + "#" + sig,
                                                                   "JDS_UPDATED")

    def flatten_radar_data(self, sensor, sig):
        """
        This function gets radar data(2D) of signal from radar data store and takes only two decimal
        places and getting the dimension of signal and update signal dimension to plot data store.
        updated signal dimension is used as scaling factor for scan index duplication ( required for plotting)

        then convert 2D radar data to 1D data and update to plot data store.

        signals like phi and theta are converted from radian to degree and updated to plot data store.
        From plot data store we can retrieve to plot scatter plots
        """
        
        # Define detection signals once at the start of the function
        detection_signals = {"range", "range_rate", "azimuth", "elevation", "snr", "amplitude", "std_rcs", "azimuth_confidence", "elevation_confidence", "Count"}
        is_detection_signal = sig in detection_signals
        
        # Define AllObjects signals (from 03_TrackerInfo/Tracker_Output/AllObjects)
        allobjects_signals = {"vcs_xposn", "vcs_yposn", "vcs_xvel", "vcs_yvel", "vcs_xaccel", "vcs_yaccel", "vcs_heading", "object_class"}
        is_allobjects_signal = sig in allobjects_signals

        try:
            sig_data = self._radar_datastore_obj.get_data(sensor, sig, "in")
            # For confidence values and object_class (categorical), keep as integers
            # For other signals - round to 2 decimal places
            if sig == "azimuth_confidence" or sig == "elevation_confidence" or sig == "object_class":
                rounded_sig_data = sig_data.astype(int)
            else:
                rounded_sig_data = (sig_data * 100).astype(int) / 100.0
            
            if is_allobjects_signal:
                # Slice based on num_elements field (0 to num_elements-1 per scan)
                try:
                    allobjects_num_elements = self._radar_datastore_obj.get_data(sensor, "allobjects_num_elements", "in")
                    num_elements_array = allobjects_num_elements.ravel().astype(int) if allobjects_num_elements.ndim > 1 else allobjects_num_elements.astype(int)
                    flattened_data = []
                    scaled_scan_indices = []
                    scan_index_in = self._radar_datastore_obj.get_data("DC", "scan_index", "in")
                    max_rows = min(len(rounded_sig_data), len(num_elements_array), len(scan_index_in))
                    for row_idx in range(max_rows):
                        row = rounded_sig_data[row_idx]
                        num_elements = min(num_elements_array[row_idx], len(row)) if row_idx < len(num_elements_array) else len(row)
                        if num_elements > 0:
                            flattened_data.extend(row[:num_elements])
                            scaled_scan_indices.extend([scan_index_in[row_idx]] * num_elements)
                    rounded_sig_data = np.array(flattened_data)
                    scaled_scan_index_array_in = np.array(scaled_scan_indices)
                except (KeyError, AttributeError, IndexError):
                    # Fallback to original behavior if num_elements not available
                    input_scan_index_scaling_factor = rounded_sig_data.shape[1] if rounded_sig_data.ndim == 2 else "Not 2D array"
                    if input_scan_index_scaling_factor != "Not 2D array":
                        scan_index_in = self._radar_datastore_obj.get_data("DC", "scan_index", "in")
                        scaled_scan_index_array_in = np.repeat(scan_index_in, input_scan_index_scaling_factor)
                    else:
                        scaled_scan_index_array_in = np.array([])
            elif is_detection_signal:
                flattened_data = []
                scaled_scan_indices = []
                scan_index_in = self._radar_datastore_obj.get_data("DC", "scan_index", "in")
                for row_idx, row in enumerate(rounded_sig_data):
                    # Filter out zeros from this row
                    non_zero_mask = row != 0
                    non_zero_values = row[non_zero_mask]
                    if len(non_zero_values) > 0:
                        flattened_data.extend(non_zero_values)
                        scaled_scan_indices.extend([scan_index_in[row_idx]] * len(non_zero_values))
                rounded_sig_data = np.array(flattened_data)
                scaled_scan_index_array_in = np.array(scaled_scan_indices)
            else:
                input_scan_index_scaling_factor = rounded_sig_data.shape[1] if rounded_sig_data.ndim == 2 else "Not 2D array"
                if input_scan_index_scaling_factor != "Not 2D array":
                    self._plot_datastore_obj.update_data(str(sig) + "Dim", input_scan_index_scaling_factor, "in")
                    scan_index_in = self._radar_datastore_obj.get_data("DC", "scan_index", "in")
                    scaled_scan_index_array_in = np.repeat(scan_index_in, input_scan_index_scaling_factor)
                else:
                    scaled_scan_index_array_in = np.array([])
            
            if len(scaled_scan_index_array_in) > 0:
                self._plot_datastore_obj.update_data(sig,
                                                     scaled_scan_index_array_in,
                                                     "in_scan_index", None, sensor)
            rounded_sig_data = rounded_sig_data.ravel() if rounded_sig_data.ndim > 1 else rounded_sig_data

            self._plot_datastore_obj.update_data(sig, rounded_sig_data, "in")
        except (KeyError, AttributeError, IndexError, TypeError) as e:
            # Signal not available in this data source (e.g., GT file might not have detection signals)
            pass

        try:
            sig_data = self._radar_datastore_obj.get_data(sensor, sig, "out")
            # For confidence values and object_class (categorical), keep as integers
            # For other signals - round to 2 decimal places
            if sig == "azimuth_confidence" or sig == "elevation_confidence" or sig == "object_class":
                rounded_sig_data = sig_data.astype(int)
            else:
                rounded_sig_data = (sig_data * 100).astype(int) / 100.0
            
    
            if is_allobjects_signal:
                # Slice based on num_elements field (0 to num_elements-1 per scan)
                try:
                    allobjects_num_elements = self._radar_datastore_obj.get_data(sensor, "allobjects_num_elements", "out")
                    num_elements_array = allobjects_num_elements.ravel().astype(int) if allobjects_num_elements.ndim > 1 else allobjects_num_elements.astype(int)
                    flattened_data = []
                    scaled_scan_indices = []
                    scan_index_out = self._radar_datastore_obj.get_data("DC", "scan_index", "out")
                    max_rows = min(len(rounded_sig_data), len(num_elements_array), len(scan_index_out))
                    for row_idx in range(max_rows):
                        row = rounded_sig_data[row_idx]
                        num_elements = min(num_elements_array[row_idx], len(row)) if row_idx < len(num_elements_array) else len(row)
                        if num_elements > 0:
                            flattened_data.extend(row[:num_elements])
                            scaled_scan_indices.extend([scan_index_out[row_idx]] * num_elements)
                    rounded_sig_data = np.array(flattened_data)
                    scaled_scan_index_array_out = np.array(scaled_scan_indices)
                    self._plot_datastore_obj.update_data(sig,
                                                         scaled_scan_index_array_out,
                                                         "out_scan_index", None, sensor)
                except (KeyError, AttributeError, IndexError):
                    # Fallback to original behavior if num_elements not available
                    input_scan_index_scaling_factor = rounded_sig_data.shape[1] if rounded_sig_data.ndim == 2 else "Not 2D array"
                    if input_scan_index_scaling_factor != "Not 2D array":
                        scan_index_out = self._radar_datastore_obj.get_data("DC", "scan_index", "out")
                        scaled_scan_index_array_out = np.repeat(scan_index_out, input_scan_index_scaling_factor)
                    else:
                        scaled_scan_index_array_out = np.array([])

                    if len(scaled_scan_index_array_out) > 0:
                        self._plot_datastore_obj.update_data(sig,
                                                             scaled_scan_index_array_out,
                                                             "out_scan_index", None, sensor)
            elif is_detection_signal:
                flattened_data = []
                scaled_scan_indices = []
                scan_index_out = self._radar_datastore_obj.get_data("DC", "scan_index", "out")
                for row_idx, row in enumerate(rounded_sig_data):
                    # Filter out zeros from this row
                    non_zero_mask = row != 0
                    non_zero_values = row[non_zero_mask]
                    if len(non_zero_values) > 0:
                        flattened_data.extend(non_zero_values)
                        scaled_scan_indices.extend([scan_index_out[row_idx]] * len(non_zero_values))
                rounded_sig_data = np.array(flattened_data)
                scaled_scan_index_array_out = np.array(scaled_scan_indices)
                self._plot_datastore_obj.update_data(sig,
                                                     scaled_scan_index_array_out,
                                                     "out_scan_index", None, sensor)
            else:
                input_scan_index_scaling_factor = rounded_sig_data.shape[1] if rounded_sig_data.ndim == 2 else "Not 2D array"
                if input_scan_index_scaling_factor != "Not 2D array":
                    self._plot_datastore_obj.update_data(str(sig) + "Dim", input_scan_index_scaling_factor, "out")
                    scan_index_out = self._radar_datastore_obj.get_data("DC", "scan_index", "out")
                    scaled_scan_index_array_out = np.repeat(scan_index_out, input_scan_index_scaling_factor)
                else:
                    scaled_scan_index_array_out = np.array([])

                if len(scaled_scan_index_array_out) > 0:
                    self._plot_datastore_obj.update_data(sig,
                                                     scaled_scan_index_array_out,
                                                     "out_scan_index", None, sensor)
            rounded_sig_data = rounded_sig_data.ravel() if rounded_sig_data.ndim > 1 else rounded_sig_data

            self._plot_datastore_obj.update_data(sig, rounded_sig_data, "out")
        except (KeyError, AttributeError, IndexError, TypeError) as e:
            pass

        # Pad arrays to equal shape

    def pad_array(self, arr, target_rows, target_cols):
        padded = np.full((target_rows, target_cols), np.nan)
        padded[:arr.shape[0], :arr.shape[1]] = arr
        return padded

    def compare_vehicle_resim_datapoints(self, sig, sensor):
        # Create dictionaries for quick lookup

        try:
            vehicle_data = self._radar_datastore_obj.get_data(sensor, sig, "in")
        except KeyError as e:
            print(f"KeyError: {e} not found in radar datastore.")
            vehicle_data = None  # or handle it another way

        try:
            resim_data = self._radar_datastore_obj.get_data(sensor, sig, "out")
        except KeyError as e:
            print(f"KeyError: {e} not found in radar datastore.")
            resim_data = None  # or handle it another way

        if vehicle_data is not None and resim_data is not None:

            input_scan_index = self._radar_datastore_obj.get_data("DC", "scan_index", "in")
            output_scan_index = self._radar_datastore_obj.get_data("DC", "scan_index", "out")
            tolerance_dict = {
                "range": 0.1,
                "range_rate": 0.015,
                "elevation": 0.00873,
                "azimuth": 0.00873,
                "snr": 0,
                "std_rcs": 0,
                "vcs_pos_x": 0,
                "vcs_pos_y": 0,
                "vcs_vel_x": 0,
                "vcs_vel_y": 0,
                "vcs_accel_x": 0,
                "vcs_accel_y": 0

            }

            # Find common scan indices and their positions
            common_indices = np.intersect1d(input_scan_index, output_scan_index)
            # np.isin(input_scan_index, common_indices) returns a boolean array indicating
            # which elements of input_scan_index are
            # present in common_indices. np.nonzero(...) returns the indices of True values in that boolean array.
            # [0] extracts the first element of the tuple returned by np.nonzero,
            # which is the array of matching indices
            input_positions = np.nonzero(np.isin(input_scan_index, common_indices))[0]
            output_positions = np.nonzero(np.isin(output_scan_index, common_indices))[0]

            # Extract rows corresponding to common scan indices
            array1_common = vehicle_data[input_positions]
            array2_common = resim_data[output_positions]

            target_rows = max(array1_common.shape[0], array2_common.shape[0])
            target_cols = max(array1_common.shape[1], array2_common.shape[1])

            array1_padded = self.pad_array(array1_common, target_rows, target_cols)
            array2_padded = self.pad_array(array2_common, target_rows, target_cols)

            # Compare with tolerance
            mask = np.isclose(array1_padded, array2_padded, atol=tolerance_dict[sig], equal_nan=False)
            # performing an element-wise comparison between two NumPy arrays (array1_padded and array2_padded)
            # to check if their values are close within a specified absolute tolerance

            # Extract matching and missing values
            matching_values = np.where(mask, array1_padded, np.nan)
            mismatch_values = np.where(mask, np.nan, array1_padded)
            # using the mask from np.isclose(...) to separate matching and non-matching values from array1_padded
            # matching_values: Keeps values from array1_padded where the mask is True (i.e., values are close to
            # array2_padded), and replaces others with NaN. missing_values: Keeps values from array1_padded where the
            # mask is False (i.e., values are not close), and replaces matching ones with NaN.

            # Clean columns with all NaNs
            matched_clean_2d = matching_values[:, ~np.all(np.isnan(matching_values), axis=0)]
            mismatch_clean_2d = mismatch_values[:, ~np.all(np.isnan(mismatch_values), axis=0)]

            # Calculate percentages
            total_elements = np.count_nonzero(~np.isnan(array1_padded))
            matched_elements = np.count_nonzero(~np.isnan(matched_clean_2d))
            mismatch_elements = np.count_nonzero(~np.isnan(mismatch_clean_2d))
            match_percentage = 0
            mismatch_percentage = 0

            if matched_elements != 0:
                match_percentage = (matched_elements / total_elements) * 100
            else:
                match_percentage = 0
            if mismatch_elements != 0:
                mismatch_percentage = (mismatch_elements / total_elements) * 100
            else:
                mismatch_percentage = 0

            if sig == "range" or sig == "range_rate" or sig == "azimuth" or sig == "elevation" or sig == "snr" \
                    or sig == "vcs_pos_x" or sig == "vcs_pos_y" or sig == "vcs_vel_x" or sig == "vcs_vel_y" \
                    or sig == "vcs_accel_x" or sig == "vcs_accel_y":
                ReportDash.signal_stats[sensor][sig]["match"] = match_percentage
                ReportDash.signal_stats[sensor][sig]["mismatch"] = mismatch_percentage
