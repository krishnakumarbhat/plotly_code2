import os
from tkinter import *
from threading import *
from datetime import *
from os import *
from subprocess import *
from io import *
from MUDP_Tool_spec_widgets import *


def MUDP_json_writer(bn_califr_log_list, faseth_log_list, srr_debug_log_list, srr_reference_log_list):
    with open('MUDP_json.json', 'w+', encoding='cp1252') as file_pointer:
        file_pointer.write('{ ')
        file_pointer.write('\n	"reprocessingInputFileStreams": [')
        file_pointer.write('\n		{')
        file_pointer.write('\n			"key": "BN_CALIFR",')
        file_pointer.write('\n			"files": [')
        file_pointer.write('\n ')
        file_pointer.write('\n ')
        for a in bn_califr_log_list:
            if a != bn_califr_log_list[-1]:
                file_pointer.write('\n                "' + str(a) + '",')
            else:
                file_pointer.write('\n                "' + str(a) + '"')
        file_pointer.write('\n ')
        file_pointer.write('\n ')
        file_pointer.write('\n			]')
        file_pointer.write('\n		},')
        file_pointer.write('\n		{')
        file_pointer.write('\n			"key": "BN_FASETH",')
        file_pointer.write('\n			"files": [')
        file_pointer.write('\n ')
        file_pointer.write('\n ')
        for b in faseth_log_list:
            if b != faseth_log_list[-1]:
                file_pointer.write('\n                "' + str(b) + '",')
            else:
                file_pointer.write('\n                "' + str(b) + '"')
        file_pointer.write('\n ')
        file_pointer.write('\n ')
        file_pointer.write('\n			]')
        file_pointer.write('\n		},')
        file_pointer.write('\n		{')
        file_pointer.write('\n			"key": "SRR_DEBUG",')
        file_pointer.write('\n			"files": [')
        file_pointer.write('\n ')
        file_pointer.write('\n ')
        for c in srr_debug_log_list:
            if c != srr_debug_log_list[-1]:
                file_pointer.write('\n                "' + str(c) + '",')
            else:
                file_pointer.write('\n                "' + str(c) + '"')
        file_pointer.write('\n ')
        file_pointer.write('\n ')
        file_pointer.write('\n			]')
        file_pointer.write('\n		},')
        file_pointer.write('\n		{')
        file_pointer.write('\n			"key":"SRR_REFERENCE",')
        file_pointer.write('\n			"files": [')
        file_pointer.write('\n ')
        file_pointer.write('\n ')
        for d in srr_reference_log_list:
            if d != srr_reference_log_list[-1]:
                file_pointer.write('\n                "' + str(d) + '",')
            else:
                file_pointer.write('\n                "' + str(d) + '"')
        file_pointer.write('\n ')
        file_pointer.write('\n ')
        file_pointer.write('\n			]')
        file_pointer.write('\n ')
        file_pointer.write('\n		}')
        file_pointer.write('\n ')
        file_pointer.write('\n	]')
        file_pointer.write('\n}')
    return
