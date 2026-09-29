import sys
import tempfile
import unittest
from pathlib import Path

import h5py
import numpy as np


ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))


from a_persistence_layer.hdf_parser import KpiHdfParser


class HdfParserRegressionTests(unittest.TestCase):
    def setUp(self) -> None:
        self._tmpdir = tempfile.TemporaryDirectory()
        self.addCleanup(self._tmpdir.cleanup)
        self._tmp_path = Path(self._tmpdir.name)
        self._required_signals = [
            "DET_RANGE",
            "DET_RANGE_VELOCITY",
            "DET_AZIMUTH",
            "DET_ELEVATION",
        ]

    def test_parse_attribute_backed_hdf(self) -> None:
        hdf_path = self._tmp_path / "legacy_attr.h5"
        self._write_attr_sensor_hdf(hdf_path)

        parser = KpiHdfParser(required_detection_signals=self._required_signals)
        parsed = parser.parse_file(str(hdf_path))
        report = parser.get_last_parse_report()

        self.assertEqual(report["status"], "ok")
        self.assertEqual(sorted(parsed.keys()), ["CEER_FL"])

        storage = parsed["CEER_FL"]["storage"]
        np.testing.assert_array_equal(storage.get_scan_index(), np.array([1, 2]))
        np.testing.assert_array_equal(
            storage.get_time_ns(),
            np.array([10_000_000_100, 11_000_000_200], dtype=np.int64),
        )
        self.assertEqual(
            [row.tolist() for row in storage.get_detection_rows("DET_RANGE")],
            [[10.0], [11.0, 21.0]],
        )

    def test_parse_dataset_backed_hdf_and_skip_sparse_sensor(self) -> None:
        hdf_path = self._tmp_path / "dataset_layout.h5"
        self._write_dataset_sensor_hdf(hdf_path, include_sparse_sensor=True)

        parser = KpiHdfParser(required_detection_signals=self._required_signals)
        parsed = parser.parse_file(str(hdf_path))
        report = parser.get_last_parse_report()

        self.assertEqual(sorted(parsed.keys()), ["CEER_FL"])
        self.assertEqual(report["status"], "partial")
        self.assertEqual(report["parsed_sensors"], ["CEER_FL"])
        self.assertEqual(report["skipped_sensors"], ["CEER_FLR"])
        self.assertTrue(any("CEER_FLR" in warning for warning in report["warnings"]))

        storage = parsed["CEER_FL"]["storage"]
        np.testing.assert_array_equal(
            storage.get_valid_detection_counts(), np.array([1, 2], dtype=np.int64)
        )
        self.assertEqual(
            [row.tolist() for row in storage.get_detection_rows("DET_RANGE_VELOCITY")],
            [[1.5], [1.6, 2.6]],
        )

    def test_parse_ceer_fc_alias_as_flr(self) -> None:
        hdf_path = self._tmp_path / "dataset_fc_alias.h5"
        self._write_dataset_sensor_hdf(hdf_path, sensor_id="CEER_FC")

        parser = KpiHdfParser(required_detection_signals=self._required_signals)
        parsed = parser.parse_file(str(hdf_path))
        report = parser.get_last_parse_report()

        self.assertEqual(sorted(parsed.keys()), ["CEER_FLR"])
        self.assertEqual(report["parsed_sensors"], ["CEER_FLR"])
        storage = parsed["CEER_FLR"]["storage"]
        np.testing.assert_array_equal(storage.get_scan_index(), np.array([1, 2]))

    def _write_attr_sensor_hdf(self, path: Path) -> None:
        with h5py.File(path, "w") as hdf:
            sensor = hdf.create_group("CEER_FL")
            self._populate_attr_sensor(sensor)

    def _write_dataset_sensor_hdf(
        self,
        path: Path,
        include_sparse_sensor: bool = False,
        sensor_id: str = "CEER_FL",
    ) -> None:
        with h5py.File(path, "w") as hdf:
            sensor = hdf.create_group(sensor_id)
            self._populate_dataset_sensor(sensor)

            if include_sparse_sensor:
                sparse = hdf.create_group("CEER_FLR")
                header = sparse.create_group("FLR_HEADER_001")
                header.create_dataset(
                    "id_FLR_HEADER_001", data=np.array([100, 101], dtype=np.int64)
                )
                detection = sparse.create_group("FLR_DETECTION_001_004")
                detection.create_dataset(
                    "timestamp_FLR_DETECTION_001_004",
                    data=np.array([200, 201], dtype=np.int64),
                )

    def _populate_attr_sensor(self, sensor: h5py.Group) -> None:
        header = sensor.create_group("SRR_FL_HEADER_001")
        payload = self._sensor_payload()
        for key in (
            "HED_SCAN_INDEX",
            "HED_NUM_OF_VALID_DETECTIONS",
            "HED_SENSOR_TIME_STAMP_SEC",
            "HED_SENSOR_TIME_STAMP_NS",
        ):
            header.attrs[key] = payload[key]

        detection = sensor.create_group("SRR_FL_DETECTION_001_004")
        for key, value in payload.items():
            if key.startswith("DET_"):
                detection.attrs[key] = value

    def _populate_dataset_sensor(self, sensor: h5py.Group) -> None:
        header = sensor.create_group("SRR_FL_HEADER_001")
        payload = self._sensor_payload()
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

    def _sensor_payload(self) -> dict[str, np.ndarray]:
        return {
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


if __name__ == "__main__":
    unittest.main()