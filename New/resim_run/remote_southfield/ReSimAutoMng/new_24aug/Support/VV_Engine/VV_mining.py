from VV_staticdata import *


##############################################################
#
# collect all the jobouts files present in minimg.txt
#
##############################################################
def collect_jobouts():
    global sessionTotal
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
    sessionTotal=len(jobouts)
    print(f"[INFO] : Collected Jobouts for {sessionTotal} sessions")
    return jobouts


##############################################################
#
# create all file to capture scenerios
#
##############################################################
def createFiles(path):
    file=path+'.txt'
    tmp = open(file,'w')
    tmp.close()
    return file



##############################################################
#
# function to collect resim and pipelined tools version
#
##############################################################
def collect_versions(jobs):
    version_collected =[0,0,0,0,0,0,0,0,0,0,0,0]
    for job in jobs:
        with open(job.split('\n')[0],'r') as file:
            messages=file.readlines()
        for message in messages:
            message=message.split('\n')[0]
            if vv_sil_version in message and version_collected[0] == 0:
                versionDetails["VV SIL Engine"][1]='V'+message.split(vv_sil_version)[-1].split(' ')[0]
                version_collected[0] = 1
                
            elif sm2_version in message and version_collected[1] == 0:
                versionDetails["Sensor Model"]['Main']='V'+message.split(sm2_version)[-1]
                version_collected[1] = 1
            elif mesh_version in message and version_collected[2] == 0:
                versionDetails["Sensor Model"]['Mesh']=message.split(mesh_version)[-1]
                version_collected[2] = 1
                
            elif lm2_version in message and version_collected[3] == 0:
                versionDetails["Logic Model"]['Main']='V'+message.split(lm2_version)[-1]
                version_collected[3] = 1
            elif fw_sil_version in message and version_collected[4] == 0:
                chunks=message.split(fw_sil_version)[-1].split(' ')
                fwversion='V'
                for chunk in chunks:
                    try:int(chunk);fwversion+=f'{chunk}.'
                    except:pass
                versionDetails["Logic Model"]['SIL FW']=fwversion
                version_collected[4] = 1
            elif sensor_version in message and version_collected[5] == 0:
                versionDetails["Logic Model"]["Sensor Software Version"]='V'+message.split(sensor_version)[-1]
                version_collected[5] = 1
            elif dc_version in message and version_collected[6] == 0:
                versionDetails["Logic Model"]["DC Software Version"]='V'+message.split(dc_version)[-1]
                version_collected[6] = 1
            elif sensor in message and version_collected[7] == 0:
                versionDetails["Logic Model"]["Sensor Cal"]=message.split(sensor)[-1]
                version_collected[7] = 1
            elif binaryBuild in message and version_collected[8] == 0:
                versionDetails["Logic Model"]["Binary Build"]=message.split(binaryBuild)[-1]
                version_collected[8] = 1
            elif tracker_version in message and version_collected[9] == 0:
                versionDetails["Logic Model"]["Tracker Version"]='V'+message.split(tracker_version)[-1]
                version_collected[9] = 1
            elif rspp_version in message and version_collected[10] == 0:
                versionDetails["Logic Model"]["RSPP Version"]='V'+message.split(rspp_version)[-1]
                version_collected[10] = 1
            elif dcCofigFor in message and version_collected[11] == 0:
                versionDetails["Logic Model"]["Sub Variant"]=message.split(dcCofigFor)[-1]
                version_collected[11] = 1
        
    
##############################################################
#
# function to mine data to capture meaningful information
#
##############################################################
def dataMining(cnt,inFile,outFile):
    global features, sessionTimeout, sessionOOM, sessionCrash, sessionPass
    ############################################ alert flags
    #print(f"{cnt} : {inFile}->{outFile}")
    ############################################ alert flags
    jobComp=False
    smJobComp=False
    fmuunload=False
    conversionosi=False
    streamend=False

    ############################################ initial init for final print variables
    jobStatus="In Run"
    jobTime=0
    smTime=0
    smYeild=0
    feature={}

    runmode=None
    silmode=None
    sencal=None

    print(inFile)
    with open(inFile,'r') as file:
        messages=file.readlines()
    with open(outFile,'a') as file:
        for message in messages:
            message=message.split('\n')[0]
            if inputExecuted in message:
                currentInput=message.split(inputExecuted)[-1]
                scenerios=int(currentInput.split(',')[-1])
                session=currentInput.split(',')[0].split('/')[-1]
                session_path=currentInput.split(session)[0]
                try:features[session_path][0]=features[session_path][0]+scenerios
                except:features[session_path]=[scenerios,0,0,0]
            elif runMode in message: runmode=message.split(runMode)[-1]

            elif silMode in message: silmode=message.split(silMode)[-1]
            
            elif sensor in message: sencal=message.split(sensor)[-1]

            elif conversionOSI in message: conversionosi=message.split(conversionOSI)[-1]#;print(message)

            elif jobComplete in message and jobStatus != 'Crash': jobTime=message.split(jobComplete)[-1]; jobStatus="Done"

            elif resim in message: smTime=message.split(resim)[-1]

            elif timeout in message: jobStatus="Time-Out";sessionTimeout+=1

            elif oomKilled in message: jobStatus="OOM-Killed";sessionOOM+=1

            elif crash in message: jobStatus="Crash"; sessionCrash+=1;print(inFile)

            elif fmuUnload in message: fmuunload = True

            elif streamEnd in message: streamend = True

            elif vvComplete in message: print(message);smJobComp = True; smYeild=100; features[session_path][2]+=1; sessionPass+=1

            
        print(features[session_path][2])
        raw_data = f"{cnt}:{session}:{jobStatus}:{smYeild}:{runmode}:{silmode}:{smTime}:{jobTime}:{sencal}:{streamend}:{conversionosi}:{fmuunload}:{smJobComp}:{session_path}\n"
        file.write(raw_data)
 

##############################################################
#
# function to update features
#
##############################################################        
def updateFeatures():
    global features
    for feature in features:
        features[feature][3] = features[feature][0]-features[feature][2]
        features[feature][1] = round((features[feature][2]/features[feature][0])*100,2)


##############################################################
#
# function to create xlsx file and summarize data
#
##############################################################        
def create_xlsx(miningFile, path):
    #section to read mining data collected in .txt
    with open(miningFile,'r') as file:
        miningData=file.readlines()

    #section to write data to xlsx file
    
    workbook = xlsxwriter.Workbook(f'{path}.xlsx')
    header_prop = workbook.add_format({'bold': True, 'bg_color': '#DAEEF3', 'align': 'center', "border": 1})
    content_prop = workbook.add_format({'align': 'center',"border": 1})
    merge_prop = workbook.add_format({'align': 'center', "valign": "vcenter","border": 1, 'bg_color':"white"})

    # creating sheets
    yieldSheet = workbook.add_worksheet("Yield")
    featureSheet = workbook.add_worksheet("Feature_details")
    minimgDetailsSheet = workbook.add_worksheet("Mining_details")
    versionDetailsSheet = workbook.add_worksheet("Software_Versions")
    
    # writing headers to the sheet
    featureSheet.write_row(0,0,breifheader,header_prop)
    minimgDetailsSheet.write_row(0,0,detailheader,header_prop)
    versionDetailsSheet.write_row(0,0,['Module','Sub-Module','Version'],header_prop)

    #writing data to Yield Sheet
    sessionFail=sessionTotal-sessionPass
    yieldSheet.write_row(0,0,['Yield', 'Session Total', 'Session Completed', 'Session Failed'],header_prop)
    yieldSheet.write_row(1,0,[round((sessionPass/sessionTotal)*100,2), sessionTotal, sessionPass, sessionFail],header_prop)
    yieldSheet.write_row(3,0,['Factor', 'Yield %','Count'],header_prop)
    yieldSheet.write_row(4,0,['Session Completed', round((sessionPass/sessionTotal)*100,2),sessionPass],content_prop)
    yieldSheet.write_row(5,0,['Session Timeout', round((sessionTimeout/sessionTotal)*100,2),sessionTimeout],content_prop)
    yieldSheet.write_row(6,0,['Session Out Of Memory', round((sessionOOM/sessionTotal)*100,2),sessionOOM],content_prop)
    yieldSheet.write_row(7,0,['Session Segmentation', round((sessionCrash/sessionTotal)*100,2),sessionCrash],content_prop)

    chart1 = workbook.add_chart({'type': 'pie'})
    chart1.set_title({'name': 'Execution_Yield (%)'})
    chart1.add_series({ 
    'name':       'Overall_JoboutMinning_Summary', 
    'categories': ['Yield', 4, 0, 7, 0],
    'values':     ['Yield', 4, 1, 7, 1],
    'points': [ {'fill': {'color': '#5cd390'}},{'fill': {'color': '#b6a9d6'}},{'fill': {'color': '#cc2d39'}}, {'fill': {'color': '#ffa191'}}],
    'data_labels': {'percentage':True},
    })
    chart1.set_style(10)
    yieldSheet.insert_chart('C2', chart1, {'x_offset': 138, 'y_offset': 14})



    #writing data to Fetature_details Sheet
    cnt=1
    for feature in features:
        print(feature,features[feature][0],features[feature][1],features[feature][2],features[feature][3])
        featureSheet.write_row(cnt,0,[feature,features[feature][0],features[feature][1],features[feature][2],features[feature][3]], content_prop)
        cnt+=1

    #writing data to Mining_details Sheet
    for data in miningData:
        values=[]
        data=data.split('\n')[0]
        
        factors=data.split(':')
        for factor in factors:
            try:factor=int(factor)
            except:pass
            values.append(factor)
        minimgDetailsSheet.write_row(values[0],0,values,content_prop)

    #writing data to Version_details Sheet
    cnt=1
    for application in versionDetails:
        if application == "VV SIL Engine":
            versionDetailsSheet.write_row(1,0,[application,versionDetails[application][0],versionDetails[application][1]],content_prop)
            cnt+=1
        elif application == "Sensor Model":
            versionDetailsSheet.merge_range("A3:A4",application,merge_prop)
            for subapplication in versionDetails[application]:
                versionDetailsSheet.write_row(cnt,1,[subapplication,versionDetails[application][subapplication]],content_prop)
                cnt+=1
        elif application == "Logic Model":
            versionDetailsSheet.merge_range("A5:A13",application,merge_prop)
            for subapplication in versionDetails[application]:
                versionDetailsSheet.write_row(cnt,1,[subapplication,versionDetails[application][subapplication]],content_prop)
                cnt+=1
                
    workbook.close()


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

if __name__=='__main__':
    print("[INFO] : minimg started")
    jobouts=collect_jobouts()

    filepath=jobouts[0].split('jobout')[-0]+'output/'
    #filepath=jobouts[0].split('10483361_1.out')[0]
    fileName=jobouts[0].split('_')[-2].split('/')[-1]
    path= filepath + fileName
    #print("path : ",path)
    rawDataFile=createFiles(path)
    collect_versions(jobouts)
    
    ########## Section for mining data file by file ***********
    cnt=1
    for jobout in jobouts:
        if cnt < 2000:
            jobout=jobout.split('\n')[0]
            #print(f"[MINING-INFO] : Mining {cnt}/{len(jobouts)} -> {jobout}")
            dataMining(cnt,jobout,rawDataFile)
            cnt+=1

    updateFeatures()
    create_xlsx(rawDataFile, path)
    print(versionDetails)


