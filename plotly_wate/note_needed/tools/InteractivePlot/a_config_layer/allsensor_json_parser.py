import json


class AllsensorJSONParser:
    """Parser for JSON files containing data for all sensors"""

    def __init__(self, json_file):
        """
        Initialize parser with configuration file path.

        Args:
            config_file: Path to JSON configuration file
        """
        self.json_file = json_file
        self.input_output_map = {}
        self.parse()

    def parse(self):
        """
        Parse JSON data according to all-sensor format requirements."
        """

        with open(self.json_file, "r") as file:
            data = json.load(file)
            inputs = data["INPUT_HDF"]
            outputs = data["OUTPUT_HDF"]
            if len(inputs) == len(outputs):
                print("Number of input and output files pairs are Matching.")
                self.input_output_map = dict(zip(inputs, outputs))
            else:
                print(
                    f"Input and output file counts do not match: {len(inputs)} != {len(outputs)}"
                )

    def get_input_output_map(self):
        return self.input_output_map
