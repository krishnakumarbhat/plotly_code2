from InteractivePlot.c_data_storage.config_loader import (
    get_stream_plot_config,
    get_plot_config,
    resolve_signal_name,
)


def test_bordnet_detection_stream_maps_required_signals():
    stream_name = "FLR_DETECTION_001_004"
    stream_config = get_stream_plot_config(stream_name)

    assert "ran" in stream_config
    assert "vel" in stream_config
    assert "phi" in stream_config
    assert "theta" in stream_config
    assert "timestamp" in stream_config
    assert stream_config["timestamp"]["plot_types"] == ["scatter_plot"]

    # Plot types must come from the active config (e.g. config.json),
    # not be forced to a single scatter_plot.
    detection_config = get_plot_config()["DETECTION_STREAM"]
    for signal_name, signal_config in detection_config.items():
        if not isinstance(signal_config, dict):
            continue
        if not signal_config.get("plot_types"):
            continue
        assert signal_name in stream_config
        assert stream_config[signal_name]["plot_types"] == list(
            signal_config["plot_types"]
        )

    assert resolve_signal_name(stream_name, "timestamp_FLR_DETECTION_001_004", stream_config) == "timestamp"
    assert resolve_signal_name(stream_name, "DET_RANGE_001", stream_config) == "ran"
    assert resolve_signal_name(stream_name, "DET_RANGE_VELOCITY_001", stream_config) == "vel"
    assert resolve_signal_name(stream_name, "DET_ELEVATION_003", stream_config) == "phi"
    assert resolve_signal_name(stream_name, "DET_AZIMUTH_004", stream_config) == "theta"
