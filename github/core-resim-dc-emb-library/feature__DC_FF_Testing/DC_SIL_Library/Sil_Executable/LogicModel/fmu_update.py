import os
import shutil
import zipfile
import platform

valid_dll_paths=[]
valid_so_paths =[]

def read_dll_files(directory):
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        if(filename.endswith(".so")):
            valid_so_paths.append((file_path))
        elif(filename.endswith(".dll")):
            valid_dll_paths.append((file_path))
        else:
            print("Invalid or missing SO/DLL file: "+str(filename))

def find_and_copy_files(dll_paths, destination_folder):
    """Copy DLLs to binaries/win64 folder in FMU structure."""
    if not os.path.exists(destination_folder):
        os.mkdir(destination_folder)
    copied = 0
    for dll_path in dll_paths:
        file_name = os.path.basename(dll_path)
        destination = os.path.join(destination_folder, file_name)
        try:
            shutil.copy2(dll_path, destination)
            print("Copied: "+str(file_name))
            copied += 1
        except Exception as e:
            print("Failed to copy "+str(dll_path)+" : "+str(e)+" ")
    print("Total files copied: "+ str(copied))

def unzip_fmu(fmu_file, extract_path):
    """Unzip FMU into a working folder after checking it's valid."""
    try:
        with zipfile.ZipFile(fmu_file, 'r') as zip_ref:
            if zip_ref.testzip() is not None:
                print("Error: FMU file is a corrupt zip archive.")
                return False
            zip_ref.extractall(extract_path)
        print("Unzipped "+str(fmu_file))
        return True
    except zipfile.BadZipFile:
        print("Error: The FMU file is not a valid zip file.")
        return False
    except Exception as e:
        print("Error unzipping FMU: "+str(e))
        return False

def zip_folder(folder_path, output_file):
    """Zip a folder into a valid FMU (.zip with .fmu extension)."""
    try:
        with zipfile.ZipFile(output_file, 'w', zipfile.ZIP_DEFLATED) as zipf:
            for root, _, files in os.walk(folder_path):
                for file in files:
                    full_path = os.path.join(root, file)
                    arcname = os.path.relpath(full_path, folder_path)
                    zipf.write(full_path, arcname)
        print("Zipped folder back.")
        return True
    except Exception as e:
        print("Error while zipping FMU: "+str(e))
        return False

def main_win():
    project_path = os.path.dirname((__file__))
    binaries_path = os.path.join(project_path,"../Radar_dlls/")
    extract_dir = os.path.join(project_path, "unzipped_fmu")
    temp_fmu = os.path.join(project_path, "temp_modified.fmu")
    read_dll_files(binaries_path)

    fmu_file = os.path.join(project_path, "LM2_FMU/Windows/SRR_Master_LogicModel2_CEER_Standalone_sil.fmu")

    if not os.path.isfile(fmu_file):
        print("Error: FMU file not found: "+str(fmu_file))
        return
    else:
        print("FMU file Path :"+fmu_file)

    if not valid_dll_paths:
        print("No valid DLLs found to copy.")
        return

     #UNZIP FMU
    print("\nUnzipping FMU to Copy DLL/SO Files")
    if not unzip_fmu(fmu_file, extract_dir):
        return

    print("\nCopying Windows .DLL files")
    win64_dir = os.path.join(extract_dir, "binaries", "win64")
    if not os.path.exists(win64_dir):
        os.mkdir(win64_dir)
    find_and_copy_files(valid_dll_paths, (win64_dir))

    #ZIPPING FMU
    if not zip_folder(extract_dir, temp_fmu):
        shutil.rmtree(extract_dir)
        return
    shutil.copy2(temp_fmu, fmu_file)
    print("FMU Zipped Successfully")

    #DELETING UNNECCESARY FILE
    shutil.rmtree(extract_dir)
    os.remove(temp_fmu)
    print("\n.DLL Files Copied to FMU.")

def main_linux():
    project_path = os.path.dirname((__file__))
    binaries_path = os.path.join(project_path,"../Radar_dlls/")
    extract_dir = os.path.join(project_path, "unzipped_fmu")
    temp_fmu = os.path.join(project_path, "temp_modified.fmu")
    read_dll_files(binaries_path)

    fmu_file = os.path.join(project_path, "LM2_FMU/Linux/SRR_Master_LogicModel2_CEER_Standalone_sil.fmu")
    if not os.path.isfile(fmu_file):
        print("Error: FMU file not found: "+str(fmu_file))
        return
    else:
        print("FMU file Path :"+fmu_file)

    if not valid_so_paths:
        print("No valid SOs found to copy.")
        return

     #UNZIP FMU
    print("\nUnzipping FMU to Copy DLL/SO Files")
    if not unzip_fmu(fmu_file, extract_dir):
        return

    print("\nCopying Linux .SO files")
    linux64_dir = os.path.join(extract_dir, "binaries", "linux64")
    if not os.path.exists(linux64_dir):
        os.mkdir(linux64_dir)
    find_and_copy_files(valid_so_paths, (linux64_dir))

    #ZIPPING FMU
    if not zip_folder(extract_dir, temp_fmu):
        shutil.rmtree(extract_dir)
        return
    shutil.copy2(temp_fmu, fmu_file)
    print("FMU Zipped Successfully")

    #DELETING UNNECCESARY FILE
    shutil.rmtree(extract_dir)
    os.remove(temp_fmu)
    print("\n.SO Files Copied to FMU.")

def main():
    system = platform.system()
    if system == "Windows":
        print("main run")
        main_win()
    elif system =="Linux":
        main_linux()

if __name__ == "__main__":
    main()
