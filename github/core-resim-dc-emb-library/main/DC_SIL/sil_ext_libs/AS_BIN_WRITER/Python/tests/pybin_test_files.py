# Auxiliary module that manages the test files used by the actual tests

import os
import zipfile
import shutil


base_dir = os.path.dirname(__file__)
extract_dir = os.path.join(base_dir, "tmp")

bin_zip_path = os.path.join(base_dir, "bin.zip")
bin_dir = os.path.join(extract_dir, "bin")

mat_zip_path = os.path.join(base_dir, "mat.zip")
mat_dir = os.path.join(extract_dir, "mat")


def extract_test_files(mat_files=False):
    with zipfile.ZipFile(bin_zip_path) as zf:
        zf.extractall(bin_dir)

    if mat_files:
        with zipfile.ZipFile(mat_zip_path) as zf:
            zf.extractall(mat_dir)


def list_bin_files():
    bin_files = os.listdir(bin_dir)
    bin_names = []
    bin_ext = []
    bin_paths = []
    for bin_file in bin_files:
        f_parts = os.path.normpath(bin_file).split(os.path.sep)
        f_name, f_ext = os.path.splitext(f_parts[-1])
        if f_ext == ".bin" or f_ext == ".bin32":
            bin_names.append(f_name)
            bin_ext.append(f_ext)
            bin_paths.append(os.path.join(bin_dir, bin_file))

    return bin_names, bin_ext, bin_paths


def remove_test_files():
    shutil.rmtree(extract_dir)
