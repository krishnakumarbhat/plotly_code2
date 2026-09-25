import numpy as np
from InteractivePlot.e_presentation_layer.plotly_visualization import PlotlyCharts
import logging
import plotly.graph_objects as go
from plotly.subplots import make_subplots


class DataCalculations:
    """
    Class containing methods for processing and calculating derived data values
    for various radar signal parameters.
    """

    def __init__(self):
        """Initialize the DataCalculations class"""
        self.stream_name = None
        self._data_frame_cache = {}
        self._range_scatter_min = 0.0
        self._range_scatter_max = 1000
        self._exp_scatter_threshold = 1000

    def set_stream_name(self, stream_name: str) -> None:
        """
        Set the stream name for data organization

        Args:
            stream_name: The name of the data stream
        """
        self.stream_name = stream_name

    def _reduce_to_scalar_series(self, values, reducer):
        if values is None:
            return None
        reduced = []
        for v in values:
            if isinstance(v, (list, tuple, np.ndarray)):
                arr = np.asarray(v, dtype=float)
                if arr.size == 0:
                    reduced.append(np.nan)
                else:
                    reduced.append(float(reducer(arr)))
            else:
                try:
                    reduced.append(float(v))
                except Exception:
                    reduced.append(np.nan)
        return np.asarray(reduced)

    def _is_range_scatter_signal(self, signal_name):
        normalized = str(signal_name or "").strip().lower()
        return normalized in {"range", "ran", "detection_range"}

    def _get_cached_signal_bundle(self, shared_data, candidate_names):
        if shared_data is None:
            return None
        for name in candidate_names:
            key = f"_signal_bundle::{str(name).strip().lower()}"
            try:
                bundle = shared_data.get(key)
                if bundle:
                    return bundle
            except Exception:
                continue
        return None

    def _as_numeric(self, values):
        if values is None:
            return np.array([], dtype=float)
        try:
            arr = np.asarray(values, dtype=float)
            return np.ravel(arr)
        except Exception:
            return np.array([], dtype=float)

    def _to_degrees_if_needed(self, values):
        arr = self._as_numeric(values)
        if arr.size == 0:
            return arr
        finite = arr[np.isfinite(arr)]
        if finite.size == 0:
            return arr
        if np.nanmax(np.abs(finite)) <= 3.5:
            return np.rad2deg(arr)
        return arr

    def _build_radar_context(self, signal_name, data_dict, shared_data):
        current_si = self._as_numeric(data_dict.get("SI"))
        current_i = self._as_numeric(data_dict.get("I"))
        current_o = self._as_numeric(data_dict.get("O"))

        range_bundle = self._get_cached_signal_bundle(shared_data, ["range", "ran", signal_name])
        az_bundle = self._get_cached_signal_bundle(shared_data, ["azimuth", "theta"])
        el_bundle = self._get_cached_signal_bundle(shared_data, ["elevation", "phi"])

        if range_bundle is None:
            range_i = current_i
            range_o = current_o
            scan_i = current_si
            scan_o = current_si
        else:
            range_i = self._as_numeric(range_bundle.get("I"))
            range_o = self._as_numeric(range_bundle.get("O"))
            scan_i = self._as_numeric(range_bundle.get("SI"))
            scan_o = scan_i.copy()

        az_i = self._to_degrees_if_needed(az_bundle.get("I") if az_bundle else current_i)
        az_o = self._to_degrees_if_needed(az_bundle.get("O") if az_bundle else current_o)
        el_i = self._to_degrees_if_needed(el_bundle.get("I") if el_bundle else current_i)
        el_o = self._to_degrees_if_needed(el_bundle.get("O") if el_bundle else current_o)

        n_i = min(scan_i.size, range_i.size, az_i.size, el_i.size)
        n_o = min(scan_o.size, range_o.size, az_o.size, el_o.size)
        if n_i == 0 or n_o == 0:
            return None

        return {
            "si_i": scan_i[:n_i],
            "si_o": scan_o[:n_o],
            "ran_i": range_i[:n_i],
            "ran_o": range_o[:n_o],
            "az_i": az_i[:n_i],
            "az_o": az_o[:n_o],
            "el_i": el_i[:n_i],
            "el_o": el_o[:n_o],
        }

    def _dbscan_labels(self, features, eps=0.85, min_samples=10):
        points = np.asarray(features, dtype=float)
        n_points = points.shape[0]
        if n_points == 0:
            return np.array([], dtype=int)

        finite_mask = np.all(np.isfinite(points), axis=1)
        labels = np.full(n_points, -1, dtype=int)
        if not np.any(finite_mask):
            return labels

        valid_points = points[finite_mask]
        step = max(float(eps), 1e-3)
        quantized = np.floor(valid_points / step).astype(np.int64)

        bins = {}
        for index, row in enumerate(quantized):
            key = tuple(row.tolist())
            bins.setdefault(key, []).append(index)

        cluster_id = 0
        valid_labels = np.full(valid_points.shape[0], -1, dtype=int)
        for _, members in bins.items():
            if len(members) >= int(min_samples):
                valid_labels[np.asarray(members, dtype=int)] = cluster_id
                cluster_id += 1

        labels[np.where(finite_mask)[0]] = valid_labels
        return labels

    def _filter_range_scatter_outliers(self, signal_name, scan_idx, i_vals, o_vals, allow_one_sided_fallback=True):
        """
        Keep only valid range values for scatter plotting.
        Valid range: [0, 1000], non-negative, finite.
        """
        scan_idx_arr = np.asarray(scan_idx)
        input_arr = np.asarray(i_vals, dtype=float)
        output_arr = np.asarray(o_vals, dtype=float)

        if not self._is_range_scatter_signal(signal_name):
            return scan_idx_arr, input_arr, output_arr

        finite_input = np.isfinite(input_arr)
        finite_output = np.isfinite(output_arr)
        valid_input = finite_input & (input_arr >= self._range_scatter_min) & (input_arr <= self._range_scatter_max)
        valid_output = finite_output & (output_arr >= self._range_scatter_min) & (output_arr <= self._range_scatter_max)
        valid_mask = valid_input & valid_output

        if valid_mask.size == 0:
            return scan_idx_arr, input_arr, output_arr

        filtered_count = int(valid_mask.size - np.count_nonzero(valid_mask))
        if filtered_count > 0:
            logging.info(
                "Filtered %d outlier points for range scatter signal '%s' outside [%s, %s] or negative/non-finite.",
                filtered_count,
                signal_name,
                self._range_scatter_min,
                self._range_scatter_max,
            )

        if not allow_one_sided_fallback:
            return scan_idx_arr[valid_mask], input_arr[valid_mask], output_arr[valid_mask]

        input_plot_mask = valid_input.copy()
        output_plot_mask = valid_output.copy()

        if np.count_nonzero(input_plot_mask) == 0 and np.count_nonzero(finite_input) > 0:
            input_plot_mask = finite_input
            logging.warning(
                "No in-range INPUT points for range signal '%s'; using finite INPUT values for visualization.",
                signal_name,
            )

        if np.count_nonzero(output_plot_mask) == 0 and np.count_nonzero(finite_output) > 0:
            output_plot_mask = finite_output
            logging.warning(
                "No in-range OUTPUT points for range signal '%s'; using finite OUTPUT values for visualization.",
                signal_name,
            )

        combined_mask = input_plot_mask | output_plot_mask
        if np.count_nonzero(combined_mask) == 0:
            return scan_idx_arr[valid_mask], input_arr[valid_mask], output_arr[valid_mask]

        if np.count_nonzero(valid_mask) == 0:
            logging.warning(
                "No overlapping in-range IN/OUT points for range signal '%s'; using one-sided data for visualization.",
                signal_name,
            )

        filtered_scan = scan_idx_arr[combined_mask]
        filtered_input = np.where(input_plot_mask[combined_mask], input_arr[combined_mask], np.nan)
        filtered_output = np.where(output_plot_mask[combined_mask], output_arr[combined_mask], np.nan)
        return filtered_scan, filtered_input, filtered_output

    def _replace_exponential_scatter_values(self, signal_name, i_vals, o_vals):
        input_arr = np.asarray(i_vals, dtype=float)
        output_arr = np.asarray(o_vals, dtype=float)

        input_exp = np.isfinite(input_arr) & (np.abs(input_arr) > self._exp_scatter_threshold)
        output_exp = np.isfinite(output_arr) & (np.abs(output_arr) > self._exp_scatter_threshold)
        replaced = int(np.count_nonzero(input_exp) + np.count_nonzero(output_exp))

        if replaced > 0:
            logging.info(
                "Replaced %d exponential scatter points for '%s' (|value| > %s) with 0.0.",
                replaced,
                signal_name,
                self._exp_scatter_threshold,
            )

        input_clean = np.where(input_exp, 0.0, input_arr)
        output_clean = np.where(output_exp, 0.0, output_arr)
        return input_clean, output_clean

    def _scatter_placeholder(self, signal_name, first_label="INPUT", second_label="OUTPUT", plot_label="IN/OUT"):
        fig_id = f"scatter_fig_{signal_name}"
        fig = PlotlyCharts.scatter_plot(
            [0],
            [0.0],
            [0.0],
            signal_name,
            first_label,
            second_label,
            "red",
            "blue",
            plot_label,
        )
        return fig_id, fig

    def scatter_plot_vector_mean(self, signal_name, data_dict, shared_data, lock):
        si = data_dict.get("SI") if isinstance(data_dict, dict) else None
        i_vals = data_dict.get("I") if isinstance(data_dict, dict) else None
        o_vals = data_dict.get("O") if isinstance(data_dict, dict) else None
        if si is None or i_vals is None or o_vals is None:
            logging.error(f"Missing required data for scatter_plot_vector_mean of {signal_name}")
            return None, None

        i_series = self._reduce_to_scalar_series(i_vals, np.nanmean)
        o_series = self._reduce_to_scalar_series(o_vals, np.nanmean)
        if i_series is None or o_series is None:
            return None, None

        fig_id = f"scatter_fig_{signal_name}_mean"
        fig = PlotlyCharts.scatter_plot(
            si,
            i_series,
            o_series,
            signal_name,
            "INPUT(mean)",
            "OUTPUT(mean)",
            "red",
            "blue",
            "IN/OUT",
        )
        return fig_id, fig

    def histogram_vector_mean(self, signal_name, data_dict, shared_data, lock):
        i_vals = data_dict.get("I") if isinstance(data_dict, dict) else None
        o_vals = data_dict.get("O") if isinstance(data_dict, dict) else None
        if i_vals is None or o_vals is None:
            logging.error(f"Missing required data for histogram_vector_mean of {signal_name}")
            return None, None

        i_series = self._reduce_to_scalar_series(i_vals, np.nanmean)
        o_series = self._reduce_to_scalar_series(o_vals, np.nanmean)
        if i_series is None or o_series is None:
            return None, None

        hist_fig_id = f"hist_fig_{signal_name}_mean"
        hist_fig = PlotlyCharts.histogram_with_count(i_series, o_series, signal_name)
        return hist_fig_id, hist_fig

    def scatter_with_mismatch_vector_mean(self, signal_name, data_dict, shared_data, lock):
        if not isinstance(data_dict, dict):
            logging.error(f"Invalid data_dict for scatter_with_mismatch_vector_mean of {signal_name}")
            return None, None

        si_vals = data_dict.get("SI")
        i_vals = data_dict.get("I")
        o_vals = data_dict.get("O")
        if si_vals is None or i_vals is None or o_vals is None:
            logging.error(f"Missing required data for scatter_with_mismatch_vector_mean of {signal_name}")
            return None, None

        i_series = self._reduce_to_scalar_series(i_vals, np.nanmean)
        o_series = self._reduce_to_scalar_series(o_vals, np.nanmean)
        if i_series is None or o_series is None:
            return None, None

        reduced_dict = dict(data_dict)
        reduced_dict["SI"] = np.asarray(si_vals)
        reduced_dict["I"] = i_series
        reduced_dict["O"] = o_series
        reduced_dict.setdefault("MI", [[], []])
        reduced_dict.setdefault("MO", [[], []])
        return self.scatter_with_mismatch(signal_name, reduced_dict, shared_data, lock)

    def scatter_plot(self, signal_name, data_dict, shared_data, lock):
        """
        Create a scatter plot with scan index

        Parameters:
        - signal_name: Name of the signal
        - data_records: Dictionary with 'I', 'O', 'M' keys each containing lists of [scan_idx, value] pairs

        Returns:
        - fig_id: ID of the figure
        - fig: Plotly figure object
        """

        si = data_dict.get("SI")
        i_vals = data_dict.get("I")
        o_vals = data_dict.get("O")

        if si is None or i_vals is None or o_vals is None:
            logging.error(f"Missing required data for scatter plot of {signal_name}")
            return None, None

        si, i_vals, o_vals = self._filter_range_scatter_outliers(signal_name, si, i_vals, o_vals)
        if len(si) == 0:
            logging.warning(
                "No valid points left after outlier filtering for scatter plot of %s; using zero placeholder.",
                signal_name,
            )
            return self._scatter_placeholder(signal_name)
        i_vals, o_vals = self._replace_exponential_scatter_values(signal_name, i_vals, o_vals)

        fig_id = f"scatter_fig_{signal_name}"
        fig = PlotlyCharts.scatter_plot(
            si,
            i_vals,
            o_vals,
            signal_name,
            "INPUT",
            "OUTPUT",
            "red",
            "blue",
            "IN/OUT",
        )
        return fig_id, fig

    def scatter_plot_mstokmh(self, signal_name, data_dict, shared_data, lock):
        """
        Create a scatter plot with scan index

        Parameters:
        - signal_name: Name of the signal
        - data_records: Dictionary with 'I', 'O', 'M' keys each containing lists of [scan_idx, value] pairs

        Returns:
        - fig_id: ID of the figure
        - fig: Plotly figure object
        """

        si = data_dict.get("SI")
        i_vals = data_dict.get("I")
        o_vals = data_dict.get("O")

        if si is None or i_vals is None or o_vals is None:
            logging.error(f"Missing required data for scatter_plot_mstokmh of {signal_name}")
            return None, None

        fig_id = f"scatter_fig_{signal_name}"
        temp_dict_i = np.asarray(i_vals) * 3.6
        temp_dict_o = np.asarray(o_vals) * 3.6
        temp_dict_i, temp_dict_o = self._replace_exponential_scatter_values(signal_name, temp_dict_i, temp_dict_o)
        fig = PlotlyCharts.scatter_plot(
            si,
            temp_dict_i,
            temp_dict_o,
            signal_name,
            "INPUT",
            "OUTPUT",
            "red",
            "blue",
            "IN/OUT",
        )
        return fig_id, fig

    def scatter_with_mismatch(self, signal_name, data_dict, shared_data, lock):
        """
        Create a scatter plot highlighting mismatch points between I/O signals.
        If there is no mismatch, plot a placeholder and append ' (no mismatch)' to the signal name.

        Parameters:
        - signal_name: Name of the signal
        - data_dict: Dictionary containing:
            'SI': Scan indices array
            'I': Input values array
            'O': Output values array
            'MI': Mismatch input container [list of indices, list of values]
            'MO': Mismatch output container (list of values)

        Returns:
        - fig_id: Unique figure identifier
        - fig: Configured Plotly figure object
        """
        # Validate required data
        if not isinstance(data_dict, dict):
            logging.error(f"Invalid data_dict for scatter_with_mismatch of {signal_name}")
            return None, None
        si_vals = data_dict.get("SI")
        i_vals = data_dict.get("I")
        o_vals = data_dict.get("O")
        if si_vals is None or i_vals is None or o_vals is None:
            logging.error(f"Missing required data for scatter_with_mismatch of {signal_name}")
            return None, None
        # Ensure mismatch containers exist with expected structure
        if "MI" not in data_dict or not isinstance(data_dict.get("MI"), list) or len(data_dict.get("MI")) < 2:
            data_dict["MI"] = [[], []]
        if "MO" not in data_dict or not isinstance(data_dict.get("MO"), list) or len(data_dict.get("MO")) < 2:
            data_dict["MO"] = [[], []]

        # Convert to numpy arrays for vectorized operations
        scan_idx = np.array(si_vals)
        i_vals = np.array(i_vals)
        o_vals = np.array(o_vals)
        scan_idx, i_vals, o_vals = self._filter_range_scatter_outliers(
            signal_name,
            scan_idx,
            i_vals,
            o_vals,
            allow_one_sided_fallback=True,
        )
        if len(scan_idx) == 0:
            logging.warning(
                "No valid points left after outlier filtering for scatter mismatch of %s; using zero mismatch placeholder.",
                signal_name,
            )
            fig_id, fig = self._scatter_placeholder(
                signal_name,
                first_label="MISMATCH",
                second_label="output mismatch",
                plot_label="MISMATCH",
            )
            return fig_id, fig

        # Calculate mismatch conditions with tolerance (for float comparisons)
        tol = {"ran": 0.1, "vel": 0.015, "phi": 0.00873, "theta": 0.00873, "snr": 0, "rcs": 0}.get(signal_name, 0)
        mismatch_mask = ~np.isclose(i_vals, o_vals, atol=tol, equal_nan=True)
        # i_nonzero = i_vals != 0
        # o_nonzero = o_vals != 0

        # Vectorized condition checks
        # case1_mask = mismatch_mask & i_nonzero & ~o_nonzero
        # case2_mask = mismatch_mask & ~i_nonzero & o_nonzero
        # case3_mask = mismatch_mask & (i_nonzero | o_nonzero)

        # Extract indices for mismatches
        all_mismatches = scan_idx[mismatch_mask]
        i_mismatches = i_vals[mismatch_mask]
        o_mismatches = o_vals[mismatch_mask]

        # Store mismatches - using extend for lists
        data_dict["MI"][0].extend(all_mismatches.tolist())
        data_dict["MI"][1].extend(i_mismatches.tolist())
        data_dict["MO"][1].extend(o_mismatches.tolist())

        has_mismatch = len(data_dict["MI"][0]) > 0 or len(data_dict["MO"][1]) > 0

        # Check if there are mismatches
        if has_mismatch:
            # There are mismatches: plot them
            plot_signal_name = signal_name
            x_vals = data_dict["MI"][0] if len(data_dict["MI"][0]) > 0 else data_dict["MO"][0]
            y_vals = data_dict["MI"][1]
            if len(y_vals) == 0:
                y_vals = [np.nan] * len(x_vals)
            y2_vals = data_dict["MO"][1]
            color = "red"
            label = "MISMATCH"
        else:
            # No mismatches: plot a visible placeholder and update signal name
            plot_signal_name = f"{signal_name} (no mismatch)"
            placeholder_x = int(scan_idx[0]) if len(scan_idx) > 0 else 0
            x_vals = [placeholder_x]
            y_vals = [0.0]
            y2_vals = [0.0]
            color = "green"
            label = "NO MISMATCH"
        # Create plot
        fig_id = f"scatter_fig_{plot_signal_name.replace(' ', '_')}"
        fig = PlotlyCharts.scatter_plot(
            x_vals,
            y_vals,
            y2_vals,
            plot_signal_name,
            label,
            "output mismatch",
            color,
            "blue",
            label,
        )

        return fig_id, fig

    def scatter_plot_bs_si_sr(self, signal_name, data_dict, shared_data, lock):
        """
        Create a scatter plot with scan index.

        Parameters:
        - signal_name: Name of the signal
        - data_dict: Dictionary with 'SI', 'I', 'O' keys, each containing lists of [scan_idx, value] pairs

        Returns:
        - fig_id: ID of the figure
        - fig: Plotly figure object
        """
        fig_id = f"scatter_fig_{signal_name}"
        scan_indices = np.array([])
        input_values = np.array([])
        output_values = np.array([])

        # Validate inputs
        si_data = data_dict.get("SI") if isinstance(data_dict, dict) else None
        i_data = data_dict.get("I") if isinstance(data_dict, dict) else None
        o_data = data_dict.get("O") if isinstance(data_dict, dict) else None
        if si_data is None or i_data is None or o_data is None:
            logging.error(f"Missing required data for scatter_plot_bs_si_sr of {signal_name}")
            return None, None

        prev_scan_idx = None
        input_sum = 0
        output_sum = 0

        for idx,scan_idx in enumerate(si_data):
            if prev_scan_idx is None or scan_idx != prev_scan_idx:
                if prev_scan_idx is not None:
                    scan_indices = np.append(scan_indices, prev_scan_idx)
                    input_values = np.append(input_values, input_sum)
                    output_values = np.append(output_values, output_sum)
                    input_sum = 0
                    output_sum = 0
                prev_scan_idx = scan_idx

            input_sum += i_data[idx]
            output_sum += o_data[idx]

        if prev_scan_idx is not None:
            scan_indices = np.append(scan_indices, prev_scan_idx)
            input_values = np.append(input_values, input_sum)
            output_values = np.append(output_values, output_sum)

        fig = PlotlyCharts.scatter_plot(
            scan_indices,
            input_values,
            output_values,
            signal_name,
            "INPUT",
            "OUTPUT",
            "red",
            "blue",
            "IN/OUT",
        )
        return fig_id, fig

    def scatter_num_af_det(self, signal_name, data_dict, shared_data, lock):
        """
        Create a scatter plot with scan index

        Parameters:
        - signal_name: Name of the signal
        - data_records: Dictionary with 'I', 'O', 'M' keys each containing lists of [scan_idx, value] pairs

        Returns:
        - fig_id: ID of the figure
        - fig: Plotly figure object
        """
        temp_dict = np.array
        fig_id = f"scatter_fig_{signal_name}"
        i_vals = data_dict.get("I")
        o_vals = data_dict.get("O")
        si_vals = data_dict.get("SI")
        if si_vals is None or i_vals is None or o_vals is None:
            logging.error(f"Missing required data for scatter_num_af_det of {signal_name}")
            return None, None
        temp_dict = np.subtract(o_vals, i_vals)
        fig = PlotlyCharts.scatter_plot(
            si_vals,
            temp_dict,
            None,
            "scatter diffrence num_af_det",
            "INPUT",
            None,
            "red",
            "IN/OUT",
        )
        return fig_id, fig

    def box_num_af_det(self, signal_name, data_dict, shared_data, lock):
        """
        Create a scatter plot with scan index

        Parameters:
        - signal_name: Name of the signal
        - data_records: Dictionary with 'I', 'O', 'M' keys each containing lists of [scan_idx, value] pairs

        Returns:
        - fig_id: ID of the figure
        - fig: Plotly figure object
        """
        temp_dict = np.array
        fig_id = f"scatter_fig_{signal_name}"
        i_vals = data_dict.get("I")
        o_vals = data_dict.get("O")
        if i_vals is None or o_vals is None:
            logging.error(f"Missing required data for box_num_af_det of {signal_name}")
            return None, None
        temp_dict = np.subtract(o_vals, i_vals)
        fig = PlotlyCharts.create_box(
            temp_dict, "box plot of num_af_det", signal_name, "red", "box of num af det"
        )
        return fig_id, fig

    def bar_mismatch_plots_all(self, signal_name, data_dict, shared_data, lock):
        """
        Create a bar chart showing match/mismatch counts for multiple signals

        Parameters:
        - signal_name: Name of the signal or 'DONE_ALL_SIGNALS' for final chart
        - data_dict: Dictionary with 'match' and 'mismatch' keys containing counts
        - shared_data: Shared dictionary for multiprocessing data exchange
        - lock: Lock for thread-safe operations

        Returns:
        - fig_id: ID of the figure
        - fig: Plotly figure object or None during data collection
        """
        # Case 1: Regular signal data collection
        if signal_name != "DONE_ALL_SIGNALS":
            try:
                if data_dict and "match" in data_dict and "mismatch" in data_dict:
                    with lock:
                        # Thread-safe operations with shared_data
                        shared_key = ("bar_mismatch_plots_all", signal_name)
                        if shared_key in shared_data:
                            shared_data[shared_key].append(
                                [data_dict["match"], data_dict["mismatch"]]
                            )
                        else:
                            shared_data[shared_key] = [
                                [data_dict["match"], data_dict["mismatch"]]
                            ]
                        logging.debug(f"Updated shared_data for {shared_key}")
                return None, None  # No figure to return during data collection
            except Exception as e:
                logging.error(f"Error storing data for {signal_name}: {str(e)}")
                return None, None

        # Case 2: Final aggregation for all signals
        elif signal_name == "DONE_ALL_SIGNALS":
            names, matches, mismatches = [], [], []

            with lock:  # Use lock when accessing shared_data
                for (func_type, stored_signal), values_list in shared_data.items():
                    if func_type == "bar_mismatch_plots_all":
                        # Calculate total matches and mismatches for each signal
                        total_match = 0
                        total_mismatch = 0

                        for value_pair in values_list:
                            total_match += value_pair[0]
                            total_mismatch += value_pair[1]

                        names.append(stored_signal)
                        matches.append(total_match)
                        mismatches.append(total_mismatch)

                if not names:  # No data collected
                    return "no_data_fig", go.Figure()

                hist_fig_id = "bar_mismatch_composite_fig"
                hist_fig = PlotlyCharts.bar_plots(
                    labels=names,
                    match_values=matches,
                    mismatch_values=mismatches,
                    title="Composite Mismatch Analysis",
                )
                return hist_fig_id, hist_fig

            # Fallback (should never reach here)
            return "invalid_fig", go.Figure()

    def bar_plots_fbi_sup_sig(self, signal_name, data_dict, shared_data, lock):
        """
        Create a bar chart showing match/mismatch counts for multiple signals (FBI sup sig variant).

        Parameters:
            signal_name (str): Name of the signal or 'DONE_FBI_SUP_SIG' for final chart.
            data_dict (dict): Dictionary with keys like 'I' and 'O' containing NumPy arrays of counts.
            shared_data (dict): Multiprocessing-safe dictionary for data exchange.
            lock (Lock): Lock for thread-safe operations.

        Returns:
            tuple: (fig_id, fig) where fig_id is the figure ID and fig is a Plotly figure object,
                or (None, None) during data collection phase.
        """

        if signal_name != "DONE_FBI_SUP_SIG":
            input_array = data_dict.get("I", np.array([]))
            output_array = data_dict.get("O", np.array([]))
            input_sum = np.sum(input_array)
            output_sum = np.sum(output_array)

            shared_key = ("bar_plots_fbi_sup_sig", signal_name)
            with lock:
                if shared_key not in shared_data:
                    shared_data[shared_key] = (input_sum, output_sum)
                else:
                    logging.debug(
                        f"Data already present in shared_data for {shared_key}"
                    )

            # Return None during data collection phase
            return None, None

        elif signal_name == "DONE_FBI_SUP_SIG":
            names = []
            all_input_counts = []
            all_output_counts = []

            with lock:
                for (func_type, stored_signal), value in shared_data.items():
                    if func_type == "bar_plots_fbi_sup_sig":
                        (input_sum, output_sum) = value
                        base_name = stored_signal
                        names.append(base_name)
                        all_input_counts.append(input_sum)
                        all_output_counts.append(output_sum)

            if not names:  # No data collected
                return "no_data_fig", go.Figure()

            hist_fig_id = "bar_mismatch_composite_fig"
            hist_fig = PlotlyCharts.bar_plots(
                labels=names,
                input_detection=all_input_counts,
                output_detection=all_output_counts,
                title="Composite Mismatch Analysis for fbi_sup_sig",
            )
            return hist_fig_id, hist_fig

        # Fallback (should never reach here)
        return "invalid_fig", go.Figure()

    def histogram_with_count(self, signal_name, data_dict, shared_data, lock):
        """
        Create a histogram with count

        Parameters:
        - signal_name: Name of the signal
        - data_records: Dictionary with 'I', 'O', 'M' keys each containing lists of [scan_idx, value] pairs

        Returns:
        - hist_fig_id: ID of the figure
        - hist_fig: Plotly figure object
        """

        hist_fig_id = f"hist_fig_{signal_name}"
        hist_fig = PlotlyCharts.histogram_with_count(
            data_dict.get("I"), data_dict.get("O"), signal_name
        )
        return hist_fig_id, hist_fig

    def histogram_with_radtodeg(self, signal_name, data_dict, shared_data, lock):
        """
        Create a histogram with count

        Parameters:
        - signal_name: Name of the signal
        - data_records: Dictionary with 'I', 'O', 'M' keys each containing lists of [scan_idx, value] pairs

        Returns:
        - hist_fig_id: ID of the figure
        - hist_fig: Plotly figure object
        """

        i_vals = data_dict.get("I")
        o_vals = data_dict.get("O")
        if i_vals is None or o_vals is None:
            logging.error(f"Missing required data for histogram_with_radtodeg of {signal_name}")
            return None, None

        hist_fig_id = f"hist_fig_{signal_name}"
        rad_temp_i = np.rad2deg(np.asarray(i_vals))
        rad_temp_o = np.rad2deg(np.asarray(o_vals))
        hist_fig = PlotlyCharts.histogram_with_count(
            rad_temp_i, rad_temp_o, signal_name
        )
        return hist_fig_id, hist_fig

    def histogram_scanindex_polar_dbscan(self, signal_name, data_dict, shared_data, lock):
        context = self._build_radar_context(signal_name, data_dict, shared_data)
        if context is None:
            return None, None

        max_points = 6000
        if context["ran_i"].size > max_points:
            idx = np.linspace(0, context["ran_i"].size - 1, max_points, dtype=int)
            for key in ("si_i", "ran_i", "az_i", "el_i"):
                context[key] = context[key][idx]
        if context["ran_o"].size > max_points:
            idx = np.linspace(0, context["ran_o"].size - 1, max_points, dtype=int)
            for key in ("si_o", "ran_o", "az_o", "el_o"):
                context[key] = context[key][idx]

        fi = np.column_stack([
            (context["ran_i"] - np.nanmean(context["ran_i"])) / (np.nanstd(context["ran_i"]) + 1e-6),
            context["az_i"] / 180.0,
            context["el_i"] / 90.0,
        ])
        fo = np.column_stack([
            (context["ran_o"] - np.nanmean(context["ran_o"])) / (np.nanstd(context["ran_o"]) + 1e-6),
            context["az_o"] / 180.0,
            context["el_o"] / 90.0,
        ])
        labels_i = self._dbscan_labels(fi)
        labels_o = self._dbscan_labels(fo)

        fig = make_subplots(
            rows=1,
            cols=2,
            specs=[[{"type": "polar"}, {"type": "polar"}]],
            subplot_titles=["INPUT clusters", "OUTPUT clusters"],
        )
        fig.add_trace(
            go.Scatterpolar(
                theta=context["az_i"],
                r=np.abs(context["ran_i"]),
                mode="markers",
                name="INPUT clusters",
                marker=dict(size=4, color=labels_i, colorscale="Turbo", opacity=0.75, showscale=True),
            ),
            row=1,
            col=1,
        )
        fig.add_trace(
            go.Scatterpolar(
                theta=context["az_o"],
                r=np.abs(context["ran_o"]),
                mode="markers",
                name="OUTPUT clusters",
                marker=dict(size=4, color=labels_o, colorscale="Turbo", opacity=0.75, showscale=False),
            ),
            row=1,
            col=2,
        )
        fig.update_layout(title=f"{signal_name} Polar DBSCAN Clusters (range/azimuth/elevation)")
        return f"hist_fig_{signal_name}_polar_dbscan", fig

    def histogram_scanindex_density_contour(self, signal_name, data_dict, shared_data, lock):
        context = self._build_radar_context(signal_name, data_dict, shared_data)
        if context is None:
            return None, None

        all_az = np.concatenate([context["az_i"], context["az_o"]])
        all_ran = np.concatenate([context["ran_i"], context["ran_o"]])
        finite_mask = np.isfinite(all_az) & np.isfinite(all_ran)
        if np.any(finite_mask):
            all_az_f = all_az[finite_mask]
            all_ran_f = all_ran[finite_mask]
            az_min, az_max = np.nanpercentile(all_az_f, [1, 99])
            ran_min, ran_max = np.nanpercentile(all_ran_f, [1, 99])
        else:
            az_min, az_max = -90, 90
            ran_min, ran_max = 0, 200

        fig = make_subplots(rows=1, cols=2, subplot_titles=["INPUT density", "OUTPUT density"])
        fig.add_trace(
            go.Histogram2dContour(
                x=context["az_i"],
                y=context["ran_i"],
                colorscale="Blues",
                ncontours=18,
                contours=dict(coloring="heatmap", showlines=True),
                name="INPUT",
                showscale=True,
                colorbar=dict(title="Input density"),
            ),
            row=1,
            col=1,
        )
        fig.add_trace(
            go.Histogram2dContour(
                x=context["az_o"],
                y=context["ran_o"],
                colorscale="Turbo",
                ncontours=18,
                contours=dict(coloring="heatmap", showlines=True),
                name="OUTPUT",
                showscale=True,
                colorbar=dict(title="Output density"),
            ),
            row=1,
            col=2,
        )
        fig.update_xaxes(title_text="Azimuth (deg)", row=1, col=1, range=[az_min, az_max])
        fig.update_xaxes(title_text="Azimuth (deg)", row=1, col=2, range=[az_min, az_max])
        fig.update_yaxes(title_text="Range", row=1, col=1, range=[ran_min, ran_max])
        fig.update_yaxes(title_text="Range", row=1, col=2, range=[ran_min, ran_max])
        fig.update_layout(title=f"{signal_name} Heatmap / Density Contours (shared scale)")
        return f"hist_fig_{signal_name}_density_contour", fig

    def histogram_scanindex_density_contour_animated(self, signal_name, data_dict, shared_data, lock):
        context = self._build_radar_context(signal_name, data_dict, shared_data)
        if context is None:
            return None, None

        all_az = np.concatenate([context["az_i"], context["az_o"]])
        all_ran = np.concatenate([context["ran_i"], context["ran_o"]])
        all_el = np.concatenate([context["el_i"], context["el_o"]])
        finite_mask = np.isfinite(all_az) & np.isfinite(all_ran)
        if np.any(finite_mask):
            all_az_f = all_az[finite_mask]
            all_ran_f = all_ran[finite_mask]
            az_min, az_max = np.nanpercentile(all_az_f, [1, 99])
            ran_min, ran_max = np.nanpercentile(all_ran_f, [1, 99])
        else:
            az_min, az_max = -90, 90
            ran_min, ran_max = 0, 200

        finite_el = all_el[np.isfinite(all_el)]
        if finite_el.size:
            el_min, el_max = np.nanpercentile(finite_el, [1, 99])
        else:
            el_min, el_max = -30, 30

        dark_blue_scale = [
            [0.0, "#050816"],
            [0.35, "#0b1f4d"],
            [0.7, "#145da0"],
            [1.0, "#56cfe1"],
        ]

        unique_scans = np.unique(context["si_i"].astype(int))
        if unique_scans.size == 0:
            return None, None

        if unique_scans.size > 40:
            unique_scans = unique_scans[np.linspace(0, unique_scans.size - 1, 40, dtype=int)]

        first_scan = int(unique_scans[0])
        m_i0 = context["si_i"].astype(int) == first_scan
        m_o0 = context["si_o"].astype(int) == first_scan

        fig = make_subplots(rows=1, cols=2, subplot_titles=["INPUT by scan", "OUTPUT by scan"])
        fig.add_trace(
            go.Scatter(
                x=context["az_i"][m_i0],
                y=context["ran_i"][m_i0],
                mode="markers",
                marker=dict(
                    size=6,
                    color=context["el_i"][m_i0],
                    cmin=el_min,
                    cmax=el_max,
                    colorscale=dark_blue_scale,
                    colorbar=dict(title="Elevation"),
                    opacity=0.82,
                ),
                name="INPUT",
            ),
            row=1,
            col=1,
        )
        fig.add_trace(
            go.Scatter(
                x=context["az_o"][m_o0],
                y=context["ran_o"][m_o0],
                mode="markers",
                marker=dict(
                    size=6,
                    color=context["el_o"][m_o0],
                    cmin=el_min,
                    cmax=el_max,
                    colorscale=dark_blue_scale,
                    showscale=False,
                    opacity=0.82,
                ),
                name="OUTPUT",
            ),
            row=1,
            col=2,
        )

        frames = []
        for scan in unique_scans:
            scan = int(scan)
            m_i = context["si_i"].astype(int) == scan
            m_o = context["si_o"].astype(int) == scan
            frames.append(
                go.Frame(
                    name=str(scan),
                    data=[
                        go.Scatter(
                            x=context["az_i"][m_i],
                            y=context["ran_i"][m_i],
                            marker=dict(
                                color=context["el_i"][m_i],
                                cmin=el_min,
                                cmax=el_max,
                                colorscale=dark_blue_scale,
                            ),
                        ),
                        go.Scatter(
                            x=context["az_o"][m_o],
                            y=context["ran_o"][m_o],
                            marker=dict(
                                color=context["el_o"][m_o],
                                cmin=el_min,
                                cmax=el_max,
                                colorscale=dark_blue_scale,
                            ),
                        ),
                    ],
                )
            )
        fig.frames = frames

        fig.update_layout(
            title=f"{signal_name} Density Without/With Animation by scanindex",
            xaxis_title="Azimuth (deg)",
            yaxis_title="Range",
            xaxis2_title="Azimuth (deg)",
            yaxis2_title="Range",
            xaxis=dict(range=[az_min, az_max]),
            yaxis=dict(range=[ran_min, ran_max]),
            xaxis2=dict(range=[az_min, az_max]),
            yaxis2=dict(range=[ran_min, ran_max]),
            updatemenus=[{
                "type": "buttons",
                "showactive": False,
                "buttons": [
                    {
                        "label": "Play",
                        "method": "animate",
                        "args": [None, {"frame": {"duration": 300, "redraw": True}, "fromcurrent": True}],
                    },
                    {
                        "label": "Pause",
                        "method": "animate",
                        "args": [[None], {"frame": {"duration": 0, "redraw": False}, "mode": "immediate"}],
                    },
                ],
            }],
            sliders=[{
                "active": 0,
                "steps": [
                    {
                        "label": str(scan),
                        "method": "animate",
                        "args": [[str(scan)], {"frame": {"duration": 0, "redraw": True}, "mode": "immediate"}],
                    }
                    for scan in unique_scans
                ],
            }],
        )
        return f"hist_fig_{signal_name}_density_animated", fig

    def histogram_scanindex_object_count_line(self, signal_name, data_dict, shared_data, lock):
        context = self._build_radar_context(signal_name, data_dict, shared_data)
        if context is None:
            return None, None

        si_i = context["si_i"].astype(int)
        si_o = context["si_o"].astype(int)
        unique_scan = np.unique(np.concatenate([si_i, si_o]))
        in_counts = np.array([np.count_nonzero(si_i == scan) for scan in unique_scan], dtype=int)
        out_counts = np.array([np.count_nonzero(si_o == scan) for scan in unique_scan], dtype=int)

        fig = go.Figure()
        fig.add_trace(go.Scatter(x=unique_scan, y=in_counts, mode="lines+markers", name="INPUT objects", line=dict(color="green")))
        fig.add_trace(go.Scatter(x=unique_scan, y=out_counts, mode="lines+markers", name="OUTPUT objects", line=dict(color="red")))
        fig.update_layout(
            title=f"{signal_name} Object Count per scanindex",
            xaxis_title="scanindex",
            yaxis_title="Number of objects",
        )
        return f"hist_fig_{signal_name}_object_count_line", fig
