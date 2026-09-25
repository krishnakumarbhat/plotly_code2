# -*- coding: utf-8 -*-
"""
Created on Created on Tue Dec 02 16:38:57 2024

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""

import os, sys
import can, cantools
import pandas as pd

dbc_file = "/net/8k3/e0fs01/irods/PLKRA-PROJECTS/HONDA-SRR6P/9-Upload/Honda-Input/DBC/SW14.0.2_Full_ver__DBC_Prod.dbc"
message_name = r"CRU_TPOBJ_HEADER"
message_id = 0xB45FF41
signal_name = r"CYCLECOUNTER_TPOBJ"


#***** function to validate input arguments *****
def validate_argv():
    
    if len(sys.argv) < 2:
        print("[ERROR] : Usage: python honda_validator.py <input_files> <output_dir>")
        sys.exit(1)

    for src_path in sys.argv[1:2]:
        if not os.path.exists(src_path):
            print(f"[ERROR] : Path does not exist: {src_path}")
            sys.exit(1)

    return sys.argv[1]


#***** function to get blf files from output path *****
def get_blf_files(oPath):
    fList = []
    for root, dirs, files in os.walk(oPath):
        for file in files:
            if file.lower().endswith('.blf'):
                fList.append(os.path.join(root, file))
    if not fList:
        print(f"[ERROR] : No BLF files found in path: {oPath}")
        sys.exit(1)
    fList.sort()
    return fList


#***** function to create blf list *****
def create_blfList(oPath, fList):
    file=f'{oPath}/honda_blf_list.txt'
    with open(file, 'w') as blfFile:
        for blf in fList:
            blfFile.write(f"{blf}\n")
    return file


#***** function to read message from dbc file *****
def read_message_from_dbc(dbc_path, message_name):
    db = cantools.database.load_file(dbc_path)
    message = db.get_message_by_name(message_name)
    if message is None:
        print(f"[ERROR] : Message '{message_name}' not found in DBC: {dbc_path}")
        sys.exit(1)
    return message

 #***** function to validate continuity of signal in blf files *****
def validate_signal_continuity(fList, msg_def):
    rows_of_interest = []
    try:
        blf_reader = can.BLFReader(fList)
        rows_out = []
        for msg in blf_reader:
            if msg.arbitration_id == message_id:
                try: decoded = msg_def.decode(msg.data)
                except Exception as e:
                    print(f"[ERROR] : Failed to decode message: {e}")
                    continue
                if signal_name not in decoded:
                    print(f"[ERROR] : Signal '{signal_name}' not found in message ID {hex(message_id)}")
                    continue
                sig_val = decoded[signal_name]
                rows_out.append({"ScanIndex": sig_val, "Channel": msg.channel})

    
        df_obj_header = pd.DataFrame(rows_out, columns=["ScanIndex", "Channel"])
        req_col = {'ScanIndex', 'Channel'}
    
        if not req_col.issubset(df_obj_header.columns):
            print(f"[ERROR] : Required columns{req_col} missing in DataFrame. Found columns: {df_obj_header.columns.tolist()}")
 
        # Clean and ensure integer types
        df = df_obj_header[["ScanIndex", "Channel"]].dropna().copy()
        df["ScanIndex"] = df["ScanIndex"].astype(int)
        df['Channel'] = df['Channel'].astype(str)

        
        channels = sorted(df["Channel"].unique())

        for ch in channels:
            sub = df.loc[df["Channel"] == ch, "ScanIndex"].sort_values().unique()
            if len(sub) > 0:
                missing = sorted(set(range(sub[0], sub[-1] + 1)) - set(sub))
                rows_of_interest.append({
                    "filename": fList.split('Honda-Input/')[-1],
                    "Channel": ch,
                    "continuous": len(missing) == 0,
                    "missing_data": missing,
                    "missing_freq": len(missing),
                    "min_scan": int(sub[0]),
                    "max_scan": int(sub[-1]),
                    "duration(sec)": round(0.05 * (int(sub[-1]) - int(sub[0])),2)
                })
            else:
                rows_of_interest.append({
                    "filename": fList.split('Honda-Input/')[-1],
                    "Channel": ch,
                    "continuous": False,
                    "missing_data": [],
                    "missing_freq" : None,
                    "min_scan": None,
                    "max_scan": None,
                    "duration(sec)": None
                })
        #print(rows_of_interest)
    except Exception as e:
        print(f"[WARNING] : {e} : {fList.split('Honda-Input/')[-1]}")
        rows_of_interest.append({
            "filename": fList.split('Honda-Input/')[-1],
            "Channel": None,
            "continuous": None,
            "missing_data": None,
            "missing_freq" : None,
            "min_scan": None,
            "max_scan": None,
            "duration(sec)": None
        })

    return pd.DataFrame(rows_of_interest)


#***** main functions body *****
if __name__ == "__main__":
    oPath = validate_argv()
    csv_file = f"{oPath}/honda_signal_continuity.csv"
    if os.path.exists(csv_file): 
        os.remove(csv_file); 
        print(f"[INFO] : Existing output file removed: {csv_file}")
    fList = get_blf_files(oPath)
    #blfList = create_blfList(oPath,fList)
    msg_def = read_message_from_dbc(dbc_file, message_name)

    for i, blf_file in enumerate(fList, start=1):
        print(f"[INFO] : Processing file {i}/{len(fList)}: {blf_file.split('Honda-Input/')[-1]}",end='\r')
        missing_df = validate_signal_continuity(blf_file, msg_def)
        missing_df.to_csv(csv_file, mode="a", index=False, header=not os.path.exists(csv_file))
    print("\n[INFO] : Processing completed.")
    