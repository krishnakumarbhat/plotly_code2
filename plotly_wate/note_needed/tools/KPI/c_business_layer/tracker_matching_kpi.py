#################################
# Tracker Matching KPI HDF Implementation
#################################
import sys
import os
import re
import numpy as np
import pandas as pd
import plotly.graph_objs as go
import plotly.subplots as sp
import plotly.io as pio
import json
import logging
from typing import Dict, List, Tuple, Any

from KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
from KPI.b_data_storage.kpi_config_storage import KPI_VALIDATION_RULES, KPI_TRACKER_CONFIG

logger = logging.getLogger(__name__)

class TrackerMappingKPIHDF:
    """Tracker KPI analysis using HDF data from self.input_data and self.output_data"""
    
    def __init__(self, input_data: KPI_DataModelStorage, output_data: KPI_DataModelStorage, 
                 sensor_id: str, stream_name: str):
        self.input_data = input_data
        self.sensor_id = sensor_id
        self.stream_name = stream_name
        
        # Get thresholds from config
        self.thresholds = KPI_VALIDATION_RULES["tracker_thresholds"]
        self.tracker_config = KPI_TRACKER_CONFIG
        
        # Initialize result variables
        self.html_content = ""
        self.kpi_results = {}
        self.matched_tracks_df = None
        self.summary_stats = {}
        
    def extract_tracker_data_from_hdf(self, data_model: KPI_DataModelStorage):
        """Extract tracker data from HDF KPI_DataModelStorage"""
        try:
            tracker_data = {}
            
            # Get tracker field mappings from config
            tracker_fields = self.tracker_config["fields"]
            
            # Extract each tracker field
            for field_key, field_config in tracker_fields.items():
                attr_name = field_config["name"]
                if hasattr(data_model, attr_name) and getattr(data_model, attr_name) is not None:
                    field_data = getattr(data_model, attr_name)
                    if isinstance(field_data, (list, np.ndarray)):
                        tracker_data[field_key] = np.array(field_data)
                    else:
                        tracker_data[field_key] = field_data
            
            return tracker_data
            
        except Exception as e:
            logger.error(f"Error extracting tracker data: {e}")
            return {}
            
    def convert_to_dataframes(self, input_tracker_data: Dict, output_tracker_data: Dict):
        """Convert tracker data to pandas DataFrames for processing"""
        try:
            # Create input/vehicle DataFrame
            input_df_data = {}
            for field_key, data in input_tracker_data.items():
                if isinstance(data, np.ndarray) and len(data) > 0:
                    input_df_data[field_key] = data
            
            input_df = pd.DataFrame(input_df_data) if input_df_data else pd.DataFrame()
            
            # Create output/simulation DataFrame
            output_df_data = {}
            for field_key, data in output_tracker_data.items():
                if isinstance(data, np.ndarray) and len(data) > 0:
                    output_df_data[field_key] = data
            
            output_df = pd.DataFrame(output_df_data) if output_df_data else pd.DataFrame()
            
            return input_df, output_df
            
        except Exception as e:
            logger.error(f"Error converting tracker data to DataFrames: {e}")
            return pd.DataFrame(), pd.DataFrame()
            
    def process_tracker_matching(self):
        """Main processing function for tracker matching KPIs"""
        try:
            # Extract data from HDF
            input_tracker_data = self.extract_tracker_data_from_hdf(self.input_data)
            output_tracker_data = self.extract_tracker_data_from_hdf(self.output_data)
            
            if not input_tracker_data or not output_tracker_data:
                logger.warning("Insufficient tracker data for KPI analysis")
                return False
            
            # Convert to DataFrames
            input_df, output_df = self.convert_to_dataframes(input_tracker_data, output_tracker_data)
            
            if input_df.empty or output_df.empty:
                logger.warning("Empty tracker DataFrames")
                return False
                
            # Perform tracker matching
            matching_results = self.match_tracker_data(input_df, output_df)
            
            if not matching_results:
                logger.warning("No matching tracker results")
                return False
                
            # Calculate KPIs
            self.calculate_tracker_kpis(matching_results)
            
            # Generate HTML report
            self.generate_html_report()
            
            return True
            
        except Exception as e:
            logger.error(f"Error in tracker matching process: {e}")
            return False
            
    def match_tracker_data(self, input_df: pd.DataFrame, output_df: pd.DataFrame):
        """Match tracker data between input and output based on position and time"""
        try:
            # Required fields for matching
            required_fields = ['track_id', 'x_position', 'y_position', 'z_position', 'time_stamp']
            
            # Check if required fields exist
            missing_input = [field for field in required_fields if field not in input_df.columns]
            missing_output = [field for field in required_fields if field not in output_df.columns]
            
            if missing_input or missing_output:
                logger.warning(f"Missing required fields - Input: {missing_input}, Output: {missing_output}")
                return {}
            
            # Get matching thresholds
            position_threshold = self.thresholds["position_threshold_m"]
            time_threshold = self.thresholds["time_threshold_s"]
            
            matched_tracks = []
            
            # For each track in input data, find matching track in output
            for idx_input, row_input in input_df.iterrows():
                input_pos = np.array([row_input['x_position'], row_input['y_position'], row_input['z_position']])
                input_time = row_input['time_stamp']
                
                best_match = None
                min_distance = float('inf')
                
                for idx_output, row_output in output_df.iterrows():
                    output_pos = np.array([row_output['x_position'], row_output['y_position'], row_output['z_position']])
                    output_time = row_output['time_stamp']
                    
                    # Calculate spatial distance
                    spatial_distance = np.linalg.norm(input_pos - output_pos)
                    
                    # Calculate time difference
                    time_diff = abs(input_time - output_time)
                    
                    # Check if within thresholds
                    if spatial_distance <= position_threshold and time_diff <= time_threshold:
                        if spatial_distance < min_distance:
                            min_distance = spatial_distance
                            best_match = {
                                'input_track_id': row_input['track_id'],
                                'output_track_id': row_output['track_id'],
                                'input_pos': input_pos,
                                'output_pos': output_pos,
                                'spatial_distance': spatial_distance,
                                'time_diff': time_diff,
                                'input_time': input_time,
                                'output_time': output_time
                            }
                
                if best_match:
                    matched_tracks.append(best_match)
            
            # Convert to DataFrame
            if matched_tracks:
                self.matched_tracks_df = pd.DataFrame(matched_tracks)
                return {'matched_tracks': matched_tracks, 'total_matches': len(matched_tracks)}
            else:
                return {}
                
        except Exception as e:
            logger.error(f"Error matching tracker data: {e}")
            return {}
            
    def calculate_tracker_kpis(self, matching_results: Dict):
        """Calculate tracker KPIs based on matching results"""
        try:
            matched_tracks = matching_results.get('matched_tracks', [])
            total_matches = matching_results.get('total_matches', 0)
            
            if not matched_tracks:
                self.kpi_results = {'total_matches': 0, 'accuracy': 0.0}
                return
            
            # Calculate position errors
            position_errors = []
            velocity_errors = []
            
            for match in matched_tracks:
                pos_error = np.linalg.norm(match['input_pos'] - match['output_pos'])
                position_errors.append(pos_error)
            
            # Calculate statistics
            position_errors = np.array(position_errors)
            
            # Count matches within accuracy thresholds
            accuracy_threshold = self.thresholds["accuracy_threshold_m"]
            accurate_matches = np.sum(position_errors <= accuracy_threshold)
            accuracy_percentage = (accurate_matches / total_matches * 100) if total_matches > 0 else 0.0
            
            # Summary statistics
            self.summary_stats = {
                'total_input_tracks': len(self.input_data.track_id) if hasattr(self.input_data, 'track_id') else 0,
                'total_output_tracks': len(self.output_data.track_id) if hasattr(self.output_data, 'track_id') else 0,
                'total_matches': total_matches,
                'accurate_matches': int(accurate_matches),
                'position_error_mean': float(np.mean(position_errors)),
                'position_error_std': float(np.std(position_errors)),
                'position_error_max': float(np.max(position_errors)),
                'position_error_min': float(np.min(position_errors))
            }
            
            # Store KPI results
            self.kpi_results = {
                'accuracy': round(accuracy_percentage, 2),
                'numerator': int(accurate_matches),
                'denominator': total_matches,
                'summary_stats': self.summary_stats,
                'thresholds': {
                    'position_threshold': self.thresholds["position_threshold_m"],
                    'time_threshold': self.thresholds["time_threshold_s"],
                    'accuracy_threshold': accuracy_threshold
                }
            }
            
        except Exception as e:
            logger.error(f"Error calculating tracker KPIs: {e}")
            self.kpi_results = {}
            
    def generate_plots(self):
        """Generate interactive plots for tracker data"""
        try:
            if self.matched_tracks_df is None or self.matched_tracks_df.empty:
                return "", ""
            
            # Position comparison plot
            fig_pos = sp.make_subplots(rows=2, cols=2, 
                                      subplot_titles=('X Position Comparison', 'Y Position Comparison',
                                                    'Z Position Comparison', 'Position Error Distribution'),
                                      specs=[[{"secondary_y": False}, {"secondary_y": False}],
                                            [{"secondary_y": False}, {"secondary_y": False}]])
            
            # X position
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index, 
                                       y=[pos[0] for pos in self.matched_tracks_df['input_pos']],
                                       mode='lines+markers', name='Input X', line=dict(color='blue')), 
                            row=1, col=1)
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index,
                                       y=[pos[0] for pos in self.matched_tracks_df['output_pos']],
                                       mode='lines+markers', name='Output X', line=dict(color='red')),
                            row=1, col=1)
            
            # Y position  
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index,
                                       y=[pos[1] for pos in self.matched_tracks_df['input_pos']],
                                       mode='lines+markers', name='Input Y', line=dict(color='blue')),
                            row=1, col=2)
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index,
                                       y=[pos[1] for pos in self.matched_tracks_df['output_pos']],
                                       mode='lines+markers', name='Output Y', line=dict(color='red')),
                            row=1, col=2)
            
            # Z position
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index,
                                       y=[pos[2] for pos in self.matched_tracks_df['input_pos']],
                                       mode='lines+markers', name='Input Z', line=dict(color='blue')),
                            row=2, col=1)
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index,
                                       y=[pos[2] for pos in self.matched_tracks_df['output_pos']],
                                       mode='lines+markers', name='Output Z', line=dict(color='red')),
                            row=2, col=1)
            
            # Position error histogram
            fig_pos.add_trace(go.Histogram(x=self.matched_tracks_df['spatial_distance'],
                                         name='Position Error', nbinsx=20),
                            row=2, col=2)
            
            fig_pos.update_layout(height=800, width=1200, title_text="Tracker Position Analysis")
            pos_plot_html = pio.to_html(fig_pos, full_html=False, include_plotlyjs='cdn')
            
            # 3D scatter plot of matched tracks
            fig_3d = go.Figure()
            
            input_x = [pos[0] for pos in self.matched_tracks_df['input_pos']]
            input_y = [pos[1] for pos in self.matched_tracks_df['input_pos']]
            input_z = [pos[2] for pos in self.matched_tracks_df['input_pos']]
            
            output_x = [pos[0] for pos in self.matched_tracks_df['output_pos']]
            output_y = [pos[1] for pos in self.matched_tracks_df['output_pos']]
            output_z = [pos[2] for pos in self.matched_tracks_df['output_pos']]
            
            fig_3d.add_trace(go.Scatter3d(x=input_x, y=input_y, z=input_z,
                                        mode='markers',
                                        name='Input Tracks',
                                        marker=dict(size=5, color='blue')))
            
            fig_3d.add_trace(go.Scatter3d(x=output_x, y=output_y, z=output_z,
                                        mode='markers',
                                        name='Output Tracks',
                                        marker=dict(size=5, color='red')))
            
            fig_3d.update_layout(title='3D Track Positions', scene=dict(
                xaxis_title='X Position (m)',
                yaxis_title='Y Position (m)',
                zaxis_title='Z Position (m)'
            ))
            
            scatter_3d_html = pio.to_html(fig_3d, full_html=False, include_plotlyjs='cdn')
            
            return pos_plot_html, scatter_3d_html
            
        except Exception as e:
            logger.error(f"Error generating tracker plots: {e}")
            return "", ""
            
    def generate_html_report(self):
        """Generate HTML report for tracker KPIs"""
        try:
            # Generate plots
            pos_plot_html, scatter_3d_html = self.generate_plots()
            
            summary = self.summary_stats
            kpi = self.kpi_results
            
            self.html_content = f"""
            <div class="kpi-section">
                <h3>Tracker Matching KPI Results - {self.sensor_id}</h3>
                
                <div class="kpi-summary">
                    <p><strong>Tracker Accuracy:</strong> ({kpi.get('numerator', 0)}/{kpi.get('denominator', 0)}) → <strong>{kpi.get('accuracy', 0):.2f}%</strong></p>
                    <p><strong>Total Matched Tracks:</strong> {summary.get('total_matches', 0)}</p>
                    <p><strong>Position Error Mean:</strong> {summary.get('position_error_mean', 0):.3f} m</p>
                    <p><strong>Position Error Std:</strong> {summary.get('position_error_std', 0):.3f} m</p>
                </div>
                
                <details>
                    <summary><strong>Detailed Results</strong></summary>
                    
                    <h4>Track Statistics</h4>
                    <ul>
                        <li>Total Input Tracks: {summary.get('total_input_tracks', 0)}</li>
                        <li>Total Output Tracks: {summary.get('total_output_tracks', 0)}</li>
                        <li>Successfully Matched: {summary.get('total_matches', 0)}</li>
                        <li>Accurate Matches: {summary.get('accurate_matches', 0)}</li>
                    </ul>
                    
                    <h4>Position Error Statistics</h4>
                    <ul>
                        <li>Mean Error: {summary.get('position_error_mean', 0):.3f} m</li>
                        <li>Std Deviation: {summary.get('position_error_std', 0):.3f} m</li>
                        <li>Maximum Error: {summary.get('position_error_max', 0):.3f} m</li>
                        <li>Minimum Error: {summary.get('position_error_min', 0):.3f} m</li>
                    </ul>
                    
                    <h4>Threshold Configuration</h4>
                    <ul>
                        <li>Position matching threshold: {kpi.get('thresholds', {}).get('position_threshold', 0):.1f} m</li>
                        <li>Time matching threshold: {kpi.get('thresholds', {}).get('time_threshold', 0):.1f} s</li>
                        <li>Accuracy threshold: {kpi.get('thresholds', {}).get('accuracy_threshold', 0):.1f} m</li>
                    </ul>
                    
                    <h4>Analysis Description</h4>
                    <p>This KPI measures tracker accuracy by matching tracks between input (vehicle) and output (simulation) 
                    data based on position and time proximity. Tracks are considered accurate if their position error 
                    is within the specified threshold.</p>
                    
                    <h4>Interactive Plots</h4>
                    <details>
                        <summary><strong>Plot A: Position Comparison and Error Distribution</strong></summary>
                        {pos_plot_html}
                    </details>
                    
                    <details>
                        <summary><strong>Plot B: 3D Track Position Visualization</strong></summary>
                        {scatter_3d_html}
                    </details>
                </details>
            </div>
            <hr>
            """
            
        except Exception as e:
            logger.error(f"Error generating HTML report: {e}")
            self.html_content = f"<p>Error generating tracker KPI report: {e}</p>"

    def get_results(self):
        """Return KPI results and HTML content"""
        return {
            'kpi_results': self.kpi_results,
            'html_content': self.html_content,
            'success': bool(self.kpi_results)
        }

# Main processing function to be called by KPI factory
def process_tracker_kpi(input_data: KPI_DataModelStorage, output_data: KPI_DataModelStorage, 
                       sensor_id: str, stream_name: str) -> dict:
    """
    Main function to process tracker KPIs from HDF data
    
    Args:
        input_data: KPI_DataModelStorage with input/vehicle data
        output_data: KPI_DataModelStorage with output/simulation data  
        sensor_id: Sensor identifier
        stream_name: Stream name being processed
        
    Returns:
        dict: KPI results and HTML content
    """
    try:
        processor = TrackerMappingKPIHDF(input_data, output_data, sensor_id, stream_name)
        success = processor.process_tracker_matching()
        
        if success:
            return processor.get_results()
        else:
            return {
                'kpi_results': {},
                'html_content': f"<p>Failed to process tracker KPIs for {sensor_id}</p>",
                'success': False
            }
            
    except Exception as e:
        logger.error(f"Error in process_tracker_kpi: {e}")
        return {
            'kpi_results': {},
            'html_content': f"<p>Error processing tracker KPIs: {e}</p>",
            'success': False
        }