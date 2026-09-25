import sys
import pandas as pd

#fixed variable values
NUMBER_OF_ROWS_TO_SKIP = 1
NUMBER_OF_COLUMNS_TO_SKIP = 1
MAX_NUMBER_OF_VALUES_IN_ONE_LINE =5

#Get the complete file name from the argument 
filepath = sys.argv[1]

#Get index of last occurance of / and . so that we get the name of the file
index1 = filepath.rfind("\\")
index2 = filepath.rfind(".")

#truncate the filepath to get the filename
filename = filepath[int(index1)+1:int(index2)]
hppfilename = "AutoGen"+filename + ".h"
cppfilename = "AutoGen"+filename + ".cpp"

#header file handler
headerfilehandler = open(hppfilename,'w')
headerfilehandler.write("#pragma once\n")
headerfilehandler.write("#include <vector>\n")

#source file handler
sourcefilehandler = open(cppfilename,'w')
sourcefilehandler.write("#include "+ "\"" + str(hppfilename)+ "\"\n\n")

#read and get objects name 
xlsx = pd.ExcelFile(filepath)
objectnames_ = xlsx.sheet_names[1:]
rcs_min_value =[]
rcs_max_value =[]
skip_first_comma_write=1

headerfilehandler.write("\n"+"namespace sm2" +"{\n\t/**\n\t* Defines SM2 RCS Table Value enum\n\t*/\n" )
sourcefilehandler.write("\n"+"namespace sm2" + "{\n" )
#loop through the sheets
for objectname in objectnames_:
    df= xlsx.parse(objectname, skiprows=NUMBER_OF_ROWS_TO_SKIP)
    if objectname == "Class_Enum_Defination":
        for column_name in df.keys(): 
            if column_name.find('enum')!= -1:
                headerfilehandler.write("\t"+ column_name + "{\n")
                for values in list(df[column_name]):
                    headerfilehandler.write("\t\t"+ values + ",\n")
                headerfilehandler.write("\t};\n") 
            elif column_name.find('struct')!= -1:
                headerfilehandler.write("\t"+ column_name + "{\n")
                for values in list(df[column_name]):
                    headerfilehandler.write("\t\t"+ values + ";\n")
                headerfilehandler.write("\t};\n")    
    else:    
        #for each sheet in excel, loop through the columns
        skip_first_comma_write=1
        for column_name in df.keys():
            if column_name == 'RCS Mean Value':
                headerfilehandler.write("\t"+ " extern std::vector<double> " + "CAS_"+ str(objectname) + "_" + str(column_name).replace(" ", "_") + "_Table ;")
                headerfilehandler.write("/*Values were taken by Cst Asymptotic Solver(CAS) */\n")
                sourcefilehandler.write("\n\t/**\n\t*"+ " Defines RCS mean values taken by Cst Asymptotic Solver(CAS) for " + str(objectname) + "\n\t*/\n")
                sourcefilehandler.write("\t"+ " std::vector<double> " + "CAS_" + str(objectname) + "_" +str(column_name).replace(" ", "_") + "_Table = {\n\t\t\t" )
                count =1
                for values in list(df[column_name]):
                    #dont add comma when its the start of writing values               
                    if skip_first_comma_write==1:
                        skip_first_comma_write=0
                    else:
                        sourcefilehandler.write(",")
                        if count == MAX_NUMBER_OF_VALUES_IN_ONE_LINE:
                           sourcefilehandler.write("\n\t\t\t")
                           count=0
                        count=count + 1
                    sourcefilehandler.write(str(values))               
                sourcefilehandler.write("};\n\n")
            elif column_name == 'RCS Min Value':
                for values in list(df[column_name]):
                    rcs_min_value.append(values)
            elif column_name == 'RCS Max Value':
                for values in list(df[column_name]):
                    rcs_max_value.append(values)
                    
        #set the skip first comma write to true since it will be use when grouping the min max values            
        skip_first_comma_write=1
        if len(rcs_min_value)!=0 and len(rcs_max_value)!=0:
            if len(rcs_min_value)== len(rcs_max_value):
                headerfilehandler.write("\t"+ " extern std::vector<std::vector<double>> "+ "CAS_" + str(objectname) + "_"+ "RCS_Min_Max_Value" + "_Table ;")
                headerfilehandler.write("/*Values were taken by Cst Asymptotic Solver(CAS) */\n")
                sourcefilehandler.write("\t" +"\n\t/**\n\t*"+ " Defines RCS min-max values taken by CAS for " + str(objectname) + "\n\t*/\n")
                sourcefilehandler.write("\t"+ " std::vector<std::vector<double>> " +  "CAS_" +str(objectname) + "_" + "RCS_Min_Max_Value" + "_Table = {\n\t\t\t" )
                count =1
                for minvalue,maxvalue in zip(rcs_min_value,rcs_max_value):               
                    if skip_first_comma_write==1:
                        skip_first_comma_write=0
                    else:
                        sourcefilehandler.write(",")
                        if count == MAX_NUMBER_OF_VALUES_IN_ONE_LINE:
                            sourcefilehandler.write("\n\t\t\t")
                            count=0
                        count=count + 1                    
                    sourcefilehandler.write("{"+ str(minvalue)+ "," + str(maxvalue)+" }")
                #clear buffer after min max value write  
                rcs_min_value = []
                rcs_max_value = []
                sourcefilehandler.write("};\n\n")
                    
headerfilehandler.write("}")
sourcefilehandler.write("}")
        
#close file that was opened 
headerfilehandler.close()
sourcefilehandler.close()
