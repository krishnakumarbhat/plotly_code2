"""
view_settings.py
================
Central store for all user-configurable display settings used by
PlanViewWidget and the main window themes.

Everything is stored as plain Python attributes so it is trivially
serialisable / copyable.  A single global instance ``SETTINGS`` is
imported by every module that needs it.
"""

from __future__ import annotations
from copy import deepcopy
from dataclasses import dataclass, field
from typing import Dict, Tuple

# ---------------------------------------------------------------------------
# Colour / style type aliases
# ---------------------------------------------------------------------------
# Style = (hex_colour, edge_linewidth, face_alpha)
StyleTuple = Tuple[str, float, float]


# ---------------------------------------------------------------------------
# Default DynProperty styles  (matches original plan_view_widget constants)
# ---------------------------------------------------------------------------
_DEFAULT_DYN: Dict[int, StyleTuple] = {
    0: ("#f44336", 1.5, 0.20),   # Invalid
    1: ("#9e9e9e", 1.5, 0.22),   # Stationary
    2: ("#2196f3", 1.5, 0.22),   # Stopped  → blue
    3: ("#4caf50", 3.0, 0.25),   # Moving
    6: ("#f44336", 1.5, 0.20),   # Error
    7: ("#607d8b", 1.5, 0.20),   # Not available
}
_DEFAULT_DYN_FALLBACK: StyleTuple = ("#4fc3f7", 1.5, 0.20)

_DYN_LABELS: Dict[int, str] = {
    0: "Invalid",
    1: "Stationary",
    2: "Stopped",
    3: "Moving",
    6: "Error",
    7: "Not available",
}

# ---------------------------------------------------------------------------
# Default ClassMostProb styles
# ---------------------------------------------------------------------------
_DEFAULT_CLASS: Dict[int, StyleTuple] = {
    # Ordering a (lightest/most transparent) → f (darkest/least transparent)
    0: ("#9e9e9e", 1.0, 0.15),   # Unknown        → a: lightest, most transparent
    3: ("#795548", 1.3, 0.22),   # Truck          → b
    1: ("#2196f3", 1.6, 0.28),   # Car            → c
    2: ("#ff9800", 1.9, 0.33),   # Motorbike      → d
    4: ("#76ff03", 2.2, 0.40),   # Bicycle        → e
    5: ("#e91e63", 2.5, 0.48),   # Pedestrian     → f: darkest, least transparent
    6: ("#9e9e9e", 1.5, 0.20),   # Error          → grey (both sources)
    7: ("#e0e0e0", 1.5, 0.15),   # Not available  → light grey (both sources)
}

CLASS_LABELS: Dict[int, str] = {
    0: "Unknown",
    1: "Car",
    2: "Motorbike",
    3: "Truck",
    4: "Bicycle",
    5: "Pedestrian",
    6: "Error",
    7: "Not available",
}

# ---------------------------------------------------------------------------
# Default velocity-vector style  (colour, alpha)
# ---------------------------------------------------------------------------
_DEFAULT_VEL_MOVING: Tuple[str, float]      = ("#ffeb3b", 1.0)   # yellow
_DEFAULT_VEL_STATIONARY: Tuple[str, float]  = ("#9e9e9e", 1.0)   # grey

# ---------------------------------------------------------------------------
# Default ID label style  (colour, alpha)
# ---------------------------------------------------------------------------
_DEFAULT_ID_COLOUR: str   = "white"
_DEFAULT_ID_ALPHA:  float = 0.75


@dataclass
class ViewSettings:
    """Holds all configurable display settings.  Deep-copy to snapshot."""

    # --- DynProperty box styles ---
    dyn_styles: Dict[int, StyleTuple] = field(
        default_factory=lambda: deepcopy(_DEFAULT_DYN))
    dyn_fallback: StyleTuple = field(
        default_factory=lambda: _DEFAULT_DYN_FALLBACK)

    # --- Class colour mode ---
    class_color_mode: bool = True
    class_styles: Dict[int, StyleTuple] = field(
        default_factory=lambda: deepcopy(_DEFAULT_CLASS))

    # --- Velocity vector ---
    vel_moving_colour: str   = _DEFAULT_VEL_MOVING[0]
    vel_moving_alpha:  float = _DEFAULT_VEL_MOVING[1]
    vel_stationary_colour: str   = _DEFAULT_VEL_STATIONARY[0]
    vel_stationary_alpha:  float = _DEFAULT_VEL_STATIONARY[1]

    # --- ID label ---
    id_colour: str   = _DEFAULT_ID_COLOUR
    id_alpha:  float = _DEFAULT_ID_ALPHA

    # --- Theme ---
    theme: str = "dark"   # "dark" | "light"

    def reset(self) -> None:
        """Restore every setting to its factory default."""
        self.dyn_styles       = deepcopy(_DEFAULT_DYN)
        self.dyn_fallback     = _DEFAULT_DYN_FALLBACK
        self.class_color_mode = True
        self.class_styles     = deepcopy(_DEFAULT_CLASS)
        self.vel_moving_colour      = _DEFAULT_VEL_MOVING[0]
        self.vel_moving_alpha       = _DEFAULT_VEL_MOVING[1]
        self.vel_stationary_colour  = _DEFAULT_VEL_STATIONARY[0]
        self.vel_stationary_alpha   = _DEFAULT_VEL_STATIONARY[1]
        self.id_colour  = _DEFAULT_ID_COLOUR
        self.id_alpha   = _DEFAULT_ID_ALPHA
        self.theme      = "dark"

    def box_style(self, dyn_property: int, class_id: int) -> StyleTuple:
        """Return the (colour, linewidth, alpha) for a bounding box."""
        if self.class_color_mode:
            return self.class_styles.get(class_id, self.dyn_fallback)
        return self.dyn_styles.get(dyn_property, self.dyn_fallback)

    def vel_style(self, dyn_property: int) -> Tuple[str, float]:
        """Return (colour, alpha) for a velocity arrow."""
        if dyn_property == 1:   # Stationary
            return self.vel_stationary_colour, self.vel_stationary_alpha
        return self.vel_moving_colour, self.vel_moving_alpha


# ---------------------------------------------------------------------------
# Module-level singleton – import this everywhere
# ---------------------------------------------------------------------------
SETTINGS = ViewSettings()
