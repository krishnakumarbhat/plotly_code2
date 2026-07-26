import importlib
from InteractivePlot.c_data_storage.config_storage import Gen7V1_V2
from InteractivePlot.e_presentation_layer.html_generator import HtmlGenerator
from InteractivePlot.c_data_storage.data_model_storage import DataModelStorage
from InteractivePlot.d_business_layer.utils import time_taken
import multiprocessing as mp
from functools import lru_cache
import logging
import time
import gc
import os
import tempfile
import shutil
try:
    from memory_profiler import memory_usage  # type: ignore
except Exception:
    memory_usage = None

try:
    import psutil  # type: ignore
except Exception:
    psutil = None

class DataPrep:
    """
    Prepares data for visualization by generating plots and handling plot data.
    Acts as a business layer between data storage and presentation.

    This class is responsible for:
    1. Retrieving data from storage layers (input and output)
    2. Processing and transforming data for visualization
    3. Generating appropriate plot types based on signal configuration
    4. Implementing multiprocessing for efficient data processing
    5. Handling KPI calculations.
    6. Passing visualization data to the presentation layer
    """
    # @profile
    def __init__(
        self,
        input_data,
        output_data,
        html_name,
        sensor,
        stream_name,
        input_file_name,
        output_file_name,
        output_dir=None,
        generate_html=True,
    ):
        """
        Initializes DataPrep with necessary parameters.

        Parameters:
            input_data: Input data storage containing signal data
            output_data: Output data storage containing processed signal data
            html_name: Name for the HTML file to be generated
            output_dir: Directory to save HTML reports (defaults to "html")
            stream_name: Type of data stream being processed (e.g., 'Radar', 'DETECTION_STREAM')
            generate_html: Whether to generate HTML file or just return plot data
        """
        logging.basicConfig(level=logging.INFO, 
                            format='%(asctime)s - %(levelname)s - %(message)s')
        self.logger = logging.getLogger(self.__class__.__name__)
        self.input_data = input_data
        self.output_data = output_data

        self.html_name = html_name
        self.output_dir = output_dir or "html"
        self.sensor = sensor
        self.stream_name = stream_name  # Temporary storage for plot data

        self._track_memory_usage()

        self.signal_plot_paths = self.generate_plots()

        # Pass plots to HTML generator if requested
        if generate_html:
            HtmlGenerator(
                self.signal_plot_paths,
                self.html_name,
                self.output_dir,
                input_file_name,
                output_file_name,
                sensor,
                stream_name,
                update_main_index=False,
            )
            shutil.rmtree(self.temp_dir)
        del(self.signal_plot_paths) # to clean up the memory

    def _track_memory_usage(self):
        """
        Track and log memory usage during initialization.
        """
        try:
            if memory_usage is None or psutil is None:
                return
            # Capture memory usage
            mem_usage = memory_usage(
                proc=(lambda: None), 
                max_iterations=1
            )
            process = psutil.Process(os.getpid())
            memory_info = process.memory_info()
            
            self.logger.debug(f"Resident Set Size (RSS): {memory_info.rss / (1024 * 1024):.2f} MB")
            self.logger.debug(f"Virtual Memory Size: {memory_info.vms / (1024 * 1024):.2f} MB")



            # Log memory usage
            self.logger.info(f"Initial memory usage: {max(mem_usage)} MiB")
        except Exception as e:
            self.logger.error(f"Error tracking memory usage: {e}")

            
    @time_taken
    def generate_plots(self):
        """
        Generates HTML content for plots by handling data and delegating plot creation.

        This method:
        1. Identifies unique keys from both input and output data
        2. Prepares data dictionary for processing
        3. Processes signals in parallel with multiprocessing
        4. Organizes plots by stream type in a dictionary following the sequence

        Returns:
        - plots_hash: A dictionary containing plots organized by stream type
        """

        # Import the data calculation module for specialized calculations
        try:
            data_cal_module = importlib.import_module(
                "InteractivePlot.d_business_layer.data_cal"
            )
            data_cal = data_cal_module.DataCalculations()
            # Set the stream name for the data calculations (context)
            data_cal.set_stream_name(self.stream_name)
        except (ImportError, AttributeError) as e:
            self.logger.info(
                f"Warning: Could not import DataCalculations class from data_cal module: {str(e)}"
            )
            data_cal = None

        # Prepare arguments for multiprocessing (one job per signal)
        mp_args = []
        lock = mp.Manager().Lock()
        shared_data = mp.Manager().dict()
        self.temp_dir = tempfile.mkdtemp(prefix="plotly_temp_")
        shared_data["temp_dir"] = self.temp_dir
        stream_signals = Gen7V1_V2.get(self.stream_name, {})
        for signal_name, signal_config in stream_signals.items():
            mp_args.append(
                (
                    signal_name,
                    signal_config,
                    data_cal,
                    shared_data,
                    lock,
                )
            )
        num_processors = max(1, mp.cpu_count() - 2)

        # Multiprocessing is fragile and slow to start on Windows (spawn), and
        # is mainly useful on Linux/HPC. Default to sequential on Windows.
        disable_mp = (os.name == 'nt') or (os.environ.get('INTERACTIVE_PLOT_DISABLE_MP', '0') == '1')

        signal_plot_path = {}

        a = time.time()
        if (not disable_mp) and num_processors > 3:
            try:
                with mp.Pool(
                    processes=max(2, int(num_processors // 1.25)),
                    maxtasksperchild=150,
                    initializer=gc.disable,
                ) as pool:
                    results = pool.map(self._process_signal_plot, mp_args)

                # Combine all dictionaries from results
                for result_dict in results:
                    signal_plot_path.update(result_dict)
            except Exception as e:
                self.logger.info(
                    f"Error in multiprocessing: {str(e)}. Falling back to sequential processing."
                )
                # Fall back to sequential processing if multiprocessing fails
                for args in mp_args:
                    result_dict = self._process_signal_plot(args)
                    signal_plot_path.update(result_dict)
        else:
            # Sequential processing for small datasets or when multiprocessing is not beneficial
            self.logger.info(
                "Using sequential processing for plot generation"
            )
            for args in mp_args:
                result_dict = self._process_signal_plot(args)
                signal_plot_path.update(result_dict)

        # self._process_post_multiprocessing_plots(
        #     data_cal, signal_plot_path, shared_data, lock
        # )

        b = time.time()
        logging.debug(
            f"Time taken by generate plot in parllel or seq figures for {b - a} in {self.stream_name}"
        )


        return signal_plot_path




    # @profile
    @lru_cache(maxsize=1024)
    def _get_data_cached(self, signal_name):
        """
        Cached version of data retrieval to avoid redundant processing.
        Uses lru_cache for performance optimization when the same data
        is requested multiple times.

        Parameters:
        - signal_name: Name of the signal to get data for
        Returns:
        - Tuple data_dict from DataModelStorage.get_data or ('no_data_in_hdf', {}) if not found
        """

        return DataModelStorage.get_data(
            self.input_data,
            self.output_data,
            signal_name,
        )

    def _process_signal_plot(self, args):
        """
        Process a single signal and generate its plots.
        This method is designed to be used with multiprocessing for parallel execution.

        Parameters:
        - args: Tuple containing (signal_name, signal_config, data_cal, data_dict)
          * signal_name: Name of the signal to process
          * signal_config: Configuration for this signal from Gen7V1_v2
          * data_cal: DataCalculations instance for special calculations
          * data_dict: Dictionary mapping scan indices to input and output rows

        Returns:
        - Dictionary mapping plot_type to figure objects
        """
        signal_name, signal_config, data_cal, shared_data, lock = (
            args
        )
        plot_data_dict = {}

        # Get the complete signal name from the 'call' field if available
        complete_signal_name = signal_name
        if "call" in signal_config and len(signal_config["call"]) > 0:
            complete_signal_name = signal_config["call"][0]

        # Get data records for this signal using cached method
        data_dict = self._get_data_cached(signal_name)

        # Try aliases if no data found (fallback mechanism)
        if isinstance(data_dict, tuple) and len(data_dict) > 0:
            if data_dict[0] == "no_data_in_hdf" and "aliases" in signal_config:
                for alias in signal_config["aliases"]:
                    data_dict, alias_data_dict = self._get_data_cached(
                        alias
                    )
                    if data_dict != "no_data_in_hdf":
                        # If we got valid data from an alias, use its data_dict if available
                        if alias_data_dict:
                            data_dict = alias_data_dict
                        break

        if (
            data_dict
            and data_dict != "no_data_in_hdf"
            and not isinstance(data_dict, tuple)
        ):
            temp_dir = shared_data.get("temp_dir")
            if not temp_dir:
                # This should be created once before multiprocessing starts
                return {}
            # Process each plot type for this signal
            for plot_type in signal_config.get("plot_types", []):
                func_name = plot_type
                # Check if the function exists in data_cal.py
                if data_cal and hasattr(data_cal, func_name):
                    # Call the function with complete signal name,data_dict
                    fig_id, figure = getattr(data_cal, func_name)(
                        complete_signal_name, data_dict, shared_data, lock
                    )
                    if figure:  # Only add if figure was created successfully
                        plot_key = f"{complete_signal_name};{plot_type}"
                        json_filename = f"{plot_key}.json"
                        json_path = os.path.join(temp_dir, json_filename)
            
                        try:
                            with open(json_path, 'w', encoding='utf-8') as f:
                                import json
                                json.dump(figure.to_dict(), f, indent=2)
                            plot_data_dict[plot_key] = json_path
                        except Exception as e:
                            logging.error(f"Failed to write plot {plot_key} to {json_path}: {e}")
                        figure = None
                        gc.collect()
                else:
                    # Route function not found messages to logs file
                    logging.debug(f"Function '{func_name}' not found in data_cal.py")

        return plot_data_dict

    # def _process_post_multiprocessing_plots(
    #     self, data_cal, signal_plot_data, shared_data, lock
    # ):
    #     """
    #     Process plots that need to run after all other plots to collect and aggregate data.
    #     These plots typically need shared data from multiple signals.

    #     Parameters:
    #     - data_cal: DataCalculations instance
    #     - signal_plot_data: Dictionary to update with new plots
    #     """
    #     if not data_cal:
    #         return

    #     temp_dir = shared_data.get("temp_dir")
    #     if not temp_dir:
    #         logging.error("Temporary directory not found in shared_data.")
    #         return

    #     # Generate the mismatch summary plot
    #     fig_id, fig = data_cal.bar_mismatch_plots_all(
    #         "DONE_ALL_SIGNALS", None, shared_data, lock
    #     )
    #     if fig:
    #         plot_key = "all_signal;bar_mismatch_plots_all"
    #         json_filename = f"{plot_key}.json"
    #         json_path = os.path.join(temp_dir, json_filename)
    #         try:
    #             with open(json_path, 'w', encoding='utf-8') as f:
    #                 f.write(fig.to_json())
    #             signal_plot_data[plot_key] = json_path
    #         except Exception as e:
    #             logging.error(f"Failed to write plot {plot_key} to {json_path}: {e}")
    #         fig = None
    #         gc.collect()

    #     # Generate the FBI support signals summary plot
    #     fig_id, fig = data_cal.bar_plots_fbi_sup_sig(
    #         "DONE_FBI_SUP_SIG", None, shared_data, lock
    #     )
    #     if fig:
    #         plot_key = "fbi_sup_sig;bar_plots_fbi_sup_sig"
    #         json_filename = f"{plot_key}.json"
    #         json_path = os.path.join(temp_dir, json_filename)
    #         try:
    #             with open(json_path, 'w', encoding='utf-8') as f:
    #                 f.write(fig.to_json())
    #             signal_plot_data[plot_key] = json_path
    #         except Exception as e:
    #             logging.error(f"Failed to write plot {plot_key} to {json_path}: {e}")
    #         fig = None
    #         gc.collect()
