#################################
# Alignment Matching KPI HDF Implementation
#################################
import numpy as np
import plotly.graph_objects as go
import logging

from KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
from KPI.b_data_storage.kpi_config_storage import KPI_VALIDATION_RULES
from KPI.d_presentation_layer.alignment_report import alignment_html

logger = logging.getLogger(__name__)

class AlignmentMappingKPIHDF:
    """Alignment KPI analysis using HDF data from self.input_data and self.output_data"""
    
    def __init__(self, data: KPI_DataModelStorage, 
                 sensor_id: str):
        self.data = data
        self.sensor_id = sensor_id
        
        # Get thresholds from config
        self.thresholds = KPI_VALIDATION_RULES["alignment_thresholds"]
        self.az_misalign_threshold = self.thresholds["az_misalign_threshold"]
        self.el_misalign_threshold = self.thresholds["el_misalign_threshold"]
        
        # Initialize result variables
        self.html_content = ""
        self.kpi_results = {}
        self.scan_index_list = []
        self.az_misalign_lists = {}
        self.el_misalign_lists = {}

    def process_alignment_matching(self):
        """Main processing function for alignment matching KPIs"""
        try:
            src = None
            # Some datasets don't contain alignment stream; skip cleanly instead of raising KeyError.
            if hasattr(self.data, 'get'):
                src = self.data.get('DYNAMIC_ALIGNMENT_STREAM')
            if src is None:
                try:
                    src = self.data['DYNAMIC_ALIGNMENT_STREAM']
                except Exception:
                    src = None
            if not src:
                logger.warning(f'Skipping alignment matching KPI for {self.sensor_id} - DYNAMIC_ALIGNMENT_STREAM not found')
                logger.info(f'Available streams in data: {list(self.data.keys()) if hasattr(self.data, "keys") else "N/A"}')
                return False
            
            # Log what we found
            logger.info(f'Processing alignment KPI for {self.sensor_id} - DYNAMIC_ALIGNMENT_STREAM found')
            
            required_signals = [
                "vacs_boresight_az_nominal_V",
                "vacs_boresight_az_estimated",
                "vacs_boresight_az_kf_internal_V",
                "vacs_boresight_el_nominal",
                "vacs_boresight_el_estimated",
                "vacs_boresight_el_kf_internal"
            ]
            input_align_data = {}
            output_align_data = {}
            for signal in required_signals:
                veh_arr, veh_status = KPI_DataModelStorage.get_value(src['input'], signal)
                sim_arr, sim_status = KPI_DataModelStorage.get_value(src['output'], signal)
                if veh_status == "success" and hasattr(veh_arr, "size") and veh_arr.size > 0:
                    vec_in = veh_arr[:, 0] if isinstance(veh_arr, np.ndarray) and veh_arr.ndim == 2 and veh_arr.shape[1] >= 1 else np.asarray(veh_arr).ravel()
                    input_align_data[signal] = vec_in
                else:
                    logger.debug(f'Signal {signal} input: status={veh_status}')
                if sim_status == "success" and hasattr(sim_arr, "size") and sim_arr.size > 0:
                    vec_out = sim_arr[:, 0] if isinstance(sim_arr, np.ndarray) and sim_arr.ndim == 2 and sim_arr.shape[1] >= 1 else np.asarray(sim_arr).ravel()
                    output_align_data[signal] = vec_out
                else:
                    logger.debug(f'Signal {signal} output: status={sim_status}')
            missing_input = [sig for sig in required_signals if sig not in input_align_data]
            missing_output = [sig for sig in required_signals if sig not in output_align_data]
            if missing_input or missing_output:
                logger.warning(f"Missing alignment signals for {self.sensor_id} - Input: {missing_input}, Output: {missing_output}")
                return False
            data_length = len(input_align_data[required_signals[0]])
            self.scan_index_list = list(range(1, data_length + 1))
            alignment_results = self.calculate_alignment_kpis(input_align_data, output_align_data)
            self.kpi_results = alignment_results
            self.generate_html_report()
            return True
            
        except Exception as e:
            logger.error(f"Error in alignment matching process: {e}")
            return False
            
    def calculate_alignment_kpis(self, input_data, output_data):
        """Calculate alignment KPIs and misalignment differences"""
        # try:
            # Extract signal arrays
        signals = {
            'az_nominal_real': input_data['vacs_boresight_az_nominal_V'],
            'az_estimated_real': input_data['vacs_boresight_az_estimated'],
            'az_kf_internal_real': input_data['vacs_boresight_az_kf_internal_V'],
            'el_nominal_real': input_data['vacs_boresight_el_nominal'],
            'el_estimated_real': input_data['vacs_boresight_el_estimated'],
            'el_kf_internal_real': input_data['vacs_boresight_el_kf_internal'],
            'az_nominal_sim': output_data['vacs_boresight_az_nominal_V'],
            'az_estimated_sim': output_data['vacs_boresight_az_estimated'],
            'az_kf_internal_sim': output_data['vacs_boresight_az_kf_internal_V'],
            'el_nominal_sim': output_data['vacs_boresight_el_nominal'],
            'el_estimated_sim': output_data['vacs_boresight_el_estimated'],
            'el_kf_internal_sim': output_data['vacs_boresight_el_kf_internal']
        }
        
        # Calculate misalignments in degrees (convert from radians)
        az_misalign_est_real = (signals['az_nominal_real'] - signals['az_estimated_real']) * (180 / np.pi)
        az_misalign_ref_real = (signals['az_nominal_real'] - signals['az_kf_internal_real']) * (180 / np.pi)
        az_misalign_est_sim = (signals['az_nominal_sim'] - signals['az_estimated_sim']) * (180 / np.pi)
        az_misalign_ref_sim = (signals['az_nominal_sim'] - signals['az_kf_internal_sim']) * (180 / np.pi)
        
        el_misalign_est_real = (signals['el_nominal_real'] - signals['el_estimated_real']) * (180 / np.pi)
        el_misalign_ref_real = (signals['el_nominal_real'] - signals['el_kf_internal_real']) * (180 / np.pi)
        el_misalign_est_sim = (signals['el_nominal_sim'] - signals['el_estimated_sim']) * (180 / np.pi)
        el_misalign_ref_sim = (signals['el_nominal_sim'] - signals['el_kf_internal_sim']) * (180 / np.pi)
        
        # Calculate differences between real and sim
        az_misalign_est_diff = az_misalign_est_real - az_misalign_est_sim
        el_misalign_est_diff = el_misalign_est_real - el_misalign_est_sim
        
        # Store lists for plotting
        self.az_misalign_lists = {
            'est_real': az_misalign_est_real.tolist(),
            'ref_real': az_misalign_ref_real.tolist(),
            'est_sim': az_misalign_est_sim.tolist(),
            'ref_sim': az_misalign_ref_sim.tolist(),
            'est_diff': az_misalign_est_diff.tolist()
        }
        
        self.el_misalign_lists = {
            'est_real': el_misalign_est_real.tolist(),
            'ref_real': el_misalign_ref_real.tolist(),
            'est_sim': el_misalign_est_sim.tolist(),
            'ref_sim': el_misalign_ref_sim.tolist(),
            'est_diff': el_misalign_est_diff.tolist()
        }
        
        # Count matches within thresholds
        num_scans_total = len(az_misalign_est_diff)
        num_scans_az_match = np.sum(np.abs(az_misalign_est_diff) < self.az_misalign_threshold)
        num_scans_el_match = np.sum(np.abs(el_misalign_est_diff) < self.el_misalign_threshold)
        
        # Calculate KPI percentages
        az_accuracy = round((num_scans_az_match / num_scans_total) * 100, 2) if num_scans_total > 0 else 0
        el_accuracy = round((num_scans_el_match / num_scans_total) * 100, 2) if num_scans_total > 0 else 0
        
        # Error statistics (degrees)
        def _stats(arr):
            if arr.size == 0:
                return {
                    'mae': None,
                    'rmse': None,
                    'max_abs': None,
                }
            abs_arr = np.abs(arr)
            mae = float(np.mean(abs_arr))
            rmse = float(np.sqrt(np.mean(arr**2)))
            max_abs = float(np.max(abs_arr))
            return {'mae': mae, 'rmse': rmse, 'max_abs': max_abs}
        az_stats = _stats(az_misalign_est_diff)
        el_stats = _stats(el_misalign_est_diff)
        
        # Return KPI results
        kpi_results = {
            'az_kpi': {
                'numerator': int(num_scans_az_match),
                'denominator': num_scans_total,
                'accuracy': az_accuracy
            },
            'el_kpi': {
                'numerator': int(num_scans_el_match),
                'denominator': num_scans_total,
                'accuracy': el_accuracy
            },
            'num_scans': num_scans_total,
            'thresholds': {
                'az_threshold': self.az_misalign_threshold,
                'el_threshold': self.el_misalign_threshold
            },
            'az_error_stats': az_stats,
            'el_error_stats': el_stats
        }
        
        return kpi_results
            
        # except Exception as e:
        #     logger.error(f"Error calculating alignment KPIs: {e}")
        #     return {}
            
    def generate_plots(self):
        """Generate interactive plots for alignment data"""
        try:
            # Create misalignment plot
            misalignment_fig = go.Figure()
            
            # Add traces for azimuth misalignment
            misalignment_fig.add_trace(go.Scatter(
                x=self.scan_index_list,
                y=self.az_misalign_lists['est_real'],
                mode='lines+markers',
                name='Veh Az Est',
                line=dict(color='#3498db', width=2),
                marker=dict(size=4)
            ))
            
            misalignment_fig.add_trace(go.Scatter(
                x=self.scan_index_list,
                y=self.az_misalign_lists['est_sim'],
                mode='lines+markers',
                name='Sim Az Est',
                line=dict(color='#e74c3c', width=2, dash='dash'),
                marker=dict(size=4)
            ))
            
            # Add traces for elevation misalignment (secondary y-axis)
            misalignment_fig.add_trace(go.Scatter(
                x=self.scan_index_list,
                y=self.el_misalign_lists['est_real'],
                mode='lines+markers',
                name='Veh El Est',
                line=dict(color='#2ecc71', width=2),
                marker=dict(size=4),
                yaxis='y2'
            ))
            
            misalignment_fig.add_trace(go.Scatter(
                x=self.scan_index_list,
                y=self.el_misalign_lists['est_sim'],
                mode='lines+markers',
                name='Sim El Est',
                line=dict(color='#f39c12', width=2, dash='dash'),
                marker=dict(size=4),
                yaxis='y2'
            ))
            
            # Update layout for misalignment plot
            misalignment_fig.update_layout(
                title='Misalignment vs Scan Index',
                xaxis=dict(title='Scan Index'),
                yaxis=dict(
                    title=dict(
                        text='Azimuth Misalignment (deg)',
                        font=dict(color='#3498db')
                    ),
                    tickfont=dict(color='#3498db')
                ),
                yaxis2=dict(
                    title=dict(
                        text='Elevation Misalignment (deg)',
                        font=dict(color='#2ecc71')
                    ),
                    tickfont=dict(color='#2ecc71'),
                    anchor='x',
                    overlaying='y',
                    side='right'
                ),
                legend=dict(
                    orientation='h',
                    yanchor='bottom',
                    y=1.02,
                    xanchor='right',
                    x=1
                ),
                height=500,
                margin=dict(l=50, r=50, t=80, b=50)
            )
            
            # Create difference plot
            difference_fig = go.Figure()
            
            # Add traces for azimuth difference
            difference_fig.add_trace(go.Scatter(
                x=self.scan_index_list,
                y=self.az_misalign_lists['est_diff'],
                mode='lines+markers',
                name='Azimuth Difference',
                line=dict(color='#9b59b6', width=2),
                marker=dict(size=4)
            ))
            
            # Add traces for elevation difference (secondary y-axis)
            difference_fig.add_trace(go.Scatter(
                x=self.scan_index_list,
                y=self.el_misalign_lists['est_diff'],
                mode='lines+markers',
                name='Elevation Difference',
                line=dict(color='#1abc9c', width=2, dash='dash'),
                marker=dict(size=4),
                yaxis='y2'
            ))
            
            # Add threshold lines
            difference_fig.add_hline(
                y=self.az_misalign_threshold,
                line_dash='dot',
                line_color='#9b59b6',
                opacity=0.5,
                annotation_text=f'Az Threshold: {self.az_misalign_threshold:.2f}°',
                annotation_position='top right'
            )
            
            difference_fig.add_hline(
                y=-self.az_misalign_threshold,
                line_dash='dot',
                line_color='#9b59b6',
                opacity=0.5
            )
            
            difference_fig.add_hline(
                y=self.el_misalign_threshold,
                line_dash='dot',
                line_color='#1abc9c',
                opacity=0.5,
                annotation_text=f'El Threshold: {self.el_misalign_threshold:.2f}°',
                annotation_position='bottom right',
                yref='y2'
            )
            
            difference_fig.add_hline(
                y=-self.el_misalign_threshold,
                line_dash='dot',
                line_color='#1abc9c',
                opacity=0.5,
                yref='y2'
            )
            
            # Update layout for difference plot
            difference_fig.update_layout(
                title='Misalignment Difference vs Scan Index',
                xaxis=dict(title='Scan Index'),
                yaxis=dict(
                    title=dict(
                        text='Azimuth Difference (deg)',
                        font=dict(color='#9b59b6')
                    ),
                    tickfont=dict(color='#9b59b6')
                ),
                yaxis2=dict(
                    title=dict(
                        text='Elevation Difference (deg)',
                        font=dict(color='#1abc9c')
                    ),
                    tickfont=dict(color='#1abc9c'),
                    anchor='x',
                    overlaying='y',
                    side='right'
                ),
                legend=dict(
                    orientation='h',
                    yanchor='bottom',
                    y=1.02,
                    xanchor='right',
                    x=1
                ),
                height=500,
                margin=dict(l=50, r=50, t=80, b=50),
                showlegend=True
            )
            
            return misalignment_fig, difference_fig
            
        except Exception as e:
            logger.error(f"Error generating plots: {e}")
            # Return empty figures on error
            return go.Figure(), go.Figure()
            
    def generate_html_report(self):
        """Generate HTML report for alignment KPIs using the template"""
        try:
            az_kpi = self.kpi_results.get('az_kpi', {})
            el_kpi = self.kpi_results.get('el_kpi', {})
            az_stats = self.kpi_results.get('az_error_stats', {})
            el_stats = self.kpi_results.get('el_error_stats', {})
            
            # Generate plots
            misalignment_fig, difference_fig = self.generate_plots()
            
            # Convert figures to JSON for the template
            misalignment_plot_json = misalignment_fig.to_json()
            difference_plot_json = difference_fig.to_json()
            
            # Format the HTML using the template
            self.html_content = alignment_html.format(
                sensor_id=self.sensor_id,
                az_numerator=az_kpi.get('numerator', 0),
                az_denominator=az_kpi.get('denominator', 1),
                az_accuracy=az_kpi.get('accuracy', 0),
                el_numerator=el_kpi.get('numerator', 0),
                el_denominator=el_kpi.get('denominator', 1),
                el_accuracy=el_kpi.get('accuracy', 0),
                total_scans=self.kpi_results.get('num_scans', 0),
                az_threshold=self.az_misalign_threshold,
                el_threshold=self.el_misalign_threshold,
                az_mae="N/A" if az_stats.get('mae') is None else f"{az_stats.get('mae'):.3f}",
                az_rmse="N/A" if az_stats.get('rmse') is None else f"{az_stats.get('rmse'):.3f}",
                az_max_abs="N/A" if az_stats.get('max_abs') is None else f"{az_stats.get('max_abs'):.3f}",
                el_mae="N/A" if el_stats.get('mae') is None else f"{el_stats.get('mae'):.3f}",
                el_rmse="N/A" if el_stats.get('rmse') is None else f"{el_stats.get('rmse'):.3f}",
                el_max_abs="N/A" if el_stats.get('max_abs') is None else f"{el_stats.get('max_abs'):.3f}",
                misalignment_plot=misalignment_plot_json,
                difference_plot=difference_plot_json
            )
            
        except Exception as e:
            logger.error(f"Error generating HTML report: {e}")
            self.html_content = f"<p>Error generating alignment KPI report: {e}</p>"

    def get_results(self):
        """Return KPI results and HTML content"""
        return {
            'kpi_results': self.kpi_results,
            'html_content': self.html_content,
            'success': bool(self.kpi_results)
        }

# Main processing function to be called by KPI factory
def process_alignment_kpi(data: KPI_DataModelStorage, 
                         sensor_id: str) -> dict:
    """
    Main function to process alignment KPIs from HDF data
    
    Args:
        input_data: KPI_DataModelStorage with input/vehicle data
        output_data: KPI_DataModelStorage with output/simulation data  
        sensor_id: Sensor identifier
        stream_name: Stream name being processed
        
    Returns:
        dict: KPI results and HTML content
    """
    # try:
    processor = AlignmentMappingKPIHDF(data, sensor_id)
    success = processor.process_alignment_matching()
    
    if success:
        return processor.get_results()
    else:
        return {
            'kpi_results': {},
            'html_content': f"<p>Failed to process alignment KPIs for {sensor_id}</p>",
            'success': False
        }
        
    # except Exception as e:
    #     logger.error(f"Error in process_alignment_kpi: {e}")
    #     return {
    #         'kpi_results': {},
    #         'html_content': f"<p>Error processing alignment KPIs: {e}</p>",
    #         'success': False
    #     }