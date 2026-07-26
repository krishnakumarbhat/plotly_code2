import numpy as np
from InteractivePlot.e_presentation_layer.plotly_visualization import PlotlyCharts
import logging
import plotly.graph_objects as go


class DataCalculations:
    """
    Class containing methods for processing and calculating derived data values
    for various radar signal parameters.
    """

    def __init__(self):
        """Initialize the DataCalculations class"""
        self.stream_name = None
        self._data_frame_cache = {}

    def set_stream_name(self, stream_name: str) -> None:
        """
        Set the stream name for data organization

        Args:
            stream_name: The name of the data stream
        """
        self.stream_name = stream_name

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

        # Check if there are mismatches
        if len(all_mismatches) > 0:
            # There are mismatches: plot them
            plot_signal_name = signal_name
            x_vals = data_dict["MI"][0]
            y_vals = data_dict["MI"][1]
            y2_vals = data_dict["MO"][1]
            color = "red"
            label = "MISMATCH"
        else:
            # No mismatches: plot a placeholder and update signal name
            plot_signal_name = f"{signal_name} (no mismatch)"
            x_vals = data_dict["MI"][0]
            y_vals = data_dict["MI"][1]
            y2_vals = data_dict["MO"][1]
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
