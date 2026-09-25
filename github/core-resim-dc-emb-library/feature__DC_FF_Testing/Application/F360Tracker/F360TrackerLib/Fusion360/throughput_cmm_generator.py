import argparse


def timing_info_cmm_generator(input_file, output_file):
    # Read the content of the input file
    with open(input_file, "r") as file:
        cpp_content = file.read()

    # Initialize a list to store the timing info variable names
    timing_info_variables = []

    # Loop through the lines to find variable assignments
    for line in cpp_content.split("\n"):
        line = line.strip()
        if line.startswith("log->overall_execution_times."):
            parts = line.split("=")
            if len(parts) == 2:
                variable_name = parts[0].strip()
                if variable_name not in ["log->overall_execution_times.time_taken_core_and_udp_packing",
                                         "log->overall_execution_times.time_taken_full_tracker_DLL_func"]:
                    # Replace "log->" with "(*(p_g_tracker_OAL_pong)).output_data.timing_log."
                    variable_name = variable_name.replace("log->", "(*(p_g_tracker_OAL_pong)).output_data.timing_log.")
                    timing_info_variables.append(variable_name)

    # Generate the header row for the CSV file
    header_row = ",".join([var.split(".")[-1] for var in timing_info_variables])

    # Generate the CMM script
    cmm_script = f"""
    PRINT "start Measuring Throughput"
    OPEN #1 ..\\..\\make\\Measurement_scripts\\export.csv /CREATE

    ; Add the header row outside the loop
    WRITE #1 "{header_row}"

    PRINT "RePeaT Measuring Throughput"
    RePeaT 0xFFFF
    (
      WinPOS ,,,,,0

      ; Concatenate the values with commas
    """
    cmm_script += '  WRITE #1 \\\n'
    # Generate the WRITE command for each timing info variable except the last one
    for variable_name in timing_info_variables[:-1]:
        cmm_script += f'    %Decimal Var.FVALUE({variable_name}) "," \\\n'

    # Handle the last variable differently (without a comma and backslash)
    last_variable = timing_info_variables[-1]
    cmm_script += f'    %Decimal Var.FVALUE({last_variable}) \n'

    # Close the CMM script
    cmm_script += ") \n\nPRINT \"Done Measuring Throughput\" \nCLOSE #1"

    # Write the generated CMM script to the output file
    with open(output_file, "w") as cmm_file:
        cmm_file.write(cmm_script)

    print(f"CMM script has been generated and saved to '{output_file}'.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate a CMM script for measuring throughput.")
    parser.add_argument("input_file", help="Path to the input file (e.g., f360_tracker.cpp)")
    parser.add_argument("output_file", help="Path to the output file (e.g., throughput.cmm)")
    args = parser.parse_args()
    timing_info_cmm_generator(args.input_file, args.output_file)
