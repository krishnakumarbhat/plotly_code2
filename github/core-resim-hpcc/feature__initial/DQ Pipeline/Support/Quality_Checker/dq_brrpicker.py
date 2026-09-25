from dq_splitter import get_brr_files
from dq_input_static import converterSimg, converterConfig
import os


#**********************************************************
#          function to pick brr files using get_brr_files
#          from dq_splitter for a given path
#**********************************************************
def pick_brr_files(path):
    brr_file = get_brr_files(path)
    print(f'[INFO] : Found {len(brr_file)} .brr file(s) in {path}')
    return brr_file


#**********************************************************
#          function to create input.json with same key-value
#          structure as 20260326.json, populating SRR_DEBUG
#          files with the brr_file list
#**********************************************************
def create_input_json(brr_file, output_json_path):
    # build SRR_DEBUG files block: each path wrapped in quotes,
    # comma after every entry except the last
    files_lines = []
    for i, f in enumerate(brr_file):
        if i < len(brr_file) - 1:
            files_lines.append(f'"{f}",')
        else:
            files_lines.append(f'"{f}"')
    files_block = '\n\t\t\t'.join(files_lines)

    content = (
        '{\n'
        '  "reprocessingInputFileStreams":\n'
        '\t[\n'
        '\t\t{\n'
        '\t\t"key": "BN_CALIFR",\n'
        '\t\t\t"files": [\n'
        '\t\t\t]\n'
        '\t\t},\n'
        '\t\t{\n'
        '\t\t"key": "BN_FASETH",\n'
        '\t\t\t"files": [\n'
        '\t\t\t]\n'
        '\t\t},\n'
        '\t\t{\n'
        '\t\t"key": "SRR_DEBUG",\n'
        '\t\t\t"files": [\n'
        f'\t\t\t{files_block}\n'
        '\t\t\t]\n'
        '\t\t},\n'
        '\t\t{\n'
        '\t\t"key": "SRR_REFERENCE",\n'
        '\t\t\t"files": [\n'
        '\t\t\t]\n'
        '\t\t}\n'
        '\t]\n'
        '}'
    )

    with open(output_json_path, 'w') as f:
        f.write(content)

    print(f'[INFO] : Created {output_json_path} with {len(brr_file)} file(s) in SRR_DEBUG')


#**********************************************************
#          function to run the converter singularity tool
#          converts .brr files (via input.json) to .mf4
#          command: singularity exec <SIMG_PATH> /RUN_CONVERTER.sh <CONFIG> <JSON> <OUTPUT_PATH>
#**********************************************************
def run_converter(simg_path, config_path, input_json_path, output_path):
    if not os.path.isfile(simg_path):
        print(f'[ERROR] : [Converter_Execution] : simg not found: {simg_path}')
        return 1
    if not os.path.isfile(config_path):
        print(f'[ERROR] : [Converter_Execution] : config not found: {config_path}')
        return 1
    if not os.path.isfile(input_json_path):
        print(f'[ERROR] : [Converter_Execution] : input json not found: {input_json_path}')
        return 1

    os.makedirs(output_path, exist_ok=True)

    cmd = f"singularity exec {simg_path} /RUN_CONVERTER.sh {config_path} {input_json_path} {output_path}"
    print(f'[INFO] : [Converter_Execution] : starting')
    print(f'[INFO] : [Converter_Execution] : {cmd}')
    exit_code = os.system(cmd)
    if exit_code == 0:
        print(f'[INFO] : [Converter_Execution] : completed successfully')
    else:
        print(f'[ERROR] : [Converter_Execution] : failed with exit code {exit_code}')
    return exit_code


#**********************************************************
#          top-level function to orchestrate the full
#          brr -> mf4 -> DQ pipeline:
#            1. input.json already created in jobout/ by
#               get_brr_files() in dq_splitter.py
#            2. run converter to produce .mf4 files
#            3. return converted output path for DQ analysis
#**********************************************************
def run_brr_dq_pipeline(job_path):
    # input.json already exists — created by get_brr_files() via dq_splitter
    input_json_path = f"{job_path}/jobout/input.json"

    # run converter; output lands in jobout/converted_mf4/
    converted_output_path = f"{job_path}/jobout/converted_mf4"
    exit_code = run_converter(converterSimg, converterConfig, input_json_path, converted_output_path)
    if exit_code != 0:
        print(f'[ERROR] : [BRR_Pipeline] : converter failed, DQ analysis will not run')
        return None

    # return converted output path so get_deb_files() can pick up .mf4 files
    print(f'[INFO] : [BRR_Pipeline] : converted .mf4 files available at {converted_output_path}')
    return converted_output_path


#**********************************************************
#          function to collect all converted .mf4 file paths
#          from jobout/converted_mf4/ and write them to
#          jobout/converted_mf4_paths.txt
#**********************************************************
def create_converted_mf4_paths(job_path):
    converted_folder = f"{job_path}/output/jobout"
    output_txt = f"{job_path}/output/jobout/converted_mf4_paths.txt"

    mf4_files = []
    for root, dirs, files in os.walk(converted_folder):
        for file in files:
            if '.mf4' in file or '.MF4' in file:
                mf4_files.append(f"{root}/{file}")
    mf4_files.sort()

    with open(output_txt, 'w') as f:
        for path in mf4_files:
            f.write(path + '\n')

    print(f'[INFO] : [Converted_MF4] : found {len(mf4_files)} .mf4 file(s), written to {output_txt}')
    return output_txt

