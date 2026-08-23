FROM python:3.11-slim

ENV PYTHONUNBUFFERED=1 \
    PYTHONPATH=/app:/app/can_interactive_plot \
    CAN_KPI_SERVER_HOST=127.0.0.1 \
    CAN_KPI_SERVER_PORT=6000 \
    CAN_KPI_SERVER_RESPONSE_TIMEOUT_MS=180000 \
    PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION=python \
    LC_ALL=C

RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app/can_interactive_plot

COPY KPI/can_interactive_plot/a_persistence_layer ./a_persistence_layer
COPY KPI/can_interactive_plot/b_data_storage ./b_data_storage
COPY KPI/can_interactive_plot/c_business_layer ./c_business_layer
COPY KPI/can_interactive_plot/d_presentation_layer ./d_presentation_layer
COPY KPI/can_inplot /app/can_inplot
COPY KPI/can_interactive_plot/kpi_proto ./kpi_proto

COPY KPI/can_interactive_plot/can_kpi_main.py ./can_kpi_main.py
COPY KPI/can_interactive_plot/can_kpi_server.py ./can_kpi_server.py
COPY KPI/can_interactive_plot/hdf_batch.py ./hdf_batch.py
COPY KPI/can_interactive_plot/sensor_matcher.py ./sensor_matcher.py
COPY KPI/can_interactive_plot/ConfigInteractivePlots.xml ./ConfigInteractivePlots.xml
COPY KPI/udp_intplot_kpi/ConfigInteractivePlots_bordnet.xml ./ConfigInteractivePlots_bordnet.xml
COPY KPI/can_interactive_plot/InputsInteractivePlot.json ./InputsInteractivePlot.json
COPY KPI/can_interactive_plot/kpi.json ./kpi.json
COPY KPI/can_interactive_plot/requirements.txt ./requirements.txt
COPY KPI/can_interactive_plot/docker-entrypoint.sh /usr/local/bin/can-interactive-entrypoint

RUN python -m pip install --no-cache-dir --upgrade pip setuptools wheel \
    && python -m pip install --no-cache-dir -r requirements.txt \
    && chmod 755 /usr/local/bin/can-interactive-entrypoint \
    && python -c "import numpy, pandas, h5py, plotly, zmq, lxml; import can_kpi_server; print('CAN KPI and Interactive Plot imports OK')"

ENTRYPOINT ["/usr/local/bin/can-interactive-entrypoint"]
CMD ["help"]
