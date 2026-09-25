# -*- coding: utf-8 -*-
"""
Created on Created on Wed Dec 10 08:38:57 2025

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

from smoketestdc_execution import *

if __name__ == "__main__":
    validateargv()
    create_execution_scripts()
    sil_cnt = multiplex_silEngine(input_parameters[1], input_parameters[3] ,f'{input_parameters[5]}/jobout')
    script_main=f"{input_parameters[5]}/jobout/.rmain.sh"
    #print(f"[M-INFO] : Total SIL Configurations generated: {sil_cnt}")
    #print(f"0: {input_parameters[0]}\n1: {input_parameters[1]}\n2: {input_parameters[2]}\n3: {input_parameters[3]}\n4: {input_parameters[4]}")
    #print(f"0: {script_main}\n1: {sil_cnt}\n2: {input_parameters[0]}\n3: {input_parameters[1]}\n4: {input_parameters[2]}\n5: {input_parameters[3]}\n6: {input_parameters[4]}")
    os.system(f"{script_main} {sil_cnt} {input_parameters[0]} {input_parameters[2]} {input_parameters[4]} {input_parameters[5]}")

