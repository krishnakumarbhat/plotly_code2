# KPI System Implementation

This directory contains the KPI (Key Performance Indicator) system implementation that integrates with the InteractivePlot system. The KPI system processes sensor data in separate processes and generates HTML reports for analysis.

## Architecture Overview

The KPI system follows a layered architecture similar to the InteractivePlot system:

```
KPI/
├── a_persistence_layer/     # Data parsing and communication
├── b_data_storage/         # Configuration and data storage
├── c_business_layer/       # KPI calculation and process management
├── d_presentation_layer/   # HTML report generation
└── kpi_integration.py      # Main integration interface
```

## Key Features

### 1. Configuration Storage
- **File**: `b_data_storage/kpi_config_storage.py`
- Utilizes the same structure as InteractivePlot's `config_storage.py`
- Focuses on KPI-specific data requirements:
  - Alignment stream data (vacs_boresight_az/el fields)
  - Detection stream data (rdd1_dindx, rdd1_rindx)
  - Tracker stream data (track_id, track_quality)

### 2. ZMQ Communication
- **File**: `a_persistence_layer/kpi_zmq_communication.py`
- Implements ZeroMQ-based communication between InteractivePlot and KPI processes
- Supports message types:
  - `SENSOR_DATA_READY`: Notify KPI system that sensor data is ready
  - `KPI_PROCESSING_COMPLETE`: Notify completion with HTML report path
  - `ERROR`: Error reporting
  - `HEARTBEAT`: Health monitoring

### 3. HDF Data Parsing
- **File**: `a_persistence_layer/kpi_hdf_parser.py`
- Reuses InteractivePlot's HDF parsing functionality
- Extracts only KPI-required data streams
- Validates data completeness and preprocesses data for analysis

### 4. Process Management
- **File**: `c_business_layer/kpi_process_manager.py`
- Executes KPI processing in separate processes
- Manages process lifecycle and communication
- Provides both multiprocessing and subprocess execution options

### 5. HTML Report Generation
- **Files**: 
  - `d_presentation_layer/alignment_report.py` (moved from `templates/`)
  - `d_presentation_layer/detection_report.py` (moved from `templates/`)
  - `d_presentation_layer/kpi_html_gen.py` (new)
- `kpi_html_gen.py` builds a per-run index page `{base_name}_kpi.html` under `{output_dir}/{base_name}` that contains stylish button links to all KPI HTMLs generated per sensor (e.g., `*_alignment_kpi.html`, `*_detection_kpi.html`).

#### KPI Index Usage
```
python -m KPI.d_presentation_layer.kpi_html_gen --output-dir <output_dir> --base-name <base_name>
```
This writes `<output_dir>/<base_name>/<base_name>_kpi.html` that links to all KPI HTML files for all sensors.

## Usage

### Basic Integration with InteractivePlot

```python
from KPI.kpi_integration import get_kpi_integration

# Get KPI integration instance
kpi = get_kpi_integration()

# Process a sensor with KPI analysis
result = kpi.process_sensor_with_kpi(
    sensor_id="sensor_001",
    hdf_file_path="/path/to/sensor_data.hdf",
    wait_for_completion=True
)

if result["status"] == "completed":
    print(f"KPI report generated: {result['html_report_path']}")
```

### Asynchronous Processing

```python
# Start KPI processing without waiting
result = kpi.process_sensor_with_kpi(
    sensor_id="sensor_001",
    hdf_file_path="/path/to/sensor_data.hdf",
    wait_for_completion=False
)

# Check status later
status = kpi.get_sensor_status("sensor_001")
if status["status"] == "completed":
    # Get completion result
    completion = kpi.wait_for_kpi_completion("sensor_001")
```

### Standalone KPI Processing

```bash
# Run KPI processing as a standalone process
python KPI/c_business_layer/kpi_standalone_processor.py \
    --sensor-id sensor_001 \
    --data-path /path/to/sensor_data.hdf \
    --server-port 5555 \
    --output-dir /path/to/reports
```

### JSON Batch vs ZMQ/Protobuf
- In JSON batch mode, protobuf is not imported or required. Run:
```
python KPI/kpi_server.py --mode json --json-path path/to/KPIPlot.json --html-output-dir path/to/output
```
- In ZMQ server mode, protobuf is loaded on demand when the server starts.

## Configuration

### KPI Data Requirements

The system is configured to process the following data streams:

#### Alignment Stream
- `vacs_boresight_az_nominal`
- `vacs_boresight_az_estimated`
- `vacs_boresight_az_kf_internal`
- `vacs_boresight_el_nominal`
- `vacs_boresight_el_estimated`
- `vacs_boresight_el_kf_internal`

#### Detection Stream
- `rdd1_dindx`
- `rdd1_rindx`

#### Tracker Stream
- `track_id`
- `track_quality`

### Validation Rules

- **Alignment tolerance**: 0.1 degrees
- **Detection threshold**: 80% completeness
- **Tracker quality minimum**: 0.7
- **Data completeness threshold**: 95%

## Dependencies

The KPI system requires the following additional dependencies:

```txt
pyzmq>=25.0.0          # ZMQ communication
protobuf>=4.21.0       # Protocol buffer serialization
```

## Process Flow

1. **Sensor Data Ready**: InteractivePlot notifies KPI system that sensor data is available
2. **Data Parsing**: KPI system parses HDF file using InteractivePlot's parser
3. **Data Extraction**: Extracts only KPI-required data streams
4. **Data Validation**: Validates data completeness and quality
5. **KPI Calculation**: Calculates alignment, detection, and tracker KPIs
6. **Report Generation**: Generates HTML report with results
7. **Completion Notification**: Notifies InteractivePlot with HTML report path

## Error Handling

The system includes comprehensive error handling:

- **Data validation errors**: Reports missing or invalid data fields
- **Processing errors**: Captures and reports calculation errors
- **Communication errors**: Handles ZMQ connection issues
- **Timeout handling**: Prevents indefinite waiting for completion

## Monitoring and Status

```python
# Get server status
status = kpi.get_kpi_server_status()
print(f"Server running: {status['server_running']}")
print(f"Active sensors: {status['active_sensors']}")

# Get all sensor statuses
all_status = kpi.get_all_sensor_status()
for sensor_id, sensor_status in all_status.items():
    print(f"Sensor {sensor_id}: {sensor_status['status']}")
```

## Integration with InteractivePlot

The KPI system is designed to integrate seamlessly with InteractivePlot:

1. **Non-blocking**: KPI processing runs in separate processes
2. **Communication**: Uses ZMQ for reliable message passing
3. **Data reuse**: Leverages existing InteractivePlot parsing infrastructure
4. **Configuration**: Uses similar configuration structure
5. **Reporting**: Provides HTML reports that can be displayed in InteractivePlot

## Troubleshooting

### Common Issues

1. **ZMQ Connection Errors**: Ensure port 5555 is available
2. **Missing Dependencies**: Install pyzmq and protobuf
3. **Data Parsing Errors**: Check HDF file format and required fields
4. **Process Timeout**: Increase timeout value for large datasets

### Logging

The system uses Python's logging module. Set log level to DEBUG for detailed information:

```python
import logging
logging.basicConfig(level=logging.DEBUG)
```

## Future Enhancements

- **Real-time monitoring**: Web-based dashboard for KPI processing status
- **Batch processing**: Support for processing multiple sensors simultaneously
- **Custom KPI definitions**: Allow user-defined KPI calculations
- **Performance optimization**: Parallel processing for large datasets
- **Integration with external systems**: Support for exporting results to other systems 