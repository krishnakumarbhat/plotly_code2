import os
import sys
import yaml

name_of_script = sys.argv[0]
customer = int((sys.argv[1]))
modelPath = sys.argv[2]

# To get preferred indentation while using lists
class Dumper(yaml.Dumper):
	def increase_indent(self, flow=False, *args, **kwargs):
		return super().increase_indent(flow=flow, indentless=False)
	   
# Function to get customer fmu path
def fetch_fmu_path(customer):
    if customer == 1:
        return "/Binaries/SRR_Master_LogicModel2_CEER.fmu"
    else:
        return "0"

# To get customer fmu path
customer_fmu_path = fetch_fmu_path(customer)

# To get current working directory
cwd = os.getcwd()

# To get full path of customer yaml
myfile_path = cwd + "/" + modelPath

with open(myfile_path) as file:
	doc = yaml.safe_load(file)

SensorModel_path = "/Binaries/SRR_Master_SensorModel2.fmu"
	
LogicModel_path = customer_fmu_path

if customer == 2:
    # ADCAM has multiple different SM2 and LM2 FMUs.
    # Derive each model's container path from the FMU filename already in the YAML.
    for model in doc['SensorSimulator']['ModelList']:
        if 'FmuArchive' in model:
            fmu_filename = os.path.basename(model['FmuArchive']['Path'])
            model['FmuArchive']['Path'] = f"/Binaries/{fmu_filename}"
else:
    for model in doc['SensorSimulator']['ModelList']:
        model_type = model.get('ModelType','').lower()
        if 'FmuArchive' in model:
            if model_type == 'sensor':
                model['FmuArchive']['Path']=SensorModel_path
            elif model_type == 'logic':
                model['FmuArchive']['Path']=LogicModel_path

with open(myfile_path, 'w') as fout:
	yaml.dump(doc, fout, Dumper=Dumper, default_flow_style=False, sort_keys=False)


