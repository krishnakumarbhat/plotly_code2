import gc, os
import plotly.graph_objects as go
from IPS.DashManager.report_dash import ReportDash
from IPS.DataPrep.plot_dataprep import PlotDataPreparation
from IPS.DataStore.idatastore import IDataStore
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.Sig_Prep.isig_observer import ISigObserver
from IPS.Sig_Prep.sig_filter import SignalFilter

class CANDataToJSONConvert(ISigObserver):
    SENSOR_MAP = {"MCIP_FR": "FR", "MCIP_FL": "FL", "MCIP_RR": "RR", "MCIP_RL": "RL", "MCIP_FLR": "FLR",
                  "CEER_FR": "FR", "CEER_FL": "FL", "CEER_RR": "RR", "CEER_RL": "RL", "CEER_FLR": "FLR"}

    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore, json_ds: IDataStore, plot_prep: PlotDataPreparation, sig_fil: SignalFilter):
        self._mediator, self._radar_ds, self._plot_ds, self._json_ds = mediator, radar_ds, plot_ds, json_ds
        self._plot_prep, self._sig_fil = plot_prep, sig_fil

    def final_poi_sensor_datapath(self, d): pass

    def consume_event(self, sensor, event):
        if event != "CAN_DATA_PREP_UPDATED": return
        signals = ["DET_RANGE", "DET_RCS", "DET_AZIMUTH", "DET_ELEVATION", "DET_RANGE_VELOCITY", "DET_SNR"]
        streams = ["SRR_FR_DETECTION", "SRR_FL_DETECTION", "SRR_RR_DETECTION", "SRR_RL_DETECTION", "FLR_DETECTION"]
        for stream in streams:
            for sig in signals:
                try:
                    types = self._sig_fil.get_sig_plot_type(stream, sig)
                    if types and "SC" in types:
                        self._scatter(sensor, stream, sig, self._sig_fil.get_sig_unit(stream, sig))
                except Exception as e:
                    print(f"[Error] {sig} for {sensor}: {e}")

    def _scatter(self, sensor, stream, sig, unit):
        if sig.lower() == "scan_index": return
        in_ts = self._plot_ds.get_data("flattened_timestamp", "in", sensor)
        in_sig = self._plot_ds.get_data(f"flattened_{sig.lower()}", "in", sensor)
        out_ts = self._plot_ds.get_data("flattened_timestamp", "out", sensor)
        out_sig = self._plot_ds.get_data(f"flattened_{sig.lower()}", "out", sensor)
        fig = go.Figure()
        if in_ts and in_sig: fig.add_trace(go.Scattergl(x=in_ts, y=in_sig, mode='markers', marker=dict(size=3, opacity=0.5, color="blue"), name="input"))
        if out_ts and out_sig: fig.add_trace(go.Scattergl(x=out_ts, y=out_sig, mode='markers', marker=dict(size=3, opacity=0.5, color="red"), name="output"))
        fig.update_layout(xaxis_title="Timestamp (ms)", yaxis_title=str(unit), title=f"{sig} vs Timestamp")
        gc.collect()
        sensor_mapped = self.SENSOR_MAP.get(sensor, sensor)
        stream_part = stream.split("_")[-1]
        folder = os.path.join(ReportDash.report_directory, "JSON", sensor_mapped)
        os.makedirs(folder, exist_ok=True)
        path = os.path.join(folder, f"{sensor_mapped}_{stream_part}_{sig.lower()}_scatter.json")
        PlotDataPreparation.list_json_path.append(os.path.basename(path))
        self._json_ds.update_data(sensor_mapped, path, "scatter")
        try:
            with open(path, "w", encoding="utf-8", newline="\n") as f: f.write(fig.to_json())
        except Exception as e:
            print(f"[Error] JSON write failed for {sig}: {e}")
