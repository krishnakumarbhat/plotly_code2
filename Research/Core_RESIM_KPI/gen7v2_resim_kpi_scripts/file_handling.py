import os
import re

from logger import logger
from config import config

def check_file(file) -> bool:
    result = False
    try:
        if os.path.exists(file):
            try:
                if os.path.getsize(file) > 0:
                    result = True
                else:
                    logger.custom_print(f"[WARNING] {file} file is empty")
            except OSError as e:
                logger.custom_print(f"[WARNING] {file} file size check failed: {e}")
        else:
            logger.custom_print(f"[WARNING] {file} file is missing")
    except Exception as e:
        logger.custom_print(f"[WARNING] {file} file unexpected error: {e}")
    return result

def find_file_pairs(base_path):
    # Pattern to identify version strings
    resim_version_pattern = re.compile(r'_r(?:R)?00\d+_')

    # Create dictionary for resim files {base_name: resim_file}
    input_output_file_dict = {}
    for filename in os.listdir(base_path):
        # go through only resim files
        if resim_version_pattern.search(filename):
            # Create base name by removing version string
            base_name = resim_version_pattern.sub('_', filename)
            input_output_file_dict[base_name] = filename

    return input_output_file_dict

def find_det_related_data_files(base_path):
    data_files = {
        'input': [],
        'output': []
    }

    # Use only those files which have a corresponding resim file
    file_pair_dict = find_file_pairs(base_path)

    input_files = list(file_pair_dict.keys())

    files_of_interest = [s for s in input_files if config.DET_FILE_SUFFIX in s or config.RDD_FILE_SUFFIX in s]
    unique_input_files = set(s.replace(config.DET_FILE_SUFFIX, "").replace(config.RDD_FILE_SUFFIX, "").strip() for s in files_of_interest)
    for filename in sorted(unique_input_files):
        det_filename = filename + config.DET_FILE_SUFFIX
        rdd_filename = filename + config.RDD_FILE_SUFFIX
        cdc_filename = filename + config.CDC_FILE_SUFFIX
        vse_filename = filename + config.VSE_FILE_SUFFIX
        input_det_file_path = os.path.join(base_path, det_filename)
        input_rdd_file_path = os.path.join(base_path, rdd_filename)
        input_cdc_file_path = os.path.join(base_path, cdc_filename)
        input_vse_file_path = os.path.join(base_path, vse_filename)
        if(det_filename in file_pair_dict.keys() and
           rdd_filename in file_pair_dict.keys()): # not checking explicitly for CDC or VSE file as it may not be available always
            output_det_file_path = os.path.join(base_path, file_pair_dict[det_filename])
            output_rdd_file_path = os.path.join(base_path, file_pair_dict[rdd_filename])
            output_cdc_file_path = ""
            if cdc_filename in file_pair_dict.keys():
                output_cdc_file_path = os.path.join(base_path, file_pair_dict[cdc_filename])
            output_vse_file_path = ""
            if vse_filename in file_pair_dict.keys():
                output_vse_file_path = os.path.join(base_path, file_pair_dict[vse_filename])
            # Do not change the order in which the file paths are added,
            # these files paths are accessed later using indexing
            data_files['input'].append((input_det_file_path,
                                        input_rdd_file_path,
                                        input_cdc_file_path,
                                        input_vse_file_path))
            data_files['output'].append((output_det_file_path,
                                         output_rdd_file_path,
                                         output_cdc_file_path,
                                         output_vse_file_path))
    return data_files

def find_ds_related_data_files(base_path):
    data_files = {
        'input': [],
        'output': []
    }

    # Use only those files which have a corresponding resim file
    file_pair_dict = find_file_pairs(base_path)

    input_files = list(file_pair_dict.keys())

    files_of_interest = [s for s in input_files if config.DS_FILE_SUFFIX in s]
    unique_input_files = set(s.replace(config.DS_FILE_SUFFIX, "").strip() for s in files_of_interest)
    for filename in sorted(unique_input_files):
        ds_filename = filename + config.DS_FILE_SUFFIX
        input_ds_file_path = os.path.join(base_path, ds_filename)
        if(ds_filename in file_pair_dict.keys()):
            output_ds_file_path = os.path.join(base_path, file_pair_dict[ds_filename])
            data_files['input'].append(input_ds_file_path)
            data_files['output'].append(output_ds_file_path)
    return data_files

def find_align_related_data_files(base_path):
    data_files = {
        'input': [],
        'output': []
    }

    # Use only those files which have a corresponding resim file
    file_pair_dict = find_file_pairs(base_path)

    input_files = list(file_pair_dict.keys())

    files_of_interest = [s for s in input_files if config.ALIGN_FILE_SUFFIX in s]
    unique_input_files = set(s.replace(config.ALIGN_FILE_SUFFIX, "").strip() for s in files_of_interest)
    for filename in sorted(unique_input_files):
        align_filename = filename + config.ALIGN_FILE_SUFFIX
        input_align_file_path = os.path.join(base_path, align_filename)
        if(align_filename in file_pair_dict.keys()):
            output_align_file_path = os.path.join(base_path, file_pair_dict[align_filename])
            data_files['input'].append(input_align_file_path)
            data_files['output'].append(output_align_file_path)
    return data_files

def find_id_related_data_files(base_path):
    data_files = {
        'input': [],
        'output': []
    }

    # Use only those files which have a corresponding resim file
    file_pair_dict = find_file_pairs(base_path)

    input_files = list(file_pair_dict.keys())

    files_of_interest = [s for s in input_files if config.ID_FILE_SUFFIX in s]
    unique_input_files = set(s.replace(config.ID_FILE_SUFFIX, "").strip() for s in files_of_interest)
    for filename in sorted(unique_input_files):
        id_filename = filename + config.ID_FILE_SUFFIX
        input_id_file_path = os.path.join(base_path, id_filename)
        if(id_filename in file_pair_dict.keys()):
            output_id_file_path = os.path.join(base_path, file_pair_dict[id_filename])
            data_files['input'].append(input_id_file_path)
            data_files['output'].append(output_id_file_path)
    return data_files

def find_rc_related_data_files(base_path):
    data_files = {
        'input': [],
        'output': []
    }

    # Use only those files which have a corresponding resim file
    file_pair_dict = find_file_pairs(base_path)

    input_files = list(file_pair_dict.keys())

    files_of_interest = [s for s in input_files if config.RC_FILE_SUFFIX in s]
    unique_input_files = set(s.replace(config.RC_FILE_SUFFIX, "").strip() for s in files_of_interest)
    for filename in sorted(unique_input_files):
        rc_filename = filename + config.RC_FILE_SUFFIX
        input_rc_file_path = os.path.join(base_path, rc_filename)
        if(rc_filename in file_pair_dict.keys()):
            output_rc_file_path = os.path.join(base_path, file_pair_dict[rc_filename])
            data_files['input'].append(input_rc_file_path)
            data_files['output'].append(output_rc_file_path)
    return data_files