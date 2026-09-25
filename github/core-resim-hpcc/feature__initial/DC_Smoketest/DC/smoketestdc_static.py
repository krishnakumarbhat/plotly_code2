# -*- coding: utf-8 -*-
"""
Created on Created on Wed Dec 10 08:38:57 2025

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

import sys, os, getpass
import datetime, shutil
import importlib.util
from pathlib import Path
from smoketestdc_masterscript import script_master
import xml.etree.ElementTree as ET


input_parameters = []
runScript = ""
loginfo = ""
tags = {}

hpcc=os.getcwd()
if 'projects' in hpcc:
    server = "projects/"
    project= hpcc.split(server)[1].split('/')[0]
    resimMining = "python /mnt/usmidet/projects/STLA-THUNDER/7-Tools/ReSimAutoMng/Support/PostProcessing/JB-min.py"
else:
    server = "PROJECTS/"
    project= hpcc.split(server)[1].split('/')[0]
    resimMining = "python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Support/PostProcessing/JB-min.py"
