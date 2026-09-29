GAP-A adaptive CFAR (knowledge / clutter-map / tracker-feedback / env-adaptive windows)
• 2022 IEICE Trans • https://doi.org/10.1587/transfun.2022eap1064 • “A CFAR Detection Algorithm Based on Clutter Knowledge for Cognitive Radar”
  trick: builds environment knowledge base (historical clutter maps + past detections) → adaptive reference windows instead of fixed CA/GO.
  crack: static knowledge base; inject online tracker-state feedback to update windows per scan (track-masked noise estimation / slow-fast dual-loop).
• 2024 IEEE TAES • https://doi.org/10.1109/TAES.2024.3445319 • “CoFAR Clutter Estimation Using Covariance-Free Bayesian Learning”
  trick: covariance-free Bayesian learning estimates clutter covariance without sample-inversion → enables environment-adaptive reference windows.
  crack: global prior; break with sector-local tracker-masked prior (exclude confirmed-target masks from clutter sample).
• 2022 IEEE TAES • https://doi.org/10.1109/TAES.2022.3206256 • “A Robust Variability Index CFAR Detector for Weibull Background”
  trick: variability-index threshold adapts to local Weibull shape → approximates env-adaptive windows.
  crack: no tracker feedback; add confirmed-target mask to exclude targets from scatter estimation (dual-loop: slow map / fast thresh).
FLAG: CoFAR ≈ env-adaptive window; 2022 transfun = knowledge-aided; slow/fast dual-loop & track-masked est flagged.

GAP-B robust filtering under non-Gaussian clutter (Student-t / VB / entropy / spray-rain)
• 2022 TIIS • https://doi.org/10.3837/tiis.2022.03.009 • “Robust Generalized Labeled Multi-Bernoulli Filter ... Variational Bayesian”
  trick: VB jointly estimates target state + clutter density → non-Gaussian measurement model via noise-estimation.
  crack: mean-field VB; upgrade with structured entropy-based covariance modulation (spectral-feature R modulation for rain/spray).
• 2022 RS • https://doi.org/10.3390/rs14215477 • “Reweighted Robust Particle Filtering Approach for Target Tracking in Automotive Radar Application”
  trick: reweighted PF with heavy-tailed likelihood → spray/rain-robust automotive tracking.
  crack: fixed proposal; modulate R by spectral rain-rate / spray-density features (spectral R modulation missing here).
• 2024 IEEE TAES • https://doi.org/10.1109/TAES.2023.3348769 • “MIMO Radar: H-Infinity Approach for Robust Multitarget Tracking in Unknown Cluttered Environment”
  trick: H∞ minimax bounds worst-case error under unknown non-Gaussian clutter.
  crack: conservative; replace with Student-t filter + entropy-adaptive R for tighter tails.
FLAG: VB = variational Bayes noise est; reweighted PF = spray/rain robust; H∞ does not modulate R by spectral features — seed #1 closes.

GAP-C maneuvering-target association (IMM/PDAF adaptive gates / track-split prevention / acceleration-aware DA)
• 2022 Sensors • https://doi.org/10.3390/s22030875 • “Hybrid Interacting Multiple Model Filtering for Improving Reliability of Radar-Based Forward Collision Warning Systems”
  trick: hybrid IMM switches motion models (CV/CA) via likelihood → adaptive model for maneuvering targets.
  crack: association gate fixed; expand Mahalanobis gate by IMM acceleration-covariance (acceleration-aware DA / cut-in split prevention).
• 2024 Measurement • https://doi.org/10.1016/j.measurement.2024.114797 • “A multi-target detection and position tracking algorithm based on mmWave-FMCW radar data”
  trick: mmWave multi-target tracking with data association pipeline.
  crack: Euclidean/constant gate; replace with acceleration-dependent χ² gate: r_gate = sqrt(χ²_mm + σₐ·|a|) using IMM model accel.
FLAG: Hybrid IMM = adaptive model; acceleration-aware Mahalanobis expansion absent in both → seeds below.

SEED 1 (GAP-A/B cross, no listed paper): Tracker-state predicted clutter map drives slow-loop clutter-map update + fast-loop detector threshold; confirmed targets masked out of covariance sample = track-masked noise estimation; R modulated by radar Cube spectral rain-rate feature (spectral-R) for spray/robust filtering.
SEED 2 (GAP-C, no listed paper): IMM acceleration covariance injected into Mahalanobis gate as time-varying χ² threshold: gate radius = sqrt(χ²_mm + k·σₐ·|a_IMM|); prevents track-splitting during cut-in by expanding gate only when acceleration covariance spikes.
