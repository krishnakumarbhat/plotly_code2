import h5py, os, re
import numpy as np
from collections import defaultdict
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.Sig_Prep.isig_observer import ISigObserver

class RadarHDFCANDataCollect(IDataCollect, ISigObserver):
    sensor_to_sig = defaultdict(list)

    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore):
        self._mediator = mediator
        self._radar_ds = radar_ds
        self._plot_ds = plot_ds
        self._poi_paths = defaultdict(list)
        self._in_file = self._out_file = None

    def clear_data(self): pass

    def collect_data(self, inp=None, out=None):
        self._in_file, self._out_file = inp, out
        for sensor, paths in self._poi_paths.items():
            print(f"processing sensor {sensor}")
            self._parse_hdf(str(inp), "in", paths, sensor.split('_')[-1] if '_' in sensor else sensor)
            self._parse_hdf(str(out), "out", paths, sensor.split('_')[-1] if '_' in sensor else sensor)
            self._mediator.notify_event(sensor.split('_')[-1] if '_' in sensor else sensor, "CAN_DATA_COLLECTION_DONE")
        self._mediator.notify_event(self, "CAN_JSON_GEN_DONE")

    def final_poi_sensor_datapath(self, data):
        self._poi_paths.clear()
        for sensor, paths in data.items():
            def extract_idx(p):
                try: return int(p.split("_")[-1])
                except: return float('inf')
            groups = defaultdict(list)
            for p in paths:
                sig = os.path.basename(p)
                m = re.match(r"(DET_RANGE|DET_RCS|DET_AZIMUTH|DET_ELEVATION|DET_RANGE_VELOCITY|DET_SNR).*?_\d+$", sig)
                groups[m.group(1) if m else "other"].append(p)
            merged = []
            for k in ["DET_RANGE", "DET_RCS", "DET_AZIMUTH", "DET_ELEVATION", "DET_RANGE_VELOCITY", "DET_SNR", "other"]:
                merged.extend(sorted(groups[k], key=extract_idx))
            self._poi_paths[sensor] = merged
            for p in merged:
                RadarHDFCANDataCollect.sensor_to_sig[sensor].append(os.path.basename(p))

    def _parse_hdf(self, hdf_file, src, paths, sensor):
        sig_groups = defaultdict(list)
        ts_dict = {}
        with h5py.File(hdf_file, 'r') as f:
            for p in paths:
                if p not in f: continue
                data = f[p][()]
                full_sig = p.split("/")[-1]
                prefix = "_".join(full_sig.split("_")[:-1])
                sig_groups[prefix].append(data.tolist() if data.ndim in (1, 2) else [])
                parent = "/".join(p.split("/")[:-1])
                stream = parent.split("/")[-1]
                ts_path = f"{parent}/timestamp_{stream}"
                if ts_path in f:
                    ts_dict[ts_path] = f[ts_path][()].tolist()
        for sig, grouped in sig_groups.items():
            self._radar_ds.update_data(sensor, sig, grouped, src)
        if ts_dict:
            self._radar_ds.update_data(sensor, "timestamp_data", ts_dict, src)

    def consume_event(self, s, e): pass
