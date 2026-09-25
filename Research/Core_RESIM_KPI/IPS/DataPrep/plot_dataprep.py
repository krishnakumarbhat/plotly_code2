"""
File Name: plot_dataprep.py
Author: Bharanidharan Subramaniam
Email : Bharanidharan.s@aptiv.com
Description:
This module has functionality that compares vehicle and resimulated
data points of all interested signals and compute match and mismatch
percentage and use Report Dash variable to update Match and mismatch
percentage.

"""



from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DashManager.report_dash import ReportDash
import numpy as np
from collections import defaultdict


class PlotDataPreparation:
    output_scan_index_scaled = np.array([])
    input_scan_index_scaled = np.array([])
    list_json_path = []
    mismatch_input_value = np.array([])
    mismatch_output_value = np.array([])
    mismatch_input_scan_index = np.array([])
    mismatch_output_scan_index = np.array([])

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore,
                 jsonpath_datastore: IDataStore,
                 sig_filter: SignalFilter):
        self._data_event_mediator_obj = event_mediator
        self._plot_datastore_obj = plot_datastore
        self._radar_datastore_obj = radar_datastore
        self._jsonpath_datastore_obj = jsonpath_datastore
        self._sig_filter_obj = sig_filter

    def clear_data(self):

        PlotDataPreparation.output_scan_index_scaled = np.array([])
        PlotDataPreparation.input_scan_index_scaled = np.array([])
        PlotDataPreparation.list_json_path.clear()
        PlotDataPreparation.mismatch_input_value = np.array([])
        PlotDataPreparation.mismatch_output_value = np.array([])
        PlotDataPreparation.mismatch_input_scan_index = np.array([])
        PlotDataPreparation.mismatch_output_scan_index = np.array([])

    def group_by_index(self, data, scan_index):
        grouped = defaultdict(list)
        for val, idx in zip(data, scan_index):
            grouped[idx].append(val)
        return grouped

    def consume_event(self, sensor, event):

        if event == "PDS_UPDATED":
            streams = self._sig_filter_obj.get_streams()
            for stream in streams:
                signal_list = self._sig_filter_obj.get_sig_in_stream(stream)

                for sig in signal_list:
                    if sig != 'scan_index':

                        plot_types = self._sig_filter_obj.get_sig_plot_type(stream, sig)
                        if "SC_MM" in plot_types:
                            self.compare_vehicle_resim_datapoints(sig, sensor)

                    self._data_event_mediator_obj.notify_event(sensor + "#" + str(stream) + "#" + sig,
                                                               "JDS_UPDATED")  # JDS--> JSON Data store

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

            input_scan_index = self._radar_datastore_obj.get_data(sensor, "scan_index", "in")
            output_scan_index = self._radar_datastore_obj.get_data(sensor, "scan_index", "out")
            tolerance_dict = {
                "ran": 0.1,
                "vel": 0.015,
                "phi": 0.00873,
                "theta": 0.00873,
                "snr": 0,
                "rcs": 0

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

            # Compute missing and additional scan indices
            if mismatch_elements != 0:
                missing_scan_index = np.setdiff1d(input_scan_index, output_scan_index)
                additional_scan_index = np.setdiff1d(output_scan_index, input_scan_index)

                input_scan_index = np.squeeze(input_scan_index)
                if input_scan_index.ndim != 1:
                    raise ValueError("input_scan_index must be a 1D array")

                mask = np.isin(input_scan_index, missing_scan_index)
                if mask.shape[0] != vehicle_data.shape[0]:
                    raise ValueError("Mask and vehicle_data row count mismatch")

                missing_values_from_array1 = vehicle_data[mask]

                output_scan_index = np.squeeze(output_scan_index)
                if output_scan_index.ndim != 1:
                    raise ValueError("output_scan_index must be a 1D array")

                mask = np.isin(output_scan_index, additional_scan_index)
                if mask.shape[0] != resim_data.shape[0]:
                    raise ValueError("Mask and vehicle_data row count mismatch")

                mask = np.isin(output_scan_index, additional_scan_index)
                additional_values_from_array2 = resim_data[mask]

                # ----------------------------------------------------------------

                '''
                  Below Logic takes value of missing values and removes zeros
                  and find the scaling factor(scan index duplication) to calculate missing scan index
                  and convert missing values from 2d to 1d using ravel api 
                  
                '''

                # Step 1: Remove zeros from each row
                non_zero_rows_missing_value = [row[row != 0.0] for row in missing_values_from_array1]

                # Step 2: Find the maximum row length after filtering
                max_row_len_missing_value = max(len(row) for row in non_zero_rows_missing_value)

                # Step 3: Pad rows to make them same length (with 0 or np.nan)
                filtered_missing_values = np.array([
                    np.pad(row, (0, max_row_len_missing_value - len(row)), constant_values=0)
                    for row in non_zero_rows_missing_value
                ])

                missing_scan_index_scaling_factor = filtered_missing_values.shape[
                    1] if filtered_missing_values.ndim == 2 else "Not 2D array"
                if missing_scan_index_scaling_factor != "Not 2D array":
                    missing_scan_index_scaled = np.repeat(missing_scan_index,
                                                          missing_scan_index_scaling_factor)

                missing_data_flatten = filtered_missing_values.ravel()  # ravel(convert 2d to 1d data)

                '''
                Below Logic takes value of additional values and removes zeros
                and find the scaling factor(scan index duplication) to calculate additional scan index
                and convert additional values from 2d to 1d using ravel api 
    
                '''

                # Step 1: Remove zeros from each row
                non_zero_rows_addition_values = [row[row != 0.0] for row in additional_values_from_array2]

                # Step 2: Find the maximum row length after filtering
                max_row_len_additional_value = max(len(row) for row in non_zero_rows_addition_values)

                # Step 3: Pad rows to make them same length (with 0 or np.nan)
                filtered_additional_values = np.array([
                    np.pad(row, (0, max_row_len_additional_value - len(row)), constant_values=0)
                    for row in non_zero_rows_addition_values
                ])

                additional_scan_index_scaling_factor = filtered_additional_values.shape[
                    1] if filtered_additional_values.ndim == 2 else "Not 2D array"
                if additional_scan_index_scaling_factor != "Not 2D array":
                    additional_scan_index_scaled = np.repeat(additional_scan_index,
                                                             additional_scan_index_scaling_factor)

                additional_data_flatten = filtered_additional_values.ravel()  # ravel(convert 2d to 1d data)

                # Prepare duplicated scan indices and missing data for plotting
                mismatch_scan_indices_for_plot = []
                mismatch_data_for_plot = []
                for idx, row in zip(input_scan_index[input_positions], mismatch_values):
                    for val in row:
                        if not np.isnan(val):
                            mismatch_scan_indices_for_plot.append(idx)
                            mismatch_data_for_plot.append(val)
            else:
                missing_data_flatten=[]
                additional_data_flatten=[]

            if sig == "ran" or sig == "vel" or sig == "phi" or sig == "theta" or sig == "snr" or sig == "rcs":

                ReportDash.signal_stats[sensor][sig]["match"] = match_percentage
                ReportDash.signal_stats[sensor][sig]["mismatch"] = mismatch_percentage

                if mismatch_percentage != 0:
                    self._plot_datastore_obj.mismatch_signal_list[sensor] = sig

                    self._plot_datastore_obj.update_data(sig,
                                                         None,
                                                         "mismatch_scan_index_in",
                                                         mismatch_scan_indices_for_plot,
                                                         sensor)

                    self._plot_datastore_obj.update_data(sig,
                                                         mismatch_data_for_plot,
                                                         "mismatch_in",
                                                         None,
                                                         sensor)

                if len(missing_data_flatten) != 0 or len(additional_data_flatten) != 0:
                    self._plot_datastore_obj.additional_missing_signal_list[sensor] = sig

                    self._plot_datastore_obj.update_data(sig,
                                                         missing_scan_index_scaled,
                                                         "missing_scan_index",
                                                         None,
                                                         sensor)

                    self._plot_datastore_obj.update_data(sig,
                                                         missing_data_flatten,
                                                         "missing_value",
                                                         None,
                                                         sensor)

                    self._plot_datastore_obj.update_data(sig,
                                                         additional_scan_index_scaled,
                                                         "additional_scan_index",
                                                         None,
                                                         sensor)

                    self._plot_datastore_obj.update_data(sig,
                                                         additional_data_flatten,
                                                         "additional_value",
                                                         None,
                                                         sensor)
