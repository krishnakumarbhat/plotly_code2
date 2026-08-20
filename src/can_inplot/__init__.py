"""can_inplot — unified CAN radar KPI + interactive plotting pipeline.

5-layer modular architecture:
    01_ingest    HDF5 (edge_hdf) parser and raw signal decoders
    02_kpi       CAN metrics: latency, detection rate, tracking KPI calculation
    03_transport ZeroMQ high-throughput message framing and distribution
    04_algo      Tracking filters (Kalman/EKF/UKF), clustering, divergence metrics
    05_visual    Interactive plotting routines and HTML canvas generation

Entry point: 00_main.py
"""

__version__ = "0.1.0"