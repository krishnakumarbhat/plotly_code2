#################################
# Packages
#################################
import sys
import os
import re
import numpy as np
import pandas as pd
import plotly.graph_objs as go
import plotly.subplots as sp
import plotly.io as pio
import time
from datetime import datetime

# user defined
from meta_data import Metadata
from variables import Session_Vars
from file_handling import check_file, find_rc_related_data_files
from logger import logger
from config import config

# properties
pd.options.display.width = 0
#################################

#################################
# Define class
#################################
class RadarCapability_Vars:
    def __init__(self):
        self._initialize()
      
    def reset(self):
        self._initialize()
      
    def _initialize(self):
        self.scan_index_list = []
        self.blockage_probability_real_list = []
        self.blockage_probability_sim_list = []
        self.blockage_value_real_list = []
        self.blockage_value_sim_list = []
        self.blockage_status_real_list = []
        self.blockage_status_sim_list = []
        self.dm_status_real_list = []
        self.dm_status_sim_list = []
        self.dm_current_dB_real_list = []
        self.dm_current_dB_sim_list = []
        self.re_max_det_range_car_0_real_list = []
        self.re_max_det_range_car_0_sim_list = []
        self.re_max_det_range_car_1_real_list = []
        self.re_max_det_range_car_1_sim_list = []
        self.re_max_det_range_car_2_real_list = []
        self.re_max_det_range_car_2_sim_list = []
        self.re_max_det_range_motorbike_0_real_list = []
        self.re_max_det_range_motorbike_0_sim_list = []
        self.re_max_det_range_motorbike_1_real_list = []
        self.re_max_det_range_motorbike_1_sim_list = []
        self.re_max_det_range_motorbike_2_real_list = []
        self.re_max_det_range_motorbike_2_sim_list = []
        self.re_max_det_range_pedestrian_0_real_list = []
        self.re_max_det_range_pedestrian_0_sim_list = []
        self.re_max_det_range_pedestrian_1_real_list = []
        self.re_max_det_range_pedestrian_1_sim_list = []
        self.re_max_det_range_pedestrian_2_real_list = []
        self.re_max_det_range_pedestrian_2_sim_list = []
        self.re_status_real_list = []
        self.re_status_sim_list = []        
        self.side_lobe_level_0_real_list = []
        self.side_lobe_level_0_sim_list = []
        self.side_lobe_level_1_real_list = []
        self.side_lobe_level_1_sim_list = []
        self.side_lobe_level_2_real_list = []
        self.side_lobe_level_2_sim_list = []
        self.sll_status_real_list = []
        self.sll_status_sim_list = []
        
#################################

#################################
# Define class objects
#################################
session_vars = Session_Vars()
rc_vars = RadarCapability_Vars()
#################################

def main(veh_csv_list, sim_csv_list) -> bool:

    logger.custom_print("[INFO] Reading the CSVs...")
    #################################
    # Read Vehicle and Resim data
    #################################
    cols_of_interest = [
        "scan_index",
        "probability",
        "value",
        "blockage_status",
        "dm_status",
        "current_dB",
        "max_det_range_car_0",
        "max_det_range_car_1",
        "max_det_range_car_2",
        "max_det_range_motorbike_0",
        "max_det_range_motorbike_1",
        "max_det_range_motorbike_2",
        "max_det_range_pedestrian_0",
        "max_det_range_pedestrian_1",
        "max_det_range_pedestrian_2",
        "re_status",
        "side_lobe_level_0",
        "side_lobe_level_1",
        "side_lobe_level_2",        
        "sll_status",
    ]

    veh_df = pd.concat([pd.read_csv(file, usecols=cols_of_interest) for file in veh_csv_list])
    sim_df = pd.concat([pd.read_csv(file, usecols=cols_of_interest) for file in sim_csv_list])
    veh_df = veh_df[veh_df['scan_index'] != 0]
    sim_df = sim_df[sim_df['scan_index'] != 0]
    number_of_scans_in_real = veh_df.shape[0]
    number_of_scans_in_sim = sim_df.shape[0]
    #################################

    logger.custom_print("[INFO] Merging the dataframes...")
    #################################
    # Merge Vehicle and Resim data
    #################################
    scan_index = "scan_index"
    final_df = pd.merge(veh_df, sim_df, on=scan_index, how="inner", suffixes=("_real", "_sim"))
    final_df = final_df.iloc[1:]
    number_of_scans_in_both_real_and_sim = final_df.shape[0]
    if(0 == number_of_scans_in_both_real_and_sim):
        logger.custom_print("[WARNING] Filtered vehicle data is empty")
        return False
    #################################

    #################################
    # Get the required parameters
    #################################
    if (config.MAX_NUM_OF_SI_TO_PROCESS != 0):
        final_df = final_df.iloc[:config.MAX_NUM_OF_SI_TO_PROCESS]
    
    rc_vars.scan_index_list = final_df["scan_index"].values.tolist()
    rc_vars.blockage_probability_real_list = final_df["probability_real"].values.tolist()
    rc_vars.blockage_probability_sim_list = final_df["probability_sim"].values.tolist()
    rc_vars.blockage_value_real_list = final_df["value_real"].values.tolist()
    rc_vars.blockage_value_sim_list = final_df["value_sim"].values.tolist()
    rc_vars.blockage_status_real_list = final_df["blockage_status_real"].values.tolist()
    rc_vars.blockage_status_sim_list = final_df["blockage_status_sim"].values.tolist()
    rc_vars.dm_current_dB_real_list = final_df["current_dB_real"].values.tolist()
    rc_vars.dm_current_dB_sim_list = final_df["current_dB_sim"].values.tolist()
    rc_vars.dm_status_real_list = final_df["dm_status_real"].values.tolist()
    rc_vars.dm_status_sim_list = final_df["dm_status_sim"].values.tolist()
    rc_vars.re_max_det_range_car_0_real_list = final_df["max_det_range_car_0_real"].values.tolist()
    rc_vars.re_max_det_range_car_0_sim_list = final_df["max_det_range_car_0_sim"].values.tolist()
    rc_vars.re_max_det_range_car_1_real_list = final_df["max_det_range_car_1_real"].values.tolist()
    rc_vars.re_max_det_range_car_1_sim_list = final_df["max_det_range_car_1_sim"].values.tolist()
    rc_vars.re_max_det_range_car_2_real_list = final_df["max_det_range_car_2_real"].values.tolist()
    rc_vars.re_max_det_range_car_2_sim_list = final_df["max_det_range_car_2_sim"].values.tolist()
    rc_vars.re_max_det_range_motorbike_0_real_list = final_df["max_det_range_motorbike_0_real"].values.tolist()
    rc_vars.re_max_det_range_motorbike_0_sim_list = final_df["max_det_range_motorbike_0_sim"].values.tolist()
    rc_vars.re_max_det_range_motorbike_1_real_list = final_df["max_det_range_motorbike_1_real"].values.tolist()
    rc_vars.re_max_det_range_motorbike_1_sim_list = final_df["max_det_range_motorbike_1_sim"].values.tolist()
    rc_vars.re_max_det_range_motorbike_2_real_list = final_df["max_det_range_motorbike_2_real"].values.tolist()
    rc_vars.re_max_det_range_motorbike_2_sim_list = final_df["max_det_range_motorbike_2_sim"].values.tolist()
    rc_vars.re_max_det_range_pedestrian_0_real_list = final_df["max_det_range_pedestrian_0_real"].values.tolist()
    rc_vars.re_max_det_range_pedestrian_0_sim_list = final_df["max_det_range_pedestrian_0_sim"].values.tolist()
    rc_vars.re_max_det_range_pedestrian_1_real_list = final_df["max_det_range_pedestrian_1_real"].values.tolist()
    rc_vars.re_max_det_range_pedestrian_1_sim_list = final_df["max_det_range_pedestrian_1_sim"].values.tolist()
    rc_vars.re_max_det_range_pedestrian_2_real_list = final_df["max_det_range_pedestrian_2_real"].values.tolist()
    rc_vars.re_max_det_range_pedestrian_2_sim_list = final_df["max_det_range_pedestrian_2_sim"].values.tolist()
    rc_vars.re_status_real_list = final_df["re_status_real"].values.tolist()
    rc_vars.re_status_sim_list = final_df["re_status_sim"].values.tolist()
    rc_vars.side_lobe_level_0_real_list = final_df["side_lobe_level_0_real"].values.tolist()
    rc_vars.side_lobe_level_0_sim_list = final_df["side_lobe_level_0_sim"].values.tolist()
    rc_vars.side_lobe_level_1_real_list = final_df["side_lobe_level_1_real"].values.tolist()
    rc_vars.side_lobe_level_1_sim_list = final_df["side_lobe_level_1_sim"].values.tolist()
    rc_vars.side_lobe_level_2_real_list = final_df["side_lobe_level_2_real"].values.tolist()
    rc_vars.side_lobe_level_2_sim_list = final_df["side_lobe_level_2_sim"].values.tolist()
    rc_vars.sll_status_real_list = final_df["sll_status_real"].values.tolist()
    rc_vars.sll_status_sim_list = final_df["sll_status_sim"].values.tolist()
        
    final_df["blockage_status_diff"] = final_df["blockage_status_real"] - final_df["blockage_status_sim"]
    final_df["dm_status_diff"] = final_df["dm_status_real"] - final_df["dm_status_sim"]
    final_df["re_status_diff"] = final_df["re_status_real"] - final_df["re_status_sim"]
    final_df["sll_status_diff"] = final_df["sll_status_real"] - final_df["sll_status_sim"]
    
    number_of_scans_with_matching_blockage_status = len(final_df[abs(final_df["blockage_status_diff"]) <= config.RC_STATUS_THRESHOLD])
    number_of_scans_with_matching_dm_status = len(final_df[abs(final_df["dm_status_diff"]) <= config.RC_STATUS_THRESHOLD])
    number_of_scans_with_matching_re_status = len(final_df[abs(final_df["re_status_diff"]) <= config.RC_STATUS_THRESHOLD])
    number_of_scans_with_matching_sll_status = len(final_df[abs(final_df["sll_status_diff"]) <= config.RC_STATUS_THRESHOLD])
    #################################

    def calculate_kpi(numerator, denominator, kpi_name):
        """Calculate KPI and return a dictionary with the results."""
        value = round((numerator / denominator) * 100, 2) if denominator != 0 else 0
        return {
            'numerator': numerator,
            'denominator': denominator,
            'value': value,
            'name': kpi_name
        }

    def append_kpi_to_html(kpi):
        """Append KPI details to the HTML content."""
        session_vars.html_content += f"""
            {kpi['name']} Accuracy: ({kpi['numerator']}/{kpi['denominator']}) --> <b>{kpi['value']}%;</b>
        """

    #################################
    # Calculate KPIs
    #################################
    kpis_rc = {
        'blockage': calculate_kpi(
            number_of_scans_with_matching_blockage_status,
            number_of_scans_in_both_real_and_sim,
            "Blockage"
        ),
        'dm': calculate_kpi(
            number_of_scans_with_matching_dm_status,
            number_of_scans_in_both_real_and_sim,
            "DM"
        ),
        're': calculate_kpi(
            number_of_scans_with_matching_re_status,
            number_of_scans_in_both_real_and_sim,
            "RE"
        ),
        'sll': calculate_kpi(
            number_of_scans_with_matching_sll_status,
            number_of_scans_in_both_real_and_sim,
            "SLL"
        )
    }

    # Append KPIs to HTML content
    session_vars.html_content += """<b>KPI:</b>"""
    
    for kpi_key in kpis_rc:
        append_kpi_to_html(kpis_rc[kpi_key])
    #################################
        
    return True
    #################################

def plot_stats():
    #############################################################################################################
    # Create subplots for Blockage v/s scanindex:
    #############################################################################################################
    fig_line = sp.make_subplots(rows=3, cols=1, horizontal_spacing=0.04, vertical_spacing=0.18)

    # Manually add traces for line plots        
    row_num = 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.blockage_status_real_list, mode="lines", name="Veh Blockage Status", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.blockage_status_sim_list, mode="lines", name="Sim Blockage Status", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Blockage Status", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)
    
    row_num = row_num + 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.blockage_probability_real_list, mode="lines", name="Veh Blockage Probability", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.blockage_probability_sim_list, mode="lines", name="Sim Blockage Probability", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Blockage Probability", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)
    
    row_num = row_num + 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.blockage_value_real_list, mode="lines", name="Veh Blockage Value", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.blockage_value_sim_list, mode="lines", name="Sim Blockage Value", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Blockage Value", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    # Update layout for bar plot
    fig_line.update_layout(height=700, width=1400, title_text="Blockage v/s scanindex", showlegend=True)
    fig_line.update_traces(marker_color="red")
    fig_line.update_xaxes(zeroline=False, showgrid=False, type="category")
    fig_line.update_yaxes(zeroline=False, showgrid=False)
    #fig_line.show()

    rc_blockage_line_plot_html = pio.to_html(fig_line, full_html=False, include_plotlyjs=False)
    #############################################################################################################
    
    #############################################################################################################
    # Create subplots for Damping v/s scanindex:
    #############################################################################################################
    fig_line = sp.make_subplots(rows=2, cols=1, horizontal_spacing=0.04, vertical_spacing=0.3)

    # Manually add traces for line plots        
    row_num = 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.dm_status_real_list, mode="lines", name="Veh Damping Status", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.dm_status_sim_list, mode="lines", name="Sim Damping Status", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Damping Status", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)
    
    row_num = row_num + 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.dm_current_dB_real_list, mode="lines", name="Veh Damping Current", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.dm_current_dB_sim_list, mode="lines", name="Sim Damping Current", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="Damping Current", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    # Update layout for bar plot
    fig_line.update_layout(height=500, width=1400, title_text="Damping v/s scanindex", showlegend=True)
    fig_line.update_traces(marker_color="red")
    fig_line.update_xaxes(zeroline=False, showgrid=False, type="category")
    fig_line.update_yaxes(zeroline=False, showgrid=False)
    #fig_line.show()

    rc_dm_line_plot_html = pio.to_html(fig_line, full_html=False, include_plotlyjs=False)
    #############################################################################################################
    
    #############################################################################################################
    # Create subplots for RangeEstimation v/s scanindex:
    #############################################################################################################
    fig_line = sp.make_subplots(rows=4, cols=1, horizontal_spacing=0.04, vertical_spacing=0.12)

    # Manually add traces for line plots        
    row_num = 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_status_real_list, mode="lines", name="Veh RE Status", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_status_sim_list, mode="lines", name="Sim RE Status", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="RE Status", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)
    
    row_num = row_num + 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_car_0_real_list, mode="lines", name="Veh RE MaxDetRangeCar0", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_car_1_real_list, mode="lines", name="Veh RE MaxDetRangeCar1", line=dict(color="cornflowerblue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_car_2_real_list, mode="lines", name="Veh RE MaxDetRangeCar2", line=dict(color="skyblue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_car_0_sim_list, mode="lines", name="Sim RE MaxDetRangeCar0", line=dict(color="red")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_car_1_sim_list, mode="lines", name="Sim RE MaxDetRangeCar1", line=dict(color="crimson")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_car_2_sim_list, mode="lines", name="Sim RE MaxDetRangeCar2", line=dict(color="maroon")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="RE MaxDetRangeCar", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)
    
    row_num = row_num + 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_motorbike_0_real_list, mode="lines", name="Veh RE MaxDetRangeMotorbike0", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_motorbike_1_real_list, mode="lines", name="Veh RE MaxDetRangeMotorbike1", line=dict(color="cornflowerblue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_motorbike_2_real_list, mode="lines", name="Veh RE MaxDetRangeMotorbike2", line=dict(color="skyblue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_motorbike_0_sim_list, mode="lines", name="Sim RE MaxDetRangeMotorbike0", line=dict(color="red")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_motorbike_1_sim_list, mode="lines", name="Sim RE MaxDetRangeMotorbike1", line=dict(color="crimson")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_motorbike_2_sim_list, mode="lines", name="Sim RE MaxDetRangeMotorbike2", line=dict(color="maroon")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="RE MaxDetRangeMotorbike", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)
    
    row_num = row_num + 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_pedestrian_0_real_list, mode="lines", name="Veh RE MaxDetRangePedestrian0", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_pedestrian_1_real_list, mode="lines", name="Veh RE MaxDetRangePedestrian1", line=dict(color="cornflowerblue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_pedestrian_2_real_list, mode="lines", name="Veh RE MaxDetRangePedestrian2", line=dict(color="skyblue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_pedestrian_0_sim_list, mode="lines", name="Sim RE MaxDetRangePedestrian0", line=dict(color="red")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_pedestrian_1_sim_list, mode="lines", name="Sim RE MaxDetRangePedestrian1", line=dict(color="crimson")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.re_max_det_range_pedestrian_2_sim_list, mode="lines", name="Sim RE MaxDetRangePedestrian2", line=dict(color="maroon")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="RE MaxDetRangePedestrian", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    # Update layout for bar plot
    fig_line.update_layout(height=1000, width=1400, title_text="RE v/s scanindex", showlegend=True)
    fig_line.update_traces(marker_color="red")
    fig_line.update_xaxes(zeroline=False, showgrid=False, type="category")
    fig_line.update_yaxes(zeroline=False, showgrid=False)
    #fig_line.show()

    rc_re_line_plot_html = pio.to_html(fig_line, full_html=False, include_plotlyjs=False)
    #############################################################################################################
    
    #############################################################################################################
    # Create subplots for SideLobeLevel v/s scanindex:
    #############################################################################################################
    fig_line = sp.make_subplots(rows=2, cols=1, horizontal_spacing=0.04, vertical_spacing=0.3)

    # Manually add traces for line plots        
    row_num = 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.sll_status_real_list, mode="lines", name="Veh SLL Status", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.sll_status_sim_list, mode="lines", name="Sim SLL Status", line=dict(color="red")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="SLL Status", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)
    
    row_num = row_num + 1
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.side_lobe_level_0_real_list, mode="lines", name="Veh SLL FovRegion0", line=dict(color="blue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.side_lobe_level_1_real_list, mode="lines", name="Veh SLL FovRegion1", line=dict(color="cornflowerblue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.side_lobe_level_2_real_list, mode="lines", name="Veh SLL FovRegion2", line=dict(color="skyblue")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.side_lobe_level_0_sim_list, mode="lines", name="Sim SLL FovRegion0", line=dict(color="red")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.side_lobe_level_1_sim_list, mode="lines", name="Sim SLL FovRegion1", line=dict(color="crimson")), row=row_num, col=1)
    fig_line.add_trace(go.Scatter(x=rc_vars.scan_index_list, y=rc_vars.side_lobe_level_2_sim_list, mode="lines", name="Sim SLL FovRegion2", line=dict(color="maroon")), row=row_num, col=1)
    fig_line.update_yaxes(title_text="SLL FOVRegions", row=row_num, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=row_num, col=1)

    # Update layout for bar plot
    fig_line.update_layout(height=500, width=1400, title_text="SLL v/s scanindex", showlegend=True)
    fig_line.update_traces(marker_color="red")
    fig_line.update_xaxes(zeroline=False, showgrid=False, type="category")
    fig_line.update_yaxes(zeroline=False, showgrid=False)
    #fig_line.show()

    rc_sll_line_plot_html = pio.to_html(fig_line, full_html=False, include_plotlyjs=False)
    #############################################################################################################

    #############################################################################################################
    # HTML Content
    #############################################################################################################
    session_vars.html_content += f"""
        <details>
            <b><u>Plots:</u></b>
            <details>
                <summary><i>Plot A</i></summary>
                {rc_blockage_line_plot_html}
            </details>
            <details>
                <summary><i>Plot B</i></summary>
                {rc_dm_line_plot_html}
            </details>
            <details>
                <summary><i>Plot C</i></summary>
                {rc_re_line_plot_html}
            </details>
            <details>
                <summary><i>Plot D</i></summary>
                {rc_sll_line_plot_html}
            </details>
        </details>
        <hr>
        </body>
        </html>
    """
    #############################################################################################################

def process_logs(data_files):
    veh_csv_list = []
    sim_csv_list = []
    
    num_of_logs = len(data_files['input'])
    for log_idx in range(num_of_logs):
        # Each log should have 2 csv files
        input_file = data_files['input'][log_idx]
        output_file = data_files['output'][log_idx]

        #################################
        # check if the csv files exists.
        # If any of the csv files have issues, then simply continue on to next log
        #################################
        if not check_file(input_file):
            continue
        if not check_file(output_file):
            continue
        #################################

        veh_csv_list.append(input_file)
        sim_csv_list.append(output_file)

    for sensor_pos in ['_FC_', '_FL_', '_FR_', '_RL_', '_RR_']:
        veh_csv_sp_list = [veh_csv for veh_csv in veh_csv_list if sensor_pos in veh_csv]
        sim_csv_sp_list = [sim_csv for sim_csv in sim_csv_list if sensor_pos in sim_csv]

        if veh_csv_sp_list and sim_csv_sp_list:
            log_name = os.path.basename(veh_csv_sp_list[0])
            log_name_wo_ext = log_name.replace(config.RC_FILE_SUFFIX, "")
            session_vars.html_content += f"""
                    <b>Log:</b> {log_name_wo_ext}({len(veh_csv_sp_list)} logs)
                    """

            #################################
            # Reset the below variables
            #################################
            rc_vars.reset()

            status = main(veh_csv_sp_list, sim_csv_sp_list)
            if status:
                plot_stats()
            else:
                session_vars.html_content += f"""
                    <b>NA</b>
                    <hr>
                    """

#####################################################################
# START
#####################################################################
# Check if all arguments are passed
num_of_argv = len(sys.argv)  
proper_command_string = "Proper usage e.g.: python interferenceDetection_matching_kpi_script.py log_path.txt meta_data.json C:\\Gitlab\\gen7v1_resim_kpi_scripts"
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
debug_file_name = config.RC_DEBUG_FILE_NAME + "_" + \
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
# HTML Content
#################################
session_vars.html_header = f"""
<html>
<head>
    <meta charset="utf-8">
    <title>{config.RC_FILE_TITLE}</title>
    <script src="https://cdn.plot.ly/plotly-2.27.0.min.js"></script>
</head>
<body>
    <h1>{config.RC_FILE_TITLE} {config.FILE_VERSION}</h1>
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
                A scan is said to match a re-simulated scan if the difference(error) in the corresponding Status is within the threshold mentioned below
                <ul>
                    <li>BLOCKAGE_STATUS : {round(config.RC_STATUS_THRESHOLD, 5)}</li>
                    <li>DM_STATUS : {round(config.RC_STATUS_THRESHOLD, 5)}</li>
                    <li>RE_STATUS : {round(config.RC_STATUS_THRESHOLD, 5)}</li>
                    <li>SLL_STATUS : {round(config.RC_STATUS_THRESHOLD, 5)}</li>
                </ul> 
           <li><b>Accuracy:</b> (Number of matching scans / total number of scans) * 100
			<li><b>Plot A:</b> Plot of Blockage across scan indices
			<li><b>Plot B:</b> Plot of DM across scan indices
			<li><b>Plot C:</b> Plot of RE across scan indices
			<li><b>Plot D:</b> Plot of SLL across scan indices
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
    data_files = find_rc_related_data_files(input_folder)
    if data_files['input'] and data_files['output']:
        process_logs(data_files)
    else:
        print("Error: No logs to process - either the input and/or the output csvs are missing.")

session_vars.html_content += "</table></body></html>"

# Write HTML content to file
output_html_file_name = f"{config.RC_HTML_FILE_NAME}_" \
                f"{session_vars.script_start_timestamp}_" \
                f"{config.RESIM_MODE}_" \
                f"MODE.html"

# Write HTML content to file
output_html = session_vars.output_folder + "/" + output_html_file_name
with open(output_html, "w", encoding='utf-8') as f:
    f.write(session_vars.html_content)
    f.close()
#####################################################################
# END
#####################################################################