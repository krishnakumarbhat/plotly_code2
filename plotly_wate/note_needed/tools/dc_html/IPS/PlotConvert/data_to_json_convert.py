"""

Description:
This module will convert processed data to Json file
"""

import gc, os
import plotly.graph_objects as go
from IPS.DashManager.report_dash import ReportDash
from IPS.DataPrep.plot_dataprep import PlotDataPreparation
from IPS.DataStore.idatastore import IDataStore
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.Sig_Prep.isig_observer import ISigObserver
from IPS.Sig_Prep.sig_filter import SignalFilter

class DataToJSONConvert(ISigObserver):
    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore, json_ds: IDataStore, plot_prep: PlotDataPreparation, sig_fil: SignalFilter):
        self._mediator = mediator
        self._radar_ds, self._plot_ds, self._json_ds = radar_ds, plot_ds, json_ds
        self._plot_prep, self._sig_fil = plot_prep, sig_fil

    def final_poi_sensor_datapath(self, data): pass

    def consume_event(self, info, event):
        if event != "JDS_UPDATED": return
        parts = info.split('#')
        if parts[1] != 'scan_index':
            self._generate_plots(parts[0], parts[1], parts[2])

    def _generate_plots(self, sensor, stream, sig):
        plots = self._sig_fil.get_sig_plot_type(stream, sig)
        unit = self._sig_fil.get_sig_unit(stream, sig)
        stream = stream.split('/')[-2] if '/' in stream else stream
        if "SC" in (plots or []):
            self._scatter(sensor, stream, sig, unit)
        if "HIS" in (plots or []):
            self._histogram(sensor, stream, sig)
        # Mismatch plots commented out as they are not proper
        # if "SC_MM" in (plots or []) and sig != "scan_index":
        #     for s, mm_sig in self._plot_ds.mismatch_signal_list.items():
        #         self._mismatch_scatter(s, stream, mm_sig, unit)

    def _write_fig(self, fig, sensor, stream, sig, suffix, ds_type):
        folder = os.path.join(ReportDash.report_directory, "JSON", sensor)
        os.makedirs(folder, exist_ok=True)
        path = os.path.join(folder, f"{sensor}_{stream}_{suffix}_{sig}.json")
        PlotDataPreparation.list_json_path.append(os.path.basename(path))
        self._json_ds.update_data(sensor, path, ds_type)
        with open(path, "w", encoding="utf-8", newline="\n") as f:
            f.write(fig.to_json())
        gc.collect()

    def _scatter(self, sensor, stream, sig, unit):
        if sig == "scan_index": return
        in_data, out_data = self._plot_ds.get_data(sig, "in"), self._plot_ds.get_data(sig, "out")
        if in_data is None and out_data is None: return
        fig = go.Figure()
        if in_data is not None:
            in_idx = self._plot_ds.get_data(sig, "in_scan_index", sensor)
            fig.add_trace(go.Scattergl(x=in_idx.tolist() if in_idx is not None else None, y=in_data.tolist(), mode='markers', marker=dict(size=.3, opacity=0.9, color="blue"), name="input"))
        if out_data is not None:
            out_idx = self._plot_ds.get_data(sig, "out_scan_index", sensor)
            fig.add_trace(go.Scattergl(x=out_idx.tolist() if out_idx is not None else None, y=out_data.tolist(), mode='markers', marker=dict(size=1.3, opacity=0.9, color="red"), name="output"))
        fig.update_layout(xaxis_title="Scan Index", yaxis_title=str(unit))
        self._write_fig(fig, sensor, stream, sig, "scatter", "scatter")

    def _histogram(self, sensor, stream, sig):
        in_data, out_data = self._plot_ds.get_data(sig, "in"), self._plot_ds.get_data(sig, "out")
        if in_data is None and out_data is None: return
        fig = go.Figure([go.Histogram(x=in_data.tolist() if in_data is not None else None, opacity=0.5, name='input', marker_color='blue'),
                         go.Histogram(x=out_data.tolist() if out_data is not None else None, opacity=0.5, name='output', marker_color='red')])
        fig.update_layout(barmode='overlay', title='Histogram', xaxis_title='Value', yaxis_title='Count')
        self._write_fig(fig, sensor, stream, sig, "histogram", "Histogram")

    def _mismatch_scatter(self, sensor, stream, sig, unit):
        data = self._plot_ds.get_data(sig, "mismatch_value_in", sensor)
        idx = self._plot_ds.get_data(sig, "mismatch_scan_index_in", sensor)
        if not data or len(data) == 0: return
        fig = go.Figure([go.Scattergl(x=idx, y=data, mode='markers', marker=dict(size=1.3, opacity=0.8, color="blue"), name='input')])
        fig.update_layout(xaxis_title="Scan Index", yaxis_title=str(unit))
        self._write_fig(fig, sensor, stream, sig, "mismatch", "mismatch")
