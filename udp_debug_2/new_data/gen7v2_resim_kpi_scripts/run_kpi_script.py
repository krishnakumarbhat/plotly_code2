
import os
import platform
import subprocess

if platform.system() == "Windows":
   subprocess.run(["cmd", "/c", "run_kpi_script.bat"])
else:
   subprocess.run(["bash", "run_kpi_script.sh"])
