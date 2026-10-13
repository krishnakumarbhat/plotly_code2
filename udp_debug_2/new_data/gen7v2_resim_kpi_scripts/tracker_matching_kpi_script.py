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
    global max_number_of_data, max_num_of_si_to_process, max_valid_distance
    global vcs_xposn_threshold, vcs_yposn_threshold, vcs_xvel_threshold, vcs_yvel_threshold
    global scan_index_list, accuracy_all_trks_list, accuracy_mov_trks_list
    global html_content
    #################################

    print("Reading the CSVs...")
    #################################
    # Read Vehicle and Resim data
    #################################
    if (max_num_of_si_to_process != 0):
        nrows = max_num_of_si_to_process
    else:
        nrows = None
    cols_of_interest = ['scan_index']
    cols_of_interest = cols_of_interest + [item for i in range(max_number_of_data) for item in (f"trkID_{i}", f"vcs_xposn_{i}", f"vcs_yposn_{i}", f"vcs_xvel_{i}", f"vcs_yvel_{i}", f"vcs_heading_{i}", f"len1_{i}", f"len2_{i}", f"wid1_{i}", f"wid2_{i}", f"f_moving_{i}")]
    veh_df = pd.read_csv(veh_csv, usecols = cols_of_interest, nrows=nrows, memory_map=True)
    sim_df = pd.read_csv(sim_csv, usecols = cols_of_interest, nrows=nrows, memory_map=True)
    num_of_SI_in_veh = veh_df.shape[0]
    num_of_SI_in_sim = sim_df.shape[0]
    #################################

    #################################
    # Tracker Stream matching
    #################################
    # Step 1: Merge the dataframes on 'scan_index'
    merged_df = pd.merge(veh_df, sim_df, on='scan_index', suffixes=('_veh', '_sim'))
    num_of_same_SI_in_veh_and_sim = merged_df.shape[0]

    veh_col_to_check_x_posn = [col for col in merged_df.columns if col.startswith(f"vcs_xposn_") and col.endswith("_veh")]
    veh_col_to_check_y_posn = [col for col in merged_df.columns if col.startswith(f"vcs_yposn_") and col.endswith("_veh")]
    sim_col_to_check_x_posn = [col for col in merged_df.columns if col.startswith(f"vcs_xposn_") and col.endswith("_sim")]
    sim_col_to_check_y_posn = [col for col in merged_df.columns if col.startswith(f"vcs_yposn_") and col.endswith("_sim")]

    # Count valid tracks in veh and sim
    veh_col_to_check_validity = [col for col in merged_df.columns if col.startswith(f"trkID_") and col.endswith("_veh")]
    sim_col_to_check_validity = [col for col in merged_df.columns if col.startswith(f"trkID_") and col.endswith("_sim")]
    veh_valid_tracks = (merged_df[veh_col_to_check_validity] > 0).values & \
                       (abs(merged_df[veh_col_to_check_x_posn]) <= max_valid_distance).values & \
                       (abs(merged_df[veh_col_to_check_y_posn]) <= max_valid_distance).values
    sim_valid_tracks = (merged_df[sim_col_to_check_validity] > 0).values & \
                       (abs(merged_df[sim_col_to_check_x_posn]) <= max_valid_distance).values & \
                       (abs(merged_df[sim_col_to_check_y_posn]) <= max_valid_distance).values
    veh_col_to_check_moving = [col for col in merged_df.columns if col.startswith(f"f_moving_") and col.endswith("_veh")]
    sim_col_to_check_moving = [col for col in merged_df.columns if col.startswith(f"f_moving_") and col.endswith("_sim")]
    veh_moving_tracks = (merged_df[veh_col_to_check_moving] == 1).values & \
                        (abs(merged_df[veh_col_to_check_x_posn]) <= max_valid_distance).values & \
                        (abs(merged_df[veh_col_to_check_y_posn]) <= max_valid_distance).values
    sim_moving_tracks = (merged_df[sim_col_to_check_moving] == 1).values & \
                        (abs(merged_df[sim_col_to_check_x_posn]) <= max_valid_distance).values & \
                        (abs(merged_df[sim_col_to_check_y_posn]) <= max_valid_distance).values

    merged_df["num_valid_trks_veh"] = veh_valid_tracks.sum(axis=1)
    merged_df["num_valid_trks_sim"] = sim_valid_tracks.sum(axis=1)
    merged_df["num_valid_mov_trks_veh"] = veh_moving_tracks.sum(axis=1)
    merged_df["num_valid_mov_trks_sim"] = sim_moving_tracks.sum(axis=1)

    # Step 2: Compute matches within thresholds
    def match_tracks(row):

        # Extract valid veh and sim indices
        veh_indices = [i for i in range(max_number_of_data) if (row[f"trkID_{i}_veh"] > 0) and (abs(row[f"vcs_xposn_{i}_veh"]) <= max_valid_distance) and (abs(row[f"vcs_yposn_{i}_veh"]) <= max_valid_distance)]
        sim_indices = [i for i in range(max_number_of_data) if (row[f"trkID_{i}_sim"] > 0) and (abs(row[f"vcs_xposn_{i}_sim"]) <= max_valid_distance) and (abs(row[f"vcs_yposn_{i}_sim"]) <= max_valid_distance)]

        # Initialize set for matched sim indices
        matched_sim_indices = set()
        all_matched_count = 0
        mov_matched_count = 0
        threshold_mul_factor = 1

        for i in veh_indices:
            veh_track_x_posn = row.get(f"vcs_xposn_{i}_veh")
            veh_track_y_posn = row.get(f"vcs_yposn_{i}_veh")
            veh_track_x_vel = row.get(f"vcs_xvel_{i}_veh")
            veh_track_y_vel = row.get(f"vcs_yvel_{i}_veh")
            veh_track_f_moving = row.get(f"f_moving_{i}_veh")

            veh_track_x_posn_abs = abs(veh_track_x_posn)
            veh_track_y_posn_abs = abs(veh_track_y_posn)
            if(veh_track_x_posn_abs >= 10 or veh_track_y_posn_abs >= 10):
                threshold_mul_factor = int(max(veh_track_x_posn_abs, veh_track_y_posn_abs)/10)

            for j in sim_indices:
                if j in matched_sim_indices:
                    continue
                sim_track_x_posn = row.get(f"vcs_xposn_{j}_sim")
                sim_track_y_posn = row.get(f"vcs_yposn_{j}_sim")
                sim_track_x_vel = row.get(f"vcs_xvel_{j}_sim")
                sim_track_y_vel = row.get(f"vcs_yvel_{j}_sim")
                sim_track_f_moving = row.get(f"f_moving_{j}_sim")

                track_x_posn_diff = veh_track_x_posn - sim_track_x_posn
                track_y_posn_diff = veh_track_y_posn - sim_track_y_posn
                track_x_vel_diff = veh_track_x_vel - sim_track_x_vel
                track_y_vel_diff = veh_track_y_vel - sim_track_y_vel

                # Check if all differences are within thresholds
                if (abs(track_x_posn_diff) <= (threshold_mul_factor*vcs_xposn_threshold) and
                    abs(track_y_posn_diff) <= (threshold_mul_factor*vcs_yposn_threshold) and
                    abs(track_x_vel_diff) <= (threshold_mul_factor*vcs_xvel_threshold) and
                    abs(track_y_vel_diff) <= (threshold_mul_factor*vcs_yvel_threshold)):
                    if veh_track_f_moving == 1 and sim_track_f_moving == 1:
                        mov_matched_count += 1
                    all_matched_count += 1
                    matched_sim_indices.add(j)
                    break  # Move to the next veh track

        return mov_matched_count, all_matched_count

    # Apply the matching function row-wise
    merged_df[['mov_trk_match_count', 'all_trk_match_count']] = merged_df.apply(match_tracks, axis=1, result_type="expand")


    merged_df["same_num_of_valid_trks"] = (merged_df["num_valid_trks_veh"] != 0) & (merged_df["num_valid_trks_veh"] == merged_df["num_valid_trks_sim"])
    merged_df["same_num_of_valid_mov_trks"] = (merged_df["num_valid_mov_trks_veh"] != 0) & (merged_df["num_valid_mov_trks_veh"] == merged_df["num_valid_mov_trks_sim"])
    merged_df['matching_pct_all_trks'] = merged_df['all_trk_match_count']/merged_df['num_valid_trks_veh']
    merged_df['matching_pct_mov_trks'] = merged_df['mov_trk_match_count']/merged_df['num_valid_mov_trks_veh']
    scan_index_list = merged_df['scan_index'].tolist()
    accuracy_all_trks_list = merged_df['matching_pct_all_trks'].tolist()
    accuracy_mov_trks_list = merged_df['matching_pct_mov_trks'].tolist()

    #report = merged_df[['scan_index', 'num_valid_trks_veh', 'num_valid_trks_sim', 'matched_tracks']]
    #print(report)

    num_of_trks_in_veh = sum(merged_df['num_valid_trks_veh'])
    num_of_trks_in_sim = sum(merged_df['num_valid_trks_sim'])
    num_of_mov_trks_in_veh = sum(merged_df['num_valid_mov_trks_veh'])
    num_of_mov_trks_in_sim = sum(merged_df['num_valid_mov_trks_sim'])
    num_of_SI_with_mov_trks_veh = len(merged_df[merged_df['num_valid_mov_trks_veh'] > 0])
    num_of_SI_with_mov_trks_sim = len(merged_df[merged_df['num_valid_mov_trks_sim'] > 0])
    num_of_SI_with_same_num_of_trks = len(merged_df[merged_df['same_num_of_valid_trks'] == 1])
    num_of_SI_with_same_num_of_mov_trks = len(merged_df[merged_df['same_num_of_valid_mov_trks'] == 1])
    num_of_SI_with_matching_trks = merged_df[(merged_df['same_num_of_valid_trks'] == 1) & (merged_df['matching_pct_all_trks'] == 1)].shape[0]
    num_of_SI_with_matching_mov_trks = merged_df[(merged_df['same_num_of_valid_mov_trks'] == 1) & (merged_df['matching_pct_mov_trks'] == 1)].shape[0]
    num_of_matching_trks = sum(merged_df['all_trk_match_count'])
    num_of_matching_mov_trks = sum(merged_df['mov_trk_match_count'])

    kpis_trks = {'result1':
                   {'numerator': num_of_SI_with_same_num_of_trks,
                    'denominator': num_of_same_SI_in_veh_and_sim,
                    'value': round((num_of_SI_with_same_num_of_trks / num_of_same_SI_in_veh_and_sim) * 100, 2) if (num_of_same_SI_in_veh_and_sim != 0) else None},
               'result2':
                   {'numerator': num_of_SI_with_matching_trks,
                    'denominator': num_of_SI_with_same_num_of_trks,
                    'value': round((num_of_SI_with_matching_trks / num_of_SI_with_same_num_of_trks) * 100, 2) if (num_of_SI_with_same_num_of_trks != 0) else None},
               'result3':
                   {'numerator': num_of_SI_with_matching_trks,
                    'denominator': num_of_same_SI_in_veh_and_sim,
                    'value': round((num_of_SI_with_matching_trks / num_of_same_SI_in_veh_and_sim) * 100, 2) if (num_of_same_SI_in_veh_and_sim != 0) else None},
               'result4':
                   {'numerator': num_of_matching_trks,
                    'denominator': num_of_trks_in_veh,
                    'value': round((num_of_matching_trks / num_of_trks_in_veh) * 100, 2) if (num_of_trks_in_veh != 0) else None},
               }

    kpis_mov_trks = {'result1':
                     {'numerator': num_of_SI_with_same_num_of_mov_trks,
                      'denominator': num_of_SI_with_mov_trks_veh,
                      'value': round((num_of_SI_with_same_num_of_mov_trks / num_of_SI_with_mov_trks_veh) * 100, 2) if (num_of_SI_with_mov_trks_veh != 0) else None},
                 'result2':
                     {'numerator': num_of_SI_with_matching_mov_trks,
                      'denominator': num_of_SI_with_same_num_of_mov_trks,
                      'value': round((num_of_SI_with_matching_mov_trks / num_of_SI_with_same_num_of_mov_trks) * 100, 2) if (num_of_SI_with_same_num_of_mov_trks != 0) else None},
                 'result3':
                     {'numerator': num_of_SI_with_matching_mov_trks,
                      'denominator': num_of_SI_with_mov_trks_veh,
                      'value': round((num_of_SI_with_matching_mov_trks / num_of_SI_with_mov_trks_veh) * 100, 2) if (num_of_SI_with_mov_trks_veh != 0) else None},
                 'result4':
                     {'numerator': num_of_matching_mov_trks,
                      'denominator': num_of_mov_trks_in_veh,
                      'value': round((num_of_matching_mov_trks / num_of_mov_trks_in_veh) * 100, 2) if (num_of_mov_trks_in_veh != 0) else None},
                 }

    print(f"Number of SI in (vehicle, simulation): ({num_of_SI_in_veh}, {num_of_SI_in_sim})")
    print(f"Number of same SI available in both vehicle and simulation: {num_of_same_SI_in_veh_and_sim}")

    print(f"% of SI with same number of tracks: "
          f"{kpis_trks['result1']['numerator']}/{kpis_trks['result1']['denominator']} --> {kpis_trks['result1']['value']}%" )
    print(f"% of SI with 100% matching tracks: "
          f"{kpis_trks['result2']['numerator']}/{kpis_trks['result2']['denominator']} --> {kpis_trks['result2']['value']}%, "
          f"{kpis_trks['result3']['numerator']}/{kpis_trks['result3']['denominator']} --> {kpis_trks['result3']['value']}%")
    print(f"Number of tracks in (vehicle, simulation): ({num_of_trks_in_veh}, {num_of_trks_in_sim})")
    print(f"Accuracy: "
          f"{kpis_trks['result4']['numerator']}/{kpis_trks['result4']['denominator']} --> {kpis_trks['result4']['value']}%")

    print(f"Number of SI with moving tracks in (vehicle, simulation): ({num_of_SI_with_mov_trks_veh}, {num_of_SI_with_mov_trks_sim})")
    print(f"% of SI with same number of moving tracks: "
          f"{kpis_mov_trks['result1']['numerator']}/{kpis_mov_trks['result1']['denominator']} --> {kpis_mov_trks['result1']['value']}%" )
    print(f"% of SI with 100% matching moving tracks: "
          f"{kpis_mov_trks['result2']['numerator']}/{kpis_mov_trks['result2']['denominator']} --> {kpis_mov_trks['result2']['value']}%, "
          f"{kpis_mov_trks['result3']['numerator']}/{kpis_mov_trks['result3']['denominator']} --> {kpis_mov_trks['result3']['value']}%")
    print(f"Number of moving tracks in (vehicle, simulation): ({num_of_mov_trks_in_veh}, {num_of_mov_trks_in_sim})")
    print(f"Accuracy: "
          f"{kpis_mov_trks['result4']['numerator']}/{kpis_mov_trks['result4']['denominator']} --> {kpis_mov_trks['result4']['value']}%")

    #################################
    # HTML Content
    #################################
    html_content += f"""
    <b>KPI:</b> Accuracy: ({kpis_trks['result4']['numerator']}/{kpis_trks['result4']['denominator']}) --> <b>{kpis_trks['result4']['value']}%</b>
    <details>
        <summary><i>Details</i></summary>
        <b>All tracks</b><br>
        Number of SI in (vehicle, simulation) : {num_of_SI_in_veh}, {num_of_SI_in_sim}<br>
        Number of same SI available in both vehicle and simulation: {num_of_same_SI_in_veh_and_sim}<br>
        % of SI with same number of tracks: 
        ({kpis_trks['result1']['numerator']}/{kpis_trks['result1']['denominator']}) --> {kpis_trks['result1']['value']}%
        <br>
        % of SI with 100% matching tracks:
        ({kpis_trks['result2']['numerator']}/{kpis_trks['result2']['denominator']}) --> {kpis_trks['result2']['value']}%,
        ({kpis_trks['result3']['numerator']}/{kpis_trks['result3']['denominator']}) --> {kpis_trks['result3']['value']}%,
        <br>
        Number of tracks in (vehicle, simulation): {num_of_trks_in_veh}, {num_of_trks_in_sim}<br>
        Accuracy:
        ({kpis_trks['result4']['numerator']}/{kpis_trks['result4']['denominator']}) --> {kpis_trks['result4']['value']}%,
        <br>
        <b>Moving tracks</b><br>
        % of SI with same number of moving tracks: 
        ({kpis_mov_trks['result1']['numerator']}/{kpis_mov_trks['result1']['denominator']}) --> {kpis_mov_trks['result1']['value']}%
        <br>
        % of SI with 100% matching moving tracks:
        ({kpis_mov_trks['result2']['numerator']}/{kpis_mov_trks['result2']['denominator']}) --> {kpis_mov_trks['result2']['value']}%,
        ({kpis_mov_trks['result3']['numerator']}/{kpis_mov_trks['result3']['denominator']}) --> {kpis_mov_trks['result3']['value']}%,
        <br>
        Number of moving tracks in (vehicle, simulation): {num_of_mov_trks_in_veh}, {num_of_mov_trks_in_sim}<br>
        Accuracy:
        ({kpis_mov_trks['result4']['numerator']}/{kpis_mov_trks['result4']['denominator']}) --> {kpis_mov_trks['result4']['value']}%,
        <br>
    """
    #################################


def func_line(x, y):
    # Return Scatter trace
    return go.Scatter(x=x, y=y, mode='lines')
				
def plot_stats():
    #################################
    # Global variables
    #################################
    global html_content
    global scan_index_list, accuracy_all_trks_list, accuracy_mov_trks_list
    #################################
    #################################

    #################################
    # Create subplots for accuracy v/s scanindex:
    #################################
    fig_line = sp.make_subplots(rows=2, cols=1, horizontal_spacing=0.04, vertical_spacing=0.2)

    # Manually add traces for line plots
    fig_line.add_trace(func_line(scan_index_list, accuracy_all_trks_list), row=1, col=1)
    fig_line.update_yaxes(title_text="All tracks accuracy", row=1, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=1, col=1)

    fig_line.add_trace(func_line(scan_index_list, accuracy_mov_trks_list), row=2, col=1)
    fig_line.update_yaxes(title_text="Moving tracks accuracy", row=2, col=1)
    fig_line.update_xaxes(title_text="Scan Index", row=2, col=1)

    # Update layout for plot
    fig_line.update_layout(height=800, width=1250, title_text="Accuracy v/s scanindex", showlegend=False)
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
    
file_suffix = '_UDP_GEN7_ROT_OBJECT_STREAM.csv'
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
    global scan_index_list, accuracy_all_trks_list, accuracy_mov_trks_list
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
        accuracy_all_trks_list = []
        accuracy_mov_trks_list = []
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
            output_html = output_folder + f"/tracker_kpi_report_{i+1:04d}.html"
            with open(output_html, "w") as f:
                f.write(html_content)
                f.close()
            html_content = "<table><body><html>"


#####################################################################
# START
#####################################################################
if len(sys.argv) != 4:
    print("Usage: python tracker_matching_kpi_script.py log_path.txt meta_data.txt C:\\Gitlab\\gen7v1_resim_kpi_scripts")
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
use_sample_data = 0
max_num_of_si_to_process = 0
#################################

#################################
# Constants
#################################
if use_sample_data == 1:
    max_number_of_data = 2
else:
    max_number_of_data = 64
max_valid_distance = 160
epsilon = 0.0000001
vcs_xposn_threshold = 0.01 + epsilon
vcs_yposn_threshold = 0.01 + epsilon
vcs_xvel_threshold = 0.02 + epsilon
vcs_yvel_threshold = 0.02 + epsilon
num_of_SI_in_veh = 0
num_of_SI_in_sim = 0
num_of_same_SI_in_veh_and_sim = 0
scan_index_list = []
accuracy_all_trks_list = []
accuracy_mov_trks_list = []
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
            <li><b>Definition of match:</b>
                A track is said to match a re-simulated track if the difference(error) in the position(x, y) and 
                velocity(x,y) are within the thresholds mentioned below
                <ul>
                    <li>X Position : {round(vcs_xposn_threshold, 5)} m</li>
                    <li>Y Position : {round(vcs_yposn_threshold, 5)} m</li>
                    <li>X Velocity : {round(vcs_xvel_threshold, 5)} m/s</li>
                    <li>Y Velocity : {round(vcs_yvel_threshold, 5)} m/s</li>
                </ul> 
                The above thresholds are valid for 0 to 10m, it gets doubled from 10 to 20m, tripled from 20 to 30m and so on.
                Tracks upto only {max_valid_distance}m are considered for matching
            <li><b>Accuracy:</b> (Number of matching tracks / total number of tracks) * 100
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