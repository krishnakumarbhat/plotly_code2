"""
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
import sys, os, time, threading, argparse
import xml.etree.ElementTree as ET
from IPS.EventMan.data_event_mediator import DataEventMediator
from IPS.DataCollect.radar_hdf_datacollect import RadarHDFDataCollect
from IPS.DataCollect.dc_radar_hdf_datacollect import RadarHDFDCDataCollect
from IPS.DataCollect.can_radar_hdf_datacollect import RadarHDFCANDataCollect
from IPS.DataCollect.parsercontext import ParserContext
from IPS.DataStore.radar_datastore import RadarDataStore
from IPS.DataStore.plot_datastore import PlotDataStore
from IPS.DataStore.json_datastore import JSONDataStore
from IPS.DataPrep.radar_dataprep import RadarDataPreparation
from IPS.DataPrep.plot_dataprep import PlotDataPreparation
from IPS.DataPrep.dc_dataprep import DCPlotDataPreparation
from IPS.DataPrep.can_dataprep import CANPlotDataPreparation
from IPS.PlotConvert.data_to_json_convert import DataToJSONConvert
from IPS.PlotConvert.dc_data_to_json_convert import DCDataToJSONConvert
from IPS.PlotConvert.can_data_to_json_convert import CANDataToJSONConvert
from IPS.RepGen.json_to_html_convert import JsonToHtmlConvertor
from IPS.RepGen.dc_json_to_html_convert import DCJsonToHtmlConvertor
from IPS.RepGen.can_json_to_html_convert import CANJsonToHtmlConvertor
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DashManager.report_dash import ReportDash
from IPS.InputParsing.input_parsing import HDFConfigSingleton
from IPS.NIPS_to_IPS_ploting import HTMLNIPStoInteractiveTool
import IPS.Metadata.GEN7V2.poi as poi

if __name__ == '__main__':
    print("HTML Interactive tool V3.0 started")
    if len(sys.argv) < 3:
        port = sys.argv[1]
        tool = HTMLNIPStoInteractiveTool(port)
        threading.Thread(target=tool.worker, daemon=True).start()
        threading.Thread(target=tool.NIPS_communication, daemon=True).start()
        print("Usage: ResimHTMLReport.exe <port>")
    else:
        parser = argparse.ArgumentParser(description="HDF KPI Plotting")
        parser.add_argument("config", help="Config file path")
        parser.add_argument("input", help="Input file path")
        parser.add_argument("report_path", help="Report output path")
        args = parser.parse_args()
        print(f"Input: {args.input}\nReport: {args.report_path}\nConfig: {args.config}")
        
        tree = ET.parse(args.config)
        source_type = tree.getroot().text.strip().lower()
        poi.datasource = source_type
        print(f"Source: {source_type}")
        
        # Look for add_signal_stream.txt in IPS folder, not Python executable folder
        ips_dir = os.path.dirname(os.path.abspath(__file__))
        sig_txt = os.path.join(ips_dir, "add_signal_stream.txt")
        sig_fil = SignalFilter(sig_txt if source_type == "udpdc" and os.path.exists(sig_txt) else None)
        event_mediator = DataEventMediator()
        radar_ds, plot_ds, json_ds = RadarDataStore(), PlotDataStore(), JSONDataStore()
        report_dash = ReportDash(event_mediator)
        
        if source_type == "udp":
            ReportDash.datasource_type = "udp"
            collector = RadarHDFDataCollect(event_mediator, radar_ds, plot_ds)
            plot_prep = PlotDataPreparation(event_mediator, radar_ds, plot_ds, json_ds, sig_fil)
            json_conv = DataToJSONConvert(event_mediator, radar_ds, plot_ds, json_ds, plot_prep, sig_fil)
            html_conv = JsonToHtmlConvertor(event_mediator, radar_ds, plot_ds, json_ds, report_dash)
        elif source_type == "udpdc":
            ReportDash.datasource_type = "udpdc"
            collector = RadarHDFDCDataCollect(event_mediator, radar_ds, plot_ds, sig_fil, report_dash)
            plot_prep = DCPlotDataPreparation(event_mediator, radar_ds, plot_ds, json_ds, sig_fil, collector)
            json_conv = DCDataToJSONConvert(event_mediator, radar_ds, plot_ds, json_ds, plot_prep, sig_fil, collector)
            html_conv = DCJsonToHtmlConvertor(event_mediator, radar_ds, plot_ds, json_ds, report_dash)
        elif source_type in ["mcip_can", "ceer_can"]:
            ReportDash.datasource_type = "can"
            collector = RadarHDFCANDataCollect(event_mediator, radar_ds, plot_ds)
            plot_prep = CANPlotDataPreparation(event_mediator, radar_ds, plot_ds, json_ds, sig_fil, collector)
            json_conv = CANDataToJSONConvert(event_mediator, radar_ds, plot_ds, json_ds, plot_prep, sig_fil)
            html_conv = CANJsonToHtmlConvertor(event_mediator, radar_ds, plot_ds, json_ds, report_dash)
        
        radar_prep = RadarDataPreparation(event_mediator, radar_ds, plot_ds)
        context = ParserContext(collector)
        
        sig_fil.attach_sig_observer(collector)
        for obj in [collector, radar_prep, plot_prep, json_conv, html_conv, report_dash]:
            event_mediator.register(obj)
        
        cfg = HDFConfigSingleton(args.input)
        for i, (in_f, out_f) in enumerate(cfg.all_pairs(), 1):
            start = time.time()
            report_dash.clear_data()
            print(f"Processing: {in_f} -> {out_f}")
            
            ReportDash.report_directory = os.path.join(args.report_path, f"IRep{i}_{os.path.basename(in_f)}")
            ReportDash.input_hdf, ReportDash.output_hdf = os.path.basename(in_f), os.path.basename(out_f)
            
            if source_type in ["mcip_can", "ceer_can"]: sig_fil.get_can_poi_ref()
            else: sig_fil.get_poi_ref()
            
            for f, ft in [(in_f, "in"), (out_f, "out")]: sig_fil.parse_group_signals(f, ft)
            sig_fil.fiter_common_signals()
            sig_fil.final_can_poi_signals() if source_type in ["mcip_can", "ceer_can"] else sig_fil.final_poi_signals()
            sig_fil.notify_sig_observer()
            context.execute(in_f, out_f)
            
            for obj in [radar_ds, plot_ds, json_ds, sig_fil, collector, radar_prep, plot_prep, html_conv]: obj.clear_data()
            
            ReportDash.report_gen_time = round(time.time() - start, 2)
            print(f"Time: {ReportDash.report_gen_time}s")
            report_dash.generate_rep()
        
        print("Completed")
