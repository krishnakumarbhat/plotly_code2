import h5py, os
import numpy as np
from collections import defaultdict
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.Sig_Prep.isig_observer import ISigObserver

class RadarHDFDataCollect(IDataCollect, ISigObserver):
    sensor_to_sig = defaultdict(list)

    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore):
        self._mediator = mediator
        self._radar_ds = radar_ds
        self._plot_ds = plot_ds
        self._in_file = self._out_file = None
        self._poi_paths = defaultdict(list)

    def clear_data(self):
        RadarHDFDataCollect.sensor_to_sig.clear()
        self._in_file = self._out_file = None
        self._poi_paths.clear()

    def collect_data(self, inp=None, out=None):
        self._in_file, self._out_file = inp, out
        self._parse_and_collect()
        self._mediator.notify_event(self, "JSON_GEN_DONE")

    def _parse_and_collect(self):
        """
        This function calls hdf parser function for input and output hdf
        after parsing of  input & output hdf, data sets are updated to radar data store.
        once in & out data set updated to radar data store, an event (RDS_UPDATES) is notified.
        radar data preparation module (radar_dataprep.py) will consume the event (RDS_UPDATES)
        parsing will be done sensor by sensor ( in & out)
        example : parse RL (in and out) --> Generate RL Json (Histogram,scatter......)
                  * to do 1* After RL Json Generation done clear data store memory
        """
        for sensor, paths in self._poi_paths.items():
            print(f"----------------------------\nprocessing sensor {sensor}")
            self._parse_hdf(str(self._in_file), "in", paths)
            self._parse_hdf(str(self._out_file), "out", paths)
            self._mediator.notify_event(sensor, "RDS_UPDATED")

    def final_poi_sensor_datapath(self, data):
        self._poi_paths = data
        for sensor, paths in self._poi_paths.items():
            for p in paths:
                RadarHDFDataCollect.sensor_to_sig[sensor].append(os.path.basename(p))

    def _parse_hdf(self, hdf_file, src, paths):
        """
        This function traverse to the data path in HDF file and read the dataset
        and store the signal value (2D) in radar data store.
        Before storing it does the following operations
        a. removes duplicate rows in 2D data set
        b. removes zeros from all rows of 2D data set

        """
        with h5py.File(hdf_file, 'r') as f:
            for p in paths:
                if p not in f:
                    print(f"Dataset {p} not in file {hdf_file}")
                    continue
                sig = os.path.basename(p)
                sensor = p.split('/')[0]
                data = f[p][()]
                
                # DEBUG: Print original data stats
                print(f"\n[DEBUG] Signal: {sig}, Source: {src}")
                print(f"  Original shape: {data.shape}, dtype: {data.dtype}")
                print(f"  Original min: {np.min(data):.6f}, max: {np.max(data):.6f}")
                print(f"  Non-zero count: {np.count_nonzero(data)} / {data.size}")
                
                # Replace truly invalid exponential values (very small or very large)
                # Only filter values that are likely sensor noise (extremely small) or invalid (extremely large)
                mask = (np.abs(data) < 1e-10) | (np.abs(data) >= 1e10) | ~np.isfinite(data)
                data = data.astype(np.float64)  # Ensure we work with float
                data[mask] = 0
                
                print(f"  After exp filter - Non-zero count: {np.count_nonzero(data)} / {data.size}")
                
                # Remove zeros from each row and use NaN for padding instead of 0
                # This way we can filter out NaN later during plotting
                rows = [r[r != 0.0] for r in data]
                maxlen = max(len(r) for r in rows) if rows else 0
                
                print(f"  Max row length after zero removal: {maxlen}")
                print(f"  Non-empty rows: {sum(1 for r in rows if len(r) > 0)} / {len(rows)}")
                
                if maxlen == 0:
                    print(f"  WARNING: All data is zero for {sig}!")
                    filtered = data  # Keep original if all zeros
                else:
                    # Use NaN for padding so we can filter it out later
                    filtered = np.array([np.pad(r.astype(np.float64), (0, maxlen - len(r)), constant_values=np.nan) for r in rows])
                
                print(f"  Final shape: {filtered.shape}")
                print(f"  Final non-zero/non-nan count: {np.count_nonzero(~np.isnan(filtered) & (filtered != 0))}")
                
                self._radar_ds.update_data(sensor, sig, filtered, src)

    def consume_event(self, sensor, event): pass
