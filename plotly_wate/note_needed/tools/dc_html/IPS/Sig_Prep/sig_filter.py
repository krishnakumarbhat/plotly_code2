import h5py, ast
from collections import defaultdict
import IPS.Metadata.GEN7V2.poi as sig
from IPS.Sig_Prep.isig_observer import ISigObserver

class SignalFilter:
    poi_ref = defaultdict(list)
    can_poi_ref = defaultdict(list)
    sig_file1 = defaultdict(list)
    sig_file2 = defaultdict(list)
    common_sig = defaultdict(list)
    final_poi_sig = defaultdict(list)

    def __init__(self, poi_txt_file=None):
        ds = sig.datasource
        if ds == "udp":
            self.data = sig.poi_data
        elif ds == "udpdc":
            self.data = self._load_poi_dict(poi_txt_file) if poi_txt_file else sig.poi_data_DC
        elif ds == "mcip_can":
            self.data = sig.poi_data_MCIP_CAN
            self.chunks_list = [f"{i:03d}_{i+3:03d}" for i in range(1, 65, 4)]
        elif ds == "ceer_can":
            self.data = sig.poi_data_CEER_CAN
            self.chunks_list = [f"{i:03d}_{i+3:03d}" for i in range(1, 201, 4)]
        else:
            self.data = {}
        self.final_poi_sig_observer = []

    def _load_poi_dict(self, fp):
        try:
            with open(fp, 'r') as f:
                content = f.read().strip()
                return ast.literal_eval(content)
        except FileNotFoundError:
            print(f"Warning: {fp} not found, using default poi data")
            return sig.poi_data_DC
        except SyntaxError as e:
            print(f"Warning: Syntax error in {fp}: {e}, using default poi data")
            return sig.poi_data_DC

    def attach_sig_observer(self, obs: ISigObserver):
        self.final_poi_sig_observer.append(obs)

    def notify_sig_observer(self):
        for k in SignalFilter.final_poi_sig:
            SignalFilter.final_poi_sig[k] = [i for sub in SignalFilter.final_poi_sig[k] for i in sub]
        for obs in self.final_poi_sig_observer:
            obs.final_poi_sensor_datapath(SignalFilter.final_poi_sig)

    def get_streams(self): return self.data.get("streams", [])
    def get_sig_in_stream(self, stream): return [s["name"] for s in self.data.get(stream, [])]
    def get_sig_unit(self, stream, sig):
        for s in self.data.get(stream, []):
            if s["name"] == sig: return s["unit"]
    def get_sig_plot_type(self, stream, sig):
        for s in self.data.get(stream, []):
            if s["name"] == sig: return s["plots"]
    def get_sig_plot_name(self, stream, sig):
        for s in self.data.get(stream, []):
            if s["name"] == sig: return s["pname"]

    def get_poi_ref(self):
        """
           Function uses Metadata POI signals and creates dataset path sensor wise.
           and will update dataset path to dictionary {poi_ref}

        """
        for sensor in self.data.get("sensors", []):
            for stream in self.get_streams():
                for signal in self.get_sig_in_stream(stream):
                    SignalFilter.poi_ref[sensor].append(f"{sensor}/{stream}/{signal}")

    def get_can_poi_ref(self):
        """
        Build CAN POI refs using exact signal names from poi_data_CAN,
        with chunking applied.
        Each sensor maps to its corresponding stream only (1:1 mapping).
        """
        sensors = self.data.get("sensors", [])
        streams = self.get_streams()
        sensor_stream = {s: next((st for st in streams if st.endswith(f"{s.split('_')[-1]}_DETECTION")), None) for s in sensors}
        for s in sensors: SignalFilter.can_poi_ref[s] = []
        for sensor, stream in sensor_stream.items():
            if not stream: continue
            sigs = self.get_sig_in_stream(stream)
            for chunk in self.chunks_list:
                start, end = int(chunk[:3]), int(chunk[4:])
                sc = f"{stream}_{chunk}"
                for sig in sigs:
                    if sig.startswith("timestamp_"):
                        SignalFilter.can_poi_ref[sensor].append(f"{sensor}/timestamp_{sc}")
                    else:
                        for i in range(start, end + 1):
                            SignalFilter.can_poi_ref[sensor].append(f"{sensor}/{sc}/{sig}_{i:03d}")

    def parse_group_signals(self, file, ftype):
        store = SignalFilter.sig_file1 if ftype == "in" else SignalFilter.sig_file2
        with h5py.File(file, 'r') as f:
            f.visititems(lambda n, node: store[n.split('/')[0]].append(n) if isinstance(node, h5py.Dataset) else None)

    def fiter_common_signals(self):
        for k in SignalFilter.sig_file1.keys() & SignalFilter.sig_file2.keys():
            try:
                v1, v2 = self._flatten(SignalFilter.sig_file1[k]), self._flatten(SignalFilter.sig_file2[k])
                common = list(set(v1) & set(v2))
                if common: SignalFilter.common_sig[k].append(common)
            except Exception as e:
                print(f"Error key '{k}': {e}")

    def _flatten(self, v):
        if isinstance(v, list) and any(isinstance(x, list) for x in v):
            return [i for sub in v for i in (sub if isinstance(sub, list) else [sub])]
        return v if isinstance(v, list) else [v]

    def final_poi_signals(self):
        self._compute_final(SignalFilter.poi_ref)

    def final_can_poi_signals(self):
        self._compute_final(SignalFilter.can_poi_ref)

    def _compute_final(self, ref):
        for k in SignalFilter.common_sig.keys() & ref.keys():
            try:
                v1, v2 = self._flatten(SignalFilter.common_sig[k]), self._flatten(ref[k])
                common = list(set(v1) & set(v2))
                if common: SignalFilter.final_poi_sig[k].append(common)
            except Exception as e:
                print(f"Error key '{k}': {e}")

    def clear_data(self):
        SignalFilter.sig_file1.clear()
        SignalFilter.sig_file2.clear()
