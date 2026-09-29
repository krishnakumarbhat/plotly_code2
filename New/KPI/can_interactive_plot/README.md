# CAN Interactive Plot

Combines the **CAN KPI** HDF parsing pipeline with the **Interactive Plot**
plotting pipeline, connected over ZeroMQ — exactly like the existing
`InteractivePlot <-> UDP_KPI` ZMQ bridge.

## Architecture

```
┌────────────────────────── can_interactive_plot/ ──────────────────────────┐
│                                                                            │
│  InteractivePlot/                     CAN layers (top level)              │
│    a_config_layer/        (from intplot_kpi)                              │
│    b_persistence_layer/   (from intplot_kpi, KPI client swapped)          │
│    c_data_storage/        (from intplot_kpi)                              │
│    d_business_layer/      (from intplot_kpi)                              │
│    e_presentation_layer/  (from intplot_kpi)                              │
│    kpi_client/                                                           │
│      can_kpi_integration.py    ZMQ client  ─────────┐                     │
│      hdf_add.proto / hdf_add_pb2.py                 │                     │
│                                                     │ ZMQ (REQ/REP)      │
│  a_persistence_layer/   (from can_kpi)  ◄───────────┘                    │
│    can_kpi_wrapper.py   parse_for_can_kpi()                              │
│  b_data_storage/        (from can_kpi)                                   │
│  c_business_layer/      (from can_kpi)                                   │
│  d_presentation_layer/  (from can_kpi)                                   │
│                                                                            │
│  can_kpi_server.py       ZMQ server (like UDP_KPI/kpi_server.py)          │
│  can_intplot_main.py     entry point (like ResimHTMLReport.py)            │
│  can_kpi_main.py         standalone CAN KPI run (like can_kpi/kpi_main.py)│
└────────────────────────────────────────────────────────────────────────────┘
```

Layers used from **CAN KPI**: `a_persistence_layer` (HDF reader/parser, JSON
config) and `b_data_storage` (scan-index keyed KPI storage) for all HDF
reading/parsing; `c_business_layer` (matching/F1 computation) and
`d_presentation_layer` (KPI HTML) for the KPI reports themselves.

Layers used from **Interactive Plot**: `a_config_layer` (XML/JSON config),
`b_persistence_layer` (plot data parsing), `c_data_storage` (plot config +
data model), `d_business_layer` (plot calculations), `e_presentation_layer`
(HTML/Plotly rendering). These are the plotting layers c/d/e.

### Data flow

1. `can_intplot_main.py` parses the XML + JSON config (same as the interactive
   plot pipeline) and starts `HdfProcessorFactory`.
2. During parsing, for every sensor, `CanKpiIntegration` (ZMQ REQ) sends a
   protobuf `RequestMessage` (sensor, input/output HDF paths, output dir,
   base name) to `can_kpi_server.py` (default port **5556**).
3. The server parses the HDF pair with the CAN parser (layer a + b), computes
   the match/precision/recall/F1/accuracy KPIs (layer c), renders the sensor
   KPI HTML (layer d) and replies with the HTML path.
4. `HtmlGenerator` fetches the latest KPI HTML path over ZMQ and injects it
   into the interactive report as a `kpi` category tab (`<base>/<SENSOR>/KPI/*.html`).

## Usage

```bash
# 1) Start the CAN KPI ZMQ server (default port 5556)
python can_kpi_server.py zmq 5556

# 2) Run the combined interactive plot pipeline (KPI enabled via ConfigInteractivePlots.xml PLOT_MODE)
python can_intplot_main.py ConfigInteractivePlots.xml InputsInteractivePlot.json html_out
```

The server also supports batch modes (same CLI as UDP_KPI/kpi_server.py):

```bash
python can_kpi_server.py kpi.json html_out        # batch from JSON config
python can_kpi_server.py input.h5 output.h5 html_out  # single pair
```

Standalone CAN KPI (no plots) works too:

```bash
python can_kpi_main.py kpi.json html_out
```

## Environment variables

| Variable | Default | Purpose |
| --- | --- | --- |
| `CAN_KPI_SERVER_HOST` | `127.0.0.1` | ZMQ server host |
| `CAN_KPI_SERVER_PORT` | `5556` | ZMQ server port (UDP KPI uses 5555) |
| `CAN_KPI_SERVER_RESPONSE_TIMEOUT_MS` | `180000` | Client reply timeout |
| `INTERACTIVE_PLOT_ENABLE_KPI` | set from XML `PLOT_MODE/KPI` | Master KPI switch |
| `CAN_KPI_LOG_LEVEL` | `INFO` | Server log level |

## Layout notes

- Per-sensor KPI pages are written to
  `<output_dir>/<base_name>/<SENSOR_ID>/KPI/<base_name>_<sensor_id>_kpi.html`,
  matching the layout the interactive plot index expects.
- Parsed HDF payloads are cached on the server per `(mtime, size)`, so a
  multi-sensor file is only parsed once per request burst.
