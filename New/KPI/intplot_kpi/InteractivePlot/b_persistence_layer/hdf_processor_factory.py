from InteractivePlot.b_persistence_layer.allsensor_hdf_parser import AllsensorHdfParser
from InteractivePlot.b_persistence_layer.Persensor_hdf_parser import PersensorHdfParser
from InteractivePlot.b_persistence_layer.paired_hdf_processor import PairedHdfProcessor
from InteractivePlot.d_business_layer.utils import time_taken
from InteractivePlot.e_presentation_layer.html_generator import HtmlGenerator


class HdfProcessorFactory:
    """
    Factory class for handling different types of HDF5 file formats and creating appropriate parsers.

    This class is part of the persistence layer and serves as a bridge between the raw data (HDF5 files)
    and the business layer. It determines which parser to use based on the HDF file type and handles
    the extraction of metadata such as available sensors.

    The factory supports three main HDF file formats:
    1. HDF_WITH_ALLSENSOR - A single HDF file containing data for all sensors
    2. HDF_PER_SENSOR - Separate HDF files for each sensor (typically with FL, FR, RL, RR suffixes)
    3. HDF_PAIRED_FILES - Paired HDF5 files for simultaneous processing
    """

    def __init__(self, input_output_data, hdf_file_type, output_dir):
        """
        Initialize the HDF processor factory.

        Parameters:
            input_output_data: Either a dictionary mapping input file paths to output file paths (for HDF_WITH_ALLSENSOR and HDF_PER_SENSOR),
                             or a list of dictionaries with input_pair and output_pair (for HDF_PAIRED_FILES)
            hdf_file_type (str): Type of HDF file format ("HDF_WITH_ALLSENSOR", "HDF_PER_SENSOR", or "HDF_PAIRED_FILES")
            output_dir (str): Directory to save the generated output files
        """
        self.input_output_data = input_output_data
        self.hdf_file_type = hdf_file_type
        self.output_dir = output_dir


    @time_taken
    async def process_async(self):
        """
        Process the HDF files asynchronously by creating and using appropriate parsers based on the file type.

        This method:
        1. Selects the appropriate parser class based on the HDF file type
        2. Creates an instance of the parser with the input/output file mapping
        3. Delegates the parsing process to the selected parser's async method if available
        4. Creates a master index HTML file with nested dropdowns after processing

        Returns:
            The result from the parser's parse method (typically processed data)

        Raises:
            ValueError: If an invalid HDF file type is specified
        """
        result = None
        
        if self.hdf_file_type == "HDF_PER_SENSOR":
            parser_wrapper = PersensorHdfParser(self.input_output_data, self.output_dir)
            # PersensorHdfParser doesn't have async method, so use the sync one
            result = parser_wrapper.parse()
        elif self.hdf_file_type == "HDF_WITH_ALLSENSOR":
            parser_wrapper = AllsensorHdfParser(self.input_output_data, self.output_dir)
            result = parser_wrapper.parse()
	    #         return asyncio.run(parser_wrapper.async_parse_files())
        elif self.hdf_file_type == "HDF_PAIRED_FILES":
            processor = PairedHdfProcessor(self.input_output_data, self.output_dir)
            # PairedHdfProcessor doesn't have async method, so use the sync one
            result = processor.parse()
        else:
            raise ValueError("Invalid HDF_FILE type in XML configuration.")

        # After all processing for this run, build a single master index once
        try:
            HtmlGenerator.create_master_index(self.output_dir, base_filename="master_index")
        except Exception:
            # Non-fatal; continue even if index creation fails
            pass

        return result
