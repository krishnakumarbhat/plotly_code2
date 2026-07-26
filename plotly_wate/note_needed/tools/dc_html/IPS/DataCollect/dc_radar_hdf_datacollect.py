import h5py, os
import numpy as np
from collections import defaultdict
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.Sig_Prep.isig_observer import ISigObserver
from IPS.RepGen.dc_json_to_html_convert import DCJsonToHtmlConvertor
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DashManager.report_dash import ReportDash
import IPS.Metadata.GEN7V2.poi as poi

class RadarHDFDCDataCollect(IDataCollect, ISigObserver):
    SENSOR_IDX = {"0": "FL", "1": "FR", "2": "RL", "3": "RR"}

    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore, sig_fil: SignalFilter, report_dash: ReportDash):
        self._mediator = mediator
        self._radar_ds = radar_ds
        self._plot_ds = plot_ds
        self._poi_paths = defaultdict(list)
        self.dc_hdf_datapath = self._extract_datapaths(sig_fil.data)
        self._in_file = self._out_file = None
        self._det_count = self._other_count = self._other_count_out = 0
        self._streamcheck = True
        self._prevstream = None
        ReportDash.Version_metadata = ["NotAvailable"] * 6

    def _extract_datapaths(self, data):
        return [f"{s}/{sig['name']}" for s in data.get("streams", []) for sig in data.get(s, [])]

    def clear_data(self):
        self._in_file = self._out_file = None
        self._poi_paths.clear()
        self._det_count = self._other_count = self._other_count_out = 0
        self._streamcheck = True
        self._prevstream = None
        DCJsonToHtmlConvertor.sensor_list.clear()
        DCJsonToHtmlConvertor.stream_list.clear()
        DCJsonToHtmlConvertor.stream_signals.clear()
        DCJsonToHtmlConvertor.sensor_stream.clear()

    def collect_data(self, inp=None, out=None):
        self._in_file, self._out_file = inp, out
        self._parse_scan_index(str(inp), "in", "01_Scan_Index/scan_index")
        self._parse_scan_index(str(out), "out", "01_Scan_Index/scan_index")
        self._parse_hdf(str(inp), "in", self.dc_hdf_datapath)
        self._parse_hdf(str(out), "out", self.dc_hdf_datapath)
        self._mediator.notify_event("Detections", "DC_DET_RDS_UPDATED")
        self._mediator.notify_event(self, "DC_JSON_GEN_DONE")

    def _parse_scan_index(self, hdf_file, src, path):
        with h5py.File(hdf_file, 'r') as f:
            if path in f:
                data = f[path][()]
                if data.ndim in (1, 2):
                    unique, idx = np.unique(data, axis=0 if data.ndim == 2 else None, return_index=True)
                    data = unique[np.argsort(idx)]
                self._radar_ds.update_data("DC", "scan_index", data, src)

    def _parse_hdf(self, hdf_file, src, paths):
        with h5py.File(hdf_file, 'r') as f:
            for p in paths:
                if p not in f:
                    continue
                sig = os.path.basename(p)
                if p.startswith("02_Input_DC_Data/Detections/Detection_Info"):
                    if src == "in":
                        self._det_count += 1
                    for idx in range(4):
                        sensor = self.SENSOR_IDX[str(idx)]
                        if src == "in":
                            suffix = "" if (self._det_count - 1) // 8 == 0 else str((self._det_count - 1) // 8)
                            stream = f"Detections{suffix}"
                            DCJsonToHtmlConvertor.stream_list.append(stream)
                            DCJsonToHtmlConvertor.sensor_list.append(sensor)
                            DCJsonToHtmlConvertor.stream_signals[stream].add(sig)
                            DCJsonToHtmlConvertor.sensor_stream[sensor].add(stream)
                        det_sigs = ['range', 'range_rate', 'snr', 'amplitude', 'azimuth', 'elevation', 'std_range', 'std_range_rate', 'std_rcs', 'std_azimuth', 'std_elevation']
                        if sig in det_sigs:
                            data3d = f[p][()][:, :, idx]
                            valid = f['02_Input_DC_Data/Detections/Detection_Info/valid'][()][:, :, idx]
                            data = np.where(valid == 1, data3d, 0) if data3d.shape == valid.shape else data3d
                            rows = [r[r != 0.0] for r in data]
                            maxlen = max(len(r) for r in rows) if rows else 0
                            filtered = np.array([np.pad(r, (0, maxlen - len(r)), constant_values=0) for r in rows])
                            self._radar_ds.update_data(sensor, sig, filtered, src)
                        elif sig == "valid":
                            data3d = f[p][()][:, :, idx]
                            count = np.sum(data3d == 1, axis=1).reshape(-1, 1)
                            self._radar_ds.update_data(sensor, sig, count, src)
                elif p.startswith("00_metadata") and src == "in":
                    meta_map = {"Created_datetime": 0, "DC_version": 1, "OCG_version": 2, "OLP_version": 3, "SFL_version": 4, "Tracker_version": 5}
                    if sig in meta_map:
                        raw = f[p][()]
                        # Decode bytes to string if needed
                        if isinstance(raw, bytes):
                            ReportDash.Version_metadata[meta_map[sig]] = raw.decode('utf-8', errors='replace')
                        elif isinstance(raw, np.ndarray) and raw.dtype.kind in ('S', 'U', 'O'):
                            try:
                                ReportDash.Version_metadata[meta_map[sig]] = raw.item().decode('utf-8', errors='replace') if isinstance(raw.item(), bytes) else str(raw.item())
                            except:
                                ReportDash.Version_metadata[meta_map[sig]] = str(raw)
                        else:
                            ReportDash.Version_metadata[meta_map[sig]] = str(raw)
                else:
                    parts = p.split('/')
                    stream = parts[-2] if len(parts) > 2 else parts[0]
                    sig = os.path.basename(p)
                    if self._streamcheck:
                        self._prevstream = stream
                        self._streamcheck = False
                    if self._prevstream != stream:
                        self._other_count = self._other_count_out = 0
                        self._streamcheck = True
                    cnt = self._other_count if src == "in" else self._other_count_out
                    if src == "in": self._other_count += 1
                    else: self._other_count_out += 1
                    sensor = "DC"
                    suffix = "" if cnt // 8 == 0 else str(cnt // 8)
                    stream = f"{stream}{suffix}"
                    DCJsonToHtmlConvertor.sensor_list.append(sensor)
                    DCJsonToHtmlConvertor.stream_list.append(stream)
                    DCJsonToHtmlConvertor.stream_signals[stream].add(sig)
                    DCJsonToHtmlConvertor.sensor_stream[sensor].add(stream)
                    data = f[p][()]
                    # Skip non-numeric data (bytes, strings)
                    if isinstance(data, (bytes, str)):
                        continue
                    # Convert to numpy array if needed
                    if not isinstance(data, np.ndarray):
                        data = np.array(data)
                    # Handle different data shapes - DO NOT filter zeros, keep all data
                    if data.ndim == 2:
                        # Keep all data as-is without filtering zeros
                        filtered = data
                    elif data.ndim == 1:
                        filtered = data.reshape(-1, 1)
                    elif data.ndim == 0:
                        filtered = np.array([[data.item()]])
                    else:
                        filtered = data
                    if sig in ["f_moveable", "f_stationary"]:
                        data = np.sum(f[p][()] == 1, axis=1).reshape(-1, 1)
                        self._radar_ds.update_data(sensor, sig, data, src)
                    self._radar_ds.update_data(sensor, sig, filtered, src)

    def get_streams(self): return poi.poi_data_DC.get("streams", [])
    def get_signals(self, stream): return [s["name"] for s in poi.poi_data_DC.get(stream, [])]
    def get_plot_types(self, sig):
        for s in poi.poi_data_DC.get("streams", []):
            for sig_d in poi.poi_data_DC.get(s, []):
                if sig_d["name"] == sig: return sig_d["plots"]
        return []
    def get_unit(self, sig):
        for s in poi.poi_data_DC.get("streams", []):
            for sig_d in poi.poi_data_DC.get(s, []):
                if sig_d["name"] == sig: return sig_d["unit"]
        return "NA"
    def consume_event(self, s, e): pass
    def final_poi_sensor_datapath(self, d): pass
