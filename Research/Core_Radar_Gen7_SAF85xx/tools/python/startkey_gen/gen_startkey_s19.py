"""StartKey S19 generator for Aptiv and CEER StartKey containers.

Generates two separate Motorola S19 files for flashing StartKey containers into the
SAF85xx external (QSPI NOR) flash.  Each file covers one StartKey provider:

  1. Aptiv StartKeys  →  aptiv_startkeys.s19              @ 0x720100
  2. CEER  StartKeys  →  ceer_startkeys_<variant>.s19     @ 0x720200

Aptiv StartKey format (FLAT — no per-entry slot/size header):
  Raw key bytes concatenated in fixed position order.
  Firmware uses the APTIV_KEY_LAYOUT[] descriptor table to map byte offsets to HSE slots.
  Layout: [MacSec_CAK(32B) | SecOC(16B) | aKEK(16B)] = 64B total.
  Matches APTIV_STARTKEY_FLAT_* constants in SWC_TKEY_Appl_Security_Manager.c.

CEER StartKey format (fixed-size entries, all 34B):
  byte[0]      : slot_num  — identifies the target HSE/AUTOSAR Crypto slot
  byte[1]      : key_size  — actual import length in bytes (16 or 32B)
  byte[2..33]  : key_value — key material (always 32B; zero-padded for 16B keys like EMK/CMAC)
  Every entry = 34B total regardless of key_size.

Slot numbers (defined in CDD_AutoKeyImport_Cfg.h):
  95  SECACCESS_L1_KEY_SLOT   Diagnostic L1             32 B  (CEER)
  96  SECACCESS_L2_KEY_SLOT   Diagnostic L2             32 B  (CEER)
  97  SECACCESS_L3_KEY_SLOT   Diagnostic L3             32 B  [NO CSM slot configured]
  98  SECACCESS_L4_KEY_SLOT   Diagnostic L4             32 B  (CEER)
  99  SECACCESS_L5_KEY_SLOT   Diagnostic L5             32 B  (CEER)
  100 SECACCESS_L6_KEY_SLOT   Diagnostic L6             32 B  (CEER)
  107 EMK_KEY_SLOT            ECU Master Key            16 B  (CEER, padded → 32 B)
  108 CMAC_KEY_SLOT           CMAC Key                  16 B  (CEER, padded → 32 B)
  109 SECOC_KEY_SLOT          SecOC AES-128             16 B  (APTIV flat @ offset 32)
  110 MACSEC_KEY_SLOT         MacSec CAK AES-256        32 B  (APTIV flat @ offset  0)
  111 AKEK_KEY_SLOT           Aptiv aKEK AES-128        16 B  (APTIV flat @ offset 48)

NOTES:
  - Aptiv format (0x720100) changed to FLAT: no slot/size header per entry.
    Slot assignments are position-based (see APTIV_KEY_LAYOUT below and firmware APTIV_KEY_LAYOUT[]).
  - CEER format (0x720200) is UNCHANGED.
  - AKEK slot 111: no AUTOSAR CsmKey configured → firmware skips silently at runtime.
  - L3 slot 97: no AUTOSAR CsmKey configured → included as placeholder, skipped at runtime.

Usage:
  python gen_startkey_s19.py                        # both files, FLR/LRR variant (default)
  python gen_startkey_s19.py --source ceer          # CEER S19 only
  python gen_startkey_s19.py --source aptiv         # Aptiv S19 only
  python gen_startkey_s19.py --variant srr          # SRR_RL variant (CEER keys)
  python gen_startkey_s19.py --out-dir /tmp/keys    # custom output directory
"""

import argparse
import re
import sys
from pathlib import Path

# ---------------------------------------------------------------------------
# Flash address constants  (must match SWC_TKEY_Appl_Security_Manager.c)
# ---------------------------------------------------------------------------
APTIV_STARTKEY_BASE_ADDR = 0x720100  # Aptiv StartKey block
CEER_STARTKEY_BASE_ADDR = 0x720200  # CEER StartKey block
STARTKEY_ENTRY_SIZE = 34  # CEER: slot(1) + size(1) + key_value(32) — fixed for ALL entries
STARTKEY_KEY_VALUE_SIZE = 32  # CEER: key data field is always 32B (zero-padded for 16B keys)
# Aptiv flat block layout (must match APTIV_STARTKEY_FLAT_* macros in C source)
APTIV_STARTKEY_FLAT_BLOCK_SIZE = 64  # total bytes: MacSec(32) + SecOC(16) + aKEK(16)
APTIV_STARTKEY_FLAT_MACSEC_OFFSET = 0  # slot 110, MacSec CAK AES-256, 32B
APTIV_STARTKEY_FLAT_SECOC_OFFSET = 32  # slot 109, SecOC AES-128, 16B
APTIV_STARTKEY_FLAT_AKEK_OFFSET = 48  # slot 111, aKEK AES-128, 16B

# ---------------------------------------------------------------------------
# Slot number constants  (mirror CDD_AutoKeyImport_Cfg.h)
# ---------------------------------------------------------------------------
SLOT_L1 = 95
SLOT_L2 = 96
SLOT_L3 = 97  # NO CSM slot configured — placeholder only
SLOT_L4 = 98
SLOT_L5 = 99
SLOT_L6 = 100
SLOT_EMK = 107
SLOT_CMAC = 108
SLOT_SECOC = 109
SLOT_MACSEC_CAK = 110
SLOT_AKEK = (
    111  # Aptiv AKEK — TESTING: mapped to MacSec_KDF CSM slot; replace when real CsmKey added
)

# ---------------------------------------------------------------------------
# CEER key order and metadata.
# Modify this list to change the order or add/remove keys.
#
# Each entry: (slot_num, key_size, flr_var_name, srr_var_name, description)
#   flr_var_name / srr_var_name: C variable name in SWC_TKEY_Appl_Security_Manager.c
#                                Use None if the variable does not exist (placeholder only)
#
# NOTE: SecOC (slot 109) and MacSec (slot 110) are APTIV-owned — see APTIV_KEY_ORDER.
# ---------------------------------------------------------------------------
CEER_KEY_ORDER = [
    # slot              size  flr_var                    srr_var                  description
    (SLOT_L1, 32, "Startkey_L1_LRR", "Startkey_L1_SRR_RL", "Diagnostic L1 (slot 95)"),
    (SLOT_L2, 32, "Startkey_L2_LRR", "Startkey_L2_SRR_RL", "Diagnostic L2 (slot 96)"),
    (SLOT_L4, 32, "Startkey_L4_LRR", "Startkey_L4_SRR_RL", "Diagnostic L4 (slot 98)"),
    (SLOT_L5, 32, "Startkey_L5_LRR", "Startkey_L5_SRR_RL", "Diagnostic L5 (slot 99)"),
    (SLOT_L6, 32, "Startkey_L6_LRR", "Startkey_L6_SRR_RL", "Diagnostic L6 (slot 100)"),
    (SLOT_EMK, 16, "EMK_Key", "EMK_Key", "ECU Master Key / EMAC (slot 107)"),
    (SLOT_CMAC, 16, "CMAC_Key", "CMAC_Key", "CMAC Key (slot 108)"),
]

# ---------------------------------------------------------------------------
# Aptiv key layout descriptor for FLAT format.
# ORDER IS FIXED — matches APTIV_KEY_LAYOUT[] in SWC_TKEY_Appl_Security_Manager.c
# and APTIV_STARTKEY_FLAT_*_OFFSET constants.  Do NOT reorder without updating both.
#
# Each entry: (slot_num, key_size, flr_var_name, srr_var_name, description)
#   MacSec and SecOC have per-variant key values (FLR vs SRR_RL).
#   AKEK is common (same value for both variants).
#
# Format in S19: raw key bytes concatenated — NO slot/size header per entry.
# ---------------------------------------------------------------------------
APTIV_KEY_ORDER = [
    # slot              size  flr_var               srr_var                description
    (
        SLOT_MACSEC_CAK,
        32,
        "MacSec_CAK_FLR",
        "MacSec_CAK_SRR_RL",
        "MacSec CAK AES-256 (slot 110, 32B @ offset 0)",
    ),
    (
        SLOT_SECOC,
        16,
        "SecOC_Key_FLR",
        "SecOC_Key_SRR_RL",
        "SecOC AES-128 (slot 109, 16B @ offset 32)",
    ),
    (SLOT_AKEK, 16, "AKEK_Key", "AKEK_Key", "Aptiv aKEK AES-128 (slot 111, 16B @ offset 48)"),
]

# ---------------------------------------------------------------------------
# Motorola S-record helpers
# ---------------------------------------------------------------------------


def _checksum(payload: list) -> int:
    """Return the Motorola S19 one-byte checksum for payload."""
    return (~sum(payload)) & 0xFF


def s0_record(text: str) -> str:
    """Return an S0 header record containing text."""
    d = [ord(c) for c in text[:60]]
    pl = [2 + len(d) + 1, 0, 0] + d
    return "S0" + "".join(f"{b:02X}" for b in pl) + f"{_checksum(pl):02X}"


def s3_records(base_addr: int, data: bytes) -> list:
    """Emit S3 records (32-bit address) for data at base_addr."""
    out = []
    for i in range(0, len(data), 16):
        chunk = list(data[i : i + 16])
        a = base_addr + i
        pl = [
            4 + len(chunk) + 1,
            (a >> 24) & 0xFF,
            (a >> 16) & 0xFF,
            (a >> 8) & 0xFF,
            a & 0xFF,
        ] + chunk
        out.append("S3" + "".join(f"{b:02X}" for b in pl) + f"{_checksum(pl):02X}")
    return out


def s7_record() -> str:
    """Return an S7 end-of-file record."""
    pl = [0x05, 0, 0, 0, 0]
    return "S7" + "".join(f"{b:02X}" for b in pl) + f"{_checksum(pl):02X}"


# ---------------------------------------------------------------------------
# C source parser: extract byte values from a named C array initializer
# ---------------------------------------------------------------------------


def parse_c_array(src_text: str, varname: str) -> list:
    """Return list of int byte values from a C array initializer, or raise ValueError."""
    m = re.search(
        r"\b" + re.escape(varname) + r"\b"
        r"(?:\s*\[[^\]]*\])*"
        r"(?:\s*__attribute__\s*\(\s*\([^)]*\)\s*\))*"
        r"\s*=\s*\{([^}]+)\}",
        src_text,
        re.DOTALL,
    )
    if not m:
        raise ValueError(f"'{varname}' not found in C source")
    return [int(v, 16) for v in re.findall(r"0[xX][0-9A-Fa-f]+", m.group(1))]


# ---------------------------------------------------------------------------
# Build StartKey binary blob from a key-order table
# ---------------------------------------------------------------------------


def build_startkey_blob(key_order, src_text: str, use_srr: bool) -> tuple:
    """Build the CEER StartKey binary blob from key_order.

    Each entry is [slot_num(1B) | key_size(1B) | key_value(32B)] = 34B fixed.
    key_size holds the actual import length (16 or 32B); the value field is always 32B
    wide (zero-padded for 16B keys such as EMK and CMAC). Matches the on-flash format
    that firmware parses by advancing STARTKEY_ENTRY_SIZE (34B) per entry.
    Returns (blob: bytearray, all_ok: bool). Placeholder entries (var_name=None) are zero-filled.
    """
    blob = bytearray()
    all_ok = True

    for slot, key_size, flr_var, srr_var, desc in key_order:
        var = srr_var if use_srr else flr_var
        # Always 34B per entry: slot(1) + size(1) + value(32B, zero-padded)
        entry = bytearray(STARTKEY_ENTRY_SIZE)
        entry[0] = slot & 0xFF
        entry[1] = key_size & 0xFF  # actual import length; value field is always 32B

        if var is None:
            print(
                f"  PLACEHOLDER  slot={slot:3d} size={key_size:2d}B  [{desc}]"
                f"  key=<zeros -- update CEER_KEY_ORDER with real variable>"
            )
        else:
            try:
                raw = parse_c_array(src_text, var)
                chunk = raw[0:key_size]
                if len(chunk) < key_size:
                    print(
                        f"  WARNING  slot={slot:3d} {var}: only {len(raw)}B in C source, "
                        f"expected {key_size}B — zero-padded",
                        file=sys.stderr,
                    )
                # Place key bytes starting at offset 2; remaining bytes stay 0x00 (padding)
                padded = (chunk + [0x00] * STARTKEY_KEY_VALUE_SIZE)[:STARTKEY_KEY_VALUE_SIZE]
                entry[2 : 2 + STARTKEY_KEY_VALUE_SIZE] = padded
                print(f"  OK  slot={slot:3d} size={key_size:2d}B  {var:<36s}  [{desc}]")
            except ValueError as e:
                print(f"  ERROR  slot={slot:3d} [{desc}]: {e}", file=sys.stderr)
                all_ok = False

        blob += entry

    return blob, all_ok


# ---------------------------------------------------------------------------
# Build Aptiv StartKey flat binary blob (NEW format — no per-entry slot/size header).
# Order and size MUST match APTIV_KEY_ORDER and APTIV_STARTKEY_FLAT_*_OFFSET constants.
# ---------------------------------------------------------------------------


def build_aptiv_flat_blob(src_text: str, use_srr: bool = False) -> tuple:
    """Build the 64-byte Aptiv StartKey flat blob.

    Layout: [MacSec_CAK(32B) | SecOC(16B) | aKEK(16B)] = 64B total.
    Raw key bytes only -- no slot_num or key_size header per entry.
    MacSec and SecOC are variant-specific; AKEK is common.
    Returns (blob: bytearray, all_ok: bool).
    """
    blob = bytearray(APTIV_STARTKEY_FLAT_BLOCK_SIZE)
    all_ok = True

    for slot, key_size, flr_var, srr_var, desc in APTIV_KEY_ORDER:
        var = srr_var if use_srr else flr_var
        # Determine byte offset for this key in the flat block
        if slot == SLOT_MACSEC_CAK:
            offset = APTIV_STARTKEY_FLAT_MACSEC_OFFSET
        elif slot == SLOT_SECOC:
            offset = APTIV_STARTKEY_FLAT_SECOC_OFFSET
        elif slot == SLOT_AKEK:
            offset = APTIV_STARTKEY_FLAT_AKEK_OFFSET
        else:
            print(
                f"  ERROR  Unknown Aptiv slot {slot} in APTIV_KEY_ORDER — fix the table.",
                file=sys.stderr,
            )
            all_ok = False
            continue

        if var is None:
            print(
                f"  PLACEHOLDER  slot={slot:3d} offset={offset:2d}B size={key_size:2d}B  [{desc}]"
                f"  key=<zeros — update APTIV_KEY_ORDER>"
            )
        else:
            try:
                raw = parse_c_array(src_text, var)
                chunk = raw[0:key_size]
                if len(chunk) < key_size:
                    print(
                        f"  WARNING  slot={slot:3d} {var}: only {len(raw)}B in C source, "
                        f"expected {key_size}B — zero-padded",
                        file=sys.stderr,
                    )
                padded = (chunk + [0x00] * key_size)[:key_size]
                blob[offset : offset + key_size] = padded
                print(
                    f"  OK  slot={slot:3d} size={key_size:2d}B  offset={offset:2d}  "
                    f"{var:<36s}  [{desc}]"
                )
            except ValueError as e:
                print(f"  ERROR  slot={slot:3d} [{desc}]: {e}", file=sys.stderr)
                all_ok = False

    return blob, all_ok


# ---------------------------------------------------------------------------
# S19 write helper
# ---------------------------------------------------------------------------


def write_s19(
    path: Path, base_addr: int, blob: bytearray, description: str, num_keys: int
) -> None:
    """Write blob as a Motorola S19 file at base_addr with a descriptive S0 header."""
    records = [s0_record(description[:60])]
    records += s3_records(base_addr, bytes(blob))
    records.append(s7_record())
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(records) + "\n", encoding="ascii")
    print(f"\nWritten {len(records) - 2} S3 records -> {path}")
    print(f"  0x{base_addr:08X}  {num_keys} key entries  ({len(blob)} B)")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------


def main():
    """Parse CLI arguments and generate Aptiv and/or CEER StartKey S19 files."""
    here = Path(__file__).resolve().parent
    default_src = (
        here.parent.parent.parent / "software/m7/autosar/swc/TKEY_SWC"
        "/SWC_TKEY_Appl_Security_Manager/Source"
        "/SWC_TKEY_Appl_Security_Manager.c"
    )

    ap = argparse.ArgumentParser(
        description="Generate Aptiv StartKey and/or CEER StartKey S19 files",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    ap.add_argument(
        "--source",
        choices=["aptiv", "ceer", "both"],
        default="both",
        help="Which StartKey container(s) to generate (default: both)",
    )
    ap.add_argument(
        "--variant",
        choices=["flr", "srr"],
        default="flr",
        help="CEER key variant: 'flr'=FLR/LRR (default), 'srr'=SRR_RL",
    )
    ap.add_argument(
        "--out-dir",
        type=Path,
        default=here,
        help="Output directory (default: same folder as this script)",
    )
    ap.add_argument(
        "--src",
        type=Path,
        default=default_src,
        metavar="FILE",
        help="Path to SWC_TKEY_Appl_Security_Manager.c",
    )
    args = ap.parse_args()

    use_srr = args.variant == "srr"

    if not args.src.exists():
        print(f"ERROR: C source not found: {args.src}", file=sys.stderr)
        sys.exit(1)

    src_text = args.src.read_text(encoding="utf-8", errors="replace")
    print(f"C source : {args.src.name}")
    print(f"Variant  : {'SRR_RL' if use_srr else 'FLR/LRR'}")
    print(f"Output   : {args.out_dir}")

    overall_ok = True

    # ------------------------------------------------------------------ Aptiv
    if args.source in ("aptiv", "both"):
        variant_tag = "srr" if use_srr else "flr"
        print(
            f"\n--- Aptiv StartKeys ({variant_tag}) (FLAT format: no per-entry slot/size header) ---"
        )
        print(
            f"  Layout: [MacSec_CAK(32B)@offset0 | SecOC(16B)@offset32 | aKEK(16B)@offset48] = {APTIV_STARTKEY_FLAT_BLOCK_SIZE}B"
        )
        blob, ok = build_aptiv_flat_blob(src_text, use_srr)
        if not ok:
            overall_ok = False
        out_path = args.out_dir / f"aptiv_startkeys_{variant_tag}.s19"
        write_s19(
            out_path,
            APTIV_STARTKEY_BASE_ADDR,
            blob,
            f"Aptiv StartKeys flat ({variant_tag}) - gen_startkey_s19.py",
            num_keys=len(APTIV_KEY_ORDER),
        )
        print(
            "  NOTE: Flat format (slot 110 MACsec/slot 109 SecOC/slot 111 aKEK) -- no per-entry headers."
        )
        print(
            "        Firmware uses APTIV_KEY_LAYOUT[] descriptor in StartKey_ImportBlock_Aptiv()."
        )

    # ------------------------------------------------------------------ CEER
    if args.source in ("ceer", "both"):
        variant_tag = "srr" if use_srr else "flr"
        print(f"\n--- CEER StartKeys ({variant_tag}) ---")
        blob, ok = build_startkey_blob(CEER_KEY_ORDER, src_text, use_srr)
        if not ok:
            overall_ok = False
        out_path = args.out_dir / f"ceer_startkeys_{variant_tag}.s19"
        write_s19(
            out_path,
            CEER_STARTKEY_BASE_ADDR,
            blob,
            f"CEER StartKeys ({variant_tag}) - gen_startkey_s19.py",
            num_keys=len(CEER_KEY_ORDER),
        )

    print()
    if not overall_ok:
        print(
            "WARNING: one or more key variables could not be found — "
            "check C source or update KEY_ORDER lists.",
            file=sys.stderr,
        )
        sys.exit(1)
    else:
        print("All key entries written successfully.")


if __name__ == "__main__":
    main()
