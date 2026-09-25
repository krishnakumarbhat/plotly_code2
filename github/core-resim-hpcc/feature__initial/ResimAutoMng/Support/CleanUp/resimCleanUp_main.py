#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Created on Tue Apr 15 16:38:57 2024

@author: kfupyw (Megha.Tandur@aptiv.com)
"""

###########################################################
from resimCleanUp_generic import *
import time
###########################################################


#**********************************************************
#          main function to trigger Quality Tool
#**********************************************************
if __name__ == '__main__':
    csv_file, path_name = validate_argv()
    triplets = read_template_csv(csv_file, path_name)
    operation = get_user_operation()
    staging_dir = get_staging_dir()

    source_list = os.path.join(staging_dir, f".resim_source_list_{os.getpid()}_{int(time.time())}.txt")
    source_count = write_source_list(triplets, source_list)

    if operation == '1':
        copy_list = os.path.join(staging_dir, f".resim_copy_list_{os.getpid()}_{int(time.time())}.txt")
        count = write_copy_list(triplets, copy_list)
        if count == 0:
            print("[INFO] : No copy pairs to submit")
            cleanup_local_files([copy_list, source_list])
        else:
            copy_job_id = submit_copy_job(copy_list, count)
            if not copy_job_id:
                print("[ERROR] : Failed to capture copy job id")
            else:
                submit_cleanup_job([copy_list, source_list], copy_job_id)
    elif operation == '2':
        if source_count == 0:
            print("[INFO] : No source paths available to zip")
            cleanup_local_files([source_list])
        else:
            submit_zip_job(source_list, path_name)
    else:
        copy_list = os.path.join(staging_dir, f".resim_copy_list_{os.getpid()}_{int(time.time())}.txt")
        count = write_copy_list(triplets, copy_list)

        if count == 0:
            print("[INFO] : No copy pairs to submit")
            if source_count == 0:
                print("[INFO] : No source paths available to zip")
                cleanup_local_files([copy_list, source_list])
            else:
                cleanup_local_files([copy_list])
                submit_zip_job(source_list, path_name)
        else:
            copy_job_id = submit_copy_job(copy_list, count)
            if not copy_job_id:
                print("[ERROR] : Failed to capture copy job id; post-copy job not submitted")
            else:
                if source_count == 0:
                    print("[INFO] : No source paths available to zip")
                    submit_cleanup_job([copy_list, source_list], copy_job_id)
                else:
                    submit_cleanup_job([copy_list], copy_job_id)
                    submit_zip_job(source_list, path_name, dependency_job_id=copy_job_id)
  
    




"""
######################################################################################################
DATE(DD/MM/YY)      NAME                JIRA Id     DESCRIPTION
15/04/2025  sdsd        Mandeep Singh       FHW-223     splitter for STLA_SCALE1 Resim(created 1st version of file )

######################################################################################################
"""
