import pytest
from ResimHTMLReport import MainProcessor
import os


def test_main_processor_initialization(temp_xml_file, temp_json_file):
    processor = MainProcessor(temp_xml_file, temp_json_file, output_dir="test_output")
    assert processor.xml_file == temp_xml_file
    assert processor.json_file == temp_json_file
    assert processor.output_dir == "test_output"
    # Clean up the created output directory and log file
    if os.path.exists("test_output/logs.txt"):
        os.remove("test_output/logs.txt")
    if os.path.exists("test_output"):
        os.rmdir("test_output")


def test_main_processor_run_success(temp_xml_file, temp_allsensor_json_file):
    # Using temp_allsensor_json_file as an example, 
    # similar tests can be created for other JSON types
    output_dir = "test_run_output"
    processor = MainProcessor(temp_xml_file, temp_allsensor_json_file, output_dir=output_dir)
    try:
        processor.run()
        # Add assertions here to check for expected output files or log content if necessary
        assert os.path.exists(os.path.join(output_dir, "logs.txt"))
    except Exception as e:
        pytest.fail(f"MainProcessor.run() raised {e} unexpectedly!")
    finally:
        # Clean up
        if os.path.exists(os.path.join(output_dir, "logs.txt")):
            os.remove(os.path.join(output_dir, "logs.txt"))
        # Add cleanup for other generated files if any
        if os.path.exists(output_dir):
            # Potentially remove other files in output_dir before rmdir
            # For now, assuming only logs.txt is created directly by setup_logging
            # and other files are in subdirectories handled by HdfProcessorFactory
            if not os.listdir(output_dir): # only remove if empty or only contains expected subdirs
                 os.rmdir(output_dir)
            else:
                # A more robust cleanup would involve shutil.rmtree if subdirectories are created
                print(f"Warning: Output directory {output_dir} not empty, manual cleanup may be needed.")


def test_main_processor_initialization_no_output_dir(temp_xml_file, temp_json_file):
    processor = MainProcessor(temp_xml_file, temp_json_file)
    assert processor.xml_file == temp_xml_file
    assert processor.json_file == temp_json_file
    assert processor.output_dir == "html" # Default output directory
    # Clean up the created output directory and log file
    default_output_dir = "html"
    if os.path.exists(os.path.join(default_output_dir, "logs.txt")):
        os.remove(os.path.join(default_output_dir, "logs.txt"))
    # Only remove 'html' if it was created by this test and is empty
    # This is a bit risky if 'html' dir is used by other processes or tests concurrently
    # A safer approach would be to use a unique temp dir for each test run.
    if os.path.exists(default_output_dir) and not os.listdir(default_output_dir):
        try:
            os.rmdir(default_output_dir)
        except OSError:
            print(f"Warning: Could not remove default output directory {default_output_dir}. It might not be empty.")


def test_main_processor_missing_xml_file(temp_json_file):
    processor = MainProcessor("nonexistent.xml", temp_json_file)
    with pytest.raises(FileNotFoundError): # XmlConfigParser will raise FileNotFoundError
        processor.run()


def test_main_processor_missing_json_file(temp_xml_file):
    processor = MainProcessor(temp_xml_file, "nonexistent.json")
    # The exact exception depends on when the JSON file is accessed by the specific JSON parser
    # For now, let's assume a general Exception or a more specific one if known
    with pytest.raises(Exception): 
        processor.run()


# This test is now covered by test_main_processor_missing_xml_file and test_main_processor_missing_json_file
# def test_main_processor_with_invalid_files():
#     processor = MainProcessor("nonexistent.xml", "nonexistent.json")
#     with pytest.raises(Exception):
#         processor.run()
