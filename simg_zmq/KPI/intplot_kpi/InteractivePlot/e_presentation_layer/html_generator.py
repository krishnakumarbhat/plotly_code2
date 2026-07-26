import os  # noqa: F401
from InteractivePlot.e_presentation_layer.front_end import html_template
import logging
import json
from typing import Dict, List, Optional
from pathlib import Path
from datetime import datetime
import re
import math
from InteractivePlot.e_presentation_layer.main_front_end import css, js, master_index_css, master_index_js




class HtmlGenerator:
    """Lightweight HTML generator: fewer helpers, same behavior."""

    MAX_PLOTS_PER_PAGE = 8

    def __init__(
        self,
        signal_plot_paths: dict,
        html_name: str,
        output_dir: Optional[str] = None,
        input_filename: str = "",
        output_filename: str = "",
        sensor_position: str = "",
        stream_name: str = "",
        update_main_index: bool = False,
    ):
        """
        Initialize the HTML generator.
        
        Parameters:
            signal_plot_paths: Dictionary mapping plot keys to file paths
            html_name: Base name for HTML file
            output_dir: Root output directory
            input_filename: Input file name for metadata
            output_filename: Output file name for metadata
            sensor_position: Sensor position information

        """
        self.signal_plot_paths = signal_plot_paths
        self.html_name = Path(html_name).stem
        self.output_dir = output_dir or "html"
        self.input_filename = input_filename
        self.output_filename = output_filename
        self.sensor_position = sensor_position
        self.stream_name = stream_name
        self.update_main_index = update_main_index

        # Initialize logging
        self.logger = logging.getLogger(self.__class__.__name__)
        

        # Get base filename for folder creation
        self.base_filename = Path(input_filename).stem if input_filename else "default"
        
        # Pipeline: folders -> plots -> HTML
        self.folder_structure = self._create_folder_structure()
        self.plots = self._load_and_categorize_plots()
        self.generate_and_save_html_files()

    def _create_folder_structure(self) -> Dict[str, Path]:
        """Create organized folder structure for different plot types."""
        folder_structure = {}
        
        # Main folder for this file
        main_folder = Path(self.output_dir) / self.base_filename
        main_folder.mkdir(parents=True, exist_ok=True)
        folder_structure['main'] = main_folder
        
        # Create subfolders
        sensor_folder_name = self.sensor_position if self.sensor_position else "sensors"
        sensors_folder = main_folder / sensor_folder_name
        sensors_folder.mkdir(exist_ok=True)
        folder_structure["sensors"] = sensors_folder
        
        streams_folder = self.stream_name if self.stream_name else "streams"
        streams_folder = sensors_folder / streams_folder
        streams_folder.mkdir(exist_ok=True)
        folder_structure['streams'] = streams_folder
        
        # Store streams folder for direct HTML placement (no 4th level categories)
        folder_structure['streams_direct'] = streams_folder
        
        self.logger.info(f"Created folder structure for {self.base_filename}")
        return folder_structure

    def _categorize_plot(self, plot_key: str) -> str:
        plot_key_lower = plot_key.lower()
        if 'histogram' in plot_key_lower:
            return 'histogram'
        if 'mismatch' in plot_key_lower:
            return 'mismatch'
        if 'kpi' in plot_key_lower:
            return 'kpi'
        if 'sensor' in plot_key_lower:
            return 'sensor_data'
        if 'stream' in plot_key_lower:
            return 'stream_plots'
        return 'general'

    def _load_and_categorize_plots(self) -> Dict[str, List[dict]]:
        """Load and categorize plots from signal_plot_paths."""
        categorized_plots = {}
        
        for plot_key, plot_path in self.signal_plot_paths.items():
            category = self._categorize_plot(plot_key)
            
            if category not in categorized_plots:
                categorized_plots[category] = []
            
            categorized_plots[category].append({
                'key': plot_key,
                'path': plot_path
            })
        
        return categorized_plots

    def _build_page_nav(self, page_links: List[dict], current_page: int) -> str:
        if len(page_links) <= 1:
            return ""

        prev_link = ""
        next_link = ""
        if current_page > 0:
            prev_link = f'<a href="{page_links[current_page - 1]["filename"]}">Previous</a>'
        if current_page < len(page_links) - 1:
            next_link = f'<a href="{page_links[current_page + 1]["filename"]}">Next</a>'

        return (
            '<div class="page-nav">'
            f'<div>{prev_link}</div>'
            f'<div class="page-count">Page {current_page + 1} of {len(page_links)}</div>'
            f'<div>{next_link}</div>'
            '</div>'
        )

    def generate_html_content_for_category(
        self,
        category: str,
        plots_data: List[dict],
        current_page: int = 0,
        page_links: Optional[List[dict]] = None,
    ) -> str:
        """Generate HTML content for a specific plot category."""
        if not plots_data:
            raise ValueError(f"No plots available for category '{category}'.")

        content_html = '<div class="plot-grid">'
        
        for plot_info in plots_data:
            plot_path = plot_info['path']
            plot_key = plot_info['key']
            
            # Load plot JSON data and convert to HTML
            try:
                # with open(plot_path, 'r', encoding='utf-8') as f:
                #     plot_json = json.load(f)
                
                # # Convert JSON to Plotly figure and then to HTML
                # import plotly.graph_objects as go
                # figure = go.Figure(plot_json)



                # plot_content = figure.to_html(full_html=False, include_plotlyjs=False)
                import plotly.graph_objects as go
                with open(plot_path, 'r', encoding='utf-8') as f:
                    plot_data = json.load(f)
                    figure = go.Figure(plot_data)
                    plot_content = figure.to_html(
                        full_html=False,
                        include_plotlyjs=False,
                        config={
                            'displayModeBar': True,
                            'scrollZoom': True,
                            'responsive': True,
                            'doubleClick': False,
                            'displaylogo': False,
                        },
                    )
            except Exception as e:
                self.logger.error(f"Failed to load plot content from {plot_path}: {e}")
                plot_content = f"<p>Error loading plot: {e}</p>"
            
            # Create individual plot container
            plot_title = f"{self.sensor_position} {plot_key}" if self.sensor_position else plot_key
            plot_html = f'''
            <section class="plot-shell" data-category="{category}">
                <div class="plot-head">
                    <h4>{plot_title}</h4>
                    <div class="plot-tools">
                        <button type="button" class="plot-expand-btn" aria-expanded="false">Expand</button>
                    </div>
                </div>
                <div class="plot-body">
                    {plot_content}
                </div>
            </section>
            '''
            content_html += plot_html
            
        content_html += "</div>"

        generation_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        page_nav = self._build_page_nav(page_links or [], current_page)
        complete_content = f'''
        {page_nav}
        {content_html}
        {page_nav}
        '''
        
        final_html = (
            html_template.replace("{{TABS}}", "")
            .replace("{{CONTENT}}", complete_content)
            .replace("{{INPUT_FILENAME}}", self.input_filename)
            .replace("{{OUTPUT_FILENAME}}", self.output_filename)
            .replace("{{SENSOR_POSITION}}", self.sensor_position)
            .replace("{{GENERATION_TIME}}", generation_time)
        )

        return final_html

    def generate_and_save_html_files(self):
        """Generate and save organized HTML files for each plot category."""
        if not self.plots:
            # Don't crash the whole pipeline if a stream produces zero plots.
            # This can happen when an HDF stream exists but none of the configured
            # signals are present (naming mismatches, partial dumps, etc.).
            self.logger.warning("No plots available; generating an empty report page.")
            generation_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            empty_html = (
                html_template.replace("{{TABS}}", "")
                .replace(
                    "{{CONTENT}}",
                    "<div class=\"empty-state\">"
                    "<strong>No plots were generated for this sensor/stream.</strong>"
                    "</div>",
                )
                .replace("{{INPUT_FILENAME}}", self.input_filename)
                .replace("{{OUTPUT_FILENAME}}", self.output_filename)
                .replace("{{SENSOR_POSITION}}", self.sensor_position)
                .replace("{{GENERATION_TIME}}", generation_time)
            )

            streams_folder = self.folder_structure['streams_direct']
            file_path = streams_folder / f"{self.html_name}_general.html"
            with open(file_path, "w", encoding="utf-8") as file:
                file.write(empty_html)

            self._create_external_assets()
            return

        self.logger.info(f"Generating HTML files for {len(self.plots)} categories")
        
        category_files = []
        
        for category, category_plots in self.plots.items():
            try:
                streams_folder = self.folder_structure['streams_direct']
                page_count = max(1, math.ceil(len(category_plots) / self.MAX_PLOTS_PER_PAGE))
                page_links = []
                for page_idx in range(page_count):
                    suffix = "" if page_idx == 0 else str(page_idx + 1)
                    filename = f"{self.html_name}_{category}{suffix}.html"
                    page_links.append({
                        'filename': filename,
                        'page': page_idx + 1,
                    })

                for page_idx in range(page_count):
                    start = page_idx * self.MAX_PLOTS_PER_PAGE
                    end = start + self.MAX_PLOTS_PER_PAGE
                    html_content = self.generate_html_content_for_category(
                        category,
                        category_plots[start:end],
                        current_page=page_idx,
                        page_links=page_links,
                    )

                    filename = page_links[page_idx]['filename']
                    file_path = streams_folder / filename
                    with open(file_path, "w", encoding="utf-8") as file:
                        file.write(html_content)

                    content_size_kb = len(html_content) / 1024
                    self.logger.info(f"Category HTML saved: {file_path} ({content_size_kb:.1f} KB)")

                    relative_path = f"{self.sensor_position}/{self.stream_name}/{filename}"
                    display_name = filename if page_count == 1 else f"{self.html_name}_{category} page {page_idx + 1}"
                    category_files.append({
                        'display_name': display_name,
                        'relative_path': relative_path,
                        'category': category,
                        'file_path': file_path
                    })

            except ValueError as e:
                self.logger.warning(f"Warning: {str(e)}")
                continue

        # Create external CSS and JavaScript files
        self._create_external_assets()
        # KPI integration is optional (protobuf may be absent in lightweight builds)
        if os.environ.get('INTERACTIVE_PLOT_ENABLE_KPI', '0') == '1':
            try:
                from InteractivePlot.kpi_client.kpi_integration import kpiIntegration

                kpi_path = kpiIntegration.receive_html_path_from_kpi_server()
                if not kpi_path:
                    self.logger.debug("No KPI HTML path available yet")
                else:
                    self.logger.info(f"Received KPI HTML path: {kpi_path}")
            except Exception as e:
                self.logger.debug(f"KPI request failed: {e}")
        self._add_kpi_link(category_files)

        # Create sub main HTML index file with sensor/stream tabs and links
        if self.update_main_index:
            self._create_main_html_index(category_files)

    def _add_kpi_link(self, category_files: List[dict]) -> None:
        """Add KPI link using direct ZMQ request or fallback."""
        if os.environ.get('INTERACTIVE_PLOT_ENABLE_KPI', '0') != '1':
            return
        if not self.sensor_position:
            return
        base_folder = self.folder_structure['main']
            
        try:
            # Direct ZMQ request to get HTML path (data already sent during parsing)
                        


            kpi_html_path = self._request_kpi_html_directly()
            
            if kpi_html_path:
                kpi_path_obj = Path(kpi_html_path)
                
                if kpi_path_obj.exists():
                    category_files.append({
                        'display_name': kpi_path_obj.name,
                        'relative_path': str(kpi_path_obj.relative_to(base_folder)),
                        'category': 'kpi',
                        'file_path': kpi_path_obj
                    })
            else:
                # Fallback to local expected path
                kpi_folder = base_folder / self.sensor_position / 'KPI'
                for kpi_path_fallback in sorted(kpi_folder.glob("*.html")):
                    category_files.append({
                        'display_name': kpi_path_fallback.name,
                        'relative_path': str(kpi_path_fallback.relative_to(base_folder)),
                        'category': 'kpi',
                        'file_path': kpi_path_fallback
                    })
        except Exception as e:
            self.logger.debug(f"Failed to add KPI link: {e}")

    def _request_kpi_html_directly(self) -> Optional[str]:
        """Request HTML path directly from KPI server via ZMQ."""
        try:
            import zmq
            import InteractivePlot.kpi_client.hdf_add_pb2 as hdf_add_pb2

            context = zmq.Context.instance()
            socket = context.socket(zmq.REQ)
            socket.setsockopt(zmq.LINGER, 0)
            host = os.environ.get("KPI_SERVER_HOST", "127.0.0.1")
            port = int(os.environ.get("KPI_SERVER_PORT", "5555"))
            socket.connect(f"tcp://{host}:{port}")

            ping = hdf_add_pb2.PingMessage(message_type="request_html")
            socket.send(ping.SerializeToString())

            if socket.poll(2000) == 0:
                socket.close()
                return None

            response_bytes = socket.recv()
            reply = hdf_add_pb2.ReplyMessage()
            reply.ParseFromString(response_bytes)
            socket.close()

            if reply.status == "success" and reply.html_file_path:
                return reply.html_file_path
            return None
        except Exception as e:
            self.logger.debug(f"KPI server request failed: {e}")
            return None

    def _create_main_html_index(self, category_files: List[dict]):
        """Create sub main HTML index with tabs and category cards."""
        main_html_path = self.folder_structure['main'] / f"{self.base_filename}.html"
        main_content = self._create_main_html_template()
        hierarchy_data = self._organize_files_by_category(category_files)
        if hierarchy_data:
            main_content = self._update_main_html_content(main_content, hierarchy_data)
        with open(main_html_path, "w", encoding="utf-8") as main_file:
            main_file.write(main_content)
        self.logger.info(f"Main HTML index updated: {main_html_path}")

    def _create_main_html_template(self) -> str:
        """Create main HTML template."""
        return f"""
        <!DOCTYPE html>
        <html>
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>{self.base_filename} - Interactive Plot Report</title>
            <link rel="stylesheet" href="tab_interface.css">
        </head>
        <body>
            <div class="container">
                <div class="header">
                    <h1>📊 Interactive Plot Report</h1>
                    <div class="subtitle">{self.base_filename}</div>
                </div>
                <div class="content">
                    <div class="metadata">
                        <h3>📋 Report Information</h3>
                        <div class="metadata-grid">
                            <div class="metadata-item">
                                <strong>Generated:</strong> {datetime.now().strftime("%Y-%m-%d %H:%M:%S")}
                            </div>
                            <div class="metadata-item">
                                <strong>Tool run on:</strong> win32
                            </div>
                            <div class="metadata-item">
                                <strong>Tool Version:</strong> 1.0
                            </div>
                            <div class="metadata-item">
                                <strong>HTML Generated Time Info:</strong> {datetime.now().strftime("%Y-%m-%d %H:%M:%S")}
                            </div>
                        </div>
                    </div>
                    <div class="sensors-section">
                        <div class="sensors-title">
                            <span>🔧 Sensors</span>
                        </div>
                        <div class="sensor-tabs">
                            <!-- Sensor tabs will be populated here -->
                        </div>
                        <div class="streams-section" style="display: none;">
                            <div class="streams-title">
                                <span>🌊 Streams</span>
                            </div>
                            <div class="stream-tabs">
                                <!-- Stream tabs will be populated here -->
                            </div>
                        </div>
                        <div class="plots-content">
                            <!-- Plots will be loaded here -->
                        </div>
                    </div>
                </div>
            </div>
            
            <script src="tab_interface.js"></script>
        </body>
        </html>
        """

    def _organize_files_by_category(self, category_files: List[dict]) -> dict:
        """Organize files by category."""
        hierarchy = {}
        
        for file_info in category_files:
            category = file_info['category']
            
            if category not in hierarchy:
                hierarchy[category] = []
                
            hierarchy[category].append(file_info)
        
        return hierarchy

    def _update_main_html_content(self, main_content: str, hierarchy_data: dict) -> str:
        """Update main HTML content with hierarchical structure."""
        sensor_tabs_start = main_content.find('<!-- Sensor tabs will be populated here -->')
        stream_tabs_start = main_content.find('<!-- Stream tabs will be populated here -->')
        
        if sensor_tabs_start != -1:
            sensor_tabs_html = self._generate_sensor_tabs_html()
            main_content = main_content.replace('<!-- Sensor tabs will be populated here -->', sensor_tabs_html)
        
        if stream_tabs_start != -1:
            stream_tabs_html = self._generate_stream_tabs_html()
            main_content = main_content.replace('<!-- Stream tabs will be populated here -->', stream_tabs_html)
        
        plot_data_script = self._generate_plot_data_script(hierarchy_data)
        main_content = main_content.replace('</script>', plot_data_script + '\n        </script>')
        
        return main_content

    def _generate_sensor_tabs_html(self) -> str:
        """Generate HTML for sensor tabs - only current sensor."""
        if not self.sensor_position:
            return '<p>No sensor data available</p>'
        
        return f'''
            <a href="#" role="button" data-sensor="{self.sensor_position}" class="sensor-tab active" onclick="return selectSensor(this, '{self.sensor_position}')">
                🔧 {self.sensor_position}
            </a>
        '''

    def _generate_stream_tabs_html(self) -> str:
        """Generate HTML for stream tabs - only current stream."""
        if not self.stream_name:
            return '<p>No stream data available</p>'
        
        return f'''
            <a href="#" class="stream-tab active" onclick="selectStream('{self.stream_name}')">
                🌊 {self.stream_name}
            </a>
        '''

    def _generate_plot_data_script(self, hierarchy_data: dict) -> str:
        """Generate JS plot data mapping sensors->streams->category cards with hrefs."""
        plot_data: Dict[str, Dict[str, List[dict]]] = {}
        if self.sensor_position and self.stream_name:
            plot_data[self.sensor_position] = {}
            plot_data[self.sensor_position][self.stream_name] = []

            category_cards: List[dict] = []
            ordered_categories = [
                'histogram', 'mismatch', 'general', 'scatter', 'kpi', 'sensor_data', 'stream_plots'
            ]
            for cat in ordered_categories:
                files = hierarchy_data.get(cat, [])
                if not files:
                    continue
                files_sorted = sorted(files, key=lambda x: x['display_name'])
                file_info = files_sorted[0]
                category_cards.append({
                    'category': cat,
                    'count': len(files_sorted),
                    'name': f"{self.sensor_position} {cat} ({len(files_sorted)} page{'s' if len(files_sorted) != 1 else ''})",
                    'file_path': file_info['relative_path']
                })

            plot_data[self.sensor_position][self.stream_name] = category_cards

        script_content = f"""
                // Inject plot data for tab interface
                window.plotData = {plot_data};
                
                // Override getStreamsForSensor function with actual data
                function getStreamsForSensor(sensor) {{
                    const plotData = window.plotData || {{}};
                    const sensorData = plotData[sensor] || {{}};
                    return Object.keys(sensorData);
                }}
        """
        
        return script_content

    def _create_external_assets(self):
        """Create external CSS and JavaScript files for the tab interface."""
        main_folder = self.folder_structure['main']
        
        # Create CSS file
        css_path = main_folder / "tab_interface.css"
        with open(css_path, "w", encoding="utf-8") as css_file:
            css_file.write(css)
        self.logger.info(f"CSS file created: {css_path}")
        
        # Create JavaScript file
        js_path = main_folder / "tab_interface.js"
        with open(js_path, "w", encoding="utf-8") as js_file:
            js_file.write(js)
        self.logger.info(f"JavaScript file created: {js_path}")

    @classmethod
    def create_master_index(cls, output_dir: str, base_filename: str = "master_index"):
        """
        Create a master index HTML file with nested dropdowns for all sensors and streams.
        
        Parameters:
            output_dir: Root output directory
            base_filename: Base name for the master index file
        """
        master_index_path = Path(output_dir) / f"{base_filename}.html"
        
        # Find all HTML files in the output directory
        html_files = []
        for html_file in Path(output_dir).rglob("*.html"):
            if html_file.name != f"{base_filename}.html":
                relative_path = html_file.relative_to(Path(output_dir))
                html_files.append({
                    'path': html_file,
                    'relative_path': str(relative_path),
                    'name': html_file.stem
                })
        
        # Organize files by sensor and stream
        sensor_stream_data = cls._organize_by_sensor_stream(html_files)
        
        # Generate master index HTML
        master_html = cls._generate_master_index_html(sensor_stream_data, base_filename)
        
        # Save master index
        with open(master_index_path, "w", encoding="utf-8") as f:
            f.write(master_html)
        
        logging.getLogger(cls.__name__).info(f"Master index created: {master_index_path}")
        return master_index_path

    @classmethod
    def create_base_index(cls, output_dir: str, base_filename: str):
        """
        Create a per-input base index HTML inside the base folder, e.g. html/<base>/<base>.html
        Scans only that base folder for category HTML files and builds a nested view.
        """
        base_folder = Path(output_dir) / base_filename
        index_path = base_folder / f"{base_filename}.html"

        # Collect HTML files within the base folder only
        html_files: List[dict] = []
        if base_folder.exists():
            for html_file in base_folder.rglob("*.html"):
                if html_file.name == f"{base_filename}.html":
                    continue
                relative_path = html_file.relative_to(base_folder)
                html_files.append({
                    'path': html_file,
                    'relative_path': str(relative_path),
                    'name': html_file.stem
                })

        # Organize and render
        sensor_stream_data = cls._organize_by_sensor_stream(html_files)
        master_html = cls._generate_master_index_html(sensor_stream_data, base_filename)

        # Save
        index_path.parent.mkdir(parents=True, exist_ok=True)
        with open(index_path, "w", encoding="utf-8") as f:
            f.write(master_html)

        logging.getLogger(cls.__name__).info(f"Per-base index created: {index_path}")
        return index_path

    @classmethod
    def _organize_by_sensor_stream(cls, html_files: List[dict]) -> dict:
        """Organize HTML files by sensor and stream (supports multiple layouts)."""
        sensor_data = {}

        sensor_pat = re.compile(r"_([A-Z]{2,4})_sil_(validation_report|narrative)$", re.IGNORECASE)

        for file_info in html_files:
            path_parts = list(Path(file_info['relative_path']).parts)

            sensor_name = "Unknown"
            stream_name = "Unknown"

            if "sensors" in path_parts:
                try:
                    idx = path_parts.index("sensors")
                    sensor_name = path_parts[idx + 1]
                    stream_name = path_parts[idx + 2]
                except Exception:
                    pass
            elif len(path_parts) >= 4:
                sensor_name = path_parts[-3]
                stream_name = path_parts[-2]
            elif len(path_parts) >= 3:
                sensor_name = path_parts[0]
                stream_name = path_parts[1]

            filename = file_info['name']
            category = "general"
            if "sil_validation_report" in filename.lower() or "kpi_f1_summary" in filename.lower():
                category = "kpi"
            elif "sil_narrative" in filename.lower() or "detailed_on_" in filename.lower():
                category = "narrative"
            for cat in ['histogram', 'mismatch', 'kpi', 'sensor_data', 'stream_plots', 'scatter', 'general']:
                if cat in filename.lower():
                    category = cat
                    break

            if category in {"kpi", "narrative"}:
                m = sensor_pat.search(filename)
                if m:
                    sensor_name = m.group(1).upper()
                    stream_name = "KPI"

            if sensor_name not in sensor_data:
                sensor_data[sensor_name] = {}

            if stream_name not in sensor_data[sensor_name]:
                sensor_data[sensor_name][stream_name] = {}

            if category not in sensor_data[sensor_name][stream_name]:
                sensor_data[sensor_name][stream_name][category] = []

            sensor_data[sensor_name][stream_name][category].append(file_info)

        return sensor_data

    @classmethod
    def _extract_f1_from_html(cls, file_path: Path) -> float:
        try:
            txt = file_path.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            return float("nan")

        patterns = [
            re.compile(r"<tr><td>f1_score</td><td>([^<]+)</td></tr>", re.IGNORECASE),
            re.compile(r"f1_score\s*[:=]\s*([0-9]*\.?[0-9]+)", re.IGNORECASE),
        ]
        for pat in patterns:
            m = pat.search(txt)
            if not m:
                continue
            try:
                return float(m.group(1).strip())
            except Exception:
                continue
        return float("nan")

    @classmethod
    def _extract_kpi_metric_from_html(cls, file_path: Path, metric_name: str) -> float:
        try:
            txt = file_path.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            return float("nan")

        escaped = re.escape(metric_name)
        patterns = [
            re.compile(rf"<tr><td>{escaped}</td><td>([^<]+)</td></tr>", re.IGNORECASE),
            re.compile(rf"{escaped}\s*[:=]\s*([0-9]*\.?[0-9]+)", re.IGNORECASE),
        ]
        for pat in patterns:
            match = pat.search(txt)
            if not match:
                continue
            try:
                return float(match.group(1).strip())
            except Exception:
                continue
        return float("nan")

    @classmethod
    def _compute_sensor_accuracy(cls, sensor_stream_data: dict) -> Dict[str, float]:
        accuracy_map: Dict[str, float] = {}
        for sensor_name, streams in sensor_stream_data.items():
            accuracy_values: List[float] = []
            for categories in streams.values():
                for category_name, plots in categories.items():
                    if category_name != "kpi":
                        continue
                    for plot_info in plots:
                        if "sil_validation_report" not in plot_info["name"].lower():
                            continue
                        file_path = Path(plot_info["path"])
                        f1 = cls._extract_f1_from_html(file_path)
                        if not math.isnan(f1) and 0.0 <= f1 <= 1.0:
                            accuracy_values.append(f1)
                            continue

                        avg_scan_match_pct = cls._extract_kpi_metric_from_html(file_path, "avg_scan_match_pct")
                        if not math.isnan(avg_scan_match_pct) and 0.0 <= avg_scan_match_pct <= 100.0:
                            accuracy_values.append(avg_scan_match_pct / 100.0)
            accuracy_map[sensor_name] = (
                (sum(accuracy_values) / len(accuracy_values)) if accuracy_values else float("nan")
            )
        return accuracy_map

    @classmethod
    def _compute_sensor_scan_summary(cls, sensor_stream_data: dict) -> Dict[str, Dict[str, float]]:
        summary_map: Dict[str, Dict[str, float]] = {}
        for sensor_name, streams in sensor_stream_data.items():
            common_total = 0.0
            input_only_total = 0.0
            output_only_total = 0.0
            has_scan_data = False

            for categories in streams.values():
                for category_name, plots in categories.items():
                    if category_name != "kpi":
                        continue
                    for plot_info in plots:
                        if "sil_validation_report" not in plot_info["name"].lower():
                            continue
                        file_path = Path(plot_info["path"])
                        common = cls._extract_kpi_metric_from_html(file_path, "common_scan_count")
                        input_only = cls._extract_kpi_metric_from_html(file_path, "input_only_scan_count")
                        output_only = cls._extract_kpi_metric_from_html(file_path, "output_only_scan_count")
                        if math.isnan(common) and math.isnan(input_only) and math.isnan(output_only):
                            continue
                        has_scan_data = True
                        common_total += 0.0 if math.isnan(common) else common
                        input_only_total += 0.0 if math.isnan(input_only) else input_only
                        output_only_total += 0.0 if math.isnan(output_only) else output_only

            if has_scan_data:
                summary_map[sensor_name] = {
                    "matched_scans": common_total,
                    "input_scans": common_total + input_only_total,
                    "output_scans": common_total + output_only_total,
                }
            else:
                summary_map[sensor_name] = {
                    "matched_scans": float("nan"),
                    "input_scans": float("nan"),
                    "output_scans": float("nan"),
                }

        return summary_map

    @classmethod
    def _generate_master_index_html(cls, sensor_stream_data: dict, base_filename: str) -> str:
        """Generate master index HTML with nested dropdowns."""
        generation_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        sensor_accuracy = cls._compute_sensor_accuracy(sensor_stream_data)
        sensor_scan_summary = cls._compute_sensor_scan_summary(sensor_stream_data)
        valid_sensor_acc = [v for v in sensor_accuracy.values() if not math.isnan(v)]
        avg_accuracy = (sum(valid_sensor_acc) / len(valid_sensor_acc)) if valid_sensor_acc else float("nan")
        avg_accuracy_text = f"{avg_accuracy:.4f}" if not math.isnan(avg_accuracy) else "NA"
        
        html_content = f"""
        <!DOCTYPE html>
        <html>
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Master Interactive Plot Report</title>
            <style>
                {master_index_css}
            </style>
        </head>
        <body>
            <div class="container">
                <div class="header">
                    <h1>📊 Master Interactive Plot Report</h1>
                    <div class="subtitle">Comprehensive Sensor and Stream Analysis</div>
                </div>
                <div class="content">
                    <div class="metadata">
                        <h3>📋 Report Information</h3>
                        <div class="metadata-grid">
                            <div class="metadata-item">
                                <strong>Generated:</strong> {generation_time}
                            </div>
                            <div class="metadata-item">
                                <strong>Tool run on:</strong> win32
                            </div>
                            <div class="metadata-item">
                                <strong>Tool Version:</strong> 1.0
                            </div>
                            <div class="metadata-item">
                                <strong>HTML Generated Time Info:</strong> {generation_time}
                            </div>
                            <div class="metadata-item">
                                <strong>Average Accuracy (F1):</strong> {avg_accuracy_text}
                            </div>
                        </div>
                    </div>
                    <div class="sensors-container">
                        {cls._generate_sensors_html(sensor_stream_data, sensor_accuracy, sensor_scan_summary)}
                    </div>
                </div>
            </div>
            
            <script>
                {master_index_js}
            </script>
        </body>
        </html>
        """
        
        return html_content

    @classmethod
    def _generate_sensors_html(cls, sensor_stream_data: dict, sensor_accuracy: dict, sensor_scan_summary: dict) -> str:
        """Generate HTML for sensors section."""
        html_parts = []
        
        def _sensor_sort_key(name: str):
            n = str(name or "").strip().lower()
            is_other = (n == "unknown" or n == "others")
            return (1 if is_other else 0, n)

        for sensor_name in sorted(sensor_stream_data.keys(), key=_sensor_sort_key):
            streams = sensor_stream_data[sensor_name]
            display_sensor_name = "Others" if str(sensor_name).strip().lower() == "unknown" else sensor_name
            acc = sensor_accuracy.get(sensor_name, float("nan"))
            scan_summary = sensor_scan_summary.get(sensor_name, {})
            input_scans = scan_summary.get("input_scans", float("nan"))
            matched_scans = scan_summary.get("matched_scans", float("nan"))
            if (
                not math.isnan(input_scans)
                and not math.isnan(matched_scans)
                and input_scans > 0
            ):
                scan_match_pct = (matched_scans / input_scans) * 100.0
                scan_txt = f" Scanindex Match % : {scan_match_pct:.2f}"
            else:
                scan_txt = " Scanindex Match % : NA"
            if not math.isnan(acc):
                match_pct = acc * 100.0
                mismatch_pct = max(0.0, (1.0 - acc) * 100.0)
                primary_txt = f"Sensor {display_sensor_name} - Match % : {match_pct:.2f}  Missmatch % : {mismatch_pct:.2f}"
            else:
                primary_txt = f"Sensor {display_sensor_name} - Match % : NA  Missmatch % : NA"
            html_parts.append(f'''
            <div class="sensor-section">
                <div class="sensor-title" onclick="toggleSection(this)">
                    <span class="sensor-summary">
                        <span class="sensor-summary-primary">🔧 {primary_txt}</span>
                        <span class="sensor-summary-secondary">{scan_txt.strip()}</span>
                    </span>
                    <span class="toggle-icon">▶</span>
                </div>
                <div class="sensor-content">
                    {cls._generate_streams_html(streams)}
                </div>
            </div>
            ''')
        
        return '\n'.join(html_parts)

    @classmethod
    def _generate_streams_html(cls, streams: dict) -> str:
        """Generate HTML for streams section."""
        html_parts = []
        
        def _stream_sort_key(name: str):
            n = str(name or "").strip().lower()
            is_other = (n == "unknown" or n == "others")
            return (1 if is_other else 0, n)

        for stream_name in sorted(streams.keys(), key=_stream_sort_key):
            categories = streams[stream_name]
            display_stream_name = "Others" if str(stream_name).strip().lower() == "unknown" else stream_name
            total_plots = sum(len(plots) for plots in categories.values())
            html_parts.append(f'''
            <div class="stream-section">
                <div class="stream-title" onclick="toggleSection(this)">
                    <span>🌊 {display_stream_name} ({total_plots} plots)</span>
                    <span class="toggle-icon">▶</span>
                </div>
                <div class="stream-content">
                    {cls._generate_categories_html(categories)}
                </div>
            </div>
            ''')
        
        return '\n'.join(html_parts)

    @classmethod
    def _generate_categories_html(cls, categories: dict) -> str:
        """Generate HTML for categories section."""
        html_parts = []
        
        category_icons = {
            'histogram': '📊',
            'mismatch': '⚠️',
            'sensor_data': '🔧',
            'stream_plots': '🌊',
            'kpi': '📈',
            'narrative': '🧾',
            'general': '📋',
            'scatter': '📈'
        }
        
        for category_name, plots in categories.items():
            icon = category_icons.get(category_name, '📄')
            category_title = category_name.replace('_', ' ').title()
            
            html_parts.append(f'''
            <div class="category-section">
                <div class="category-title">
                    <span class="category-icon">{icon}</span>
                    {category_title} ({len(plots)} plots)
                </div>
                <div class="plot-links">
            ''')
            
            for plot_info in plots:
                display_name = plot_info['name'].replace('_', ' ')
                html_parts.append(f'''
                    <a href="{plot_info['relative_path']}" class="plot-link">
                        <span class="plot-icon">📄</span>
                        {display_name}
                    </a>
                ''')
            
            html_parts.append('</div></div>')
        
        return '\n'.join(html_parts)
