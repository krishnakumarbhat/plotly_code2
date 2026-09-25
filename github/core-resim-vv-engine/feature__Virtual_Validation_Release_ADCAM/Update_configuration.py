import os
import sys
import yaml

name_of_script = sys.argv[0]
customer = int((sys.argv[1]))

# To get preferred indentation while using lists
class Dumper(yaml.Dumper):
    def increase_indent(self, flow=False, *args, **kwargs):
        return super().increase_indent(flow=flow, indentless=False)
       
# Function to get customer yaml path
def fetch_customer_yaml_path(customer):
    if customer == 1:
        return "/Binaries/modelconfig_sil_CEER_With_FC.yaml"
    elif customer == 2:
        return "/Binaries/modelconfig_3radarSM2_1camSM2_1camLM2_1IFVLM2.yaml"
    else :
        return "0"

# To get customer yaml path
customer_yaml_file_path = fetch_customer_yaml_path(customer)

# To get current working directory
cwd = os.getcwd()

# To get full path of customer yaml path
customer_yaml_path = customer_yaml_file_path

VVEngineConfigFile_path = cwd +"/Binaries/VVEngineConfig.yaml"

# File open operation
with open(VVEngineConfigFile_path) as file:
    doc = yaml.safe_load(file)
 
doc['VVEngine']['General']['SpecialConfigPath'] = customer_yaml_path
doc['VVEngine']['General']['CloudMode'] = "true"
doc['VVEngine']['Debug']['CreateTimingProfile'] = "false"  # /Binaries is read-only in container

with open(VVEngineConfigFile_path, 'w') as fout:
    yaml.dump(doc, fout, Dumper=Dumper, default_flow_style=False, sort_keys=False)


