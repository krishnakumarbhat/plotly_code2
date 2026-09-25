import sys
import os

# Fixed paths
SOURCE_PATH = f"./"
destination_file_path = f"../../pyproject.toml"

# Check if command-line arguments are provided
if len(sys.argv) != 2:
    print("Arguments were not provided properly. Syntax: python update_TOML.py <Source File name in Default Path>")
    sys.exit(1)

# Get file names from command-line arguments
source_file_name = sys.argv[1]
# destination_file_name = sys.argv[2]

# Construct full file paths
source_file_path = SOURCE_PATH + source_file_name
# destination_file_path = DESTINATION_PATH + destination_file_name

# Check if the destination file exists and delete its contents
if os.path.exists(destination_file_path):
    # Open the destination file in write mode to delete its contents
    with open(destination_file_path, 'w') as destination_file:
        # Writing an empty string to the file effectively deletes its contents
        destination_file.write('')
else:
    # If the file doesn't exist, create it
    with open(destination_file_path, 'w') as destination_file:
        pass

# Copy content from source file to destination file
with open(source_file_path, 'r') as source_file:
    with open(destination_file_path, 'a') as destination_file:
        # Read the contents of the source file
        source_content = source_file.read()
        # Append the source content to the destination file
        destination_file.write(source_content)

print(f"Content are copied from {source_file_name} to {destination_file_path}")
