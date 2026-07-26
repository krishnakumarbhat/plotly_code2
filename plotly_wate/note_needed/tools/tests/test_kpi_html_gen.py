from pathlib import Path

from KPI.d_presentation_layer.kpi_html_gen import generate_kpi_index


def test_generate_kpi_index_creates_index_with_links(tmp_path: Path):
    base_name = "R11"
    # Create per-sensor directories and dummy KPI htmls
    for sensor in ["FC", "FL", "FR"]:
        sensor_dir = tmp_path / base_name / sensor
        sensor_dir.mkdir(parents=True, exist_ok=True)
        # Create alignment and detection KPI files
        (sensor_dir / f"{base_name}_alignment_kpi.html").write_text("<html>alignment</html>", encoding="utf-8")
        (sensor_dir / f"{base_name}_detection_kpi.html").write_text("<html>detection</html>", encoding="utf-8")

    index_path = generate_kpi_index(str(tmp_path), base_name)

    assert index_path
    index_file = Path(index_path)
    assert index_file.exists()
    contents = index_file.read_text(encoding="utf-8")

    # Verify links exist for each sensor and KPI type
    assert "FC/" in contents
    assert "FL/" in contents
    assert "FR/" in contents
    assert f"{base_name}_alignment_kpi.html" in contents
    assert f"{base_name}_detection_kpi.html" in contents


def test_generate_kpi_index_handles_empty(tmp_path: Path):
    base_name = "EMPTY"
    # Only create base directory, no sensor KPI files
    (tmp_path / base_name).mkdir(parents=True, exist_ok=True)

    index_path = generate_kpi_index(str(tmp_path), base_name)
    index_file = Path(index_path)
    assert index_file.exists()
    contents = index_file.read_text(encoding="utf-8")
    assert "No KPI HTML files found" in contents
