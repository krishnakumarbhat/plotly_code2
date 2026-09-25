import json
import subprocess
import sys
from pathlib import Path


SCRIPT_PATH = Path(__file__).resolve().parents[1] / "check_hdf_pairs.py"


def test_cli_generates_matched_json_and_revalidates(tmp_path):
    source_payload = {
        "INPUT_HDF": [
            "/tmp/sample_001_b05.h5",
            "/tmp/sample_003_b05.h5",
            "/tmp/sample_010_b05.h5",
        ],
        "OUTPUT_HDF": [
            "/tmp/sample_001_b05_r123.h5",
            "/tmp/sample_010_b05_r123.h5",
            "/tmp/sample_999_b05_r123.h5",
        ],
    }
    source_path = tmp_path / "source.json"
    matched_path = tmp_path / "matched.json"
    source_path.write_text(json.dumps(source_payload), encoding="utf-8")

    generate = subprocess.run(
        [sys.executable, str(SCRIPT_PATH), str(source_path), "--output-json", str(matched_path)],
        capture_output=True,
        text=True,
        check=False,
    )

    assert generate.returncode == 1
    assert matched_path.exists()
    assert "Missing in OUTPUT_HDF:" in generate.stdout
    assert "Missing in INPUT_HDF:" in generate.stdout

    matched_payload = json.loads(matched_path.read_text(encoding="utf-8"))
    assert matched_payload["INPUT_HDF"] == [
        "/tmp/sample_001_b05.h5",
        "/tmp/sample_010_b05.h5",
    ]
    assert matched_payload["OUTPUT_HDF"] == [
        "/tmp/sample_001_b05_r123.h5",
        "/tmp/sample_010_b05_r123.h5",
    ]

    validate = subprocess.run(
        [sys.executable, str(SCRIPT_PATH), str(matched_path)],
        capture_output=True,
        text=True,
        check=False,
    )

    assert validate.returncode == 0
    assert "OK: 2 HDF ids match" in validate.stdout