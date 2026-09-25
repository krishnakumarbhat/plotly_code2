import os
import sys
import yaml
import zipfile
import shutil

name_of_script = sys.argv[0]
customer = int((sys.argv[1]))

script_dir = os.path.dirname(os.path.abspath(__file__))
binaries_dir = os.path.join(script_dir, 'Binaries')

if customer == 1:
    yaml_path = os.path.join(binaries_dir, "modelconfig_sil_CEER_With_FC.yaml")
elif customer == 2:
    yaml_path = os.path.join(binaries_dir, "modelconfig_3radarSM2_1camSM2_1camLM2_1IFVLM2.yaml")
else:
    print(f"Unknown customer id: {customer}")
    sys.exit(1)

with open(yaml_path) as file:
    doc = yaml.safe_load(file)

model_list = doc['SensorSimulator']['ModelList']

for idx, model in enumerate(model_list):
    model_type = model.get('ModelType')
    if model_type == 'Sensor':
        unzip_dir = os.path.join(binaries_dir, f"Model2Id{idx}")
    elif model_type == 'Logic':
        unzip_dir = os.path.join(binaries_dir, f"LogicModelId{idx}")
    else:
        continue

    # Use the FMU filename from the (already-updated) YAML path: /Binaries/<filename>
    fmu_archive_path = model.get('FmuArchive', {}).get('Path', '')
    fmu_filename = os.path.basename(fmu_archive_path)
    fmu_path = os.path.join(binaries_dir, fmu_filename)
    print(f"id {idx} - ModelType:{model_type} FMU:{fmu_filename}")

    if not os.path.exists(fmu_path):
        print(f"  ERROR: FMU not found: {fmu_path}")
        continue

    # Always extract fresh — do not rely on pre-existing dirs from native runs
    if os.path.exists(unzip_dir):
        shutil.rmtree(unzip_dir)
    with zipfile.ZipFile(fmu_path, 'r') as zip_ref:
        zip_ref.extractall(unzip_dir)

    # Some Logic FMUs (e.g. CAMERAECU_LM2) pack their config JSON inside
    # resources/LM2Fmu.json. At runtime the FMU copies it to the working
    # directory root — but inside a Singularity image that path is read-only
    # squashfs, so the copy fails and fmi2ExitInitializationMode returns failure.
    # We promote it here at image-build time instead.
    if model_type == 'Logic':
        json_src = os.path.join(unzip_dir, 'resources', 'LM2Fmu.json')
        json_dst = os.path.join(unzip_dir, 'LM2Fmu.json')
        if os.path.exists(json_src) and not os.path.exists(json_dst):
            shutil.copy2(json_src, json_dst)
            print(f"  Promoted resources/LM2Fmu.json to FMU root")

