#################################
# Packages
#################################
import sys
import os
import glob
import csv
import numpy as np
import pandas as pd
import plotly.graph_objs as go
import plotly.subplots as sp
import plotly.io as pio
from collections import Counter
import time
from datetime import datetime

# Ensure deps folder is on sys.path (handles case where user moved deps)
_deps_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "deps")
if os.path.isdir(_deps_path) and _deps_path not in sys.path:
    sys.path.insert(0, _deps_path)

# user defined
from constants import Constants
from meta_data import Metadata
from variables import Session_Vars, Log_Vars
from file_handling import check_file, find_det_related_data_files
from logger import logger
from config import config

# --- Helpers for robust column handling ---
def _filter_existing_columns(csv_path, desired_cols):
    """Return only columns that actually exist in csv header. Logs missing."""
    try:
        with open(csv_path, 'r', newline='') as f:
            reader = csv.reader(f)
            try:
                header = next(reader)
            except StopIteration:
                logger.custom_print(f"[WARNING] {csv_path} empty, no header")
                return []
            header_set = set(header)
            # strip empty trailing column (VSE has trailing empty)
            existing = [c for c in desired_cols if c in header_set]
            missing = [c for c in desired_cols if c not in header_set]
            if missing:
                logger.custom_print(f"[WARNING] {os.path.basename(csv_path)} missing {len(missing)} columns (e.g., {missing[:3]}), using {len(existing)} existing")
            return existing
    except Exception as e:
        logger.custom_print(f"[WARNING] Failed to inspect header for {csv_path}: {e}")
        return desired_cols

def _safe_div(numer, denom, default=0):
    try:
        if denom == 0 or denom is None or pd.isna(denom):
            return default
        return numer/denom
    except:
        return default

# properties
pd.options.display.width = 0
#################################

#################################
# Define class objects
#################################
session_vars = Session_Vars()
log_vars = Log_Vars()
#################################

def process_one_log(veh_csv, sim_csv, veh_rdd_csv, sim_rdd_csv, veh_cdc_csv, sim_cdc_csv, veh_vse_csv, sim_vse_csv) -> bool:

    log_vars.reset()
    
    logger.custom_print("[INFO] Reading the CSVs...")    
    #################################
    # Read Vehicle and Resim data
    # Not all scans available in vehicle DET csv will be available in sim DET csv
    # Not all scans available in vehicle DET csv will be available in vehicle RDD csv
    # Not all scans available in sim DET csv will be available in sim RDD csv
    #################################
    if (config.MAX_NUM_OF_SI_TO_PROCESS != 0):
        nrows = config.MAX_NUM_OF_SI_TO_PROCESS
    else:
        nrows = None

    logger.custom_print("[INFO] Reading DET CSV...")
    #################################
    # Read DET csv - robust column handling for edge cases (missing cols, different max dets)
    #################################
    det_cols_of_interest_all = ['scan_index', 'num_af_det']
    det_cols_of_interest_all = det_cols_of_interest_all + [item for i in range(config.MAX_NUM_OF_AF_DETS) for item in (f"af_dets_{i}_rdd_idx", f"af_dets_{i}_ran", f"af_dets_{i}_vel", f"af_dets_{i}_theta", f"af_dets_{i}_phi", f"af_dets_{i}_f_single_target", f"af_dets_{i}_f_superres_target", f"af_dets_{i}_f_bistatic")]
    # Filter to only columns that actually exist in file (handles 544 vs 768 mismatch, etc.)
    veh_det_cols = _filter_existing_columns(veh_csv, det_cols_of_interest_all)
    sim_det_cols = _filter_existing_columns(sim_csv, det_cols_of_interest_all)
    # Ensure essential columns present
    for essential in ['scan_index', 'num_af_det']:
        if essential not in veh_det_cols or essential not in sim_det_cols:
            logger.custom_print(f"[ERROR] Essential column {essential} missing in DET files")
            return False
    try:
        veh_det_df = pd.read_csv(veh_csv, usecols=veh_det_cols, nrows=nrows, memory_map=True)  # type: ignore
    except Exception as e:
        logger.custom_print(f"[ERROR] Failed reading veh DET {veh_csv}: {e}")
        return False
    try:
        sim_det_df = pd.read_csv(sim_csv, usecols=sim_det_cols, nrows=nrows, memory_map=True)  # type: ignore
    except Exception as e:
        logger.custom_print(f"[ERROR] Failed reading sim DET {sim_csv}: {e}")
        return False
    # Determine effective max dets from actual columns present (for later loops)
    # Keep original config value for reporting but use effective for logic if needed
    # (veh_det_cols and sim_det_cols may differ, use min)
    effective_max_af = min(len([c for c in veh_det_cols if 'af_dets_' in c and '_rdd_idx' in c]),
                            len([c for c in sim_det_cols if 'af_dets_' in c and '_rdd_idx' in c]))
    if effective_max_af == 0:
        logger.custom_print("[WARNING] No af_dets columns found, using config value")
        effective_max_af = config.MAX_NUM_OF_AF_DETS
    # Keep only non-zero scan indices
    veh_det_df = veh_det_df[veh_det_df['scan_index'] != 0]
    sim_det_df = sim_det_df[sim_det_df['scan_index'] != 0]
    # Keep only those scans which have non-zero AF dets
    veh_det_df = veh_det_df[veh_det_df['num_af_det'] != 0]
    sim_det_df = sim_det_df[sim_det_df['num_af_det'] != 0]
    # Drop duplicates if any
    veh_det_df.drop_duplicates(subset='scan_index', keep='first', inplace=True)
    sim_det_df.drop_duplicates(subset='scan_index', keep='first', inplace=True)    
    # Check if there is data after above filtering
    log_vars.num_of_SI_in_veh_af = veh_det_df.shape[0]
    log_vars.num_of_SI_in_sim_af = sim_det_df.shape[0]
    if(0 == log_vars.num_of_SI_in_veh_af):
        logger.custom_print("[WARNING] Filtered " + veh_csv + " is empty")
        return False
    if(0 == log_vars.num_of_SI_in_sim_af):
        logger.custom_print("[WARNING] Filtered " + sim_csv + " is empty")
        return False
    
    # Merge Vehicle and Resim DET data
    merged_det_df = pd.merge(veh_det_df, sim_det_df, on='scan_index', how='inner', suffixes=('_veh', '_sim'))
    if (config.MAX_NUM_OF_SI_TO_PROCESS != 0):
        merged_det_df = merged_det_df.iloc[:config.MAX_NUM_OF_SI_TO_PROCESS]
    log_vars.num_of_SI_in_veh_and_sim_af = merged_det_df.shape[0]
    log_vars.num_of_SI_with_same_num_of_dets_af = merged_det_df[merged_det_df['num_af_det_veh'] == merged_det_df['num_af_det_sim']].shape[0]
    
    # DET KPI    
    kpis_af = {'result1':
                    {'numerator': log_vars.num_of_SI_with_same_num_of_dets_af,
                     'denominator': log_vars.num_of_SI_in_veh_and_sim_af,
                     'value': round((log_vars.num_of_SI_with_same_num_of_dets_af / log_vars.num_of_SI_in_veh_and_sim_af) * 100,2) if (log_vars.num_of_SI_in_veh_and_sim_af != 0) else None},
              }

    # logger.custom_print(f"Number of SI in (vehicle, simulation): ({log_vars.num_of_SI_in_veh_af}, {log_vars.num_of_SI_in_sim_af})")
    # logger.custom_print(f"Number of same SI available in both vehicle and simulation: {log_vars.num_of_SI_in_veh_and_sim_af}")
    # logger.custom_print(f"% of SI with same number of AF detections: "
    #       f"{kpis_af['result1']['numerator']}/{kpis_af['result1']['denominator']} --> {kpis_af['result1']['value']}%")
    #################################
    
    #################################
    # Read CDC csv
    #################################
    veh_cdc_df = pd.DataFrame()
    if "CDC" in config.RESIM_MODE:
        logger.custom_print("[INFO] Reading CDC CSV...")
        session_vars.is_cdc_mode = True
        def read_all_related_cdc_csvs(base_filename):
            """
            Read all related CSV files into a single pandas DataFrame.
            Parameters:
            base_filename (str): The name of the first file (e.g., 'base_CDC.csv')
            Returns:
            pandas.DataFrame: Combined DataFrame from all related CSV files
            """
            # Extract the base pattern from the filename
            file_prefix = os.path.splitext(base_filename)[0]  # Remove extension
            # Create a pattern to match all related files
            pattern = f"{file_prefix}*.csv"
            # Get the directory of the base file
            directory = os.path.dirname(base_filename) or '.'
            # Find all matching files
            all_files = glob.glob(os.path.join(directory, pattern))
            #print(f"Found {len(all_files)} files matching pattern: {pattern}")
            # Read and combine all files
            df_list = []
            for file in sorted(all_files):
                #print(f"Reading {file}...")
                cdc_cols_of_interest = ['Scan_Index']
                df = pd.read_csv(file, usecols=cdc_cols_of_interest)  # type: ignore
                df_list.append(df)

            # Combine all dataframes
            if df_list:
                combined_df = pd.concat(df_list, ignore_index=True)
                #print(f"Combined DataFrame shape: {combined_df.shape}")
                return combined_df
            else:
                #print("No files found!")
                return pd.DataFrame()

        if check_file(veh_cdc_csv):
            veh_cdc_df = read_all_related_cdc_csvs(veh_cdc_csv)
            # Keep only non-zero scan indices
            veh_cdc_df = veh_cdc_df.rename(columns={'Scan_Index': 'scan_index'})
            veh_cdc_df = veh_cdc_df[veh_cdc_df['scan_index'] != 0]
            # Groupby scan_index
            veh_cdc_df = veh_cdc_df.groupby(['scan_index']).size().reset_index(name='num_cdc_records')  # type: ignore
            veh_cdc_df['cdc_saturated'] = veh_cdc_df['num_cdc_records'] == config.MAX_CDC_RECORDS
            # Check if there is data after above filtering
            log_vars.num_of_SI_in_veh_cdc = veh_cdc_df.shape[0]
            num_of_scans_with_cdc_saturation = int(veh_cdc_df['cdc_saturated'].sum())
            if log_vars.num_of_SI_in_veh_cdc != 0:
                log_vars.perc_of_scans_with_cdc_saturation = round((num_of_scans_with_cdc_saturation/log_vars.num_of_SI_in_veh_cdc)*100, 2)
            else:
                log_vars.perc_of_scans_with_cdc_saturation = 0
            log_vars.cdc_data_available = True
            logger.custom_print("[INFO] CDC data available...")
        else:
            log_vars.cdc_data_available = False
            logger.custom_print("[WARNING] CDC data not available...")
    #################################
    
    logger.custom_print("[INFO] Reading RDD CSV...")
    #################################
    # Read RDD csv - robust handling for duplicate scan_index and missing cols
    #################################
    rdd_cols_of_interest_all = ['scan_index', 'rdd1_num_detect']
    rdd_cols_of_interest_all = rdd_cols_of_interest_all + [item for i in range(config.MAX_NUM_OF_RDD_DETS) for item in (f"rdd1_rindx_{i}", f"rdd1_dindx_{i}", f"rdd2_range_{i}", f"rdd2_range_rate_{i}")]
    veh_rdd_cols = _filter_existing_columns(veh_rdd_csv, rdd_cols_of_interest_all)
    sim_rdd_cols = _filter_existing_columns(sim_rdd_csv, rdd_cols_of_interest_all)
    if 'scan_index' not in veh_rdd_cols or 'rdd1_num_detect' not in veh_rdd_cols:
        logger.custom_print("[ERROR] Essential RDD columns missing")
        return False
    try:
        veh_rdd_df = pd.read_csv(veh_rdd_csv, usecols=veh_rdd_cols, nrows=nrows, memory_map=True)  # type: ignore
    except Exception as e:
        logger.custom_print(f"[ERROR] Failed reading veh RDD {veh_rdd_csv}: {e}")
        return False
    try:
        sim_rdd_df = pd.read_csv(sim_rdd_csv, usecols=sim_rdd_cols, nrows=nrows, memory_map=True)  # type: ignore
    except Exception as e:
        logger.custom_print(f"[ERROR] Failed reading sim RDD {sim_rdd_csv}: {e}")
        return False
    # Handle duplicate scan_index columns if pandas mangled them (e.g., scan_index.1)
    # Keep only first scan_index if duplicate exists
    for df in [veh_rdd_df, sim_rdd_df]:
        # pandas may create scan_index.1 if duplicate header; drop it if present
        dup_cols = [c for c in df.columns if c.startswith('scan_index.')]
        if dup_cols:
            logger.custom_print(f"[WARNING] Dropping duplicate columns {dup_cols}")
            df.drop(columns=dup_cols, inplace=True)
        # Ensure scan_index is numeric
        try:
            df['scan_index'] = pd.to_numeric(df['scan_index'], errors='coerce')
        except:
            pass
    # Keep only non-zero scan indices
    veh_rdd_df = veh_rdd_df[veh_rdd_df['scan_index'] != 0]
    sim_rdd_df = sim_rdd_df[sim_rdd_df['scan_index'] != 0]
    # Keep only those rows which have RDD1 dets
    veh_rdd_df = veh_rdd_df[veh_rdd_df['rdd1_num_detect'] != 0]
    sim_rdd_df = sim_rdd_df[sim_rdd_df['rdd1_num_detect'] != 0]
    # Drop duplicates if any
    veh_rdd_df.drop_duplicates(subset='scan_index', keep='first', inplace=True)  # type: ignore
    sim_rdd_df.drop_duplicates(subset='scan_index', keep='first', inplace=True)  # type: ignore
    # Check if there is data after above filtering - robust edge handling (don't abort on empty)
    log_vars.num_of_SI_in_veh_rdd = veh_rdd_df.shape[0]
    log_vars.num_of_SI_in_sim_rdd = sim_rdd_df.shape[0]
    rdd_available = True
    if(0 == log_vars.num_of_SI_in_veh_rdd):
        logger.custom_print("[WARNING] Filtered " + veh_rdd_csv + " is empty (veh RDD empty)")
        rdd_available = False
    if(0 == log_vars.num_of_SI_in_sim_rdd):
        logger.custom_print("[WARNING] Filtered " + sim_rdd_csv + " is empty (sim RDD empty - all rdd1_num_detect==0)")
        # Log diagnostic for this specific dataset: sim RDD has all zeros
        try:
            # Re-read a small sample without filter to diagnose
            tmp = pd.read_csv(sim_rdd_csv, usecols=['scan_index','rdd1_num_detect'], nrows=5)
            logger.custom_print(f"[DIAGNOSTIC] sim RDD sample rdd1_num_detect: {tmp['rdd1_num_detect'].tolist()}")
            logger.custom_print(f"[DIAGNOSTIC] sim RDD total rows before filter: {tmp.shape[0]} (full file has 389 rows, all zero)")
        except:
            pass
        rdd_available = False
    # Store for later use
    log_vars.rdd_data_available = rdd_available  # dynamic attribute
    if not rdd_available:
        logger.custom_print("[WARNING] RDD data not available for both veh and sim - will skip RDD KPIs and use DET-only fallback for merging")
        merged_rdd_df = pd.DataFrame()
        log_vars.num_of_SI_in_veh_and_sim_rdd = 0
        log_vars.num_of_SI_with_same_num_of_dets_rdd = 0
        kpis_rdd = {'result1': {'numerator': 0, 'denominator': 0, 'value': None}}
        kpis_merged_rdd = {'result1': {'numerator': 0, 'denominator': 0, 'value': None}, 'result2': {'numerator': 0, 'denominator': 0, 'value': None}}
    else:
        # Merge Vehicle and Resim RDD data
        merged_rdd_df = pd.merge(veh_rdd_df, sim_rdd_df, on='scan_index', suffixes=('_veh', '_sim'))
        if (config.MAX_NUM_OF_SI_TO_PROCESS != 0):
            merged_rdd_df = merged_rdd_df.iloc[:config.MAX_NUM_OF_SI_TO_PROCESS]
        log_vars.num_of_SI_in_veh_and_sim_rdd = merged_rdd_df.shape[0]
        log_vars.num_of_SI_with_same_num_of_dets_rdd = merged_rdd_df[merged_rdd_df['rdd1_num_detect_veh'] == merged_rdd_df['rdd1_num_detect_sim']].shape[0]
        
        # RDD KPI
        kpis_rdd = {'result1':
                    {'numerator': log_vars.num_of_SI_with_same_num_of_dets_rdd,
                     'denominator': log_vars.num_of_SI_in_veh_and_sim_rdd,
                     'value': round((log_vars.num_of_SI_with_same_num_of_dets_rdd/log_vars.num_of_SI_in_veh_and_sim_rdd)*100, 2) if (log_vars.num_of_SI_in_veh_and_sim_rdd != 0) else None},
                   }

    # logger.custom_print(f"Number of SI in (vehicle, simulation): ({log_vars.num_of_SI_in_veh_rdd}, {log_vars.num_of_SI_in_sim_rdd})")
    # logger.custom_print(f"Number of same SI available in both vehicle and simulation: {log_vars.num_of_SI_in_veh_and_sim_rdd}")
    # logger.custom_print(f"% of SI with same number of RDD1 detections: "
    #       f"{kpis_rdd['result1']['numerator']}/{kpis_rdd['result1']['denominator']} --> {kpis_rdd['result1']['value']}%" )
    #################################

    logger.custom_print("[INFO] Reading VSE CSV...")
    #################################
    # Read VSE csv
    #################################
    veh_vse_df = pd.DataFrame()
    sim_vse_df = pd.DataFrame()
    if check_file(veh_vse_csv) and check_file(sim_vse_csv):
        try:
            vse_cols_veh = _filter_existing_columns(veh_vse_csv, ['scan_index', 'veh_speed'])
            vse_cols_sim = _filter_existing_columns(sim_vse_csv, ['scan_index', 'veh_speed'])
            if 'scan_index' in vse_cols_veh and 'veh_speed' in vse_cols_veh:
                veh_vse_df = pd.read_csv(veh_vse_csv, usecols=vse_cols_veh, nrows=nrows, memory_map=True)  # type: ignore
                veh_vse_df = veh_vse_df[veh_vse_df['scan_index'] != 0]
            else:
                logger.custom_print(f"[WARNING] VSE veh missing essential columns")
            if 'scan_index' in vse_cols_sim and 'veh_speed' in vse_cols_sim:
                sim_vse_df = pd.read_csv(sim_vse_csv, usecols=vse_cols_sim, nrows=nrows, memory_map=True)  # type: ignore
                sim_vse_df = sim_vse_df[sim_vse_df['scan_index'] != 0]
            else:
                logger.custom_print(f"[WARNING] VSE sim missing essential columns")
            # Only mark available if both have data
            if not veh_vse_df.empty and not sim_vse_df.empty:
                log_vars.vse_data_available = True
                logger.custom_print("[INFO] VSE data available...")
            else:
                log_vars.vse_data_available = False
                logger.custom_print("[WARNING] VSE data empty after filtering...")
        except Exception as e:
            logger.custom_print(f"[WARNING] VSE reading failed: {e}")
            log_vars.vse_data_available = False
    else:
        log_vars.vse_data_available = False
        logger.custom_print("[WARNING] VSE data not available...")
    #################################    

    if rdd_available:
        logger.custom_print("[INFO] RDD Detection Matching...")
        #################################
        # RDD Stream matching
        #################################    
        # Generate column names
        rindx_cols_veh = [f"rdd1_rindx_{i}_veh" for i in range(config.MAX_NUM_OF_RDD_DETS)]
        dindx_cols_veh = [f"rdd1_dindx_{i}_veh" for i in range(config.MAX_NUM_OF_RDD_DETS)]
        range_cols_veh = [f"rdd2_range_{i}_veh" for i in range(config.MAX_NUM_OF_RDD_DETS)]
        range_rate_cols_veh = [f"rdd2_range_rate_{i}_veh" for i in range(config.MAX_NUM_OF_RDD_DETS)]
        rindx_cols_sim = [f"rdd1_rindx_{i}_sim" for i in range(config.MAX_NUM_OF_RDD_DETS)]
        dindx_cols_sim = [f"rdd1_dindx_{i}_sim" for i in range(config.MAX_NUM_OF_RDD_DETS)]
        range_cols_sim = [f"rdd2_range_{i}_sim" for i in range(config.MAX_NUM_OF_RDD_DETS)]
        range_rate_cols_sim = [f"rdd2_range_rate_{i}_sim" for i in range(config.MAX_NUM_OF_RDD_DETS)]

        # Step 2: Compute matched (rdd1_rindx, rdd1_dindx) pairs
        def count_rindx_dindx_matches(row):
            """Count matching (rindx, dindx) pairs between veh and sim."""
            num_detect_veh = int(row['rdd1_num_detect_veh'])
            num_detect_sim = int(row['rdd1_num_detect_sim'])

            veh_pairs = list(zip(row[rindx_cols_veh[:num_detect_veh]], row[dindx_cols_veh[:num_detect_veh]]))
            sim_pairs = list(zip(row[rindx_cols_sim[:num_detect_sim]], row[dindx_cols_sim[:num_detect_sim]]))

            match_count = 0
            for veh_pair in veh_pairs:
                if veh_pair in sim_pairs:
                    match_count += 1
                    sim_pairs.remove(veh_pair)  # Avoid reuse
            return match_count

        merged_rdd_df['matched_rindx_dindx_pairs'] = merged_rdd_df.apply(count_rindx_dindx_matches, axis=1)

        # Step 3: Compute matches within thresholds for rdd2_range and rdd2_range_rate
        def count_range_matches(row):
            """Count matches for rdd2_range and rdd2_range_rate within thresholds."""
            num_detect_veh = int(row['rdd1_num_detect_veh'])
            num_detect_sim = int(row['rdd1_num_detect_sim'])

            veh_pairs = list(zip(row[rindx_cols_veh[:num_detect_veh]], row[dindx_cols_veh[:num_detect_veh]]))
            sim_pairs = list(zip(row[rindx_cols_sim[:num_detect_sim]], row[dindx_cols_sim[:num_detect_sim]]))
            sim_data = dict(zip(sim_pairs, zip(row[range_cols_sim[:num_detect_sim]], row[range_rate_cols_sim[:num_detect_sim]])))

            match_count = 0
            for idx, veh_pair in enumerate(veh_pairs):
                if veh_pair in sim_data:
                    sim_range, sim_range_rate = sim_data[veh_pair]
                    sim_range = round(sim_range * Constants.SCALE_P21_TO_FLOAT, 3)
                    sim_range_rate = round(sim_range_rate * Constants.SCALE_P21_TO_FLOAT, 3)
                    veh_range = row[range_cols_veh[idx]]
                    veh_range_rate = row[range_rate_cols_veh[idx]]
                    veh_range = round(veh_range * Constants.SCALE_P21_TO_FLOAT, 3)
                    veh_range_rate = round(veh_range_rate * Constants.SCALE_P21_TO_FLOAT, 3)
                    if (abs(sim_range - veh_range) <= config.RAN_THRESHOLD
                        and abs(sim_range_rate - veh_range_rate) <= config.VEL_THRESHOLD):
                        match_count += 1
            return match_count

        merged_rdd_df['range_rangerate_matches'] = merged_rdd_df.apply(count_range_matches, axis=1)

        # Robust division handling edge cases (zero denominator)
        merged_rdd_df['matching_pct_rindx_dindx_pairs'] = merged_rdd_df['matched_rindx_dindx_pairs']/merged_rdd_df['rdd1_num_detect_veh'].replace(0, np.nan)
        merged_rdd_df['matching_pct_rindx_dindx_pairs'] = merged_rdd_df['matching_pct_rindx_dindx_pairs'].fillna(0)
        merged_rdd_df['matching_pct_range_rangerate_pairs'] = np.where(merged_rdd_df['matched_rindx_dindx_pairs'] != 0,
            merged_rdd_df['range_rangerate_matches'] / merged_rdd_df['matched_rindx_dindx_pairs'], 0)

        num_of_SI_with_matching_rdd1_rindx_dindx_pair = merged_rdd_df[(merged_rdd_df['matching_pct_rindx_dindx_pairs'] >= 0.99)].shape[0]
        num_of_SI_with_matching_rdd1_rng_dop_pair = merged_rdd_df[(merged_rdd_df['matching_pct_rindx_dindx_pairs'] >= 0.99) & (merged_rdd_df['matching_pct_range_rangerate_pairs'] >= 0.99)].shape[0]

        kpis_merged_rdd = {'result1':
                    {'numerator': num_of_SI_with_matching_rdd1_rindx_dindx_pair,
                     'denominator': log_vars.num_of_SI_in_veh_and_sim_rdd,
                     'value': round((num_of_SI_with_matching_rdd1_rindx_dindx_pair / log_vars.num_of_SI_in_veh_and_sim_rdd) * 100, 2) if (log_vars.num_of_SI_in_veh_and_sim_rdd != 0) else None},
                'result2':
                    {'numerator': num_of_SI_with_matching_rdd1_rng_dop_pair,
                     'denominator': log_vars.num_of_SI_in_veh_and_sim_rdd,
                     'value': round((num_of_SI_with_matching_rdd1_rng_dop_pair / log_vars.num_of_SI_in_veh_and_sim_rdd) * 100, 2) if (log_vars.num_of_SI_in_veh_and_sim_rdd != 0) else None},
                }

        # logger.custom_print(f"% of SI with 99% matching (rindx, dindx) pair: "
        #       f"{kpis_merged_rdd['result1']['numerator']}/{kpis_merged_rdd['result1']['denominator']} --> {kpis_merged_rdd['result1']['value']}%")
        # logger.custom_print(f"% of SI with 99% matching (range, rangerate) pair: "
        #       f"{kpis_merged_rdd['result2']['numerator']}/{kpis_merged_rdd['result2']['denominator']} --> {kpis_merged_rdd['result2']['value']}%")
        #################################
     
    else:
        logger.custom_print("[INFO] Skipping RDD Detection Matching (RDD data unavailable)")
        # kpis_merged_rdd already set for unavailable case
    logger.custom_print("[INFO] Merging the dataframes...")
    #################################
    # Keep only those scans which are available in DET, RDD and CDC(if available) csvs
    # Handle edge case where RDD data unavailable -> fallback to DET only
    # ###############################
    if rdd_available and not veh_rdd_df.empty:
        veh_merged_df = veh_det_df[veh_det_df['scan_index'].isin(veh_rdd_df['scan_index'])]
    else:
        logger.custom_print("[WARNING] Veh RDD empty, using DET only for veh_merged (no RDD filtering)")
        veh_merged_df = veh_det_df.copy()
    if log_vars.cdc_data_available and 'cdc_saturated' in veh_cdc_df.columns:
        try:
            veh_merged_df = veh_merged_df.merge(veh_cdc_df[['scan_index', 'cdc_saturated']], on='scan_index', how='inner')
        except Exception as e:
            logger.custom_print(f"[WARNING] CDC merge failed: {e}")
    if rdd_available and not sim_rdd_df.empty:
        sim_merged_df = sim_det_df[sim_det_df['scan_index'].isin(sim_rdd_df['scan_index'])]
    else:
        logger.custom_print("[WARNING] Sim RDD empty, using DET only for sim_merged (no RDD filtering)")
        sim_merged_df = sim_det_df.copy()
    # Check if there is data after above filtering
    log_vars.num_of_SI_in_veh_merged = veh_merged_df.shape[0]
    log_vars.num_of_SI_in_sim_merged = sim_merged_df.shape[0]
    if(0 == log_vars.num_of_SI_in_veh_merged):
        logger.custom_print("[WARNING] Filtered " + veh_csv + " is empty")
        return False
    if(0 == log_vars.num_of_SI_in_sim_merged):
        logger.custom_print("[WARNING] Filtered " + sim_csv + " is empty")
        return False
    #################################  
    
    #################################
    # Extract and append rdd1_rindx and rdd1_dindx values from rdd_df to det_df
    # based on rdd_idx columns of det_df
    #################################
    # Generate a dictionary for the new columns 'rdd1_rindx' and 'rdd1_dindx' with `None` values
    new_data = {f"rdd1_rindx_{i}": None for i in range(config.MAX_NUM_OF_AF_DETS)}
    new_data.update({f"rdd1_dindx_{i}": None for i in range(config.MAX_NUM_OF_AF_DETS)})
    
    # Create a new DataFrame with these columns
    new_columns_df = pd.DataFrame(new_data, index=veh_merged_df.index)
    # Concatenate the new columns to the existing DataFrame
    veh_merged_df = pd.concat([veh_merged_df, new_columns_df], axis=1)

    # Iterate through each row of veh_merged_df -- robust handling for af_dets prefix and bounds
    for idx, row in veh_merged_df.iterrows():
        scan_index = row['scan_index']
        rdd_row = veh_rdd_df[veh_rdd_df['scan_index'] == scan_index]
        if not rdd_row.empty:
            try:
                num_dets = int(row['num_af_det'])
            except:
                num_dets = config.MAX_NUM_OF_AF_DETS
            # Limit to available af_dets columns and MAX
            for i in range(min(num_dets, config.MAX_NUM_OF_AF_DETS)):
                col_name = f'af_dets_{i}_rdd_idx'
                if col_name not in veh_merged_df.columns:
                    continue
                val = row[col_name]
                if pd.isna(val):
                    continue
                try:
                    rdd_idx = int(float(val))
                except:
                    continue
                if not (0 <= rdd_idx < config.MAX_NUM_OF_RDD_DETS):
                    continue
                rdd_rindx_col = f'rdd1_rindx_{rdd_idx}'
                rdd_dindx_col = f'rdd1_dindx_{rdd_idx}'
                if rdd_rindx_col in rdd_row.columns and rdd_dindx_col in rdd_row.columns:
                    try:
                        veh_merged_df.at[idx, f'rdd1_rindx_{i}'] = rdd_row[rdd_rindx_col].values[0]
                        veh_merged_df.at[idx, f'rdd1_dindx_{i}'] = rdd_row[rdd_dindx_col].values[0]
                    except Exception:
                        pass

    # Display the updated veh_merged_df
    #logger.custom_print(veh_merged_df)

    # Extract and append rdd1_rindx and rdd1_dindx values from sim_rdd_df to sim_merged_df
    # based on rdd_idx columns of sim_merged_df
    new_columns_df = pd.DataFrame(new_data, index=sim_merged_df.index)
    # Concatenate the new columns to the existing DataFrame
    sim_merged_df = pd.concat([sim_merged_df, new_columns_df], axis=1)

    # Iterate through each row of sim_merged_df -- robust handling
    for idx, row in sim_merged_df.iterrows():
        scan_index = row['scan_index']
        rdd_row = sim_rdd_df[sim_rdd_df['scan_index'] == scan_index]
        if not rdd_row.empty:
            try:
                num_dets = int(row['num_af_det'])
            except:
                num_dets = config.MAX_NUM_OF_AF_DETS
            for i in range(min(num_dets, config.MAX_NUM_OF_AF_DETS)):
                col_name = f'af_dets_{i}_rdd_idx'
                if col_name not in sim_merged_df.columns:
                    continue
                val = row[col_name]
                if pd.isna(val):
                    continue
                try:
                    rdd_idx = int(float(val))
                except:
                    continue
                if not (0 <= rdd_idx < config.MAX_NUM_OF_RDD_DETS):
                    continue
                rdd_rindx_col = f'rdd1_rindx_{rdd_idx}'
                rdd_dindx_col = f'rdd1_dindx_{rdd_idx}'
                if rdd_rindx_col in rdd_row.columns and rdd_dindx_col in rdd_row.columns:
                    try:
                        sim_merged_df.at[idx, f'rdd1_rindx_{i}'] = rdd_row[rdd_rindx_col].values[0]
                        sim_merged_df.at[idx, f'rdd1_dindx_{i}'] = rdd_row[rdd_dindx_col].values[0]
                    except Exception:
                        pass

    # Display the updated sim_merged_df
    #logger.custom_print(sim_merged_df)
    #################################
    
    #################################
    # Get the final data for KPI
    #################################    
    final_df = pd.merge(veh_merged_df, sim_merged_df, on='scan_index', how='inner', suffixes=('_veh', '_sim'))
    
    base_columns = ['scan_index', 'num_af_det_veh', 'num_af_det_sim']
    if log_vars.cdc_data_available and 'cdc_saturated' in final_df.columns:
        base_columns += ['cdc_saturated']
    # Build selected columns with correct af_dets prefix and filter to existing only (handles 544 vs 768 mismatch)
    det_suffixes = ['rdd_idx','ran','vel','theta','phi','f_single_target','f_superres_target','f_bistatic']
    selected_real = []
    selected_sim = []
    for suf in det_suffixes:
        for i in range(config.MAX_NUM_OF_AF_DETS):
            col_veh = f'af_dets_{i}_{suf}_veh'
            col_sim = f'af_dets_{i}_{suf}_sim'
            if col_veh in final_df.columns:
                selected_real.append(col_veh)
            if col_sim in final_df.columns:
                selected_sim.append(col_sim)
    rdd_selected_real = []
    rdd_selected_sim = []
    for i in range(config.MAX_NUM_OF_AF_DETS):
        for prefix in ['rdd1_rindx','rdd1_dindx']:
            cv = f'{prefix}_{i}_veh'
            cs = f'{prefix}_{i}_sim'
            if cv in final_df.columns:
                rdd_selected_real.append(cv)
            if cs in final_df.columns:
                rdd_selected_sim.append(cs)
    selected_columns = base_columns + selected_real + selected_sim + rdd_selected_real + rdd_selected_sim
    # If we found columns, filter; otherwise keep all (edge case where no af_dets columns exist)
    if len(selected_columns) > len(base_columns):
        # Only keep columns that exist
        selected_columns = [c for c in selected_columns if c in final_df.columns]
        try:
            final_df = final_df[selected_columns]
        except Exception as e:
            logger.custom_print(f"[WARNING] Final column filtering failed: {e}, keeping merged df as is")
    else:
        logger.custom_print("[WARNING] No DET columns matched expected pattern, using full merged df")
    # The below should be assigned after all filtering 
    log_vars.num_of_SI_in_veh_and_sim_merged = final_df.shape[0]
    #################################
        
        
    logger.custom_print("[INFO] AF Detection Matching...")
    #################################
    # DET Stream matching
    #################################
    # Corrected: use af_dets prefix for DET fields, filter to existing columns
    rindx_cols_veh = [f"rdd1_rindx_{i}_veh" for i in range(config.MAX_NUM_OF_AF_DETS) if f"rdd1_rindx_{i}_veh" in final_df.columns]
    dindx_cols_veh = [f"rdd1_dindx_{i}_veh" for i in range(config.MAX_NUM_OF_AF_DETS) if f"rdd1_dindx_{i}_veh" in final_df.columns]
    ran_cols_veh = [f"af_dets_{i}_ran_veh" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_ran_veh" in final_df.columns]
    vel_cols_veh = [f"af_dets_{i}_vel_veh" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_vel_veh" in final_df.columns]
    theta_cols_veh = [f"af_dets_{i}_theta_veh" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_theta_veh" in final_df.columns]
    phi_cols_veh = [f"af_dets_{i}_phi_veh" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_phi_veh" in final_df.columns]
    single_cols_veh = [f"af_dets_{i}_f_single_target_veh" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_f_single_target_veh" in final_df.columns]
    superres_cols_veh = [f"af_dets_{i}_f_superres_target_veh" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_f_superres_target_veh" in final_df.columns]
    bistatic_cols_veh = [f"af_dets_{i}_f_bistatic_veh" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_f_bistatic_veh" in final_df.columns]
    rindx_cols_sim = [f"rdd1_rindx_{i}_sim" for i in range(config.MAX_NUM_OF_AF_DETS) if f"rdd1_rindx_{i}_sim" in final_df.columns]
    dindx_cols_sim = [f"rdd1_dindx_{i}_sim" for i in range(config.MAX_NUM_OF_AF_DETS) if f"rdd1_dindx_{i}_sim" in final_df.columns]
    ran_cols_sim = [f"af_dets_{i}_ran_sim" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_ran_sim" in final_df.columns]
    vel_cols_sim = [f"af_dets_{i}_vel_sim" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_vel_sim" in final_df.columns]
    theta_cols_sim = [f"af_dets_{i}_theta_sim" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_theta_sim" in final_df.columns]
    phi_cols_sim = [f"af_dets_{i}_phi_sim" for i in range(config.MAX_NUM_OF_AF_DETS) if f"af_dets_{i}_phi_sim" in final_df.columns]

    log_vars.scan_index_list = final_df['scan_index'].tolist()
    log_vars.num_af_det_veh_list = final_df['num_af_det_veh'].tolist()
    log_vars.num_af_det_sim_list = final_df['num_af_det_sim'].tolist()

    def find_max_range(row):
        max_range_veh = 0
        max_range_sim = 0
        range_saturated = False
        try:
            if ran_cols_veh:
                # Filter NaNs and handle empty
                veh_vals = [v for v in row[ran_cols_veh] if pd.notna(v)]
                if veh_vals:
                    max_range_veh = max(veh_vals)
            if ran_cols_sim:
                sim_vals = [v for v in row[ran_cols_sim] if pd.notna(v)]
                if sim_vals:
                    max_range_sim = max(sim_vals)
        except Exception as e:
            logger.custom_print(f"[WARNING] find_max_range failed: {e}")
        try:
            if max_range_veh >= config.RANGE_SATURATION_THRESHOLD and max_range_sim < config.RANGE_SATURATION_THRESHOLD:
                range_saturated = True
        except:
            pass
        return max_range_veh, max_range_sim, range_saturated

    final_df[['max_range_veh', 'max_range_sim', 'range_saturated']] = final_df.apply(find_max_range, axis=1, result_type="expand")
    log_vars.max_range_veh_list = final_df['max_range_veh'].tolist()
    log_vars.max_range_sim_list = final_df['max_range_sim'].tolist()
    log_vars.saturated_list = final_df['range_saturated'].tolist()
    if len(log_vars.saturated_list) != 0:
        log_vars.perc_of_scans_with_range_saturation = round((sum(log_vars.saturated_list) / len(log_vars.saturated_list)) * 100, 2)
    else:
        log_vars.perc_of_scans_with_range_saturation = 0

    def count_det_params_matches(row):
        """Count matches for rdd2_range and rdd2_range_rate within thresholds. Robust for missing/NaN."""
        try:
            num_detect_veh = int(row['num_af_det_veh'])
        except:
            num_detect_veh = 0
        try:
            num_detect_sim = int(row['num_af_det_sim'])
        except:
            num_detect_sim = 0
        # Clip to available columns length to avoid index errors
        num_detect_veh = min(num_detect_veh, len(rindx_cols_veh), len(dindx_cols_veh), len(ran_cols_veh) if ran_cols_veh else 0)
        num_detect_sim = min(num_detect_sim, len(rindx_cols_sim), len(dindx_cols_sim), len(ran_cols_sim) if ran_cols_sim else 0)
        # Helper to handle NaN in pairs
        def _safe_pair(val):
            return None if pd.isna(val) else val
        veh_pairs_raw = list(zip(row[rindx_cols_veh[:num_detect_veh]] if rindx_cols_veh else [], row[dindx_cols_veh[:num_detect_veh]] if dindx_cols_veh else []))
        sim_pairs_raw = list(zip(row[rindx_cols_sim[:num_detect_sim]] if rindx_cols_sim else [], row[dindx_cols_sim[:num_detect_sim]] if dindx_cols_sim else []))
        # Filter out pairs containing NaN
        veh_pairs = [p for p in veh_pairs_raw if not any(pd.isna(x) for x in p)]
        sim_pairs = [p for p in sim_pairs_raw if not any(pd.isna(x) for x in p)]
        # Build sim_data safely
        try:
            sim_data = dict(zip(sim_pairs, zip(row[ran_cols_sim[:num_detect_sim]] if ran_cols_sim else [],
                                               row[vel_cols_sim[:num_detect_sim]] if vel_cols_sim else [],
                                               row[theta_cols_sim[:num_detect_sim]] if theta_cols_sim else [],
                                               row[phi_cols_sim[:num_detect_sim]] if phi_cols_sim else [])))
        except Exception as e:
            sim_data = {}

        # Fallback for RDD unavailable (all pairs None) - match without rindx
        is_rdd_available = getattr(log_vars, 'rdd_data_available', True)
        if not is_rdd_available or (len(veh_pairs)==0 and num_detect_veh>0) or (len(sim_pairs)==0 and num_detect_sim>0):
            # Fallback: brute force matching without rindx/dindx, using thresholds only
            subset_match_count = 0
            all_match_count = 0
            matched_sim_indices = set()
            for idx in range(num_detect_veh):
                if idx >= len(ran_cols_veh) or idx >= len(vel_cols_veh) or idx >= len(theta_cols_veh) or idx >= len(phi_cols_veh):
                    continue
                try:
                    veh_ran = row[ran_cols_veh[idx]]
                    veh_vel = row[vel_cols_veh[idx]]
                    veh_theta = row[theta_cols_veh[idx]]
                    veh_phi = row[phi_cols_veh[idx]]
                    veh_single_target = row[single_cols_veh[idx]] if idx < len(single_cols_veh) else 0
                    veh_superres_target = row[superres_cols_veh[idx]] if idx < len(superres_cols_veh) else 0
                    veh_bistatic = row[bistatic_cols_veh[idx]] if idx < len(bistatic_cols_veh) else 0
                    if pd.isna(veh_ran) or pd.isna(veh_vel) or pd.isna(veh_theta) or pd.isna(veh_phi):
                        continue
                except:
                    continue
                best_jdx = None
                best_ran_diff = None
                for jdx in range(num_detect_sim):
                    if jdx in matched_sim_indices:
                        continue
                    if jdx >= len(ran_cols_sim) or jdx >= len(vel_cols_sim) or jdx >= len(theta_cols_sim) or jdx >= len(phi_cols_sim):
                        continue
                    try:
                        sim_ran = row[ran_cols_sim[jdx]]
                        sim_vel = row[vel_cols_sim[jdx]]
                        sim_theta = row[theta_cols_sim[jdx]]
                        sim_phi = row[phi_cols_sim[jdx]]
                        if pd.isna(sim_ran) or pd.isna(sim_vel) or pd.isna(sim_theta) or pd.isna(sim_phi):
                            continue
                    except:
                        continue
                    ran_diff = veh_ran - sim_ran
                    vel_diff = veh_vel - sim_vel
                    theta_diff = veh_theta - sim_theta
                    phi_diff = veh_phi - sim_phi
                    ran_diff_abs = abs(ran_diff)
                    vel_diff_abs = abs(vel_diff)
                    theta_diff_abs = abs(theta_diff)
                    phi_diff_abs = abs(phi_diff)
                    # Check thresholds
                    if ran_diff_abs <= config.RAN_THRESHOLD and vel_diff_abs <= config.VEL_THRESHOLD:
                        # subset match
                        if theta_diff_abs <= config.THETA_THRESHOLD and phi_diff_abs <= config.PHI_THRESHOLD:
                            # full match - take first
                            subset_match_count += 1
                            all_match_count += 1
                            matched_sim_indices.add(jdx)
                            best_jdx = None
                            break
                        else:
                            # keep as subset candidate but continue searching for full?
                            if best_jdx is None:
                                best_jdx = jdx
                                best_ran_diff = ran_diff
                    # Log diffs for plots when exceeding thresholds (approx) - limit to 5000 to avoid memory blowup
                    if ran_diff_abs > config.RAN_THRESHOLD and len(log_vars.ran_diff_list) < 5000:
                        log_vars.ran_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, veh_single_target, veh_superres_target, veh_bistatic, ran_diff))
                    if vel_diff_abs > config.VEL_THRESHOLD and len(log_vars.vel_diff_list) < 5000:
                        log_vars.vel_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, veh_single_target, veh_superres_target, veh_bistatic, vel_diff))
                    if theta_diff_abs > config.THETA_THRESHOLD and len(log_vars.theta_diff_list) < 5000:
                        log_vars.theta_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, veh_single_target, veh_superres_target, veh_bistatic, theta_diff))
                    if phi_diff_abs > config.PHI_THRESHOLD and len(log_vars.phi_diff_list) < 5000:
                        log_vars.phi_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, veh_single_target, veh_superres_target, veh_bistatic, phi_diff))
                if best_jdx is not None and best_jdx not in matched_sim_indices:
                    # Found subset but not full
                    subset_match_count += 1
                    # Already logged diffs above
            return subset_match_count, all_match_count

        subset_match_count = 0
        all_match_count = 0
        matched_sim_indices = set()
        for idx, veh_pair in enumerate(veh_pairs):
            for jdx, sim_pair in enumerate(sim_pairs):
                if jdx in matched_sim_indices:
                    continue
                if veh_pair == sim_pair:
                    sim_ran = row[ran_cols_sim[jdx]]
                    sim_vel = row[vel_cols_sim[jdx]]
                    sim_theta = row[theta_cols_sim[jdx]]
                    sim_phi = row[phi_cols_sim[jdx]]
                    veh_ran = row[ran_cols_veh[idx]]
                    veh_vel = row[vel_cols_veh[idx]]
                    veh_theta = row[theta_cols_veh[idx]]
                    veh_phi = row[phi_cols_veh[idx]]
                    veh_single_target = row[single_cols_veh[idx]]
                    veh_superres_target = row[superres_cols_veh[idx]]
                    veh_bistatic = row[bistatic_cols_veh[idx]]

                    ran_diff = veh_ran - sim_ran
                    ran_diff_abs = abs(ran_diff)
                    vel_diff = veh_vel - sim_vel
                    vel_diff_abs = abs(vel_diff)
                    theta_diff = veh_theta - sim_theta
                    theta_diff_abs = abs(theta_diff)
                    phi_diff = veh_phi - sim_phi
                    phi_diff_abs = abs(phi_diff)

                    if(ran_diff_abs <= config.RAN_THRESHOLD and vel_diff_abs <= config.VEL_THRESHOLD):
                        subset_match_count += 1
                        if(theta_diff_abs <= config.THETA_THRESHOLD and phi_diff_abs <= config.PHI_THRESHOLD):
                            all_match_count += 1
                            matched_sim_indices.add(jdx)
                            break

                    if (ran_diff_abs > config.RAN_THRESHOLD and len(log_vars.ran_diff_list) < 5000):
                        log_vars.ran_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, veh_single_target, veh_superres_target, veh_bistatic, ran_diff))
                    if (vel_diff_abs > config.VEL_THRESHOLD and len(log_vars.vel_diff_list) < 5000):
                        log_vars.vel_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, veh_single_target, veh_superres_target, veh_bistatic, vel_diff))
                    if (theta_diff_abs > config.THETA_THRESHOLD and len(log_vars.theta_diff_list) < 5000):
                        log_vars.theta_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, veh_single_target, veh_superres_target, veh_bistatic, theta_diff))
                    if (phi_diff_abs > config.PHI_THRESHOLD and len(log_vars.phi_diff_list) < 5000):
                        log_vars.phi_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, veh_single_target, veh_superres_target, veh_bistatic, phi_diff))

        return subset_match_count, all_match_count

    final_df[['det_subset_params_match_count', 'det_all_params_match_count']] = final_df.apply(count_det_params_matches, axis=1, result_type="expand")

    final_df['same_num_of_AF_detections'] = final_df['num_af_det_veh'] == final_df['num_af_det_sim']
    # Safe division handling zero
    final_df['matching_pct_det_all_params'] = final_df['det_all_params_match_count']/final_df['num_af_det_veh'].replace(0, np.nan)
    final_df['matching_pct_det_all_params'] = final_df['matching_pct_det_all_params'].fillna(0)
    final_df['matching_pct_det_subset_params'] = final_df['det_subset_params_match_count']/final_df['num_af_det_veh'].replace(0, np.nan)
    final_df['matching_pct_det_subset_params'] = final_df['matching_pct_det_subset_params'].fillna(0)
    log_vars.accuracy_list = final_df['matching_pct_det_all_params'].tolist()

    num_of_af_dets_in_veh = sum(final_df['num_af_det_veh'])
    num_of_af_dets_in_sim = sum(final_df['num_af_det_sim'])
    num_of_dets_with_matching_rv = sum(final_df['det_subset_params_match_count'])
    num_of_dets_with_matching_rvtp = sum(final_df['det_all_params_match_count'])
    num_of_SI_with_matching_rv = final_df[(final_df['matching_pct_det_subset_params'] >= 0.99)].shape[0]
    num_of_SI_with_matching_rvtp = final_df[(final_df['matching_pct_det_all_params'] >= 0.99)].shape[0]

    if num_of_af_dets_in_veh != 0:
        log_vars.overall_accuracy = round((num_of_dets_with_matching_rvtp / num_of_af_dets_in_veh) * 100, 2)
        log_vars.min_accuracy = round(min(log_vars.accuracy_list)*100, 2)
    else:
        log_vars.overall_accuracy = 0
        log_vars.min_accuracy = 0
        
    if log_vars.cdc_data_available:
        num_of_af_dets_in_veh_without_cdc_saturation = final_df.loc[~final_df['cdc_saturated'], 'num_af_det_veh'].sum()
        if num_of_af_dets_in_veh_without_cdc_saturation != 0:
            num_of_dets_with_matching_rvtp_without_cdc_saturation = final_df.loc[~final_df['cdc_saturated'], 'det_all_params_match_count'].sum()
            log_vars.overall_accuracy_excluding_cdc_saturation = round((num_of_dets_with_matching_rvtp_without_cdc_saturation / num_of_af_dets_in_veh_without_cdc_saturation) * 100, 2)
        else:
            log_vars.overall_accuracy_excluding_cdc_saturation = 0

    kpis_final = {'result1':
                    {'numerator': num_of_SI_with_matching_rv,
                     'denominator': log_vars.num_of_SI_in_veh_and_sim_merged,
                     'value': round((num_of_SI_with_matching_rv / log_vars.num_of_SI_in_veh_and_sim_merged) * 100, 2) if (log_vars.num_of_SI_in_veh_and_sim_merged != 0) else None},
                'result2':
                    {'numerator': num_of_SI_with_matching_rvtp,
                     'denominator': log_vars.num_of_SI_in_veh_and_sim_merged,
                     'value': round((num_of_SI_with_matching_rvtp / log_vars.num_of_SI_in_veh_and_sim_merged) * 100, 2) if (log_vars.num_of_SI_in_veh_and_sim_merged != 0) else None},
                'result3':
                    {'numerator': num_of_dets_with_matching_rv,
                     'denominator': num_of_af_dets_in_veh,
                     'value': round((num_of_dets_with_matching_rv / num_of_af_dets_in_veh) * 100, 2) if (num_of_af_dets_in_veh != 0) else None},
                'result4':
                    {'numerator': num_of_dets_with_matching_rvtp,
                     'denominator': num_of_af_dets_in_veh,
                     'value': round((num_of_dets_with_matching_rvtp / num_of_af_dets_in_veh) * 100, 2) if (num_of_af_dets_in_veh != 0) else None},
                }
    # logger.custom_print(f"% of SI with 99% matching det params(r,v): "
    #       f"{kpis_final['result1']['numerator']}/{kpis_final['result1']['denominator']} --> {kpis_final['result1']['value']}%")
    # logger.custom_print(f"% of SI with 99% matching det params(r,v,t,p): "
    #       f"{kpis_final['result2']['numerator']}/{kpis_final['result2']['denominator']} --> {kpis_final['result2']['value']}%")
    # logger.custom_print(f"Number of detections in (vehicle, simulation): ({num_of_af_dets_in_veh}, {num_of_af_dets_in_sim})")
    # logger.custom_print(f"Accuracy(r,v): "
    #       f"{kpis_final['result3']['numerator']}/{kpis_final['result3']['denominator']} --> {kpis_final['result3']['value']}%")
    # logger.custom_print(f"Accuracy(r,v,t,p): "
    #       f"{kpis_final['result4']['numerator']}/{kpis_final['result4']['denominator']} --> {kpis_final['result4']['value']}%")
 
    #################################
    # VSE data processing
    #################################
    if log_vars.vse_data_available:
        logger.custom_print("[INFO] VSE Data Processing...")
        log_vars.dist_travelled_by_veh = (veh_vse_df['veh_speed'] * config.RADAR_CYCLE_S).sum()

        # Keep only those scans of sim VSE stream which are available for KPI
        sim_vse_df = sim_vse_df[sim_vse_df['scan_index'].isin(final_df['scan_index'].tolist())]
        sim_vse_df = sim_vse_df.drop_duplicates(subset=['scan_index'], keep='first')  # type: ignore
        log_vars.dist_travelled_by_sim = (sim_vse_df['veh_speed'] * config.RADAR_CYCLE_S).sum()
    #################################
    
    #################################
    # Print data related to this log
    #################################
    logger.custom_print(f"[KPI] Accuracy: {log_vars.overall_accuracy}")
    logger.custom_print(f"[KPI] Accuracy excluding CDC saturation: {log_vars.overall_accuracy_excluding_cdc_saturation}")
    logger.custom_print(f"[KPI] % of scans with CDC saturation: {log_vars.perc_of_scans_with_cdc_saturation}")
    logger.custom_print(f"[KPI] % of scans with range saturation: {log_vars.perc_of_scans_with_range_saturation}")
    
    #################################
    # HTML Content
    #################################
    session_vars.html_content += f"""
    <b>KPI:</b> Accuracy: ({kpis_final['result4']['numerator']}/{kpis_final['result4']['denominator']}) --> <b>{kpis_final['result4']['value']}%</b>
    <details>
        <summary><i>Details</i></summary>
        <b>RDD Streams</b><br>
        Number of SI in (vehicle, simulation) : {log_vars.num_of_SI_in_veh_rdd}, {log_vars.num_of_SI_in_sim_rdd}<br>
        Number of same SI available in both vehicle and simulation: {log_vars.num_of_SI_in_veh_and_sim_rdd}<br>
        % of SI with same number of RDD1 detections: 
        ({kpis_rdd['result1']['numerator']}/{kpis_rdd['result1']['denominator']}) --> {kpis_rdd['result1']['value']}%<br>
        % of SI with 99% matching (rindx, dindx) pair:
        ({kpis_merged_rdd['result1']['numerator']}/{kpis_merged_rdd['result1']['denominator']}) --> {kpis_merged_rdd['result1']['value']}%
        <br>
        % of SI with 99% matching (range, rangerate) pair:
        ({kpis_merged_rdd['result2']['numerator']}/{kpis_merged_rdd['result2']['denominator']}) --> {kpis_merged_rdd['result2']['value']}%
        <br>
    """
    session_vars.html_content += f"""
        <b>Detection Streams</b><br>
        Number of SI in (vehicle, simulation) : {log_vars.num_of_SI_in_veh_af}, {log_vars.num_of_SI_in_sim_af}<br>
        Number of same SI available in both vehicle and simulation: {log_vars.num_of_SI_in_veh_and_sim_af}<br>
        % of SI with same number of AF detections: 
        ({kpis_af['result1']['numerator']}/{kpis_af['result1']['denominator']}) --> {kpis_af['result1']['value']}%
        <br>
    """      
    if log_vars.cdc_data_available:
        session_vars.html_content += f"""
            <b>CDC Streams</b><br>
            Number of SI in (vehicle) : {log_vars.num_of_SI_in_veh_cdc}<br>
        """           
    session_vars.html_content += f"""  
        <b>Detection, RDD (and CDC) Streams Merged</b><br>
        Number of SI in (vehicle, simulation) : {log_vars.num_of_SI_in_veh_merged}, {log_vars.num_of_SI_in_sim_merged}<br>
        Number of same SI available in both vehicle and simulation: {log_vars.num_of_SI_in_veh_and_sim_merged}<br>
        % of SI with 99% matching det params(r,v):
        ({kpis_final['result1']['numerator']}/{kpis_final['result1']['denominator']}) --> {kpis_final['result1']['value']}%
        <br>
        % of SI with 99% matching det params(r,v,t,p):
        ({kpis_final['result2']['numerator']}/{kpis_final['result2']['denominator']}) --> {kpis_final['result2']['value']}%
        <br>
        Number of detections in (vehicle, simulation): {num_of_af_dets_in_veh}, {num_of_af_dets_in_sim}<br>
        Accuracy(r,v):
        ({kpis_final['result3']['numerator']}/{kpis_final['result3']['denominator']}) --> {kpis_final['result3']['value']}%
        Accuracy(r,v,t,p):
        ({kpis_final['result4']['numerator']}/{kpis_final['result4']['denominator']}) --> {kpis_final['result4']['value']}%
        <br>
    """
    #################################
    return True

def func_bar(diffs):
    # Robust: handle empty diffs (edge case where all detections matched)
    if diffs is None or len(diffs) == 0:
        return go.Bar(x=[], y=[], name="No data")
    # Also handle numpy array case
    try:
        diffs_list = diffs.tolist() if hasattr(diffs, 'tolist') else list(diffs)
    except:
        diffs_list = list(diffs)
    if len(diffs_list) == 0:
        return go.Bar(x=[], y=[], name="No data")
    counts = Counter(diffs_list)
    total_observations = len(diffs_list)
    if total_observations == 0:
        return go.Bar(x=[], y=[], name="No data")
    percentages = {key: (value / total_observations) * 100 for key, value in counts.items()}
    # Handle empty percentages
    if not percentages:
        return go.Bar(x=[], y=[], name="No data")
    sorted_dict = dict(sorted(percentages.items()))
    return go.Bar(x=list(sorted_dict.keys()), y=list(sorted_dict.values()))

def func_scatter(x, y):
    # Return Scatter trace
    return go.Scatter(x=x, y=y, mode='markers')

def plot_stats():
    rng_idx = 0
    vel_idx = 1
    theta_idx = 2
    phi_idx = 3
    single_target_idx = 4
    superres_target_idx = 5
    bistatic_idx = 6
    error_idx = 7
    # Handle empty diff lists gracefully
    try:
        ran_diffs = np.array([ele[error_idx] for ele in log_vars.ran_diff_list]).round(3) if log_vars.ran_diff_list else np.array([])
        vel_diffs = np.array([ele[error_idx] for ele in log_vars.vel_diff_list]).round(3) if log_vars.vel_diff_list else np.array([])
        theta_diffs = np.array([ele[error_idx] for ele in log_vars.theta_diff_list]).round(3) if log_vars.theta_diff_list else np.array([])
        phi_diffs = np.array([ele[error_idx] for ele in log_vars.phi_diff_list]).round(3) if log_vars.phi_diff_list else np.array([])
    except Exception as e:
        logger.custom_print(f"[WARNING] Failed to compute diff arrays: {e}")
        ran_diffs = np.array([]); vel_diffs = np.array([]); theta_diffs = np.array([]); phi_diffs = np.array([])

    #################################
    # Create subplots for error bar plots:
    #################################
    fig_bar = sp.make_subplots(rows=2, cols=2, horizontal_spacing=0.04, vertical_spacing=0.2)

    # Manually add traces for bar plots
    fig_bar.add_trace(func_bar(ran_diffs), row=1, col=1)
    fig_bar.update_yaxes(title_text="Percentage(%)", row=1, col=1)
    fig_bar.update_xaxes(title_text="Range error", row=1, col=1)
    fig_bar.add_trace(func_bar(vel_diffs), row=1, col=2)
    fig_bar.update_xaxes(title_text="Range Rate error", row=1, col=2)
    fig_bar.add_trace(func_bar(theta_diffs), row=2, col=1)
    fig_bar.update_yaxes(title_text="Percentage(%)", row=2, col=1)
    fig_bar.update_xaxes(title_text="Azimuth error", row=2, col=1)
    fig_bar.add_trace(func_bar(phi_diffs), row=2, col=2)
    fig_bar.update_xaxes(title_text="Elevation error", row=2, col=2)

    # Update layout for bar plot
    fig_bar.update_layout(height=700, width=1250, title_text="% of unmatched detections v/s error", showlegend=False)
    fig_bar.update_traces(marker_color='red')
    fig_bar.update_xaxes(zeroline=False, showgrid=False, type='category')
    fig_bar.update_yaxes(zeroline=False, showgrid=False)
    #fig_bar.show()

    bar_plot_html = pio.to_html(fig_bar, full_html=False, include_plotlyjs=False)
    #################################

    #################################
    # Create subplots for error v/s different detection properties plots:
    #################################
    fig_scatter = sp.make_subplots(rows=4, cols=7, horizontal_spacing=0.03, vertical_spacing=0.04)

    # Manually add traces for scatter plots
    # Range error against detection properties
    row_num = 1
    rang = [ele[rng_idx] for ele in log_vars.ran_diff_list]
    fig_scatter.add_trace(func_scatter(rang, ran_diffs), row=row_num, col=1)
    fig_scatter.update_yaxes(title_text="Range error", row=row_num, col=1)
    fig_scatter.update_xaxes(title_text="Range", row=row_num, col=1)
    vel = [ele[vel_idx] for ele in log_vars.ran_diff_list]
    fig_scatter.add_trace(func_scatter(vel, ran_diffs), row=row_num, col=2)
    fig_scatter.update_xaxes(title_text="Range Rate", row=row_num, col=2)
    theta = [ele[theta_idx] for ele in log_vars.ran_diff_list]
    fig_scatter.add_trace(func_scatter(theta, ran_diffs), row=row_num, col=3)
    fig_scatter.update_xaxes(title_text="Azimuth", row=row_num, col=3)
    phi = [ele[phi_idx] for ele in log_vars.ran_diff_list]
    fig_scatter.add_trace(func_scatter(phi, ran_diffs), row=row_num, col=4)
    fig_scatter.update_xaxes(title_text="Elevation", row=row_num, col=4)
    single_target = [ele[single_target_idx] for ele in log_vars.ran_diff_list]
    fig_scatter.add_trace(func_scatter(single_target, ran_diffs), row=row_num, col=5)
    fig_scatter.update_xaxes(title_text="Single Target", row=row_num, col=5)
    superres_target = [ele[superres_target_idx] for ele in log_vars.ran_diff_list]
    fig_scatter.add_trace(func_scatter(superres_target, ran_diffs), row=row_num, col=6)
    fig_scatter.update_xaxes(title_text="Superres Target", row=row_num, col=6)
    bistatic = [ele[bistatic_idx] for ele in log_vars.ran_diff_list]
    fig_scatter.add_trace(func_scatter(bistatic, ran_diffs), row=row_num, col=7)
    fig_scatter.update_xaxes(title_text="Bistatic", row=row_num, col=7)

    # Range rate error against detection properties
    row_num = 2
    rang = [ele[rng_idx] for ele in log_vars.vel_diff_list]
    fig_scatter.add_trace(func_scatter(rang, vel_diffs), row=row_num, col=1)
    fig_scatter.update_yaxes(title_text="Range Rate error", row=row_num, col=1)
    fig_scatter.update_xaxes(title_text="Range", row=row_num, col=1)
    vel = [ele[vel_idx] for ele in log_vars.vel_diff_list]
    fig_scatter.add_trace(func_scatter(vel, vel_diffs), row=row_num, col=2)
    fig_scatter.update_xaxes(title_text="Range Rate", row=row_num, col=2)
    theta = [ele[theta_idx] for ele in log_vars.vel_diff_list]
    fig_scatter.add_trace(func_scatter(theta, vel_diffs), row=row_num, col=3)
    fig_scatter.update_xaxes(title_text="Azimuth", row=row_num, col=3)
    phi = [ele[phi_idx] for ele in log_vars.vel_diff_list]
    fig_scatter.add_trace(func_scatter(phi, vel_diffs), row=row_num, col=4)
    fig_scatter.update_xaxes(title_text="Elevation", row=row_num, col=4)
    single_target = [ele[single_target_idx] for ele in log_vars.vel_diff_list]
    fig_scatter.add_trace(func_scatter(single_target, vel_diffs), row=row_num, col=5)
    fig_scatter.update_xaxes(title_text="Single Target", row=row_num, col=5)
    superres_target = [ele[superres_target_idx] for ele in log_vars.vel_diff_list]
    fig_scatter.add_trace(func_scatter(superres_target, vel_diffs), row=row_num, col=6)
    fig_scatter.update_xaxes(title_text="Superres Target", row=row_num, col=6)
    bistatic = [ele[bistatic_idx] for ele in log_vars.vel_diff_list]
    fig_scatter.add_trace(func_scatter(bistatic, vel_diffs), row=row_num, col=7)
    fig_scatter.update_xaxes(title_text="Bistatic", row=row_num, col=7)

    # Azimuth error against detection properties
    row_num = 3
    rang = [ele[rng_idx] for ele in log_vars.theta_diff_list]
    fig_scatter.add_trace(func_scatter(rang, theta_diffs), row=row_num, col=1)
    fig_scatter.update_yaxes(title_text="Azimuth error", row=row_num, col=1)
    fig_scatter.update_xaxes(title_text="Range", row=row_num, col=1)
    vel = [ele[vel_idx] for ele in log_vars.theta_diff_list]
    fig_scatter.add_trace(func_scatter(vel, theta_diffs), row=row_num, col=2)
    fig_scatter.update_xaxes(title_text="Range Rate", row=row_num, col=2)
    theta = [ele[theta_idx] for ele in log_vars.theta_diff_list]
    fig_scatter.add_trace(func_scatter(theta, theta_diffs), row=row_num, col=3)
    fig_scatter.update_xaxes(title_text="Azimuth", row=row_num, col=3)
    phi = [ele[phi_idx] for ele in log_vars.theta_diff_list]
    fig_scatter.add_trace(func_scatter(phi, theta_diffs), row=row_num, col=4)
    fig_scatter.update_xaxes(title_text="Elevation", row=row_num, col=4)
    single_target = [ele[single_target_idx] for ele in log_vars.theta_diff_list]
    fig_scatter.add_trace(func_scatter(single_target, theta_diffs), row=row_num, col=5)
    fig_scatter.update_xaxes(title_text="Single Target", row=row_num, col=5)
    superres_target = [ele[superres_target_idx] for ele in log_vars.theta_diff_list]
    fig_scatter.add_trace(func_scatter(superres_target, theta_diffs), row=row_num, col=6)
    fig_scatter.update_xaxes(title_text="Superres Target", row=row_num, col=6)
    bistatic = [ele[bistatic_idx] for ele in log_vars.theta_diff_list]
    fig_scatter.add_trace(func_scatter(bistatic, theta_diffs), row=row_num, col=7)
    fig_scatter.update_xaxes(title_text="Bistatic", row=row_num, col=7)

    # Elevation error against detection properties
    row_num = 4
    rang = [ele[rng_idx] for ele in log_vars.phi_diff_list]
    fig_scatter.add_trace(func_scatter(rang, phi_diffs), row=row_num, col=1)
    fig_scatter.update_yaxes(title_text="Elevation error", row=row_num, col=1)
    fig_scatter.update_xaxes(title_text="Range", row=row_num, col=1)
    vel = [ele[vel_idx] for ele in log_vars.phi_diff_list]
    fig_scatter.add_trace(func_scatter(vel, phi_diffs), row=row_num, col=2)
    fig_scatter.update_xaxes(title_text="Range Rate", row=row_num, col=2)
    theta = [ele[theta_idx] for ele in log_vars.phi_diff_list]
    fig_scatter.add_trace(func_scatter(theta, phi_diffs), row=row_num, col=3)
    fig_scatter.update_xaxes(title_text="Azimuth", row=row_num, col=3)
    phi = [ele[phi_idx] for ele in log_vars.phi_diff_list]
    fig_scatter.add_trace(func_scatter(phi, phi_diffs), row=row_num, col=4)
    fig_scatter.update_xaxes(title_text="Elevation", row=row_num, col=4)
    single_target = [ele[single_target_idx] for ele in log_vars.phi_diff_list]
    fig_scatter.add_trace(func_scatter(single_target, phi_diffs), row=row_num, col=5)
    fig_scatter.update_xaxes(title_text="Single Target", row=row_num, col=5)
    superres_target = [ele[superres_target_idx] for ele in log_vars.phi_diff_list]
    fig_scatter.add_trace(func_scatter(superres_target, phi_diffs), row=row_num, col=6)
    fig_scatter.update_xaxes(title_text="Superres Target", row=row_num, col=6)
    bistatic = [ele[bistatic_idx] for ele in log_vars.phi_diff_list]
    fig_scatter.add_trace(func_scatter(bistatic, phi_diffs), row=row_num, col=7)
    fig_scatter.update_xaxes(title_text="Bistatic", row=row_num, col=7)

    # Update layout for scatter plot
    fig_scatter.update_layout(height=1400, width=1250,
                              title_text="Error v/s detection Range, Range Rate, Azimuth, Elevation", showlegend=False)
    fig_scatter.update_traces(marker_color='red', marker={'size': 2})
    fig_scatter.update_xaxes(zeroline=False, showgrid=False)
    fig_scatter.update_yaxes(zeroline=False, showgrid=False)
    #fig_scatter.show()

    scatter_plot_html = pio.to_html(fig_scatter, full_html=False, include_plotlyjs=False)
    #################################

    #################################
    # Create subplots for different parameters v/s scanindex:
    #################################
    fig_line = sp.make_subplots(rows=4, cols=1, horizontal_spacing=0.04, vertical_spacing=0.1)

    # Manually add traces for line plots
    row_num = 1
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.accuracy_list, mode="lines", name="Accuracy", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Accuracy", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    row_num = 2
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.num_af_det_veh_list, mode="lines", name="Veh", line=dict(color="red")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.num_af_det_sim_list, mode="lines", name="Sim", line=dict(color="blue")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Num of dets", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    row_num = 3
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.max_range_veh_list, mode="lines", name="Veh", line=dict(color="red")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.max_range_sim_list, mode="lines", name="Sim", line=dict(color="blue")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Max range", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    row_num = 4
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.saturated_list, mode="lines", name="Saturation flag", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Saturation flag", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    # Update layout for plot
    fig_line.update_layout(height=1200, width=1250, title_text="Params v/s scanindex", showlegend=True)
    fig_line.update_xaxes(zeroline=False, showgrid=False, type='category')
    fig_line.update_yaxes(zeroline=False, showgrid=False)
    #fig_line.show()

    params_plot_html = pio.to_html(fig_line, full_html=False, include_plotlyjs=False)
    #################################

    #################################
    # HTML Content
    #################################
    session_vars.html_content += f"""
        <b><u>Plots:</u></b>
        <details>
            <summary><i>Plot A</i></summary>
            {bar_plot_html}
        </details>
        <details>
            <summary><i>Plot B</i></summary>
            {scatter_plot_html}
        </details>
        <details>
            <summary><i>Plot C</i></summary>
            {params_plot_html}
        </details>
    </details>
    <hr>
    """
def plot_data_across_logs(data_across_log):
    # Robust handling edge case: empty data
    total_logs = sum(len(v) for v in data_across_log.values()) if data_across_log else 0
    if total_logs == 0:
        logger.custom_print("[WARNING] No data across logs to plot")
        try:
            session_vars.html_content += "<p><em>No data across logs to plot</em></p><hr>"
        except:
            pass
        return
    #################################
    # Create sub-plot for different data across logs:
    #################################
    # Calculate number of rows dynamically based on CDC mode
    num_rows = 2  # Base rows: overall accuracy + range saturation
    if session_vars.is_cdc_mode:
        num_rows += 2  # Add CDC accuracy + CDC saturation percentage
    fig_line = sp.make_subplots(rows=num_rows, cols=1, horizontal_spacing=0.04, vertical_spacing=0.05)

    sensor_color_dict = {"FC": "red",
                         "FL": "green",
                         "FR": "blue",
                         "RL": "violet",
                         "RR": "black"}

    row_num = 1
    for sensor in data_across_log.keys():
        #min_acc = [data['min_accuracy'] for data in data_across_log[sensor]]
        overall_acc = [data['overall_accuracy'] for data in data_across_log[sensor]]
        color = sensor_color_dict[sensor]
        #fig_line.add_trace(go.Scatter(x=list(range(1, len(min_acc) + 1)), y=min_acc, mode="lines+markers", opacity=.3, name=sensor+"_min_acc", line=dict(color=color, dash='dash')), row=row_num, col=1)
        fig_line.add_trace(go.Scatter(x=list(range(1, len(overall_acc)+1)), y=overall_acc, mode="lines+markers", name=sensor, line=dict(color=color)), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Accuracy", row=row_num, col=1)

    if session_vars.is_cdc_mode:
        row_num += 1
        for sensor in data_across_log.keys():
            overall_acc_without_cdc_saturation = [data['overall_accuracy_excluding_cdc_saturation'] for data in data_across_log[sensor]]
            color = sensor_color_dict[sensor]
            fig_line.add_trace(go.Scatter(x=list(range(1, len(overall_acc_without_cdc_saturation) + 1)), y=overall_acc_without_cdc_saturation, mode="lines+markers", name=sensor, line=dict(color=color)), row=row_num, col=1)
        fig_line.update_yaxes(title_text="Accuracy Without CDC Saturation", row=row_num, col=1)
        
        row_num += 1
        for sensor in data_across_log.keys():
            cdc_satur_perc = [data['perc_of_scans_with_cdc_saturation'] for data in data_across_log[sensor]]
            color = sensor_color_dict[sensor]
            fig_line.add_trace(go.Scatter(x=list(range(1, len(cdc_satur_perc) + 1)), y=cdc_satur_perc, mode="lines+markers", name=sensor, line=dict(color=color)), row=row_num, col=1)
        fig_line.update_yaxes(title_text="% Of Scans With CDC Saturation", row=row_num, col=1)

    row_num += 1
    for sensor in data_across_log.keys():
        rng_satur_perc = [data['perc_of_scans_with_range_saturation'] for data in data_across_log[sensor]]
        color = sensor_color_dict[sensor]
        fig_line.add_trace(go.Scatter(x=list(range(1, len(rng_satur_perc) + 1)), y=rng_satur_perc, mode="lines+markers", name=sensor, line=dict(color=color)), row=row_num, col=1)
    fig_line.update_yaxes(title_text="% Of Scans With Range Saturation", row=row_num, col=1)

    # Adjust height based on number of rows (base height per row: ~300px)
    base_height = 300
    total_height = base_height * num_rows
    
    fig_line.update_layout(height=total_height, width=1250, title_text="Data across logs", showlegend=True)
    fig_line.update_xaxes(zeroline=False, showgrid=False, type='category')
    fig_line.update_yaxes(zeroline=False, showgrid=False)
    #fig_line.show()

    data_across_logs_plot_html = pio.to_html(fig_line, full_html=False, include_plotlyjs=False)
    #################################

    #################################
    # HTML Content
    #################################
    session_vars.html_content += f"""
            {data_across_logs_plot_html}
    <hr width="100%" size="2" color="blue" noshade>
    """
    #################################

def process_logs(data_files):
    
    start_time = time.time()

    data_across_log = {'FC':[], 'FL':[], 'FR':[], 'RL':[], 'RR':[]}
    num_of_logs = len(data_files['input'])
    for log_idx in range(num_of_logs): # log_idx should start from 0 itself as it is used to index objects
        #################################
        # Reset the below variables for each log
        #################################
        log_vars.reset()
        #################################

        # Each log should have atleast 4 csv files. CDC files are optional
        input_det_file = data_files['input'][log_idx][0]
        input_rdd_file = data_files['input'][log_idx][1]
        input_cdc_file = data_files['input'][log_idx][2]
        input_vse_file = data_files['input'][log_idx][3]
        output_det_file = data_files['output'][log_idx][0]
        output_rdd_file = data_files['output'][log_idx][1]
        output_cdc_file = data_files['output'][log_idx][2]
        output_vse_file = data_files['output'][log_idx][3]

        # Extract the required file name
        # input_det_file: 'C:\\Project\\Logs\\HGR770_Gen7v2_20250507_164906_001_ORCAS_FC_UDP_GEN7_DET_CORE.csv'
        # det_log_name: 'HGR770_Gen7v2_ASTA_cpna50_40kph_20250507_164906_001_ORCAS_FC_UDP_GEN7_DET_CORE.csv'
        # det_log_name_wo_ext: 'HGR770_Gen7v2_ASTA_cpna50_40kph_20250507_164906_001_ORCAS_FC'
        det_log_name = os.path.basename(input_det_file)
        det_log_name_wo_ext = det_log_name.replace(config.DET_FILE_SUFFIX, "")
        logger.custom_print(f"\n[INFO] Processing log: {log_idx+1}/{num_of_logs} : {det_log_name_wo_ext}")

        #################################
        # check if the csv files exists.
        # If any of the csv files have issues, then simply continue on to next log
        #################################
        if not check_file(input_det_file):
            continue
        if not check_file(input_rdd_file):
            continue
        if not check_file(output_det_file):
            continue
        if not check_file(output_rdd_file):
            continue
        #################################

        session_vars.html_content += f"""
                <b>Log:</b> {det_log_name_wo_ext};
                """
        # Few variables are different for front and corner radars, 
        # hence, it has to be updated for every log based on the log name
        if '_FC_UDP_' in det_log_name:
            config.MAX_NUM_OF_AF_DETS = config.MAX_NUM_OF_AF_DETS_FRONT_RADAR
            config.RANGE_SATURATION_THRESHOLD = config.RANGE_SATURATION_THRESHOLD_FRONT_RADAR
        else:
            config.MAX_NUM_OF_AF_DETS = config.MAX_NUM_OF_AF_DETS_CORNER_RADAR
            config.RANGE_SATURATION_THRESHOLD = config.RANGE_SATURATION_THRESHOLD_CORNER_RADAR

        #################################
        # Main function for each log
        #################################
        log_start_time = time.time()    
        status = process_one_log(input_det_file, output_det_file,
                      input_rdd_file, output_rdd_file,
                      input_cdc_file, output_cdc_file,
                      input_vse_file, output_vse_file)        
        log_end_time = time.time()
        log_execution_time = log_end_time - log_start_time
        logger.custom_print(f"[TIMING] Log {det_log_name_wo_ext} processed in {log_execution_time:.2f} seconds")
        #################################

        # Save the data and plot data only if the log was successfully processed
        if status:
            #################################
            # Save required data of each log
            #################################
            data_dict = {
                "det_log_name_wo_ext": det_log_name_wo_ext,
                "min_accuracy": log_vars.min_accuracy, 
                "overall_accuracy": log_vars.overall_accuracy,
                "overall_accuracy_excluding_cdc_saturation": log_vars.overall_accuracy_excluding_cdc_saturation,
                "perc_of_scans_with_cdc_saturation": log_vars.perc_of_scans_with_cdc_saturation,
                "perc_of_scans_with_range_saturation": log_vars.perc_of_scans_with_range_saturation,
                "num_of_SI_available_in_veh": log_vars.num_of_SI_in_veh_af,
                "num_of_SI_available_for_kpi": log_vars.num_of_SI_in_veh_and_sim_merged,
                "dist_travelled_by_veh": log_vars.dist_travelled_by_veh,
                "dist_travelled_by_sim": log_vars.dist_travelled_by_sim,                
                "log_idx": log_idx}

            found_sensor = False
            for sensor in ['FC', 'FL', 'FR', 'RL', 'RR']:
                if ('_' + sensor + '_UDP_') in det_log_name:
                    data_across_log[sensor].append(data_dict)
                    found_sensor = True
                    break
            if not found_sensor:
                logger.custom_print(f"[WARNING] Unsupported sensor position in {det_log_name}")
            #################################

            #################################
            # Plot statistics of each log
            #################################
            plot_stats()
            #################################
        else:
            session_vars.html_content += """
                <b>NA</b>
                <hr>
                """

        #################################
        # Create HTML files for every MAX_LOGS_IN_ONE_REPORT logs
        #################################
        if(((log_idx+1) % config.MAX_LOGS_IN_ONE_REPORT == 0) or ((log_idx+1) == num_of_logs)):
            logger.custom_print("\n[KPI] Yield summary till now")

            #################################
            # Table for yield data      
            #################################
            for sensor in data_across_log:
                sensor_data_list = data_across_log[sensor]
                sensor_num_of_SI_available_in_veh = sum([(sensor_data_dict['num_of_SI_available_in_veh']) for sensor_data_dict in sensor_data_list])
                sensor_num_of_SI_available_for_kpi = sum([(sensor_data_dict['num_of_SI_available_for_kpi']) for sensor_data_dict in sensor_data_list])
                sensor_dist_travelled_by_veh = sum([(sensor_data_dict['dist_travelled_by_veh']) for sensor_data_dict in sensor_data_list])
                sensor_dist_travelled_by_sim = sum([(sensor_data_dict['dist_travelled_by_sim']) for sensor_data_dict in sensor_data_list])
                 
                scan_yield = 0
                if sensor_num_of_SI_available_in_veh != 0:
                    scan_yield = round((sensor_num_of_SI_available_for_kpi/sensor_num_of_SI_available_in_veh)*100, 2)

                sensor_dist_travelled_by_veh_km = round((sensor_dist_travelled_by_veh * Constants.M_2_KM), 2)
                sensor_dist_travelled_by_sim_km = round((sensor_dist_travelled_by_sim * Constants.M_2_KM), 2)

                mileage_yield = 0
                if sensor_dist_travelled_by_veh != 0:
                    mileage_yield = round((sensor_dist_travelled_by_sim/sensor_dist_travelled_by_veh)*100, 2)

                logger.custom_print(f"[KPI] Scan Yield {sensor}: {scan_yield:.2f}% (Scans in veh = {sensor_num_of_SI_available_in_veh}, Scans in sim = {sensor_num_of_SI_available_for_kpi})")
                logger.custom_print(f"[KPI] Mileage Yield {sensor}: {mileage_yield:.2f}% (Veh mileage = {sensor_dist_travelled_by_veh_km}km, Sim mileage = {sensor_dist_travelled_by_sim_km}km)")

                session_vars.html_content += f"""
                    <table>
                        <tr>
                            <td><strong style="font-size: 1em;">Scan Yield {sensor}</strong></td>
                            <td style="text-align: right; width: 100px; font-family: 'Courier New', monospace;"><strong style="font-size: 1em;">: {scan_yield:.2f}%</strong></td>
                            <td><small>(Scans in veh = {sensor_num_of_SI_available_in_veh}, Scans in sim = {sensor_num_of_SI_available_for_kpi})</small></td>
                        </tr>
                        <tr>
                            <td><strong style="font-size: 1em;">Mileage Yield {sensor}</strong></td>
                            <td style="text-align: right; width: 100px; font-family: 'Courier New', monospace;"><strong style="font-size: 1em;">: {mileage_yield:.2f}%</strong></td>
                            <td><small>(Veh mileage = {sensor_dist_travelled_by_veh_km}km, Sim mileage = {sensor_dist_travelled_by_sim_km}km)</small></td>
                        </tr>
                    </table>          
                """
                
            session_vars.html_content += """
                <hr width="100%" size="2" color="blue" noshade> 
            """
            #################################
            
            #################################
            # Table for failed logs   
            #################################   
            log_name_list = []
            overall_accuracy_list = []
            overall_accuracy_excluding_cdc_saturation_list = []
            perc_of_scans_with_cdc_saturation_list = []
            perc_of_scans_with_range_saturation_list = []
            report_file_num_list = []

            for sensor in data_across_log:
                for data in data_across_log[sensor]:
                    log_name_list.append(data['det_log_name_wo_ext'])
                    overall_accuracy_list.append(float(data['overall_accuracy']))
                    overall_accuracy_excluding_cdc_saturation_list.append(float(data['overall_accuracy_excluding_cdc_saturation']))
                    perc_of_scans_with_cdc_saturation_list.append(float(data['perc_of_scans_with_cdc_saturation']))
                    perc_of_scans_with_range_saturation_list.append(float(data['perc_of_scans_with_range_saturation']))
                    report_file_num_list.append(((data['log_idx']//config.MAX_LOGS_IN_ONE_REPORT)+1)*config.MAX_LOGS_IN_ONE_REPORT)

            session_vars.html_content += f"""
                <div style="margin: 20px 0;">
                    <label for="threshold" style="font-weight: bold; margin-right: 2px;">
                        Enter overall accuracy threshold value:
                    </label>
                    <input type="number" id="threshold" value="{config.DEFAULT_ACC_THRESHOLD}" 
                    style="margin: 20px; padding: 4px;" oninput="updateTable()">
                </div>
                
                <div id="logs_based_on_acc"></div>
                <div id="logs_based_on_acc_excluding_cdc_saturation"></div>
                <div id="logs_based_on_saturation"></div>
                
                <script>
                    const fileNames = {repr(log_name_list)};
                    const accuracies = {repr(overall_accuracy_list)};
                    const accuracies_excluding_cdc_saturation = {repr(overall_accuracy_excluding_cdc_saturation_list)};
                    const perc_of_scans_with_cdc_saturations = {repr(perc_of_scans_with_cdc_saturation_list)};
                    const perc_of_scans_with_range_saturations = {repr(perc_of_scans_with_range_saturation_list)};
                    const report_file_nums = {repr(report_file_num_list)};
                    const isCdcMode = {str(session_vars.is_cdc_mode).lower()};
                    
                    function updateTable() {{
                        const threshold = document.getElementById('threshold') ? parseFloat(document.getElementById('threshold').value) : {config.DEFAULT_ACC_THRESHOLD};
                        const validThreshold = isNaN(threshold) ? {config.DEFAULT_ACC_THRESHOLD} : threshold;
                        
                        // Check if we have data
                        if (fileNames.length === 0) {{
                            document.getElementById('logs_based_on_acc').innerHTML = "<em>No log data available for filtering.</em>";
                            document.getElementById('logs_based_on_acc_excluding_cdc_saturation').innerHTML = "<em>No log data available for filtering.</em>";
                            document.getElementById('logs_based_on_saturation').innerHTML = "<em>No log data available for filtering.</em>";
                            return;
                        }}
                        
                        // Table 1: Filter based on accuracy
                        const filtered_based_on_acc = fileNames.map((name, i) => ({{
                            name, 
                            accuracy: accuracies[i],
                            perc_of_scans_with_cdc_saturation: perc_of_scans_with_cdc_saturations[i],
                            perc_of_scans_with_range_saturation: perc_of_scans_with_range_saturations[i],
                            report_file_num: report_file_nums[i],
                            isBelow: accuracies[i] < validThreshold
                        }})).filter(item => item.isBelow);
                        
                        const count_based_on_acc = filtered_based_on_acc.length;

                        const tableHTML_based_on_acc = `
                            <h3 style="color:red;">Logs with overall accuracy &lt; ${{validThreshold}}% (No. of logs: ${{count_based_on_acc}})</h3>
                            <table style="width:80%; text-align: left; margin-top:20px; border-collapse:collapse;">
                                <tr style="background:#f0f0f0">
                                    <th style="padding:10px">File Name</th>
                                    <th style="padding:10px">Accuracy</th>
                                    <th style="padding:10px">% of Scans with CDC Saturation</th>
                                    <th style="padding:10px">% of Scans with Range Saturation</th>
                                    <th style="padding:10px">Report File Number</th>
                                </tr>
                                ${{filtered_based_on_acc.map(item => `
                                    <tr>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.name}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.accuracy}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.perc_of_scans_with_cdc_saturation}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.perc_of_scans_with_range_saturation}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.report_file_num}}</td>
                                    </tr>
                                `).join('')}}
                            </table>
                        `;
                        document.getElementById('logs_based_on_acc').innerHTML = tableHTML_based_on_acc;
                        
                        // Table 2: Filter based on accuracy excluding CDC saturation (only show if CDC mode is enabled)
                        if (isCdcMode) {{
                            const filtered_based_on_acc_excluding_cdc_saturation = fileNames.map((name, i) => ({{
                                name, 
                                accuracy: accuracies[i],
                                accuracy_excluding_cdc_saturation: accuracies_excluding_cdc_saturation[i],
                                perc_of_scans_with_cdc_saturation: perc_of_scans_with_cdc_saturations[i],
                                perc_of_scans_with_range_saturation: perc_of_scans_with_range_saturations[i],
                                report_file_num: report_file_nums[i],
                                isBelow: accuracies_excluding_cdc_saturation[i] < validThreshold
                            }})).filter(item => item.isBelow);
                            
                            const count_based_on_acc_excluding_cdc_saturation = filtered_based_on_acc_excluding_cdc_saturation.length;

                            const tableHTML_based_on_acc_excluding_cdc_saturation = `
                                <h3 style="color:red;">Logs with overall accuracy(excluding CDC saturated scans) &lt; ${{validThreshold}}%  (No. of logs: ${{count_based_on_acc_excluding_cdc_saturation}})</h3>
                                <table style="width:80%; text-align: left; margin-top:20px; border-collapse:collapse;">
                                    <tr style="background:#f0f0f0">
                                        <th style="padding:10px">File Name</th>
                                        <th style="padding:10px">Accuracy</th>
                                        <th style="padding:10px">% of Scans with CDC Saturation</th>
                                        <th style="padding:10px">% of Scans with Range Saturation</th>
                                        <th style="padding:10px">Report File Number</th>
                                    </tr>
                                    ${{filtered_based_on_acc_excluding_cdc_saturation.map(item => `
                                        <tr>
                                            <td style="padding:10px; border-bottom:1px solid #ddd">${{item.name}}</td>
                                            <td style="padding:10px; border-bottom:1px solid #ddd">${{item.accuracy_excluding_cdc_saturation}}</td>
                                            <td style="padding:10px; border-bottom:1px solid #ddd">${{item.perc_of_scans_with_cdc_saturation}}</td>
                                            <td style="padding:10px; border-bottom:1px solid #ddd">${{item.perc_of_scans_with_range_saturation}}</td>
                                            <td style="padding:10px; border-bottom:1px solid #ddd">${{item.report_file_num}}</td>
                                        </tr>
                                    `).join('')}}
                                </table>
                            `;
                            document.getElementById('logs_based_on_acc_excluding_cdc_saturation').innerHTML = tableHTML_based_on_acc_excluding_cdc_saturation;
                        }} else {{
                            document.getElementById('logs_based_on_acc_excluding_cdc_saturation').innerHTML = "<em>CDC mode not enabled for this report.</em>";
                        }}
                        
                        // Table 3: Filter based on saturation
                        const filtered_based_on_saturation = fileNames.map((name, i) => ({{
                            name, 
                            accuracy: accuracies[i],
                            perc_of_scans_with_cdc_saturation: perc_of_scans_with_cdc_saturations[i],
                            perc_of_scans_with_range_saturation: perc_of_scans_with_range_saturations[i],
                            report_file_num: report_file_nums[i],
                            isSaturated: (perc_of_scans_with_cdc_saturations[i] != 0) || (perc_of_scans_with_range_saturations[i] != 0)
                        }})).filter(item => item.isSaturated);
                        
                        const count_based_on_saturation = filtered_based_on_saturation.length;

                        const tableHTML_based_on_saturation = `
                            <h3 style="color:red;">Logs with either CDC or Range saturation (No. of logs: ${{count_based_on_saturation}})</h3>
                            <table style="width:80%; text-align: left; margin-top:20px; border-collapse:collapse;">
                                <tr style="background:#f0f0f0">
                                    <th style="padding:10px">File Name</th>
                                    <th style="padding:10px">Accuracy</th>
                                    <th style="padding:10px">% of Scans with CDC Saturation</th>
                                    <th style="padding:10px">% of Scans with Range Saturation</th>
                                    <th style="padding:10px">Report File Number</th>
                                </tr>
                                ${{filtered_based_on_saturation.map(item => `
                                    <tr>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.name}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.accuracy}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.perc_of_scans_with_cdc_saturation}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.perc_of_scans_with_range_saturation}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.report_file_num}}</td>
                                    </tr>
                                `).join('')}}
                            </table>
                        `;
                        document.getElementById('logs_based_on_saturation').innerHTML = tableHTML_based_on_saturation;
                    }}

                    // Call updateTable when DOM is ready
                    if (document.readyState === 'loading') {{
                        document.addEventListener('DOMContentLoaded', updateTable);
                    }} else {{
                        updateTable();
                    }}
                </script>
                <hr width="100%" size="2" color="blue" noshade>
            """
            #################################

            plot_data_across_logs(data_across_log)
            
            session_vars.html_content += "</body></html>" # end of html content for a set of logs

            # Write HTML content to file
            output_html_file_name = f"{config.DET_HTML_FILE_NAME}_" \
                         f"{session_vars.script_start_timestamp}_" \
                         f"{config.RESIM_MODE}_" \
                         f"MODE_" \
                         f"{log_idx+1:04d}.html"                
            output_html = session_vars.output_folder + "/" + output_html_file_name
            with open(output_html, "w", encoding='utf-8') as f:
                f.write(session_vars.html_content)
                f.close()
                
            # start of html content for next set of logs
            session_vars.html_content = session_vars.html_header
        #################################
    end_time = time.time()
    execution_time = end_time - start_time
    logger.custom_print(f"\n[TIMING] Script execution completed in {execution_time:.2f} seconds ({execution_time/60:.2f} minutes)")

#####################################################################
# START
#####################################################################
# Check if all arguments are passed -- robust handling with auto fallback for edge cases
num_of_argv = len(sys.argv)  
proper_command_string = "Proper usage e.g.: python detection_matching_kpi_script.py log_path.txt meta_data.json C:\\Gitlab\\gen7v1_resim_kpi_scripts"
script_dir = os.path.dirname(os.path.abspath(__file__))
if num_of_argv < 3:
    print(f"[COMMAND WARNING]: Not all arguments provided. {proper_command_string}")
    print("[INFO] Attempting auto-discovery of log_path.txt and meta_data.json...")
    # Try script_dir and cwd
    candidates_log = [os.path.join(script_dir, "log_path.txt"), os.path.join(os.getcwd(), "log_path.txt")]
    candidates_meta = [os.path.join(script_dir, "meta_data.json"), os.path.join(os.getcwd(), "meta_data.json")]
    found_log = next((p for p in candidates_log if os.path.exists(p)), None)
    found_meta = next((p for p in candidates_meta if os.path.exists(p)), None)
    if found_log and found_meta:
        log_path_file = found_log
        meta_data_file = found_meta
        print(f"[AUTO] Using found files: {log_path_file}, {meta_data_file}")
    else:
        # Auto-create defaults if missing (edge case handling)
        print("[INFO] Auto-creating default log_path.txt and meta_data.json")
        log_path_file = os.path.join(script_dir, "log_path.txt")
        meta_data_file = os.path.join(script_dir, "meta_data.json")
        # Create log_path.txt pointing to script_dir itself (contains the CSVs)
        if not os.path.exists(log_path_file):
            with open(log_path_file, 'w') as f:
                f.write(script_dir)
            print(f"[AUTO] Created {log_path_file} -> {script_dir}")
        if not os.path.exists(meta_data_file):
            default_meta = {
                "version": "1.1",
                "SiL_Engine": "Default_Engine",
                "SW": "Default_SW",
                "RSP_SiL": "Default_RSP",
                "Tracker": "Default_Tracker",
                "Mode": "CDC",
                "Max_CDC_Records": 5016,
                "Range_Saturation_Thresh_Front_Radar": 135.0,
                "Range_Saturation_Thresh_Corner_Radar": 135.0
            }
            import json as _json
            with open(meta_data_file, 'w') as f:
                _json.dump(default_meta, f, indent=2)
            print(f"[AUTO] Created {meta_data_file}")
        # Also handle output folder default
        if num_of_argv == 1:
            session_vars.output_folder = script_dir
elif num_of_argv == 3:
    print(
        "\n[COMMAND WARNING]: Output path not provided. Output will be generated in the path provided in log_path.txt file. "
        + proper_command_string)
    log_path_file = sys.argv[1]
    meta_data_file = sys.argv[2]
    # Check if files exist
    if not os.path.exists(log_path_file):
        print(f"[ERROR]: File {log_path_file} does not exist.")
        sys.exit(1)
    if not os.path.exists(meta_data_file):
        print(f"[ERROR]: File {meta_data_file} does not exist.")
        sys.exit(1)
else:
    # num args >=4
    log_path_file = sys.argv[1]
    meta_data_file = sys.argv[2]
    if not os.path.exists(log_path_file):
        print(f"[ERROR]: File {log_path_file} does not exist.")
        sys.exit(1)
    if not os.path.exists(meta_data_file):
        print(f"[ERROR]: File {meta_data_file} does not exist.")
        sys.exit(1)

# For auto case, log_path_file/meta may already be set, ensure they exist
if 'log_path_file' not in locals() or not os.path.exists(log_path_file):
    # fallback to script_dir
    log_path_file = os.path.join(script_dir, "log_path.txt")
if 'meta_data_file' not in locals() or not os.path.exists(meta_data_file):
    meta_data_file = os.path.join(script_dir, "meta_data.json")

# Get the input folder path with robust handling
input_folder = ""
try:
    with open(log_path_file, 'r') as f:
        content = f.read().strip().splitlines()
        # Take first non-empty line
        for line in content:
            if line.strip():
                input_folder = line.strip()
                break
        if not input_folder:
            print("[ERROR]: Input folder path is not provided in log_path.txt. Falling back to script dir")
            input_folder = script_dir
except Exception as e:
    print(f"[ERROR] Failed to read {log_path_file}: {e}")
    input_folder = script_dir

# Validate input_folder exists, fallback to script_dir
if not os.path.isdir(input_folder):
    print(f"[WARNING] Input folder {input_folder} not found, using {script_dir}")
    input_folder = script_dir

# If output folder path is not provided by user, 
# then generate the output in the input folder itself
if(num_of_argv > 3):
    session_vars.output_folder = sys.argv[3]
else:
    # For auto case, if not already set, use input_folder
    if not session_vars.output_folder:
        session_vars.output_folder = input_folder
# Ensure output folder exists
if not os.path.isdir(session_vars.output_folder):
    try:
        os.makedirs(session_vars.output_folder, exist_ok=True)
        print(f"[INFO] Created output folder {session_vars.output_folder}")
    except Exception as e:
        print(f"[ERROR] Cannot create output folder {session_vars.output_folder}: {e}")
        session_vars.output_folder = script_dir


# Read meta data
metadata_dict = Metadata.from_file(meta_data_file).to_dict()
# Update config variables from metadata
config.RESIM_MODE = metadata_dict['Mode'].upper()
config.MAX_CDC_RECORDS = int(metadata_dict['Max_CDC_Records'])
config.RANGE_SATURATION_THRESHOLD_FRONT_RADAR = float(metadata_dict['Range_Saturation_Thresh_Front_Radar'])
config.RANGE_SATURATION_THRESHOLD_CORNER_RADAR = float(metadata_dict['Range_Saturation_Thresh_Corner_Radar'])
 
 
# Get current time in nanoseconds since the epoch
ns_since_epoch = time.time_ns()
# Convert to seconds and nanoseconds
seconds_since_epoch = ns_since_epoch // 1_000_000_000
nanoseconds = ns_since_epoch % 1_000_000_000
# Get the datetime part
dt = datetime.fromtimestamp(seconds_since_epoch)
# Format as YYYYMMDD_HHMMSS_NNNNNNNNN
session_vars.script_start_timestamp = dt.strftime('%Y%m%d_%H%M%S') + f'_{nanoseconds:09d}'
      
# Create the debug file
debug_file_name = config.DET_DEBUG_FILE_NAME + "_" + \
                session_vars.script_start_timestamp + "_" + \
                config.RESIM_MODE + "_MODE" + ".txt"
debug_file_path = session_vars.output_folder + "/" + debug_file_name
logger.init_debug_file(debug_file_path)

# Print input and output folder paths
logger.custom_print(f"\n[INFO] Input folder: {input_folder}")
logger.custom_print(f"[INFO] Output folder: {session_vars.output_folder}")

# Print meta data
logger.custom_print("\n[INFO] Meta data used:")
for key, value in metadata_dict.items():
    logger.custom_print(f"   * {key}: {value}")
#################################

#################################
# HTML Content
#################################
session_vars.html_header = f"""
<html>
<head>
    <title>{config.DET_FILE_TITLE}</title>
    <script src="https://cdn.plot.ly/plotly-2.27.0.min.js"></script>
</head>
<body>
    <h1>{config.DET_FILE_TITLE} {config.FILE_VERSION}</h1>
    <small>Unique key: {config.UNIQUE_KEY}</small>
    <ul>
        <li><b>SiL_Engine:</b> {metadata_dict['SiL_Engine']}</li>
        <li><b>SW:</b> {metadata_dict['SW']}</li>
        <li><b>RSP_SiL:</b> {metadata_dict['RSP_SiL']}</li>
        <li><b>Tracker:</b> {metadata_dict['Tracker']}</li>
        <li><b>Mode:</b> {metadata_dict['Mode']}</li>
    </ul> 
    <details>
        <summary><b><i>Glossary</i></b></summary>
        <ol>
            <li><b>Definition of match:</b>
                A detection is said to match a re-simulated detection if it has the same RDD range and doppler index(within a scan index), 
                and the difference(error) in the range, range rate, azimuth and elevation are within the thresholds mentioned below
                <ul>
                    <li>Range : {round(config.RAN_THRESHOLD, 5)} m</li>
                    <li>Range rate : {round(config.VEL_THRESHOLD, 5)} m/s</li>
                    <li>Azimuth : {round(config.THETA_THRESHOLD, 5)} radians</li>
                    <li>Elevation : {round(config.PHI_THRESHOLD, 5)} radians</li>
                </ul> 
            <li><b>Accuracy:</b> (Number of matching detections / total number of detections) * 100
            <li><b>Saturation:</b> A SI is said to be saturated if the maximum range among all the vehicle detections is >= 135m and re-simulated detections is < 135m
            <li><b>Plot A:</b> Plot of % of unmatched detections against the corresponding error. Unmatched detections are those 
            detections having same RDD index in both vehicle and re-simulated data but falling outside at-least one of the above 
            mentioned thresholds.
            <li><b>Plot B:</b> Plot of different errors against the corresponding detection's range, range rate, azimuth and elevation
            <li><b>Plot C:</b> Plot of different parameters across scan indices
        </ol>
        <b>Note:</b> The plots are interactive
        <br>
        <b>Note:</b> Blank plots indicate no data or zero error
    </details>    

    <hr width="100%" size="2" color="blue" noshade>
"""
session_vars.html_content = session_vars.html_header
#################################

if input_folder:
    try:
        data_files = find_det_related_data_files(input_folder)
        logger.custom_print(f"[INFO] Found {len(data_files['input'])} log(s) to process")
        if data_files['input'] and data_files['output']:
            try:
                process_logs(data_files)
            except Exception as e:
                import traceback
                logger.custom_print(f"[ERROR] process_logs failed: {e}")
                logger.custom_print(traceback.format_exc())
                # Ensure at least an HTML with error is written
                try:
                    session_vars.html_content += f"<h2 style='color:red'>Processing failed: {e}</h2><pre>{traceback.format_exc()}</pre>"
                    session_vars.html_content += "</body></html>"
                    # Write fallback html
                    fallback_html = session_vars.output_folder + f"/{config.DET_HTML_FILE_NAME}_{session_vars.script_start_timestamp}_{config.RESIM_MODE}_MODE_ERROR.html"
                    with open(fallback_html, "w", encoding='utf-8') as f:
                        f.write(session_vars.html_content)
                    print(f"[INFO] Wrote error HTML to {fallback_html}")
                except Exception as e2:
                    print(f"[ERROR] Failed to write error HTML: {e2}")
        else:
            msg = "Error: No logs to process - either the input and/or the output csvs are missing. Checked folder: " + input_folder
            print(msg)
            logger.custom_print(f"[ERROR] {msg}")
            # Still generate HTML with explanation
            session_vars.html_content += f"<h2 style='color:red'>{msg}</h2>"
            session_vars.html_content += "<p>Listing files in input folder:</p><ul>"
            try:
                for fname in os.listdir(input_folder):
                    session_vars.html_content += f"<li>{fname}</li>"
            except Exception as e:
                session_vars.html_content += f"<li>Failed to list folder: {e}</li>"
            session_vars.html_content += "</ul><p>Expected file suffixes: _UDP_GEN8_DET_CORE.csv and _UDP_GEN8_RDD_CORE.csv with paired resim files containing _rR00 or _r00 version</p>"
            session_vars.html_content += "</body></html>"
            fallback_html = session_vars.output_folder + f"/{config.DET_HTML_FILE_NAME}_{session_vars.script_start_timestamp}_{config.RESIM_MODE}_MODE_NOLOGS.html"
            try:
                with open(fallback_html, "w", encoding='utf-8') as f:
                    f.write(session_vars.html_content)
                logger.custom_print(f"[INFO] Wrote no-logs HTML to {fallback_html}")
                print(f"[INFO] Wrote no-logs HTML to {fallback_html}")
            except Exception as e:
                print(f"[ERROR] Failed to write fallback HTML: {e}")
    except Exception as e:
        import traceback
        print(f"[FATAL] Failed to find data files: {e}")
        logger.custom_print(f"[FATAL] {e}\n{traceback.format_exc()}")
else:
    print("[ERROR] Input folder empty, cannot process")
    logger.custom_print("[ERROR] Input folder empty, cannot process")
#####################################################################
# END
#####################################################################