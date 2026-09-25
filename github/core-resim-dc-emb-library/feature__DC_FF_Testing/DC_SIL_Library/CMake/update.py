import os 

file_list = []
# list of all files with full path
def get_file_list():
    os.getcwd()
    os.chdir( '../../../Application/F360Tracker/F360TrackerLib')
    path = os.getcwd()
    path = path.replace('\\', '/')
    for path, subdirs, files in os.walk(path):
        for f in files:
            if f in ['f360_detection_hist.h', 'f360_object_track_initialization.cpp', 'f360_tracker.cpp', 'f360_xtrk_logging.h', 'f360_xtrk_logging.cpp', 'f360_tracker.h']:
                current_file_path = os.path.join(path, f)
                current_file_path = current_file_path.replace('\\', '/')
                file_list.append(current_file_path) 


# remove _DEBUG macro for file_list
def remove_debug_macro():
    debug_file_list = ['f360_object_track_initialization.cpp', 'f360_tracker.cpp', 'f360_xtrk_logging.h', 'f360_xtrk_logging.cpp', 'f360_tracker.h']
    for file in file_list:
        if any(file_cpp in file for file_cpp in debug_file_list):
            # read file
            with open(file, 'r') as f:
                lines = f.readlines()
            updated_lines = []
            for line in lines:
                line = line.replace('#ifdef _DEBUG', '#if 1')
                updated_lines.append(line)
            # write back to file
            with open(file, 'w') as f:
                f.writelines(updated_lines)
            #print(f"updated #ifdef _DEBUG Macro to #if 1 for: {file}")

# add struct packing to 2 bytes to f360_xtrk_logging.cpp and f360_detection_hist.h
def add_packing():
    packing_file_list = ['f360_detection_hist.h', 'f360_xtrk_logging.cpp']
    struct_start_line_detection_hist = 'typedef struct F360_Detection_Hist_Data_Tag'
    struct_end_line_detection_hist = 'F360_Detection_Hist_T;'
    struct_start_line_detection_hist_props = 'typedef struct F360_Detection_Hist_Props_Tag'
    struct_end_line_detection_hist_props = 'F360_Detection_Hist_Props_T;'

    detection_hist_props_start_line_check = False
    detection_hist_props_end_line_check = False
    detection_hist_start_line_check = False
    detection_hist_end_line_check = False
    for file in file_list:
        if any(file_cpp in file for file_cpp in packing_file_list):
            if 'f360_detection_hist.h' in file:
                # read file
                with open(file, 'r') as f:
                    lines = f.readlines()
                updated_lines = []
                for line in lines:
                    if '#pragma pack(push, save_pack)' in line:
                        detection_hist_start_line_check = True
                    elif '#pragma pack(pop, save_pack)' in line:
                        detection_hist_end_line_check = True
                for line in lines:
                    if struct_start_line_detection_hist in line and detection_hist_start_line_check == False:
                        updated_lines.append('#pragma pack(push, save_pack)'+'\n')
                        updated_lines.append('#pragma pack(push, 2)'+'\n')
                    updated_lines.append(line)
                    if struct_end_line_detection_hist in line and detection_hist_end_line_check == False:
                        updated_lines.append('#pragma pack(pop, save_pack)'+'\n')  
                # write back to file
                with open(file, 'w') as f:
                    f.writelines(updated_lines)
                #print(f"updated #pragma_pack for: {file}")
            elif 'f360_xtrk_logging.cpp' in file:
                # read file
                with open(file, 'r') as f:
                    lines = f.readlines()
                updated_lines = []
                for line in lines:
                    if '#pragma pack(push, save_pack)' in line:
                        detection_hist_props_start_line_check = True
                    elif '#pragma pack(pop, save_pack)' in line:
                        detection_hist_props_end_line_check = True
                for line in lines:
                    if struct_start_line_detection_hist_props in line and detection_hist_props_start_line_check == False:
                        updated_lines.append('#pragma pack(push, save_pack)'+'\n')
                        updated_lines.append('#pragma pack(push, 2)'+'\n')
                    updated_lines.append(line)
                    if struct_end_line_detection_hist_props in line and detection_hist_props_end_line_check == False:
                        updated_lines.append('#pragma pack(pop, save_pack)'+'\n')
                # write back to file
                with open(file, 'w') as f:
                    f.writelines(updated_lines)
                #print(f"updated #pragma_pack for: {file}")

if __name__ == '__main__':
    get_file_list()
    remove_debug_macro()
    add_packing()
