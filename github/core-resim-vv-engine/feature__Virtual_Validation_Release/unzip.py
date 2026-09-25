import os
import sys
import yaml
import zipfile

name_of_script = sys.argv[0]
customer = int((sys.argv[1]))

script_dir = os.path.dirname(os.path.abspath(__file__))
binaries_dir = os.path.join(script_dir, 'Binaries')

if customer == 1:
    yaml_path = os.path.join(binaries_dir, "modelconfig_sil_CEER_With_FC.yaml")
elif customer == 2:
    for file in os.listdir(binaries_dir):
        if file.startswith("modelconfig_sil_GPO"):
            yaml_path = os.path.join(binaries_dir, file)
            break
elif customer == 3:
    for file in os.listdir(binaries_dir):
        if file.startswith("modelconfig_sil") and ("40_42_44_46" in file or "4825" in file or "BUS" in file or "AL" in file):
            yaml_path = os.path.join(binaries_dir, file)
            break
else:
    yaml_path = None

if yaml_path is None:
    print("YAML file not found")
    exit(1)

with open(yaml_path) as file:
    doc = yaml.safe_load(file)

model_list = doc['SensorSimulator']['ModelList']
SensorModel_path = os.path.join(binaries_dir, 'SRR_Master_SensorModel2.fmu')

if customer == 1:
    LogicModel_path = os.path.join(binaries_dir, 'SRR_Master_LogicModel2_CEER.fmu')
elif customer == 2:
    LogicModel_path = os.path.join(binaries_dir, 'SRR_Master_LogicModel2_GPO_Gen8.fmu')
elif customer == 3:
    LogicModel_path = os.path.join(binaries_dir, 'SRR_Master_LogicModel2_AL.fmu')
else:
    LogicModel_path = None


for idx, model in enumerate(model_list):
    model_type = model.get('ModelType')
    if model_type == 'Sensor':
        fmu_path = SensorModel_path
        unzip_dir = os.path.join(binaries_dir, f"Model2Id{idx}")
        print(f"id {idx} - ModelType:{model_type}")
    elif model_type == 'Logic':
        fmu_path = LogicModel_path
        unzip_dir = os.path.join(binaries_dir, f"LogicModelId{idx}")
        print(f"id {idx} - ModelType:{model_type}")
    else:
        continue
    
    if os.path.exists(fmu_path):
        with zipfile.ZipFile(fmu_path, 'r') as zip_ref:
            zip_ref.extractall(unzip_dir)
    else:
        print("FMU not found")

