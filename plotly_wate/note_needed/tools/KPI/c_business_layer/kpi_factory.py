import os
import tempfile
import logging
import json
import gc
import time
from typing import Optional, List, Dict, Any
from KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
from KPI.c_business_layer.alignment_matching_kpi import process_alignment_kpi
from KPI.c_business_layer.detection_matching_kpi import process_detection_kpi
from KPI.c_business_layer.tracker_matching_kpi import process_tracker_kpi
import concurrent.futures

class KpiDataModel:
    """
    Class for generating KPI plots from KPI_DataModelStorage.
    This class processes data from KPI_DataModelStorage to generate KPI visualizations.
    """
    
    def __init__(self, data: Dict[str, KPI_DataModelStorage], sensor: str):
        """
        Initialize the KPI data model with input and output data.
        
        Parameters:
            data: Dictionary containing 'input' and 'output' KPI_DataModelStorage instances
            sensor: Name of the sensor being processed
        """
        self.data = data
        self.sensor = sensor

        # Create temporary directory for plot files
        self.temp_dir = tempfile.mkdtemp(prefix="kpi_plots_temp_")
        
        # Initialize plots list
        self.plots = []
        
        # Initialize HTML content list
        self.html_sections = []
        # Per-KPI HTML payloads to be saved by caller
        self.per_kpi_sections: List[Dict[str, Any]] = []
        
        # Process KPIs based on stream type
        self._process_kpis()
    
    def _process_kpis(self):
        """
        Process KPIs in parallel using multiprocessing
        Runs alignment, tracker, and detection KPIs concurrently
        """
        import os
        
        # Determine which KPIs to process based on stream name
        tasks = [
            ("alignment", self._process_alignment_kpi),
            # ("tracker", self._process_tracker_kpi),
            ("detection", self._process_detection_kpi)
        ]
            
        if not tasks:
            logging.warning(f"No KPI processors available for stream: {self.stream_name}")
            self.plots = []
            return
            
        self.plots = []

        # Debug mode: disable ProcessPool so logs/prints are visible and debugging is easier.
        disable_mp = str(os.environ.get('KPI_DISABLE_MULTIPROC', '0')).lower() in ('1', 'true', 'yes', 'on')
        if disable_mp or len(tasks) == 1:
            for name, func in tasks:
                try:
                    result = func()
                    self.plots.extend(result)
                    if isinstance(result, list) and len(result) > 0:
                        for item in result:
                            if isinstance(item, dict) and 'html_content' in item:
                                self.html_sections.append(item['html_content'])
                                sensor_id = item.get('sensor_id') if isinstance(item, dict) else None
                                self.per_kpi_sections.append({
                                    'type': name,
                                    'sensor_id': sensor_id,
                                    'html_content': item['html_content']
                                })
                except Exception as e:
                    logging.error(f"Error processing {name} KPI: {e}")
            return

        # Default: Process tasks in parallel
        with concurrent.futures.ProcessPoolExecutor(max_workers=len(tasks)) as executor:
            futures = {executor.submit(func): name for name, func in tasks}
            for future in concurrent.futures.as_completed(futures):
                try:
                    result = future.result()
                    self.plots.extend(result)
                    if isinstance(result, list) and len(result) > 0:
                        for item in result:
                            if isinstance(item, dict) and 'html_content' in item:
                                self.html_sections.append(item['html_content'])
                                kpi_type = futures[future]
                                sensor_id = item.get('sensor_id') if isinstance(item, dict) else None
                                self.per_kpi_sections.append({
                                    'type': kpi_type,
                                    'sensor_id': sensor_id,
                                    'html_content': item['html_content']
                                })
                except Exception as e:
                    logging.error(f"Error processing {futures[future]} KPI: {e}")
        
    def _save_html_report(self, html_report_path: str):
        """Save collected HTML sections to the report file"""
        try:
            full_html = f"""
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>KPI Report - {self.sensor}</title>
                <style>
                    body {{ font-family: Arial, sans-serif; margin: 20px; }}
                    .kpi-section {{ margin-bottom: 30px; padding: 20px; border: 1px solid #ddd; border-radius: 8px; }}
                    .kpi-summary {{ background-color: #f9f9f9; padding: 15px; border-radius: 5px; margin-bottom: 15px; }}
                    details {{ margin-top: 10px; }}
                    summary {{ cursor: pointer; font-weight: bold; margin-bottom: 10px; }}
                    ul {{ margin-left: 20px; }}
                    hr {{ margin: 30px 0; }}
                </style>
            </head>
            <body>
                <h1>KPI Analysis Report for Sensor: {self.sensor}</h1>
                <p><strong>Report Generated:</strong> {time.strftime('%Y-%m-%d %H:%M:%S')}</p>
                <hr>
                {''.join(self.html_sections)}
            </body>
            </html>
            """
            
            with open(html_report_path, 'w', encoding='utf-8') as f:
                f.write(full_html)
            
            logging.info(f"HTML report saved to: {html_report_path}")
            
        except Exception as e:
            logging.error(f"Error saving HTML report to {html_report_path}: {e}")

    def save_html_report(self, html_report_path: str):
        """Public helper to persist the generated HTML sections"""
        if not self.html_sections:
            logging.warning("No HTML content generated for KPI report; skipping save.")
            return
        self._save_html_report(html_report_path)
    
    def get_kpi_htmls(self) -> List[Dict[str, Any]]:
        """Expose per-KPI HTML payloads for external saving"""
        return self.per_kpi_sections
    
    def _process_alignment_kpi(self):
        """
        Process alignment KPIs using the alignment_matching_kpi module.
        """
        try:
            from KPI.c_business_layer.alignment_matching_kpi import process_alignment_kpi
            
            # Process alignment KPI for each sensor
            results = []
            for sensor_id, stream_name in self._get_alignment_streams():
                result = process_alignment_kpi(
                    self.data, 
                    sensor_id
                )
                results.append(result)
            
            return results
        except Exception as e:
            logging.error(f"Error processing alignment KPIs: {str(e)}")
            return []

    def _process_detection_kpi(self):
        """
        Process detection KPIs using the detection_matching_kpi module.
        """
        try:
            from KPI.c_business_layer.detection_matching_kpi import process_detection_kpi
            
            # Process detection KPI for each sensor
            result = process_detection_kpi(
                    self.data,
                    self.sensor,
                )
            
            # Return as a list to match the expected format
            return [result]
            
        except Exception as e:
            logging.error(f"Error processing detection KPIs: {str(e)}")
            return []

    def _process_tracker_kpi(self):
        """
        Process tracker KPIs using the tracker_matching_kpi module.
        """
        try:
            from KPI.c_business_layer.tracker_matching_kpi import process_tracker_kpi

            results = []
            # Use the parsed TRACKER_STREAM models if available
            if isinstance(self.data, dict) and 'TRACKER_STREAM' in self.data:
                models = self.data['TRACKER_STREAM']
                if isinstance(models, dict) and 'input' in models and 'output' in models:
                    result = process_tracker_kpi(
                        models['input'],
                        models['output'],
                        self.sensor,
                        'TRACKER_STREAM'
                    )
                    results.append(result)
            return results
        except Exception as e:
            logging.error(f"Error processing tracker KPIs: {str(e)}")
            return []

    def _get_alignment_streams(self):
        """Get alignment streams from config"""
        return [(self.sensor, "alignment")]

    def _get_detection_streams(self):
        """Get detection streams from config"""
        return [("FC", "detection"), ("FL", "detection"), ("FR", "detection")]

    def _get_tracker_streams(self):
        """Get tracker streams from config"""
        return [(self.sensor, "tracker")]
