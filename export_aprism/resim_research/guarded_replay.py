"""Guarded error-transfer algebra for certified replay (N28).

S=(a,b,h): e->a e+b valid on [0,h). Composition (S2 after S1):
a=a2 a1, b=a2 b1+b2, h=min(h1,(h2-b1)/a1) for a1>0; a1=0 constant-guard;
empty absorbs. Associative over exact reals; fixed-point needs
upward (a,b)/downward (h) enclosure + canonical tree.
# ponytail: exact rational fixtures, margin demo, rounding counterexample.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from fractions import Fraction as Q

import numpy as np


def compose(s2, s1):
    """Compose guarded summaries: S2 after S1. None/empty handling."""
    if s1 is None or s2 is None:
        return None
    a1, b1, h1 = s1
    a2, b2, h2 = s2
    a, b = a2 * a1, a2 * b1 + b2
    if a1 > 0:
        h = min(h1, (h2 - b1) / a1)
        h = max(h, 0.0) if isinstance(h, float) else max(h, 0)
    else:
        h = h1 if b1 < h2 else (0.0 if isinstance(h1, float) else 0)
    return (a, b, h)


def demo() -> None:
    # 1. exact composition fixture
    S1 = (0.5, 0.01, 0.2)
    S2 = (2.0, 0.05, 0.1)
    fwd = compose(S2, S1)
    rev = compose(S1, S2)
    print(f"[compose] S2oS1={fwd} (expect (1,0.07,0.18))")
    print(f"[compose] S1oS2={rev} (expect (1,0.035,0.075))")
    assert fwd is not None and rev is not None, "empty certificate"
    af, bf, hf = fwd
    assert abs(af - 1.0) < 1e-12 and abs(bf - 0.07) < 1e-12 and abs(hf - 0.18) < 1e-12
    ar, br, hr = rev
    assert abs(ar - 1.0) < 1e-12 and abs(br - 0.035) < 1e-12 and abs(hr - 0.075) < 1e-12
    # strict-guard endpoint: e=0.18 gives output 0.1 = h2 -> invalid (strict <)
    assert 0.5 * 0.18 + 0.01 >= 0.1 - 1e-9, "endpoint excluded"
    # a1=0 branches
    c_pass = compose((0.5, 0.01, 0.2), (0.0, 0.03, 0.2))
    c_kill = compose((0.5, 0.01, 0.2), (0.0, 0.25, 0.2))
    assert c_pass is not None and c_kill is not None, "empty const"
    assert c_pass[2] == 0.2, "const pass"
    assert c_kill[2] == 0.0, "const kill"

    # 2. associativity over exact rationals
    rng = np.random.default_rng(4)
    for _ in range(300):
        vals = [rng.choice([0, 0.5, 1, 2]) for _ in range(3)]
        S = [(Q(v).limit_denominator(), Q(rng.uniform(0, 0.1)).limit_denominator(),
              Q(rng.uniform(0.05, 0.3)).limit_denominator()) for v in vals]
        # rational compose
        def qc(s2, s1):
            if s1 is None or s2 is None:
                return None
            a1, b1, h1 = s1
            a2, b2, h2 = s2
            a, b = a2 * a1, a2 * b1 + b2
            if a1 > 0:
                h = min(h1, (h2 - b1) / a1)
                h = max(h, Q(0))
            else:
                h = h1 if b1 < h2 else Q(0)
            return (a, b, h)
        l = qc(S[2], qc(S[1], S[0]))
        r = qc(qc(S[2], S[1]), S[0])
        assert l == r, f"assoc {l} vs {r}"
    print("[assoc] 300 rational triples identical")

    # 3. margin fixture: winner preserved inside radius, flips outside
    # costs c1=(x-1)^2, c2=(x+1)^2, ref x*=0.5 -> gap 2, C1=2, C2=4 on |x-0.5|<=0.5
    m, C1, C2 = 2.0, 2.0, 4.0
    h = m / (C1 + C2)
    print(f"[margin] certified e<{h:.4f} (expect 1/3)")
    assert abs(h - 1.0 / 3.0) < 1e-12, "radius"
    for e, same in ((0.30, True), (0.60, False)):
        x = 0.5 - e  # perturb toward boundary x=0
        assert ((x - 1) ** 2 < (x + 1) ** 2) == same, "branch behavior"

    # 4. fixed-point rounding is NOT associative (counterexample documented)
    # toy decimal q2: round-down h after each merge vs once
    h_true = ((0.3 - 0.1) / 0.7 - 0.05) / 0.6
    h_step = np.floor(((0.3 - 0.1) / 0.7) * 100) / 100
    h_step = np.floor((h_step - 0.05) / 0.6 * 100) / 100
    h_once = np.floor(h_true * 100) / 100
    print(f"[fixedpt] stepwise h={h_step:.4f} once h={h_once:.4f} (may differ -> canonical tree)")
    assert h_step <= h_true + 1e-12 and h_once <= h_true + 1e-12, "sound (under)"
    print("demo PASS")


if __name__ == "__main__":
    demo()
