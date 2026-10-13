import json
import sys
import tempfile
import unittest
from pathlib import Path

import h5py
import numpy as np


ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))


from kpi_main import KpiMain


class KpiMainRenderingTests(unittest.TestCase):
    def setUp(self) -> None:
        self._tmpdir = tempfile.TemporaryDirectory()
        self.addCleanup(self._tmpdir.cleanup)
        self._tmp_path = Path(self._tmpdir.name)

    def test_run_pair_shows_flr_status_tab_for_fc_alias(self) -> None:
        input_hdf = self._tmp_path / "input_fc_alias.h5"
        output_hdf = self._tmp_path / "output_flr_sparse.h5"
        self._write_pair_hdf(input_hdf, sparse_sensor_id="CEER_FC")
        self._write_pair_hdf(output_hdf, sparse_sensor_id="CEER_FLR")

        result = KpiMain().run_pair(str(input_hdf), str(output_hdf))

        self.assertIn("SRR / FLR", result["html"])
        self.assertIn("Input: skipped because no HED_SCAN_INDEX", result["html"])
        self.assertIn("Output: skipped because no HED_SCAN_INDEX", result["html"])
        self.assertNotIn("CEER_FC", result["html"])

    def test_run_writes_index_accuracy_plot(self) -> None:
        input_hdf = self._tmp_path / "input_fc_alias.h5"
        output_hdf = self._tmp_path / "output_flr_sparse.h5"
        self._write_pair_hdf(input_hdf, sparse_sensor_id="CEER_FC")
        self._write_pair_hdf(output_hdf, sparse_sensor_id="CEER_FLR")

        config_path = self._tmp_path / "kpi.json"
        out_dir = self._tmp_path / "out_html"
        config_path.write_text(
            json.dumps(
                {
                    "INPUT_HDF": [str(input_hdf)],
                    "OUTPUT_HDF": [str(output_hdf)],
                }
            ),
            encoding="utf-8",
        )

        index_path = KpiMain().run(str(config_path), str(out_dir))
        index_html = index_path.read_text(encoding="utf-8")

        self.assertIn("Average Accuracy Across Logs", index_html)
        self.assertIn("Average Accuracy %", index_html)
        self.assertIn("Overall Average Accuracy", index_html)
        self.assertIn("input_fc_alias", index_html)

    def _write_pair_hdf(self, path: Path, sparse_sensor_id: str) -> None:
        with h5py.File(path, "w") as hdf:
            parsed_sensor = hdf.create_group("CEER_FL")
            self._populate_dataset_sensor(parsed_sensor)

            sparse_sensor = hdf.create_group(sparse_sensor_id)
            header = sparse_sensor.create_group("FLR_HEADER_001")
            header.create_dataset(
                "id_FLR_HEADER_001", data=np.array([100, 101], dtype=np.int64)
            )
            detection = sparse_sensor.create_group("FLR_DETECTION_001_004")
            detection.create_dataset(
                "timestamp_FLR_DETECTION_001_004",
                data=np.array([200, 201], dtype=np.int64),
            )

    def _populate_dataset_sensor(self, sensor: h5py.Group) -> None:
        payload = {
            "HED_SCAN_INDEX": np.array([1, 2], dtype=np.int64),
            "HED_NUM_OF_VALID_DETECTIONS": np.array([1, 2], dtype=np.int64),
            "HED_SENSOR_TIME_STAMP_SEC": np.array([10, 11], dtype=np.int64),
            "HED_SENSOR_TIME_STAMP_NS": np.array([100, 200], dtype=np.int64),
            "DET_RANGE_001": np.array([10.0, 11.0], dtype=np.float64),
            "DET_RANGE_002": np.array([20.0, 21.0], dtype=np.float64),
            "DET_RANGE_VELOCITY_001": np.array([1.5, 1.6], dtype=np.float64),
            "DET_RANGE_VELOCITY_002": np.array([2.5, 2.6], dtype=np.float64),
            "DET_AZIMUTH_001": np.array([0.1, 0.2], dtype=np.float64),
            "DET_AZIMUTH_002": np.array([0.3, 0.4], dtype=np.float64),
            "DET_ELEVATION_001": np.array([0.01, 0.02], dtype=np.float64),
            "DET_ELEVATION_002": np.array([0.03, 0.04], dtype=np.float64),
        }
        header = sensor.create_group("SRR_FL_HEADER_001")
        for key in (
            "HED_SCAN_INDEX",
            "HED_NUM_OF_VALID_DETECTIONS",
            "HED_SENSOR_TIME_STAMP_SEC",
            "HED_SENSOR_TIME_STAMP_NS",
        ):
            header.create_dataset(key, data=payload[key])

        detection = sensor.create_group("SRR_FL_DETECTION_001_004")
        for key, value in payload.items():
            if key.startswith("DET_"):
                detection.create_dataset(key, data=value)


if __name__ == "__main__":
    unittest.main()