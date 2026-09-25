import os, sys, getpass, datetime
server = os.getcwd()

if 'projects' in server:
    dqScript = "/mnt/usmidet/projects/STLA-THUNDER/7-Tools/ReSimAutoMng/Shell/QualityChecker_main.sh"
    dqSimg = "/mnt/usmidet/projects/STLA-THUNDER/7-Tools/Resim/OfficialRelease/Data_Quality_Tool/tools_mudp.simg"
    dqCong = "/mnt/usmidet/projects/STLA-THUNDER/7-Tools/Resim/OfficialRelease/Data_Quality_Tool/MUDP_DATA_Quality_config.xml"
    dqMining = "python /mnt/usmidet/projects/STLA-THUNDER/7-Tools/ReSimAutoMng/Support/PostProcessing/DQ_Mining.py"
    converterSimg = "/mnt/usmidet/projects/STLA-THUNDER/7-Tools/Resim/OfficialRelease/Converter/resim_tool_converter.simg"
    converterConfig = "/mnt/usmidet/projects/STLA-THUNDER/7-Tools/Resim/OfficialRelease/Converter/Mdf4_Converter_Config.xml"

else:
    dqScript = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/7-Tools/ReSimAutoMng/Shell/QualityChecker_main.sh"
    dqSimg = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/7-Tools/Tools/Quality_Checker/resim_tool_mudp.simg"
    dqCong = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/7-Tools/Tools/Quality_Checker/MUDP_DATA_Quality_config_gen6.xml"
    dqMining = "python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/7-Tools/ReSimAutoMng/Support/PostProcessing/DQ_Mining.py"
    converterSimg = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/2-Sim/USER_DATA/v0bivq/resim_tool_converter.simg"
    converterConfig = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/2-Sim/USER_DATA/v0bivq/Mdf4_Converter_Config.xml"