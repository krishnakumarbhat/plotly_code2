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
    global elapsed_time_s_real_list, elapsed_time_s_sim_list
    global reduced_num_active_objs_real_list, reduced_num_active_objs_sim_list
    global num_active_clusters_real_list, num_active_clusters_sim_list
    global number_of_historic_detections_real_list, number_of_historic_detections_sim_list
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
    cols_of_interest = [
        'scan_index',
        "elapsed_time_s",
        "reduced_num_active_objs",
        "num_active_clusters",
        "number_of_historic_detections",
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
    elapsed_time_s_real_list = merged_df["elapsed_time_s_veh"].values.tolist()
    elapsed_time_s_sim_list = merged_df["elapsed_time_s_sim"].values.tolist()
    reduced_num_active_objs_real_list = merged_df["reduced_num_active_objs_veh"].values.tolist()
    reduced_num_active_objs_sim_list = merged_df["reduced_num_active_objs_sim"].values.tolist()
    num_active_clusters_real_list = merged_df["num_active_clusters_veh"].values.tolist()
    num_active_clusters_sim_list = merged_df["num_active_clusters_sim"].values.tolist()
    number_of_historic_detections_real_list = merged_df["number_of_historic_detections_veh"].values.tolist()
    number_of_historic_detections_sim_list = merged_df["number_of_historic_detections_sim"].values.tolist()
    #################################


def func_line(x, y):
    # Return Scatter trace
    return go.Scatter(x=x, y=y, mode='lines')
				
def plot_stats():
    #################################
    # Global variables
    #################################
    global html_content
    global scan_index_list
    global elapsed_time_s_real_list, elapsed_time_s_sim_list
    global reduced_num_active_objs_real_list, reduced_num_active_objs_sim_list
    global num_active_clusters_real_list, num_active_clusters_sim_list
    global number_of_historic_detections_real_list, number_of_historic_detections_sim_list
    #################################

    #################################
    # Create subplots for accuracy v/s scanindex:
    #################################
    fig_line = sp.make_subplots(rows=4, cols=1, horizontal_spacing=0.0, vertical_spacing=0.12)

    # Manually add traces for line plots
    row, col = 1, 1
    fig_line.add_trace(
        go.Scatter(
            x=scan_index_list,
            y=elapsed_time_s_real_list,
            mode="lines",
            name="Veh Elapsed Time",
            line=dict(color="blue"),
        ),
        row=row, col=col,
    )
    fig_line.add_trace(
        go.Scatter(
            x=scan_index_list,
            y=elapsed_time_s_sim_list,
            mode="lines",
            name="Sim Elapsed Time",
            line=dict(color="red"),
        ),
        row=row,
        col=col,
    )
    fig_line.update_yaxes(title_text="Elapsed Time", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 2, 1
    fig_line.add_trace(
        go.Scatter(
            x=scan_index_list,
            y=reduced_num_active_objs_real_list,
            mode="lines",
            name="Veh Reduced No. Of Active Objs",
            line=dict(color="blue"),
        ),
        row=row,
        col=col,
    )
    fig_line.add_trace(
        go.Scatter(
            x=scan_index_list,
            y=reduced_num_active_objs_sim_list,
            mode="lines",
            name="Sim Reduced No. Of Active Objs",
            line=dict(color="red"),
        ),
        row=row,
        col=col,
    )
    fig_line.update_yaxes(title_text="Reduced No. Of Active Objs", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 3, 1
    fig_line.add_trace(
        go.Scatter(
            x=scan_index_list,
            y=num_active_clusters_real_list,
            mode="lines",
            name="Veh No. Of Active Clusters",
            line=dict(color="blue"),
        ),
        row=row,
        col=col,
    )
    fig_line.add_trace(
        go.Scatter(
            x=scan_index_list,
            y=num_active_clusters_sim_list,
            mode="lines",
            name="Sim No. Of Active Clusters",
            line=dict(color="red"),
        ),
        row=row,
        col=col,
    )
    fig_line.update_yaxes(title_text="No. Of Active Clusters", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    row, col = 4, 1
    fig_line.add_trace(
        go.Scatter(
            x=scan_index_list,
            y=number_of_historic_detections_real_list,
            mode="lines",
            name="Veh No. Of Historic Detections",
            line=dict(color="blue"),
        ),
        row=row,
        col=col,
    )
    fig_line.add_trace(
        go.Scatter(
            x=scan_index_list,
            y=number_of_historic_detections_real_list,
            mode="lines",
            name="Sim No. Of Historic Detections",
            line=dict(color="red"),
        ),
        row=row,
        col=col,
    )
    fig_line.update_yaxes(title_text="No. Of Historic Detections", row=row, col=col)
    fig_line.update_xaxes(title_text="Scan Index", row=row, col=col)

    # Update layout for plot
    fig_line.update_layout(height=1200, width=1250, title_text="Parameters v/s scanindex", showlegend=True)
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


file_suffix = '_UDP_GEN7_ROT_TRACKER_INFO.csv'
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
    global elapsed_time_s_real_list, elapsed_time_s_sim_list
    global reduced_num_active_objs_real_list, reduced_num_active_objs_sim_list
    global num_active_clusters_real_list, num_active_clusters_sim_list
    global number_of_historic_detections_real_list, number_of_historic_detections_sim_list
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
        elapsed_time_s_real_list = []
        elapsed_time_s_sim_list = []
        reduced_num_active_objs_real_list = []
        reduced_num_active_objs_sim_list = []
        num_active_clusters_real_list = []
        num_active_clusters_sim_list = []
        number_of_historic_detections_real_list = []
        number_of_historic_detections_sim_list = []
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
            output_html = output_folder + f"/tracker_info_kpi_report_{i+1:04d}.html"
            with open(output_html, "w") as f:
                f.write(html_content)
                f.close()
            html_content = "<table><body><html>"


#####################################################################
# START
#####################################################################
if len(sys.argv) != 4:
    print("Usage: python tracker_info_kpi_script.py log_path.txt meta_data.txt C:\\Gitlab\\gen7v1_resim_kpi_scripts")
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
elapsed_time_s_real_list = []
elapsed_time_s_sim_list = []
reduced_num_active_objs_real_list = []
reduced_num_active_objs_sim_list = []
num_active_clusters_real_list = []
num_active_clusters_sim_list = []
number_of_historic_detections_real_list = []
number_of_historic_detections_sim_list = []
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