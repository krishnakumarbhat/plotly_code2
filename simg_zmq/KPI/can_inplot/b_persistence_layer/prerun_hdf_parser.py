import h5py
import logging
import os
from collections import deque
from can_inplot.sensor_matcher import match_sensor_pairs, match_stream_pairs


class PreRun:
    """
    PreRun performs pre-parsing of the provided input and output HDF5 files for a specified sensor.
    It attempts to retrieve any missing data from the input file by comparing it to the output file.
    """

    def __init__(self, input_file, output_file):
        """
        Initialize the PreRun instance.

        Args:
            input_file (str): Path to the input HDF5 file.
            output_file (str): Path to the output HDF5 file.
        """
        self.input_file = input_file
        self.output_file = output_file

        # Lists to store names of datasets and groups.
        self.datasetsin = []
        self.datasetsout = []
        self.grp_listin = []
        self.grp_listout = []

        self.missing_data = None
        self.sensor_list = None
        self.streams = None
        self.read_error = None
        self.get_missing_data()

    def get_missing_data(self):
        """
        Compares the data groups of the specified sensor in the input and output HDF5 files.
        It constructs lists of dataset names and group names and then computes what is missing.

        Returns:
            A dictionary with keys 'missing_groups' and 'missing_datasets' that contain the names
            of groups and datasets present in the output file but missing in the input file.
            Also returns the sensor_list and streams.
        """
        level1_groups_in = []
        level2_groups_in = []
        level1_groups_out = []
        level2_groups_out = []

        # Function to traverse an HDF5 file and collect data in a single pass
        def traverse_hdf(
            file_path, datasets_list, grp_list, level1_groups, level2_groups
        ):
            with h5py.File(file_path, "r") as hdf_file:
                queue = deque([(hdf_file, "")])
                while queue:
                    group, prefix = queue.popleft()
                    for name, item in group.items():
                        full_name = f"{prefix}/{name}" if prefix else name
                        if isinstance(item, h5py.Dataset):
                            datasets_list.append(full_name)
                        elif isinstance(item, h5py.Group):
                            if not prefix:
                                level1_groups.append(name)
                            elif prefix in level1_groups:
                                level2_groups.append(name)
                            queue.append((item, full_name))
                            grp_list.append(full_name)

        try:
            # Traverse input and output files in parallel (still sequential execution but combined logic)
            traverse_hdf(
                self.input_file,
                self.datasetsin,
                self.grp_listin,
                level1_groups_in,
                level2_groups_in,
            )
            traverse_hdf(
                self.output_file,
                self.datasetsout,
                self.grp_listout,
                level1_groups_out,
                level2_groups_out,
            )
        except Exception as exc:
            self.read_error = str(exc)
            self.missing_data = {
                "missing_groups": [],
                "missing_datasets": [],
                "read_error": self.read_error,
            }
            self.sensor_list = []
            self.streams = []
            return (
                self.missing_data,
                self.sensor_list,
                self.streams,
                level1_groups_in,
                level1_groups_out,
                level2_groups_in,
                level2_groups_out,
            )

        # Compute missing groups and datasets using set operations
        missing_groups_in = set(self.grp_listout) - set(self.grp_listin)
        missing_groups_out = set(self.grp_listin) - set(self.grp_listout)
        missing_datasets_in = set(self.datasetsout) - set(self.datasetsin)
        missing_datasets_out = set(self.datasetsin) - set(self.datasetsout)

        # Union operations for final missing elements
        missing_groups = missing_groups_in.union(missing_groups_out)
        missing_datasets = missing_datasets_in.union(missing_datasets_out)

        # Combine results into a dictionary
        self.missing_data = {
            "missing_groups": list(missing_groups),
            "missing_datasets": list(missing_datasets),
        }

        # Get the sensor_list and streams - use set intersection for efficiency
        # Sensor group names can differ between the input and output HDF files
        # (e.g. input "CEER_ FL" vs output "CEER_FL", or even renamed families
        # like "MCIP_FL" vs "CEER_FL").  Robust tiered matching handles this:
        # normalized exact -> position token (FL/FR/RL/RR/FLR) -> fuzzy.
        self.sensor_map_in = {}
        self.sensor_map_out = {}
        self.stream_map_in = {}
        self.stream_map_out = {}

        if level1_groups_in and level1_groups_out:
            sensor_pairs, unmatched_in, unmatched_out = match_sensor_pairs(
                level1_groups_in, level1_groups_out
            )
            self.sensor_list = sorted(sensor_pairs.keys())
            self.sensor_map_in = {key: pair[0] for key, pair in sensor_pairs.items()}
            self.sensor_map_out = {key: pair[1] for key, pair in sensor_pairs.items()}
            if unmatched_in or unmatched_out:
                logging.debug(
                    "Unmatched sensor groups in=%s out=%s",
                    unmatched_in,
                    unmatched_out,
                )
        else:
            self.sensor_list = list(set(level1_groups_in) & set(level1_groups_out))
            for sensor in self.sensor_list:
                self.sensor_map_in[sensor] = sensor
                self.sensor_map_out[sensor] = sensor

        # Streams are matched per sensor and per category (DETECTION/ALIGNMENT/
        # HEADER/...) plus numeric chunk, because stream group names are
        # sensor-prefixed (e.g. "SRR_FL_DETECTION_001_004" vs "FLR_DETECTION_001_004")
        # and the producer may rename them freely.  Maps are therefore keyed by
        # canonical sensor id -> {canonical stream key: actual group name}.
        if level2_groups_in and level2_groups_out:
            stream_keys_all = []
            for sensor in self.sensor_list:
                s_in = self.sensor_map_in.get(sensor, sensor)
                s_out = self.sensor_map_out.get(sensor, sensor)
                in_names = [
                    g.split("/", 1)[1]
                    for g in self.grp_listin
                    if g.startswith(s_in + "/")
                ]
                out_names = [
                    g.split("/", 1)[1]
                    for g in self.grp_listout
                    if g.startswith(s_out + "/")
                ]
                stream_pairs, _, _ = match_stream_pairs(in_names, out_names)
                self.stream_map_in[sensor] = {
                    key: pair[0] for key, pair in stream_pairs.items()
                }
                self.stream_map_out[sensor] = {
                    key: pair[1] for key, pair in stream_pairs.items()
                }
                stream_keys_all.extend(stream_pairs.keys())
            self.streams = sorted(set(stream_keys_all))
        else:
            self.streams = list(set(level2_groups_in) & set(level2_groups_out))
            for sensor in self.sensor_list:
                self.stream_map_in[sensor] = {
                    stream: stream for stream in self.streams
                }
                self.stream_map_out[sensor] = {
                    stream: stream for stream in self.streams
                }

        return (
            self.missing_data,
            self.sensor_list,
            self.streams,
            level1_groups_in,
            level1_groups_out,
            level2_groups_in,
            level2_groups_out,
        )


class PreRunPaired:
    """
    PreRunPaired performs pre-parsing of the provided paired HDF5 files.
    It analyzes each input-output pair independently, not requiring common streams across all files.
    """

    def __init__(self, input_files, output_files):
        """
        Initialize the PreRunPaired instance.

        Args:
            input_files (list): List of paths to input HDF5 files
            output_files (list): List of paths to output HDF5 files
        """
        self.input_files = input_files
        self.output_files = output_files

        # Check that input and output files have the same length
        if len(input_files) != len(output_files):
            raise ValueError("Number of input files must match number of output files")

        # Dictionary to hold sensors and streams for each pair
        # Structure: { pair_index: { 'sensors': [], 'streams': {} } }
        self.pair_data = {}

        self.analyze_files()

    def get_groups_and_datasets(self, file_path):
        """
        Extracts groups and datasets from an HDF5 file.

        Args:
            file_path (str): Path to the HDF5 file.

        Returns:
            tuple: (level1_groups, level2_groups_by_parent, datasets)
        """
        level1_groups = []
        level2_groups_by_parent = {}
        datasets = []

        try:
            with h5py.File(file_path, "r") as hdf_file:
                # Get top-level groups (sensors)
                for key in hdf_file.keys():
                    if (
                        key != "Header"
                        and key != "data"
                        and isinstance(hdf_file[key], h5py.Group)
                    ):
                        level1_groups.append(key)
                        level2_groups_by_parent[key] = []

                        # Get second-level groups (streams) for each sensor
                        for stream in hdf_file[key].keys():
                            if isinstance(hdf_file[key][stream], h5py.Group):
                                level2_groups_by_parent[key].append(stream)

                                # Collect datasets in each stream
                                for dataset in hdf_file[key][stream]:
                                    if isinstance(
                                        hdf_file[key][stream][dataset], h5py.Dataset
                                    ):
                                        datasets.append(f"{key}/{stream}/{dataset}")
        except Exception as e:
            print(f"Error analyzing {file_path}: {str(e)}")

        return level1_groups, level2_groups_by_parent, datasets

    def analyze_files(self):
        """
        Analyzes each pair of files independently to find sensors and streams.
        Each input file is only compared with its corresponding output file.
        """
        # Process each pair of files
        for i, (input_file, output_file) in enumerate(
            zip(self.input_files, self.output_files)
        ):
            # Initialize data structure for this pair
            self.pair_data[i] = {
                "sensors": [],
                "streams": {},
                "input_file": input_file,
                "output_file": output_file,
            }

            # Get structure from input and output files
            input_groups, input_streams, input_datasets = self.get_groups_and_datasets(
                input_file
            )
            output_groups, output_streams, output_datasets = (
                self.get_groups_and_datasets(output_file)
            )

            # Find common sensors between input and output
            input_sensors = set(input_groups)
            output_sensors = set(output_groups)
            common_sensors = list(input_sensors & output_sensors)
            self.pair_data[i]["sensors"] = common_sensors

            # Find streams for each sensor in this pair
            for sensor in common_sensors:
                input_sensor_streams = set(input_streams.get(sensor, []))
                output_sensor_streams = set(output_streams.get(sensor, []))
                common_streams = input_sensor_streams & output_sensor_streams

                if common_streams:
                    self.pair_data[i]["streams"][sensor] = list(common_streams)

            # Print summary for this pair
            print(f"\nFile Pair {i + 1} Analysis:")
            print(f"  Input: {os.path.basename(input_file)}")
            print(f"  Output: {os.path.basename(output_file)}")

            if common_sensors:
                print(
                    f"  Found {len(common_sensors)} sensors: {', '.join(common_sensors)}"
                )

                total_streams = sum(
                    len(streams) for streams in self.pair_data[i]["streams"].values()
                )
                print(f"  Found {total_streams} streams")

                for sensor, streams in self.pair_data[i]["streams"].items():
                    print(
                        f"    - {sensor}: {len(streams)} streams: {', '.join(streams)}"
                    )
            else:
                print("  No common sensors found in this pair")
