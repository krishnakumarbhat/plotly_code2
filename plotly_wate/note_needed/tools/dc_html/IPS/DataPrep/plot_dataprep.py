"""
Description:
This module has functionality that compares vehicle and resimulated
data points of all interested signals and compute match and mismatch
percentage and use Report Dash variable to update Match and mismatch
percentage.

"""

import numpy as np
from collections import defaultdict
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DashManager.report_dash import ReportDash

class PlotDataPreparation:
    output_scan_index_scaled = input_scan_index_scaled = np.array([])
    list_json_path = []
    mismatch_input_value = mismatch_output_value = np.array([])
    mismatch_input_scan_index = mismatch_output_scan_index = np.array([])

    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore, json_ds: IDataStore, sig_fil: SignalFilter):
        self._mediator = mediator
        self._plot_ds = plot_ds
        self._radar_ds = radar_ds
        self._json_ds = json_ds
        self._sig_fil = sig_fil

    def clear_data(self):
        PlotDataPreparation.output_scan_index_scaled = PlotDataPreparation.input_scan_index_scaled = np.array([])
        PlotDataPreparation.list_json_path.clear()
        PlotDataPreparation.mismatch_input_value = PlotDataPreparation.mismatch_output_value = np.array([])
        PlotDataPreparation.mismatch_input_scan_index = PlotDataPreparation.mismatch_output_scan_index = np.array([])

    def consume_event(self, sensor, event):
        if event != "PDS_UPDATED": return
        for stream in self._sig_fil.get_streams():
            for sig in self._sig_fil.get_sig_in_stream(stream):
                if sig == 'scan_index': continue
                plots = self._sig_fil.get_sig_plot_type(stream, sig)
                if plots and "SC_MM" in plots:
                    self._compare_datapoints(sig, sensor)
                self._mediator.notify_event(f"{sensor}#{stream}#{sig}", "JDS_UPDATED")

    # Pad arrays to equal shape
    def _pad_array(self, arr, rows, cols):
        padded = np.full((rows, cols), np.nan)
        padded[:arr.shape[0], :arr.shape[1]] = arr
        return padded

    def _compare_datapoints(self, sig, sensor):
        # Create dictionaries for quick lookup
        try:
            vehicle = self._radar_ds.get_data(sensor, sig, "in")
            resim = self._radar_ds.get_data(sensor, sig, "out")
        except: return
        if vehicle is None or resim is None: return
        in_idx = self._radar_ds.get_data(sensor, "scan_index", "in")
        out_idx = self._radar_ds.get_data(sensor, "scan_index", "out")
        tol = {"ran": 0.1, "vel": 0.015, "phi": 0.00873, "theta": 0.00873, "snr": 0, "rcs": 0}.get(sig, 0)

            # Find common scan indices and their positions
        common = np.intersect1d(in_idx, out_idx)
            # np.isin(input_scan_index, common_indices) returns a boolean array indicating
            # which elements of input_scan_index are
            # present in common_indices. np.nonzero(...) returns the indices of True values in that boolean array.
            # [0] extracts the first element of the tuple returned by np.nonzero,
            # which is the array of matching indices
        in_pos = np.nonzero(np.isin(in_idx, common))[0]
        out_pos = np.nonzero(np.isin(out_idx, common))[0]

            # Extract rows corresponding to common scan indices
        a1, a2 = vehicle[in_pos], resim[out_pos]
        rows, cols = max(a1.shape[0], a2.shape[0]), max(a1.shape[1], a2.shape[1])
        a1_pad, a2_pad = self._pad_array(a1, rows, cols), self._pad_array(a2, rows, cols)

            # Compare with tolerance
        mask = np.isclose(a1_pad, a2_pad, atol=tol, equal_nan=False)
            # performing an element-wise comparison between two NumPy arrays (array1_padded and array2_padded)
            # to check if their values are close within a specified absolute tolerance

            # Extract matching and missing values
        matched = np.where(mask, a1_pad, np.nan)
        mismatch = np.where(mask, np.nan, a1_pad)
            # using the mask from np.isclose(...) to separate matching and non-matching values from array1_padded
            # matching_values: Keeps values from array1_padded where the mask is True (i.e., values are close to
            # array2_padded), and replaces others with NaN. missing_values: Keeps values from array1_padded where the
            # mask is False (i.e., values are not close), and replaces matching ones with NaN.

            # Clean columns with all NaNs
        matched_clean = matched[:, ~np.all(np.isnan(matched), axis=0)]
        mismatch_clean = mismatch[:, ~np.all(np.isnan(mismatch), axis=0)]
        total = np.count_nonzero(~np.isnan(a1_pad))
        match_pct = (np.count_nonzero(~np.isnan(matched_clean)) / total * 100) if total else 0
        mismatch_pct = (np.count_nonzero(~np.isnan(mismatch_clean)) / total * 100) if total else 0
        if sig in ("ran", "vel", "phi", "theta", "snr", "rcs"):
            ReportDash.signal_stats[sensor][sig]["match"] = match_pct
            ReportDash.signal_stats[sensor][sig]["mismatch"] = mismatch_pct
            if mismatch_pct > 0:

                '''
                  Below Logic takes value of missing values and removes zeros
                  and find the scaling factor(scan index duplication) to calculate missing scan index
                  and convert missing values from 2d to 1d using ravel api 
                  
                '''
                self._plot_ds.mismatch_signal_list[sensor] = sig
                mm_idx, mm_val = [], []
                for idx, row in zip(in_idx[in_pos], mismatch):
                    for v in row:
                        if not np.isnan(v):
                            mm_idx.append(idx)
                            mm_val.append(v)
                self._plot_ds.update_data(sig, None, "mismatch_scan_index_in", mm_idx, sensor)
                self._plot_ds.update_data(sig, mm_val, "mismatch_in", None, sensor)
