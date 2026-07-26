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
from IPS.DataCollect.radar_hdf_datacollect import RadarHDFDataCollect

class RadarDataPreparation:
    input_scan_index_arr = output_scan_index_arr = None

    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore):
        self._mediator = mediator
        self._radar_ds = radar_ds
        self._plot_ds = plot_ds

    def clear_data(self):
        RadarDataPreparation.input_scan_index_arr = RadarDataPreparation.output_scan_index_arr = None

    def consume_event(self, sensor, event):
        """
        Once HDF data collection is completed, an event RDS_UPDATED is notified.
        the event RDS_UPDATED is consumed here. Here we call flatten ( conversion of 2d to 1d)
        scan index and flatten ( conversion of 2d to 1d) radar data and store the flattened
        in plot data store (PDS).

        an event PDS_UPDATED is notified
        """
        if event != "RDS_UPDATED": return
        sigs = RadarHDFDataCollect.sensor_to_sig[sensor]
        if 'scan_index' in sigs:
            sigs.remove('scan_index')
            sigs.insert(0, 'scan_index')
        for sig in sigs:
            if sig == 'scan_index':
                self._flatten_scan_index(sensor, sig)
            else:
                self._flatten_radar_data(sensor, sig)
        self._mediator.notify_event(sensor, "PDS_UPDATED")

    def _flatten_scan_index(self, sensor, sig):
        """
           This function get scan index from radar data store and convert 2D scan index
           to 1D scan index using ravel() and update flatted scan index to plot data store
        """
        for src in ('in', 'out'):
            arr = self._radar_ds.get_data(sensor, sig, src).ravel()
            # Filter out NaN values from scan_index as well
            valid_mask = ~np.isnan(arr) & (arr != 0.0)
            arr_filtered = arr[valid_mask]
            print(f"[DEBUG PREP] scan_index {src}: {len(arr)} -> {len(arr_filtered)} after filtering")
            self._plot_ds.update_data(sig, arr_filtered, src)

    def _flatten_radar_data(self, sensor, sig):
        """
        This function gets radar data(2D) of signal from radar data store and takes only two decimal
        places and getting the dimension of signal and update signal dimension to plot data store.
        updated signal dimension is used as scaling factor for scan index duplication ( required for plotting)

        then convert 2D radar data to 1D data and update to plot data store.

        signals like phi and theta are converted from radian to degree and updated to plot data store.
        From plot data store we can retrieve to plot scatter plots

        """
        for src, scan_src in [('in', 'in'), ('out', 'out')]:
            data = self._radar_ds.get_data(sensor, sig, src)
            
            # DEBUG: Print data stats before processing
            print(f"\n[DEBUG PREP] Signal: {sig}, Source: {src}")
            print(f"  Input shape: {data.shape}, dtype: {data.dtype}")
            
            scale = data.shape[1] if data.ndim == 2 else None
            if scale:
                self._plot_ds.update_data(f"{sig}Dim", scale, src)
                scan = self._plot_ds.get_data('scan_index', src)
                if src == 'out':
                    in_scan = self._plot_ds.get_data('scan_index', 'in')
                    mask = np.isin(scan, in_scan)
                    data = data[mask]
                    scan = scan[mask]
                scaled = np.repeat(scan, scale)
                self._plot_ds.update_data(sig, scaled, f"{src}_scan_index", None, sensor)
            
            flat = data.ravel().astype(np.float32)
            
            # Filter out NaN and zero values BEFORE storing for plotting
            # This ensures we only plot actual data, not padding
            valid_mask = ~np.isnan(flat) & (flat != 0.0)
            flat_filtered = flat[valid_mask]
            
            print(f"  Before filter: {len(flat)} values, After filter: {len(flat_filtered)} values")
            print(f"  Filtered min: {np.min(flat_filtered) if len(flat_filtered) > 0 else 'N/A':.6f}, max: {np.max(flat_filtered) if len(flat_filtered) > 0 else 'N/A':.6f}")
            
            if sig in ("theta", "phi", "std_theta_1", "std_phi_1"):
                np.degrees(flat_filtered, out=flat_filtered)
                print(f"  After degrees conversion - min: {np.min(flat_filtered) if len(flat_filtered) > 0 else 'N/A':.6f}, max: {np.max(flat_filtered) if len(flat_filtered) > 0 else 'N/A':.6f}")
            
            # Also filter the scan index to match the filtered data
            if scale:
                scaled_filtered = scaled[valid_mask]
                self._plot_ds.update_data(sig, scaled_filtered, f"{src}_scan_index", None, sensor)
            
            self._plot_ds.update_data(sig, flat_filtered, src)
