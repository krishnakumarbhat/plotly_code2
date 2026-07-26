import argparse
import json
import re
from pathlib import Path


HDF_ID_PATTERN = re.compile(r"_(\d+)_b\d+(?:_r\d+)?\.h5$", re.IGNORECASE)


def extract_hdf_id(path_value: str) -> str:
    match = HDF_ID_PATTERN.search(Path(path_value).name)
    if not match:
        raise ValueError(f"Could not extract HDF id from: {path_value}")
    return match.group(1)


def load_hdf_ids(items: list[str], label: str) -> dict[str, list[str]]:
    ids_to_paths: dict[str, list[str]] = {}
    for item in items:
        hdf_id = extract_hdf_id(item)
        ids_to_paths.setdefault(hdf_id, []).append(item)

    duplicates = {hdf_id: paths for hdf_id, paths in ids_to_paths.items() if len(paths) > 1}
    if duplicates:
        duplicate_ids = ", ".join(sorted(duplicates))
        raise ValueError(f"Duplicate {label} ids found: {duplicate_ids}")

    return ids_to_paths


def load_payload(json_path: Path) -> tuple[dict, dict[str, list[str]], dict[str, list[str]]]:
    with json_path.open("r", encoding="utf-8") as file_handle:
        payload = json.load(file_handle)

    input_hdf = payload.get("INPUT_HDF")
    output_hdf = payload.get("OUTPUT_HDF")

    if not isinstance(input_hdf, list) or not isinstance(output_hdf, list):
        raise ValueError("JSON must contain INPUT_HDF and OUTPUT_HDF arrays")

    input_ids = load_hdf_ids(input_hdf, "INPUT_HDF")
    output_ids = load_hdf_ids(output_hdf, "OUTPUT_HDF")

    return payload, input_ids, output_ids


def build_matched_payload(
    payload: dict,
    input_ids: dict[str, list[str]],
    output_ids: dict[str, list[str]],
) -> dict:
    matched_ids = sorted(set(input_ids) & set(output_ids), key=int)
    matched_payload = dict(payload)
    matched_payload["INPUT_HDF"] = [input_ids[hdf_id][0] for hdf_id in matched_ids]
    matched_payload["OUTPUT_HDF"] = [output_ids[hdf_id][0] for hdf_id in matched_ids]
    return matched_payload


def write_json(payload: dict, output_path: Path) -> None:
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with output_path.open("w", encoding="utf-8") as file_handle:
        json.dump(payload, file_handle, indent=2)
        file_handle.write("\n")


def validate_hdf_pairs(json_path: Path, output_json: Path | None = None) -> int:
    payload, input_ids, output_ids = load_payload(json_path)

    missing_in_output = sorted(set(input_ids) - set(output_ids))
    missing_in_input = sorted(set(output_ids) - set(input_ids))

    if output_json is not None:
        matched_payload = build_matched_payload(payload, input_ids, output_ids)
        write_json(matched_payload, output_json)
        print(
            f"Wrote matched JSON with {len(matched_payload['INPUT_HDF'])} ids to {output_json}"
        )

    if not missing_in_output and not missing_in_input:
        print(f"OK: {len(input_ids)} HDF ids match between INPUT_HDF and OUTPUT_HDF")
        return 0

    if missing_in_output:
        print("Missing in OUTPUT_HDF:")
        for hdf_id in missing_in_output:
            print(f"  {hdf_id} -> {input_ids[hdf_id]}")

    if missing_in_input:
        print("Missing in INPUT_HDF:")
        for hdf_id in missing_in_input:
            print(f"  {hdf_id} -> {output_ids[hdf_id]}")

    return 1


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Check that INPUT_HDF and OUTPUT_HDF contain the same HDF sequence ids."
    )
    parser.add_argument("json_path", type=Path, help="Path to the MUDP JSON file")
    parser.add_argument(
        "--output-json",
        type=Path,
        help="Optional path for a normalized JSON file containing only matched INPUT_HDF and OUTPUT_HDF pairs.",
    )
    args = parser.parse_args()
    return validate_hdf_pairs(args.json_path, args.output_json)


if __name__ == "__main__":
    raise SystemExit(main())