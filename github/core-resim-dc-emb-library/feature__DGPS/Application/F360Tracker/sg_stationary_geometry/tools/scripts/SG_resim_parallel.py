# This script runs SG_resim processing a few logs in parallel.
# Set max_parallel_run below according to the performance of your machine. 4 occupy almost all resources on i5.
#
# Set paths before running!
#
# Run without parametres.

import subprocess
import concurrent.futures
import datetime
import time
import os, sys

############ Configuration ############
resim_path     = r"C:\builds\SG\bin\COMPONENT_RESIM_EXECUTABLE.exe"
log_list_path = r'C:\Users\R4689V\Desktop\FZD-2454\Logs\DS32\input_logs\log_list.txt'
logs_path     = r'C:\Users\R4689V\Desktop\FZD-2454\Logs\DS32\input_logs\\'
lib_include_folder = r"C:\builds\SG\components\resim\resim_wrapper\Release"

max_parallel_run = 6
#######################################

def run_prcoess(log_path):
   print(f"Running: {''.join(log_path)}")
   start_time = time.time()

   try:
      process = subprocess.Popen(
         [
             resim_path,
             "-input_file",
             log_path,
             "-lib_name",
             "stationary_geometries_wrapper",
             "-lib_include_folder",
             lib_include_folder,
             "-logging_folder",
             "out",
             "-data_injection/fill_missing",
             "true",
             "-data_injection/remove_excessing",
             "true",
             "-mode/copy_lib_on_load",
             "true",
         ],
         stdout=subprocess.PIPE,
         stderr=subprocess.PIPE,
         text=True,
         close_fds=True
      )
      stdout, stderr = process.communicate()

      return {
            'command': log_path,
            'returncode': process.returncode,
            'stdout': stdout,
            'stderr': stderr,
            'execution_time': time.time() - start_time
      }
   except Exception as e:
      return {
            'command': log_path,
            'error': str(e),
            'execution_time': time.time() - start_time
      }

# prepare data
log_list_file = open(log_list_path, 'r')

log_lines = log_list_file.readlines()

for i in range(len(log_lines)):
    log_lines[i] = log_lines[i].rstrip('\n')


with concurrent.futures.ThreadPoolExecutor(max_parallel_run) as executor:
   future_to_command = {executor.submit(run_prcoess, log_path): log_path for log_path in log_lines}

   for future in concurrent.futures.as_completed(future_to_command):
      cmd = future_to_command[future]
      try:
         result = future.result()
         print(f"Finished: {''.join(cmd)} with code: {result.get('returncode', 'ERROR')}")
      except Exception as e:
         print(f"Process {''.join(cmd)}  thrown error: {e}")
