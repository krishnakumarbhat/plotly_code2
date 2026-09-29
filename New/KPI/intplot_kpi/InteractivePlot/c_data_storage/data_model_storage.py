from typing import Dict, List, Any
import logging
import numpy as np


class DataModelStorage:
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

        # Counter for unique identifiers
        self._parent_counter: int = -1
        self._child_counter: int = -1

    def initialize(self, scan_index, sensor, stream) -> None:
        """
        Initialize the data container with scan indices.

        Args:
            scan_index: List or NumPy array of scan indices to initialize the container with
        """

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
                    f"Missing scan indices at this signal {sensor}/{stream}: {missing_indices}"
                )
            if duplicates:
                logging.debug(
                    f"Duplicate scan indices at {sensor}/{stream}: {duplicates}"
                )

        # Use defaultdict to avoid checking if key exists later
        self._data_container = {j: [] for j in scan_index}

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

        # Length checks (important for preventing invalid parent/child state)
        dataset_len = len(dataset) if dataset is not None else 0
        container_len = len(self._data_container)

        if dataset is not None and dataset_len > container_len:
            logging.warning(
                f"Truncating dataset for {signal_name}: dataset length ({dataset_len}) exceeds scan indices length ({container_len})"
            )
            dataset = dataset[:container_len]
            dataset_len = container_len

        if is_new_parent:
            # If the first dataset we see for a group doesn't align with scan_index length,
            # skip it without advancing counters; otherwise subsequent datasets would be
            # treated as children and crash when appending to a non-existent parent row.
            if dataset_len != container_len:
                logging.debug(
                    f"Skipping plot for {signal_name}: dataset length ({dataset_len}) does not match scan indices length ({container_len})"
                )
                return f"{self._parent_counter}_skipped"

            # Handle new parent group
            key_grp = f"{self._parent_counter}_None"
            self._child_counter += 1
            key_stream = f"{self._parent_counter}_{self._child_counter}"

            # Process and store the data
            self._process_dataset(dataset, key_stream, signal_name, key_grp)
        else:
            # Skip if lengths don't match
            if dataset_len != container_len:
                # Route messages about skipping child processing to logs file
                logging.debug(
                    f"Skipping child processing for {signal_name} in {grp_name}: dataset length ({dataset_len}) does not match scan indices length ({container_len})"
                )
                return f"{self._parent_counter}_skipped"

            # Handle child item (only advance the counter when we will actually append)
            self._child_counter += 1
            key = f"{self._parent_counter}_{self._child_counter}"

            # Process and store data for child
            for idx, (row, scanidx) in enumerate(zip(dataset, self._data_container)):
                if isinstance(row, np.ndarray):
                    with np.errstate(invalid="ignore"):
                        rounded_row = np.round(np.asarray(row, dtype=np.float64), decimals=2)
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

        # Return key for later reference
        return key if not is_new_parent else key_stream

    def _process_dataset(self, dataset, key_stream, signal_name, key_grp):
        """Helper method to process and store dataset for new parent groups."""

        # Get the length of dataset and data_container
        dataset_len = len(dataset) if dataset is not None else 0
        container_len = len(self._data_container)

        # Skip if lengths don't match
        if dataset_len != container_len:
            logging.debug(
                f"Skipping plot for {signal_name}: dataset length ({dataset_len}) does not match scan indices length ({container_len})"
            )
            return

        # Process all rows in the dataset and store them
        for idx, (row, scanidx) in enumerate(zip(dataset, self._data_container)):
            if isinstance(row, np.ndarray):
                with np.errstate(invalid="ignore"):
                    rounded_row = np.round(np.asarray(row, dtype=np.float64), decimals=2)
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
            return "".join(ch for ch in str(name or "").strip().lower() if ch.isalnum())

        def _candidate_normalized_names(name):
            normalized = _normalize_signal_name(name)
            groups = [
                {"ran", "range", "detectionrange"},
                {"vel", "velocity", "detectionvelocity"},
                {"phi", "elevation"},
                {"theta", "azimuth"},
            ]
            for group in groups:
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
                if _normalize_signal_name(existing_name) in candidate_names and existing_value:
                    return existing_value
            return None

        # Resolve signal mapping per side (input/output may use different aliases)
        unique_in = _resolve_signal_mapping(input_data, signal_name)
        unique_out = _resolve_signal_mapping(output_data, signal_name)

        # Return early if signal not found in either map
        if not unique_in and not unique_out:
            return "no_data_in_hdf", {}

        grp_idx_in = None
        plt_idx_in = None

        grp_idx_out = None
        plt_idx_out = None
        # try:
        def _parse_index_pair(unique_value):
            if isinstance(unique_value, str) and unique_value:
                return map(int, unique_value.split("_"))
            if isinstance(unique_value, list) and unique_value:
                try:
                    return map(int, list(unique_value[0].values())[0].split("_"))
                except Exception:
                    return (None, None)
            return (None, None)

        # Parse group and plot indices independently for input and output.
        grp_idx_in, plt_idx_in = _parse_index_pair(unique_in)
        grp_idx_out, plt_idx_out = _parse_index_pair(unique_out)

        scan_indices = sorted(
            set(getattr(input_data, "_data_container", {}).keys())
            | set(getattr(output_data, "_data_container", {}).keys())
        )

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

                    # If one side is completely missing, still populate SI/I/O so plots can be generated.
                    # Use NaN as placeholder for the missing side.
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
