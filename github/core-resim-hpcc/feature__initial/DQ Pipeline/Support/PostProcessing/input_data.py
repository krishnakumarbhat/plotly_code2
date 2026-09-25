import sys, os, xlsxwriter

session=[]
fw_version="APT_SRR_RESIM_"
sensor_version="Sensor Software Version: "
bord_version="BORDNET_TOOL V"
html_version="ResimHTMLReport.exe version"


busSpecId = 'Please provide trace file with Bus Spec ID,'
logDuration = 'Log duration is '

timeout = "CANCELLED AT "
oomKilled = "Killed"
crash = "(core dumped)"
badLog = "[WARNING]: Resim encountered bad Log:"
calibError = "Resimulation Operation Completed with calib errors..."
mdfError = "Resimulation Operation Completed with mdf file errors..."
strmError = "[ERROR]: Unsupported stream size :"
blfdeletion = "The log is having zero timestamp for continuous 100 cycles hence deleting the BLF resimulated file"

jobCompleted = "[Pipeline_Execution] : total pipeline execution time is "
resim = "Time taken by SIL engine is "
bordnet = "Time taken for the Overall Execution of Bordnet application is "
html = "[HTML_Execution] : total html execution time is "
inputExecuted = "[Splitter_Execution] : execution requested for - "

log1="[INFO]: Calibration data decoding is successful for the log"
log2="[INFO]: Running Resim for Log"
log3="[INFO]: Processing "
log4="[INFO]: Running Sequential mode Resim for Log"
