import sys, os, xlsxwriter, re
import xml.etree.ElementTree as ET
from pathlib import Path


###################### Section for ReSIm mining message to capture
masterList=[]
session=[]
silentry="SIL entrypoint is "
fw_version="APT_SRR_RESIM_"
sensor_version="Sensor Software Version: "
bord_version="BORDNET_TOOL V"
html_version="ResimHTMLReport.exe version"
mudp_version="MUDP_Log_DataExtracter.exe version"
logVersion="Log Software Version:"
srr_dc="[SRR DC] Software Version:"
mrr_dc="[MRR DC] Software Version:"
bl_check="[INFO]: BAD LOG Checks are"


busSpecId = 'Please provide trace file with Bus Spec ID,'
logDuration = 'Log duration is '

timeout = "CANCELLED AT "
oomKilled = "Killed"
crash = "(core dumped)"
badLog = "[WARNING]: Resim encountered bad Log:"
asyncError = "*[WARNING]: ASYNC Observed: " 
calibErrorPartial = "*[WARNING]: CAIBRATIONFAIL: "
calibErrorComplete = "Resimulation Operation Completed with calib errors..."
mdfError = "Resimulation Operation Completed with mdf file errors..."

jobCompleted = "[Pipeline_Execution] : total pipeline execution time is "
resim = "Time taken by SIL engine is "
resimJobEnd = "[Resim_Execution] : total resim execution time is"
bordIp = ".json : input"
bordOp = ".json : output"
bordnet = "Time taken for the Overall Execution of Bordnet application is "
html = "Total Time taken for"
mudpIp = ".txt : input"
mudpOp = ".txt : output"
mudp = "Time taken for the Overall Execution of MUDP-Extractor application is "
inputExecuted = "[Splitter_Execution] : execution requested for - "

latchedScanCnt = "[INFO]: Scanindexes Latched:"
missedScanCnt = "[INFO]: Bad Scanindexes Observed:"


log1="[INFO]: Calibration data decoding is successful for the log"
log2="[INFO]: Running Resim for Log"
log3="[INFO]: Processing "
log4="[INFO]: Running Sequential mode Resim for Log"



###################### Section for KPI mining message to capture
kpi_new_log = "[INFO] Processing log:"
kpi_accuracy_w_cdc = "[KPI] Accuracy:"
kpi_accuracy_wo_cdc = "[KPI] Accuracy excluding CDC saturation:"
kpi_scan_w_CDC = "[KPI] % of scans with CDC saturation:"
kpi_scan_w_range = "[KPI] % of scans with range saturation:"
kpi_files=["detection_kpi_debug_CDC_MODE.txt"]
mileageYield="[KPI] Mileage Yield "
mileageData={'RL' : 'NA','RR' : 'NA','FR' : 'NA','FL' : 'NA','FC' : 'NA','RC' : 'NA','BL' : 'NA','BR' : 'NA'}

kpiLogDetails={}
slownessAvgJR=[]
slownessAvgRR=[]
slownessAvgHR=[]
slownessAvgBiR=[]
slownessAvgBoR=[]
slownessAvgMiR=[]
slownessAvgMoR=[]


###################### Section for HTML files creation variables
html_Metrix_values={"Total":[], "Resimulated":[], "Segmentation":[], "Bad Log":[], "Bus-Spec Id":[], "TimeOut":[], "Out Of Memory":[],"Partial Calib":[], "No Calib":[], "MDF":[], "Not Resimulated":[]}
html_application_versions = {}#"SIL_ReSim" : "NA", "HTML" : "NA", "BORDNET" : "NA", "MUDP" : "NA"}
html_sensor_versions = {}     # {"SENSOR_Corner_RL" : "NA",, "SENSOR_Corner_RR" : "NA", "SENSOR_Corner_FL" : "NA", "SENSOR_Corner_FL" : "NA"  "SENSOR_Side" : "NA", "SENSOR_Front" : "NA"}
html_log_versions = {}
html_mining_data=[]
html_badLog=[]
html_crashLog=[]

html_SlowReSim=[]
html_SlowHTML=[]
html_SlowIBORD=[]
html_SlowOBORD=[]
html_SlowIMUDP=[]
html_SlowOMUDP=[]
html_SlowJob=[]
html_Slowverall=[]

DQ_simg = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/7-Tools/Tools/Quality_Checker/resim_tool_mudp.simg"
DQ_config="/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/7-Tools/ReSimAutoMng/config/MUDP_DATA_Quality_config_gen7_seq.xml"
