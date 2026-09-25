import os
import sys
import yaml

name_of_script = sys.argv[0]
customer = int((sys.argv[1]))
model_path = sys.argv[2]

# To get preferred indentation while using lists
class Dumper(yaml.Dumper):
    def increase_indent(self, flow=False, *args, **kwargs):
        return super().increase_indent(flow=flow, indentless=False)

# To get current working directory
cwd = os.getcwd()

# Set the customer yaml path
customer_yaml_path = model_path

VVEngineConfigFile_path = cwd +"/Binaries/VVEngineConfig.yaml"

# File open operation
with open(VVEngineConfigFile_path) as file:
    doc = yaml.safe_load(file)
 
doc['VVEngine']['General']['SpecialConfigPath'] = customer_yaml_path
doc['VVEngine']['General']['CloudMode'] = "true"
doc['VVEngine']['Debug']['CreateTimingProfile'] = "true"

with open(VVEngineConfigFile_path, 'w') as fout:
    yaml.dump(doc, fout, Dumper=Dumper, default_flow_style=False, sort_keys=False)


