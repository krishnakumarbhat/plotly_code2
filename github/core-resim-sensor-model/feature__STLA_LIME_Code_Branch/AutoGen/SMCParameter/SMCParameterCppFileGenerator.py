import sys
import os
import pandas as pd
import os.path


print("==================================================================")
print("Note: Python script is implemented in such a way it assumes that")
print("smc parameters for Sensor Model code remains same only the values")
print("for those parameters changes for different customer, ")
print("if there is a change in number of parameters then re-run the script")
print("again so that older generated varaibles be inline with the")
print("new parameter/structure")
print("==================================================================")

#fixed variable values
NUMBER_OF_ROWS_TO_SKIP = 2
NUMBER_OF_COLUMNS_TO_SKIP = 1
MAX_NUMBER_OF_COLUMNS =50

#Get the complete file name from the argument 
filepath = sys.argv[1];

#Get index of last occurance of / and . so that we get the name of the file
index1 = filepath.rfind("/")
index2 = filepath.rfind(".")

#truncate the filepath to get the filename
filename = "SMCParameter"
hppfilename = "AutoGen"+filename + ".h"
cppfilename = "AutoGen"+filename + ".cpp"

def GetCppDataType(datatype):
    line=""
    if datatype == 'uint32':
        line="unsigned int"
    elif datatype == 'uint16':
        line="unsigned short"
    elif datatype == 'uint8':
        line="unsigned char"
    elif datatype == 'int32':
        line="int"
    elif datatype == 'int16':
        line="short"
    elif datatype == 'char':
        line="char"
    elif datatype == 'float':
        line="float"
    elif datatype == 'double':
        line="double"
    elif datatype == 'bool':
        line="bool"
    elif datatype == 'uint32[]':
        line="std::vector<unsigned int>"     
    elif datatype == 'uint16[]':
        line="std::vector<unsigned short>"
    elif datatype == 'uint8[]':
        line="std::vector<unsigned char>"
    elif datatype == 'int32[]':
        line="std::vector<int>"
    elif datatype == 'int16[]':
        line="std::vector<short>"
    elif datatype == 'char[]':
        line="std::vector<char>"
    elif datatype == 'float[]':
        line="std::vector<float>"
    elif datatype == 'double[]':
        line="std::vector<double>"
    elif datatype == 'bool[]':
        line="std::vector<bool>"
    elif datatype == 'uint32[][]':
        line="std::vector<std::vector<unsigned int>>"     
    elif datatype == 'uint16[][]':
        line="std::vector<std::vector<unsigned short>>"
    elif datatype == 'uint8[][]':
        line="std::vector<std::vector<unsigned char>>"
    elif datatype == 'int32[][]':
        line="std::vector<std::vector<int>>"
    elif datatype == 'int16[][]':
        line="std::vector<std::vector<short>>"
    elif datatype == 'char[][]':
        line="std::vector<std::vector<char>>"
    elif datatype == 'float[][]':
        line="std::vector<std::vector<float>>"
    elif datatype == 'double[][]':
        line="std::vector<std::vector<double>>"
    elif datatype == 'bool[][]':
        line="std::vector<std::vector<bool>>"
    elif datatype == 'string':
        line="std::string"
    return line

def CheckIfHeaderFileExists(headerfile):
    if(os.path.exists(headerfile)):
        return True
    else:
        return False

def GetParametersFromExistedHeaderFile(headerfile):
    existingparameters=[]
    if(CheckIfHeaderFileExists(headerfile)):
        file=open(headerfile,"r")
        for line in file.readlines():
            if(-1!=line.find("struct") or -1!=line.rfind("{")):
                continue
            if(-1!=line.rfind("};")):
                break
            if(-1!=line.rfind(";")):
                parameter= line[line.rfind(" ")+1:line.rfind(";")] #find member name from stucuture members removing the datatype of it
                existingparameters.append(parameter)
        file.close()
    return existingparameters
    
def CheckIfNewParamertersAddedToSM2Sheet(xlfile, headerfile):
    returnval=True
    oldparameterslist = GetParametersFromExistedHeaderFile(headerfile)
    newparameterlist=[]
    foundlist = []
    
    for idx in range(0,len(oldparameterslist)):
        foundlist.append(-1)
        
    handler = pd.read_excel(xlfile, sheet_name='SM2SMCParameter',
                             skiprows=range(NUMBER_OF_ROWS_TO_SKIP),
                             usecols=range(NUMBER_OF_COLUMNS_TO_SKIP,MAX_NUMBER_OF_COLUMNS))
    structurename = handler.keys()[0]
    datatypelist =handler.keys()[1]
    ordered_number=[]
    newparameterlist=[]
	
    for member in handler[structurename]:
        newparameterlist.append(member)
	
    for datatype in handler[datatypelist]:
        if(datatype=="double[][]"):
            ordered_number.append(1)
        elif(datatype=="double[]"):
            ordered_number.append(2)
        elif(datatype=="double"):
            ordered_number.append(3)
        else:
            ordered_number.append(4)
		
    for i in range(0,len(ordered_number)):
        for j in range(i,len(ordered_number)):
            if(ordered_number[i]>ordered_number[j]):
                temp_number = ordered_number[i]
                ordered_number[i]=ordered_number[j]
                ordered_number[j]=temp_number

                temp_parameter = newparameterlist[i]
                newparameterlist[i]=newparameterlist[j]
                newparameterlist[j]=temp_parameter
	
    #match the olderparameters with newparameters, if they exists and are in same order
    oldindex =0
    for oldparameter in oldparameterslist:
        newindex=0
        for newparameter in newparameterlist:
            if(newparameter == oldparameter):
                foundlist[oldindex]=newindex
                break
            newindex=newindex+1
        oldindex=oldindex+1
    
    allparameterinorder=0
    for idx in range(0,len(oldparameterslist)):
        if foundlist[idx]==idx:
            allparameterinorder=1
        else:
            print("\n\n[ERROR]:Sequence of structure members in existed structure in AutoGenSMCParamter")
            print(".h file doesnt match with the new SM2 SMC sheet, older struct variable in AutoGenSMCParameter.cpp")
            print("file will be incompatible with the new structure. Please regenerate the")
            print("existing old struct variable in AutoGenSMCParater.cpp file with the same order as the new")
            print("excel file used OR if the struct member is removed from new excel sheet then")
            print("keep struct member as it is in new excel file and make the value as ND so")
            print("that AutoGenSMCParameter.h file structure declaration is compatible with older variable in AutoGenSMCParameter.cpp")

            print("\n\n mismatched occured when trying to compare AutoGenSMCParameters.h and new excel sheet shown below")
            if idx > len(newparameterlist):
                print("\n\t old list = " + oldparameterslist[idx] +  "\t new list = " + " ")
            else:
                print("\n\t old list = " + oldparameterslist[idx] +  "\t new list = " + newparameterlist[idx])
            returnval=False
            allparameterinorder=0
            
    nothinghaschanged=0
    if allparameterinorder == 1 and len(oldparameterslist)==len(newparameterlist):
        nothinghaschanged=1
        print("\n[INFO]: Existed AutoGenSMCParameter.h structure member matches with excel file")
    elif allparameterinorder == 1 and len(newparameterlist)>len(oldparameterslist):  
        print("\n\n[ERROR]: New structure member added to the new excel sheet")
        print("Please regenerate the existing structure variable in AutoGenSMCParameter.cpp")
        print("with the new structure memebr added to its SMC excel file")
        print("in SM2SMCParameter sheet with value being ND")
        print("\n\n mismatched occured when trying to compare AutoGenSMCParameters.h and new excel sheet shown below")
        print("\n\t old list = " + " " +  "\t new list = " + newparameterlist[len(oldparameterslist)])
        nothinghaschanged=0
        returnval=False
        
    return returnval
       
# Create file , if file already exist then copy old file to new file without the endline else create new file and return filehandler
def CreateCppFile(outputfile, include_string="",searchkeyword=""):
    if(os.path.exists(outputfile)):
        tempoutputfile = outputfile[:outputfile.rfind(".")]+"_old"+outputfile[outputfile.rfind("."):]
        os.rename(outputfile,tempoutputfile)
        outputfilehandleroriginal= open(tempoutputfile,"r+")
        outputfilenewhandler= open(outputfile,"w+")
        for line in outputfilehandleroriginal.readlines():
            if(-1!=line.find("}/*endoffile*/") or (searchkeyword!="" and -1!=line.find(searchkeyword))):
                break
            else:
                outputfilenewhandler.write(line)
        outputfilehandleroriginal.close()
        os.remove(tempoutputfile)
        return outputfilenewhandler
    else:
        outputfilehandler = open(outputfile,'w')
        outputfilehandler.write(includefilestatement)
        outputfilehandler.write("\nnamespace sm2 { \n")
        return outputfilehandler

# Create file , if file already exist then copy old file to new file without the endline else create new file and return filehandler
def SearchExternLineIfFileExist(outputfile):
    extern_line = []
    if(os.path.exists(outputfile)):
        outputfilehandleroriginal= open(outputfile,"r+")
        for line in outputfilehandleroriginal.readlines():
            if(-1!=line.find("extern ")):
                extern_line.append(line)
        
    return extern_line



#================================main function ========================================
if(not CheckIfNewParamertersAddedToSM2Sheet(filepath,hppfilename)):
    exit()
    
SM2excelfile = pd.read_excel(filepath, sheet_name='SM2SMCParameter',
                             skiprows=range(NUMBER_OF_ROWS_TO_SKIP),
                             usecols=range(NUMBER_OF_COLUMNS_TO_SKIP,MAX_NUMBER_OF_COLUMNS))

columnname = SM2excelfile.keys()

#write cpp file
includefilestatement = "#include \""+str(hppfilename)+"\"\n"
c_sourcefile = CreateCppFile(cppfilename,includefilestatement)

customer = []
for cust in columnname[3:]:
    if not cust.startswith("Unnamed"):
        customer.append(cust)

#This was done to order the file generation based on first vector<vector> then vector<> then other variables
ordered_member_name = []
ordered_member_value = []
ordered_member_type = []
ordered_number=[]
for (membername, variablevalue,datatype) in zip( SM2excelfile[str(columnname[0])], SM2excelfile[customer[0]], SM2excelfile[str(columnname[1])]):
    ordered_member_name.append(membername)
    ordered_member_value.append(str(variablevalue))
    ordered_member_type.append(datatype)
    if(datatype=="double[][]"):
        ordered_number.append(1)
    elif(datatype=="double[]"):
        ordered_number.append(2)
    elif(datatype=="double"):
        ordered_number.append(3)
    else:
        ordered_number.append(4)

for i in range(0,len(ordered_number)):
    for j in range(i,len(ordered_number)):
        if(ordered_number[i]>ordered_number[j]):
            temp_number = ordered_number[i]
            temp_member_name = ordered_member_name[i]
            temp_member_value = ordered_member_value[i]
            temp_member_type = ordered_member_type[i]
            
            ordered_number[i]=ordered_number[j]
            ordered_member_name[i]=ordered_member_name[j]
            ordered_member_value[i]=ordered_member_value[j]
            ordered_member_type[i]=ordered_member_type[j]
        
            ordered_number[j]=temp_number
            ordered_member_name[j]=temp_member_name
            ordered_member_value[j]=temp_member_value
            ordered_member_type[j]=temp_member_type
 

#print(membervalue)
structobjlist = []
variablename= cppfilename[:int(cppfilename.rfind("."))] +" " +str(customer[0])+"_SM2_SMC_Parameters"
structobjlist.append(variablename[(int(variablename.rfind(" "))+1):])
c_sourcefile.write(variablename)
c_sourcefile.write(" = {\n")
index =0
for (membername, variablevalue,datatype) in zip( ordered_member_name, ordered_member_value, ordered_member_type):
    if(datatype=="double[]"):
        if(variablevalue=="ND"):
            c_sourcefile.write("\t{}")
        else:
            c_sourcefile.write("\t{"+str(variablevalue) + "}")
    elif(datatype=="double[][]"):
        if(variablevalue=='ND'):
            c_sourcefile.write("\t{}")
        else: 
            temp= str(variablevalue).replace("[", "{")
            temp = temp.replace("]","}")
            temp = temp.replace("{", "{{")
            temp = temp.replace(" ;",";")
            temp = temp.replace(";","},{")
            temp = temp.replace(" }","}")
            temp = temp[:temp.rfind("}")]+"}}"  
            #replace space " " with ","
            temp = temp.replace(" ",",")
            c_sourcefile.write("\t"+temp)
    else:
        if(variablevalue=='ND'):
            c_sourcefile.write("\t0")
        else:
            c_sourcefile.write("\t"+str(variablevalue))

    if index!=len(ordered_member_name)-1:
        c_sourcefile.write(",")
    comment = "/* "+ membername + " */ \n"                                           
    c_sourcefile.write(comment)
    index=index+1
c_sourcefile.write("};\n\n")
     
for var in structobjlist:
    c_sourcefile.write("/**\n*"+ " Initializes " + str(var) + "\n*/\n")
    addfunction2file = "void init_"+ str(var) + "(" + cppfilename[:int(cppfilename.rfind("."))] +" *ptr){\n"
    c_sourcefile.write(addfunction2file )
    addfunctionbody2file = "\t*ptr = " + str(var) +";\n"
    c_sourcefile.write(addfunctionbody2file)
    c_sourcefile.write("}\n\n")

c_sourcefile.write("\n}/*endoffile*/")
c_sourcefile.close()

#=======================Write Header file====================================
addline=SearchExternLineIfFileExist(hppfilename)
c_headerfile = open(hppfilename,'w')
c_headerfile.write("#pragma once\n")
c_headerfile.write("#include <vector>\n")
c_headerfile.write("#include <string>\n")
c_headerfile.write("\nnamespace sm2 { \n\n")
c_headerfile.write("\n/**\n* Declares auto generated SMC parameters\n*/\n")
structure_declaration = "struct " + cppfilename[:int(cppfilename.rfind("."))] +"{\n"
c_headerfile.write(structure_declaration)

for (names,datatype) in zip( ordered_member_name, ordered_member_type):
    #print(names, datatype)
    type_ = GetCppDataType(datatype)
    c_headerfile.write("\t\t"+type_+" "+names)
    c_headerfile.write(";\n")

c_headerfile.write("};\n\n")
for oldline in addline:
    c_headerfile.write(oldline)
    
for var in structobjlist:
    addfunctiondeclaration2file = "extern void init_"+ str(var) + "(" + cppfilename[:int(cppfilename.rfind("."))] + " *ptr);\n"
    c_headerfile.write(addfunctiondeclaration2file)

c_headerfile.write("\n}/*endoffile*/")
c_headerfile.close()


                


