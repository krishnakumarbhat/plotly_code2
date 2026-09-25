import sys
import os
import pandas as pd
import plotly.graph_objs as go
import plotly.subplots as sp
import plotly.io as pio

def main(veh_csv, sim_csv):
    #################################
    # Global variables
    #################################
    global num_of_SI_in_veh, num_of_SI_in_sim, num_of_same_SI_in_veh_and_sim
    global max_num_of_si_to_process
    global scan_index_list
    global align_angle_az_rad_0_veh_list, align_angle_az_rad_0_sim_list
    global align_angle_el_rad_0_veh_list, align_angle_el_rad_0_sim_list
    global speed_correction_factor_veh_list, speed_correction_factor_sim_list
    global world_x_veh_list, world_x_sim_list
    global world_y_veh_list, world_y_sim_list
    global heading_veh_list, heading_sim_list
    global delta_pointing_veh_list, delta_pointing_sim_list
    global delta_position_x_veh_list, delta_position_x_sim_list
    global delta_position_y_veh_list, delta_position_y_sim_list
    global speed_veh_list, speed_sim_list
    global acceleration_veh_list, acceleration_sim_list
    global curvature_rear_veh_list, curvature_rear_sim_list
    global vcs_speed_veh_list, vcs_speed_sim_list
    global vcs_lat_acceleration_veh_list, vcs_lat_acceleration_sim_list
    global vcs_long_acceleration_veh_list, vcs_long_acceleration_sim_list
    global vcs_sideslip_veh_list, vcs_sideslip_sim_list
    global rear_cornering_compliance_veh_list, rear_cornering_compliance_sim_list
    global raw_speed_veh_list, raw_speed_sim_list
    global steering_angle_rad_veh_list, steering_angle_rad_sim_list
    global raw_yaw_rate_rad_veh_list, raw_yaw_rate_rad_sim_list
    global dist_rear_axle_to_vcs_m_veh_list, dist_rear_axle_to_vcs_m_sim_list
    global prndl_veh_list, prndl_sim_list
    global f_reverse_gear_veh_list, f_reverse_gear_sim_list
    global f_trailer_present_veh_list, f_trailer_present_sim_list
    #################################

    print("Reading the CSVs...")
    #################################
    # Read Vehicle and Resim data
    #################################
    if (max_num_of_si_to_process != 0):
        nrows = max_num_of_si_to_process
    else:
        nrows = None
    cols_of_interest = [
        "scan_index",
        "align_angle_az_rad_0",
        "align_angle_el_rad_0",
        "speed_correction_factor",
        "world_x",
        "world_y",
        "heading",
        "delta_pointing",
        "delta_position_x",
        "delta_position_y",
        "speed",
        "acceleration",
        "curvature_rear",
        "vcs_speed",
        "vcs_lat_acceleration",
        "vcs_long_acceleration",
        "vcs_sideslip",
        "rear_cornering_compliance",
        "raw_speed",
        "steering_angle_rad",
        "raw_yaw_rate_rad",
        "dist_rear_axle_to_vcs_m",
        "prndl",
        "f_reverse_gear",
        "f_trailer_present",
    ]
    veh_df = pd.read_csv(veh_csv, usecols = cols_of_interest, nrows=nrows, memory_map=True)
    sim_df = pd.read_csv(sim_csv, usecols = cols_of_interest, nrows=nrows, memory_map=True)
    num_of_SI_in_veh = veh_df.shape[0]
    num_of_SI_in_sim = sim_df.shape[0]
    #################################

    #################################
    #
    #################################
    # Step 1: Merge the dataframes on 'scan_index'
    merged_df = pd.merge(veh_df, sim_df, on='scan_index', suffixes=('_veh', '_sim'))
    num_of_same_SI_in_veh_and_sim = merged_df.shape[0]

    scan_index_list = merged_df['scan_index'].tolist()
    align_angle_az_rad_0_veh_list = merged_df["align_angle_az_rad_0_veh"].values.tolist()
    align_angle_az_rad_0_sim_list = merged_df["align_angle_az_rad_0_sim"].values.tolist()
    align_angle_el_rad_0_veh_list = merged_df["align_angle_el_rad_0_veh"].values.tolist()
    align_angle_el_rad_0_sim_list = merged_df["align_angle_el_rad_0_sim"].values.tolist()
    speed_correction_factor_veh_list = merged_df["speed_correction_factor_veh"].values.tolist()
    speed_correction_factor_sim_list = merged_df["speed_correction_factor_sim"].values.tolist()
    world_x_veh_list = merged_df["world_x_veh"].values.tolist()
    world_x_sim_list = merged_df["world_x_sim"].values.tolist()
    world_y_veh_list = merged_df["world_y_veh"].values.tolist()
    world_y_sim_list = merged_df["world_y_sim"].values.tolist()
    heading_veh_list = merged_df["heading_veh"].values.tolist()
    heading_sim_list = merged_df["heading_sim"].values.tolist()
    delta_pointing_veh_list = merged_df["delta_pointing_veh"].values.tolist()
    delta_pointing_sim_list = merged_df["delta_pointing_sim"].values.tolist()
    delta_position_x_veh_list = merged_df["delta_position_x_veh"].values.tolist()
    delta_position_x_sim_list = merged_df["delta_position_x_sim"].values.tolist()
    delta_position_y_veh_list = merged_df["delta_position_y_veh"].values.tolist()
    delta_position_y_sim_list = merged_df["delta_position_y_sim"].values.tolist()
    speed_veh_list = merged_df["speed_veh"].values.tolist()
    speed_sim_list = merged_df["speed_sim"].values.tolist()
    acceleration_veh_list = merged_df["acceleration_veh"].values.tolist()
    acceleration_sim_list = merged_df["acceleration_sim"].values.tolist()
    curvature_rear_veh_list = merged_df["curvature_rear_veh"].values.tolist()
    curvature_rear_sim_list = merged_df["curvature_rear_sim"].values.tolist()
    vcs_speed_veh_list = merged_df["vcs_speed_veh"].values.tolist()
    vcs_speed_sim_list = merged_df["vcs_speed_sim"].values.tolist()
    vcs_lat_acceleration_veh_list = merged_df["vcs_lat_acceleration_veh"].values.tolist()
    vcs_lat_acceleration_sim_list = merged_df["vcs_lat_acceleration_sim"].values.tolist()
    vcs_long_acceleration_veh_list = merged_df["vcs_long_acceleration_veh"].values.tolist()
    vcs_long_acceleration_sim_list = merged_df["vcs_long_acceleration_sim"].values.tolist()
    vcs_sideslip_veh_list = merged_df["vcs_sideslip_veh"].values.tolist()
    vcs_sideslip_sim_list = merged_df["vcs_sideslip_sim"].values.tolist()
    rear_cornering_compliance_veh_list = merged_df["rear_cornering_compliance_veh"].values.tolist()
    rear_cornering_compliance_sim_list = merged_df["rear_cornering_compliance_sim"].values.tolist()
    raw_speed_veh_list = merged_df["raw_speed_veh"].values.tolist()
    raw_speed_sim_list = merged_df["raw_speed_sim"].values.tolist()
    steering_angle_rad_veh_list = merged_df["steering_angle_rad_veh"].values.tolist()
    steering_angle_rad_sim_list = merged_df["steering_angle_rad_sim"].values.tolist()
    raw_yaw_rate_rad_veh_list = merged_df["raw_yaw_rate_rad_veh"].values.tolist()
    raw_yaw_rate_rad_sim_list = merged_df["raw_yaw_rate_rad_sim"].values.tolist()
    dist_rear_axle_to_vcs_m_veh_list = merged_df["dist_rear_axle_to_vcs_m_veh"].values.tolist()
    dist_rear_axle_to_vcs_m_sim_list = merged_df["dist_rear_axle_to_vcs_m_sim"].values.tolist()
    prndl_veh_list = merged_df["prndl_veh"].values.tolist()
    prndl_sim_list = merged_df["prndl_sim"].values.tolist()
    f_reverse_gear_veh_list = merged_df["f_reverse_gear_veh"].values.tolist()
    f_reverse_gear_sim_list = merged_df["f_reverse_gear_sim"].values.tolist()
    f_trailer_present_veh_list = merged_df["f_trailer_present_veh"].values.tolist()
    f_trailer_present_sim_list = merged_df["f_trailer_present_sim"].values.tolist()
    #################################

def plot_stats():
    #################################
    # Global variables
    #################################
    global html_content
    global scan_index_list
    global align_angle_az_rad_0_veh_list, align_angle_az_rad_0_sim_list
    global align_angle_el_rad_0_veh_list, align_angle_el_rad_0_sim_list
    global speed_correction_factor_veh_list, speed_correction_factor_sim_list
    global world_x_veh_list, world_x_sim_list
    global world_y_veh_list, world_y_sim_list
    global heading_veh_list, heading_sim_list
    global delta_pointing_veh_list, delta_pointing_sim_list
    global delta_position_x_veh_list, delta_position_x_sim_list
    global delta_position_y_veh_list, delta_position_y_sim_list
    global speed_veh_list, speed_sim_list
    global acceleration_veh_list, acceleration_sim_list
    global curvature_rear_veh_list, curvature_rear_sim_list
    global vcs_speed_veh_list, vcs_speed_sim_list
    global vcs_lat_acceleration_veh_list, vcs_lat_acceleration_sim_list
    global vcs_long_acceleration_veh_list, vcs_long_acceleration_sim_list
    global vcs_sideslip_veh_list, vcs_sideslip_sim_list
    global rear_cornering_compliance_veh_list, rear_cornering_compliance_sim_list
    global raw_speed_veh_list, raw_speed_sim_list
    global steering_angle_rad_veh_list, steering_angle_rad_sim_list
    global raw_yaw_rate_rad_veh_list, raw_yaw_rate_rad_sim_list
    global dist_rear_axle_to_vcs_m_veh_list, dist_rear_axle_to_vcs_m_sim_list
    global prndl_veh_list, prndl_sim_list
    global f_reverse_gear_veh_list, f_reverse_gear_sim_list
    global f_trailer_present_veh_list, f_trailer_present_sim_list
    #################################

    #################################
    # Create subplots for accuracy v/s scanindex:
    #################################
    fig_line = sp.make_subplots(rows=12, cols=2, horizontal_spacing=0.15, vertical_spacing=0.04)

    # Manually add traces for line plots
    row, col = 1, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=align_angle_az_rad_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=align_angle_az_rad_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Az mis-align", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 1, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=align_angle_el_rad_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=align_angle_el_rad_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="El mis-align", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 2, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=speed_correction_factor_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=speed_correction_factor_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Speed Correction Factor", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 2, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=world_x_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=world_x_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="World X", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 3, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=world_y_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=world_y_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="World Y", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 3, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=heading_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=heading_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Heading", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 4, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=delta_pointing_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=delta_pointing_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Delta Pointing", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 4, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=delta_position_x_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=delta_position_x_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Delta Position X", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 5, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=delta_position_y_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=delta_position_y_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Delta Position Y", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 5, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=speed_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=speed_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Speed", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 6, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=acceleration_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=acceleration_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Acc", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 6, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=curvature_rear_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=curvature_rear_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Curvature Rear", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 7, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_speed_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_speed_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="VCS Speed", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 7, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_lat_acceleration_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_lat_acceleration_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Lat Acc", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 8, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_long_acceleration_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_long_acceleration_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Long Acc", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 8, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_sideslip_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_sideslip_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="VCS Sideslip", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 9, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=rear_cornering_compliance_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=rear_cornering_compliance_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Rear Cornering Compliance", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 9, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=raw_speed_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=raw_speed_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Raw Speed", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 10, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=steering_angle_rad_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=steering_angle_rad_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Steering Angle", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 10, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=raw_yaw_rate_rad_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=raw_yaw_rate_rad_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Raw Yaw Rate", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 11, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=dist_rear_axle_to_vcs_m_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=dist_rear_axle_to_vcs_m_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Dist Rear Axle To VCS", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 11, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=prndl_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=prndl_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="PRNDL", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 12, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=f_reverse_gear_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=f_reverse_gear_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Flag Reverse Gear", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 12, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=f_trailer_present_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=f_trailer_present_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Flag Trailer Present", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    # Update layout for plot
    fig_line.update_layout(height=3000, width=1250, title_text="Parameters v/s scanindex", showlegend=False)
    fig_line.update_traces(marker_color='red')
    fig_line.update_xaxes(zeroline=False, showgrid=False, type='category')
    fig_line.update_yaxes(zeroline=False, showgrid=False)
    fig_line.show()

    line_plot_html = pio.to_html(fig_line, full_html=False, include_plotlyjs='cdn')
    #################################

    #################################
    # HTML Content
    #################################
    html_content += f"""
        <b><u>Plots:</u></b>
        <details>
            <summary><i>Plot A</i></summary>
            {line_plot_html}
        </details>
    </details>
    <hr>
    </body>
    </html>
    """
    

file_suffix = '_UDP_GEN7_ROT_VEHICLE_INFO.csv'
def find_data_files(base_path):
    data_files = {
        'input': [],
        'output': []
    }
    files = os.listdir(base_path)
    files_of_interest = [s for s in files if file_suffix in s]
    unique_files = set(s.replace(file_suffix, "").strip() for s in files_of_interest)
    for filename in sorted(unique_files):
        trk_file_path = os.path.join(base_path, filename + file_suffix)
        if "_r0" in filename:
            data_files['output'].append(trk_file_path)
        else:
            data_files['input'].append(trk_file_path)
    return data_files

def process_logs(data_files):
    #################################
    # Global variables
    #################################
    global num_of_SI_in_veh, num_of_SI_in_sim, num_of_same_SI_in_veh_and_sim
    global scan_index_list
    global align_angle_az_rad_0_veh_list, align_angle_az_rad_0_sim_list
    global align_angle_el_rad_0_veh_list, align_angle_el_rad_0_sim_list
    global speed_correction_factor_veh_list, speed_correction_factor_sim_list
    global world_x_veh_list, world_x_sim_list
    global world_y_veh_list, world_y_sim_list
    global heading_veh_list, heading_sim_list
    global delta_pointing_veh_list, delta_pointing_sim_list
    global delta_position_x_veh_list, delta_position_x_sim_list
    global delta_position_y_veh_list, delta_position_y_sim_list
    global speed_veh_list, speed_sim_list
    global acceleration_veh_list, acceleration_sim_list
    global curvature_rear_veh_list, curvature_rear_sim_list
    global vcs_speed_veh_list, vcs_speed_sim_list
    global vcs_lat_acceleration_veh_list, vcs_lat_acceleration_sim_list
    global vcs_long_acceleration_veh_list, vcs_long_acceleration_sim_list
    global vcs_sideslip_veh_list, vcs_sideslip_sim_list
    global rear_cornering_compliance_veh_list, rear_cornering_compliance_sim_list
    global raw_speed_veh_list, raw_speed_sim_list
    global steering_angle_rad_veh_list, steering_angle_rad_sim_list
    global raw_yaw_rate_rad_veh_list, raw_yaw_rate_rad_sim_list
    global dist_rear_axle_to_vcs_m_veh_list, dist_rear_axle_to_vcs_m_sim_list
    global prndl_veh_list, prndl_sim_list
    global f_reverse_gear_veh_list, f_reverse_gear_sim_list
    global f_trailer_present_veh_list, f_trailer_present_sim_list
    global html_content
    global output_folder
    #################################
    num_of_logs = len(data_files['input'])
    for i in range(num_of_logs):
        print(f"Processing log: {i+1}/{num_of_logs}")
        #################################
        # Reset the below variables
        #################################
        num_of_SI_in_veh = 0
        num_of_SI_in_sim = 0
        num_of_same_SI_in_veh_and_sim = 0
        scan_index_list = []
        align_angle_az_rad_0_veh_list = []
        align_angle_az_rad_0_sim_list = []
        align_angle_el_rad_0_veh_list = []
        align_angle_el_rad_0_sim_list = []
        speed_correction_factor_veh_list = []
        speed_correction_factor_sim_list = []
        world_x_veh_list = []
        world_x_sim_list = []
        world_y_veh_list = []
        world_y_sim_list = []
        heading_veh_list = []
        heading_sim_list = []
        delta_pointing_veh_list = []
        delta_pointing_sim_list = []
        delta_position_x_veh_list = []
        delta_position_x_sim_list = []
        delta_position_y_veh_list = []
        delta_position_y_sim_list = []
        speed_veh_list = []
        speed_sim_list = []
        acceleration_veh_list = []
        acceleration_sim_list = []
        curvature_rear_veh_list = []
        curvature_rear_sim_list = []
        vcs_speed_veh_list = []
        vcs_speed_sim_list = []
        vcs_lat_acceleration_veh_list = []
        vcs_lat_acceleration_sim_list = []
        vcs_long_acceleration_veh_list = []
        vcs_long_acceleration_sim_list = []
        vcs_sideslip_veh_list = []
        vcs_sideslip_sim_list = []
        rear_cornering_compliance_veh_list = []
        rear_cornering_compliance_sim_list = []
        raw_speed_veh_list = []
        raw_speed_sim_list = []
        steering_angle_rad_veh_list = []
        steering_angle_rad_sim_list = []
        raw_yaw_rate_rad_veh_list = []
        raw_yaw_rate_rad_sim_list = []
        dist_rear_axle_to_vcs_m_veh_list = []
        dist_rear_axle_to_vcs_m_sim_list = []
        prndl_veh_list = []
        prndl_sim_list = []
        f_reverse_gear_veh_list = []
        f_reverse_gear_sim_list = []
        f_trailer_present_veh_list = []
        f_trailer_present_sim_list = []

        if data_files['input']:
            file_name = os.path.basename(data_files['input'][i])
            log_name_wo_ext = file_name.replace(file_suffix, "")
        html_content += f"""
                <b>Log:</b> {log_name_wo_ext}
                """

        input_file = data_files['input'][i]
        output_file = data_files['output'][i]
        main(input_file, output_file)

        plot_stats()

        max_logs_in_one_report = 20
        if(((i+1) % max_logs_in_one_report == 0) or (i == (num_of_logs-1))):
            html_content += "</table></body></html>"

            # Write HTML content to file
            output_html = output_folder + f"/tracker_vehicle_info_kpi_report_{i+1:04d}.html"
            with open(output_html, "w") as f:
                f.write(html_content)
                f.close()
            html_content = "<table><body><html>"


#####################################################################
# START
#####################################################################
if len(sys.argv) != 4:
    print("Usage: python tracker_vehicle_info_kpi_script.py log_path.txt meta_data.txt C:\\Gitlab\\gen7v1_resim_kpi_scripts")
    sys.exit(1)

log_path_file = sys.argv[1]
meta_data_file = sys.argv[2]
output_folder = sys.argv[3]

if not os.path.exists(log_path_file):
    print(f"Error: File {log_path_file} does not exist.")
    sys.exit(1)
if not os.path.exists(meta_data_file):
    print(f"Error: File {meta_data_file} does not exist.")
    sys.exit(1)
#################################
# User modifiable variables
#################################
max_num_of_si_to_process = 0
#################################

#################################
# Constants
#################################
num_of_SI_in_veh = 0
num_of_SI_in_sim = 0
num_of_same_SI_in_veh_and_sim = 0
scan_index_list = []
align_angle_az_rad_0_veh_list = []
align_angle_az_rad_0_sim_list = []
align_angle_el_rad_0_veh_list = []
align_angle_el_rad_0_sim_list = []
speed_correction_factor_veh_list = []
speed_correction_factor_sim_list = []
world_x_veh_list = []
world_x_sim_list = []
world_y_veh_list = []
world_y_sim_list = []
heading_veh_list = []
heading_sim_list = []
delta_pointing_veh_list = []
delta_pointing_sim_list = []
delta_position_x_veh_list = []
delta_position_x_sim_list = []
delta_position_y_veh_list = []
delta_position_y_sim_list = []
speed_veh_list = []
speed_sim_list = []
acceleration_veh_list = []
acceleration_sim_list = []
curvature_rear_veh_list = []
curvature_rear_sim_list = []
vcs_speed_veh_list = []
vcs_speed_sim_list = []
vcs_lat_acceleration_veh_list = []
vcs_lat_acceleration_sim_list = []
vcs_long_acceleration_veh_list = []
vcs_long_acceleration_sim_list = []
vcs_sideslip_veh_list = []
vcs_sideslip_sim_list = []
rear_cornering_compliance_veh_list = []
rear_cornering_compliance_sim_list = []
raw_speed_veh_list = []
raw_speed_sim_list = []
steering_angle_rad_veh_list = []
steering_angle_rad_sim_list = []
raw_yaw_rate_rad_veh_list = []
raw_yaw_rate_rad_sim_list = []
dist_rear_axle_to_vcs_m_veh_list = []
dist_rear_axle_to_vcs_m_sim_list = []
prndl_veh_list = []
prndl_sim_list = []
f_reverse_gear_veh_list = []
f_reverse_gear_sim_list = []
f_trailer_present_veh_list = []
f_trailer_present_sim_list = []
#################################

#################################
# Read meta data
#################################
# Initialize an empty dictionary
metadata_dict = {}

# Open and read the file
with open(meta_data_file, "r") as file:
    for line in file:
        # Split each line by the first whitespace to get key and value
        key, value = line.strip().split(maxsplit=1)
        # Add to dictionary
        metadata_dict[key] = value
#print(metadata_dict)
#################################

#################################
# HTML Content
#################################
html_content = f"""
<html>
<head>
    <title>Tracker KPIs and Plots</title>
    <style>
        body {{ font-family: Arial, sans-serif; margin: 20px; }}
        table {{ width: 75%; border-collapse: collapse; margin: 20px 0; }}
        th, td {{ border: 1px solid #dddddd; text-align: left; padding: 8px; }}
        th {{ background-color: #f2f2f2; }}
    </style>
</head>
<body>
    <h1>Tracker KPIs and Plots</h1>
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
			<li><b>Plot A:</b> Plot of accuracy across scan indices
        </ol>
        <b>Note:</b> The plots are interactive
        <br>
        <b>Note:</b> Blank plots indicate no data or zero error
    </details>    

    <hr width="100%" size="2" color="blue" noshade>
"""
#################################

with open(log_path_file, 'r') as f:
    directory = f.readline().strip()
    data_files = find_data_files(directory)
    #print(data_files)
    if data_files['input'] and data_files['output']:
        process_logs(data_files)
#####################################################################
# END
#####################################################################