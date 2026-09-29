# -*- coding: utf-8 -*-
"""
Created on Created on Tue Apr 15 16:38:57 2024

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

# import default modules
import os, sys, getpass, datetime, json, re
from glob import glob

version='5.0'
input_parameter = []
sessions = []
jsonLists = []
input_file_list = []
upuFlag = False

PAIRS_PER_JOB = 10

bus_tag = 'b04'
highPrio = False
rm_zero = False
use_cross_session_batching = False  # Set to True if nested sessions detected (Scenario 2)
menu_options = ["Customize Docker (CDC | RDD | DET | AF)", "dSpace Docker", "Default Docker"]

# Optional trailing args (argv[3:]) may contain, in ANY order:
#   "highPrio"     -> run in high-priority (splitter) mode
#   "b02" / "b04"  -> bus-log tag override (default "b04")
#   "rm_zero"      -> exclude the 0th log segment (e.g. "..._0000_b05.MF4") and its
#                     corresponding b04/b02/bus log entirely from the resim JSONs
# NOTE: On Southfield, Main/rResim_Gen7.sh and Main/rResim_VV.sh route between two
# different python entry points based on argv[3] == 'highPrio'. To keep that routing
# working, always place "highPrio" BEFORE the bus tag/rm_zero when both are used.
# Supported invocations:
#   rResim.sh <input> <simg>
#   rResim.sh <input> <simg> highPrio
#   rResim.sh <input> <simg> b02
#   rResim.sh <input> <simg> highPrio b02
#   rResim.sh <input> <simg> rm_zero
#   rResim.sh <input> <simg> highPrio b02 rm_zero
for _arg in sys.argv[3:]:
    if _arg == 'highPrio':
        highPrio = True
    elif _arg.lower() in ('b02', 'b04'):
        bus_tag = _arg.lower()
    elif _arg.lower() == 'rm_zero':
        rm_zero = True

if highPrio:
    menu_options = ["Customize Docker (CDC | RDD | DET | AF)", "Default Docker"]

menu_open = """
****************************************************
* Option | Option For                              *
*--------|-----------------------------------------*"""

menu_close="""****************************************************
"""

hpcc=os.getcwd()
_script_dir = os.path.dirname(os.path.abspath(__file__))
_repo_root = os.path.dirname(os.path.dirname(_script_dir))

# Set defaults for cluster-specific variables
load_simg = "module load singularity/3.11.4_no_local"
simg_exec = "singularity exec --no-mount /local --bind /app:/app"

if 'projects' in hpcc:
    cluster = "Southfield"
    server = "projects/"
    inputfile = os.path.join(_repo_root, 'Support', 'PostProcessing', 'support_files.txt')
    splitter_path = os.path.join(_repo_root, 'Shell', 'splitter_highPrio.sh')
    resimMining = f"python {os.path.join(_repo_root, 'Support', 'PostProcessing', 'JB-min.py')}"
    statsMining = f"python {os.path.join(_repo_root, 'Support', 'PostProcessing', 'stats_mining.py')}"
    sourceEnv = f"source {os.path.join(_repo_root, 'env', 'gen7v2', 'bin', 'activate')}"
    input_parameter.append(os.path.join(_repo_root, 'Shell', 'rResim_main.sh'))
    load_simg = "module load singularity/3.8.0"
    simg_exec = "singularity exec"
else:
    # Helios and Krakow share the same CEER-PROGRAM codebase.
    # Routing is handled by trig_pip.sh, so this branch handles both clusters.
    cluster = "Helios/Krakow"
    server = "PROJECTS/"
    inputfile = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Pipeline/Support/PostProcessing/support_files.txt"
    splitter_path = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Pipeline/Shell/splitter_highPrio.sh"
    resimMining = "python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Pipeline/Support/PostProcessing/JB-min.py"
    statsMining = "python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Pipeline/Support/PostProcessing/stats_mining.py"
    sourceEnv = "source /net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Pipeline/Environment/env/bin/activate"
    input_parameter.append("/net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Pipeline/Shell/rResim_main_highPrio.sh")
    load_simg = "module load singularity/3.11.4_no_local"
    simg_exec = "singularity exec --no-mount /local --bind /app:/app"


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
\t* version: {version}                   Help : Pranjal.Singh@aptiv.com       *
\t************************************************************************
****************************************************************************************'''
