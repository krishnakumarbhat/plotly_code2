"""
Created on Friday Oct 23 2024
@author: d1cse7 (mandeep.singh1@aptiv.com)
@copyright: APTIV
"""

from input_data import *
jobout_list=sys.argv[1]
customer=sys.argv[2]
#jobout_list="C:/Users/d1cse7/Desktop/Work/Scripts/mining.txt"
#customer="stla_scale3"

##############################################################
#
# collect all the jobouts files present in minimg.txt
#
##############################################################
def collect_jobouts():
    print("[INFO] : Collecting Jobouts", end='\r')
    jobouts=[]
    tmp=[]
    file=open(jobout_list,'r')
    paths=file.readlines()
    file.close()

    for i in paths:
        num=i.split('_')[-1].split('.')[0]
        tmp.append(int(num))
        tmp.sort()
    for i in tmp:
        x=f'_{i}.'
        for path in paths:
            if x in path:
                jobouts.append(path)
                break
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
    tmp_strm = open(path+'_strm_mismatch.txt','w')
    tmp_blfDeletion = open(path+'_blf_deletion.txt','w')
    return tmp, tmp_abort, tmp_bad, tmp_bad_session, tmp_strm, tmp_blfDeletion


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
    file.write("******************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************\n")
    file.write(" Session".ljust(9, ' ')+"| Status".ljust(10, ' ')+"| yield".ljust(9, ' ')+"| Logs_T".ljust(9, ' ')+"| Logs_E".ljust(9, ' ')+"| Logs_N".ljust(9, ' ')+"| Logs_B".ljust(9, ' ')+"| Log_C".ljust(9, ' ')+"| ReSim".ljust(8,' ')+"| HTML".ljust(8,' ')+"| BORD".ljust(8,' ')+"| busSpec".ljust(9, ' ')+"| J_Time".ljust(9,' ')+"| R_Time".ljust(9,' ')+"| H_Time".ljust(9,' ')+"| B_Time".ljust(9,' ')+"| J_rate".ljust(9,' ')+"| R_rate".ljust(9,' ')+"| H_rate".ljust(9,' ')+"| B_rate".ljust(9,' ')+f"| Jobout : {jobout_list}".ljust(210,' ')+"| Bad Logs Encountered".ljust(70,' ')+'\n')
    file.write("*********|*********|********|********|********|********|********|********|*******|*******|*******|********|********|********|********|********|********|********|********|********|*****************************************************************************************************************************************************************************************************************|*************************************************************************************\n")


##############################################################
#
# function to collect resim and pipelined tools version
#
##############################################################
def collect_versions(job, file):
    version_details={}
    job_file=open(job.split('\n')[0],'r')
    raw=job_file.readlines()
    job_file.close()
    for message in raw:
        if fw_version in message and 'fw' not in version_details.keys():
            version_details["SRR_SIL_RESIM"]=message.split(fw_version)[-1].split('\t')[0]
        elif sensor_version in message and 'SENSOR_SRR6P' not in version_details.keys() and "SRR6P" in message:
            version_details["SENSOR_SRR6P"]=message.split(sensor_version)[-1].split('\n')[0]
        elif sensor_version in message and 'SENSOR_FLR4' not in version_details.keys() and "FLR4" in message:
            version_details["SENSOR_FLR4"]=message.split(sensor_version)[-1].split('\n')[0]
        elif sensor_version in message and 'SENSOR_FLR4P' not in version_details.keys() and "FLR4P" in message:
            version_details["SENSOR_FLR4P"]=message.split(sensor_version)[-1].split('\n')[0]
        elif sensor_version in message:
            version_details["SENSOR"]=message.split(sensor_version)[-1].split('\n')[0]
        elif bord_version in message and 'bord' not in version_details.keys():
            version_details["BORDNET"]=message.split(bord_version)[-1].split('\t')[0]
        elif html_version in message and 'html' not in version_details.keys():
            version_details["HTML"]=message.split(html_version)[-1].split('\t')[0]
    for key, value in version_details.items():
        file.write(f'{key}\t: {value}\n')
    file.write('\n\n')


##############################################################
#
# function to collect session
#
##############################################################
def get_session(currentInput):
    session = "Customer support not added"
    if customer == "stla_scale1" :
        session = currentInput.split('/')[-2]
    elif customer == "stla_scale3":
        session = currentInput.split('/')[-3].split('_')[-1]
    elif customer == "dc":
        session = currentInput.split('/')[-1].split('_')[-2]
    elif customer == "honda" :
        log = currentInput.split('/')[-1]
        session = currentInput.split('Honda-Input')[-1].split(log)[0]
    elif customer == "traton" :
        session = currentInput.split('/')[-3]
    return session


##############################################################
#
# function to mine data to capture meaningful information
#
##############################################################
def data_mining(raw,inputFiles):
    ############################################ alert flags
    resimComp=False
    bordComp=False
    htmlComp=False
    timeoutAlert=False
    oomAlert=False
    calibAlert = False
    mdfAlert = False
    jobComp=False
    busSpecid=None
    busSpecAlert=False
    logCrashed=False

    ############################################ initial init for final print variables
    jobStatus="In Run"
    previousLog = ""
    currentInput=""
    totalLogs=0
    logsRan=0
    badLogs=0
    badLogList=[]
    busSpecLogs=0
    logTime=20
    jobTime=0
    resimTime=0
    bordTime=0
    htmlTime=0
    jobRate=-1
    resimRate=-1
    htmlRate=-1
    bordRate=-1
    prevSession=0
    

    ############################################ data mining
    for message in raw:
        if logDuration in message :
            logTime=int(message.split(logDuration)[-1].split(' ')[0])
        elif inputExecuted in message :
            currentInput=message.split(inputExecuted)[-1].split('\n')[0]
            totalLogs = int(currentInput.split()[-1])
            currSession=get_session(currentInput)
            if currSession != prevSession:
                session=currSession
                prevSession=currSession
        elif log1 in message or log2 in message or log3 in message or log4 in message:
            previousLog = message
            currLog = previousLog.split('\n')[0]
            if log2 in message or log4 in message:
                logsRan+=1
        elif busSpecId in message:
            busSpecid = message.split(busSpecId)[1].split('\n')[0]
            busSpecLogs += 1
            busSpecAlert = True
        elif resim in message:
            resimTime = message.split(resim)[-1].split(' ')[0].split('\n')[0]
            resimComp = True
        elif html in message :
            htmlTime = message.split(html)[-1].split('s')[0].split('\n')[0]
            htmlComp=True
        elif bordnet in message :
            bordTime = message.split(bordnet)[-1].split('s')[0].split('\n')[0]
            bordComp=True
        elif badLog in message:
            badLogs+=1
            if logsRan>=1:
                logsRan-=1
            logName=previousLog.split('<')[-1].split('>')[0].split('/')[-1]
            logNum=logName.split('_')[-1].split('.')[0]
            badLogList.append(logNum)
            if busSpecAlert == False:
                inputFiles[2].write(f'{logName}\n')
                inputFiles[3].write(f'{currentInput}\n')
        elif crash in message:
            if resimComp == False:
                logCrashed = previousLog.split('_')[-1].split('.')[0]
                if logsRan>=1:
                    logsRan-=1
            else: pass
            inputFiles[1].write(f'{currentInput} : {totalLogs-logsRan}\n')
        elif timeout in message:
            timeoutAlert = True
            jobStatus = "TimeOut"
        elif oomKilled in message:
            oomAlert = True
            jobStatus = "OOM"
        elif calibError in message:
            calibAlert = True
            jobStatus = "CALIB"
        elif mdfError in message:
            mdfAlert = True
            jobStatus = "MDF"
        elif jobCompleted in message :
            jobComp=True
            if timeoutAlert == False and oomAlert == False and calibAlert == False and mdfAlert == False:
                if resimComp==False or htmlComp==False or bordComp==False:
                    jobStatus = "Partial"
                else:
                    jobStatus = "Done"
            jobTime = message.split(jobCompleted)[-1].split('\n')[0]
        elif strmError in message :
            inputFiles[4].write(f"{currLog} : {message}")
        elif blfdeletion in message :
            inputFiles[5].write(f"{currLog} : {message}")

    if jobComp == True:
        if jobTime != 0:
            try:jobRate=round(float(jobTime)/(logsRan*logTime),2)
            except:print("jobRate Exception :",jobTime, logsRan, logTime)
        if resimTime != 0:
            try:resimRate=round(float(resimTime)/(logsRan*logTime),2)
            except:print("resimRate Exception :",jobTime, logsRan, logTime)
        if htmlTime != 0:
            try:htmlRate=round(float(htmlTime)/(logsRan*logTime),2)
            except:print("htmlRate Exception :",jobTime, logsRan, logTime)
        if bordTime != 0:
            try:bordRate=round(float(bordTime)/(logsRan*logTime),2)
            except:print("bordRate Exception :",jobTime, logsRan, logTime)

    logNotRan=totalLogs-logsRan
    try:sessionYield=round(float(logsRan*100/totalLogs),2)
    except:sessionYield=-1
    
    inputFiles[0].write(f" {session}".ljust(9, ' ')+f"| {jobStatus}".ljust(10, ' ')+f"| {sessionYield}".ljust(9, ' ')+f"| {totalLogs}".ljust(9, ' ')+f"| {logsRan}".ljust(9, ' ')+f"| {logNotRan}".ljust(9, ' ')+f"| {badLogs}".ljust(9, ' ')+f"| {logCrashed}".ljust(9, ' ')+f"| {resimComp}".ljust(8,' ')+f"| {htmlComp}".ljust(8,' ')+f"| {bordComp}".ljust(8,' ')+f"| {busSpecid}".ljust(9,' ')+f"| {jobTime}".ljust(9,' ')+f"| {resimTime}".ljust(9,' ')+f"| {htmlTime}".ljust(9,' ')+f"| {bordTime}".ljust(9,' ')+f"| {jobRate}".ljust(9,' ')+f"| {resimRate}".ljust(9,' ')+f"| {htmlRate}".ljust(9,' ')+f"| {bordRate}".ljust(9,' ')+f"| {currentInput}".ljust(210,' ')+f"| {badLogList}".ljust(70,' ')+'\n')


##############################################################
#
# function to create xlsx file and summarize data
#
##############################################################        
def create_xlsx(path):
    row=line=logsTotal=logsBad=sessionCrashed=logsCrashed=0
    logsResimulated=logsNotResimulated=0
    logsBusspecId=currLine=sessionTimeout=logsTimeout=sessionOOM=logsOOM=0
    sessionCalib=logsCalib=sessionMdf=logsMdf=0
    versionDetails=[]
    versionCaptured=False

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

# writing data to xlsx file
    content=['Session', 'Job_Status', 'Session_Yield', 'Logs_Total', 'Logs_Executed', 'Logs_NotExecuted', 'Logs_Bag', 'Log_Crashed', 'ReSim_Status', 'HTML_Status', 'BORDNET_Status', 'Missing_BusSpecId', 'TotalTime_Job', 'TotalTime_ReSim', 'TotalTime_HTML', 'TotalTime_BORDNET', 'SlownessRate_Job', 'SlownessRate_ReSim', 'SlownessRate_HTML', 'SlownessRate_BORDNET', 'Session_Details', 'BadLogs_Encountered ']
    workbook = xlsxwriter.Workbook(f'{path}.xlsx')
    header_prop = workbook.add_format({'bold': True, 'bg_color': '#DAEEF3', 'align': 'center'})
    content_prop = workbook.add_format({'align': 'center'})
    logsCrashed = logsTimeout = 0

    yieldSheet = workbook.add_worksheet("Yield")
    minimgDetailsSheet = workbook.add_worksheet("Mining_details")
    versionDetailsSheet = workbook.add_worksheet("Software_Versions")
    versionDetailsSheet.write_row(row,0,['Application','Version'],header_prop)
    
    minimgDetailsSheet.write_row(row,0,content,header_prop)
    for detail in all_details:
        line+=1
        if '********************' not in detail and versionCaptured == False:
            versionDetails.append(detail)
        elif versionCaptured == False:
            versionCaptured = True
            versionDetailsSheet.write_row(currLine,0,['Application','Version'],header_prop)
            for info in versionDetails:
                currLine+=1
                data = info.split(':')
                versionDetailsSheet.write_row(currLine,0,data,content_prop)
            currLine=line+3
        if line >= currLine and versionCaptured == True:
            row+=1
            content=detail.split('|')
            content[2]=float(content[2])
            content[3]=int(content[3])
            content[4]=int(content[4])
            content[5]=int(content[5])
            content[6]=int(content[6])
            content[12]=float(content[12])
            content[13]=float(content[13])
            content[14]=float(content[14])
            content[15]=float(content[15])
            content[16]=float(content[16])
            content[17]=float(content[17])
            content[18]=float(content[18])
            content[19]=float(content[19])

            logsTotal+=content[3]
            logsResimulated+=content[4]
            logsNotResimulated+=content[5]
            logsBad+=content[6]

            #print(f"'{content[1]}'", len(content[1]))
            if 'Partial' in content[1] and 'False' not in content[7]:
                sessionCrashed+=1
                logsCrashed += content[5]-content[6]; print("inside partial and crash",logsCrashed); print(f"{line} : {detail}")
            elif 'TimeOut' in content[1]:
                sessionTimeout+=1
                logsTimeout+=(content[5]-content[6])
            elif 'OOM' in content[1]:
                sessionOOM+=1
                if 'False' in content[7]: logsOOM+=(content[5]-content[6])
                else: logsCrashed += content[5]-content[6]; print("inside OOM and crash",logsCrashed); print(f"{line} : {detail}")
            elif 'CALIB' in content[1]:
                sessionCalib+=1
                logsCalib+=(content[5]-content[6])
            elif 'MDF' in content[1]:
                sessionMdf+=1
                logsMdf+=(content[5]-content[6])    
            elif 'None' not in content[11]:
                logsBusspecId+=content[6]
                logsBad-=content[6]
            minimgDetailsSheet.write_row(row,0,content,content_prop)
            

    if len(abort_details)>=1:
        row=0
        crashDetailsSheet = workbook.add_worksheet("Crashed_Session")
        crashDetailsSheet.write_row(row,0,['Session encountered Segmentation Fault'],header_prop)
        for detail in abort_details:
            row+=1
            crashDetailsSheet.write_row(row,0,[detail],content_prop)

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

#creating Pie chart for Pass-fail yield
    yieldSheet.write_row(0,0,['Factor','Yield', 'Logs Count'], header_prop)
    yieldSheet.write_row(1,0,['Total', '-',logsTotal], content_prop)
    yieldSheet.write_row(2,0,['Resimulated', round(logsResimulated*100/logsTotal,2),logsResimulated], content_prop)
    yieldSheet.write_row(3,0,['Segmentation impact', round(logsCrashed*100/logsTotal,2),logsCrashed], content_prop)
    yieldSheet.write_row(4,0,['Bad Log Quality Impact', round(logsBad*100/logsTotal,2),logsBad], content_prop)
    yieldSheet.write_row(5,0,['BusSpec Id Impact', round(logsBusspecId*100/logsTotal,2),logsBusspecId], content_prop)
    yieldSheet.write_row(6,0,['Timeout impact', round(logsTimeout*100/logsTotal,2),logsTimeout], content_prop)
    yieldSheet.write_row(7,0,['OOM-Killed Impact', round(logsOOM*100/logsTotal,2),logsOOM], content_prop)
    yieldSheet.write_row(8,0,['No Calib Impact', round(logsCalib*100/logsTotal,2),logsCalib], content_prop)
    yieldSheet.write_row(9,0,['MDF Impact', round(logsMdf*100/logsTotal,2),logsMdf], content_prop)
    yieldSheet.write_row(10,0,['Not_Resimulated_Overall', round(logsNotResimulated*100/logsTotal,2),logsNotResimulated], content_prop)

    yieldSheet.write_row(15,0,['Occurance','Session Count'], header_prop)
    yieldSheet.write_row(16,0,['Segmentation Fault',sessionCrashed ], content_prop)
    yieldSheet.write_row(17,0,['Timeout',sessionTimeout ], content_prop)
    yieldSheet.write_row(18,0,['OOM Killed',sessionOOM ], content_prop)
    yieldSheet.write_row(19,0,['No Calib',sessionCalib ], content_prop)
    yieldSheet.write_row(20,0,['MDF error',sessionMdf ], content_prop)

    chart1 = workbook.add_chart({'type': 'pie'})
    chart1.set_title({'name': 'ReSim_Execution_Yield (%)'})
    chart1.add_series({ 
    'name':       'Overall_JoboutMinning_Summary', 
    'categories': ['Yield', 2, 0, 9, 0],
    'values':     ['Yield', 2, 1, 9, 1],
    'points': [ {'fill': {'color': '#5cd390'}}, {'fill': {'color': '#ffa191'}}, {'fill': {'color': '#ffd700'}}, {'fill': {'color': '#f0e4d7'}}, {'fill': {'color': '#b6a9d6'}}, {'fill': {'color': '#cc2d39'}}, {'fill': {'color': '#26081f'}}, {'fill': {'color': '#000000'}}],
    'data_labels': {'percentage':True},
    })

    chart1.set_style(10)
    yieldSheet.insert_chart('C2', chart1, {'x_offset': 128, 'y_offset': 4})
    workbook.close()
    print('[INFO] : workbook created')

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
    filepath=jobouts[0].split('jobout')[-0]+'output/'
    fileName=jobouts[0].split('_')[-2].split('/')[-1]
    path= filepath + fileName
    tmp_file, tmp_abort_file, tmp_bad_file, tmp_bad_session_file, tmp_strm, tmp_blfDeletion = create_files(path)                           # creating required output files
    collect_versions(jobouts[0],tmp_file)
    write_initial(tmp_file)

    for job in jobouts:                                                                                         # mining on each file
        job_file=open(job.split('\n')[0],'r')
        job_content=job_file.readlines()
        job_file.close()

        data_mining(job_content,[tmp_file, tmp_abort_file, tmp_bad_file, tmp_bad_session_file, tmp_strm, tmp_blfDeletion])
    close_files([tmp_file, tmp_abort_file, tmp_bad_file, tmp_bad_session_file, tmp_strm, tmp_blfDeletion])                                # closing required output files
    create_xlsx(path)
    delete_files(path)
    print("[INFO] : minimg completed")
