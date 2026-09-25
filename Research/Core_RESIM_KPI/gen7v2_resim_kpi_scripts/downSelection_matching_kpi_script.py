#################################
# Packages
#################################
import sys
import os
import glob
import numpy as np
import pandas as pd
import plotly.graph_objs as go
import plotly.subplots as sp
import plotly.io as pio
from collections import Counter
import time
from datetime import datetime

# user defined
from constants import Constants
from meta_data import Metadata
from variables import Session_Vars, Log_Vars
from file_handling import check_file, find_ds_related_data_files
from logger import logger
from config import config

# properties
pd.options.display.width = 0
#################################

#################################
# Define class objects
#################################
session_vars = Session_Vars()
log_vars = Log_Vars()
#################################

def process_one_log(veh_csv, sim_csv) -> bool:

    log_vars.reset()
    
    logger.custom_print("[INFO] Reading the CSVs...")    
    #################################
    # Read Vehicle and Resim data
    # Not all scans available in vehicle DET csv will be available in sim DET csv
    #################################
    if (config.MAX_NUM_OF_SI_TO_PROCESS != 0):
        nrows = config.MAX_NUM_OF_SI_TO_PROCESS
    else:
        nrows = None

    logger.custom_print("[INFO] Reading DS CSV...")
    #################################
    # Read DS csv
    #################################
    ds_cols_of_interest = ['scan_index', 'num_af_ds_det']
    ds_cols_of_interest = ds_cols_of_interest + [item for i in range(config.MAX_NUM_OF_DS_DETS) for item in (f"rdd_idx_{i}", f"ran_{i}", f"vel_{i}", f"theta_{i}", f"phi_{i}")]
    veh_ds_df = pd.read_csv(veh_csv, usecols=ds_cols_of_interest, nrows=nrows, memory_map=True)  # type: ignore
    sim_ds_df = pd.read_csv(sim_csv, usecols=ds_cols_of_interest, nrows=nrows, memory_map=True)  # type: ignore
    # Keep only non-zero scan indices
    veh_ds_df = veh_ds_df[veh_ds_df['scan_index'] != 0]
    sim_ds_df = sim_ds_df[sim_ds_df['scan_index'] != 0]
    # Keep only those scans which have non-zero DS AF dets
    veh_ds_df = veh_ds_df[veh_ds_df['num_af_ds_det'] != 0]
    sim_ds_df = sim_ds_df[sim_ds_df['num_af_ds_det'] != 0]
    # Drop duplicates if any
    veh_ds_df.drop_duplicates(subset='scan_index', keep='first', inplace=True)
    sim_ds_df.drop_duplicates(subset='scan_index', keep='first', inplace=True)    
    # Check if there is data after above filtering
    log_vars.num_of_SI_in_veh_ds = veh_ds_df.shape[0]
    log_vars.num_of_SI_in_sim_ds = sim_ds_df.shape[0]
    if(0 == log_vars.num_of_SI_in_veh_ds):
        logger.custom_print("[WARNING] Filtered " + veh_csv + " is empty")
        return False
    if(0 == log_vars.num_of_SI_in_sim_ds):
        logger.custom_print("[WARNING] Filtered " + sim_csv + " is empty")
        return False
    
    # Merge Vehicle and Resim DS data
    final_df = pd.merge(veh_ds_df, sim_ds_df, on='scan_index', how='inner', suffixes=('_veh', '_sim'))
    if (config.MAX_NUM_OF_SI_TO_PROCESS != 0):
        final_df = final_df.iloc[:config.MAX_NUM_OF_SI_TO_PROCESS]
    log_vars.num_of_SI_in_veh_and_sim_ds = final_df.shape[0]
    log_vars.num_of_SI_with_same_num_of_dets_ds = final_df[final_df['num_af_ds_det_veh'] == final_df['num_af_ds_det_sim']].shape[0]
    
    # DS KPI    
    kpis_ds = {'result1':
                    {'numerator': log_vars.num_of_SI_with_same_num_of_dets_ds,
                     'denominator': log_vars.num_of_SI_in_veh_and_sim_ds,
                     'value': round((log_vars.num_of_SI_with_same_num_of_dets_ds / log_vars.num_of_SI_in_veh_and_sim_ds) * 100,2) if (log_vars.num_of_SI_in_veh_and_sim_ds != 0) else None},
              }

    # logger.custom_print(f"Number of SI in (vehicle, simulation): ({log_vars.num_of_SI_in_veh_af}, {log_vars.num_of_SI_in_sim_af})")
    # logger.custom_print(f"Number of same SI available in both vehicle and simulation: {log_vars.num_of_SI_in_veh_and_sim_af}")
    # logger.custom_print(f"% of SI with same number of AF detections: "
    #       f"{kpis_af['result1']['numerator']}/{kpis_af['result1']['denominator']} --> {kpis_af['result1']['value']}%")
    #################################
        
        
    logger.custom_print("[INFO] DS Detection Matching...")
    #################################
    # DS Stream matching
    #################################
    rdd_idx_cols_veh = [f"rdd_idx_{i}_veh" for i in range(config.MAX_NUM_OF_DS_DETS)]
    ran_cols_veh = [f"ran_{i}_veh" for i in range(config.MAX_NUM_OF_DS_DETS)]
    vel_cols_veh = [f"vel_{i}_veh" for i in range(config.MAX_NUM_OF_DS_DETS)]
    theta_cols_veh = [f"theta_{i}_veh" for i in range(config.MAX_NUM_OF_DS_DETS)]
    phi_cols_veh = [f"phi_{i}_veh" for i in range(config.MAX_NUM_OF_DS_DETS)]
    rdd_idx_cols_sim = [f"rdd_idx_{i}_sim" for i in range(config.MAX_NUM_OF_DS_DETS)]
    ran_cols_sim = [f"ran_{i}_sim" for i in range(config.MAX_NUM_OF_DS_DETS)]
    vel_cols_sim = [f"vel_{i}_sim" for i in range(config.MAX_NUM_OF_DS_DETS)]
    theta_cols_sim = [f"theta_{i}_sim" for i in range(config.MAX_NUM_OF_DS_DETS)]
    phi_cols_sim = [f"phi_{i}_sim" for i in range(config.MAX_NUM_OF_DS_DETS)]

    log_vars.scan_index_list = final_df['scan_index'].tolist()
    log_vars.num_ds_det_veh_list = final_df['num_af_ds_det_veh'].tolist()
    log_vars.num_ds_det_sim_list = final_df['num_af_ds_det_sim'].tolist()

    def find_max_range(row):
        max_range_veh = 0
        max_range_sim = 0

        max_range_veh = max(row[ran_cols_veh])
        max_range_sim = max(row[ran_cols_sim])

        return max_range_veh, max_range_sim

    final_df[['max_range_veh', 'max_range_sim']] = final_df.apply(find_max_range, axis=1, result_type="expand")
    log_vars.max_range_veh_list = final_df['max_range_veh'].tolist()
    log_vars.max_range_sim_list = final_df['max_range_sim'].tolist()

    def count_det_params_matches(row):
        """Count matches for detectionparameters within thresholds."""
        num_detect_veh = int(row['num_af_ds_det_veh'])
        num_detect_sim = int(row['num_af_ds_det_sim'])

        veh_rdd_indices = row[rdd_idx_cols_veh[:num_detect_veh]]
        sim_rdd_indices = row[rdd_idx_cols_sim[:num_detect_sim]]

        all_match_count = 0
        rdd_idx_not_matching = 0
        matched_sim_indices = set()
        for idx, veh_rdd_idx in enumerate(veh_rdd_indices):
            for jdx, sim_rdd_idx in enumerate(sim_rdd_indices):
                if jdx in matched_sim_indices:
                    continue
                if veh_rdd_idx == sim_rdd_idx:
                    sim_ran = row[ran_cols_sim[jdx]]
                    sim_vel = row[vel_cols_sim[jdx]]
                    sim_theta = row[theta_cols_sim[jdx]]
                    sim_phi = row[phi_cols_sim[jdx]]
                    veh_ran = row[ran_cols_veh[idx]]
                    veh_vel = row[vel_cols_veh[idx]]
                    veh_theta = row[theta_cols_veh[idx]]
                    veh_phi = row[phi_cols_veh[idx]]

                    ran_diff = veh_ran - sim_ran
                    ran_diff_abs = abs(ran_diff)
                    vel_diff = veh_vel - sim_vel
                    vel_diff_abs = abs(vel_diff)
                    theta_diff = veh_theta - sim_theta
                    theta_diff_abs = abs(theta_diff)
                    phi_diff = veh_phi - sim_phi
                    phi_diff_abs = abs(phi_diff)

                    if(ran_diff_abs <= config.RAN_THRESHOLD and vel_diff_abs <= config.VEL_THRESHOLD and 
                       theta_diff_abs <= config.THETA_THRESHOLD and phi_diff_abs <= config.PHI_THRESHOLD):
                        all_match_count += 1
                        matched_sim_indices.add(jdx)
                        break

                    if (ran_diff_abs > config.RAN_THRESHOLD):
                        log_vars.ran_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, ran_diff))
                    if (vel_diff_abs > config.VEL_THRESHOLD):
                        log_vars.vel_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, vel_diff))
                    if (theta_diff_abs > config.THETA_THRESHOLD):
                        log_vars.theta_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, theta_diff))
                    if (phi_diff_abs > config.PHI_THRESHOLD):
                        log_vars.phi_diff_list.append((veh_ran, veh_vel, veh_theta, veh_phi, phi_diff))
                else:
                    rdd_idx_not_matching += 1

        return all_match_count

    final_df['det_all_params_match_count'] = final_df.apply(count_det_params_matches, axis=1, result_type="expand")

    final_df['same_num_of_DS_detections'] = final_df['num_af_ds_det_veh'] == final_df['num_af_ds_det_sim']
    final_df['matching_pct_det_all_params'] = final_df['det_all_params_match_count']/final_df['num_af_ds_det_veh']
    log_vars.accuracy_list = final_df['matching_pct_det_all_params'].tolist()

    num_of_ds_dets_in_veh = sum(final_df['num_af_ds_det_veh'])
    num_of_ds_dets_in_sim = sum(final_df['num_af_ds_det_sim'])
    num_of_dets_with_matching_rvtp = sum(final_df['det_all_params_match_count'])
    num_of_SI_with_matching_rvtp = final_df[(final_df['matching_pct_det_all_params'] >= 0.99)].shape[0]

    if num_of_ds_dets_in_veh != 0:
        log_vars.overall_accuracy = round((num_of_dets_with_matching_rvtp / num_of_ds_dets_in_veh) * 100, 2)
        log_vars.min_accuracy = round(min(log_vars.accuracy_list)*100, 2)
    else:
        log_vars.overall_accuracy = 0
        log_vars.min_accuracy = 0

    kpis_final = {'result1':
                    {'numerator': num_of_SI_with_matching_rvtp,
                     'denominator': log_vars.num_of_SI_in_veh_and_sim_ds,
                     'value': round((num_of_SI_with_matching_rvtp / log_vars.num_of_SI_in_veh_and_sim_ds) * 100, 2) if (log_vars.num_of_SI_in_veh_and_sim_ds != 0) else None},
                'result2':
                    {'numerator': num_of_dets_with_matching_rvtp,
                     'denominator': num_of_ds_dets_in_veh,
                     'value': round((num_of_dets_with_matching_rvtp / num_of_ds_dets_in_veh) * 100, 2) if (num_of_ds_dets_in_veh != 0) else None},
                }

    
    #################################
    # Print data related to this log
    #################################
    logger.custom_print(f"[KPI] Accuracy: {log_vars.overall_accuracy}")
    
    #################################
    # HTML Content
    #################################
    session_vars.html_content += f"""
    <b>KPI:</b> Accuracy: ({kpis_final['result2']['numerator']}/{kpis_final['result2']['denominator']}) --> <b>{kpis_final['result2']['value']}%</b>
    <details>
        <summary><i>Details</i></summary>
        <b>DS Streams</b><br>
        Number of SI in (vehicle, simulation) : {log_vars.num_of_SI_in_veh_ds}, {log_vars.num_of_SI_in_sim_ds}<br>
        Number of same SI available in both vehicle and simulation: {log_vars.num_of_SI_in_veh_and_sim_ds}<br>
        % of SI with same number of DS detections: 
        ({kpis_ds['result1']['numerator']}/{kpis_ds['result1']['denominator']}) --> {kpis_ds['result1']['value']}%<br>
        % of SI with 99% matching det params(r,v,t,p):
        ({kpis_final['result1']['numerator']}/{kpis_final['result1']['denominator']}) --> {kpis_final['result1']['value']}%
        <br>
        Number of detections in (vehicle, simulation): {num_of_ds_dets_in_veh}, {num_of_ds_dets_in_sim}<br>
        Accuracy(r,v,t,p):
        ({kpis_final['result2']['numerator']}/{kpis_final['result2']['denominator']}) --> {kpis_final['result2']['value']}%
        <br>
    """
    #################################
    return True

def func_bar(diffs):
    # Count the frequency of each unique value
    counts = Counter(diffs)
    # Total number of observations
    total_observations = len(diffs)
    # Calculate percentages
    percentages = {key: (value / total_observations) * 100 for key, value in counts.items()}
    sorted_dict = dict(sorted(percentages.items()))
    # Return Bar trace
    return go.Bar(x=list(sorted_dict.keys()), y=list(sorted_dict.values()))

def func_scatter(x, y):
    # Return Scatter trace
    return go.Scatter(x=x, y=y, mode='markers')

def plot_stats():

    rng_idx = 0
    vel_idx = 1
    theta_idx = 2
    phi_idx = 3
    error_idx = 4
    ran_diffs = np.array([ele[error_idx] for ele in log_vars.ran_diff_list]).round(3)
    vel_diffs = np.array([ele[error_idx] for ele in log_vars.vel_diff_list]).round(3)
    theta_diffs = np.array([ele[error_idx] for ele in log_vars.theta_diff_list]).round(3)
    phi_diffs = np.array([ele[error_idx] for ele in log_vars.phi_diff_list]).round(3)

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
    fig_scatter = sp.make_subplots(rows=4, cols=4, horizontal_spacing=0.03, vertical_spacing=0.04)

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
    fig_line = sp.make_subplots(rows=3, cols=1, horizontal_spacing=0.04, vertical_spacing=0.1)

    # Manually add traces for line plots
    row_num = 1
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.accuracy_list, mode="lines", name="Accuracy", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Accuracy", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    row_num = 2
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.num_ds_det_veh_list, mode="lines", name="Veh", line=dict(color="red")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.num_ds_det_sim_list, mode="lines", name="Sim", line=dict(color="blue")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Num of dets", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    row_num = 3
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.max_range_veh_list, mode="lines", name="Veh", line=dict(color="red")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=log_vars.scan_index_list, y=log_vars.max_range_sim_list, mode="lines", name="Sim", line=dict(color="blue")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Max range", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    # Update layout for plot
    fig_line.update_layout(height=1000, width=1250, title_text="Params v/s scanindex", showlegend=True)
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
    
    #################################
    # Create sub-plot for different data across logs:
    #################################
    # Calculate number of rows dynamically based on CDC mode
    num_rows = 1  # Base rows: overall accuracy
    
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

        # Each log should have 1 csv files.
        input_ds_file = data_files['input'][log_idx]
        output_ds_file = data_files['output'][log_idx]

        # Extract the required file name
        # input_det_file: 'C:\\Project\\Logs\\HGR770_Gen7v2_20250507_164906_001_ORCAS_FC_UDP_GEN7_DET_CORE.csv'
        # det_log_name: 'HGR770_Gen7v2_ASTA_cpna50_40kph_20250507_164906_001_ORCAS_FC_UDP_GEN7_DET_CORE.csv'
        # det_log_name_wo_ext: 'HGR770_Gen7v2_ASTA_cpna50_40kph_20250507_164906_001_ORCAS_FC'
        ds_log_name = os.path.basename(input_ds_file)
        ds_log_name_wo_ext = ds_log_name.replace(config.DS_FILE_SUFFIX, "")
        logger.custom_print(f"\n[INFO] Processing log: {log_idx+1}/{num_of_logs} : {ds_log_name_wo_ext}")

        #################################
        # check if the csv files exists.
        # If any of the csv files have issues, then simply continue on to next log
        #################################
        if not check_file(input_ds_file):
            continue
        if not check_file(output_ds_file):
            continue
        #################################

        session_vars.html_content += f"""
                <b>Log:</b> {ds_log_name_wo_ext};
                """
        # Few variables are different for front and corner radars, 
        # hence, it has to be updated for every log based on the log name
        if '_FC_UDP_' in ds_log_name:
            config.MAX_NUM_OF_DS_DETS = config.MAX_NUM_OF_DS_DETS_FRONT_RADAR
        else:
            config.MAX_NUM_OF_DS_DETS = config.MAX_NUM_OF_DS_DETS_CORNER_RADAR

        #################################
        # Main function for each log
        #################################
        log_start_time = time.time()    
        status = process_one_log(input_ds_file, output_ds_file)        
        log_end_time = time.time()
        log_execution_time = log_end_time - log_start_time
        logger.custom_print(f"[TIMING] Log {ds_log_name_wo_ext} processed in {log_execution_time:.2f} seconds")
        #################################

        # Save the data and plot data only if the log was successfully processed
        if status:
            #################################
            # Save required data of each log
            #################################
            data_dict = {
                "ds_log_name_wo_ext": ds_log_name_wo_ext,
                "overall_accuracy": log_vars.overall_accuracy,              
                "log_idx": log_idx}

            found_sensor = False
            for sensor in ['FC', 'FL', 'FR', 'RL', 'RR']:
                if ('_' + sensor + '_UDP_') in ds_log_name:
                    data_across_log[sensor].append(data_dict)
                    found_sensor = True
                    break
            if not found_sensor:
                logger.custom_print(f"[WARNING] Unsupported sensor position in {ds_log_name}")
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
                
            session_vars.html_content += """
                <hr width="100%" size="2" color="blue" noshade> 
            """
            #################################
            
            #################################
            # Table for failed logs   
            #################################   
            log_name_list = []
            overall_accuracy_list = []
            report_file_num_list = []

            for sensor in data_across_log:
                for data in data_across_log[sensor]:
                    log_name_list.append(data['ds_log_name_wo_ext'])
                    overall_accuracy_list.append(float(data['overall_accuracy']))
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
                
                <script>
                    const fileNames = {repr(log_name_list)};
                    const accuracies = {repr(overall_accuracy_list)};
                    const report_file_nums = {repr(report_file_num_list)};
                    const isCdcMode = {str(session_vars.is_cdc_mode).lower()};
                    
                    function updateTable() {{
                        const threshold = document.getElementById('threshold') ? parseFloat(document.getElementById('threshold').value) : {config.DEFAULT_ACC_THRESHOLD};
                        const validThreshold = isNaN(threshold) ? {config.DEFAULT_ACC_THRESHOLD} : threshold;
                        
                        // Check if we have data
                        if (fileNames.length === 0) {{
                            document.getElementById('logs_based_on_acc').innerHTML = "<em>No log data available for filtering.</em>";
                            return;
                        }}
                        
                        // Table 1: Filter based on accuracy
                        const filtered_based_on_acc = fileNames.map((name, i) => ({{
                            name, 
                            accuracy: accuracies[i],
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
                                    <th style="padding:10px">Report File Number</th>
                                </tr>
                                ${{filtered_based_on_acc.map(item => `
                                    <tr>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.name}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.accuracy}}</td>
                                        <td style="padding:10px; border-bottom:1px solid #ddd">${{item.report_file_num}}</td>
                                    </tr>
                                `).join('')}}
                            </table>
                        `;
                        document.getElementById('logs_based_on_acc').innerHTML = tableHTML_based_on_acc;
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
            output_html_file_name = f"{config.DS_HTML_FILE_NAME}_" \
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
# Check if all arguments are passed
num_of_argv = len(sys.argv)  
proper_command_string = "Proper usage e.g.: python downSelection_matching_kpi_script.py log_path.txt meta_data.json C:\\Gitlab\\gen7v1_resim_kpi_scripts"
if num_of_argv < 3:
    print("[COMMAND ERROR]: Not all arguments provided. " 
          + proper_command_string)
    sys.exit(1)
elif num_of_argv == 3:
    print(
        "\n[COMMAND WARNING]: Output path not provided. Output will be generated in the path provided in log_path.txt file. "
        + proper_command_string)
else:
    pass

log_path_file = sys.argv[1]
meta_data_file = sys.argv[2]

# Check if log_path and meta_data files exists
if not os.path.exists(log_path_file):
    print(f"[ERROR]: File {log_path_file} does not exist.")
    sys.exit(1)
if not os.path.exists(meta_data_file):
    print(f"[ERROR]: File {meta_data_file} does not exist.")
    sys.exit(1)
    
# Get the input folder path
input_folder = ""
with open(log_path_file, 'r') as f:
    input_folder = f.readline().strip()
    if not input_folder:
        print("[ERROR]: Input folder path is not povided in log_path.txt.")
        sys.exit(1)        

# If output folder path is not provided by user, 
# then generate the output in the input folder itself
if(num_of_argv > 3):
    session_vars.output_folder = sys.argv[3]
else:
    session_vars.output_folder = input_folder


# Read meta data
metadata_dict = Metadata.from_file(meta_data_file).to_dict()
# Update config variables from metadata
config.RESIM_MODE = metadata_dict['Mode'].upper()
config.MAX_CDC_RECORDS = int(metadata_dict['Max_CDC_Records'])
config.RANGE_SATURATION_THRESHOLD_FRONT_RADAR = float(metadata_dict['Range_Saturation_Thresh_Front_Radar'])
config.RANGE_SATURATION_THRESHOLD_CORNER_RADAR = float(metadata_dict['Range_Saturation_Thresh_Corner_Radar'])
config.MAX_NUM_OF_DS_DETS_FRONT_RADAR = int(metadata_dict['Max_DS_Detections_Front_Radar'])
config.MAX_NUM_OF_DS_DETS_CORNER_RADAR = int(metadata_dict['Max_DS_Detections_Corner_Radar'])
 
 
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
debug_file_name = config.DS_DEBUG_FILE_NAME + "_" + \
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
    <title>{config.DS_FILE_TITLE}</title>
    <script src="https://cdn.plot.ly/plotly-2.27.0.min.js"></script>
</head>
<body>
    <h1>{config.DS_FILE_TITLE} {config.FILE_VERSION}</h1>
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
                A detection is said to match a re-simulated detection if it has the same RDD index and the 
                difference(error) in the range, range rate, azimuth and elevation are within the thresholds mentioned below
                <ul>
                    <li>Range : {round(config.RAN_THRESHOLD, 5)} m</li>
                    <li>Range rate : {round(config.VEL_THRESHOLD, 5)} m/s</li>
                    <li>Azimuth : {round(config.THETA_THRESHOLD, 5)} radians</li>
                    <li>Elevation : {round(config.PHI_THRESHOLD, 5)} radians</li>
                </ul> 
            <li><b>Accuracy:</b> (Number of matching detections / total number of detections) * 100
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
    data_files = find_ds_related_data_files(input_folder)
    # TODO ANANTHESH: Do we need to check for input logs availability here itself?
    if data_files['input'] and data_files['output']:
        process_logs(data_files)
    else:
        print("Error: No logs to process - either the input and/or the output csvs are missing.")
#####################################################################
# END
#####################################################################