import h5py
import os
from collections import deque


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
        self.sensor_list = list(set(level1_groups_in) & set(level1_groups_out))
        self.streams = list(set(level2_groups_in) & set(level2_groups_out))

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
