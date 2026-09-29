import xml.etree.ElementTree as ET


class XmlConfigParser:
    """Parser for XML configuration files with validation and error handling"""

    def __init__(self, xml_file):
        """
        Initialize XML configuration parser.

        Args:
            config_path: Path to XML configuration file
        """
        self.xml_file = xml_file
        self.hdf_source_selection = None
        self.hdf_file_type = None
        self.plot_mode = {}
        self.which_costumer = ""

    def parse(self):
        """
        Parse and validate XML configuration file.

        Returns:
            Parsed configuration data
        """

        tree = ET.parse(self.xml_file)
        root = tree.getroot()

        # Extracting HDF source selection and file type
        self.hdf_source_selection = root.find("HDF_SOURCE_SELECTION").text
        self.hdf_file_type = root.find("HDF_FILE").text

        # Extracting plot mode settings
        plot_mode_element = root.find("PLOT_MODE")
        if plot_mode_element is not None:
            for plot in plot_mode_element:
                self.plot_mode[plot.tag] = int(plot.text)
        return self.hdf_file_type

    def get_hdf_source_selection(self):
        return self.hdf_source_selection

    def get_hdf_file_type(self):
        return self.hdf_file_type

    def get_plot_mode(self):
        return self.plot_mode
