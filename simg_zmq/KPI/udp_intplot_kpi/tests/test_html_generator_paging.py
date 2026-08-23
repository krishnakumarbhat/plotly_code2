import json
from pathlib import Path

from InteractivePlot.e_presentation_layer.html_generator import HtmlGenerator


def _write_plot_json(path: Path, seed: int) -> None:
    payload = {
        "data": [
            {
                "type": "scatter",
                "mode": "lines",
                "x": [0, 1, 2],
                "y": [seed, seed + 1, seed + 2],
                "name": f"plot-{seed}",
            }
        ],
        "layout": {"title": {"text": f"Plot {seed}"}},
    }
    path.write_text(json.dumps(payload), encoding="utf-8")


def test_html_generator_splits_category_pages(tmp_path):
    plots_dir = tmp_path / "plots"
    plots_dir.mkdir()

    signal_plot_paths = {}
    for idx in range(9):
        plot_path = plots_dir / f"plot_{idx}.json"
        _write_plot_json(plot_path, idx)
        signal_plot_paths[f"general_plot_{idx}"] = str(plot_path)

    HtmlGenerator(
        signal_plot_paths=signal_plot_paths,
        html_name="sample_report",
        output_dir=str(tmp_path / "out"),
        input_filename="sample_input.h5",
        output_filename="sample_output.h5",
        sensor_position="RR",
        stream_name="DETECTION_STREAM",
    )

    stream_dir = tmp_path / "out" / "sample_input" / "RR" / "DETECTION_STREAM"
    page_one = stream_dir / "sample_report_general.html"
    page_two = stream_dir / "sample_report_general2.html"

    assert page_one.exists()
    assert page_two.exists()

    page_one_text = page_one.read_text(encoding="utf-8")
    page_two_text = page_two.read_text(encoding="utf-8")

    assert page_one_text.count('class="plot-shell"') == 8
    assert page_two_text.count('class="plot-shell"') == 1
    assert "Page 1 of 2" in page_one_text
    assert "sample_report_general2.html" in page_one_text
    assert "Page 2 of 2" in page_two_text


def test_compute_sensor_scan_summary_from_kpi_html(tmp_path):
    kpi_file = tmp_path / "RR_sil_validation_report.html"
    kpi_file.write_text(
        """
        <html><body>
        <table>
            <tr><td>common_scan_count</td><td>7</td></tr>
            <tr><td>input_only_scan_count</td><td>2</td></tr>
            <tr><td>output_only_scan_count</td><td>3</td></tr>
            <tr><td>f1_score</td><td>0.8</td></tr>
        </table>
        </body></html>
        """,
        encoding="utf-8",
    )

    sensor_stream_data = {
        "RR": {
            "KPI": {
                "kpi": [
                    {
                        "path": kpi_file,
                        "name": "RR_sil_validation_report",
                        "relative_path": str(kpi_file.name),
                    }
                ]
            }
        }
    }

    summary = HtmlGenerator._compute_sensor_scan_summary(sensor_stream_data)

    assert summary["RR"]["matched_scans"] == 7
    assert summary["RR"]["input_scans"] == 9
    assert summary["RR"]["output_scans"] == 10


def test_compute_sensor_accuracy_falls_back_to_avg_scan_match_pct(tmp_path):
    kpi_file = tmp_path / "FR_sil_validation_report.html"
    kpi_file.write_text(
        """
        <html><body>
        <table>
            <tr><td>avg_scan_match_pct</td><td>99.486032</td></tr>
            <tr><td>f1_score</td><td>1.986399</td></tr>
        </table>
        </body></html>
        """,
        encoding="utf-8",
    )

    sensor_stream_data = {
        "FR": {
            "KPI": {
                "kpi": [
                    {
                        "path": kpi_file,
                        "name": "FR_sil_validation_report",
                        "relative_path": str(kpi_file.name),
                    }
                ]
            }
        }
    }

    accuracy = HtmlGenerator._compute_sensor_accuracy(sensor_stream_data)

    assert accuracy["FR"] == 99.486032 / 100.0


def test_generate_sensors_html_uses_scan_match_percentage():
    sensor_stream_data = {
        "RR": {
            "KPI": {
                "kpi": [
                    {
                        "name": "RR_sil_validation_report",
                        "relative_path": "RR_sil_validation_report.html",
                    }
                ]
            }
        }
    }

    sensor_accuracy = {"RR": 0.8}
    sensor_scan_summary = {
        "RR": {
            "matched_scans": 7.0,
            "input_scans": 9.0,
            "output_scans": 10.0,
        }
    }

    html = HtmlGenerator._generate_sensors_html(
        sensor_stream_data,
        sensor_accuracy,
        sensor_scan_summary,
    )

    assert 'class="sensor-summary-primary"' in html
    assert 'class="sensor-summary-secondary">Scanindex Match % : 77.78' in html
    assert "Input scans :" not in html
    assert "Output scans :" not in html
    assert "Matched scans :" not in html