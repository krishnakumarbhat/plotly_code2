import xml.etree.ElementTree as ET
from xml.dom import minidom
import sys
import os

def prettify(xmlStr):
    INDENT = "\t"
    rough_string = ET.tostring(xmlStr, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent=INDENT, newl="")

path = os.path.dirname(os.path.realpath(sys.argv[0]))
file = path + "/pyinput.xml"
loop=int(sys.argv[1])
t = ET.parse(file)
r = t.getroot()

index = 0;
print("Generating modelDescription.in.xml with {} input and outputs".format(loop))
for child in r:
    if child.tag == "ModelVariables":
        for i in range(loop):
            inputmodelvar = ET.Element('ScalarVariable')
            inputmodelvar.set('name',"SensorDataIn.base.lo")
            inputmodelvar.set('valueReference', "0")
            inputmodelvar.set('causality',"input")
            inputmodelvar.set('variability',"discrete")
            intvar = ET.Element('Integer')
            intvar.set("start","0")
            inputmodelvar.append(intvar)
            inputmodelvar.tail="\n"
            if i!=0:
                string_name ="SensorDataIn{}.base.lo".format(i)
                inputmodelvar.set("name",string_name)
            inputmodelvar.set("valueReference",str(index))
            child.insert(index, inputmodelvar)
            index+=1

            inputmodelvar = ET.Element('ScalarVariable')
            inputmodelvar.set('name',"SensorDataIn.base.hi")
            inputmodelvar.set('valueReference', "0")
            inputmodelvar.set('causality',"input")
            inputmodelvar.set('variability',"discrete")
            intvar = ET.Element('Integer')
            intvar.set("start","0")
            inputmodelvar.append(intvar)
            inputmodelvar.tail="\n"
            if i!=0:
                string_name ="SensorDataIn{}.base.hi".format(i)
                inputmodelvar.set("name",string_name)
            inputmodelvar.set("valueReference",str(index))
            child.insert(index, inputmodelvar)
            index+=1

            inputmodelvar = ET.Element('ScalarVariable')
            inputmodelvar.set('name',"SensorDataIn.size")
            inputmodelvar.set('valueReference', "0")
            inputmodelvar.set('causality',"input")
            inputmodelvar.set('variability',"discrete")
            intvar = ET.Element('Integer')
            intvar.set("start","0")
            inputmodelvar.append(intvar)
            inputmodelvar.tail="\n"
            if i!=0:
                string_name ="SensorDataIn{}.size".format(i)
                inputmodelvar.set("name",string_name)
            inputmodelvar.set("valueReference",str(index))
            child.insert(index, inputmodelvar)
            index+=1

        for i in range(loop):
            outputmodelvar = ET.Element('ScalarVariable')
            outputmodelvar.set('name',"SensorDataOut.base.lo")
            outputmodelvar.set('valueReference', "0")
            outputmodelvar.set('causality',"output")
            outputmodelvar.set('variability',"discrete")
            outputmodelvar.set('initial', "exact")
            intvar = ET.Element('Integer')
            intvar.set("start","0")
            outputmodelvar.append(intvar)
            outputmodelvar.tail="\n"
            if i!=0:
                string_name ="SensorDataOut{}.base.lo".format(i)
                outputmodelvar.set("name",string_name)
            outputmodelvar.set("valueReference",str(index))
            child.insert(index, outputmodelvar)
            index+=1

            outputmodelvar = ET.Element('ScalarVariable')
            outputmodelvar.set('name',"SensorDataOut.base.hi")
            outputmodelvar.set('valueReference', "0")
            outputmodelvar.set('causality',"output")
            outputmodelvar.set('variability',"discrete")
            outputmodelvar.set('initial', "exact")
            intvar = ET.Element('Integer')
            intvar.set("start","0")
            outputmodelvar.append(intvar)
            outputmodelvar.tail="\n"
            if i!=0:
                string_name ="SensorDataOut{}.base.hi".format(i)
                outputmodelvar.set("name",string_name)
            outputmodelvar.set("valueReference",str(index))
            child.insert(index, outputmodelvar)
            index+=1

            outputmodelvar = ET.Element('ScalarVariable')
            outputmodelvar.set('name',"SensorDataOut.size")
            outputmodelvar.set('valueReference', "0")
            outputmodelvar.set('causality',"output")
            outputmodelvar.set('variability',"discrete")
            outputmodelvar.set('initial', "exact")
            intvar = ET.Element('Integer')
            intvar.set("start","0")
            outputmodelvar.append(intvar)
            outputmodelvar.tail="\n"
            if i!=0:
                string_name ="SensorDataOut{}.size".format(i)
                outputmodelvar.set("name",string_name)
            outputmodelvar.set("valueReference",str(index))
            child.insert(index, outputmodelvar)
            index+=1

        for b in child.findall('ScalarVariable'):
            if "_ECU_" in b.attrib["name"]:
                for childofchild in b:
                    if childofchild.tag=="Integer":
                        b.attrib["valueReference"] = str(index)
                        index+=1



prettified_xmlStr = prettify(r)
output_file = open(path + "/modelDescription.in.xml", "w")
output_file.write(prettified_xmlStr)
output_file.close()

