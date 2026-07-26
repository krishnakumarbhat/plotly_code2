import json


class PairedJSONParser:
    """Parser for JSON files containing nested pairs of HDF5 files for dual processing"""

    def __init__(self, json_file):
        """
        Initialize parser with configuration file path.

        Args:
            json_file: Path to JSON configuration file with nested pairs
        """
        self.json_file = json_file
        self.paired_files = []
        self.parse()

    def parse(self):
        """
        Parse JSON data according to nested pair format requirements.
        Structure expected:
        {
            "INPUT_HDF": [
                [file1, file2],
                [file3, file4]
            ],
            "OUTPUT_HDF": [
                [output1, output2],
                [output3, output4]
            ]
        }
        """
        with open(self.json_file, "r") as file:
            data = json.load(file)
            inputs = data["INPUT_HDF"]
            outputs = data["OUTPUT_HDF"]

            if len(inputs) == len(outputs):
                print("Matching input and output pairs.")
                for i, (input_pair, output_pair) in enumerate(zip(inputs, outputs)):
                    if len(input_pair) == 2 and len(output_pair) == 2:
                        self.paired_files.append(
                            {"input_pair": input_pair, "output_pair": output_pair}
                        )
                    else:
                        print(
                            f"Warning: Pair #{i + 1} has incorrect number of files (should be 2 files per pair)"
                        )
            else:
                print(
                    f"Input and output pair counts do not match: {len(inputs)} != {len(outputs)}"
                )

    def get_paired_files(self):
        """
        Returns list of dictionaries, each containing 'input_pair' and 'output_pair' keys,
        each with a list of two file paths.
        """
        return self.paired_files

    def get_input_output_map(self):
        """
        Returns the paired files list to maintain compatibility with the existing code.
        This method is required by MainProcessor in ResimHTMLReport.py.
        """
        return self.paired_files
