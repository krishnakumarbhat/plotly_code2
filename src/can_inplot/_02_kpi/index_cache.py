"""Preprocessed index cache with dependency resolution.

Purpose: verify that the per-sensor KPI index/preprocessed cache exists and is
fresh; when missing, regenerate it by running the KPI index generator first.
Inputs : cache root, HDF pair metadata (path, mtime, size), optional generator.
Outputs: cache validity verdict; auto-regeneration when stale.
"""

import hashlib
import json
import logging
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Callable, Dict, Optional

logger = logging.getLogger(__name__)

CACHE_META_NAME = "index_meta.json"


@dataclass
class IndexCache:
    """Tracks and validates a preprocessed KPI index cache directory."""

    root: Path
    fingerprint: str = ""
    entries: Dict[str, Any] = field(default_factory=dict)

    @staticmethod
    def fingerprint_for(input_hdf: str, output_hdf: str) -> str:
        """Purpose: content fingerprint of an HDF pair (size+mtime, fast).
        Inputs : input/output HDF paths.
        Outputs: stable hex digest string."""
        digest = hashlib.sha256()
        for path in (input_hdf, output_hdf):
            p = Path(path)
            stat = p.stat() if p.exists() else None
            digest.update(p.name.encode("utf-8"))
            digest.update(
                (f"{stat.st_size}:{stat.st_mtime_ns}" if stat else "missing").encode()
            )
        return digest.hexdigest()[:16]

    def is_valid(self, fingerprint: str) -> bool:
        """Purpose: check whether the cache is present and matches the fingerprint.
        Inputs : expected content fingerprint.
        Outputs: True when cache is fresh."""
        meta_path = self.root / CACHE_META_NAME
        if not meta_path.exists():
            return False
        try:
            meta = json.loads(meta_path.read_text(encoding="utf-8"))
        except Exception:
            return False
        return meta.get("fingerprint") == fingerprint

    def write_meta(self, fingerprint: str) -> None:
        """Purpose: persist cache metadata after successful generation.
        Inputs : content fingerprint.
        Outputs: None."""
        self.root.mkdir(parents=True, exist_ok=True)
        meta = {
            "fingerprint": fingerprint,
            "entries": self.entries,
            "generated_by": "can_inplot.index_cache",
        }
        (self.root / CACHE_META_NAME).write_text(
            json.dumps(meta, indent=2), encoding="utf-8"
        )
        logger.info("Wrote index cache meta: %s", self.root / CACHE_META_NAME)


def verify_index_cache(
    input_hdf: str,
    output_hdf: str,
    cache_root: Path,
    generator: Optional[Callable[[], Any]] = None,
    force: bool = False,
) -> IndexCache:
    """Purpose: verify index cache freshness; regenerate when missing/stale.
    Inputs : HDF pair paths, cache root, optional generator callable, force flag.
    Outputs: IndexCache verdict; runs the generator when regeneration needed."""
    cache_root = Path(cache_root)
    cache = IndexCache(root=cache_root)
    fingerprint = IndexCache.fingerprint_for(input_hdf, output_hdf)
    cache.fingerprint = fingerprint

    if not force and cache.is_valid(fingerprint):
        logger.info("Index cache fresh for %s", cache_root)
        try:
            cache.entries = json.loads(
                (cache_root / CACHE_META_NAME).read_text(encoding="utf-8")
            ).get("entries", {})
        except Exception:
            pass
        return cache

    if generator is None:
        raise FileNotFoundError(
            f"Index cache missing/stale at {cache_root} and no generator provided. "
            "Run the can_kpi index generator first."
        )
    logger.info("Index cache missing/stale; invoking KPI index generator...")
    generator()
    cache.write_meta(fingerprint)
    return cache