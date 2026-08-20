"""Quantized hashmap detection matcher.

Purpose: count true positives between input and output detection sets using
epsilon-quantized multi-dimensional keys with +/-1 offset tolerance.
Inputs : per-scan input/output detection records.
Outputs: TP counts per scan and per signal.
"""

from itertools import product
from typing import Dict, List


MATCH_SIGNALS = ["DET_RANGE", "DET_RANGE_VELOCITY", "DET_AZIMUTH", "DET_ELEVATION"]
MATCH_EPSILON = 10.0


class DetectionMatcher:
    """Hashmap-based detection matcher with configurable quantization epsilon."""

    def __init__(self, epsilon: float = MATCH_EPSILON) -> None:
        """Purpose: configure quantization grid.
        Inputs : epsilon quantization step per signal unit.
        Outputs: matcher instance."""
        self.epsilon = float(epsilon)
        self._offsets_4d = sorted(
            list(product([-1, 0, 1], repeat=4)),
            key=lambda o: abs(o[0]) + abs(o[1]) + abs(o[2]) + abs(o[3]),
        )
        self._offsets_1d = [(-1,), (0,), (1,)]

    def match_scan(
        self, in_candidates: List[Dict[str, float]], out_candidates: List[Dict[str, float]]
    ) -> Dict[str, float]:
        """Purpose: compute TP / precision / recall / F1 / accuracy for one scan.
        Inputs : input and output detection record lists.
        Outputs: metric dict (tp, fp, fn, precision, recall, f1, accuracy)."""
        tp_all = self._match_4d(in_candidates, out_candidates)
        in_n = len(in_candidates)
        out_n = len(out_candidates)
        fp = max(0, out_n - tp_all)
        fn = max(0, in_n - tp_all)
        prec = tp_all / (tp_all + fp) if (tp_all + fp) > 0 else 0.0
        rec = tp_all / (tp_all + fn) if (tp_all + fn) > 0 else 0.0
        f1 = (2.0 * prec * rec / (prec + rec)) if (prec + rec) > 0 else 0.0
        acc = tp_all / (tp_all + fp + fn) if (tp_all + fp + fn) > 0 else 0.0
        return {
            "tp": tp_all,
            "fp": fp,
            "fn": fn,
            "precision": prec,
            "recall": rec,
            "f1": f1,
            "accuracy": acc,
        }

    def match_signal_1d(
        self, in_vals: List[float], out_vals: List[float]
    ) -> int:
        """Purpose: TP count for a single signal with +/-1 bucket tolerance.
        Inputs : input and output value lists.
        Outputs: number of matched detections."""
        out_map: Dict[tuple[int], int] = {}
        for v in out_vals:
            key = (self._quantize(v),)
            out_map[key] = out_map.get(key, 0) + 1
        matches = 0
        for v in in_vals:
            base = self._quantize(v)
            for off in self._offsets_1d:
                key = (base + off[0],)
                cnt = out_map.get(key, 0)
                if cnt > 0:
                    matches += 1
                    if cnt == 1:
                        del out_map[key]
                    else:
                        out_map[key] = cnt - 1
                    break
        return matches

    def _match_4d(
        self,
        in_candidates: List[Dict[str, float]],
        out_candidates: List[Dict[str, float]],
    ) -> int:
        """Purpose: TP count over the 4D detection key (range/vel/az/el).
        Inputs : input and output detection record lists.
        Outputs: matched count."""
        out_map: Dict[tuple[int, int, int, int], int] = {}
        for row in out_candidates:
            key = self._key4(row)
            out_map[key] = out_map.get(key, 0) + 1
        matches = 0
        for row in in_candidates:
            base = self._key4(row)
            for off in self._offsets_4d:
                key = (
                    base[0] + off[0],
                    base[1] + off[1],
                    base[2] + off[2],
                    base[3] + off[3],
                )
                cnt = out_map.get(key, 0)
                if cnt > 0:
                    matches += 1
                    if cnt == 1:
                        del out_map[key]
                    else:
                        out_map[key] = cnt - 1
                    break
        return matches

    def _key4(self, row: Dict[str, float]) -> tuple[int, int, int, int]:
        """Purpose: build the quantized 4D hash key for a record.
        Inputs : detection record.
        Outputs: quantized key tuple."""
        return (
            self._quantize(row["DET_RANGE"]),
            self._quantize(row["DET_RANGE_VELOCITY"]),
            self._quantize(row["DET_AZIMUTH"]),
            self._quantize(row["DET_ELEVATION"]),
        )

    def _quantize(self, value: float) -> int:
        """Purpose: map a raw value onto the epsilon grid.
        Inputs : raw value.
        Outputs: integer grid coordinate."""
        return int(round(float(value) / self.epsilon))