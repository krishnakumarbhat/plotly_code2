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

        # Optional ordered row indices to keep when ingesting datasets
        self._selected_indices = None

        # Optional set of scan indices to skip when ingesting datasets
        self._missing_indices = set()

        # Counter for unique identifiers
        self._parent_counter: int = -1
        self._child_counter: int = -1

    def initialize(self, scan_index, sensor, missing_idx=None, selected_idx=None) -> None:
        """
        Initialize the data container with scan indices.

        Args:
            scan_index: List or NumPy array of scan indices to initialize the container with
        """

        # Track indices that should be skipped later, if provided
        self._selected_indices = tuple(selected_idx) if selected_idx is not None else None
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

    def _align_dataset_rows(self, dataset: Any, signal_name: str):
        if dataset is None:
            return None

        if self._selected_indices is not None:
            dataset_len = len(dataset)
            aligned = []
            for idx in self._selected_indices:
                if idx >= dataset_len:
                    logging.debug(
                        f"Skipping aligned row {idx} for {signal_name}: dataset length is {dataset_len}"
                    )
                    continue
                aligned.append(dataset[idx])
            return aligned

        if self._missing_indices:
            missing_set = self._missing_indices
            return [item for idx, item in enumerate(dataset) if idx not in missing_set]

        return dataset

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

        dataset = self._align_dataset_rows(dataset, signal_name)

        if is_new_parent:
            # Handle new parent group
            key_grp = f"{self._parent_counter}_None"
            self._child_counter += 1
            key_stream = f"{self._parent_counter}_{self._child_counter}"

            # Process and store the data
            self._process_dataset(dataset, key_stream, signal_name, key_grp)

            return key_stream

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
        self._selected_indices = None
        self._missing_indices = set()
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
            "SI": None,
            "I": None,
            "O": None,
            "MI": [[], []],
            "MO": [[], []],
            "match": 0,
            "mismatch": 0,
            "scan_input_values": {},
            "scan_output_values": {},
            "scan_input_counts": {},
            "scan_output_counts": {},
            "common_scan_indices": [],
            "input_only_scan_indices": [],
            "output_only_scan_indices": [],
            "input_point_total": 0,
            "output_point_total": 0,
        }

        def _normalize_signal_name(name):
            return "".join(
                ch for ch in str(name or "").strip().lower() if ch.isalnum()
            )

        def _candidate_normalized_names(name):
            normalized = _normalize_signal_name(name)
            alias_groups = [
                {"ran", "range", "detectionrange"},
                {"vel", "velocity", "detectionvelocity", "rr", "rangerate"},
                {"phi", "elevation", "eli"},
                {"theta", "azimuth", "azi"},
            ]
            for group in alias_groups:
                if normalized in group:
                    return group
            return {normalized}

        def _resolve_signal_mapping(data_obj, requested_name):
            signal_map = getattr(data_obj, "_signal_to_value", None)
            if not signal_map:
                return None

            direct = signal_map.get(requested_name)
            if direct:
                return direct

            candidate_names = _candidate_normalized_names(requested_name)
            for existing_name, existing_value in signal_map.items():
                if (
                    _normalize_signal_name(existing_name) in candidate_names
                    and existing_value
                ):
                    return existing_value
            return None

        def _parse_index_pair(unique_value):
            if isinstance(unique_value, str) and unique_value:
                return map(int, unique_value.split("_"))
            if isinstance(unique_value, list) and unique_value:
                try:
                    return map(int, list(unique_value[0].values())[0].split("_"))
                except Exception:
                    return (None, None)
            return (None, None)

        unique_in = _resolve_signal_mapping(input_data, signal_name)
        unique_out = _resolve_signal_mapping(output_data, signal_name)

        if not unique_in and not unique_out:
            return "no_data_in_hdf", {}

        grp_idx_in, plt_idx_in = _parse_index_pair(unique_in)
        grp_idx_out, plt_idx_out = _parse_index_pair(unique_out)

        scan_indices = sorted(
            set(getattr(input_data, "_data_container", {}).keys())
            | set(getattr(output_data, "_data_container", {}).keys())
        )

        for scan_idx in scan_indices:
            data_in = None
            data_out = None

            if (
                input_data._data_container.get(scan_idx) is not None
                and grp_idx_in is not None
                and plt_idx_in is not None
            ):
                if grp_idx_in < len(input_data._data_container[scan_idx]) and plt_idx_in < len(
                    input_data._data_container[scan_idx][grp_idx_in]
                ):
                    data_in = input_data._data_container[scan_idx][grp_idx_in][plt_idx_in]
                    data_in = np.array([data_in]) if np.isscalar(data_in) else np.array(data_in)

            if (
                output_data._data_container.get(scan_idx) is not None
                and grp_idx_out is not None
                and plt_idx_out is not None
            ):
                if grp_idx_out < len(output_data._data_container[scan_idx]) and plt_idx_out < len(
                    output_data._data_container[scan_idx][grp_idx_out]
                ):
                    data_out = output_data._data_container[scan_idx][grp_idx_out][plt_idx_out]
                    data_out = np.array([data_out]) if np.isscalar(data_out) else np.array(data_out)

            if data_in is None and data_out is None:
                continue

            data_in = np.array([]) if data_in is None else data_in
            data_out = np.array([]) if data_out is None else data_out

            len_in, len_out = data_in.size, data_out.size
            scan_idx_int = int(scan_idx)

            data_dict["scan_input_values"][scan_idx_int] = data_in.tolist()
            data_dict["scan_output_values"][scan_idx_int] = data_out.tolist()
            data_dict["scan_input_counts"][scan_idx_int] = int(len_in)
            data_dict["scan_output_counts"][scan_idx_int] = int(len_out)
            data_dict["input_point_total"] += int(len_in)
            data_dict["output_point_total"] += int(len_out)

            if len_in > 0 and len_out > 0:
                data_dict["common_scan_indices"].append(scan_idx_int)
            elif len_in > 0:
                data_dict["input_only_scan_indices"].append(scan_idx_int)
            elif len_out > 0:
                data_dict["output_only_scan_indices"].append(scan_idx_int)

            if len_in == len_out and len_in > 0:
                n = len_in
                if data_dict["SI"] is None:
                    data_dict["SI"] = np.full(n, scan_idx)
                    data_dict["I"] = data_in
                    data_dict["O"] = data_out
                else:
                    data_dict["SI"] = np.append(data_dict["SI"], np.full(n, scan_idx))
                    data_dict["I"] = np.append(data_dict["I"], data_in)
                    data_dict["O"] = np.append(data_dict["O"], data_out)

                data_dict["match"] += n
                continue

            n_min = min(len_in, len_out)
            n_diff = max(len_in, len_out) - n_min

            if n_min == 0 and n_diff > 0 and (len_in == 0 or len_out == 0):
                n_only = max(len_in, len_out)
                si_only = np.full(n_only, scan_idx)
                if len_in == 0:
                    i_only = np.full(n_only, np.nan)
                    o_only = data_out
                else:
                    i_only = data_in
                    o_only = np.full(n_only, np.nan)

                if data_dict["SI"] is None:
                    data_dict["SI"] = si_only
                    data_dict["I"] = i_only
                    data_dict["O"] = o_only
                else:
                    data_dict["SI"] = np.append(data_dict["SI"], si_only)
                    data_dict["I"] = np.append(data_dict["I"], i_only)
                    data_dict["O"] = np.append(data_dict["O"], o_only)

            if n_min > 0:
                if data_dict["SI"] is None:
                    data_dict["SI"] = np.full(n_min, scan_idx)
                    data_dict["I"] = data_in[:n_min]
                    data_dict["O"] = data_out[:n_min]
                else:
                    data_dict["SI"] = np.append(data_dict["SI"], np.full(n_min, scan_idx))
                    data_dict["I"] = np.append(data_dict["I"], data_in[:n_min])
                    data_dict["O"] = np.append(data_dict["O"], data_out[:n_min])

                data_dict["match"] += n_min

            if n_diff > 0:
                if len_in > len_out:
                    extra_values = data_in[n_min:]
                    if not data_dict["MI"][0]:
                        data_dict["MI"] = [
                            [scan_idx] * len(extra_values),
                            extra_values.tolist(),
                        ]
                    else:
                        data_dict["MI"][0].extend([scan_idx] * len(extra_values))
                        data_dict["MI"][1].extend(extra_values.tolist())
                    data_dict["mismatch"] += len(extra_values)
                else:
                    extra_values = data_out[n_min:]
                    if not data_dict["MO"][0]:
                        data_dict["MO"] = [
                            [scan_idx] * len(extra_values),
                            extra_values.tolist(),
                        ]
                    else:
                        data_dict["MO"][0].extend([scan_idx] * len(extra_values))
                        data_dict["MO"][1].extend(extra_values.tolist())
                    data_dict["mismatch"] += len(extra_values)

        return data_dict
