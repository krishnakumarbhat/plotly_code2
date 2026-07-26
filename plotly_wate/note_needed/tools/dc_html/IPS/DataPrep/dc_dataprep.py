"""
Description:
This module uses Radar Data Store(RDS) and prepare the data for
plotting. RDS have signal data in 2D format.
In this module convert 2D to 1D.
It also scales up scan index as per dimension of 2D data
Example : Range Signal 2D ( rows:200, column : 250)
Each row    ---> corresponds to Scan index
Each Column ---> corresponds to data
"""



import numpy as np
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.DashManager.report_dash import ReportDash
from IPS.RepGen.dc_json_to_html_convert import DCJsonToHtmlConvertor

class DCPlotDataPreparation:
    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore, json_ds: IDataStore, sig_fil: SignalFilter, dc_collector: IDataCollect):
        self._mediator = mediator
        self._plot_ds = plot_ds
        self._radar_ds = radar_ds
        self._json_ds = json_ds
        self._sig_fil = sig_fil
        self._dc_collector = dc_collector

    def clear_data(self): pass

    def consume_event(self, sensor, event):
        if event != "DC_DET_RDS_UPDATED": return
        for stream in set(DCJsonToHtmlConvertor.stream_list):
            sigs = DCJsonToHtmlConvertor.stream_signals.get(stream, set())
            for sig in sigs:
                sensors = ["FL", "FR", "RR", "RL"] if stream.startswith("Detections") else ["DC"]
                for s in sensors:
                    self._flatten(s, sig)
                    if sig in ["range", "range_rate", "azimuth", "elevation", "snr", "vcs_pos_x", "vcs_pos_y", "vcs_vel_x", "vcs_vel_y", "vcs_accel_x", "vcs_accel_y"]:
                        self._compare(sig, s)
                    self._mediator.notify_event(f"{s}#{stream}#{sig}", "JDS_UPDATED")

    def _flatten(self, sensor, sig):
        """
        This function gets radar data(2D) of signal from radar data store and takes only two decimal
        places and getting the dimension of signal and update signal dimension to plot data store.
        updated signal dimension is used as scaling factor for scan index duplication ( required for plotting)

        then convert 2D radar data to 1D data and update to plot data store.

        signals like phi and theta are converted from radian to degree and updated to plot data store.
        From plot data store we can retrieve to plot scatter plots

        """
        for src in ('in', 'out'):
            try:
                data = self._radar_ds.get_data(sensor, sig, src)
                
                # DEBUG: Print raw data stats
                # print(f"\n[DEBUG DC_FLATTEN] Signal: {sig}, Sensor: {sensor}, Source: {src}")
                # print(f"  Raw data shape: {data.shape}, dtype: {data.dtype}")
                # print(f"  Raw data min: {np.min(data):.6f}, max: {np.max(data):.6f}")
                # print(f"  Non-zero count: {np.count_nonzero(data)} / {data.size}")
                
                # Round to 6 decimal places to keep precision for small values
                # Using np.round instead of int conversion to preserve data
                rounded = np.round(data.astype(np.float64), 6)
                
                # print(f"  After rounding min: {np.min(rounded):.6f}, max: {np.max(rounded):.6f}")
                
                if rounded.ndim == 2:
                    scale = rounded.shape[1]
                    self._plot_ds.update_data(f"{sig}Dim", scale, src)
                    scan = self._radar_ds.get_data("DC", "scan_index", src)
                    self._plot_ds.update_data("DC", np.repeat(scan, scale), f"{src}_scan_index", None, sensor)
                elif rounded.ndim == 1:
                    # For 1D data (like Feature Functions), use array indices as scan index
                    scale = 1
                    self._plot_ds.update_data(f"{sig}Dim", scale, src)
                    # Use sequential indices as scan index for 1D data
                    scan_indices = np.arange(len(rounded))
                    self._plot_ds.update_data("DC", scan_indices, f"{src}_scan_index", None, sensor)
                
                flat = rounded.ravel()
                self._plot_ds.update_data(sig, flat, src)
            except Exception:
                pass  # Silently skip signals that can't be flattened

    def _pad(self, arr, rows, cols):
        p = np.full((rows, cols), np.nan)
        p[:arr.shape[0], :arr.shape[1]] = arr
        return p

    def _compare(self, sig, sensor):
        try:
            vehicle, resim = self._radar_ds.get_data(sensor, sig, "in"), self._radar_ds.get_data(sensor, sig, "out")
        except: return
        if vehicle is None or resim is None: return
        in_idx, out_idx = self._radar_ds.get_data("DC", "scan_index", "in"), self._radar_ds.get_data("DC", "scan_index", "out")
        tol = {"range": 0.1, "range_rate": 0.015, "elevation": 0.00873, "azimuth": 0.00873, "snr": 0, "std_rcs": 0, "vcs_pos_x": 0, "vcs_pos_y": 0, "vcs_vel_x": 0, "vcs_vel_y": 0, "vcs_accel_x": 0, "vcs_accel_y": 0}.get(sig, 0)

            # Find common scan indices and their positions
        common = np.intersect1d(in_idx, out_idx)
            # np.isin(input_scan_index, common_indices) returns a boolean array indicating
            # which elements of input_scan_index are
            # present in common_indices. np.nonzero(...) returns the indices of True values in that boolean array.
            # [0] extracts the first element of the tuple returned by np.nonzero,
            # which is the array of matching indices
        in_pos, out_pos = np.nonzero(np.isin(in_idx, common))[0], np.nonzero(np.isin(out_idx, common))[0]
        a1, a2 = vehicle[in_pos], resim[out_pos]
        rows, cols = max(a1.shape[0], a2.shape[0]), max(a1.shape[1], a2.shape[1])
        a1_pad, a2_pad = self._pad(a1, rows, cols), self._pad(a2, rows, cols)

            # Compare with tolerance
        mask = np.isclose(a1_pad, a2_pad, atol=tol, equal_nan=False)
            # performing an element-wise comparison between two NumPy arrays (array1_padded and array2_padded)
            # to check if their values are close within a specified absolute tolerance

            # Extract matching and missing values
        matched = np.where(mask, a1_pad, np.nan)[:, ~np.all(np.isnan(np.where(mask, a1_pad, np.nan)), axis=0)]
            # using the mask from np.isclose(...) to separate matching and non-matching values from array1_padded
            # matching_values: Keeps values from array1_padded where the mask is True (i.e., values are close to
            # array2_padded), and replaces others with NaN. missing_values: Keeps values from array1_padded where the
            # mask is False (i.e., values are not close), and replaces matching ones with NaN.

            # Clean columns with all NaNs
        mismatch = np.where(mask, np.nan, a1_pad)[:, ~np.all(np.isnan(np.where(mask, np.nan, a1_pad)), axis=0)]

            # Calculate percentages
        total = np.count_nonzero(~np.isnan(a1_pad))
        match_pct = (np.count_nonzero(~np.isnan(matched)) / total * 100) if total else 0
        mismatch_pct = (np.count_nonzero(~np.isnan(mismatch)) / total * 100) if total else 0
        if sig in ["range", "range_rate", "azimuth", "elevation", "snr", "vcs_pos_x", "vcs_pos_y", "vcs_vel_x", "vcs_vel_y", "vcs_accel_x", "vcs_accel_y"]:
            ReportDash.signal_stats[sensor][sig]["match"] = match_pct
            ReportDash.signal_stats[sensor][sig]["mismatch"] = mismatch_pct
