"""
File Name: ResimHTMLReport.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This module parse input arguments and imports all the modules and creates objects of it.
Parses input and output files ( both for single and continuous file mode ) and call
execute function to start sequence of operations.

This is main and starting application file.

To generate executable on win and Lin environment use below command
Go to IPS folder and open command prompt and enter the below command
--------------------------------------------
pyinstaller --onefile ResimHTMLReport.py
-------------------------------------------
Binary will be created in dist folder.
Use Linux binary and create docker 


"""


import html
import sys
import threading
import traceback

from IPS.EventMan.data_event_mediator import DataEventMediator
from IPS.DataCollect.radar_hdf_datacollect import RadarHDFDataCollect
from IPS.DataCollect.dc_radar_hdf_datacollect import RadarHDFDCDataCollect
from IPS.DataCollect.parsercontext import ParserContext
from IPS.DataStore.radar_datastore import RadarDataStore
from IPS.DataStore.plot_datastore import PlotDataStore
from IPS.DataPrep.dc_dataprep import DCPlotDataPreparation
from IPS.DataStore.json_datastore import JSONDataStore
from IPS.DataPrep.radar_dataprep import RadarDataPreparation
from IPS.DataPrep.plot_dataprep import PlotDataPreparation
from IPS.PlotConvert.data_to_json_convert import DataToJSONConvert
from IPS.PlotConvert.dc_data_to_json_convert import DCDataToJSONConvert
from IPS.RepGen.json_to_html_convert import JsonToHtmlConvertor
from IPS.RepGen.dc_json_to_html_convert import DCJsonToHtmlConvertor
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DashManager.report_dash import ReportDash
from IPS.InputParsing.input_parsing import HDFConfigSingleton
from IPS.DataCollect.can_radar_hdf_datacollect import RadarHDFCANDataCollect
from IPS.DataPrep.can_dataprep import CANPlotDataPreparation
from IPS.PlotConvert.can_data_to_json_convert import CANDataToJSONConvert
from IPS.RepGen.can_json_to_html_convert import CANJsonToHtmlConvertor
import IPS.Metadata.GEN7V2.poi as poi
from IPS.NIPS_to_IPS_ploting import HTMLNIPStoInteractiveTool
from IPS.DGPSFilter.dgps_filter import DGPSFilter, FilteredResults
from IPS.DGPSFilter.hdf_temp_manager import create_filtered_temp_hdfs, cleanup_filtered_temp_hdfs
from IPS.Utilities.hdf_validator import HDFValidator
import os
import json
import zmq
import plotly.io as pio
from collections import defaultdict
import time
import argparse
import shutil
import plotly.graph_objects as go
import xml.etree.ElementTree as ET
import tempfile

if __name__ == '__main__':
    print("--------------------------------------------------")
    print("HTML Interactive tool...started")
    print("Tool version : V3.0")
    if len(sys.argv) < 3:
        port = sys.argv[1]
        tool = HTMLNIPStoInteractiveTool(port)
        threading.Thread(target=tool.worker, daemon=True).start()
        threading.Thread(target=tool.NIPS_communication, daemon=True).start()
        print("Usage: ResimHTMLReport.exe <port>")

    else:
        parser = argparse.ArgumentParser(description="Parsing HDF for KPI Plotting")
        parser.add_argument("config", help="path of config file")
        parser.add_argument("input", help="Path to input file")
        parser.add_argument("report_path", help="Path to report file")

        args = parser.parse_args()
        print("--------------------------------------------------")
        print("Input File Path :", args.input)
        print("Report  File Path:", args.report_path)
        print("Config:", args.config)
        print("-----------------------------------------")

        input_json = None
        try:
            with open(args.input, 'r') as f:
                input_json = json.load(f)
        except Exception:
            input_json = None

        try:
            if input_json is not None and 'export_png' in input_json:
                ReportDash.export_png = bool(input_json.get('export_png', True))
        except Exception:
            pass

        # Load and parse XML
        tree = ET.parse(args.config)
        root = tree.getroot()

        # Extract source type
        source_type = root.text.strip()
        print(f"Selected source: {source_type}")

        source_type = source_type.lower()
        poi.datasource = source_type
        print(f" poi.datasource datasource: {poi.datasource}")
        # object creations
        # Signal selection is defined by Metadata (poi.py) for all source types.
        sig_fil = SignalFilter(None)

        event_mediator = DataEventMediator()
        radar_ds = RadarDataStore()
        plot_ds = PlotDataStore()
        json_ds = JSONDataStore()

        report_dash = ReportDash(event_mediator)

        if source_type == "udp":
            ReportDash.datasource_type = "udp"
            radar_data_collect = RadarHDFDataCollect(event_mediator, radar_ds, plot_ds)
            radar_plot_prep = PlotDataPreparation(event_mediator, radar_ds, plot_ds, json_ds, sig_fil)
            data_json_conv = DataToJSONConvert(event_mediator, radar_ds, plot_ds, json_ds, radar_plot_prep, sig_fil)
            json_html_conv = JsonToHtmlConvertor(event_mediator, radar_ds, plot_ds, json_ds, report_dash)
        elif source_type in ["udpdc", "dcdgps"]:
            ReportDash.datasource_type = "udpdc"
            radar_data_collect = RadarHDFDCDataCollect(event_mediator, radar_ds, plot_ds,sig_fil,report_dash)
            radar_plot_prep = DCPlotDataPreparation(event_mediator, radar_ds, plot_ds, json_ds, sig_fil,
                                                    radar_data_collect)
            data_json_conv = DCDataToJSONConvert(event_mediator, radar_ds, plot_ds, json_ds, radar_plot_prep, sig_fil,
                                                 radar_data_collect)
            json_html_conv = DCJsonToHtmlConvertor(event_mediator, radar_ds, plot_ds, json_ds, report_dash)
        elif source_type in ["mcip_can", "ceer_can"]:
            ReportDash.datasource_type = "can"
            radar_data_collect = RadarHDFCANDataCollect(event_mediator, radar_ds, plot_ds)
            radar_plot_prep = CANPlotDataPreparation(event_mediator, radar_ds, plot_ds, json_ds, sig_fil,
                                                     radar_data_collect)
            data_json_conv = CANDataToJSONConvert(event_mediator, radar_ds, plot_ds, json_ds, radar_plot_prep, sig_fil)
            json_html_conv = CANJsonToHtmlConvertor(event_mediator, radar_ds, plot_ds, json_ds, report_dash)

        radar_data_prep = RadarDataPreparation(event_mediator, radar_ds, plot_ds)

        context = ParserContext(radar_data_collect)

        # object registration
        sig_fil.attach_sig_observer(radar_data_collect)
        event_mediator.register(radar_data_collect)
        event_mediator.register(radar_data_prep)
        event_mediator.register(radar_plot_prep)
        event_mediator.register(data_json_conv)
        event_mediator.register(json_html_conv)
        event_mediator.register(report_dash)

        cfg = HDFConfigSingleton(args.input)
        
        # ============ DGPS Configuration (for DCDGPS mode) ============
        dgps_config = None
        dgps_filter = None
        is_dcdgps_mode = (source_type == "dcdgps")
        
        if is_dcdgps_mode:
            if input_json and 'DGPS' in input_json:
                dgps_filter = DGPSFilter(input_json=input_json)
            else:
                print("Error: DCDGPS mode requires a 'DGPS' section with 'roi_radius' in the input JSON.")
                raise SystemExit(1)
        # ============ End DGPS Configuration ============
        
        report_pair_count = 0
        for in_f, out_f in cfg.all_pairs():
            print("-----------------------------------------")
            start_time = time.time()
            report_dash.clear_data()
            report_pair_count = report_pair_count + 1
            
            # Get current working directory
            cwd = os.getcwd()
            # Define the report folder path
            report_folder = os.path.join(args.report_path,
                                         "IRep" + str(report_pair_count) + "_" + os.path.basename(in_f))
            ReportDash.report_directory = report_folder
            ReportDash.input_hdf = os.path.basename(in_f)
            ReportDash.output_hdf = os.path.basename(out_f)
            
            # Run normal processing flow (same for UDPDC and DCDGPS)
            print("Processing:", in_f, "->", out_f)
            print("please wait ......")          
            # Input parsing
            file_list = [in_f, out_f]
            file_type = ["in", "out"]
            
            # Validate HDF files before processing
            print("Validating HDF files...")
            is_valid, error_msg = HDFValidator.validate_hdf_pair(in_f, out_f)
            if not is_valid:
                print(f"ERROR: {error_msg}")
                print(f"Skipping processing for {in_f} and {out_f}")
                continue
            print("HDF files validation passed.")

            # function calling
            # use the metadata POI signals dictionary and create dataset path
            if source_type in ["mcip_can", "ceer_can"]:
                sig_fil.get_can_poi_ref()
            else:
                sig_fil.get_poi_ref()
            for file, file_type in zip(file_list, file_type):
                try:
                    sig_fil.parse_group_signals(file, file_type)
                except ValueError as e:
                    print(f"ERROR: {e}")
                    print(f"Skipping processing for this file pair.")
                    continue

            sig_fil.fiter_common_signals()
            if source_type in ["mcip_can", "ceer_can"]:
                sig_fil.final_can_poi_signals()
                final_value = SignalFilter.common_sig
            else:
                sig_fil.final_poi_signals()
            sig_fil.notify_sig_observer()

            # For DCDGPS mode always create temp HDFs with ROI-filtered detections
            in_use = in_f
            out_use = out_f
            temp_files = []
            if is_dcdgps_mode and dgps_filter is not None:
                try:
                    in_use, out_use, temp_files = create_filtered_temp_hdfs(report_folder, in_f, out_f, dgps_filter)
                    if temp_files:
                        print(f"  Using filtered temp HDFs: {', '.join(temp_files)}")
                except Exception as e:
                    print(f"  Error creating temp HDFs: {e}")

            # radar_data_collect.collect_data(in_use, out_use)
            context.execute(in_use, out_use)
            # ============ End DCDGPS ============
            
            print("completed")
            radar_ds.clear_data()
            plot_ds.clear_data()
            json_ds.clear_data()
            sig_fil.clear_data()

            radar_data_collect.clear_data()
            radar_data_prep.clear_data()
            radar_plot_prep.clear_data()
            json_html_conv.clear_data()

            # End timing
            end_time = time.time()
            elapsed_time = end_time - start_time
            ReportDash.report_gen_time = round(elapsed_time, 2)
            print("ReportDash.report_gen_time", ReportDash.report_gen_time)
            report_dash.generate_rep()

            # Cleanup temp HDFs if created
            if temp_files:
                try:
                    cleanup_filtered_temp_hdfs(temp_files, report_folder)
                except Exception as e:
                    print(f"  Failed cleaning up temp HDFs: {e}")

            print(f"Execution time: {elapsed_time:.4f} seconds")
            print("--------------------------------------------------")

        print("Resim Interactive HTML...Completed")
