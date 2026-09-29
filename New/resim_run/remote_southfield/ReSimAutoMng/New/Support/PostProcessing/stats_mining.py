# importing element tree
# under the alias of ET
import xml.etree.ElementTree as ET
# Passing the path of the
# xml document to enable the
# parsing process
import pandas as pd
import sys
import os
from texttable import Texttable
import re

i = 0
blank_count = 0
filelist = []
sensor_list = ['REAR_LEFT_OverAll_Statistic', 'REAR_RIGHT_OverAll_Statistic', 'FRONT_RIGHT_OverAll_Statistic',
               'FRONT_LEFT_OverAll_Statistic']
sensor_name_list = ['RL', 'RR', 'FR', 'FL']
dist = None
cta = None
ced = None
lcda = None
df_final = pd.DataFrame(columns=['Dist_Travelled (kms)', 'Time_Duration (Sec)'])
sheets = ['DIST', 'CTA', 'CED', 'LCDA']

df_dict = {'RL': [dist, cta, ced, lcda], 'RR': [dist, cta, ced, lcda], 'FR': [dist, cta, ced, lcda],
           'FL': [dist, cta, ced, lcda]}
dfs_new = {}
df_error_msgs_list = []
t = Texttable()
print_flag = False

def dfs_tabs(writer, df_list, sheet_list, sensor):
    for dataframe, sheet in zip(df_list, sheet_list):
        dataframe.to_excel(writer, sheet_name=sheet + '_' + sensor, startrow=0, startcol=0, index=False)


def segregate_file(filelist, df_error_msgs_list):
    files =[]
    error_msg = "[ERROR] : File Not Exist"
    for line in filelist:
        if '*.xml' in line: df_error_msgs_list.append([line,error_msg])
        else :
            file = line.split(' ')
            for i in file:
                if not os.path.isfile(i): df_error_msgs_list.append([i,error_msg])
                else:
                    if i not in files: files.append(i)
                    else: print('repeat : ',line)
    return files

def stat_func(filelist):
    for i in range(len(filelist)):
        if print_flag:
            print(filelist[i])
        xml_file_path, xml_file_name = os.path.split(filelist[i])
        if print_flag:
            print('xml_file_path is: ', xml_file_path)
        if print_flag:
            print('xml_file_name is: ', xml_file_name)
        if os.path.isfile(filelist[i]):
            try:
                tree = ET.parse(filelist[i])
                #print(tree)
                root = tree.getroot()
                #print(root)
                for child in root.findall("./OVERALL_STATISTIC_REPORT/JSON_FILE_PATH"):
                    #print(child.tag, child.attrib)
                    json_file_path = child.text

                for sensor, sensor_name in zip(sensor_list, sensor_name_list):
                    if print_flag:
                        print('sensor is: ', sensor)
                    for child in root.findall("./OVERALL_STATISTIC_REPORT/" + sensor + "/Vehicle_Dynamics/Vehicle_Log"):
                        #print(child.tag, child.attrib)
                        keys_data = child.attrib.keys()
                        keys_list = list(keys_data)
                        if not isinstance(df_dict[sensor_name][0], pd.DataFrame):
                            keys_list.append('File_name')
                            keys_list.append('Logs_count')
                            #print('df_dist does not exist for sensor: ', sensor_name)
                            df_dict[sensor_name][0] = pd.DataFrame(columns=keys_list)

                        #print('keys data is: ', keys_data)

                        #print(child.tag, child.attrib)
                        dist_out = []
                        for key in keys_data:
                            dist_out.append(child.attrib[key])
                        if print_flag:
                            print('xml file name appending is: ', xml_file_name)
                        dist_out.append(xml_file_name)

                        log_count = len(root.findall("./STATISTIC_REPORT"))
                        dist_out.append(log_count)
                        #print('the length of dataframe is: ', len(df_dict[sensor_name][0].index))
                        #print('the length of columns of dataframe is: ', len(df_dict[sensor_name][0].columns))

                        df_dict[sensor_name][0].loc[len(df_dict[sensor_name][0].index)] = dist_out
                        #print('df_dict is: ', df_dict)
                    # for child in root.findall("./OVERALL_STATISTIC_REPORT/"+sensor+"/Feature_Function_Info"):
                    # rint(child.tag, child.attrib)
                    #    new_var = child
                    #    for child in new_var:
                    #        print(child.tag, child.attrib)

                    #    for child in new_var.findall("./CTA/Resim_Data"):
                    #        keys_data = child.attrib.keys()
                    #        if not isinstance(df_dict[sensor_name][1], pd.DataFrame):
                    #            print('df_cta does not exist')
                    #            df_dict[sensor_name][1] = pd.DataFrame(columns=keys_data)
                    #        print(child.tag, child.attrib)
                    #        cta_out = []
                    #        for key in keys_data:
                    #            cta_out.append(child.attrib[key])

                    #        print('sensor is: ', sensor)
                    #        print('cta_out is: ', cta_out)
                    #        print('length of dataframe is: ', len(df_dict[sensor_name][1]))
                    #        df_dict[sensor_name][1].loc[len(df_dict[sensor_name][1].index)] = cta_out

                    #    for child in new_var.findall("./CED/Resim_Data"):
                    #        keys_data = child.attrib.keys()
                    #        if not isinstance(df_dict[sensor_name][2], pd.DataFrame):
                    #            print('df_ced does not exist')
                    #           df_dict[sensor_name][2] = pd.DataFrame(columns=keys_data)
                    #        print(child.tag, child.attrib)
                    #        ced_out = []
                    #        for key in keys_data:
                    #            ced_out.append(child.attrib[key])

                    #        df_dict[sensor_name][2].loc[len(df_dict[sensor_name][2].index)] = ced_out

                    #    for child in new_var.findall("./LCDA/Resim_Data"):

                    #        keys_data = child.attrib.keys()
                    #        if not isinstance(df_dict[sensor_name][3], pd.DataFrame):
                    #            df_dict[sensor_name][3] = pd.DataFrame(columns=keys_data)
                    #        print(child.tag, child.attrib)
                    #        lcda_out = []
                    #        for key in keys_data:
                    #            lcda_out.append(child.attrib[key])

                    #        df_dict[sensor_name][3].loc[len(df_dict[sensor_name][3].index)] = lcda_out

                    # dfs = [df_dict[sensor_name][0], df_dict[sensor_name][1], df_dict[sensor_name][2], df_dict[sensor_name][3]]
                    # dfs = [df_dict[sensor_name][0]]
                    # dfs_tabs(writer, dfs, sheets, sensor_name)

            except Exception as error:
                exc_type, exc_obj, exc_tb = sys.exc_info()
                error_msg = "ERROR: " + str(error) + " FOUND IN LINE: " + str(exc_tb.tb_lineno)
                df_error_msgs_list.append([filelist[i], error_msg])
                print('\n      ## XML FILE PARSING ERROR :--> ')
                t.add_rows([['File_name', 'Error'], [filelist[i], error_msg]])
                print(t.draw())

        else:
            pass

    if print_flag:
        print('\n\n\ndf_dict RL is: ')
    df_error_msgs = pd.DataFrame(data=df_error_msgs_list, columns=['file_path', 'error_message'])
    if print_flag:
        print(df_dict['RL'][0])
        print('\n\n\n')
    #df_dict['RL'][0] = df_dict['RL'][0].assign(dist_km_rl=lambda x: (float(re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Dist_Travelled']).split(',')[1])[0])))
    #df_dict['RR'][0] = df_dict['RR'][0].assign(dist_km_rr=lambda x: (float(re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Dist_Travelled']).split(',')[1])[0])))
    #df_dict['FL'][0] = df_dict['FL'][0].assign(dist_km_fl=lambda x: (float(re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Dist_Travelled']).split(',')[1])[0])))
    #df_dict['FR'][0] = df_dict['FR'][0].assign(dist_km_fr=lambda x: (float(re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Dist_Travelled']).split(',')[1])[0])))

    df_dict['RL'][0]['dist_km_rl'] = ''

    if print_flag:
        print('df_dict RL -2st time is: \n\n')
        print(df_dict['RL'][0])
        print('\n\n\n')

    for i in range(len(df_dict['RL'][0])):
        if re.findall(r"[-+]?(?:\d*\.*\d+)", str(df_dict['RL'][0]['Dist_Travelled'][i]).split(',')[1]):
            df_dict['RL'][0]['dist_km_rl'][i] = (
                float(re.findall(r"[-+]?(?:\d*\.*\d+)", str(df_dict['RL'][0]['Dist_Travelled'][i]).split(',')[1])[0]))
            df_dict['RL'][0]['Dist_Travelled'][i] = df_dict['RL'][0]['dist_km_rl'][i]
        else:
            df_dict['RL'][0]['dist_km_rl'][i] = 0
            df_dict['RL'][0]['Dist_Travelled'][i] = str(df_dict['RL'][0]['Dist_Travelled'][i]).split(',')[1]

    df_dict['RR'][0]['dist_km_rr'] = ''
    for i in range(len(df_dict['RR'][0])):
        if re.findall(r"[-+]?(?:\d*\.*\d+)", str(df_dict['RR'][0]['Dist_Travelled'][i]).split(',')[1]):
            df_dict['RR'][0]['dist_km_rr'][i] = (
                float(re.findall(r"[-+]?(?:\d*\.*\d+)", str(df_dict['RR'][0]['Dist_Travelled'][i]).split(',')[1])[0]))
            df_dict['RR'][0]['Dist_Travelled'][i] = df_dict['RR'][0]['dist_km_rr'][i]
        else:
            df_dict['RR'][0]['dist_km_rr'][i] = 0
            df_dict['RR'][0]['Dist_Travelled'][i] = str(df_dict['RR'][0]['Dist_Travelled'][i]).split(',')[1]

    df_dict['FL'][0]['dist_km_fl'] = ''
    for i in range(len(df_dict['FL'][0])):
        if re.findall(r"[-+]?(?:\d*\.*\d+)", str(df_dict['FL'][0]['Dist_Travelled'][i]).split(',')[1]):
            df_dict['FL'][0]['dist_km_fl'][i] = (
                float(re.findall(r"[-+]?(?:\d*\.*\d+)", str(df_dict['FL'][0]['Dist_Travelled'][i]).split(',')[1])[0]))
            df_dict['FL'][0]['Dist_Travelled'][i] = df_dict['FL'][0]['dist_km_fl'][i]
        else:
            df_dict['FL'][0]['dist_km_fl'][i] = 0
            df_dict['FL'][0]['Dist_Travelled'][i] = str(df_dict['FL'][0]['Dist_Travelled'][i]).split(',')[1]

    df_dict['FR'][0]['dist_km_fr'] = ''
    for i in range(len(df_dict['FR'][0])):
        if re.findall(r"[-+]?(?:\d*\.*\d+)", str(df_dict['FR'][0]['Dist_Travelled'][i]).split(',')[1]):
            df_dict['FR'][0]['dist_km_fr'][i] = (
                float(re.findall(r"[-+]?(?:\d*\.*\d+)", str(df_dict['FR'][0]['Dist_Travelled'][i]).split(',')[1])[0]))
            df_dict['FR'][0]['Dist_Travelled'][i] = df_dict['FR'][0]['dist_km_fr'][i]
        else:
            df_dict['FR'][0]['dist_km_fr'][i] = 0
            df_dict['FR'][0]['Dist_Travelled'][i] = str(df_dict['FR'][0]['Dist_Travelled'][i]).split(',')[1]

    if print_flag:
        print('df_dict RL -1st time is: \n\n')
        print(df_dict['RL'][0])
        print('\n\n\n')

    df_var = pd.DataFrame(columns=['dist_km', 'dist_time'])
    df_var['dist_km'] = df_dict['RL'][0]['dist_km_rl']

    for i in range(len(df_dict['RL'][0])):
        df_var['dist_km'][i] = max(df_dict['RL'][0]['dist_km_rl'][i], df_dict['RR'][0]['dist_km_rr'][i],
                                             df_dict['FL'][0]['dist_km_fl'][i], df_dict['FR'][0]['dist_km_fr'][i])

    if print_flag:
        print('df_dict RL 1st time is: \n\n')
        print(df_dict['RL'][0])
        print('\n\n\n')

    #df_dict['RL'][0].drop('Dist_Travelled', axis=1, inplace=True)
    #df_dict['RR'][0].drop('Dist_Travelled', axis=1, inplace=True)
    #df_dict['FL'][0].drop('Dist_Travelled', axis=1, inplace=True)
    #df_dict['FR'][0].drop('Dist_Travelled', axis=1, inplace=True)

    df_dict['RL'][0].rename(columns={'Dist_Travelled': 'Dist_Travelled (kms)'}, inplace=True)
    df_dict['RR'][0].rename(columns={'Dist_Travelled': 'Dist_Travelled (kms)'}, inplace=True)
    df_dict['FL'][0].rename(columns={'Dist_Travelled': 'Dist_Travelled (kms)'}, inplace=True)
    df_dict['FR'][0].rename(columns={'Dist_Travelled': 'Dist_Travelled (kms)'}, inplace=True)

    if print_flag:
        print('df_dict RL 2nd time is: \n\n')
        print(df_dict['RL'][0])
        print('\n\n\n')

    #df_dict['RL'][0] = df_dict['RL'][0].assign(time_sec=lambda x: (((float(
    #    re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[0])[0]) * 60) + float(
    #    re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[1])[0]) * 60) + float(
    #    re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[2])[0])))
    #df_dict['RR'][0] = df_dict['RR'][0].assign(time_sec=lambda x: (((float(
    #    re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[0])[0]) * 60) + float(
    #    re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[1])[0]) * 60) + float(
    #    re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[2])[0])))
    #df_dict['FL'][0] = df_dict['FL'][0].assign(time_sec=lambda x: (((float(
    #    re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[0])[0]) * 60) + float(
    #    re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[1])[0]) * 60) + float(
    #    re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[2])[0])))
    #df_dict['FR'][0] = df_dict['FR'][0].assign(time_sec=lambda x: (((float(
    #        re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[0])[0]) * 60) + float(
    #        re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[1])[0]) * 60) + float(
    #        re.findall(r"[-+]?(?:\d*\.*\d+)", str(x['Time_Duration']).split(':')[2])[0])))

    df_dict['RL'][0]['Travelled_Time (sec)'] = ''
    for i in range(len(df_dict['RL'][0])):
        time = 0
        var = df_dict['RL'][0]['Time_Duration'][i].split(':')
        hr = re.search("[-+]?(?:\d*\.*\d+)", var[0])
        if hr:
            time = time + (float(hr.group()) * 3600)
        minute = re.search("[-+]?(?:\d*\.*\d+)", var[1])
        if minute:
            time = time + (float(minute.group()) * 60)
        sec = re.search("[-+]?(?:\d*\.*\d+)", var[2])
        if sec:
            time = time + (float(sec.group()))
        df_dict['RL'][0]['Travelled_Time (sec)'][i] = time

    df_dict['RR'][0]['Travelled_Time (sec)'] = ''
    for i in range(len(df_dict['RR'][0])):
        time = 0
        var = df_dict['RR'][0]['Time_Duration'][i].split(':')
        hr = re.search("[-+]?(?:\d*\.*\d+)", var[0])
        if hr:
            time = time + (float(hr.group()) * 3600)
        minute = re.search("[-+]?(?:\d*\.*\d+)", var[1])
        if minute:
            time = time + (float(minute.group()) * 60)
        sec = re.search("[-+]?(?:\d*\.*\d+)", var[2])
        if sec:
            time = time + (float(sec.group()))
        df_dict['RR'][0]['Travelled_Time (sec)'][i] = time

    df_dict['FL'][0]['Travelled_Time (sec)'] = ''
    for i in range(len(df_dict['FL'][0])):
        time = 0
        var = df_dict['FL'][0]['Time_Duration'][i].split(':')
        hr = re.search("[-+]?(?:\d*\.*\d+)", var[0])
        if hr:
            time = time + (float(hr.group()) * 3600)
        minute = re.search("[-+]?(?:\d*\.*\d+)", var[1])
        if minute:
            time = time + (float(minute.group()) * 60)
        sec = re.search("[-+]?(?:\d*\.*\d+)", var[2])
        if sec:
            time = time + (float(sec.group()))
        df_dict['FL'][0]['Travelled_Time (sec)'][i] = time

    df_dict['FR'][0]['Travelled_Time (sec)'] = ''
    for i in range(len(df_dict['FR'][0])):
        time = 0
        var = df_dict['FR'][0]['Time_Duration'][i].split(':')
        hr = re.search("[-+]?(?:\d*\.*\d+)", var[0])
        if hr:
            time = time + (float(hr.group()) * 3600)
        minute = re.search("[-+]?(?:\d*\.*\d+)", var[1])
        if minute:
            time = time + (float(minute.group()) * 60)
        sec = re.search("[-+]?(?:\d*\.*\d+)", var[2])
        if sec:
            time = time + (float(sec.group()))
        df_dict['FR'][0]['Travelled_Time (sec)'][i] = time

    for i in range(len(df_dict['FR'][0])):
        df_var['dist_time'][i] = max(df_dict['RL'][0]['Travelled_Time (sec)'][i], df_dict['RR'][0]['Travelled_Time (sec)'][i],
                                     df_dict['FL'][0]['Travelled_Time (sec)'][i], df_dict['FR'][0]['Travelled_Time (sec)'][i])

    if print_flag:
        print('\n\n')
        print('2nd df_dict RL is: ')
        print(df_dict['RL'][0])
        print('\n\n')

    df_dict['RL'][0] = df_dict['RL'][0][['File_name', 'Logs_count', 'Dist_Travelled (kms)','Travelled_Time (sec)',  'Time_Duration', 'Avg_VehSpeed']]
    df_dict['RR'][0] = df_dict['RR'][0][['File_name', 'Logs_count', 'Dist_Travelled (kms)','Travelled_Time (sec)',  'Time_Duration', 'Avg_VehSpeed']]
    df_dict['FL'][0] = df_dict['FL'][0][['File_name', 'Logs_count', 'Dist_Travelled (kms)','Travelled_Time (sec)',  'Time_Duration', 'Avg_VehSpeed']]
    df_dict['FR'][0] = df_dict['FR'][0][['File_name', 'Logs_count', 'Dist_Travelled (kms)','Travelled_Time (sec)',  'Time_Duration', 'Avg_VehSpeed']]

    df_final.loc[len(df_final.index)] = [df_var['dist_km'].sum(), df_var['dist_time'].sum()]

    df_final.to_excel(writer, sheet_name='OVERALL_DETAILS', startrow=0, startcol=0, index=False)
    df_dict['RL'][0].to_excel(writer, sheet_name='DIST_RL', startrow=0, startcol=0, index=False)
    df_dict['RR'][0].to_excel(writer, sheet_name='DIST_RR', startrow=0, startcol=0, index=False)
    df_dict['FL'][0].to_excel(writer, sheet_name='DIST_FL', startrow=0, startcol=0, index=False)
    df_dict['FR'][0].to_excel(writer, sheet_name='DIST_FR', startrow=0, startcol=0, index=False)

    df_error_msgs.to_excel(writer, sheet_name='Error_details', startrow=0, startcol=0, index=False)

    if print_flag:
        print('final df_final is: ')
        print(df_final)
    writer.close()

    print('Execution is completed successfully')

if __name__ == '__main__':
    input_file = sys.argv[1]
    input_file_path, input_file_name = os.path.split(input_file)

    if print_flag:
        print('file_path is: ', input_file_path)
        print('file name is: ', input_file_name)

    output_path = input_file_path + '/Stats_Mining.xlsx'
    writer = pd.ExcelWriter(output_path, engine='xlsxwriter')

    if input_file.endswith('.txt'):
        print("Checking xml files in xml filelist (DQ_fList.txt) :-->")
        with open(input_file) as file:
            filelist = file.read().splitlines()
        print("Done!!!!")

        for j in range(len(filelist)):
            if not bool(filelist[j]):
                blank_count += 1
        filelist = segregate_file(filelist, df_error_msgs_list)
        print("\nPROCESSING DETAILS :--> \n")

        if len(filelist) > 0:
            print("   ** Extraction will start for " + str(len(filelist) - blank_count) + " sets"); stat_func(filelist)
        else:
            print("   ** Extraction Stopped as we have " + str(len(filelist) - blank_count) + " sets to process")
            df_error_msgs = pd.DataFrame(data=df_error_msgs_list, columns=['file_path', 'error_message'])
            df_error_msgs.to_excel(writer, sheet_name='Error_details', startrow=0, startcol=0, index=False)
            writer.close()