"""
Created on Friday Oct 23 2024
@author: d1cse7 (mandeep.singh1@aptiv.com)
@copyright: APTIV
"""

#from JB_min_html import *
from input_data import *
import json
import sys
jobout_list=sys.argv[1]
kpi_list=sys.argv[2]
#customer=sys.argv[3]
#if customer == 'stla_small' : DQ_config="/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/7-Tools/ReSimAutoMng/config/MUDP_DATA_Quality_config_small_gen7_seq.xml"

#jobout_list="C:/Users/d1cse7/Desktop/Work/Scripts/mining.txt"
#customer="stla_scale3"

##############################################################
#
# collect all the jobouts files present in minimg.txt
#
##############################################################
def collect_jobouts():
    print("[INFO] : Collecting Jobouts", end='\r')
    file=open(jobout_list,'r')
    paths=file.readlines()
    file.close()

    # De-dupe by the full line, not just the trailing SLURM_ARRAY_TASK_ID number: Helios
    # chunking restarts that number at 1 for every chunk, so two different chunks can
    # share the same trailing number (e.g. "12345_1.out" vs "12346_1.out") while being
    # completely different sessions - matching on the number alone silently dropped one
    # chunk's entries. A genuine requeue of the same array task re-appends an identical
    # line, so full-line de-dupe still collapses those correctly.
    seen = set()
    jobouts = []
    for path in paths:
        if path not in seen:
            seen.add(path)
            jobouts.append(path)
    print(f"[INFO] : Collected Jobouts for {len(jobouts)} sessions")
    return jobouts


##############################################################
#
# create all file to capture scenerios
#
##############################################################
def create_files(path):
    tmp = open(path+'.txt','w')
    tmp_abort = open(path+'_aborted.txt','w')
    tmp_bad = open(path+'_bad.txt','w')
    tmp_bad_session = open(path+'_bad_session.txt','w')
    scan_session = open(path+'_scan_session.txt','w')
    scanLog_session = open(path+'_scanLog_session.txt','w')
    kpi_session = open(path+'_kpi_session.txt','w')
    kpiLog_session = open(path+'_kpiLog_session.txt','w')
    return tmp, tmp_abort, tmp_bad, tmp_bad_session, scan_session,scanLog_session, kpi_session, kpiLog_session


##############################################################
#
# delete all .txt files
#
##############################################################
def delete_files(path):
    os.remove(path+'.txt')
    os.remove(path+'_aborted.txt')
    os.remove(path+'_bad.txt')
    os.remove(path+'_bad_session.txt')
    os.remove(path+'_scan_session.txt')
    os.remove(path+'_scanLog_session.txt')
    os.remove(path+'_kpi_session.txt')
    os.remove(path+'_kpiLog_session.txt')


##############################################################
#
# close all created files
#
##############################################################
def close_files(files):
    for file in files:
        file.close()


##############################################################
#
# collect all the jobouts files present in minimg.txt
#
##############################################################
def write_initial(file):
    file.write("*******************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************\n")
    file.write(" Logs_Version".ljust(14, ' ')+"| Session".ljust(9, ' ')+"| Status".ljust(10, ' ')+"| yield".ljust(9, ' ')+"| SIL Mode".ljust(12, ' ')+"| File Mode".ljust(12, ' ')+"| BadLog Check".ljust(14, ' ')+"| Logs_T".ljust(9, ' ')+"| Logs_D(sec)".ljust(9, ' ')+"| Logs_E".ljust(9, ' ')+"| Logs_N".ljust(9, ' ')+"| Logs_B".ljust(9, ' ')+"| Log_C".ljust(9, ' ')+"| ReSim".ljust(8,' ')+"| busSpec".ljust(9, ' ')+"| R_Time".ljust(9,' ')+"| R_Time_Profile".ljust(16,' ')+"| R_Mem_Profile".ljust(15,' ')+f"| Jobout : {jobout_list}".ljust(210,' ')+"| Bad Logs Encountered".ljust(70,' ')+'\n')
    file.write("**************|*********************|************|*********|********|********|********|********|********|********|********|*******|*******|*******|********|********|*****************|****************|*****************************************************************************************************************************************************************************************************************|*************************************************************************************\n")


##############################################################
#
# function to collect resim and pipelined tools version
#
##############################################################
def collect_versions(jobs, file):
    version_details={}
    version_collected =[0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    for job in jobs:
        job_file=open(job.split('\n')[0],'r')
        raw=job_file.readlines()
        job_file.close()
        for message in raw:
            if fw_version in message and 'SRR_SIL_RESIM' not in version_details.keys() and version_collected[0] == 0:
                version_details["SRR_SIL_RESIM"]=message.split(fw_version)[-1].split(' ')[0]
                html_application_versions["SIL_ReSim"] = version_details["SRR_SIL_RESIM"]
                version_collected[0] = 1
            
            elif sensor_version in message and 'SENSOR_RL' not in version_details.keys() and "_RL]" in message and version_collected[1] == 0:
                version_details["SENSOR_RL"]=message.split(sensor_version)[-1].split('\n')[0]
                html_sensor_versions["RL"] = version_details["SENSOR_RL"]
                version_collected[1] = 1

            elif sensor_version in message and 'SENSOR_RR' not in version_details.keys() and "_RR]" in message and version_collected[2] == 0:
                version_details["SENSOR_RR"]=message.split(sensor_version)[-1].split('\n')[0]
                html_sensor_versions["RR"] = version_details["SENSOR_RR"]
                version_collected[2] = 1

            elif sensor_version in message and 'SENSOR_FR' not in version_details.keys() and "_FR]" in message and version_collected[3] == 0:
                version_details["SENSOR_FR"]=message.split(sensor_version)[-1].split('\n')[0]
                html_sensor_versions["FR"] = version_details["SENSOR_FR"]
                version_collected[3] = 1

            elif sensor_version in message and 'SENSOR_FL' not in version_details.keys() and "_FL]" in message and version_collected[4] == 0:
                version_details["SENSOR_FL"]=message.split(sensor_version)[-1].split('\n')[0]
                html_sensor_versions["FL"] = version_details["SENSOR_FL"]
                version_collected[4] = 1
            
            elif sensor_version in message and 'SENSOR_FC' not in version_details.keys() and "_FC]" in message and version_collected[5] == 0:
                version_details["SENSOR_FC"]=message.split(sensor_version)[-1].split('\n')[0]
                html_sensor_versions["FC"] = version_details["SENSOR_FC"]
                version_collected[5] = 1
            
            elif sensor_version in message and 'SENSOR_RC' not in version_details.keys() and "_RC]" in message and version_collected[6] == 0:
                version_details["SENSOR_RC"]=message.split(sensor_version)[-1].split('\n')[0]
                html_sensor_versions["RC"] = version_details["SENSOR_RC"]
                version_collected[6] = 1

            elif sensor_version in message and 'SENSOR_BPIL_L' not in version_details.keys() and "BPIL_L]" in message and version_collected[7] == 0:
                version_details["SENSOR_BPIL_L"]=message.split(sensor_version)[-1].split('\n')[0]
                html_sensor_versions["BPIL_L"] = version_details["SENSOR_BPIL_L"]
                version_collected[7] = 1
            
            elif sensor_version in message and 'SENSOR_BPIL_R' not in version_details.keys() and "BPIL_L]" in message and version_collected[8] == 0:
                version_details["SENSOR_BPIL_R"]=message.split(sensor_version)[-1].split('\n')[0]
                html_sensor_versions["BPIL_R"] = version_details["SENSOR_BPIL_R"]
                version_collected[8] = 1

            elif srr_dc in message and 'DC_SRR' not in version_details.keys() and version_collected[9] == 0:
               version_details["DC_SRR"]=message.split(srr_dc)[-1].split('\n')[0]
               html_sensor_versions["DC_SRR"] = version_details["DC_SRR"]
               version_collected[9] = 1

            elif mrr_dc in message and 'DC_MRR' not in version_details.keys() and version_collected[10] == 0:
               version_details["DC_MRR"]=message.split(mrr_dc)[-1].split('\n')[0]
               html_sensor_versions["DC_MRR"] = version_details["DC_MRR"]
               version_collected[10] = 1
            
            #elif bord_version in message and 'BORDNET' not in version_details.keys() and version_collected[11] == 0:
            #    version_details["BORDNET"]=message.split(bord_version)[-1].split('\t')[0]
            #    html_application_versions["BORDNET"] = version_details["BORDNET"]
            #    version_collected[11] = 1
            
            #elif html_version in message and 'HTML' not in version_details.keys() and version_collected[12] == 0:
            #    version_details["HTML"]=message.split(html_version)[-1].split('\t')[0]
            #    html_application_versions["HTML"] = version_details["HTML"]
            #    version_collected[12] = 1

            #elif mudp_version in message and 'MUDP' not in version_details.keys() and version_collected[13] == 0:
            #    version_details["MUDP"]=message.split(mudp_version)[-1].split('\t')[0]
            #    html_application_versions["MUDP"] = version_details["MUDP"]
            #    version_collected[13] = 1
            
    for key, value in version_details.items():
        file.write(f'{key}\t: {value}\n')
        #print(f"{key} : {value}")
    file.write('\n\n')
    print(html_application_versions)
    print(html_sensor_versions)
    html_application_versions["members"] = len(html_application_versions)+len(html_sensor_versions)


##############################################################
#
# function to collect session
#
##############################################################
def get_session(currentInput):
    session="Not Defined"
    filename=Path(currentInput.split(',')[0]).name
    pattern=r'_(\d{6})_'
    match = re.search(pattern, filename)
    if match : session=match.group(1)
    return session


##############################################################
#
# function to collect session latched and missed scan index
#
##############################################################
def update_latched(message, file, latchedScan, missedScan, currentInput):
    def update_data(overall, data):
        for key in data.keys():
            if overall[key] =='NA':overall[key]=data[key]
            else : overall[key]+=data[key]
    scancnt={'RL' : 'NA','RR' : 'NA','FR' : 'NA','FL' : 'NA','FC' : 'NA','RC' : 'NA','BL' : 'NA','BR' : 'NA'}
    scanLogmissed=0
    rawData = message.split('\n')[0].split(':')
    logname = currentInput.split(currentInput.split('/')[-1])[0] + rawData[2].split('/')[-1]
    logname=logname.split('PLKRA-PROJECTS')[-1]
    scansraw = message.split('\n')[0].split(rawData[2])[-1]
    sensorScan = scansraw.split('[')
    for sensor in  sensorScan[1:]:
        info=sensor.split(']:')
        scancnt[info[0]]=int(info[1])

    if latchedScanCnt in message :
        #print(f"{logname} | {scancnt['RL']}| {scancnt['RR']}| {scancnt['FR']}| {scancnt['FL']}| {scancnt['FC']}| {scancnt['RC']}| {scancnt['BL']}| {scancnt['BR']}|")
        update_data(latchedScan,scancnt)
        file.write(f" {logname} | {scancnt['RL']}| {scancnt['RR']}| {scancnt['FR']}| {scancnt['FL']}| {scancnt['FC']}| {scancnt['RC']}| {scancnt['BL']}| {scancnt['BR']}| ")
    elif missedScanCnt in message :
        update_data(missedScan,scancnt)
        for value in scancnt.values():
            if value != 'NA': scanLogmissed+=value
        file.write(f"{scanLogmissed}| {scancnt['RL']}| {scancnt['RR']}| {scancnt['FR']}| {scancnt['FL']}| {scancnt['FC']}| {scancnt['RC']}| {scancnt['BL']}| {scancnt['BR']}\n")


##############################################################
#
# function to mine ReSim data to capture insights
#
##############################################################
def data_mining(raw,inputFiles,cnt,job_path):
    global slownessAvgResimTime,slownessAvgResimMem

    ############################################ alert flags
    resimComp=False
    resimJC=False
    timeoutAlert=False
    oomAlert=False
    calibPAlert = False
    calibFAlert = False
    mdfAlert = False
    noUdpAlert = False
    asyncAlert=False
    busSpecid=None
    busSpecAlert=False
    logCrashed=False
    badSessionAdded=False

    ############################################ initial init for final print variables
    jobStatus="In Run"
    previousLog = ""
    currentInput=""
    totalLogs=0
    logsRan=0
    badLogs=0
    badLogList=[]
    busSpecLogs=0
    sessionTime=0
    resimTime=0
    resimRate=-1
    prevSession=0
    logversion="Not Fetched"
    fileexecution="Not Fetched"
    entrymode="Not Fetched"
    blc="Not Fetched"
    currentRetryJson=""

    logversion_collected ={'RL' : 'NA','RR' : 'NA','FR' : 'NA','FL' : 'NA','FC' : 'NA','RC' : 'NA','BL' : 'NA','BR' : 'NA'}
    latchedScan={'RL' : 'NA','RR' : 'NA','FR' : 'NA','FL' : 'NA','FC' : 'NA','RC' : 'NA','BL' : 'NA','BR' : 'NA'}
    missedScan={'RL' : 'NA','RR' : 'NA','FR' : 'NA','FL' : 'NA','FC' : 'NA','RC' : 'NA','BL' : 'NA','BR' : 'NA'}
    totalSessionScanMissed=0

    # Pre-scan: a log that crashes/times-out/OOMs in one retry attempt can be flagged as a bad log
    # only in a later attempt further down the file. Build the complete bad-log-number set up front
    # so the crash/timeout/OOM checks below aren't fooled by message order.
    badLogNumbersFinal = set()
    _bp_previousLog = ""
    for _bp_message in raw:
        if log1 in _bp_message or log2 in _bp_message or log3 in _bp_message or log4 in _bp_message:
            _bp_previousLog = _bp_message
        elif badLog in _bp_message:
            # Try to extract log name from previousLog first
            _bp_logName = ""
            if _bp_previousLog:
                if '<' in _bp_previousLog and '>' in _bp_previousLog:
                    _bp_logName = _bp_previousLog.split('<')[-1].split('>')[0]
                else:
                    _bp_logName = _bp_previousLog.split('\n')[0]
            # Fallback: try to extract from badLog message itself (look for .MF4 or .mf4 filename)
            if not _bp_logName or '_' not in _bp_logName:
                mf4_match = re.search(r'(IFV[^,\s]+\.MF4|IFV[^,\s]+\.mf4)', _bp_message)
                if mf4_match:
                    _bp_logName = mf4_match.group(1)
            # Extract the log number using the same method as the main loop
            if _bp_logName:
                _bp_match = re.search('_(\d{4})_', _bp_logName)
                _bp_logNum = _bp_match.group(1) if _bp_match else _bp_logName.split('_')[-1].split('.')[0]
                badLogNumbersFinal.add(_bp_logNum)

    ############################################ data mining
    for message in raw:
        if "[Resim_Execution] : attempt " in message:
            crashRecorded = False
            previousLog = ""
            if "JSON=" in message:
                currentRetryJson = message.split("JSON=")[-1].split('\n')[0]
        if fw_version in message: resimComp="Running"
        #elif html_version in message: htmlComp="Running"
        #elif bord_version in message and bordIComp == "Started": bordIComp="Running"
        #elif bord_version in message and bordOComp == "Started": bordOComp="Running"
        #elif mudp_version in message and mudpIComp == "Started": mudpIComp="Running"
        #elif mudp_version in message and mudpOComp == "Started": mudpOComp="Running"
        elif logVersion in message:
            if "_RL]" in message and logversion_collected['RL'] == 'NA':
                logversion_collected['RL']=message.split(logVersion)[-1].split('\n')[0]
                html_log_versions['RL']=logversion_collected['RL']

            elif "_RR]" in message and logversion_collected['RR'] == 'NA':
                logversion_collected['RR']=message.split(logVersion)[-1].split('\n')[0]
                html_log_versions['RR']=logversion_collected['RR']

            elif"_FR]" in message and logversion_collected['FR'] == 'NA':
                logversion_collected['FR']=message.split(logVersion)[-1].split('\n')[0]
                html_log_versions['FR']=logversion_collected['FR']

            elif"_FL]" in message and logversion_collected['FL'] == 'NA':
                logversion_collected['FL']=message.split(logVersion)[-1].split('\n')[0]               
                html_log_versions['FL']=logversion_collected['FL']
                
            elif "_FC]" in message and logversion_collected['FC'] == 'NA':
                logversion_collected['FC']=message.split(logVersion)[-1].split('\n')[0]
                html_log_versions['FC']=logversion_collected['FC']
            
            elif "_RC]" in message and logversion_collected['RC'] == 'NA':
                logversion_collected['RC']=message.split(logVersion)[-1].split('\n')[0]
                html_log_versions['RC']=logversion_collected['RC']

            elif "BPIL_L]" in message and logversion_collected['BL'] == 'NA':
                logversion_collected['BL']=message.split(logVersion)[-1].split('\n')[0]
                html_log_versions['BL']=logversion_collected['BL']
                           
            elif "BPIL_R]" in message and logversion_collected['BR'] == 'NA':
               logversion_collected['BR']=message.split(logVersion)[-1].split('\n')[0]
               html_log_versions['BR']=logversion_collected['BR']
                
        elif resimJobEnd in message : resimJC = True
        elif logDuration in message and resimJC == False:
            sessionTime += int(message.split(logDuration)[-1].split(' ')[0])
        elif inputExecuted in message :
            currentInput=message.split(inputExecuted)[-1].split('\n')[0]
            totalLogs = int(currentInput.split(',')[1])
            
            # For DGPS paired logs, the session marker might over-count if it was derived from b05 files before filtering.
            # We can cross-verify with the actual JSON if available to get the true count of logs intended for resimulation.
            if currentRetryJson != '' and os.path.exists(currentRetryJson):
                try:
                    with open(currentRetryJson, 'r') as f:
                        js_data = json.load(f)
                    for stream in js_data.get('reprocessingInputFileStreams', []):
                        if stream.get('key') == 'SRR_DEBUG':
                            actual_count = len(stream.get('files', []))
                            if actual_count > 0 and actual_count < totalLogs:
                                #print(f"[MINING-FIX] : Adjusting totalLogs from {totalLogs} to {actual_count} based on {currentRetryJson}")
                                totalLogs = actual_count
                            break
                except Exception:
                    pass

            currSession=get_session(currentInput)
            if currSession != prevSession:
                session=currSession
                prevSession=currSession
        elif log1 in message or log2 in message or log3 in message or log4 in message:
            previousLog = message
            if log2 in message or log4 in message:
                logsRan+=1
            if log4 in message: fileexecution="Sequential"
            elif log2 in message: fileexecution="Continous"
        elif silentry in message:
             entrymode=message.split(silentry)[-1].split('\n')[0]
        elif bl_check in message:
            blc=message.split(bl_check)[-1].split('\n')[0]
        elif busSpecId in message:
            busSpecid = message.split(busSpecId)[1].split('\n')[0]
            busSpecLogs += 1
            busSpecAlert = True
        elif resim in message:
            resimTime += float(message.split(resim)[-1].split(' ')[0].split('\n')[0])
            resimComp = "Done"
        #elif html in message and 'secs' in message :
        #    htmlTime = message.split(html)[-1].split(' ')[-2].split('\n')[0]
        #    htmlComp="Done"
        #elif mudpIp in message:
        #    mudpIComp="Started"
        #elif mudpOp in message:
        #    mudpOComp="Started"
        #elif bordIp in message:
        #    bordIComp="Started"
        #elif bordOp in message:
        #    bordOComp="Started"
        #elif bordnet in message :
        #    if bordIComp == "Running" and bordOComp == False :bordITime += float(message.split(bordnet)[-1].split('s')[0].split('\n')[0]); bordIComp="Done"
        #    elif bordOComp == "Running" :bordOTime += float(message.split(bordnet)[-1].split('s')[0].split('\n')[0]); bordOComp="Done"
        #elif mudp in message :
        #    if mudpIComp == "Running" and mudpOComp == False :mudpITime += int(message.split(mudp)[-1].split('s')[0].split('\n')[0]); mudpIComp="Done"
        #    elif mudpOComp == "Running" :mudpOTime += int(message.split(mudp)[-1].split('s')[0].split('\n')[0]); mudpOComp="Done"
        elif badLog in message:
            #print(message)
            index = 1
            logName=previousLog.split('<')[-1].split('>')[0]#.split('/')[-1]
            match=re.search('_(\d{4})_', logName)
            if match : logNum=match.group(1)
            else: logNum=logName.split('_')[-1].split('.')[0]
            if logName not in badLogList:
                badLogs+=1
                if logsRan>=1:
                    logsRan-=1
                badLogList.append(logNum)
                if busSpecAlert == False:
                    inputFiles[2].write(f'{logName}\n')
                    if not badSessionAdded : inputFiles[3].write(f'{currentInput}\n');badSessionAdded=True
        # resimJC (set at "total resim execution time" marker) means the SIL resim engine phase is
        # already over; any "(core dumped)" seen afterwards belongs to the unrelated VIDEO converter
        # step and must not be counted as a resim log crash (it was polluting Crashed_Session/OOM stats).
        elif crash in message and resimJC == False:
            # Extract the log name and number to check if it's already in badLogList
            crash_log_name_check = ''
            if '<' in previousLog and '>' in previousLog:
                crash_log_name_check = previousLog.split('<')[-1].split('>')[0]
            elif previousLog:
                crash_log_name_check = previousLog.split('\n')[0]
            
            # Extract log number the same way as badLogList does to enable proper comparison
            crash_log_num = ''
            match = re.search('_(\d{4})_', crash_log_name_check)
            if match:
                crash_log_num = match.group(1)
            else:
                crash_log_num = crash_log_name_check.split('_')[-1].split('.')[0]
            
            # Do NOT count as crash if this log is already logged as a bad log (to avoid double-counting)
            if crash_log_num not in badLogNumbersFinal:
                # Flag the crash regardless of resimComp's current state, so Log_Crashed always
                # matches what gets written to Crashed_Session below (resimComp can already have
                # flipped away from "Running" by the time this message is seen).
                logCrashed = previousLog.split('_')[-1].split('.')[0]
                if resimComp == "Running":
                    resimComp = "Crash"
                    if logsRan>=1:
                        logsRan-=1
                
                crash_log_name = ''
                if '<' in previousLog and '>' in previousLog:
                    crash_log_name = previousLog.split('<')[-1].split('>')[0].split('/')[-1]
                elif previousLog:
                    crash_log_name = previousLog.split('\n')[0]
                if crash_log_name == '' and currentRetryJson != '':
                    try:
                        with open(currentRetryJson, 'r') as retry_file:
                            retry_json = json.load(retry_file)
                        for stream in retry_json.get('reprocessingInputFileStreams', []):
                            if stream.get('key') == 'SRR_DEBUG':
                                files = stream.get('files', [])
                                if files:
                                        crash_log_name = files[0]
                                break
                    except Exception:
                        pass

                if crashRecorded == False:
                    inputFiles[1].write(f'{job_path}|{crash_log_name}\n')
                    crashRecorded = True
                else:
                    inputFiles[1].write(f'CONTINUE|{crash_log_name}\n')
        elif timeout in message and resimJC == False:
            # Extract the log number to check if it's already in badLogList (to avoid double-counting)
            timeout_log_check = ''
            if '<' in previousLog and '>' in previousLog:
                timeout_log_check = previousLog.split('<')[-1].split('>')[0]
            elif previousLog:
                timeout_log_check = previousLog.split('\n')[0]
            
            # Extract log number the same way as badLogList does
            timeout_log_num = ''
            match = re.search('_(\d{4})_', timeout_log_check)
            if match:
                timeout_log_num = match.group(1)
            else:
                timeout_log_num = timeout_log_check.split('_')[-1].split('.')[0]
            
            if timeout_log_num not in badLogNumbersFinal:
                timeoutAlert = True
                jobStatus = "TimeOut"
                if resimComp == "Running" and logsRan>=1:
                    logsRan-=1
        elif oomKilled in message and resimJC == False:
            # Extract the log number to check if it's already in badLogList (to avoid double-counting)
            oom_log_check = ''
            if '<' in previousLog and '>' in previousLog:
                oom_log_check = previousLog.split('<')[-1].split('>')[0]
            elif previousLog:
                oom_log_check = previousLog.split('\n')[0]
            
            # Extract log number the same way as badLogList does
            oom_log_num = ''
            match = re.search('_(\d{4})_', oom_log_check)
            if match:
                oom_log_num = match.group(1)
            else:
                oom_log_num = oom_log_check.split('_')[-1].split('.')[0]
            
            if oom_log_num not in badLogNumbersFinal:
                oomAlert = True
                jobStatus = "OOM"
                if resimComp == "Running" and logsRan>=1:
                    logsRan-=1
        elif calibErrorPartial in message:
            calibPAlert = True
            jobStatus = "CALIB_P"
        elif calibErrorComplete in message and calibPAlert == False:
            calibFAlert = True
            jobStatus = "CALIB_F"
        elif asyncError in message:
            asyncAlert=message.split(asyncError)[-1].split(' ')[0]
        elif mdfError in message:
            mdfAlert = True
            jobStatus = "MDF"
        elif noUdpError in message:
            noUdpAlert = True
            jobStatus = "NO_UDP"
        elif jobCompleted in message :
            if timeoutAlert == False and oomAlert == False and calibPAlert == False and calibFAlert == False and mdfAlert == False and noUdpAlert == False:
                jobStatus = "Done" if resimComp == "Done" else "Partial"
        elif latchedScanCnt in message or missedScanCnt in message: update_latched(message, inputFiles[5], latchedScan, missedScan,currentInput)
    if resimTime != 0:
            try:resimRate=round(float(resimTime)/(sessionTime),2)
            except:print("resimRate Exception :",resimTime, logsRan, sessionTime)
    if asyncAlert!= False: print(f"ASYN Observed : {asyncAlert} Occurances" )

    # Guard against over-counting when retries print repeated success lines.
    logsRan = max(0, min(logsRan, totalLogs))
    logNotRan = max(totalLogs - logsRan, 0)
    try:
        if totalLogs > 0:
            sessionYield = round(float(logsRan * 100 / totalLogs), 2)
        else:
            sessionYield = -1
    except:
        sessionYield = -1
    for sensor in logversion_collected:
        if logversion_collected[sensor] != 'NA': 
            if logversion == "Not Fetched" : logversion=f"{sensor}_{logversion_collected[sensor]}"
            else : logversion+=f" : {sensor}_{logversion_collected[sensor]}"  
    
    try: 
        if session:pass
    except:
        session=currentInput.split(currentInput.split('/')[-1])[0].split('PLKRA-PROJECTS')[-1]
    calib_found=False
    for value in missedScan.values():
        if value != 'NA':
            totalSessionScanMissed += value
            calib_found=True
    if calib_found == False: totalSessionScanMissed = -10

    inputFiles[0].write(f" {logversion}".ljust(14, ' ')+f"| {session}".ljust(9, ' ')+f"| {jobStatus}".ljust(10, ' ')+f"| {sessionYield}".ljust(9, ' ')+f"| {entrymode}".ljust(12, ' ')+f"| {fileexecution}".ljust(12, ' ')+f"| {blc}".ljust(14, ' ')+f"| {totalLogs}".ljust(9, ' ')+f"| {sessionTime}".ljust(9, ' ')+f"| {logsRan}".ljust(9, ' ')+f"| {logNotRan}".ljust(9, ' ')+f"| {badLogs}".ljust(9, ' ')+f"| {logCrashed}".ljust(9, ' ')+f"| {resimComp}".ljust(8,' ')+f"| {busSpecid}".ljust(9,' ')+f"| {resimTime}".ljust(9,' ')+f"| {resimRate}".ljust(9,' ')+f"| NA".ljust(9,' ')+f"| {currentInput}".ljust(210,' ')+f"| {badLogList}".ljust(70,' ')+'\n')
    currsession=currentInput.split('/')[-1]
    inputFiles[4].write(f" {logversion}".ljust(14, ' ')+f"| {session.split(currsession)[0]}".ljust(9, ' ')+f"| {currsession}".ljust(9, ' ')+f"| {totalSessionScanMissed}".ljust(8, ' ')+f"| {missedScan['RL']}".ljust(8, ' ')+f"| {missedScan['RR']}".ljust(8, ' ')+f"| {missedScan['FR']}".ljust(8, ' ')+f"| {missedScan['FL']}".ljust(8, ' ')+f"| {missedScan['FC']}".ljust(8, ' ')+f"| {missedScan['RC']}".ljust(8, ' ')+f"| {missedScan['BL']}".ljust(8, ' ')+f"| {missedScan['BR']}".ljust(8, ' ')+f"| {latchedScan['RL']}".ljust(8, ' ')+f"| {latchedScan['RR']}".ljust(8, ' ')+f"| {latchedScan['FR']}".ljust(8, ' ')+f"| {latchedScan['FL']}".ljust(8, ' ')+f"| {latchedScan['FC']}".ljust(8, ' ')+f"| {latchedScan['RC']}".ljust(8, ' ')+f"| {latchedScan['BL']}".ljust(8, ' ')+f"| {latchedScan['BR']}".ljust(8, ' ')+"\n")
    inputFiles[5].write(f" {logversion}\n")
    
    if resimRate != -1 :slownessAvgResimTime.append(resimRate)

# html mining data update
    row_data=[cnt,logversion,session,sessionYield,entrymode,fileexecution,blc,sessionTime,jobStatus,resimComp,totalLogs,logsRan,logNotRan,badLogs,logCrashed,resimTime,resimRate,'NA',badLogList,currentInput]
    html_mining_data.append(row_data)
    html_SlowResimTime.append(resimRate)


##############################################################
#
# function to mine KPI data to capture insights
#
##############################################################        
def kpi_mining(raw,inputFiles,cnt):
    #kpiLogDetails={'RL' : 'NA','RR' : 'NA','FR' : 'NA','FL' : 'NA','FC' : 'NA','RC' : 'NA','BL' : 'NA','BR' : 'NA'}
    kpiLogDetails={}
    for message in raw:
        message=message.split('\n')[0]
        if kpi_new_log in message:
            sensor=message.split(':')[-1].split('_')[-1]
            log=message.split(':')[-1].split(f"_{sensor}")
            if len(log) >= 3:log=f"{log[0]}_{sensor}{log[1]}"
            else: log=log[0]
            if log not in kpiLogDetails: kpiLogDetails[log]={'RL' : 'NA','RR' : 'NA','FR' : 'NA','FL' : 'NA','FC' : 'NA','RC' : 'NA','BL' : 'NA','BR' : 'NA'}
            kpiLogDetails[log][sensor]=[]     
        elif kpi_accuracy_w_cdc in message:
            kpiLogDetails[log][sensor].append(float(message.split(kpi_accuracy_w_cdc)[-1]))
            print(f"{sensor} : {kpiLogDetails[log][sensor]}---")
        elif kpi_accuracy_wo_cdc in message:
            kpiLogDetails[log][sensor].append(float(message.split(kpi_accuracy_wo_cdc)[-1]))
        elif kpi_scan_w_CDC in message:
            kpiLogDetails[log][sensor].append(float(message.split(kpi_scan_w_CDC)[-1]))
        elif kpi_scan_w_range in message:
            kpiLogDetails[log][sensor].append(float(message.split(kpi_scan_w_range)[-1]))
        elif mileageYield in message:
            info=message.split('\n')[0].split(':')
            sensor = info[0].split(mileageYield)[-1]
            milYield = float(info[1].split('%')[0])
            mil=info[1].split('=')
            vehMil = float(mil[1].split('km')[0])
            simMil = mil[2].split('km')[0]
            #print(f"{info}\nsensor:{sensor}\nmileageYield:{milYield}\nvehmil:{vehMil}\nsimmil:{simMil}")
            mileageData[sensor]=[milYield, vehMil, simMil]

    for log,value in kpiLogDetails.items():
        inputFiles[1].write(f"{log}.mf4 ")
        print(log, value)
        for sensor,yields in value.items():
            if yields == 'NA' :yields=['NA','NA','NA','NA']
            if len(yields) == 0:yields=[-10,-10,-10,-10]
            for val in yields:
                inputFiles[1].write(f"| {val} ")
        inputFiles[1].write("\n")
    #print(mileageData)        
    #print(kpiLogDetails)


##############################################################
#
# function to create xlsx file and summarize data
#
##############################################################        
def create_xlsx(path):
    global html_badLog
    row=line=logsTotal=logsBad=sessionCrashed=logsCrashed=0
    logsResimulated=logsNotResimulated=0
    logsBusspecId=currLine=sessionTimeout=logsTimeout=sessionOOM=logsOOM=0
    totalLogsDuration= 0
    sessionFCalib=sessionPCalib=logsPCalib=logsFCalib=sessionMdf=logsMdf=0
    sessionNoUdp=logsNoUdp=0
    versionDetails=[]
    versionCaptured=False
    scansessioncnt=scanlogcnt=0
    logVersion=""

# reading data for worksheets
    file = open(path+'.txt','r')
    all_details = file.readlines()
    file.close()
    file = open(f"{path}_aborted.txt",'r')
    abort_details = file.readlines()
    file.close()
    file = open(f"{path}_bad.txt",'r')
    bad_details = file.readlines()
    file.close()
    file = open(f"{path}_bad_session.txt",'r')
    badSession_details = file.readlines()
    file.close()
    file = open(f"{path}_scan_session.txt",'r')
    scan_details = file.readlines()
    file.close()
    file = open(f"{path}_scanLog_session.txt",'r')
    scanLog_details = file.readlines()
    file.close()
    file = open(f"{path}_kpi_session.txt",'r')
    kpi_details = file.readlines()
    file.close()
    file = open(f"{path}_kpiLog_session.txt",'r')
    kpiLog_details = file.readlines()
    file.close()
    

# writing data to xlsx file
    content=["Logs_Version", 'Session', 'Job_Status', 'Session_Yield', 'SIL Mode', 'File Mode', 'Bad Log Check', 'Logs_Total', 'Logs_Duration', 'Logs_Executed', 'Logs_NotExecuted', 'Logs_Bag', 'Log_Crashed', 'ReSim_Status', 'Missing_BusSpecId', 'TotalTime_ReSim', 'ResimTime_Profiling', 'ResimMemory_Profiling', 'Session_Details', 'BadLogs_Encountered ']
    content_scan=[]
    content_scanLog=[]

    workbook = xlsxwriter.Workbook(f'{path}.xlsx')
    header_prop = workbook.add_format({'bold': True, 'bg_color': '#DAEEF3', 'align': 'center', 'border' : 2})
    content_prop = workbook.add_format({'align': 'center','border' : 1})
    logsCrashed = logsTimeout = 0

    yieldSheet = workbook.add_worksheet("Yield")
    minimgDetailsSheet = workbook.add_worksheet("Mining_details")
    versionDetailsSheet = workbook.add_worksheet("Software_Versions")
    versionDetailsSheet.write_row(row,0,['Application','Version'],header_prop)
    
    minimgDetailsSheet.write_row(row,0,content,header_prop)
    for detail in all_details:
        line+=1
        if '********************' not in detail and versionCaptured == False:
            if detail.strip(): versionDetails.append(detail)  # skip blank line left after version block
        elif versionCaptured == False:
            versionCaptured = True
            versionDetailsSheet.write_row(currLine,0,['Application','Version'],header_prop)
            yieldSheet .write_row(currLine,13,['Application','Version'],header_prop)
            for info in versionDetails:
                currLine+=1
                data = info.split(':')
                versionDetailsSheet.write_row(currLine,0,data,content_prop)
                yieldSheet .write_row(currLine,13,data,content_prop)
            currLine=line+3
        if line >= currLine and versionCaptured == True:
            row+=1
            content=detail.split('|')
            #print(content)
            if logVersion == "" : logVersion=content[0]
            content[3]=float(content[3])
            
            content[7]=int(content[7])
            content[8]=int(content[8])
            content[9]=int(content[9])
            content[10]=int(content[10])
            content[11]=int(content[11])
            content[15]=float(content[15])
            content[16]=float(content[16])


            # Normalize row counters so downstream percentages do not go negative.
            if content[9] > content[7]:
                content[9] = content[7]
            if content[10] < 0:
                content[10] = 0
            if content[11] < 0:
                content[11] = 0
            if content[11] > content[10]:
                content[11] = content[10]
            failed_logs = max(content[10] - content[11], 0)

            logsTotal += content[7]
            logsResimulated += content[9]
            logsNotResimulated += content[10]
            logsBad += content[11]
            totalLogsDuration += content[8]

            #print(f"'{content[1]}'", len(content[1]))
            if content[2] == ' Partial ' and content[12] !=  ' False  ':
                sessionCrashed+=1
                logsCrashed += failed_logs
            # A session can crash mid-run (Log_Crashed set) and still finish Job_Status=Done once
            # crash-recovery retries the remaining logs; the crashed log itself is never re-run
            # (retry resumes from the next log), so it must still count as segmentation-impacted -
            # otherwise it silently disappears from the Yield sheet (was 0).
            elif content[2].strip() == 'Done' and content[12].strip() != 'False':
                sessionCrashed += 1
                logsCrashed += failed_logs if failed_logs > 0 else 1
            if content[2] == ' TimeOut ':
                sessionTimeout+=1
                logsTimeout += failed_logs
            if content[2] == ' OOM     ':
                sessionOOM+=1
                if content[13] ==  ' Running':
                    if content[12] == ' False  ': logsOOM += failed_logs
                    else: logsCrashed += failed_logs
                else:
                    sessionPCalib+=1
                    logsPCalib += failed_logs
            if content[2] == ' CALIB_P ' or content[2] == ' CALIB_F ' :
                if failed_logs == 0:
                    print(f"***{content[13]}******{content[14]}******{content[15]}***")
                    if content[13] != ' Done  ' or content[14] != ' Done  ' or content[15] != ' Done ' or content[16] != ' Done ' or content[17] != ' Done ' or content[18] != ' Done ': content[2] = 'Partial'
                    else: content[2]='Done'

                elif failed_logs == content[10]:
                    content[2] = ' CALIB_F '
                    sessionFCalib+=1
                    logsFCalib += failed_logs
                
                elif failed_logs != content[10]:
                    content[2] = ' CALIB_P '
                    sessionPCalib+=1
                    logsPCalib += failed_logs

            if content[2] == ' MDF     ':
                sessionMdf+=1
                logsMdf += failed_logs
            if content[2] == ' NO_UDP  ':
                sessionNoUdp+=1
                logsNoUdp += failed_logs
            if content[14]!=' None   ':
                logsBusspecId+=content[11]
                logsBad-=content[11]
            minimgDetailsSheet.write_row(row,0,content,content_prop)

    if len(abort_details)>=1:
        row=0
        crashDetailsSheet = workbook.add_worksheet("Crashed_Session")
        crashDetailsSheet.write_row(row,0,['Jobout Path','Crashed Log Name'],header_prop)
        current_job = ""
        for detail in abort_details:
            detail = detail.strip()
            if '|' in detail:
                row+=1
                job_p, log = detail.split('|')
                if job_p != "CONTINUE":
                    current_job = job_p
                    crashDetailsSheet.write_row(row,0,[current_job, log],content_prop)
                else:
                    parent_job = os.path.dirname(current_job)
                    crashDetailsSheet.write_row(row,0,[f"  (cont) {parent_job}", log],content_prop)

    if len(badSession_details)>=1:
        row=0
        badLogDetailsSheet = workbook.add_worksheet("BadLog_Details")
        badLogDetailsSheet.write_row(row,0,['Session encountered Bad Logs'],header_prop)
        for detail in badSession_details:
            row+=1
            badLogDetailsSheet.write_row(row,0,[detail],content_prop)

        row+=5
        badLogDetailsSheet.write_row(row,0,['Bad Logs'],header_prop)
        for detail in bad_details:
            row+=1
            badLogDetailsSheet.write_row(row,0,[detail],content_prop)

    if len(scan_details)>=1:
       row=0
       scanDetailsSheet = workbook.add_worksheet("Session_ScanDrop_Details")
       scanDetailsSheet.write_row(row,0,[' S.No. ', ' Log Version ', ' Session ', 'Curr Session', 'Drop_Total ', ' Drop-Scan_RL ',' Drop-Scan_RR ',' Drop-Scan_FR ',' Drop-Scan_FL ',' Drop-Scan_FC ',' Drop-Scan_RC ',' Drop-Scan_BL ',' Drop-Scan_BR ', ' Latched-Scan_RL ',' Latched-Scan_RR ',' Latched-Scan_FR ',' Latched-Scan_FL ',' Latched-Scan_FC ',' Latched-Scan_RC ',' Latched-Scan_BL ',' Latched-Scan_BR '],header_prop)
       for sessioninfo in scan_details:
           row+=1
           content=sessioninfo.split('|')
           final_data=[row]
           for val in content[3:]:
               if 'NA' not in val: index = content.index(val); content[index] = int(content[index])
           final_data += content
           scanDetailsSheet.write_row(row,0,final_data,content_prop)
           

    if len(scanLog_details)>=1:
        row=0
        scanLogDetailsSheet = workbook.add_worksheet("Log_ScanDrop_Details")
        scanLogDetailsSheet.write_row(row,0,[' S.No. ','Log Name',' Drop_Total ', ' Drop-Scan_RL ',' Drop-Scan_RR ',' Drop-Scan_FR ',' Drop-Scan_FL ',' Drop-Scan_FC ',' Drop-Scan_RC ',' Drop-Scan_BL ',' Drop-Scan_BR ', ' Latched-Scan_RL ',' Latched-Scan_RR ',' Latched-Scan_FR ',' Latched-Scan_FL ',' Latched-Scan_FC ',' Latched-Scan_RC ',' Latched-Scan_BL ',' Latched-Scan_BR '],header_prop)
        for loginfo in scanLog_details:
            loginfo=loginfo.split('\n')[0]
            if '.mf4' in loginfo or '.MF4' in loginfo:
                row+=1
                content=loginfo.split('|')
                for val in content[1:]:
                    if 'NA' not in val: index = content.index(val); content[index] = int(content[index])
                final_data=[row,content[0]]
                for val in content[9:]:final_data.append(val)
                for val in content[1:9]:final_data.append(val)
                #print(final_data)
                scanLogDetailsSheet.write_row(row,0,final_data,content_prop)
            else:row+=1

    if len(kpi_details)>=1:
       row=0
       kpiDetailsSheet = workbook.add_worksheet("Session_KPI_Details")
       kpiDetailsSheet.write_row(row,0,[' S No ', ' Log Version ', ' Session ', ' Latched-Scan_RL ',' Latched-Scan_RR ',' Latched-Scan_FR ',' Latched-Scan_FL ',' Latched-Scan_FC ',' Latched-Scan_RC ',' Latched-Scan_BL ',' Latched-Scan_BR ',' Missed-Scan_RL ',' Missed-Scan_RR ',' Missed-Scan_FR ',' Missed-Scan_FL ',' Missed-Scan_FC ',' Missed-Scan_RC ',' Missed-Scan_BL ',' Missed-Scan_BR '],header_prop)
       for sessioninfo in scan_details:
           row+=1
           kpiDetailsSheet.write_row(row,0,sessioninfo.split('|'),content_prop)

    if len(kpiLog_details)>=1:
       row=0
       kpiLogDetailsSheet = workbook.add_worksheet("Log_KPI_Details")
       kpiLogDetailsSheet.merge_range("A1:A2",' Log Name ',header_prop)
       kpiLogDetailsSheet.merge_range("B1:E1",' Sensor RL ( value : -10 :- no data  | NA:- Not Applicable) ',header_prop)
       kpiLogDetailsSheet.merge_range("F1:I1",' Sensor RR ',header_prop)
       kpiLogDetailsSheet.merge_range("J1:M1",' Sensor FR ',header_prop)
       kpiLogDetailsSheet.merge_range("N1:Q1",' Sensor FL ',header_prop)
       kpiLogDetailsSheet.merge_range("R1:U1",' Sensor FC ',header_prop)
       kpiLogDetailsSheet.merge_range("V1:Y1",' Sensor RC ',header_prop)
       kpiLogDetailsSheet.merge_range("Z1:AC1",' Sensor BL ',header_prop)
       kpiLogDetailsSheet.merge_range("AD1:AG1",' Sensor BR ',header_prop)
       row+=1
       kpiLogDetailsSheet.write_row(row,1,[' Accuracy (%)',' Accuracy Exclude CDC (%) ', ' Scan with CDC Saturation (%) ',' Scan with Range Saturation (%) ',' Accuracy (%)',' Accuracy Exclude CDC (%) ', ' Scan with CDC Saturation (%) ',' Scan with Range Saturation (%) '],header_prop)
       kpiLogDetailsSheet.write_row(row,9,[' Accuracy (%)',' Accuracy Exclude CDC (%) ', ' Scan with CDC Saturation (%) ',' Scan with Range Saturation (%) ',' Accuracy (%)',' Accuracy Exclude CDC (%) ', ' Scan with CDC Saturation (%) ',' Scan with Range Saturation (%) '],header_prop)
       kpiLogDetailsSheet.write_row(row,17,[' Accuracy (%)',' Accuracy Exclude CDC (%) ', ' Scan with CDC Saturation (%) ',' Scan with Range Saturation (%) ',' Accuracy (%)',' Accuracy Exclude CDC (%) ', ' Scan with CDC Saturation (%) ',' Scan with Range Saturation (%) '],header_prop)
       kpiLogDetailsSheet.write_row(row,25,[' Accuracy (%)',' Accuracy Exclude CDC (%) ', ' Scan with CDC Saturation (%) ',' Scan with Range Saturation (%) ',' Accuracy (%)',' Accuracy Exclude CDC (%) ', ' Scan with CDC Saturation (%) ',' Scan with Range Saturation (%) '],header_prop)
       #kpiLogDetailsSheet.write_row([' Latched-Scan_RL ',' Latched-Scan_RR ',' Latched-Scan_FR ',' Latched-Scan_FL ',' Latched-Scan_FC ',' Latched-Scan_RC ',' Latched-Scan_BL ',' Latched-Scan_BR ',' Missed-Scan_RL ',' Missed-Scan_RR ',' Missed-Scan_FR ',' Missed-Scan_FL ',' Missed-Scan_FC ',' Missed-Scan_RC ',' Missed-Scan_BL ',' Missed-Scan_BR '],header_prop)
       for loginfo in kpiLog_details:
           row+=1
           content=loginfo.split('|')
           for val in content[1:]:
               if 'NA' not in val: index = content.index(val); content[index] = float(content[index])
           kpiLogDetailsSheet.write_row(row,0,content,content_prop)

############### Chart plotting for fields

#creating Pie chart for Pass-fail yield
    logsResimulated = min(max(logsResimulated, 0), logsTotal)
    logsNotResimulated = max(logsTotal - logsResimulated, 0)
    logsCrashed = max(logsCrashed, 0)
    logsBad = max(logsBad, 0)
    logsBusspecId = max(logsBusspecId, 0)
    logsTimeout = max(logsTimeout, 0)
    logsOOM = max(logsOOM, 0)
    logsPCalib = max(logsPCalib, 0)
    logsFCalib = max(logsFCalib, 0)
    logsMdf = max(logsMdf, 0)
    logsNoUdp = max(logsNoUdp, 0)
    logs_unknown = max(logsTotal - (logsResimulated + logsCrashed + logsBad + logsBusspecId + logsTimeout + logsOOM + logsPCalib + logsFCalib + logsMdf + logsNoUdp), 0)
    yieldSheet.merge_range("E1:L1",'Logs Version', header_prop)
    yieldSheet.merge_range("E2:L2",logVersion, content_prop)

    yieldSheet.merge_range("F20:H20",'AVG-Slowness Factor', header_prop)
    yieldSheet.write_row(21,5,["Tool","Timing_Profile","Mem_Profile"], header_prop)
    yieldSheet.write_row(22,5,["Resim","NA","NA"], content_prop)
    yieldSheet.write_row(23,5,["Video","NA","NA"], content_prop)   # Video profiling not collected yet
    if len(slownessAvgResimTime) > 0: yieldSheet.write_row(22,6,[round(sum(slownessAvgResimTime)/len(slownessAvgResimTime),2)], content_prop)
    if len(slownessAvgResimMem) > 0: yieldSheet.write_row(22,7,[round(sum(slownessAvgResimMem)/len(slownessAvgResimMem),2)], content_prop)


    
    yieldSheet.write_row(0,0,['Factor','Yield', 'Logs Count'], header_prop)
    yieldSheet.write_row(1,0,['Total', '-',logsTotal], content_prop)
    yieldSheet.write_row(2,0,['Resimulated', round(logsResimulated*100/logsTotal,2),logsResimulated], content_prop)
    yieldSheet.write_row(3,0,['Segmentation impact', round(logsCrashed*100/logsTotal,2),logsCrashed], content_prop)
    yieldSheet.write_row(4,0,['Bad Log Quality Impact', round(logsBad*100/logsTotal,2),logsBad], content_prop)
    yieldSheet.write_row(5,0,['BusSpec Id Impact', round(logsBusspecId*100/logsTotal,2),logsBusspecId], content_prop)
    yieldSheet.write_row(6,0,['Timeout impact', round(logsTimeout*100/logsTotal,2),logsTimeout], content_prop)
    yieldSheet.write_row(7,0,['OOM-Killed Impact', round(logsOOM*100/logsTotal,2),logsOOM], content_prop)
    yieldSheet.write_row(8,0,['Partial Calib Impact', round(logsPCalib*100/logsTotal,2),logsPCalib], content_prop)
    yieldSheet.write_row(9,0,['No Calib Impact', round(logsFCalib*100/logsTotal,2),logsFCalib], content_prop)
    yieldSheet.write_row(10,0,['No UDP Data Impact', round(logsNoUdp*100/logsTotal,2),logsNoUdp], content_prop)
    yieldSheet.write_row(11,0,['MDF Impact', round(logsMdf*100/logsTotal,2),logsMdf], content_prop)
    yieldSheet.write_row(12,0,['Unknown Impact', round(logs_unknown*100/logsTotal,2),logs_unknown], content_prop)
    yieldSheet.write_row(13,0,['Not_Resimulated_Overall', round(logsNotResimulated*100/logsTotal,2),logsNotResimulated], content_prop)

    yieldSheet.write_row(15,0,['Occurance','Session Count'], header_prop)
    yieldSheet.write_row(16,0,['Segmentation Fault',sessionCrashed ], content_prop)
    yieldSheet.write_row(17,0,['Timeout',sessionTimeout ], content_prop)
    yieldSheet.write_row(18,0,['OOM Killed',sessionOOM ], content_prop)
    yieldSheet.write_row(19,0,['Partial Calib',sessionPCalib ], content_prop)
    yieldSheet.write_row(20,0,['No Calib',sessionFCalib ], content_prop)
    yieldSheet.write_row(21,0,['No UDP Data',sessionNoUdp ], content_prop)
    yieldSheet.write_row(22,0,['MDF error',sessionMdf ], content_prop)
    yieldSheet.write_row(24,0,['Total Logs Duration (sec)',totalLogsDuration ], content_prop)

    chart1 = workbook.add_chart({'type': 'pie'})
    chart1.set_title({'name': 'ReSim_Execution_Yield (%)'})
    chart1.add_series({ 
    'name':       'Overall_JoboutMinning_Summary', 
    'categories': ['Yield', 2, 0, 12, 0],
    'values':     ['Yield', 2, 1, 12, 1],
    'points': [ {'fill': {'color': '#5cd390'}}, {'fill': {'color': '#ffa191'}}, {'fill': {'color': '#ffd700'}}, {'fill': {'color': '#f0e4d7'}}, {'fill': {'color': '#b6a9d6'}}, {'fill': {'color': '#cc2d39'}}, {'fill': {'color': '#0099ff'}}, {'fill': {'color': '#0066cc'}}, {'fill': {'color': '#ff6600'}}, {'fill': {'color': '#000000'}}, {'fill': {'color': '#aaaaaa'}}],
    'data_labels': {'percentage':True},
    })

    chart1.set_style(10)
    yieldSheet.insert_chart('C4', chart1, {'x_offset': 145, 'y_offset': 4})


#creating Histogram chart for Scan Session Drop
    plotSheet = workbook.add_worksheet("Plot_Details")
    chart2 = workbook.add_chart({'type':'column'})
    chart2.set_x_axis({"name": "Session"})
    chart2.set_y_axis({"name": "Scan Drop"})
    chart2.add_series({
    'name'      : "Scan Drop/session",
    'categories': ['Session_ScanDrop_Details',1,0,len(scan_details),0],
    'values'    : ['Session_ScanDrop_Details',1,4,len(scan_details),4]
    })

    chart2.set_style(11)
    plotSheet.insert_chart('B2', chart2, {'x_scale': 3, 'y_scale': 1})

#creating Histogram chart for Scan Log Drop
    chart3 = workbook.add_chart({'type':'column'})
    chart3.set_x_axis({"name": "Session"})
    chart3.set_y_axis({"name": "Scan Drop"})
    chart3.add_series({
    'name'      : "Scan Drop/Log",
    'categories': ['Log_ScanDrop_Details',1,0,len(scanLog_details),0],
    'values'    : ['Log_ScanDrop_Details',1,2,len(scanLog_details),2],
    'line'	: {'color':'red'}
    })

    chart3.set_style(12)
    plotSheet.insert_chart('B18', chart3, {'x_scale': 7, 'y_scale': 1})

#creating Histogram chart for ReSim Slowness
    chart3 = workbook.add_chart({'type':'column'})
    chart3.set_x_axis({"name": "Session"})
    chart3.set_y_axis({"name": "Slowness"})
    chart3.add_series({
    'name'      : "ReSim Slowness/Session",
    'categories': ['Mining_details',1,1,len(all_details),1],
    'values'    : ['Mining_details',1,16,len(all_details),16],
    'line'	: {'color':'red'}
    })

    chart3.set_style(12)
    plotSheet.insert_chart('B34', chart3, {'x_scale': 3, 'y_scale': 1})

    workbook.close()
    print('[INFO] : workbook created')
    
#************************************* section to upload data for HTML
#yeild section
    html_Metrix_values["Total"]=[logsTotal,len(jobouts)]
    html_Metrix_values["Resimulated"]=[logsResimulated,len(jobouts)-sessionCrashed]
    html_Metrix_values["Segmentation"]=[logsCrashed,sessionCrashed]
    html_Metrix_values["Bad Log"]=[logsBad,len(badSession_details)]
    html_Metrix_values["Bus-Spec Id"]=[logsBusspecId,"-"]
    html_Metrix_values["TimeOut"]=[logsTimeout,sessionTimeout]
    html_Metrix_values["Out Of Memory"]=[logsOOM,sessionOOM]
    html_Metrix_values["Partial Calib"]=[logsPCalib,sessionPCalib]
    html_Metrix_values["No Calib"]=[logsFCalib,sessionFCalib]
    html_Metrix_values["No UDP Data"]=[logsNoUdp,sessionNoUdp]
    html_Metrix_values["MDF"]=[logsMdf,sessionMdf]
    html_Metrix_values["Not Resimulated"]=[logsNotResimulated,"-"]

    #html_mudp_I_Metrix_values["Total"]=[logsTotal,"-"]
    #html_mudp_I_Metrix_values["Resimulated"]=[mudpILogsResimulated,"-"]
    #html_mudp_I_Metrix_values["TimeOut"]=[mudpILogsTimeout,"-"]
    #html_mudp_I_Metrix_values["Out Of Memory"]=[mudpILogsOOM,"-"]
    #html_mudp_I_Metrix_values["Not Resimulated"]=[mudpILogsNotResimulated,"-"]

    #html_mudp_O_Metrix_values["Total"]=[logsTotal,"-"]
    #html_mudp_O_Metrix_values["Resimulated"]=[mudpOLogsResimulated,"-"]
    #html_mudp_O_Metrix_values["TimeOut"]=[mudpOLogsTimeout,"-"]
    #html_mudp_O_Metrix_values["Out Of Memory"]=[mudpOLogsOOM,"-"]
    #html_mudp_O_Metrix_values["Not Resimulated"]=[mudpOLogsNotResimulated,"-"]

#crash details
    for detail in abort_details:
        html_crashLog.append(detail)

#badLog details
    for detail in bad_details:
        html_badLog.append(detail)
    
#******************************************************************************


##############################################################
#
# function to read data quality report
#
##############################################################
def DatMini(path):
    bad_files = f"{path}"+'_bad.txt'
    out_path = path.split(path.split('/')[-1])[0][:-1]

    #print(f"singularity exec {DQ_simg} /RUN_MUDP.sh {DQ_config} {bad_files} {out_path}")      
    #os.system(f"singularity exec {DQ_simg} /RUN_MUDP.sh {DQ_config} {bad_files} {out_path}")

    xmlsTags = ET.parse('./Data_Logging_Quality.xml').getroot()
    for child in xmlsTags:
        if child.tag == 'Overall_Log_Quality_Check_Summary':
            lines=child.text.split('\n')
            for line in lines[4:]:
                if '-------+-' in line: break;
                tmplist=[]
                for content in line.split(' '):
                    if content != '':tmplist.append(content)
                masterList.append([tmplist[1], tmplist[2], tmplist[3]])
                tmplist.clear()
    #print(masterList)


"""********************************************************************************* main section *********************************************************************************"""
##############################################################
#
# main function
# flow :
#       collect jobouts path
#       create required files
#       jobout mininfg operation
#       close open files
#
##############################################################
if __name__ == '__main__':
    print("[INFO] : minimg started")
    jobouts=collect_jobouts()# collecting jobouts path
    kpiReports=["detection_kpi_debug_CDC_MODE.txt"]
    #filepath=jobouts[0].split('PostProcessing')[-0]+'PostProcessing/'
    filepath=jobouts[0].split('jobout')[-0]+'output/'
    #filepath=jobouts[0].split('1/')[0]
    fileName=jobouts[0].split('_')[-2].split('/')[-1].split('\n')[0]
    path= filepath + fileName
    tmp_file, tmp_abort_file, tmp_bad_file, tmp_bad_session_file, scan_bad_session,scanLog_bad_session, kpi_session, kpiLog_session  = create_files(path)                           # creating required output files
    collect_versions(jobouts,tmp_file)
    write_initial(tmp_file)

    ########## Section for mining data file by file ***********
    cnt = 1
    for job in jobouts:                                                                                         # mining on each file
        job_full_path = job.split('\n')[0]
        print(f"[MINING-INFO] : Jobout Mining {cnt}/{len(jobouts)} -> {job_full_path}")
        job_file=open(job_full_path,'r')
        job_content=job_file.readlines()
        job_file.close()
        
        data_mining(job_content,[tmp_file, tmp_abort_file, tmp_bad_file, tmp_bad_session_file, scan_bad_session,scanLog_bad_session, kpi_session, kpiLog_session], cnt, job_full_path)
        cnt+=1
        
    close_files([tmp_file, tmp_abort_file, tmp_bad_file, tmp_bad_session_file, scan_bad_session,scanLog_bad_session])                                # closing required output files

    ########## Section for mining data file by file ***********
    '''cnt = 1
    for report in kpiReports:                                                                                         # mining on each file
        print(f"[MINING-INFO] : KPI Mining {cnt}/{len(kpiReports)} -> {report}")
        kpi_file=open(report.split('\n')[0],'r')
        kpi_content=kpi_file.readlines()
        kpi_file.close()

        kpi_mining(kpi_content,[kpi_session, kpiLog_session],cnt)
        cnt+=1'''
        
    close_files([kpi_session, kpiLog_session])
    ########## Section to create report files ******************
    #DatMini(path)
    create_xlsx(path)
    #with open(f'{path}.html','w') as html_file:
    #    html_file.write(get_html_data())
    delete_files(path)
    print("[INFO] : minimg completed")
    
