import h5py
import os
import logging
from InteractivePlot.b_persistence_layer import hdf_parser
from InteractivePlot.b_persistence_layer.prerun_hdf_parser import PreRunPaired
from InteractivePlot.c_data_storage.data_model_storage import DataModelStorage
from InteractivePlot.d_business_layer.data_prep import DataPrep
from InteractivePlot.e_presentation_layer.html_generator import HtmlGenerator


class PairedHdfProcessor:
    """
    Processor for paired HDF5 files that handles synchronization and processing of
    sensors common to both input and output files. The processor simultaneously opens
    all files in a pair, identifies common sensors and streams, and aggregates
    the data into a unified data store for KPI calculations and HTML generation.
    """

    def __init__(self, paired_files_list, output_dir=None):
        """
        Initialize the PairedHdfProcessor.

        Args:
            paired_files_list: List of dictionaries, each containing 'input_pair' and 'output_pair' keys
                            where each value is a list of file paths
            output_dir: Directory to save HTML reports
        """
        self.paired_files_list = paired_files_list
        self.output_dir = output_dir or "html"

        # Create output directory if it doesn't exist
        os.makedirs(self.output_dir, exist_ok=True)

    def _process_file(
        self, hdf_file, sensor: str, stream: str, data_storage: DataModelStorage
    ) -> bool:
        """
        Process a single HDF file for a sensor/stream combination.

        Args:
            hdf_file: Already opened h5py.File object
            sensor: Sensor name
            stream: Stream name
            data_storage: DataModelStorage to store the parsed data

        Returns:
            bool: True if processing was successful, False otherwise
        """
        # try:
        sensor_stream_path = f"{sensor}/{stream}"

        if sensor_stream_path in hdf_file:
            data_group = hdf_file[sensor_stream_path]

            # Define possible header names and find the correct one
            header_variants = [
                "Stream_Hdr",
                "stream_hdr",
                "StreamHdr",
                "STREAM_HDR",
                "streamheader",
                "stream_header",
            ]
            header_path = next(
                (variant for variant in header_variants if variant in data_group), None
            )

            if header_path:
                scan_index = data_group[f"{header_path}/scan_index"][()]
                data_storage.initialize(scan_index, sensor, stream,False)
                data_storage = hdf_parser.HDF5Parser.parse(
                    data_group, data_storage, scan_index
                )
                return True
            else:
                print(f"No stream header found in {sensor_stream_path}")
                return False
        else:
            print(f"Sensor/stream path {sensor_stream_path} not found in HDF file")
            return False
        # except Exception as e:
        #     print(f"Error processing HDF file for {sensor}/{stream}: {str(e)}")
        #     return False

    def parse(self):
        """
        Process all paired HDF5 files.

        For each file pair:
        1. Identifies sensors and streams common to both input and output files
        2. For each matching sensor/stream, processes each file in the pair
        3. Generates an HTML report with tabs for each sensor/stream

        Returns:
            List of DataPrep objects
        """
        all_data_preps = []

        # Loop through each pair set
        for pair_index, pair_data in enumerate(self.paired_files_list):
            input_files = pair_data.get("input_pair", [])
            output_files = pair_data.get("output_pair", [])

            # Ensure input and output file lists have the same length
            if len(input_files) != len(output_files):
                print(
                    f"Skipping pair #{pair_index + 1}: Number of input files ({len(input_files)}) "
                    f"doesn't match number of output files ({len(output_files)})"
                )
                continue

            print(f"\nProcessing file pair #{pair_index + 1}:")
            print(
                f"Input files: {', '.join([os.path.basename(f) for f in input_files])}"
            )
            print(
                f"Output files: {', '.join([os.path.basename(f) for f in output_files])}"
            )

            # Use PreRunPaired to analyze files and find common sensors/streams
            prerun = PreRunPaired(input_files, output_files)
            pair_info = prerun.pair_data

            # Create dictionaries to store opened file handles
            input_file_handles = {}
            output_file_handles = {}

            # Open all HDF files simultaneously
            try:
                for i in range(len(pair_info)):
                    input_file_handles[i] = h5py.File(pair_info[i]["input_file"], "r")
                    output_file_handles[i] = h5py.File(pair_info[i]["output_file"], "r")

                # Process each sensor common to all pairs
                for sensor in pair_info[0]["sensors"]:
                    # Make sure sensor has streams defined
                    if sensor in pair_info[0]["streams"]:
                        # streams = pair_info[0]['streams'][sensor]
                        print(f"\nProcessing sensor {sensor}")

                        # Dictionary to store plots for each stream of this sensor
                        sensor_plots_hash = {}
                        sensor_kpi_plots = {}
                        sensor_html_name = None

                        # Process each stream for this sensor
                        for i in range(len(pair_info)):
                            # Process each file pair for this sensor/stream
                            for stream in pair_info[i]["streams"][sensor]:
                                print(f"  Processing stream: {stream}")
                                input_file = pair_info[i]["input_file"]

                                base_name = os.path.basename(input_file).split(".")[0]
                                sensor_html_name = f"{base_name}_{sensor}.html"

                                # Create data storage instances for this pair
                                input_data = DataModelStorage()
                                output_data = DataModelStorage()

                                input_data.init_parent(stream)
                                output_data.init_parent(stream)

                                # Process input file
                                input_success = self._process_file(
                                    input_file_handles[i], sensor, stream, input_data
                                )

                                # Process output file
                                output_success = self._process_file(
                                    output_file_handles[i], sensor, stream, output_data
                                )

                                # If either file was processed successfully, create a DataPrep
                                if input_success or output_success:
                                    try:
                                        # Create DataPrep without generating HTML
                                        data_prep = DataPrep(
                                            input_data,
                                            output_data,
                                            sensor_html_name,
                                            self.output_dir,
                                            stream,
                                            generate_html=False,
                                        )

                                        # Store the plots from this stream
                                        if stream not in sensor_plots_hash:
                                            sensor_plots_hash[stream] = (
                                                data_prep.plots_hash
                                            )

                                        # Store KPI plots if any
                                        if data_prep.kpi_plots:
                                            sensor_kpi_plots["KPI"] = (
                                                data_prep.kpi_plots
                                            )

                                        all_data_preps.append(data_prep)
                                        logging.debug(
                                            f"  Successfully created DataPrep for {sensor}/{stream} in pair {i + 1}"
                                        )
                                    except Exception as e:
                                        print(
                                            f"  Error creating DataPrep for {sensor}/{stream} in pair {i + 1}: {str(e)}"
                                        )

                            # Generate HTML for this sensor with all streams as tabs
                            if sensor_plots_hash and sensor_html_name:
                                # try:
                                HtmlGenerator(
                                    sensor_plots_hash,
                                    sensor_html_name,
                                    self.output_dir,
                                    kpi_plots=sensor_kpi_plots if sensor_kpi_plots else None,
                                )
                                print(
                                    f"Generated HTML report for sensor {sensor} with {len(sensor_plots_hash)} streams"
                                )
                            # except Exception as e:
                            #     print(f"Error generating HTML for sensor {sensor} in {self.output_dir}: {str(e)}")

                print(
                    f"Completed processing all sensors and streams of {self.output_dir}"
                )

            finally:
                # Close all opened file handles
                for handle in input_file_handles.values():
                    handle.close()
                for handle in output_file_handles.values():
                    handle.close()

        print(
            f"\nCompleted processing all file pairs. Generated {len(all_data_preps)} DataPrep objects."
        )
        return all_data_preps
