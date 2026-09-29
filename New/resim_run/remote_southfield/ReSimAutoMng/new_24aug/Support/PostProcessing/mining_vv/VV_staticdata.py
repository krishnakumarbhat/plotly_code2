import sys, os, xlsxwriter

####### job success | failure scenerios
jobComplete="[Pipeline_Execution] : total pipeline execution time is "
timeout = "CANCELLED AT "
oomKilled = "OOM Killed"
crash = "(core dumped)"


####### versions        
vv_sil_version="VV_SIL_ENGINE_"

sm2_version="SM2 Ver:"
mesh_version="Mesh Version = "

lm2_version="LM2 Ver:"
fw_sil_version="[SIL FW] Major version:"
sensor_version="Sensor Software Version: "
dc_version="ECU Software Version: "
sensor="[DC_INFO : Sensor-cal    ] : "
binaryBuild="[DC_INFO : Binary-detail ] : "
tracker_version="[DC_INFO : Tracker       ] : "
rspp_version="[DC_INFO : RSPP Version  ] : "
dcCofigFor="[DC_INFO : Sub-Variant   ] : "


####### additional information
inputExecuted="[Splitter_Execution] : execution requested for - "
runMode="Configured Run Mode : "
silMode="SIL entrypoint is "
fmuUnload="Library FMU Unload Success"
conversionOSI="conversion of  OSI trace to Sensor Data - "

streamEnd="End of stream reached"
vvComplete="SM SIL ENGINE execution completed"
resim="[Resim_Execution] : total resim execution time is "

features={}
versionDetails={
    "VV SIL Engine":['Main', 'NA'],
    "Sensor Model":{'Main':'NA', 'Mesh':'NA'},
    "Logic Model":{'Main':'NA', 'SIL FW':'NA','Sensor Software Version':'NA', 'DC Software Version':'NA','Sensor Cal':'NA','Binary Build':'NA','Tracker Version':'NA','RSPP Version':'NA','Sub Variant':'NA'}
    }



####### global values
try: jobout_list=sys.argv[1]
except: jobout_list="./mining1.txt"

#xlsx data
sessionTotal=0
sessionPass=0
sessionFail=0
sessionTimeout=0
sessionOOM=0
sessionCrash=0

breifheader=['Feature','Total Scenerios','Yeild', 'Pass', 'Fail']
detailheader=['Jobout S. No', 'Session', 'Job Status', 'Yield %', 'Run-Mode', 'Sil-Mode', 'SM2-Time', 'Job-Time', 'Sensor-Cal', 'Stream-End', 'OSI-Conversion', 'FMU-UNLOAD', 'VV-Completed', 'Feature']

