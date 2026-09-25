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
    global vcs_long_posn_0_veh_list, vcs_long_posn_0_sim_list
    global vcs_lat_posn_0_veh_list, vcs_lat_posn_0_sim_list
    global vcs_long_vel_0_veh_list, vcs_long_vel_0_sim_list
    global vcs_lat_vel_0_veh_list, vcs_lat_vel_0_sim_list
    global vcs_height_offset_m_0_veh_list, vcs_height_offset_m_0_sim_list
    global vcs_boresight_az_angle_0_veh_list, vcs_boresight_az_angle_0_sim_list
    global vcs_boresight_elev_angle_0_veh_list, vcs_boresight_elev_angle_0_sim_list
    global range_rate_interval_width_0_veh_list, range_rate_interval_width_0_sim_list
    global useful_fov_0_0_veh_list, useful_fov_0_0_sim_list
    global useful_fov_1_0_veh_list, useful_fov_1_0_sim_list
    global align_angle_az_rad_0_veh_list, align_angle_az_rad_0_sim_list
    global align_angle_el_rad_0_veh_list, align_angle_el_rad_0_sim_list
    global look_index_0_veh_list, look_index_0_sim_list
    global sensorID_0_veh_list, sensorID_0_sim_list
    global f_sens_valid_0_veh_list, f_sens_valid_0_sim_list
    global new_measurement_update_0_veh_list, new_measurement_update_0_sim_list
    global mount_location_0_veh_list, mount_location_0_sim_list
    global sensor_type_0_veh_list, sensor_type_0_sim_list
    global look_id_0_veh_list, look_id_0_sim_list
    global radar_polarity_0_veh_list, radar_polarity_0_sim_list
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
        "vcs_long_posn_0",
        "vcs_lat_posn_0",
        "vcs_long_vel_0",
        "vcs_lat_vel_0",
        "vcs_height_offset_m_0",
        "vcs_boresight_az_angle_0",
        "vcs_boresight_elev_angle_0",
        "range_rate_interval_width_0",
        "useful_fov_0_0",
        "useful_fov_1_0",
        "align_angle_az_rad_0",
        "align_angle_el_rad_0",
        "look_index_0",
        "sensorID_0",
        "f_sens_valid_0",
        "new_measurement_update_0",
        "mount_location_0",
        "sensor_type_0",
        "look_id_0",
        "radar_polarity_0",
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
    vcs_long_posn_0_veh_list = merged_df["vcs_long_posn_0_veh"].values.tolist()
    vcs_long_posn_0_sim_list = merged_df["vcs_long_posn_0_sim"].values.tolist()
    vcs_lat_posn_0_veh_list = merged_df["vcs_lat_posn_0_veh"].values.tolist()
    vcs_lat_posn_0_sim_list = merged_df["vcs_lat_posn_0_sim"].values.tolist()
    vcs_long_vel_0_veh_list = merged_df["vcs_long_vel_0_veh"].values.tolist()
    vcs_long_vel_0_sim_list = merged_df["vcs_long_vel_0_sim"].values.tolist()
    vcs_lat_vel_0_veh_list = merged_df["vcs_lat_vel_0_veh"].values.tolist()
    vcs_lat_vel_0_sim_list = merged_df["vcs_lat_vel_0_sim"].values.tolist()
    vcs_height_offset_m_0_veh_list = merged_df["vcs_height_offset_m_0_veh"].values.tolist()
    vcs_height_offset_m_0_sim_list = merged_df["vcs_height_offset_m_0_sim"].values.tolist()
    vcs_boresight_az_angle_0_veh_list = merged_df["vcs_boresight_az_angle_0_veh"].values.tolist()
    vcs_boresight_az_angle_0_sim_list = merged_df["vcs_boresight_az_angle_0_sim"].values.tolist()
    vcs_boresight_elev_angle_0_veh_list = merged_df["vcs_boresight_elev_angle_0_veh"].values.tolist()
    vcs_boresight_elev_angle_0_sim_list = merged_df["vcs_boresight_elev_angle_0_sim"].values.tolist()
    range_rate_interval_width_0_veh_list = merged_df["range_rate_interval_width_0_veh"].values.tolist()
    range_rate_interval_width_0_sim_list = merged_df["range_rate_interval_width_0_sim"].values.tolist()
    useful_fov_0_0_veh_list = merged_df["useful_fov_0_0_veh"].values.tolist()
    useful_fov_0_0_sim_list = merged_df["useful_fov_0_0_sim"].values.tolist()
    useful_fov_1_0_veh_list = merged_df["useful_fov_1_0_veh"].values.tolist()
    useful_fov_1_0_sim_list = merged_df["useful_fov_1_0_sim"].values.tolist()
    align_angle_az_rad_0_veh_list = merged_df["align_angle_az_rad_0_veh"].values.tolist()
    align_angle_az_rad_0_sim_list = merged_df["align_angle_az_rad_0_sim"].values.tolist()
    align_angle_el_rad_0_veh_list = merged_df["align_angle_el_rad_0_veh"].values.tolist()
    align_angle_el_rad_0_sim_list = merged_df["align_angle_el_rad_0_sim"].values.tolist()
    look_index_0_veh_list = merged_df["look_index_0_veh"].values.tolist()
    look_index_0_sim_list = merged_df["look_index_0_sim"].values.tolist()
    sensorID_0_veh_list = merged_df["sensorID_0_veh"].values.tolist()
    sensorID_0_sim_list = merged_df["sensorID_0_sim"].values.tolist()
    f_sens_valid_0_veh_list = merged_df["f_sens_valid_0_veh"].values.tolist()
    f_sens_valid_0_sim_list = merged_df["f_sens_valid_0_sim"].values.tolist()
    new_measurement_update_0_veh_list = merged_df["new_measurement_update_0_veh"].values.tolist()
    new_measurement_update_0_sim_list = merged_df["new_measurement_update_0_sim"].values.tolist()
    mount_location_0_veh_list = merged_df["mount_location_0_veh"].values.tolist()
    mount_location_0_sim_list = merged_df["mount_location_0_sim"].values.tolist()
    sensor_type_0_veh_list = merged_df["sensor_type_0_veh"].values.tolist()
    sensor_type_0_sim_list = merged_df["sensor_type_0_sim"].values.tolist()
    look_id_0_veh_list = merged_df["look_id_0_veh"].values.tolist()
    look_id_0_sim_list = merged_df["look_id_0_sim"].values.tolist()
    radar_polarity_0_veh_list = merged_df["radar_polarity_0_veh"].values.tolist()
    radar_polarity_0_sim_list = merged_df["radar_polarity_0_sim"].values.tolist()
    #################################

def plot_stats():
    #################################
    # Global variables
    #################################
    global html_content
    global scan_index_list
    global vcs_long_posn_0_veh_list, vcs_long_posn_0_sim_list
    global vcs_lat_posn_0_veh_list, vcs_lat_posn_0_sim_list
    global vcs_long_vel_0_veh_list, vcs_long_vel_0_sim_list
    global vcs_lat_vel_0_veh_list, vcs_lat_vel_0_sim_list
    global vcs_height_offset_m_0_veh_list, vcs_height_offset_m_0_sim_list
    global vcs_boresight_az_angle_0_veh_list, vcs_boresight_az_angle_0_sim_list
    global vcs_boresight_elev_angle_0_veh_list, vcs_boresight_elev_angle_0_sim_list
    global range_rate_interval_width_0_veh_list, range_rate_interval_width_0_sim_list
    global useful_fov_0_0_veh_list, useful_fov_0_0_sim_list
    global useful_fov_1_0_veh_list, useful_fov_1_0_sim_list
    global align_angle_az_rad_0_veh_list, align_angle_az_rad_0_sim_list
    global align_angle_el_rad_0_veh_list, align_angle_el_rad_0_sim_list
    global look_index_0_veh_list, look_index_0_sim_list
    global sensorID_0_veh_list, sensorID_0_sim_list
    global f_sens_valid_0_veh_list, f_sens_valid_0_sim_list
    global new_measurement_update_0_veh_list, new_measurement_update_0_sim_list
    global mount_location_0_veh_list, mount_location_0_sim_list
    global sensor_type_0_veh_list, sensor_type_0_sim_list
    global look_id_0_veh_list, look_id_0_sim_list
    global radar_polarity_0_veh_list, radar_polarity_0_sim_list
    #################################

    #################################
    # Create subplots for accuracy v/s scanindex:
    #################################
    fig_line = sp.make_subplots(rows=10, cols=2, horizontal_spacing=0.15, vertical_spacing=0.04)

    # Manually add traces for line plots
    row, col = 1, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_long_posn_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_long_posn_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="VCS Long Posn", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 1, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_lat_posn_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_lat_posn_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="VCS Lat Posn", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 2, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_long_vel_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_long_vel_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="VCS Long Vel", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 2, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_lat_vel_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_lat_vel_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="VCS Lat Vel", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 3, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_height_offset_m_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_height_offset_m_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="VCS Height Offset", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 3, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_boresight_az_angle_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_boresight_az_angle_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="VCS Boresight Az", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 4, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_boresight_elev_angle_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=vcs_boresight_elev_angle_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="VCS Boresight El", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 4, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=range_rate_interval_width_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=range_rate_interval_width_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Range Rate Interval Width", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 5, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=useful_fov_0_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=useful_fov_0_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Useful FOV 0", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 5, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=useful_fov_1_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=useful_fov_1_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Useful FOV 1", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 6, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=align_angle_az_rad_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=align_angle_az_rad_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Align Angle Az", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 6, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=align_angle_el_rad_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=align_angle_el_rad_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Align Angle El", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 7, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=look_index_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=look_index_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Look Index", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 7, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=sensorID_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=sensorID_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Sensor ID", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 8, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=f_sens_valid_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=f_sens_valid_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Flag Sensor Valid", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 8, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=new_measurement_update_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=new_measurement_update_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="New Measurement Update", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 9, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=mount_location_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=mount_location_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Mount Location", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 9, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=sensor_type_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=sensor_type_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Sensor Type", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 10, 1
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=look_id_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=look_id_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Look ID", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 10, 2
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=radar_polarity_0_veh_list, mode="lines", name="Veh", line=dict(color="blue")),
        row=row, col=col,)
    fig_line.add_trace(
        go.Scatter(x=scan_index_list, y=radar_polarity_0_sim_list, mode="lines", name="Sim", line=dict(color="red")),
        row=row, col=col,
    )
    fig_line.update_yaxes(title_text="Radar Polarity", row=row, col=col)
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
    

file_suffix = '_UDP_GEN7_ROT_PROCESSED_DETECTION_STREAM.csv'
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
    global vcs_long_posn_0_veh_list, vcs_long_posn_0_sim_list
    global vcs_lat_posn_0_veh_list, vcs_lat_posn_0_sim_list
    global vcs_long_vel_0_veh_list, vcs_long_vel_0_sim_list
    global vcs_lat_vel_0_veh_list, vcs_lat_vel_0_sim_list
    global vcs_height_offset_m_0_veh_list, vcs_height_offset_m_0_sim_list
    global vcs_boresight_az_angle_0_veh_list, vcs_boresight_az_angle_0_sim_list
    global vcs_boresight_elev_angle_0_veh_list, vcs_boresight_elev_angle_0_sim_list
    global range_rate_interval_width_0_veh_list, range_rate_interval_width_0_sim_list
    global useful_fov_0_0_veh_list, useful_fov_0_0_sim_list
    global useful_fov_1_0_veh_list, useful_fov_1_0_sim_list
    global align_angle_az_rad_0_veh_list, align_angle_az_rad_0_sim_list
    global align_angle_el_rad_0_veh_list, align_angle_el_rad_0_sim_list
    global look_index_0_veh_list, look_index_0_sim_list
    global sensorID_0_veh_list, sensorID_0_sim_list
    global f_sens_valid_0_veh_list, f_sens_valid_0_sim_list
    global new_measurement_update_0_veh_list, new_measurement_update_0_sim_list
    global mount_location_0_veh_list, mount_location_0_sim_list
    global sensor_type_0_veh_list, sensor_type_0_sim_list
    global look_id_0_veh_list, look_id_0_sim_list
    global radar_polarity_0_veh_list, radar_polarity_0_sim_list
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
        vcs_long_posn_0_veh_list = []
        vcs_long_posn_0_sim_list = []
        vcs_lat_posn_0_veh_list = []
        vcs_lat_posn_0_sim_list = []
        vcs_long_vel_0_veh_list = []
        vcs_long_vel_0_sim_list = []
        vcs_lat_vel_0_veh_list = []
        vcs_lat_vel_0_sim_list = []
        vcs_height_offset_m_0_veh_list = []
        vcs_height_offset_m_0_sim_list = []
        vcs_boresight_az_angle_0_veh_list = []
        vcs_boresight_az_angle_0_sim_list = []
        vcs_boresight_elev_angle_0_veh_list = []
        vcs_boresight_elev_angle_0_sim_list = []
        range_rate_interval_width_0_veh_list = []
        range_rate_interval_width_0_sim_list = []
        useful_fov_0_0_veh_list = []
        useful_fov_0_0_sim_list = []
        useful_fov_1_0_veh_list = []
        useful_fov_1_0_sim_list = []
        align_angle_az_rad_0_veh_list = []
        align_angle_az_rad_0_sim_list = []
        align_angle_el_rad_0_veh_list = []
        align_angle_el_rad_0_sim_list = []
        look_index_0_veh_list = []
        look_index_0_sim_list = []
        sensorID_0_veh_list = []
        sensorID_0_sim_list = []
        f_sens_valid_0_veh_list = []
        f_sens_valid_0_sim_list = []
        new_measurement_update_0_veh_list = []
        new_measurement_update_0_sim_list = []
        mount_location_0_veh_list = []
        mount_location_0_sim_list = []
        sensor_type_0_veh_list = []
        sensor_type_0_sim_list = []
        look_id_0_veh_list = []
        look_id_0_sim_list = []
        radar_polarity_0_veh_list = []
        radar_polarity_0_sim_list = []

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
            output_html = output_folder + f"/tracker_processed_det_kpi_report_{i+1:04d}.html"
            with open(output_html, "w") as f:
                f.write(html_content)
                f.close()
            html_content = "<table><body><html>"


#####################################################################
# START
#####################################################################
if len(sys.argv) != 4:
    print("Usage: python tracker_processed_det_kpi_script.py log_path.txt meta_data.txt C:\\Gitlab\\gen7v1_resim_kpi_scripts")
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
vcs_long_posn_0_veh_list = []
vcs_long_posn_0_sim_list = []
vcs_lat_posn_0_veh_list = []
vcs_lat_posn_0_sim_list = []
vcs_long_vel_0_veh_list = []
vcs_long_vel_0_sim_list = []
vcs_lat_vel_0_veh_list = []
vcs_lat_vel_0_sim_list = []
vcs_height_offset_m_0_veh_list = []
vcs_height_offset_m_0_sim_list = []
vcs_boresight_az_angle_0_veh_list = []
vcs_boresight_az_angle_0_sim_list = []
vcs_boresight_elev_angle_0_veh_list = []
vcs_boresight_elev_angle_0_sim_list = []
range_rate_interval_width_0_veh_list = []
range_rate_interval_width_0_sim_list = []
useful_fov_0_0_veh_list = []
useful_fov_0_0_sim_list = []
useful_fov_1_0_veh_list = []
useful_fov_1_0_sim_list = []
align_angle_az_rad_0_veh_list = []
align_angle_az_rad_0_sim_list = []
align_angle_el_rad_0_veh_list = []
align_angle_el_rad_0_sim_list = []
look_index_0_veh_list = []
look_index_0_sim_list = []
sensorID_0_veh_list = []
sensorID_0_sim_list = []
f_sens_valid_0_veh_list = []
f_sens_valid_0_sim_list = []
new_measurement_update_0_veh_list = []
new_measurement_update_0_sim_list = []
mount_location_0_veh_list = []
mount_location_0_sim_list = []
sensor_type_0_veh_list = []
sensor_type_0_sim_list = []
look_id_0_veh_list = []
look_id_0_sim_list = []
radar_polarity_0_veh_list = []
radar_polarity_0_sim_list = []
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