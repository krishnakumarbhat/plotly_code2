from typing import Dict, List, Any
import logging
import numpy as np


class KPI_DataModelStorage:
    """
    A storage class for managing signal data with hierarchical relationships.
    Implements a bidirectional mapping between values and signals with support for parent-child relationships.
    """

    def __init__(self):
        # Bidirectional mapping between values and signals
        self._value_to_signal: Dict[str, str] = {}
        self._signal_to_value: Dict[str, Any] = {}

        # Main data container for storing scan index data
        self._data_container: Dict[str, List] = {}

        # Optional set of scan indices to skip when ingesting datasets
        self._missing_indices = set()

        # Counter for unique identifiers
        self._parent_counter: int = -1
        self._child_counter: int = -1

    def initialize(self, scan_index, sensor, missing_idx=None) -> None:
        """
        Initialize the data container with scan indices.

        Args:
            scan_index: List or NumPy array of scan indices to initialize the container with
        """

        # Track indices that should be skipped later, if provided
        self._missing_indices = set(missing_idx) if missing_idx is not None else set()

        # Check if scan_index is sorted and sequential
        if len(scan_index) > 0:
            expected_scan_index = list(range(min(scan_index), max(scan_index) + 1))
            freq = {}
            duplicates = []
            for idx in scan_index:
                freq[idx] = freq.get(idx, 0) + 1
            duplicates = sorted([k for k, v in freq.items() if v > 1])

            # Find missing indices
            missing_indices = [i for i in expected_scan_index if i not in scan_index]

            if missing_indices:
                logging.debug(
                    f"Missing scan indices at this signal {sensor}: {missing_indices}"
                )
            if duplicates:
                logging.debug(
                    f"Duplicate scan indices at {sensor}: {duplicates}"
                )
    
        # Use defaultdict to avoid checking if key exists later
        self._data_container = {j: [] for j in scan_index}
        
    def initialize_comp(self, scan_index, sensor, data_cont=None) -> None:
        """
        Compare and validate data container with scan indices for stream processing.
        This method ensures data container keys match the expected scan indices.

        Args:
            scan_index: List or NumPy array of scan indices to validate against
            sensor: Sensor identifier for logging purposes
            data_cont: Optional data container to validate (defaults to self._data_container)
        """
        if data_cont is None:
            data_cont = self._data_container

        if scan_index is None or len(scan_index) == 0:
            logging.warning(f"Empty or None scan_index provided for sensor {sensor}")
            return

        # Convert scan_index to set for efficient lookup
        scan_index_set = set(scan_index)
        
        # Check if scan_index is sorted and sequential
        if len(scan_index) > 0:
            expected_scan_index = list(range(min(scan_index), max(scan_index) + 1))
            freq = {}
            duplicates = []
            
            # Count frequency of each scan index
            for idx in scan_index:
                freq[idx] = freq.get(idx, 0) + 1
            duplicates = sorted([k for k, v in freq.items() if v > 1])

            # Find missing indices in the expected range
            missing_indices = [i for i in expected_scan_index if i not in scan_index_set]

            # Log validation results
            if missing_indices:
                logging.debug(
                    f"Missing scan indices in expected range for sensor {sensor}: {missing_indices}"
                )
            if duplicates:
                logging.debug(
                    f"Duplicate scan indices found for sensor {sensor}: {duplicates}"
                )

        # Validate data container keys against scan indices
        data_cont_keys = set(data_cont.keys()) if data_cont else set()
        
        # Find keys in data container that are not in scan_index
        extra_keys = data_cont_keys - scan_index_set
        if extra_keys:
            logging.debug(
                f"Extra keys in data container for sensor {sensor}: {sorted(extra_keys)}"
            )

        # Find scan indices that are missing from data container
        missing_keys = scan_index_set - data_cont_keys
        if missing_keys:
            logging.debug(
                f"Missing keys in data container for sensor {sensor}: {sorted(missing_keys)}"
            )

        # Log summary of validation
        logging.info(
            f"Data container validation for sensor {sensor}:"
            f"scan_indices={len(scan_index)}, data_keys={len(data_cont_keys)}, "
            f"missing={len(missing_keys)}, extra={len(extra_keys)}"
        )
        return missing_keys , extra_keys

            
    def init_parent(self, stream_name) -> None:
        """Reset child counter when starting a new parent group."""
        self._parent_counter += 1
        self._child_counter = -1
        self.stream_name = stream_name

    def set_value(self, dataset: Any, signal_name: str, grp_name: str) -> str:
        """
        Set a value in the storage with group relationship.

        Args:
            dataset: The data to store
            scan_index: The scan index to store the data under
            signal_name: Name of the signal
            grp_name: Name of the group this signal belongs to

        Returns:
            str: The generated key for the stored data
        """

        # Check if this is a new parent group
        is_new_parent = (
            grp_name not in self._signal_to_value and self._child_counter == -1
        )

        if is_new_parent:
            # Handle new parent group
            key_grp = f"{self._parent_counter}_None"
            self._child_counter += 1
            key_stream = f"{self._parent_counter}_{self._child_counter}"

            # Process and store the data
            if self._missing_indices:
                missing_set = set(self._missing_indices)
                # Filter out missing indices in a single pass
                dataset = [item for idx, item in enumerate(dataset) if idx not in missing_set]
            
            self._process_dataset(dataset, key_stream, signal_name, key_grp)

            return key_stream

        # Filter out missing indices from the dataset before processing
        if self._missing_indices:
            missing_set = set(self._missing_indices)
            dataset = [item for idx, item in enumerate(dataset) if idx not in missing_set]

        # Get the length of dataset and data_container
        # dataset_len = len(dataset)- len(getattr(self, "_missing_indices", set())) if dataset is not None else 0
        dataset_len = len(dataset) if dataset is not None else 0
        container_len = len(self._data_container)

        # Skip if lengths don't match
        if dataset_len != container_len:
            logging.debug(
                f"Skipping child processing for {signal_name} in {grp_name}: dataset length ({dataset_len}) does not match scan indices length ({container_len})"
            )
            return ""

        # Handle child item
        self._child_counter += 1
        key = f"{self._parent_counter}_{self._child_counter}"

        # Process and store data for child (dataset is now filtered)
        for idx, (row, scanidx) in enumerate(zip(dataset, self._data_container)):
            # Skip rows whose scan index is marked as missing for this storage
            # if idx in getattr(self, "_missing_indices", set()):
            #     continue
            if isinstance(row, np.ndarray):
                rounded_row = np.round(row.astype(float), decimals=2)
                self._data_container[scanidx][-1].append(rounded_row)
            else:
                self._data_container[scanidx][-1].append(row)

        # Update mappings
        self._value_to_signal[key] = signal_name

        # Update signal-to-value mapping with optimized approach
        if signal_name not in self._signal_to_value:
            self._signal_to_value[signal_name] = [{grp_name: key}]
        else:
            signal_value = self._signal_to_value[signal_name]
            if isinstance(signal_value, list):
                signal_value.append({grp_name: key})
            else:
                self._signal_to_value[signal_name] = [{grp_name: key}]

        return key

    def _process_dataset(self, dataset, key_stream, signal_name, key_grp):

        # Get the length of dataset and data_container
        dataset_len = len(dataset) if dataset is not None else 0
        container_len = len(self._data_container)

        # Skip if lengths don't match
        if dataset_len != container_len:
            logging.debug(
                f"Skiping plot for {signal_name}: dataset length ({dataset_len}) does not match scan indices length ({container_len})"
            )
            return

        # Process all rows in the dataset and store them (dataset is pre-filtered)
        for idx, (row, scanidx) in enumerate(zip(dataset, self._data_container)):

        # """Helper method to process and store dataset for new parent groups."""
            # if idx in getattr(self, "_missing_indices", set()):
            #     continue
                
            if isinstance(row, np.ndarray):
                rounded_row = np.round(row.astype(float), decimals=2)
                self._data_container[scanidx].append([rounded_row])
            else:
                self._data_container[scanidx].append([row])
        # Update mappings
        self._value_to_signal[key_stream] = signal_name
        self._value_to_signal[key_grp] = self.stream_name
        self._signal_to_value[signal_name] = key_stream
        self._signal_to_value[self.stream_name] = key_grp

    def clear(self) -> None:
        """Clear all stored data and reset counters."""
        self._value_to_signal.clear()
        self._signal_to_value.clear()
        self._data_container.clear()
        self._parent_counter = 0
        self._child_counter = -1

    @staticmethod
    def round_to_2_decimals(data):
        # If data is already a NumPy array, round directly
        if isinstance(data, np.ndarray):
            rounded_arr = np.round(data, 2)
            return tuple(rounded_arr.tolist())

        # If data is a tuple or list, convert to np.array, round, convert back to tuple
        elif isinstance(data, (tuple, list)):
            arr = np.array(data)
            rounded_arr = np.round(arr, 2)
            return tuple(rounded_arr.tolist())

        # If scalar (int, float, np scalar), round directly and return 1-element tuple
        elif isinstance(data, (int, float, np.uint32, np.uint8, np.float32)):
            return (round(float(data), 2),)

        # If anything else, just return as single-element tuple (rounded if possible)
        else:
            try:
                return (round(float(data), 2),)
            except Exception:
                return (data,)


    @staticmethod
    def get_value(data, signal_name, grp_name=None):
        """
        Get all data values for a specific signal, returned as a list of [scan_index, value(s)] pairs.

        Args:
            data (KPI_DataModelStorage): The data storage instance.
            signal_name (str): Name of the signal to get data for.
            grp_name (str, optional): The group name to filter by if the signal exists in multiple groups. Defaults to None.

        Returns:
            tuple: A tuple containing:
                - list: A list where each item is a sublist [scan_index, value(s)...].
                - str: A status string, "success" or an error message.
        """
        
        signal_info = data._signal_to_value.get(signal_name)

        # Return early if signal not found in either map
        if not signal_info:
            return [], f"Signal '{signal_name}' not found in data model."

        storage_key = None
        if isinstance(signal_info, str):
            storage_key = signal_info
        elif isinstance(signal_info, list):
            if grp_name:
                # Find the key for the specified group
                for item in signal_info:
                    if grp_name in item:
                        storage_key = item[grp_name]
                        break
            elif signal_info:
                # Default to the first available key if no group is specified
                storage_key = list(signal_info[0].values())[0]

        if not storage_key:
            return [], f"Could not find a valid storage key for signal '{signal_name}' with group '{grp_name}'."

        try:
            grp_idx, plt_idx = map(int, storage_key.split("_"))
        except (ValueError, AttributeError):
            return [], f"Invalid storage key format '{storage_key}' for signal '{signal_name}'."

        all_values = []
        scan_indices = list(data._data_container.keys())

        for scan_idx in scan_indices:
            scan_data = data._data_container.get(scan_idx)
            if scan_data and grp_idx < len(scan_data):
                group_data = scan_data[grp_idx]
                if plt_idx < len(group_data):
                    value = group_data[plt_idx]
                    if np.isscalar(value):
                        row = np.array([float(value)])
                    else:
                        row = np.asarray(value).ravel()
                    all_values.append(row)

        # After the loop
        if all_values:
            # Find maximum length of rows
            max_len = max(len(r) for r in all_values)
            # Pad all rows to same length with NaN, but use a more robust approach
            padded_rows = []
            for row in all_values:
                if len(row) < max_len:
                    # Pad with NaN values
                    padded = np.pad(row, (0, max_len - len(row)), constant_values=np.nan)
                else:
                    padded = row
                padded_rows.append(padded)

            stacked = np.vstack(padded_rows)
        else:
            stacked = np.empty((0, 0))

        return stacked, "success"



# value = group_data[plt_idx]
# # Ensure value is converted into a 1D sequence, then make a row with scan_idx first

# else:


# # After collecting rows, stack into a single 2D numpy array
# if all_values:
#     all_values = np.vstack(all_values)
# else:
#     all_values = np.empty((0, 0))





    @staticmethod
    def get_data(
        input_data, output_data, signal_name, grp_name=None
    ):  # need to add grp_name funtionality
        """
        Get data records for the specified signal from input and output data.

        This static method can be called from anywhere by providing all required parameters.

        Args:

            input_data: Container for input data
            output_data: Container for output data
            signal_name: Name of the signal to get data for
            scan_indexs: List of unique scan indices
        Returns:
            Dictionary with keys containing lists of [scan_idx, value] pairs:
            - 'SI': Scan indices for matched data
            - 'I': All input values
            - 'O': All output values
            - 'MI': Values missing in input
            - 'MO': Values missing in output
            - 'match': Count of matched elements
            - 'mismatch': Count of mismatched elements
        """
        data_dict = {
            "SI": None,  # Scan indices for matched data
            "I": None,  # All input values
            "O": None,  # All output values
            "MI": [[], []],  # for missing in input
            "MO": [[], []],  # for missing in output
            "match": 0,
            "mismatch": 0,
        }

        # Check if signal exists in input and output maps
        unique_in = (
            input_data._signal_to_value.get(signal_name)
            if input_data._signal_to_value
            else None
        )
        unique_out = (
            output_data._signal_to_value.get(signal_name)
            if output_data._signal_to_value
            else None
        )

        # Return early if signal not found in either map
        if not unique_in and not unique_out:
            return "no_data_in_hdf", {}

        grp_idx_in = None
        plt_idx_in = None

        grp_idx_out = None
        plt_idx_out = None
        # try:
        # Parse group and plot indices for input and output
        if isinstance(unique_in, str) and isinstance(unique_out, str):
            grp_idx_in, plt_idx_in = (
                map(int, unique_in.split("_")) if unique_in else (None, None)
            )
            grp_idx_out, plt_idx_out = (
                map(int, unique_out.split("_")) if unique_out else (None, None)
            )
        elif isinstance(unique_in, list) and isinstance(unique_out, list):
            grp_idx_in, plt_idx_in = (
                map(int, list(unique_in[0].values())[0].split("_"))
                if unique_in
                else (None, None)
            )
            grp_idx_out, plt_idx_out = (
                map(int, list(unique_out[0].values())[0].split("_"))
                if unique_out
                else (None, None)
            )

        # Pre-filter valid scan indices to avoid if it thier in innput not ouput data viceversa
        scan_indices = list(input_data._data_container.keys())
        for key in output_data._data_container.keys():
            if key not in input_data._data_container:
                scan_indices.append(key)

        # Process all scan indices at once
        for scan_idx in scan_indices:
            data_in = None
            data_out = None

            # Get input data if available
            if (
                input_data._data_container.get(scan_idx) is not None
                and grp_idx_in is not None
                and plt_idx_in is not None
            ):
                if grp_idx_in < len(
                    input_data._data_container[scan_idx]
                ) and plt_idx_in < len(
                    input_data._data_container[scan_idx][grp_idx_in]
                ):
                    data_in = input_data._data_container[scan_idx][grp_idx_in][
                        plt_idx_in
                    ]

                    # Convert scalar to 1D numpy array
                    if np.isscalar(data_in):
                        data_in = np.array([data_in])
                    else:
                        data_in = np.array(data_in)

            # Get output data if available
            if (
                output_data._data_container.get(scan_idx) is not None
                and grp_idx_out is not None
                and plt_idx_out is not None
            ):
                if grp_idx_out < len(
                    output_data._data_container[scan_idx]
                ) and plt_idx_out < len(
                    output_data._data_container[scan_idx][grp_idx_out]
                ):
                    data_out = output_data._data_container[scan_idx][grp_idx_out][
                        plt_idx_out
                    ]

                    # Convert scalar to 1D numpy array
                    if np.isscalar(data_out):
                        data_out = np.array([data_out])
                    else:
                        data_out = np.array(data_out)

            # Proceed if at least one of data_in or data_out is not None
            if data_in is not None or data_out is not None:
                # Replace None with empty arrays for consistent handling
                data_in = np.array([]) if data_in is None else data_in
                data_out = np.array([]) if data_out is None else data_out

                len_in, len_out = data_in.size, data_out.size

                # Equal length case - both arrays have matching data
                if len_in == len_out and len_in > 0:
                    n = len_in

                    if data_dict["SI"] is None:
                        data_dict["SI"] = np.full(n, scan_idx)
                        data_dict["I"] = data_in
                        data_dict["O"] = data_out
                    else:
                        data_dict["SI"] = np.append(
                            data_dict["SI"], np.full(n, scan_idx)
                        )
                        data_dict["I"] = np.append(data_dict["I"], data_in)
                        data_dict["O"] = np.append(data_dict["O"], data_out)

                    data_dict["match"] += n

                # Mismatched lengths case
                else:
                    n_min = min(len_in, len_out)
                    n_diff = max(len_in, len_out) - n_min

                    # Handle matched portion first (if any)
                    if n_min > 0:
                        if data_dict["SI"] is None:
                            data_dict["SI"] = np.full(n_min, scan_idx)
                            data_dict["I"] = data_in[:n_min]
                            data_dict["O"] = data_out[:n_min]
                        else:
                            data_dict["SI"] = np.append(
                                data_dict["SI"], np.full(n_min, scan_idx)
                            )
                            data_dict["I"] = np.append(data_dict["I"], data_in[:n_min])
                            data_dict["O"] = np.append(data_dict["O"], data_out[:n_min])

                        data_dict["match"] += n_min

                    # Handle mismatched portion
                    if n_diff > 0:
                        if len_in > len_out:
                            # Input has extra values
                            extra_values = data_in[n_min:]
                            if not data_dict["MI"][0]:  # First mismatch
                                data_dict["MI"] = [
                                    [scan_idx] * len(extra_values),
                                    extra_values.tolist(),
                                ]
                            else:
                                data_dict["MI"][0].extend(
                                    [scan_idx] * len(extra_values)
                                )
                                data_dict["MI"][1].extend(extra_values.tolist())
                            data_dict["mismatch"] += len(extra_values)
                        else:
                            # Output has extra values
                            extra_values = data_out[n_min:]
                            if not data_dict["MO"][0]:  # First mismatch
                                data_dict["MO"] = [
                                    [scan_idx] * len(extra_values),
                                    extra_values.tolist(),
                                ]
                            else:
                                data_dict["MO"][0].extend(
                                    [scan_idx] * len(extra_values)
                                )
                                data_dict["MO"][1].extend(extra_values.tolist())
                            data_dict["mismatch"] += len(extra_values)

        return data_dict
