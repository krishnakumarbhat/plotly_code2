#!/usr/bin/env python3
"""
CoordinateConverter_qt.py

Single-file PyQt5 tool for editing sensor mounting positions in a LiME
`modelconfig` YAML.

Workflow:
  1. Select a modelconfig YAML and parse it.
  2. The tool discovers every `ModelType: Sensor` (3, 5, ... whatever exists)
     and shows one editable input row per sensor.
  3. Each row is pre-filled with the values currently in the file and is
     interpreted as VCS input (X, Y, Z in metres; Roll, Pitch, Yaw in radians).
  4. On "Convert to OSI & Save", the VCS values are converted to OSI and
     written back to the same file (a `<file>.bak` backup is created first).

VCS -> OSI conversion (inverse of STLA_Small_IFV600_Converter.cpp OsiToVcs).
The VCS origin and Y polarity are selectable in the GUI. The origin is a 3D
point expressed in OSI (bbox-center) coordinates:
    Front Bumper Center -> (Length/2, 0, -Height/2)
    Front Axle Center   -> Host.BbCenterToFront.(X, Y, Z)   (editable)
    Rear Axle Center    -> Host.BbCenterToRear.(X, Y, Z)    (editable)
Then, per field:
    osi_x   =  vcs_x + origin_x
    osi_y   = (+vcs_y if Left +Y else -vcs_y) + origin_y
    osi_z   =  vcs_z + origin_z
    osi_yaw = +vcs_yaw (Left +Y) or -vcs_yaw (Right +Y)
    roll, pitch left unchanged

Usage:
    python CoordinateConverter_qt.py
"""

from __future__ import annotations

import os
import re
import sys
from dataclasses import dataclass, field
from typing import Optional

from PyQt5.QtCore import Qt
from PyQt5.QtGui import QDoubleValidator, QFont, QIcon, QPixmap
from PyQt5.QtWidgets import (
    QApplication,
    QComboBox,
    QFileDialog,
    QFrame,
    QHBoxLayout,
    QHeaderView,
    QLabel,
    QLineEdit,
    QMessageBox,
    QPlainTextEdit,
    QPushButton,
    QStatusBar,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QWidget,
)

# =========================================================================== #
# Conversion / parse core (text-based, formatting-preserving)
# =========================================================================== #

# Variable-name suffixes we read / write inside each Sensor model block.
POS_X = "SRR.SM2.SensorMountingPosition.Position.X_m"
POS_Y = "SRR.SM2.SensorMountingPosition.Position.Y_m"
POS_Z = "SRR.SM2.SensorMountingPosition.Position.Z_m"
ORI_ROLL = "SRR.SM2.SensorMountingPosition.Orientation.Roll_rad"
ORI_PITCH = "SRR.SM2.SensorMountingPosition.Orientation.Pitch_rad"
ORI_YAW = "SRR.SM2.SensorMountingPosition.Orientation.Yaw_rad"
HOST_LENGTH = "SRR.SM2.Host.Length_m"
HOST_WIDTH = "SRR.SM2.Host.Width_m"
HOST_HEIGHT = "SRR.SM2.Host.Height_m"
HOST_BBC_TO_REAR_X = "SRR.SM2.Host.BbCenterToRear.X_m"
HOST_BBC_TO_REAR_Y = "SRR.SM2.Host.BbCenterToRear.Y_m"
HOST_BBC_TO_REAR_Z = "SRR.SM2.Host.BbCenterToRear.Z_m"
HOST_BBC_TO_FRONT_X = "SRR.SM2.Host.BbCenterToFront.X_m"
HOST_BBC_TO_FRONT_Y = "SRR.SM2.Host.BbCenterToFront.Y_m"
HOST_BBC_TO_FRONT_Z = "SRR.SM2.Host.BbCenterToFront.Z_m"
SENSOR_NAME = "SRR.SM2.SensorName"

# Bb-center-to-axle offset field groups (component label -> YAML variable name).
BBC_REAR_FIELDS: dict[str, str] = {
    "X": HOST_BBC_TO_REAR_X,
    "Y": HOST_BBC_TO_REAR_Y,
    "Z": HOST_BBC_TO_REAR_Z,
}
BBC_FRONT_FIELDS: dict[str, str] = {
    "X": HOST_BBC_TO_FRONT_X,
    "Y": HOST_BBC_TO_FRONT_Y,
    "Z": HOST_BBC_TO_FRONT_Z,
}

# --- Input Coordinate System options (GUI dropdowns) --- #
# Dropdown 1: which point is the VCS X-origin.
VCS_ORIGIN_FRONT_BUMPER = "Front Bumper Center"
VCS_ORIGIN_REAR_AXLE = "Rear Axle Center"
VCS_ORIGIN_FRONT_AXLE = "Front Axle Center"
VCS_ORIGIN_OPTIONS = [
    VCS_ORIGIN_FRONT_BUMPER,
    VCS_ORIGIN_REAR_AXLE,
    VCS_ORIGIN_FRONT_AXLE,
]

# Dropdown 2: which lateral side is +Y.
Y_POS_LEFT = "Left side +ve Y"
Y_POS_RIGHT = "Right side +ve Y"
Y_OPTIONS = [Y_POS_LEFT, Y_POS_RIGHT]

# Host geometry fields (label -> YAML variable name), editable in the GUI.
HOST_FIELDS: dict[str, str] = {
    "Length": HOST_LENGTH,
    "Width": HOST_WIDTH,
    "Height": HOST_HEIGHT,
}

# Ordered mounting fields (label -> YAML variable name) exposed for editing.
MOUNT_FIELDS: dict[str, str] = {
    "X": POS_X,
    "Y": POS_Y,
    "Z": POS_Z,
    "Roll": ORI_ROLL,
    "Pitch": ORI_PITCH,
    "Yaw": ORI_YAW,
}

# Units per field, for display.
FIELD_UNITS: dict[str, str] = {
    "X": "m", "Y": "m", "Z": "m",
    "Roll": "rad", "Pitch": "rad", "Yaw": "rad",
}

# Matches a numeric literal (int / float / scientific).
_NUM = r"[-+]?\d*\.?\d+(?:[eE][-+]?\d+)?"

# Matches the top-level list item `    - Model:` (4-space indent, list dash).
_MODEL_RE = re.compile(r"^\s*-\s*Model:\s*(\S+)")


class ConversionError(Exception):
    """Raised when the input file cannot be interpreted as a modelconfig."""


@dataclass
class SensorInfo:
    """Parsed state of a single Sensor model, used to build the input form."""
    model_id: str
    sensor_name: str
    host_length: float
    host_width: float
    host_height: float
    # Current values read from the file, keyed by MOUNT_FIELDS label.
    values: dict = field(default_factory=dict)
    # Which fields are actually present in the file (writable).
    present: set = field(default_factory=set)
    # Bb-center-to-axle offsets read from the file ({X,Y,Z} -> float).
    bbc_rear: dict = field(default_factory=dict)
    bbc_front: dict = field(default_factory=dict)


@dataclass
class SensorConversion:
    """Result record for a single converted Sensor model, used for the log."""
    model_id: str
    sensor_name: str = "?"
    host_length: float = 0.0
    host_width: float = 0.0
    host_height: float = 0.0
    osi: dict = field(default_factory=dict)
    vcs: dict = field(default_factory=dict)


def _fmt(value: float) -> str:
    """Format a float back into a compact YAML-friendly literal."""
    value = round(value, 6)
    if value == 0.0:
        value = 0.0  # normalise -0.0 -> 0.0
    text = f"{value:.6f}".rstrip("0").rstrip(".")
    if text in ("", "-", "-0"):
        text = "0"
    if "." not in text:
        text += ".0"
    return text


def _find_var_line(lines: list[str], start: int, end: int, name: str) -> Optional[int]:
    """Return the index of the ModelVariables line declaring `name`, or None."""
    pattern = re.compile(r"Name:\s*" + re.escape(name) + r"\b")
    for i in range(start, end):
        if pattern.search(lines[i]):
            return i
    return None


def _read_start(line: str) -> Optional[str]:
    """Extract the raw `Start:` value text from a ModelVariables line."""
    m = re.search(r"Start:\s*(" + _NUM + r")", line)
    return m.group(1) if m else None


def _replace_start(line: str, new_value: str) -> str:
    """Replace the numeric `Start:` value on a ModelVariables line, keep the rest."""
    return re.sub(r"(Start:\s*)" + _NUM, r"\g<1>" + new_value, line, count=1)


def _read_string_start(line: str) -> str:
    """Extract a String-typed `Start:` value (e.g. sensor name)."""
    m = re.search(r"Start:\s*([^,}\r\n]+)", line)
    return m.group(1).strip() if m else "?"


def parse_sensors(text: str) -> list[SensorInfo]:
    """
    Discover every `ModelType: Sensor` block and read its mounting values.

    Returns one SensorInfo per sensor, in file order. Raises ConversionError
    if the file has no `- Model:` entries or no Sensor models.
    """
    lines = text.splitlines(keepends=True)

    model_starts = [i for i, ln in enumerate(lines) if _MODEL_RE.match(ln)]
    if not model_starts:
        raise ConversionError("No `- Model:` entries found under ModelList.")
    model_bounds = list(zip(model_starts, model_starts[1:] + [len(lines)]))

    sensors: list[SensorInfo] = []
    for start, end in model_bounds:
        model_id = _MODEL_RE.match(lines[start]).group(1)
        block = "".join(lines[start:end])
        if not re.search(r"ModelType:\s*Sensor\b", block):
            continue

        lname = _find_var_line(lines, start, end, SENSOR_NAME)
        llen = _find_var_line(lines, start, end, HOST_LENGTH)
        lwid = _find_var_line(lines, start, end, HOST_WIDTH)
        lhgt = _find_var_line(lines, start, end, HOST_HEIGHT)
        if llen is None or lhgt is None:
            raise ConversionError(
                f"Model {model_id} (Sensor) is missing Host.Length_m / Host.Height_m."
            )

        try:
            host_length = float(_read_start(lines[llen]))
            host_width = float(_read_start(lines[lwid])) if lwid is not None else 0.0
            host_height = float(_read_start(lines[lhgt]))
        except (TypeError, ValueError) as exc:
            raise ConversionError(
                f"Model {model_id}: could not parse Host geometry ({exc})."
            ) from exc

        info = SensorInfo(
            model_id=model_id,
            sensor_name=_read_string_start(lines[lname]) if lname is not None else "?",
            host_length=host_length,
            host_width=host_width,
            host_height=host_height,
        )

        # Read Bb-center-to-axle reference offsets (used by axle-center origins).
        for comp, name in BBC_REAR_FIELDS.items():
            li = _find_var_line(lines, start, end, name)
            if li is not None:
                raw = _read_start(lines[li])
                if raw is not None:
                    try:
                        info.bbc_rear[comp] = float(raw)
                    except ValueError:
                        pass
        for comp, name in BBC_FRONT_FIELDS.items():
            li = _find_var_line(lines, start, end, name)
            if li is not None:
                raw = _read_start(lines[li])
                if raw is not None:
                    try:
                        info.bbc_front[comp] = float(raw)
                    except ValueError:
                        pass

        for label, var_name in MOUNT_FIELDS.items():
            li = _find_var_line(lines, start, end, var_name)
            if li is None:
                continue
            raw = _read_start(lines[li])
            if raw is None:
                continue
            try:
                info.values[label] = float(raw)
                info.present.add(label)
            except ValueError:
                continue

        # Require the core position fields to be present and parseable.
        missing = [f for f in ("X", "Y", "Z", "Yaw") if f not in info.present]
        if missing:
            raise ConversionError(
                f"Model {model_id} (Sensor) is missing mounting fields: "
                + ", ".join(missing)
            )

        sensors.append(info)

    if not sensors:
        raise ConversionError("No `ModelType: Sensor` models found.")

    return sensors


def _resolve_origin(
    lines: list[str], start: int, end: int, vcs_origin: str, model_id: str,
    host_length: float, host_height: float, origin_params: Optional[dict],
) -> tuple[float, float, float]:
    """Return (ox, oy, oz): OSI coords of the selected VCS origin.

    For axle origins, `origin_params` (edited {X,Y,Z}) is written back to the
    matching Host.BbCenterTo* lines and used as the origin. When a component is
    not supplied, the file's current value is used.
    """
    if vcs_origin == VCS_ORIGIN_FRONT_BUMPER:
        return (host_length / 2.0, 0.0, -host_height / 2.0)

    if vcs_origin == VCS_ORIGIN_FRONT_AXLE:
        fields, label = BBC_FRONT_FIELDS, "BbCenterToFront"
    elif vcs_origin == VCS_ORIGIN_REAR_AXLE:
        fields, label = BBC_REAR_FIELDS, "BbCenterToRear"
    else:
        raise ConversionError(f"Unknown VCS origin option: {vcs_origin!r}")

    comp_vals: dict[str, float] = {}
    for comp in ("X", "Y", "Z"):
        li = _find_var_line(lines, start, end, fields[comp])
        if origin_params is not None and comp in origin_params:
            value = float(origin_params[comp])
            if li is not None:
                lines[li] = _replace_start(lines[li], _fmt(value))
        elif li is not None:
            value = float(_read_start(lines[li]))
        elif comp == "X":
            raise ConversionError(
                f"Model {model_id}: Host.{label}.X_m not found; "
                f"cannot use '{vcs_origin}' origin."
            )
        else:
            value = 0.0
        comp_vals[comp] = value

    return (comp_vals["X"], comp_vals["Y"], comp_vals["Z"])


def vcs_to_osi_value(field_label: str, vcs_value: float, *,
                     origin: tuple[float, float, float],
                     y_positive_left: bool) -> float:
    """Inverse of the OSI->VCS transform for a single mounting field.

    `origin` is the OSI (ox, oy, oz) of the VCS origin (see _resolve_origin).
    `y_positive_left` True keeps Y/Yaw sign (VCS is Y-left, same as OSI);
    False negates them (VCS is Y-right).
    """
    origin_x, origin_y, origin_z = origin
    if field_label == "X":
        return vcs_value + origin_x
    if field_label == "Y":
        return (vcs_value if y_positive_left else -vcs_value) + origin_y
    if field_label == "Z":
        return vcs_value + origin_z
    if field_label == "Yaw":
        return vcs_value if y_positive_left else -vcs_value
    # Roll / Pitch are not transformed between OSI and VCS.
    return vcs_value


def apply_vcs_to_text(
    text: str,
    vcs_by_model: dict[str, dict],
    host_params: Optional[dict] = None,
    vcs_origin: str = VCS_ORIGIN_FRONT_BUMPER,
    y_positive_left: bool = False,
    origin_params: Optional[dict] = None,
) -> tuple[str, list[SensorConversion]]:
    """
    Convert user-supplied VCS mounting values to OSI and write them into `text`.

    `vcs_by_model` maps a model_id to a dict of {field_label: vcs_value} for the
    fields the user actually filled in. Only those fields are written; a field
    the user left blank keeps its existing value in the file.

    `host_params` (optional) is a dict {"Length", "Width", "Height"} that, when
    given, overrides the host geometry of *every* Sensor model in the file
    (written back to the Host.* variables) and is used for the VCS->OSI
    transform. When None, each sensor's existing host geometry is used.

    `vcs_origin` selects the VCS origin (see VCS_ORIGIN_OPTIONS).
    `origin_params` (optional) is a dict {"X","Y","Z"} of the axle-center offset
    used when `vcs_origin` is an axle option; it is written back to the matching
    Host.BbCenterTo* variables of every Sensor model.
    `y_positive_left` True means the VCS input uses Y-left (no Y/Yaw negation);
    False means Y-right (Y/Yaw negated), matching the C++ reference.
    """
    lines = text.splitlines(keepends=True)

    model_starts = [i for i, ln in enumerate(lines) if _MODEL_RE.match(ln)]
    if not model_starts:
        raise ConversionError("No `- Model:` entries found under ModelList.")
    model_bounds = list(zip(model_starts, model_starts[1:] + [len(lines)]))

    results: list[SensorConversion] = []
    for start, end in model_bounds:
        model_id = _MODEL_RE.match(lines[start]).group(1)
        block = "".join(lines[start:end])
        if not re.search(r"ModelType:\s*Sensor\b", block):
            continue

        lname = _find_var_line(lines, start, end, SENSOR_NAME)
        llen = _find_var_line(lines, start, end, HOST_LENGTH)
        lwid = _find_var_line(lines, start, end, HOST_WIDTH)
        lhgt = _find_var_line(lines, start, end, HOST_HEIGHT)
        if llen is None or lhgt is None:
            raise ConversionError(
                f"Model {model_id} (Sensor) is missing Host.Length_m / Host.Height_m."
            )

        # Host geometry: override from host_params (and write back) or read file.
        if host_params is not None:
            host_length = float(host_params["Length"])
            host_width = float(host_params["Width"])
            host_height = float(host_params["Height"])
            lines[llen] = _replace_start(lines[llen], _fmt(host_length))
            if lwid is not None:
                lines[lwid] = _replace_start(lines[lwid], _fmt(host_width))
            lines[lhgt] = _replace_start(lines[lhgt], _fmt(host_height))
        else:
            host_length = float(_read_start(lines[llen]))
            host_width = float(_read_start(lines[lwid])) if lwid is not None else 0.0
            host_height = float(_read_start(lines[lhgt]))

        # Resolve the VCS origin (OSI x/y/z of the origin) for this sensor;
        # axle offsets are written back when origin_params is provided.
        origin = _resolve_origin(
            lines, start, end, vcs_origin, model_id,
            host_length, host_height, origin_params,
        )

        # Mounting fields: only those the user provided for this model.
        vcs_vals = vcs_by_model.get(model_id, {})
        osi_out: dict = {}
        vcs_out: dict = {}
        for label, var_name in MOUNT_FIELDS.items():
            if label not in vcs_vals:
                continue
            li = _find_var_line(lines, start, end, var_name)
            if li is None:
                continue
            osi_val = vcs_to_osi_value(
                label, vcs_vals[label],
                origin=origin, y_positive_left=y_positive_left,
            )
            lines[li] = _replace_start(lines[li], _fmt(osi_val))
            osi_out[label] = osi_val
            vcs_out[label] = vcs_vals[label]

        if osi_out or host_params is not None or origin_params is not None:
            results.append(SensorConversion(
                model_id=model_id,
                sensor_name=_read_string_start(lines[lname]) if lname is not None else "?",
                host_length=host_length,
                host_width=host_width,
                host_height=host_height,
                osi=osi_out,
                vcs=vcs_out,
            ))

    if not results:
        raise ConversionError("Nothing to write: no Sensor models were updated.")

    return "".join(lines), results


def apply_vcs_to_file(
    input_path: str,
    vcs_by_model: dict[str, dict],
    output_path: Optional[str] = None,
    make_backup: bool = True,
    host_params: Optional[dict] = None,
    vcs_origin: str = VCS_ORIGIN_FRONT_BUMPER,
    y_positive_left: bool = False,
    origin_params: Optional[dict] = None,
) -> list[SensorConversion]:
    """
    Read `input_path`, convert VCS mounting values to OSI, and write the result.

    Writes to `output_path` (defaults to `input_path`, i.e. in-place). When
    writing in place and `make_backup` is True, a `<input>.bak` copy of the
    original is created first. `host_params` overrides host geometry,
    `origin_params` overrides the axle-center offset, and `vcs_origin` /
    `y_positive_left` select the VCS convention (see apply_vcs_to_text).
    """
    with open(input_path, "r", encoding="utf-8") as fh:
        text = fh.read()

    converted, results = apply_vcs_to_text(
        text, vcs_by_model, host_params, vcs_origin, y_positive_left, origin_params
    )

    target = output_path or input_path
    if make_backup and os.path.abspath(target) == os.path.abspath(input_path):
        with open(input_path + ".bak", "w", encoding="utf-8", newline="") as bak:
            bak.write(text)

    with open(target, "w", encoding="utf-8", newline="") as fh:
        fh.write(converted)
    return results


# def format_apply_summary(conversions: list[SensorConversion]) -> str:
#     """Human-readable summary of a VCS -> OSI apply operation."""
#     out: list[str] = []
#     for c in conversions:
#         out.append(f"Model {c.model_id}  [{c.sensor_name}]")
#         out.append(
#             f"    host: length={c.host_length:g} m  width={c.host_width:g} m  "
#             f"height={c.host_height:g} m"
#         )
#         if c.vcs:
#             out.append("    VCS (input)  ->  OSI (written)")
#             for label in MOUNT_FIELDS:
#                 if label in c.vcs:
#                     unit = FIELD_UNITS[label]
#                     out.append(
#                         f"    {label:<5}: {c.vcs[label]:+.5f}  ->  "
#                         f"{c.osi[label]:+.5f}  {unit}"
#                     )
#         else:
#             out.append("    (host geometry updated; mounting left unchanged)")
#         out.append("")
#     out.append(f"Updated {len(conversions)} Sensor model(s).")
#     return "\n".join(out)


# def format_detailed_conversions(conversions: list[SensorConversion]) -> str:
#     """More verbose per-field listing of the VCS input and calculated OSI output."""
#     out: list[str] = []
#     out.append("Detailed per-field VCS -> OSI values:")
#     for c in conversions:
#         out.append(f"Model {c.model_id}  [{c.sensor_name}]")
#         if c.vcs:
#             for label in MOUNT_FIELDS:
#                 if label in c.vcs:
#                     unit = FIELD_UNITS.get(label, "")
#                     vcs_val = c.vcs[label]
#                     osi_val = c.osi.get(label)
#                     if osi_val is not None:
#                         out.append(
#                             f"    {label:<5}: VCS={vcs_val:+.6f} {unit}  ->  OSI={osi_val:+.6f} {unit}"
#                         )
#                     else:
#                         out.append(
#                             f"    {label:<5}: VCS={vcs_val:+.6f} {unit}  ->  OSI=(n/a)"
#                         )
#         else:
#             out.append("    (host geometry updated; mounting left unchanged)")
#         out.append("")
#     return "\n".join(out)

def format_apply_summary(conversions: list[SensorConversion]) -> str:
    """Human-readable summary of a VCS -> OSI apply operation."""
    out: list[str] = []
    for c in conversions:
        out.append(f"Model {c.model_id}  [{c.sensor_name}]")
        out.append(
            f"    host: length={c.host_length:g} m  width={c.host_width:g} m  "
            f"height={c.host_height:g} m"
        )
        if c.vcs:
            out.append("    Vehicle Reference (Input)  ->  OSI Coordinate System (Exported)")
            for label in MOUNT_FIELDS:
                if label in c.vcs:
                    unit = FIELD_UNITS[label]
                    out.append(
                        f"    {label:<5}: {c.vcs[label]:+.5f}  ->  "
                        f"{c.osi[label]:+.5f}  {unit}"
                    )
        else:
            out.append("    (host geometry updated; mounting left unchanged)")
        out.append("")
    out.append(f"Successfully processed {len(conversions)} sensor(s).")
    return "\n".join(out)


def format_detailed_conversions(conversions: list[SensorConversion]) -> str:
    """More verbose per-field listing of the VCS input and calculated OSI output."""
    out: list[str] = []
    out.append("Detailed structural coordinate logs:")
    for c in conversions:
        out.append(f"Model {c.model_id}  [{c.sensor_name}]")
        if c.vcs:
            for label in MOUNT_FIELDS:
                if label in c.vcs:
                    unit = FIELD_UNITS.get(label, "")
                    vcs_val = c.vcs[label]
                    osi_val = c.osi.get(label)
                    if osi_val is not None:
                        out.append(
                            f"    {label:<5}: Vehicle Local={vcs_val:+.6f} {unit}  ->  OSI={osi_val:+.6f} {unit}"
                        )
        out.append("")
    return "\n".join(out)



# =========================================================================== #
# PyQt5 GUI
# =========================================================================== #

# Editable field labels in display order.
FIELDS = list(MOUNT_FIELDS.keys())  # X, Y, Z, Roll, Pitch, Yaw


class ConverterWindow(QWidget):
    """Main window: parse a modelconfig, edit VCS mounting, write back as OSI."""

    def __init__(self) -> None:
        super().__init__()
        self.setWindowTitle("VCS \u2192 OSI Mounting Editor")
        icon_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "CoordinateConverter_logo.jpg")
        if os.path.isfile(icon_path):
            self.setWindowIcon(QIcon(icon_path))
        self.resize(980, 680)
        self.setMinimumSize(820, 560)
        self._sensors: list[SensorInfo] = []
        self._bbc_rear: dict = {}
        self._bbc_front: dict = {}
        self._build_ui()
        self._preload_sample()

    # ---------------------------------------------------------------- UI --- #
    def _build_ui(self) -> None:
        root = QVBoxLayout(self)
        root.setContentsMargins(14, 14, 14, 8)
        root.setSpacing(10)

        title = QLabel("VCS \u2192 OSI Mounting Editor")
        title.setFont(QFont("Segoe UI", 14, QFont.Bold))
        root.addWidget(title)

        subtitle = QLabel(
            "Parse a modelconfig, set the <b>host geometry</b>, then enter each "
            "sensor mounting in <b>VCS</b> (X/Y/Z in m, Roll/Pitch/Yaw in rad). "
            "On save, values are converted to <b>OSI</b> and written back to the "
            "same file. Blank mounting fields keep their existing value."
        )
        subtitle.setWordWrap(True)
        subtitle.setStyleSheet("color: #555;")
        root.addWidget(subtitle)

        # --- input file row --- #
        file_row = QHBoxLayout()
        file_row.addWidget(QLabel("Sample modelconfig:"))
        self.in_edit = QLineEdit()
        self.in_edit.setPlaceholderText("Select a modelconfig YAML\u2026")
        file_row.addWidget(self.in_edit, 1)
        browse_btn = QPushButton("Browse\u2026")
        browse_btn.clicked.connect(self._browse_input)
        file_row.addWidget(browse_btn)
        self.parse_btn = QPushButton("Parse")
        self.parse_btn.clicked.connect(self._do_parse)
        file_row.addWidget(self.parse_btn)
        root.addLayout(file_row)

        # --- host geometry (step 1: configure the vehicle) --- #
        host_row = QHBoxLayout()
        host_row.addWidget(QLabel("<b>Host geometry:</b>"))
        self.host_edits: dict[str, QLineEdit] = {}
        for hlabel in HOST_FIELDS:
            host_row.addWidget(QLabel(f"{hlabel} (m)"))
            edit = QLineEdit()
            edit.setMaximumWidth(120)
            edit.setEnabled(False)
            edit.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
            validator = QDoubleValidator(edit)
            validator.setNotation(QDoubleValidator.StandardNotation)
            validator.setDecimals(8)
            edit.setValidator(validator)
            host_row.addWidget(edit)
            self.host_edits[hlabel] = edit
        host_row.addStretch(1)
        root.addLayout(host_row)

        # --- Input Coordinate System (step 1b: origin + Y polarity) --- #
        conv_row = QHBoxLayout()
        conv_row.addWidget(QLabel("<b>Input Coordinate System:</b>"))
        conv_row.addWidget(QLabel("Origin:"))
        self.origin_combo = QComboBox()
        self.origin_combo.addItems(VCS_ORIGIN_OPTIONS)
        conv_row.addWidget(self.origin_combo)
        conv_row.addSpacing(16)
        conv_row.addWidget(QLabel("+Y side:"))
        self.ypol_combo = QComboBox()
        self.ypol_combo.addItems(Y_OPTIONS)
        self.ypol_combo.setCurrentText(Y_POS_RIGHT)
        conv_row.addWidget(self.ypol_combo)
        conv_row.addStretch(1)
        root.addLayout(conv_row)

        # --- axle reference offset (shown only for axle-center origins) --- #
        self.axle_container = QWidget()
        axle_row = QHBoxLayout(self.axle_container)
        axle_row.setContentsMargins(0, 0, 0, 0)
        self.bbc_label = QLabel("<b>Axle offset:</b>")
        axle_row.addWidget(self.bbc_label)
        self.bbc_edits: dict[str, QLineEdit] = {}
        for comp in ("X", "Y", "Z"):
            axle_row.addWidget(QLabel(f"{comp} (m)"))
            edit = QLineEdit()
            edit.setMaximumWidth(120)
            edit.setEnabled(False)
            edit.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
            validator = QDoubleValidator(edit)
            validator.setNotation(QDoubleValidator.StandardNotation)
            validator.setDecimals(8)
            edit.setValidator(validator)
            axle_row.addWidget(edit)
            self.bbc_edits[comp] = edit
        axle_row.addStretch(1)
        root.addWidget(self.axle_container)
        self.axle_container.setVisible(False)
        self.origin_combo.currentTextChanged.connect(self._on_origin_changed)

        # --- per-sensor input table (step 2: enter mounting in VCS) --- #
        headers = ["Model", "Sensor"] + [
            f"{f} ({FIELD_UNITS[f]})" for f in FIELDS
        ]
        self.table = QTableWidget(0, len(headers))
        self.table.setHorizontalHeaderLabels(headers)
        self.table.verticalHeader().setVisible(False)
        self.table.setAlternatingRowColors(True)
        hdr = self.table.horizontalHeader()
        hdr.setSectionResizeMode(0, QHeaderView.ResizeToContents)
        hdr.setSectionResizeMode(1, QHeaderView.ResizeToContents)
        for col in range(2, len(headers)):
            hdr.setSectionResizeMode(col, QHeaderView.Stretch)
        root.addWidget(self.table, 2)

        # --- action row --- #
        actions = QHBoxLayout()
        self.apply_btn = QPushButton("Convert to OSI && Save")
        self.apply_btn.setEnabled(False)
        self.apply_btn.clicked.connect(self._do_apply)
        self.apply_btn.setStyleSheet(
            "QPushButton { padding: 6px 18px; font-weight: bold; }"
        )
        actions.addWidget(self.apply_btn)
        self.reset_btn = QPushButton("Reset to file values")
        self.reset_btn.setEnabled(False)
        self.reset_btn.clicked.connect(self._do_parse)
        actions.addWidget(self.reset_btn)
        actions.addStretch(1)
        root.addLayout(actions)

        # --- log --- #
        log_label = QLabel("Log")
        log_label.setStyleSheet("color: #555; font-weight: bold;")
        root.addWidget(log_label)
        self.log = QPlainTextEdit()
        self.log.setReadOnly(True)
        self.log.setFont(QFont("Consolas", 9))
        self.log.setFrameShape(QFrame.StyledPanel)
        self.log.setMaximumBlockCount(2000)
        root.addWidget(self.log, 1)

        # --- status bar --- #
        self.status = QStatusBar()
        self.status.showMessage("Select a modelconfig YAML file, then click Parse.")
        root.addWidget(self.status)

    # ------------------------------------------------------------ events --- #
    def _browse_input(self) -> None:
        path, _ = QFileDialog.getOpenFileName(
            self,
            "Select modelconfig YAML",
            self.in_edit.text() or os.getcwd(),
            "YAML files (*.yaml *.yml);;All files (*.*)",
        )
        if path:
            self.in_edit.setText(path)
            self.status.showMessage("Click Parse to load sensors.")

    def _do_parse(self) -> None:
        in_path = self.in_edit.text().strip()
        if not in_path:
            QMessageBox.warning(self, "Missing input", "Please select an input file.")
            return
        if not os.path.isfile(in_path):
            QMessageBox.critical(
                self, "Not found", f"Input file does not exist:\n{in_path}"
            )
            return
        try:
            with open(in_path, "r", encoding="utf-8") as fh:
                text = fh.read()
            self._sensors = parse_sensors(text)
        except (ConversionError, OSError) as exc:
            self._sensors = []
            self._populate_table([])
            self.apply_btn.setEnabled(False)
            self.reset_btn.setEnabled(False)
            self.status.showMessage("Parse failed.")
            self.log.setPlainText(f"ERROR: {exc}")
            QMessageBox.critical(self, "Parse failed", str(exc))
            return

        self._populate_table(self._sensors)
        # Pre-fill host geometry from the (shared) parsed values and enable it.
        h0 = self._sensors[0]
        host_defaults = {
            "Length": h0.host_length,
            "Width": h0.host_width,
            "Height": h0.host_height,
        }
        for hlabel, edit in self.host_edits.items():
            edit.setText(self._fmt_cell(host_defaults[hlabel]))
            edit.setEnabled(True)
        # Store axle offsets (shared) and refresh the axle-offset row.
        self._bbc_rear = dict(h0.bbc_rear)
        self._bbc_front = dict(h0.bbc_front)
        self._on_origin_changed(self.origin_combo.currentText())
        self.apply_btn.setEnabled(True)
        self.reset_btn.setEnabled(True)
        names = ", ".join(f"{s.sensor_name}" for s in self._sensors)
        self.status.showMessage(
            f"Parsed {len(self._sensors)} sensor(s): {names}. "
            "Edit VCS values, then Convert to OSI && Save."
        )
        self.log.setPlainText(
            f"Parsed {len(self._sensors)} sensor(s) from:\n{in_path}\n\n"
            "Values shown below are the current file values, pre-filled into the "
            "VCS input boxes. Edit as needed; on save they are converted to OSI."
        )

    def _do_apply(self) -> None:
        in_path = self.in_edit.text().strip()
        if not in_path or not os.path.isfile(in_path):
            QMessageBox.critical(self, "Not found", "Input file is not available.")
            return
        if not self._sensors:
            QMessageBox.warning(self, "Nothing to do", "Parse a file first.")
            return

        host_params, host_invalid = self._collect_host()
        if host_invalid:
            QMessageBox.critical(
                self,
                "Invalid host geometry",
                "Please fix the host fields:\n\n" + "\n".join(host_invalid),
            )
            return

        vcs_by_model, invalid = self._collect_inputs()
        if invalid:
            QMessageBox.critical(
                self,
                "Invalid value",
                "Please fix these non-numeric cells:\n\n" + "\n".join(invalid),
            )
            return

        vcs_origin = self.origin_combo.currentText()
        origin_params: Optional[dict] = None
        if vcs_origin in (VCS_ORIGIN_FRONT_AXLE, VCS_ORIGIN_REAR_AXLE):
            origin_params, bbc_invalid = self._collect_bbc()
            if bbc_invalid:
                QMessageBox.critical(
                    self,
                    "Invalid axle offset",
                    "Please fix the axle-offset fields:\n\n" + "\n".join(bbc_invalid),
                )
                return

        try:
            results = apply_vcs_to_file(
                in_path, vcs_by_model, output_path=in_path, host_params=host_params,
                vcs_origin=vcs_origin,
                y_positive_left=(self.ypol_combo.currentText() == Y_POS_LEFT),
                origin_params=origin_params,
            )
        except (ConversionError, OSError) as exc:
            self.status.showMessage("Save failed.")
            self.log.setPlainText(f"ERROR: {exc}")
            QMessageBox.critical(self, "Save failed", str(exc))
            return

        self.log.setPlainText(
            f"Saved (OSI) to: {in_path}\n"
            f"Backup of original: {in_path}.bak\n"
            f"VCS convention: origin={self.origin_combo.currentText()}, "
            f"{self.ypol_combo.currentText()}\n\n"
            + format_apply_summary(results)
            + "\n\n"
            + format_detailed_conversions(results)
        )
        self.status.showMessage(
            f"Done \u2014 {len(results)} sensor(s) written as OSI to "
            f"{os.path.basename(in_path)} (backup: .bak)"
        )
        # Re-parse so the table reflects the freshly written (OSI) file.
        # self._do_parse()

    # ------------------------------------------------------------ helpers -- #
    def _populate_table(self, sensors: list[SensorInfo]) -> None:
        self.table.setRowCount(0)
        for s in sensors:
            row = self.table.rowCount()
            self.table.insertRow(row)

            model_item = QTableWidgetItem(str(s.model_id))
            model_item.setFlags(Qt.ItemIsEnabled)
            model_item.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(row, 0, model_item)

            name_item = QTableWidgetItem(s.sensor_name)
            name_item.setFlags(Qt.ItemIsEnabled)
            self.table.setItem(row, 1, name_item)

            for col, field_label in enumerate(FIELDS, start=2):
                if field_label in s.values:
                    edit = QLineEdit("")
                    edit.setPlaceholderText(
                        f"Enter {field_label} in VCS ({FIELD_UNITS[field_label]})"
                    )
                    validator = QDoubleValidator(edit)
                    validator.setNotation(QDoubleValidator.StandardNotation)
                    validator.setDecimals(8)
                    edit.setValidator(validator)
                    edit.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
                else:
                    # Field not present in this sensor -> non-editable blank.
                    edit = QLineEdit("")
                    edit.setPlaceholderText("n/a")
                    edit.setEnabled(False)
                self.table.setCellWidget(row, col, edit)

    def _collect_inputs(self) -> tuple[dict[str, dict], list[str]]:
        """Read the table into {model_id: {field: vcs_value}}; report bad cells.

        Blank cells are skipped (their existing file value is kept).
        """
        vcs_by_model: dict[str, dict] = {}
        invalid: list[str] = []
        for row, s in enumerate(self._sensors):
            field_vals: dict[str, float] = {}
            for col, field_label in enumerate(FIELDS, start=2):
                if field_label not in s.values:
                    continue
                widget = self.table.cellWidget(row, col)
                text = widget.text().strip() if widget is not None else ""
                if text == "":
                    continue  # left blank -> keep existing file value
                try:
                    field_vals[field_label] = float(text)
                except ValueError:
                    invalid.append(
                        f"Model {s.model_id} [{s.sensor_name}] {field_label}: '{text}'"
                    )
            if field_vals:
                vcs_by_model[s.model_id] = field_vals
        return vcs_by_model, invalid

    def _collect_host(self) -> tuple[dict, list[str]]:
        """Read the host geometry boxes; report any invalid/empty values."""
        host_params: dict = {}
        invalid: list[str] = []
        for hlabel, edit in self.host_edits.items():
            text = edit.text().strip()
            if text == "":
                invalid.append(f"Host {hlabel}: empty")
                continue
            try:
                host_params[hlabel] = float(text)
            except ValueError:
                invalid.append(f"Host {hlabel}: '{text}'")
        return host_params, invalid

    def _on_origin_changed(self, origin: str) -> None:
        """Show/label/prefill the axle-offset row for axle-center origins."""
        is_axle = origin in (VCS_ORIGIN_FRONT_AXLE, VCS_ORIGIN_REAR_AXLE)
        self.axle_container.setVisible(is_axle)
        if not is_axle:
            return
        if origin == VCS_ORIGIN_REAR_AXLE:
            self.bbc_label.setText("<b>BbCenterToRear (m):</b>")
            defaults = self._bbc_rear
        else:
            self.bbc_label.setText("<b>BbCenterToFront (m):</b>")
            defaults = self._bbc_front
        have_file = bool(self._sensors)
        for comp, edit in self.bbc_edits.items():
            if have_file and comp in defaults:
                edit.setText(self._fmt_cell(defaults[comp]))
            edit.setEnabled(have_file)

    def _collect_bbc(self) -> tuple[dict, list[str]]:
        """Read the axle-offset boxes; report any invalid/empty values."""
        params: dict = {}
        invalid: list[str] = []
        for comp, edit in self.bbc_edits.items():
            text = edit.text().strip()
            if text == "":
                invalid.append(f"Axle {comp}: empty")
                continue
            try:
                params[comp] = float(text)
            except ValueError:
                invalid.append(f"Axle {comp}: '{text}'")
        return params, invalid

    @staticmethod
    def _fmt_cell(value: float) -> str:
        text = f"{value:.6f}".rstrip("0").rstrip(".")
        if text in ("", "-", "-0"):
            text = "0"
        return text

    def _preload_sample(self) -> None:
        sample = os.path.join(
            os.path.dirname(os.path.abspath(__file__)),
            "modelconfig_sil_CEER_With_FC.yaml",
        )
        if os.path.isfile(sample):
            self.in_edit.setText(sample)
            self.status.showMessage("Sample file loaded. Click Parse.")


def main() -> int:
    app = QApplication(sys.argv)
    window = ConverterWindow()
    window.show()
    return app.exec_()


if __name__ == "__main__":
    raise SystemExit(main())
