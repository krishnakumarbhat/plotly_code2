import json
from typing import Dict, List, Any
import IPS.Metadata.GEN7V2.poi as sig

from IPS.Sig_Prep.isig_observer import ISigObserver
import h5py
from collections import defaultdict
from IPS.Utilities.hdf_validator import HDFValidator


class SignalFilter:
    poi_ref = defaultdict(list)
    can_poi_ref = defaultdict(list)
    sig_file1 = defaultdict(list)
    sig_file2 = defaultdict(list)
    common_sig = defaultdict(list)
    final_poi_sig = defaultdict(list)

    def __init__(self, poi_txt_file=None):
        if sig.datasource == "udp":
            self.data = sig.poi_data
        elif sig.datasource in ["udpdc", "dcdgps"]:
            self.data = sig.poi_data_DC

        elif sig.datasource == "mcip_can":
            self.data = sig.poi_data_MCIP_CAN
            self.chunks_list = [
                "001_004", "005_008", "009_012", "013_016", "017_020", "021_024", "025_028", "029_032", "033_036",
                "037_040", "041_044", "045_048", "049_052", "053_056", "057_060", "061_064"
            ]
        elif sig.datasource == "ceer_can":
            self.data = sig.poi_data_CEER_CAN
            self.chunks_list = [
                "001_004", "005_008", "009_012", "013_016", "017_020", "021_024", "025_028", "029_032",
                "033_036", "037_040", "041_044", "045_048", "049_052", "053_056", "057_060", "061_064",
                "065_068", "069_072", "073_076", "077_080", "081_084", "085_088", "089_092", "093_096",
                "097_100", "101_104", "105_108", "109_112", "113_116", "117_120", "121_124", "125_128",
                "129_132", "133_136", "137_140", "141_144", "145_148", "149_152", "153_156", "157_160",
                "161_164", "165_168", "169_172", "173_176", "177_180", "181_184", "185_188", "189_192",
                "193_196", "197_200"
            ]

        self.final_poi_sig_observer = []

    def initialize_sig_filter(self):
        if sig.datasource == "udp":
            self.data = sig.poi_data
        elif sig.datasource in ["udpdc", "dcdgps"]:
            self.data = sig.poi_data_DC
        elif sig.datasource == "mcip_can":
            self.data = sig.poi_data_MCIP_CAN
        elif sig.datasource == "ceer_can":
            self.data = sig.poi_data_CEER_CAN
        self.final_poi_sig_observer = []

    def attach_sig_observer(self, observer: ISigObserver):
        self.final_poi_sig_observer.append(observer)

    def notify_sig_observer(self):
        for k in SignalFilter.final_poi_sig:
            SignalFilter.final_poi_sig[k] = [item for sublist in SignalFilter.final_poi_sig[k] for item in sublist]

        for observer in self.final_poi_sig_observer:
            observer.final_poi_sensor_datapath(SignalFilter.final_poi_sig)

    def get_streams(self):
        stream_list = []
        # print("get_streams")
        # print(self.data["streams"])
        return self.data["streams"]

    def get_sig_in_stream(self, stream):
        sig_list = []
        for str_key in self.data.get(stream, []):
            sig_list.append(str_key["name"])
        return sig_list

    def get_sig_unit(self, stream, sig):
        # print(f"{sig}")
        # print(f"{stream}")
        for str_key in self.data.get(stream, []):
            # print(f"{str_key}")
            if str_key["name"] == sig:
                return str_key["unit"]

    def get_sig_plot_type(self, stream, sig):
        # print(f"{sig}")
        for str_key in self.data.get(stream, []):
            if str_key["name"] == sig:
                return str_key["plots"]

    def get_sig_plot_name(self, stream, sig):
        # print(f"{sig}")
        for str_key in self.data.get(stream, []):
            if str_key["name"] == sig:
                return str_key["pname"]

    def get_poi_ref(self):
        """
           Function uses Metadata POI signals and creates dataset path sensor wise.
           and will update dataset path to dictionary {poi_ref}

        """
        # print("get_poi_ref")

        for sensor in self.data.get("sensors", []):
            stream_list = self.get_streams()
            for stream in stream_list:
                sig_list = self.get_sig_in_stream(stream)
                for signal in sig_list:
                    SignalFilter.poi_ref[sensor].append(str(sensor) + "/" + str(stream) + "/" + str(signal))
                    # print(SignalFilter.poi_ref)

    def get_can_poi_ref(self):
        """
        Build CAN POI refs using exact signal names from poi_data_CAN,
        with chunking applied.
        Each sensor maps to its corresponding stream only (1:1 mapping).
        """

        sensor_list_ref = self.data.get("sensors", [])
        stream_list = self.get_streams()

        # Build direct mapping: MCIP_FR -> SRR_FR_DETECTION, etc.
        sensor_to_stream = {}
        for sensor in sensor_list_ref:
            suffix = sensor.split("_")[-1]  # FR, FL, RR, RL, FLR
            for stream in stream_list:
                if stream.endswith(f"{suffix}_DETECTION"):
                    sensor_to_stream[sensor] = stream
                    break

        # clear previous data
        for sensor in sensor_list_ref:
            SignalFilter.can_poi_ref[sensor] = []

        # print("sensor_list_ref", sensor_list_ref)

        for sensor, matched_stream in sensor_to_stream.items():
            # print(f"Processing sensor: {sensor} -> {matched_stream}")
            sig_list = self.get_sig_in_stream(matched_stream)
            if not sig_list:
                # print(f"  No signals found in {matched_stream}")
                continue

            for chunk_spec in self.chunks_list:
                start_str, end_str = chunk_spec.split("_")
                start_index, end_index = int(start_str), int(end_str)

                stream_chunked = f"{matched_stream}_{chunk_spec}"

                for signal in sig_list:
                    # keep timestamp signals as-is
                    if signal.startswith("timestamp_"):
                        path = f"{sensor}/timestamp_{stream_chunked}"
                        SignalFilter.can_poi_ref[sensor].append(path)
                    else:
                        for index in range(start_index, end_index + 1):
                            signal_indexed = f"{signal}_{str(index).zfill(3)}"
                            path = f"{sensor}/{stream_chunked}/{signal_indexed}"
                            SignalFilter.can_poi_ref[sensor].append(path)

                # print(f"    Added {len(SignalFilter.can_poi_ref[sensor])} paths for {sensor}")

    def parse_group_signals(self, file1, file_type):
        # print("get_group_signals")
        
        # Validate HDF file before processing
        is_valid, error_msg = HDFValidator.validate_hdf_file(file1)
        if not is_valid:
            raise ValueError(f"HDF file validation failed for {file1}: {error_msg}")
        
        groups = []
        datasets = []

        def visit_func(name, node):
            if isinstance(node, h5py.Group):
                groups.append(name)
            elif isinstance(node, h5py.Dataset):
                sensor = name.split('/')[0]
                if file_type == "in":
                    SignalFilter.sig_file1[sensor].append(name)
                if file_type == "out":
                    SignalFilter.sig_file2[sensor].append(name)
                datasets.append(name)

        with h5py.File(file1, 'r') as f:
            f.visititems(visit_func)

    def fiter_common_signals(self):
        for key in SignalFilter.sig_file1.keys() & SignalFilter.sig_file2.keys():  # Only common keys
            try:
                # Ensure values are lists; convert single strings or wrap if needed
                val1 = SignalFilter.sig_file1[key]
                val2 = SignalFilter.sig_file2[key]
                # print(key)

                # Check and flatten if value is a list of lists
                if isinstance(val1, list) and any(isinstance(v, list) for v in val1):
                    val1 = [item for sublist in val1 for item in (sublist if isinstance(sublist, list) else [sublist])]

                if isinstance(val2, list) and any(isinstance(v, list) for v in val2):
                    val2 = [item for sublist in val2 for item in (sublist if isinstance(sublist, list) else [sublist])]

                # Make sure val1 and val2 are now flat lists of strings
                val1 = val1 if isinstance(val1, list) else [val1]
                val2 = val2 if isinstance(val2, list) else [val2]

                # print("val1", val1)
                # print("val2", val2)
                # Now safely compare
                common_values = list(set(val1) & set(val2))
                # print("@@@common_values", common_values)
                if common_values:
                    SignalFilter.common_sig[key].append(common_values)
                    # print(SignalFilter.common_sig)
            except Exception as e:
                print(f"Error processing key '{key}': {e}")
                continue

    def final_poi_signals(self):
        # print("final_poi_signals")
        for key in SignalFilter.common_sig.keys() & SignalFilter.poi_ref.keys():  # Only common keys
            try:
                # Ensure values are lists; convert single strings or wrap if needed
                val1 = SignalFilter.common_sig[key]
                val2 = SignalFilter.poi_ref[key]
                # print(key)

                # Check and flatten if value is a list of lists
                if isinstance(val1, list) and any(isinstance(v, list) for v in val1):
                    val1 = [item for sublist in val1 for item in (sublist if isinstance(sublist, list) else [sublist])]

                if isinstance(val2, list) and any(isinstance(v, list) for v in val2):
                    val2 = [item for sublist in val2 for item in (sublist if isinstance(sublist, list) else [sublist])]

                # Make sure val1 and val2 are now flat lists of strings
                val1 = val1 if isinstance(val1, list) else [val1]
                val2 = val2 if isinstance(val2, list) else [val2]

                # print("val1", val1)
                # print("val2", val2)
                # Now safely compare
                common_values = list(set(val1) & set(val2))
                # print("@@@common_values", common_values)
                if common_values:
                    SignalFilter.final_poi_sig[key].append(common_values)
                    # print(SignalFilter.final_poi_sig)
            except Exception as e:
                print(f"Error processing key '{key}': {e}")
                continue

    def final_can_poi_signals(self):
        # print("final_can_poi_signals")
        for key in SignalFilter.common_sig.keys() & SignalFilter.can_poi_ref.keys():  # Only common keys
            try:
                # Ensure values are lists; convert single strings or wrap if needed
                val1 = SignalFilter.common_sig[key]
                val2 = SignalFilter.can_poi_ref[key]
                # print(key)

                # Check and flatten if value is a list of lists
                if isinstance(val1, list) and any(isinstance(v, list) for v in val1):
                    val1 = [item for sublist in val1 for item in (sublist if isinstance(sublist, list) else [sublist])]

                if isinstance(val2, list) and any(isinstance(v, list) for v in val2):
                    val2 = [item for sublist in val2 for item in (sublist if isinstance(sublist, list) else [sublist])]

                # Make sure val1 and val2 are now flat lists of strings
                val1 = val1 if isinstance(val1, list) else [val1]
                val2 = val2 if isinstance(val2, list) else [val2]

                # print("val1", val1)
                # print("val2", val2)
                # Now safely compare
                common_values = list(set(val1) & set(val2))
                # print("@@@common_values", common_values)
                if common_values:
                    SignalFilter.final_poi_sig[key].append(common_values)
                    # print(SignalFilter.final_poi_sig)
            except Exception as e:
                print(f"Error processing key '{key}': {e}")
                continue

    def clear_data(self):
        # self.data.clear()
        # self.final_poi_sig_observer.clear()

        # SignalFilter.poi_ref.clear()
        SignalFilter.sig_file1.clear()
        SignalFilter.sig_file2.clear()
        # SignalFilter.common_sig.clear()
        # SignalFilter.final_poi_sig.clear()
