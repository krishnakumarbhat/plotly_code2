"""Robust sensor/stream name matching for CAN HDF pairs.

The HDF producers can change group names between files/runs (e.g. ``CEER_ FL``,
``CEER_FL``, ``MCIP_FL``, ``SRR_FL`` or just ``FL``).  Matching is tiered:

1. exact match after normalization (alphanumeric only, lower case)
2. position token embedded in the name (``FLR/FL/FR/RL/RR/FC``) as whole word
3. fuzzy similarity (difflib) above a threshold

This module is intentionally dependency-free (stdlib only) so both the
InteractivePlot package and the flat CAN KPI package can import it.
"""

import re
from difflib import SequenceMatcher
from typing import Dict, List, Optional, Tuple

POSITION_TOKENS = ("FLR", "FL", "FR", "RL", "RR", "FC")

# Stable canonical sensor id per position token.  Both the interactive plot
# pipeline and the CAN KPI pipeline use these ids so their output folders and
# links line up regardless of the producer's outer group naming.
CEER_BY_POSITION = {
    "FL": "CEER_FL",
    "FLR": "CEER_FLR",
    "FR": "CEER_FR",
    "RL": "CEER_RL",
    "RR": "CEER_RR",
    "FC": "CEER_FLR",
}

_POS_RE = {
    tok: re.compile(rf"(?:^|[^A-Z0-9]){tok}(?:$|[^A-Z0-9])")
    for tok in POSITION_TOKENS
}


def normalize_name(name) -> str:
    """Lower-case alphanumeric form: ``CEER_ FL`` -> ``ceerfl``."""
    return re.sub(r"[^a-z0-9]", "", str(name or "").lower())


def extract_position(name) -> Optional[str]:
    """Return the whole-word position token inside ``name`` (e.g. ``FL``)."""
    u = str(name or "").upper()
    for tok in POSITION_TOKENS:
        if _POS_RE[tok].search(u):
            return tok
    return None


def canonical_sensor_id(name) -> str:
    """Stable id for a sensor name: ``CEER_<POSITION>`` when the name carries a
    position token, otherwise the normalized alphanumeric name."""
    pos = extract_position(name)
    if pos and pos in CEER_BY_POSITION:
        return CEER_BY_POSITION[pos]
    norm = normalize_name(name)
    return norm or str(name or "").strip().lower()


def fuzzy_ratio(a: str, b: str) -> float:
    return SequenceMatcher(None, normalize_name(a), normalize_name(b)).ratio()


def match_sensor_pairs(
    in_names: List[str],
    out_names: List[str],
    fuzzy_threshold: float = 0.8,
) -> Tuple[Dict[str, Tuple[str, str]], List[str], List[str]]:
    """Pair input sensor names with output sensor names.

    Returns ``(pairs, unmatched_in, unmatched_out)`` where ``pairs`` maps a
    canonical key -> ``(actual_in_name, actual_out_name)``.
    """
    pairs: Dict[str, Tuple[str, str]] = {}
    used_in: set = set()
    used_out: set = set()

    # Tier 1: exact normalized match.
    norm_in = {normalize_name(n): n for n in in_names}
    norm_out = {normalize_name(n): n for n in out_names}
    for key, in_name in norm_in.items():
        out_name = norm_out.get(key)
        if out_name is None:
            continue
        pairs[canonical_sensor_id(in_name)] = (in_name, out_name)
        used_in.add(in_name)
        used_out.add(out_name)

    # Tier 2: position-token match (e.g. CEER_ FL <-> MCIP_FL).
    for in_name in in_names:
        if in_name in used_in:
            continue
        pos = extract_position(in_name)
        if not pos:
            continue
        for out_name in out_names:
            if out_name in used_out:
                continue
            if extract_position(out_name) == pos:
                pairs[canonical_sensor_id(in_name)] = (in_name, out_name)
                used_in.add(in_name)
                used_out.add(out_name)
                break

    # Tier 3: fuzzy fallback for the leftovers.
    remaining_in = [n for n in in_names if n not in used_in]
    remaining_out = [n for n in out_names if n not in used_out]
    for in_name in remaining_in:
        best_out: Optional[str] = None
        best_ratio = fuzzy_threshold
        for out_name in remaining_out:
            ratio = fuzzy_ratio(in_name, out_name)
            if ratio > best_ratio:
                best_ratio = ratio
                best_out = out_name
        if best_out is not None:
            pairs[canonical_sensor_id(in_name)] = (in_name, best_out)
            used_in.add(in_name)
            used_out.add(best_out)
            remaining_out.remove(best_out)

    unmatched_in = [n for n in in_names if n not in used_in]
    unmatched_out = [n for n in out_names if n not in used_out]
    return pairs, unmatched_in, unmatched_out


def match_stream_pairs(
    in_names: List[str],
    out_names: List[str],
    fuzzy_threshold: float = 0.8,
) -> Tuple[Dict[str, Tuple[str, str]], List[str], List[str]]:
    """Pair stream group names between files, category-aware.

    Streams are matched by their semantic category first (``DETECTION``,
    ``ALIGNMENT``, ``HEADER``, ``CAPABILITY``, ``STATUS``, ``OTHER``) so
    producers may freely rename the payload prefix (``SRR_FL_DETECTION_001_004``
    vs ``FL_DETECTION_001_004``) without losing the pairing.  Within a category
    the numeric chunk (e.g. ``001_004``) is compared; if it is identical the
    streams pair directly, otherwise fuzzy matching is used.
    """
    pairs: Dict[str, Tuple[str, str]] = {}
    used_in: set = set()
    used_out: set = set()

    def _category(name: str) -> str:
        u = str(name or "").upper()
        for cat in ("DETECTION", "ALIGNMENT", "HEADER", "CAPABILITY", "STATUS"):
            if cat in u:
                return cat
        return "OTHER"

    def _chunk(name: str) -> str:
        digits = re.findall(r"\d{3}(?:_\d{3})?", str(name or ""))
        return digits[-1] if digits else ""

    for in_name in in_names:
        cat = _category(in_name)
        chunk = _chunk(in_name)
        key = f"{cat}_{chunk}" if chunk else cat
        for out_name in out_names:
            if out_name in used_out:
                continue
            if _category(out_name) != cat:
                continue
            if chunk and _chunk(out_name) == chunk:
                pairs[key] = (in_name, out_name)
                used_in.add(in_name)
                used_out.add(out_name)
                break
        else:
            for out_name in out_names:
                if out_name in used_out:
                    continue
                if _category(out_name) != cat:
                    continue
                if fuzzy_ratio(in_name, out_name) >= fuzzy_threshold:
                    pairs[key] = (in_name, out_name)
                    used_in.add(in_name)
                    used_out.add(out_name)
                    break

    unmatched_in = [n for n in in_names if n not in used_in]
    unmatched_out = [n for n in out_names if n not in used_out]
    return pairs, unmatched_in, unmatched_out
