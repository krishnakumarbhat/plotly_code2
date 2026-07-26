from __future__ import annotations

import argparse
import json
import shutil
from pathlib import Path

from huggingface_hub import snapshot_download


def _download_bge_small(model_dir: Path) -> None:
    embedding_target = model_dir / "bge-small-en-v1.5"
    embedding_target.mkdir(parents=True, exist_ok=True)

    print(f"Downloading BAAI/bge-small-en-v1.5 to: {embedding_target}")
    snapshot_download(
        repo_id="BAAI/bge-small-en-v1.5",
        local_dir=str(embedding_target),
        local_dir_use_symlinks=False,
    )
    print("Done. Fast embedding model downloaded locally.")


def _download_nomic(model_dir: Path) -> None:
    project_root = Path(__file__).resolve().parent
    embedding_target = model_dir / "nomic-embed-text-v1.5"
    embedding_base_target = model_dir / "nomic-bert-2048"

    embedding_target.mkdir(parents=True, exist_ok=True)

    print(f"Downloading nomic-ai/nomic-embed-text-v1.5 to: {embedding_target}")
    snapshot_download(
        repo_id="nomic-ai/nomic-embed-text-v1.5",
        local_dir=str(embedding_target),
        local_dir_use_symlinks=False,
    )
    snapshot_download(repo_id="nomic-ai/nomic-embed-text-v1.5")

    print(f"Downloading nomic-ai/nomic-bert-2048 to: {embedding_base_target}")
    snapshot_download(
        repo_id="nomic-ai/nomic-bert-2048",
        local_dir=str(embedding_base_target),
        local_dir_use_symlinks=False,
    )
    snapshot_download(repo_id="nomic-ai/nomic-bert-2048")

    shutil.copy2(
        embedding_base_target / "configuration_hf_nomic_bert.py",
        embedding_target / "configuration_hf_nomic_bert.py",
    )
    shutil.copy2(
        embedding_base_target / "modeling_hf_nomic_bert.py",
        embedding_target / "modeling_hf_nomic_bert.py",
    )

    config_path = embedding_target / "config.json"
    config = json.loads(config_path.read_text(encoding="utf-8"))
    auto_map = config.get("auto_map", {})
    config["auto_map"] = {
        key: value.split("--", 1)[-1] if "--" in value else value
        for key, value in auto_map.items()
    }
    config_path.write_text(json.dumps(config, indent=2), encoding="utf-8")

    print("Done. Nomic embedding model downloaded locally.")


def main() -> None:
    parser = argparse.ArgumentParser(description="Download local embedding model assets")
    parser.add_argument(
        "--embedding",
        choices=["bge-small", "nomic"],
        default="nomic",
        help="Embedding model preset to download",
    )
    args = parser.parse_args()

    project_root = Path(__file__).resolve().parent
    model_dir = project_root / "model"
    model_dir.mkdir(parents=True, exist_ok=True)

    if args.embedding == "nomic":
        _download_nomic(model_dir)
        return

    _download_bge_small(model_dir)


if __name__ == "__main__":
    main()
