"""
File Name: dgps_filter.py
Description:
DGPS HDF-based ROI filtering utilities.

Overview:
- Load Ground Truth (vcs_long_posn, vcs_lat_posn) from a DGPS HDF file.
- Create ROI-filtered copies of detection HDFs by zeroing detections outside a
    circular ROI (written back into a temporary/copy HDF under the report folder).
- Compute detection vehicle coordinates from range/azimuth/elevation while
    applying azimuth polarity, boresight rotation and sensor mounting offsets.
- Mask optional detection signals (`snr`, `amplitude`, `azimuth_confidence`)
    in the copied HDF so downstream pipeline runs unchanged on the filtered files.

Primary API:
- `create_filtered_hdf_copy(src_hdf, dst_hdf, gt_hdf=None, detection_stream=None)`
    — copies `src_hdf` to `dst_hdf` and applies ROI masking in-place on the copy.

Notes:
- Legacy plotting helper methods were removed from this module; the HDF-copy
    approach is the canonical DCDGPS workflow now.
"""

import math
import numpy as np
import h5py
from typing import Tuple, List, Optional, Dict
from dataclasses import dataclass, field
import IPS.Metadata.GEN7V2.poi as poi
import shutil


@dataclass
class Detection:
    """Represents a single radar detection with all properties."""
    scan_index: int
    det_index: int      # Detection index within scan
    x: float            # vcs_x_posn
    y: float            # vcs_y_posn
    range: float = 0.0
    range_rate: float = 0.0
    azimuth: float = 0.0
    elevation: float = 0.0
    within_roi: bool = False
    gt_distance: float = float('inf')
    snr: float = 0.0
    amplitude: float = 0.0
    azimuth_confidence: float = 0.0
    valid: int = 0

@dataclass
class GroundTruth:
    """Represents GT position for a scan index."""
    scan_index: int
    x: float            # vcs_long_posn
    y: float            # vcs_lat_posn


@dataclass
class FilteredResults:
    """Results from ROI filtering - stores filtered detection data for plotting."""
    scan_indices: List[int] = field(default_factory=list)
    det_indices: List[int] = field(default_factory=list)
    range_vals: List[float] = field(default_factory=list)
    range_rate_vals: List[float] = field(default_factory=list)
    azimuth_vals: List[float] = field(default_factory=list)
    elevation_vals: List[float] = field(default_factory=list)
    x_vals: List[float] = field(default_factory=list)
    y_vals: List[float] = field(default_factory=list)
    gt_distances: List[float] = field(default_factory=list)


class DGPSFilter:
    def GetRotationMatrix2D(self, yaw):
        """
        Returns a 2D rotation matrix for rotation by 'yaw' radians.
        """
        cos_yaw = math.cos(yaw)
        sin_yaw = math.sin(yaw)
        return np.matrix([[cos_yaw, -sin_yaw],
                          [sin_yaw,  cos_yaw]])

    def __init__(self, config=None, input_json: Optional[dict] = None, roi_radius: Optional[float] = None):
        """
        Initialize DGPSFilter.

        Parameters:
            config: legacy config object (kept for backward compatibility)
            input_json: full parsed Inputs.json dict; expects a 'DGPS' section
            roi_radius: explicit ROI radius in meters

        One of `roi_radius`, `input_json` (with DGPS section), or `config` must provide the radius.
        """
        # Determine ROI radius (priority: roi_radius arg > input_json > config)
        self.roi_radius: Optional[float] = None
        if roi_radius is not None:
            try:
                self.roi_radius = float(roi_radius)
            except Exception:
                raise ValueError("Provided roi_radius must be numeric")
        elif input_json is not None:
            dgps_section = input_json.get('DGPS') if isinstance(input_json, dict) else None
            if not dgps_section:
                raise ValueError("Input JSON must contain a 'DGPS' section with 'roi_radius'.")
            if 'roi_radius' in dgps_section:
                try:
                    self.roi_radius = float(dgps_section['roi_radius'])
                except Exception:
                    raise ValueError("'DGPS.roi_radius' must be numeric")
            elif 'roi' in dgps_section and isinstance(dgps_section['roi'], dict) and 'radius' in dgps_section['roi']:
                try:
                    self.roi_radius = float(dgps_section['roi']['radius'])
                except Exception:
                    raise ValueError("'DGPS.roi.radius' must be numeric")
            else:
                raise ValueError("DGPS section must include 'roi_radius' or 'roi':{'radius':...}")
        elif config is not None:
            # legacy support: try to read config.roi.radius
            try:
                self.roi_radius = float(getattr(config, 'roi').radius)
            except Exception:
                raise ValueError("Legacy config provided but missing roi.radius")
        else:
            raise ValueError("DGPSFilter requires roi_radius (or input_json with DGPS section)")

        # store a minimal config-like object for compatibility with existing code
        class _Cfg:
            def __init__(self, r):
                self.roi = type('R', (), {'radius': r})()
        self.config = _Cfg(self.roi_radius)

        self.gt_per_scan: Dict[int, Tuple[float, float]] = {}
        self.detections_per_scan: Dict[int, List[Detection]] = {}
        self.filtered_results = FilteredResults()
        self.outside_results = FilteredResults()
        self.all_gt_x: np.ndarray = np.array([])
        self.all_gt_y: np.ndarray = np.array([])
        self.gt_source_scan_indices = set()
        self.common_scan_indices = None
        self.dgps_poi = getattr(poi, 'poi_data_DGPS', None)
    
    def clear_data(self):
        """Clear all loaded data."""
        self.gt_per_scan = {}
        self.detections_per_scan = {}
        self.filtered_results = FilteredResults()
        self.outside_results = FilteredResults()
        self.all_gt_x = np.array([])
        self.all_gt_y = np.array([])
        self.gt_source_scan_indices = set()
        self.common_scan_indices = None
 
    def set_common_scan_indices(self, scan_indices):
        """Restrict ROI filtering to scan_index values shared by both HDF files."""
        if scan_indices is None:
            self.common_scan_indices = None
        else:
            self.common_scan_indices = {int(scan_value) for scan_value in scan_indices}
   
    def has_valid_gt(self) -> bool:
        """Check if valid GT data exists (non-zero positions loaded)."""
        return len(self.gt_per_scan) > 0
    
    def load_gt_from_hdf(self, hdf_path: str) -> bool:
        """
        Load Ground Truth positions from DGPS HDF file.
        Reads: vcs_long_posn (x), vcs_lat_posn (y), and scan_index
        Ignores zeros - only keeps valid GT positions.
        Stores GT keyed by scan_index VALUE (not array index).
        """
        gt_stream = '02_Input_DC_Data/GroundTruth'
        x_signal = 'vcs_long_posn'
        y_signal = 'vcs_lat_posn'

        # Override from poi config if available
        if self.dgps_poi:
            gt_source = self.dgps_poi.get('gt_source', {})
            gt_stream = gt_source.get('stream', gt_stream)
            signals = gt_source.get('signals', {})
            x_signal = signals.get('x', {}).get('name', x_signal)
            y_signal = signals.get('y', {}).get('name', y_signal)

        try:
            with h5py.File(hdf_path, 'r') as f:
                if gt_stream not in f:
                    print(f"❌ GT stream not found: {gt_stream}")
                    return False
                gt_group = f[gt_stream]
                if x_signal not in gt_group or y_signal not in gt_group:
                    print(f"❌ GT signals not found")
                    return False

                x_data = np.array(gt_group[x_signal])
                y_data = np.array(gt_group[y_signal])
               
                # Load scan_indices to key GT by scan_index value
                scan_index_data = np.array(f['01_Scan_Index/scan_index'][()])
                self.gt_source_scan_indices = {int(scan_value) for scan_value in scan_index_data.tolist()}
 
                # Do NOT flatten!
                self.all_gt_x = x_data
                self.all_gt_y = y_data
                self.gt_per_scan = {}
                valid_count = 0
                num_scans = x_data.shape[0]
                for array_idx in range(num_scans):
                    scan_index_val = int(scan_index_data[array_idx])
                    x_val = np.array(x_data[array_idx]).squeeze()
                    y_val = np.array(y_data[array_idx]).squeeze()
                    # Both x_val and y_val are 1D arrays (e.g., length 64)
                    valid_gt = []
                    for i in range(x_val.shape[0]):
                        x = x_val[i]
                        y = y_val[i]
                        if x != 0.0 or y != 0.0:
                            valid_gt.append((float(x), float(y)))
                    if valid_gt:
                        # Key by scan_index VALUE, not array index
                        self.gt_per_scan[scan_index_val] = valid_gt
                        valid_count += len(valid_gt)
        
            return True

        except Exception as e:
            print(f"❌ Error loading GT: {e}")
            import traceback
            traceback.print_exc()
            return False  
    
    def create_filtered_hdf_copy(self, src_hdf: str, dst_hdf: str, gt_hdf: Optional[str] = None, detection_stream: Optional[str] = None) -> bool:
        """
        Make a temporary copy of `src_hdf` at `dst_hdf` and zero out detections that lie outside the DGPS ROI.

        Args:
            src_hdf: Source HDF5 file path (will be copied).
            dst_hdf: Destination HDF5 file path (copy where modifications are written).
            gt_hdf: HDF file to load ground truth from (if None, uses src_hdf).
            detection_stream: Optional explicit HDF group path for detections.
        Returns:
            True on success, False otherwise.
        """
        if gt_hdf is None:
            gt_hdf = src_hdf

        # Ensure GT is loaded from gt_hdf
        if not self.load_gt_from_hdf(gt_hdf):
            print(f"❌ Unable to load GT from {gt_hdf} for HDF filtering")
            return False

        try:
            # Copy source to destination
            shutil.copyfile(src_hdf, dst_hdf)
        except Exception as e:
            print(f"❌ Failed to copy HDF file: {e}")
            return False

        # Now open destination and zero out detections outside ROI
        try:
            with h5py.File(dst_hdf, 'r+') as f:
                det_stream = detection_stream or '02_Input_DC_Data/Detections/Detection_Info'
                if det_stream not in f:
                    print(f"⚠️ Detection stream {det_stream} not present in {dst_hdf}; skipping modification")
                    return True

                det_group = f[det_stream]

                # Identify keys
                range_key = 'range' if 'range' in det_group else next((k for k in det_group.keys() if 'range' in k), 'range')
                az_key = 'azimuth' if 'azimuth' in det_group else next((k for k in det_group.keys() if 'azi' in k), 'azimuth')
                elev_key = 'elevation' if 'elevation' in det_group else next((k for k in det_group.keys() if 'elev' in k), 'elevation')
                rr_key = 'range_rate' if 'range_rate' in det_group else None

                range_data = np.array(det_group[range_key])
                azimuth_data = np.array(det_group[az_key])
                elevation_data = np.array(det_group[elev_key]) if elev_key in det_group else None
                range_rate_data = np.array(det_group[rr_key]) if rr_key and rr_key in det_group else None
                # Optional additional signals (may not exist in all HDFs)
                snr_data = np.array(det_group['snr']) if 'snr' in det_group else None
                amplitude_data = np.array(det_group['amplitude']) if 'amplitude' in det_group else None
                az_conf_data = np.array(det_group['azimuth_confidence']) if 'azimuth_confidence' in det_group else None

                # Mounting info (to compute cartesian coords)
                mounting_stream = '02_Input_DC_Data/MountingPosition'
                boresight_key = 'boresight_angle'
                polarity_key = 'azimuth_polarity'
                if mounting_stream not in f:
                    print(f"❌ MountingPosition not found in {dst_hdf}")
                    return False
                mounting_group = f[mounting_stream]
                boresight_angles = np.array(mounting_group[boresight_key])
                polarities = np.array(mounting_group[polarity_key])
                # Try to read sensor mounting positions (vcs long / lat). Names vary, try broad matches.
                def _find_key(group, substrs):
                    for k in group.keys():
                        kl = k.lower()
                        for s in substrs:
                            if s in kl:
                                return k
                    return None

                long_key = _find_key(mounting_group, ['vcs_long', 'vcs_lon', 'long', 'lon', 'longitude'])
                lat_key = _find_key(mounting_group, ['vcs_lat', 'lat', 'latitude'])
                mount_long = np.array(mounting_group[long_key]) if (long_key and long_key in mounting_group) else None
                mount_lat = np.array(mounting_group[lat_key]) if (lat_key and lat_key in mounting_group) else None

                def _get_mount_val(arr, idx):
                    """Return mounting value for sensor idx. Flatten arrays and fallback to single value.
                    Returns 0.0 on any error.
                    """
                    try:
                        if arr is None:
                            return 0.0
                        a = np.array(arr)
                        flat = a.squeeze().flatten()
                        if flat.size == 0:
                            return 0.0
                        if idx < 0:
                            return float(flat[0])
                        if idx < flat.size:
                            return float(flat[idx])
                        # Fallback: return first element
                        return float(flat[0])
                    except Exception:
                        return 0.0

                # Prepare mask with same shape as range_data
                mask = np.zeros_like(range_data, dtype=bool)
               
                # Load scan_indices from the detection HDF file to map array indices to scan_index values
                try:
                    scan_indices_data = np.array(f['01_Scan_Index/scan_index'][()])
                except Exception as e:
                    print(f"⚠️ Could not load scan_indices: {e}; will use array index as fallback")
                    scan_indices_data = None
 
                # Helper to safely extract scalar
                def _get(v):
                    import numpy as _np
                    if isinstance(v, _np.ndarray):
                        v = v.squeeze()
                        if v.size == 1:
                            return float(v.item())
                        # else fall through
                    return float(v)

                # Iterate shapes similarly to load_detections_from_hdf
                if range_data.ndim == 1:
                    # 1D: single scan, single sensor vector
                    num_scans = 1
                    num_sensors = 1
                    num_dets = range_data.shape[0]
                    for array_idx in range(num_scans):
                        if scan_indices_data is not None and array_idx < len(scan_indices_data):
                            scan_index_val = int(scan_indices_data[array_idx])
                        else:
                            scan_index_val = array_idx
                        if self.common_scan_indices is not None and scan_index_val not in self.common_scan_indices:
                            continue
                        if scan_index_val not in self.gt_source_scan_indices:
                            continue
                        if scan_index_val not in self.gt_per_scan:
                            # No GT at this scan: keep all detections
                            mask[:] = True
                            continue
                        gt_list = self.gt_per_scan[scan_index_val]
                        for det_idx in range(num_dets):
                            det_range = _get(range_data[det_idx])
                            if det_range == 0.0:
                                continue
                            det_az = _get(azimuth_data[det_idx])
                            det_el = _get(elevation_data[det_idx]) if elevation_data is not None else 0.0
                            polarity = _get_mount_val(polarities, sensor_idx_for_det) if polarities is not None else 1.0
                            RangeXY = math.cos(polarity * det_el) * det_range
                            x = math.cos(polarity * det_az) * RangeXY
                            y = math.sin(polarity * det_az) * RangeXY
                            # Determine sensor index for 1D data: if mounting arrays
                            # length matches number of detections, map det->sensor.
                            if mount_long is not None and hasattr(mount_long, 'size') and mount_long.size == num_dets:
                                sensor_idx_for_det = det_idx
                            else:
                                sensor_idx_for_det = 0
                            mount_long_val = _get_mount_val(mount_long, sensor_idx_for_det)
                            mount_lat_val = _get_mount_val(mount_lat, sensor_idx_for_det)
                            x += mount_long_val
                            y += mount_lat_val
                            min_d = float('inf')
                            for gt_x, gt_y in gt_list:
                                d = math.hypot(x - gt_x, y - gt_y)
                                if d < min_d:
                                    min_d = d
                            mask[det_idx] = (min_d <= self.config.roi.radius)

                elif range_data.ndim == 2:
                    # 2D: [num_scans, num_sensors]
                    num_scans, num_sensors = range_data.shape
                    for array_idx in range(num_scans):
                        if scan_indices_data is not None and array_idx < len(scan_indices_data):
                            scan_index_val = int(scan_indices_data[array_idx])
                        else:
                            scan_index_val = array_idx
                        if self.common_scan_indices is not None and scan_index_val not in self.common_scan_indices:
                            continue
                        if scan_index_val not in self.gt_source_scan_indices:
                            continue
                        if scan_index_val not in self.gt_per_scan:
                            # No GT at this scan: keep all detections
                            mask[array_idx, :] = True
                            continue
                        gt_list = self.gt_per_scan[scan_index_val]
                        for sensor_idx in range(num_sensors):
                            det_range = _get(range_data[array_idx, sensor_idx])
                            if det_range == 0.0:
                                continue
                            det_az = _get(azimuth_data[array_idx, sensor_idx])
                            det_el = _get(elevation_data[array_idx, sensor_idx]) if elevation_data is not None else 0.0
                            polarity = _get_mount_val(polarities, sensor_idx) if polarities is not None else 1.0
                            RangeXY = math.cos(polarity * det_el) * det_range
                            x = math.cos(polarity * det_az) * RangeXY
                            y = math.sin(polarity * det_az) * RangeXY
                            # add sensor mounting position for this sensor
                            mount_long_val = _get_mount_val(mount_long, sensor_idx)
                            mount_lat_val = _get_mount_val(mount_lat, sensor_idx)
                            x += mount_long_val
                            y += mount_lat_val
                            min_d = float('inf')
                            for gt_x, gt_y in gt_list:
                                d = math.hypot(x - gt_x, y - gt_y)
                                if d < min_d:
                                    min_d = d
                            mask[array_idx, sensor_idx] = (min_d <= self.config.roi.radius)
 
                elif range_data.ndim == 3:
                    # 3D: [num_scans, num_dets_per_sensor, num_sensors]
                    num_scans, num_dets_per_sensor, num_sensors = range_data.shape
                    # Flatten boresight if needed
                    if isinstance(boresight_angles, np.ndarray) and boresight_angles.ndim > 1:
                        boresight_angles_flat = boresight_angles.flatten()
                    else:
                        boresight_angles_flat = boresight_angles
 
                    for array_idx in range(num_scans):
                        # Get scan_index value for this array row
                        if scan_indices_data is not None and array_idx < len(scan_indices_data):
                            scan_index_val = int(scan_indices_data[array_idx])
                        else:
                            scan_index_val = array_idx  # Fallback
 
                        if self.common_scan_indices is not None and scan_index_val not in self.common_scan_indices:
                            continue
                        if scan_index_val not in self.gt_source_scan_indices:
                            continue
                        if scan_index_val not in self.gt_per_scan:
                            # No GT at this scan: keep all detections
                            mask[array_idx, :, :] = True
                            continue
                        gt_list = self.gt_per_scan[scan_index_val]
                        for sensor_idx in range(num_sensors):
                            for det_idx in range(num_dets_per_sensor):
                                det_range = _get(range_data[array_idx, det_idx, sensor_idx])
                                if det_range == 0.0:
                                    continue
                                det_az = _get(azimuth_data[array_idx, det_idx, sensor_idx])
                                det_el = _get(elevation_data[array_idx, det_idx, sensor_idx]) if elevation_data is not None else 0.0
                                polarity = _get_mount_val(polarities, sensor_idx) if polarities is not None else 1.0
                                RangeXY = math.cos(polarity * det_el) * det_range
                                x = math.cos(polarity * det_az) * RangeXY
                                y = math.sin(polarity * det_az) * RangeXY

                                try:
                                    if isinstance(boresight_angles, np.ndarray) and boresight_angles.ndim > 1:
                                        b_flat = boresight_angles.flatten()
                                    else:
                                        b_flat = boresight_angles
                                    boresight_angle = float(np.array(b_flat)[sensor_idx])
                                except Exception:
                                    boresight_angle = 0.0
                                RotationMatrixMP = self.GetRotationMatrix2D(boresight_angle/57.2958)
                                det_position = np.matrix([[x], [y]])
                                vcs_pos = RotationMatrixMP * det_position
                                # Add sensor mounting position (vcs long/lat) to obtain final vehicle coords
                                mount_long_val = _get_mount_val(mount_long, sensor_idx)
                                mount_lat_val = _get_mount_val(mount_lat, sensor_idx)
                                sensor_mp_position = np.matrix([[mount_long_val], [mount_lat_val]])
                                vcs_pos = vcs_pos + sensor_mp_position
                                x = float(vcs_pos[0, 0])
                                y = float(vcs_pos[1, 0])
                                min_d = float('inf')
                                for gt_x, gt_y in gt_list:
                                    d = math.hypot(x - gt_x, y - gt_y)
                                    if d < min_d:
                                        min_d = d
                                # range_data is organized as [scan, det, sensor]
                                mask[array_idx, det_idx, sensor_idx] = (min_d <= self.config.roi.radius)
 
                else:
                    print(f"❌ Unsupported detection array shape: {range_data.shape}")
                    return False

                # Apply mask: zero-out entries outside ROI for all relevant keys
                def apply_mask_and_write(key, data_arr):
                    if data_arr is None or key not in det_group:
                        return
                    try:
                        masked = np.where(mask, data_arr, 0.0)
                        det_group[key][...] = masked
                    except Exception:
                        # If shapes mismatch, attempt best-effort elementwise write
                        try:
                            det_group[key][...] = data_arr
                        except Exception:
                            pass

                apply_mask_and_write(range_key, range_data)
                apply_mask_and_write(rr_key, range_rate_data)
                apply_mask_and_write(az_key, azimuth_data)
                apply_mask_and_write(elev_key, elevation_data)
                # Extra optional signals
                apply_mask_and_write('snr', snr_data)
                apply_mask_and_write('amplitude', amplitude_data)
                apply_mask_and_write('azimuth_confidence', az_conf_data)

                # Detection_Header/Count is a separate dataset (not masked above);
                header_stream = '02_Input_DC_Data/Detections/Detection_Header'
                if header_stream in f and 'Count' in f[header_stream]:
                    if mask.ndim == 3:
                        # [scan, det, sensor] -> per-scan, per-sensor count
                        recomputed_count = np.sum(mask, axis=1)
                    elif mask.ndim == 2:
                        recomputed_count = np.sum(mask, axis=1, keepdims=True)
                    else:
                        recomputed_count = None
                    if recomputed_count is not None:
                        try:
                            f[header_stream]['Count'][...] = recomputed_count
                        except Exception as e:
                            print(f" Could not update Detection_Header/Count: {e}")

            return True
        except Exception as e:
            print(f"❌ Error while modifying HDF {dst_hdf}: {e}")
            import traceback
            traceback.print_exc()
            return False




