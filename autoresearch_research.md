# Autoresearch Research: Automotive Radar HDF5 KPI & Tracking Engine

## Objective
Refactor the CAN/UDP radar telemetry stack into a unified zero-copy ZeroMQ pipeline (`can_inplot`, `udp_inplot`) with strict 5-layer separation, and research + numerically verify information-divergence filtering (KL/JS/Wasserstein) and continuous-discrete EKF/UKF multi-target tracking on `edge_hdf` HDF5 data. Deliver interactive HTML visualizations and an `overnight_research_report.html`.

Publishable win: a verified, benchmarked equation/algorithm story (divergence-based false-alarm suppression fused into Kalman-family tracking) with real numbers from real radar HDF5 logs, plus a reusable 5-layer ZMQ architecture.

## Publish Venue
Primary: NeurIPS/ICML/ICLR (ML signal-processing track) or arXiv eess.SP / cs.LG. Alternative: IEEE FUSION / RadarConf (sensor fusion). Style: NeurIPS template if paper is produced.

## Metrics
- **Primary**: novelty_score (0-100, higher is better)
- **Secondary**: sota_gap_pct, contrast_score, proof_strength, prior_art_clear

## Research Resources (nearest venues for this topic)
- arXiv eess.SP (radar signal processing), cs.LG (tracking/state estimation)
- Semantic Scholar citation-sorted for: Kalman filter radar tracking, UKF/EKF multi-target, KL/JS divergence detection, Wasserstein sensor fusion
- IEEE FUSION proceedings, RadarConf

## High-Index Targets
- Wan & van der Merwe, "The Unscented Kalman Filter" (van der Merwe 2000 / Julier & Uhlmann), cited >6000 — crack: sigma-point design assumes Gaussian; tuning of alpha/beta/kappa is heuristic.
- Bar-Shalom, "Tracking and Data Association" — crack: association gates use Euclidean/Mahalanobis only, no divergence structure.
- Kantas et al., "An overview of sequential Monte Carlo methods" — crack: particle degeneracy.
- Kullback-Leibler / Cover & Thomas Elements of Information Theory — crack: KL asymmetric, unbounded; JS bounded; Wasserstein metric on matched supports.
- Research question: can a divergence-gated measurement innovation (replacing raw Mahalanobis gating) suppress radar false alarms without losing track continuity?

## Files in Scope
- `simg_zmq/KPI/can_kpi/`, `simg_zmq/KPI/can_interactive_plot/` → consolidated into `src/can_inplot/`
- `simg_zmq/KPI/UDP_KPI/`, `simg_zmq/KPI/intplot_kpi/` → consolidated into `src/udp_inplot/`
- `edge_hdf/` (read-only data)
- `src/can_inplot/` (01_ingest, 02_kpi, 03_transport, 04_algo, 05_visual)
- `src/udp_inplot/` (mirror)
- `experiments/`, `equations.md`, `strategies.md`, `papers/`
- Deliverables: `overnight_research_report.html`, dashboard

## Off Limits
- Do NOT modify files in `edge_hdf/` (raw data, read-only).
- Do NOT touch `simg_zmq/Hyperlink_tool/`, `simg_zmq/jira/`, `simg_zmq/main_html/` (unrelated runtime).
- Do NOT fabricate measurements; all metrics must come from numeric runs or real HDF5 logs.

## Constraints
- Zero fabricated citations. Every source paper logged in `equations.md` with Semantic Scholar citedByCount.
- MATH MUST RUN: every equation implemented + numerically verified (fault/variation protocol).
- Novelty check mandatory before any keep.
- Strict 5-layer separation; no redundant inner `inplot/` subfolders.
- ZMQ transport mandatory in 03_transport.
- Absolute imports from package root; type hints + docstrings (Purpose/Inputs/Outputs).

## What's Been Tried
- (baseline) Existing `can_kpi` (a-d layers) + `can_interactive_plot` (a-e layers with inner InteractivePlot/) + `UDP_KPI` (a-d) + `intplot_kpi` (a-e). ZMQ only exists in UDP_KPI kpi_server (REP/REQ protobuf). CAN inplot pushes KPI requests to a ZMQ server. No divergence metrics, no Kalman-family tracking, no index-cache dependency resolution.