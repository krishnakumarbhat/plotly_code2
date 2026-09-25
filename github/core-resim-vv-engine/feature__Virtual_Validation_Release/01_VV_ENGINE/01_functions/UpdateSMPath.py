import os
import yaml

# get all VehicleConfigs in Path
cwd = os.getcwd()
SM_Config_Path = os.path.join("r\04_MODEL_CONFIG")
SM_FMU_Path = r"\02_SENSOR_MODEL\FMU\Windows\SRR_Master_SensorModel2.fmu"
SM_Config_Path = r"C:\Users\vj432g\wkspaces\Virtual_Validation_PG\04_MODEL_CONFIG"
VehicleConfigList = []
for dirpath, dirnames, filenames in os.walk(SM_Config_Path):
    for filename in [f for f in filenames if f.endswith(".yaml")]:
        VehicleConfigList.append(os.path.join(dirpath, filename))

for VehicleConfig in VehicleConfigList:
    with open(VehicleConfig) as file:

        # load yaml config
        Yamlconfig = yaml.load(file, Loader=yaml.BaseLoader)

        # update SM path
        for n_Model in range(0, len(Yamlconfig["SensorSimulator"]["ModelList"]) - 1):
            Yamlconfig["SensorSimulator"]["ModelList"][n_Model]["FmuArchive"]["Path"] = SM_FMU_Path

        file.close()
    with open(VehicleConfig, 'w') as file:
        yaml.dump(Yamlconfig, file, default_flow_style=False, sort_keys=False)
