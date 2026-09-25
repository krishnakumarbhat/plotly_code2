#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Created on Tue Apr 15 16:38:57 2024

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

###########################################################
from resim_generic import *
###########################################################


#**********************************************************
#          main function to trigger Quality Tool
#**********************************************************
if __name__ == '__main__':
    validate_argv()
    childCreation()
    collectsessions()
    collectInputLogs()
    #os.system(f"{input_parameter[0]} {input_parameter[2]} {input_parameter[3]} {input_parameter[4]} {input_parameter[6]} {input_parameter[5]}")
    print(input_parameter)
    os.system(f"{input_parameter[0]} {','.join(str(x) for x in input_parameter)}")
