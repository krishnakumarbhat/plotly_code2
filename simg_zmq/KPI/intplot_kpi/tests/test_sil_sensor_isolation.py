from types import SimpleNamespace

import numpy as np

from InteractivePlot.kpi_client import sil_radar_validation as sil


def test_process_pair_continues_after_one_sensor_validation_error(tmp_path, monkeypatch):
    output_dir = tmp_path / 'sil'
    pair = sil.FilePair(
        sensor='UNKNOWN',
        base_key='sample',
        veh_path=tmp_path / 'input.h5',
        resim_path=tmp_path / 'output.h5',
    )

    monkeypatch.setattr(sil, 'sensors_in_file', lambda _path: ['FC', 'FL', 'FR'])

    def fake_load(_path, sensor, build_frame=False):
        if sensor == 'FC':
            raise ValueError("FC: missing required signals ['ran']")
        return SimpleNamespace(sensor=sensor)
 
    monkeypatch.setattr(sil, 'load_radar_hdf', fake_load)
    monkeypatch.setattr(sil, '_build_comparison_bundle', lambda _veh, _resim: object())
    monkeypatch.setattr(
        sil,
        'match_loaded_radars',
        lambda _bundle, metric: SimpleNamespace(
            matched_veh_idx=np.array([0]),
            common_scan_indices=np.array([1]),
        ),
    )
    monkeypatch.setattr(sil, 'compute_loaded_kpis', lambda _bundle, _match: {})
    monkeypatch.setattr(sil, 'build_report_html', lambda *_args: '<html>ok</html>')

    reports = sil.process_pair(pair, output_dir, gate=1.0, metric='euclidean')

    assert [report.name for report in reports] == [
        'sample_FL_sil_validation_report.html',
        'sample_FR_sil_validation_report.html',
    ]
    assert all(report.read_text(encoding='utf-8') == '<html>ok</html>' for report in reports)
