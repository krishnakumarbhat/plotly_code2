# -*- coding: utf-8 -*-
"""
Created on Created on Tue Apr 15 16:38:57 2024

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

# import default modules
import os, sys, getpass, datetime, json
from glob import glob

version='4.0'
input_parameter = []
sessions = []
jsonLists = []
input_file_list = []
upuFlag = False


try: 
    if sys.argv[3] == 'highPrio' :
        highPrio = True
        menu_options = ["Customize Docker (CDC | RDD | DET | AF)", "Default Docker"]
except:
    highPrio = False
    menu_options = ["Customize Docker (CDC | RDD | DET | AF)", "dSpace Docker", "Default Docker"]

menu_open = """
****************************************************
* Option | Option For                              *
*--------|-----------------------------------------*"""

menu_close="""****************************************************
"""

hpcc=os.getcwd()
if 'projects' in hpcc:
    cluster = "Southfield"
    server = "projects/"
    inputfile = "/mnt/usmidet/projects/GPO-IFV7XX/7-Tools/ReSimAutoMng/Support/PostProcessing/support_files.txt"
    resimMining = "python /mnt/usmidet/projects/GPO-IFV7XX/7-Tools/ReSimAutoMng/Support/PostProcessing/JB-min.py"
    statsMining = "python /mnt/usmidet/projects/GPO-IFV7XX/7-Tools/ReSimAutoMng/Support/PostProcessing/stats_mining.py"
    input_parameter.append("/mnt/usmidet/projects/GPO-IFV7XX/7-Tools/ReSimAutoMng/Shell/rResim_main.sh")
else:
    cluster = "Krakow"
    server = "PROJECTS/"
    inputfile = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Support/PostProcessing/support_files.txt"
    resimMining = "python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Support/PostProcessing/JB-min.py"
    statsMining = "python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Support/PostProcessing/stats_mining.py"
    input_parameter.append("/net/8k3/e0fs01/irods/PLKRA-PROJECTS/STLA-SMALL/7-Tools/ReSimAutoMng/Shell/rResim_main_highPrio.sh")


resimScript = '/RUN_RESIM.sh'
base_out = "#SBATCH -o"
base_err = "#SBATCH -e"

aptiv = f'''
\t************************************************************************
\t*           ___       ______  _________ _________ __          __       *
\t*          / _ \     |  __  | \__   __/ \__   __/ \ \        / /       *
\t*         / / \ \    | |__| |    | |       | |     \ \      / /        *
\t*        / /___\ \   |  ____|    | |       | |      \ \    / /         *
\t*       / ______\ \  | |         | |     __| |__     \ \__/ /          *
\t*      /_/       \_\ |_|         |_|    /_______\     \____/           *
\t*                                                                      *
\t************************************************************************
\t*                                                                      *
\t*          ReSET:Resim Singularity Execution Pipeline Tool             *
\t*                                                                      *
\t************************************************************************
\t************************************************************************
\t* version: {version}                   Help : mandeep.singh1@aptiv.com       *
\t************************************************************************
****************************************************************************************'''






"""
######################################################################################################
DATE(DD/MM/YY)      NAME                JIRA Id     DESCRIPTION
15/04/2025          Mandeep Singh       FHW-223     splitter for STLA_SCALE1 Resim(created 1st version of file )

######################################################################################################
"""
