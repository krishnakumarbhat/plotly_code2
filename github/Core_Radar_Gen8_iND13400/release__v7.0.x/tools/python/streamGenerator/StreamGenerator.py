"""
This is a script to regenerate the Logging streams file with .xml, .c, .h, etc formate files while bazel build running.

It shall utilize the stream generation tool and for inclusion of herader files(ex: radar_sw_config.h),
it consider input (location of header file)as config file which is generated/created by this script.
"""
import subprocess
import shutil
import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--stream_gen_executable", type=str, required=True)
parser.add_argument("--input_logging_files", type=argparse.FileType("r"), required=True)
parser.add_argument("--config_file", type=str, required=True)
parser.add_argument("--output_folder", type=str, required=True)
parser.add_argument("--supported_folder", type=str, required=True)
parser.add_argument("--input_supported_files", type=argparse.FileType("r"), required=True)
parser.add_argument("--resim_stream_def_zip_path", type=str, required=True)
parser.add_argument("--is_windows", default=0, type=int, required=True)
args = parser.parse_args()

if not os.path.isdir(args.supported_folder):
    os.makedirs(args.supported_folder)

for file_temp in args.input_supported_files.readlines():
    file_temp = file_temp.strip()
    filename = os.path.basename(file_temp)
    dest_temp = f"{args.supported_folder}/{filename}"
    shutil.copyfile(file_temp, dest_temp)

for file in args.input_logging_files.readlines():
    file = file.strip()
    filename = os.path.basename(file)
    dest = f"{args.output_folder}/{filename}"
    shutil.copyfile(file, dest)
    if os.path.exists(f"{args.stream_gen_executable}"):
        if args.is_windows:
            command = f"{args.stream_gen_executable} {dest} {dest} single {args.config_file}"
            subprocess.call(command, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        else:
            command = f"./{args.stream_gen_executable} {dest} {dest} single {args.config_file}"
            subprocess.call(command, shell=True)
    else:
        print("Path error : " + f"{args.stream_gen_executable}")

archived = shutil.make_archive(args.resim_stream_def_zip_path, "zip", args.output_folder)
if os.path.exists(args.resim_stream_def_zip_path):
    print(archived)
else:
    print("ZIP file not created")
