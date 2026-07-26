"""
Description:
This module will convert DC processed data to Json file
"""
import gc, os
import math
import plotly.graph_objects as go
import matplotlib.pyplot as plt
from IPS.DashManager.report_dash import ReportDash
from IPS.DataPrep.plot_dataprep import PlotDataPreparation
from IPS.DataStore.idatastore import IDataStore
from IPS.DataCollect.icollectdata import IDataCollect
from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.Sig_Prep.isig_observer import ISigObserver
from IPS.Sig_Prep.sig_filter import SignalFilter

class DCDataToJSONConvert(ISigObserver):
    # Unit conversions: (display_unit, conversion_factor)
    # conversion_factor is multiplied with the original value
    UNIT_CONVERSIONS = {
        # Position: m -> km (divide by 1000) - EXCEPT vcs_pos_x/y which stay in meters
        'xpos': ('km', 0.001),
        'ypos': ('km', 0.001),
        'xposn': ('km', 0.001),
        'yposn': ('km', 0.001),
        'curvi_pos_x': ('km', 0.001),
        'curvi_pos_y': ('km', 0.001),
        # Velocity: m/s -> km/hr (multiply by 3.6)
        'vel_x': ('km/hr', 3.6),
        'vel_y': ('km/hr', 3.6),
        'xvel': ('km/hr', 3.6),
        'yvel': ('km/hr', 3.6),
        'curvi_vel_x': ('km/hr', 3.6),
        'curvi_vel_y': ('km/hr', 3.6),
        'speed': ('km/hr', 3.6),
        'raw_speed': ('km/hr', 3.6),
        # Heading/angles: already in deg or convert rad -> deg
        'heading': ('deg', 1.0),
        'curvi_heading': ('deg', 1.0),
        'azimuth': ('deg', 180.0 / math.pi),  # rad -> deg
        'elevation': ('deg', 180.0 / math.pi),  # rad -> deg
        'phi': ('deg', 1.0),
        'theta': ('deg', 1.0),
        # Yaw rate: rad/s -> deg/s or already deg/s
        'yaw_rate': ('deg/sec', 1.0),
        'raw_yaw_rate': ('deg/sec', 1.0),
    }

    # Signals that should NOT be converted (stay in original units)
    NO_CONVERSION = {'vcs_pos_x', 'vcs_pos_y'}

    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore, json_ds: IDataStore, plot_prep: PlotDataPreparation, sig_fil: SignalFilter, dc_collector: IDataCollect):
        self._mediator = mediator
        self._radar_ds, self._plot_ds, self._json_ds = radar_ds, plot_ds, json_ds
        self._sig_fil, self._dc_collector = sig_fil, dc_collector

    def final_poi_sensor_datapath(self, d): pass

    def _get_unit_and_factor(self, sig, default_unit):
        """Get display unit and conversion factor for a signal."""
        sig_lower = sig.lower()
        # Check if signal should NOT be converted (stay in original units)
        if sig_lower in self.NO_CONVERSION:
            return default_unit, 1.0
        for key, (unit, factor) in self.UNIT_CONVERSIONS.items():
            if key in sig_lower:
                return unit, factor
        # Default: no conversion
        return default_unit, 1.0

    def consume_event(self, info, event):
        if event != "JDS_UPDATED": return
        parts = info.split('#')
        if parts[1] != 'scan_index':
            self._generate(parts[0], parts[1], parts[2])

    def _generate(self, sensor, stream, sig):
        default_unit = self._dc_collector.get_unit(sig)
        unit, factor = self._get_unit_and_factor(sig, default_unit)
        self._scatter(sensor, stream, sig, unit, factor)
        self._histogram(sensor, stream, sig, factor)

    def _write_fig(self, fig, sensor, stream, sig, suffix, ds_type):
        folder = os.path.join(ReportDash.report_directory, "JSON", sensor)
        os.makedirs(folder, exist_ok=True)
        stream = stream.split('/')[-2] if '/' in stream else stream.replace("_", "")
        path = os.path.join(folder, f"{sensor}_{stream}_{suffix}_{sig}.json")
        PlotDataPreparation.list_json_path.append(os.path.basename(path))
        self._json_ds.update_data(sensor, path, ds_type)
        with open(path, "w", encoding="utf-8", newline="\n") as f:
            f.write(fig.to_json())
        gc.collect()

    def _histogram(self, sensor, stream, sig, factor):
        in_data, out_data = self._plot_ds.get_data(sig, "in"), self._plot_ds.get_data(sig, "out")
        if in_data is None and out_data is None: return
        # Apply conversion factor
        in_conv = [v * factor for v in in_data.tolist()] if in_data is not None else None
        out_conv = [v * factor for v in out_data.tolist()] if out_data is not None else None
        fig = go.Figure([go.Histogram(x=in_conv, opacity=0.5, name='input', marker_color='blue'),
                         go.Histogram(x=out_conv, opacity=0.5, name='output', marker_color='green')])
        fig.update_layout(barmode='overlay', title='Histogram', xaxis_title='Value', yaxis_title='Count')
        self._write_fig(fig, sensor, stream, sig, "histogram", "Histogram")

    def _scatter(self, sensor, stream, sig, unit, factor):
        if sig == "scan_index": return
        import numpy as np
        in_data = self._plot_ds.get_data(sig, "in")
        out_data = self._plot_ds.get_data(sig, "out")
        in_idx = self._plot_ds.get_data("DC", "in_scan_index", sensor) if in_data is not None else None
        out_idx = self._plot_ds.get_data("DC", "out_scan_index", sensor) if out_data is not None else None
        
        # Flatten data if 2D to ensure consistent shape
        if in_data is not None and in_data.ndim > 1:
            in_data = in_data.ravel()
        if out_data is not None and out_data.ndim > 1:
            out_data = out_data.ravel()
        if in_idx is not None and in_idx.ndim > 1:
            in_idx = in_idx.ravel()
        if out_idx is not None and out_idx.ndim > 1:
            out_idx = out_idx.ravel()
        
        # # DEBUG: Print what we're plotting
        # print(f"\n[DEBUG SCATTER] Signal: {sig}, Sensor: {sensor}")
        # if in_data is not None:
        #     print(f"  in_data: len={len(in_data)}, min={np.min(in_data):.4f}, max={np.max(in_data):.4f}, non-zero={np.count_nonzero(in_data)}")
        # if out_data is not None:
        #     print(f"  out_data: len={len(out_data)}, min={np.min(out_data):.4f}, max={np.max(out_data):.4f}, non-zero={np.count_nonzero(out_data)}")
        # if in_idx is not None:
        #     print(f"  in_idx: len={len(in_idx)}, min={np.min(in_idx):.4f}, max={np.max(in_idx):.4f}")
        # if out_idx is not None:
        #     print(f"  out_idx: len={len(out_idx)}, min={np.min(out_idx):.4f}, max={np.max(out_idx):.4f}")
        
        fig = go.Figure()
        
        # Filter out zeros and NaN, then apply conversion factor
        # Plot ALL data points directly instead of using dictionary (which loses duplicate indices)
        if in_idx is not None and in_data is not None:
            # Ensure arrays have matching lengths
            min_len = min(len(in_idx), len(in_data))
            in_idx_slice = in_idx[:min_len]
            in_data_slice = in_data[:min_len]
            # Filter valid data (non-zero, non-nan)
            valid_mask = (~np.isnan(in_data_slice)) & (in_data_slice != 0.0)
            in_x = in_idx_slice[valid_mask]
            in_y = in_data_slice[valid_mask] * factor
            # print(f"  After filtering in_data: {len(in_y)} points")
            if len(in_y) > 0:
                fig.add_trace(go.Scattergl(
                    x=in_x.tolist(), 
                    y=in_y.tolist(), 
                    mode='markers', 
                    marker=dict(size=1.5, opacity=0.8, color="blue"), 
                    name="input"
                ))
        
        if out_idx is not None and out_data is not None:
            # Ensure arrays have matching lengths
            min_len = min(len(out_idx), len(out_data))
            out_idx_slice = out_idx[:min_len]
            out_data_slice = out_data[:min_len]
            # Filter valid data (non-zero, non-nan)
            valid_mask = (~np.isnan(out_data_slice)) & (out_data_slice != 0.0)
            out_x = out_idx_slice[valid_mask]
            out_y = out_data_slice[valid_mask] * factor
            # print(f"  After filtering out_data: {len(out_y)} points")
            if len(out_y) > 0:
                fig.add_trace(go.Scattergl(
                    x=out_x.tolist(), 
                    y=out_y.tolist(), 
                    mode='markers', 
                    marker=dict(size=1.5, opacity=0.8, color="red"), 
                    name="output"
                ))
        
        fig.update_layout(xaxis_title="Scan Index", yaxis_title=str(unit))
        self._write_fig(fig, sensor, stream, sig, "scatter", "scatter")
        if ReportDash.datasource_type == "udpdc":
            self._save_png(sensor, stream, sig, unit, factor, in_idx, in_data, out_idx, out_data)

    def _save_png(self, sensor, stream, sig, unit, factor, x1, y1, x2, y2):
        import numpy as np
        folder = os.path.join(ReportDash.report_directory, "image", sensor)
        os.makedirs(folder, exist_ok=True)
        stream = stream.split('/')[-2] if '/' in stream else stream.replace("_", "")
        path = os.path.join(folder, f"{sensor}_{stream}_scatter_{sig}.png")
        try:
            fig_m, ax = plt.subplots(figsize=(8, 5), dpi=150)
            
            # Plot ALL data points directly, filtering out zeros and NaN
            if x1 is not None and y1 is not None:
                # Flatten arrays if needed
                x1_flat = x1.ravel() if hasattr(x1, 'ravel') else np.array(x1).ravel()
                y1_flat = y1.ravel() if hasattr(y1, 'ravel') else np.array(y1).ravel()
                # Ensure matching lengths
                min_len = min(len(x1_flat), len(y1_flat))
                x1_slice = x1_flat[:min_len]
                y1_slice = y1_flat[:min_len]
                valid_mask = (~np.isnan(y1_slice)) & (y1_slice != 0.0)
                in_x = x1_slice[valid_mask]
                in_y = y1_slice[valid_mask] * factor
                if len(in_y) > 0:
                    ax.scatter(in_x, in_y, s=1, alpha=0.7, c="blue", label="input", rasterized=True)
            
            if x2 is not None and y2 is not None:
                # Flatten arrays if needed
                x2_flat = x2.ravel() if hasattr(x2, 'ravel') else np.array(x2).ravel()
                y2_flat = y2.ravel() if hasattr(y2, 'ravel') else np.array(y2).ravel()
                # Ensure matching lengths
                min_len = min(len(x2_flat), len(y2_flat))
                x2_slice = x2_flat[:min_len]
                y2_slice = y2_flat[:min_len]
                valid_mask = (~np.isnan(y2_slice)) & (y2_slice != 0.0)
                out_x = x2_slice[valid_mask]
                out_y = y2_slice[valid_mask] * factor
                if len(out_y) > 0:
                    ax.scatter(out_x, out_y, s=1, alpha=0.7, c="red", label="output", rasterized=True)
            
            ax.set_xlabel("Scan Index")
            ax.set_ylabel(str(unit))
            # Only add legend if there are labeled artists
            handles, labels = ax.get_legend_handles_labels()
            if handles:
                ax.legend(loc="best", fontsize=6)
            ax.grid(True, linestyle="--", alpha=0.4)
            fig_m.tight_layout()
            fig_m.savefig(path, bbox_inches="tight")
            plt.close(fig_m)
        except Exception as e:
            print(f"PNG export failed: {e}")
