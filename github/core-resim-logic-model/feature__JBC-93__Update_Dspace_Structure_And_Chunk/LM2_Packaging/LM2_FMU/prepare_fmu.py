import os
import shutil
import sys
import glob

# Optional 6th argument: path to real SIL library directory.
# If provided its .so files are copied last, overriding FW_LIME/RADAR_DLLS.
sil_lib_dir = sys.argv[6].strip() if len(sys.argv) > 6 else ""

# Get arguments
build_dir = sys.argv[1]
deliverables_dir = sys.argv[2]
project_root = sys.argv[3]
customer = sys.argv[4]
model_name = sys.argv[5]

# Customers that share the same data files as CEER
CEER_COMPATIBLE_CUSTOMERS = {"GPO_Gen8"}
data_customer = "CEER" if customer in CEER_COMPATIBLE_CUSTOMERS else customer

# Create necessary directories
buildfmu_dir = os.path.join(build_dir, 'buildfmu')
win64_dir = os.path.join(buildfmu_dir, 'binaries', 'win64')
linux64_dir = os.path.join(buildfmu_dir, 'binaries', 'linux64')

if os.path.exists(buildfmu_dir):
    shutil.rmtree(buildfmu_dir)

os.makedirs(win64_dir, exist_ok=True)
os.makedirs(linux64_dir, exist_ok=True)

# Copy Windows DLL from deliverables
dll_path = os.path.join(deliverables_dir, model_name + '.dll')
if os.path.exists(dll_path):
    shutil.copy(dll_path, win64_dir)

# Copy Linux .SO from deliverables
so_path = os.path.join(deliverables_dir, model_name + '.so')
if os.path.exists(so_path):
    shutil.copy(so_path, linux64_dir)

# Copy additional files based on FMU type
config_path = os.path.join(project_root, 'Data', 'Aptiv_Libraries', 'DEFAULT_CONFIG_XMLS', data_customer)
fw_lime_path = os.path.join(project_root, 'Data', 'Aptiv_Libraries', 'FW_LIME')
radar_dlls_path = os.path.join(project_root, 'Data', 'Aptiv_Libraries', 'RADAR_DLLS', data_customer)
dc_path = os.path.join(project_root, 'Data', 'Aptiv_Libraries', 'DC')

# Copy Windows and Linux config files
if os.path.exists(config_path):
    for file in os.listdir(config_path):
        file_path = os.path.join(config_path, file)
        if os.path.isfile(file_path) and file.endswith('.xml'):
            shutil.copy(file_path, win64_dir)
            shutil.copy(file_path, linux64_dir)
 
# Copy Windows DLLs from data folders
    for src_dir in [fw_lime_path, radar_dlls_path, dc_path]:
        if os.path.exists(src_dir):
            for dll_file in glob.glob(os.path.join(src_dir, '*.dll')):
                shutil.copy(dll_file, win64_dir)

# Copy Linux SOs from data folders
    for src_dir in [fw_lime_path, radar_dlls_path, dc_path]:
        if os.path.exists(src_dir):
            for so_file in glob.glob(os.path.join(src_dir, '*.so*')):
                shutil.copy(so_file, linux64_dir)

# If a real SIL library directory was provided, copy its .so files last so
# they always override whatever FW_LIME or RADAR_DLLS contained (e.g. a stub).
if sil_lib_dir and os.path.isdir(sil_lib_dir):
    print(f"[INFO] Overriding SIL libraries from: {sil_lib_dir}")
    for so_file in glob.glob(os.path.join(sil_lib_dir, '*.so')):
        shutil.copy(so_file, linux64_dir)
        print(f"[INFO]   Copied {os.path.basename(so_file)}")
    #copy .mf4 files from SIL_LIB_DIR if they exist
    for mf4_file in glob.glob(os.path.join(sil_lib_dir, '*.mf4')):
        shutil.copy(mf4_file, linux64_dir)
elif sil_lib_dir:
    print(f"[WARNING] SIL_LIB_DIR specified but not found: {sil_lib_dir}")

# Copy CoreProtocol DBC files (required for CoreProtocol_Decoder)
# DBC is at <Core_RESIM_Logic_Model>/external_deps/v1/dbc, compute relative to script location
script_dir = os.path.dirname(os.path.abspath(__file__))
# From LM2_Packaging/LM2_FMU, go up 2 levels to reach Core_RESIM_Logic_Model