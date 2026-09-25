"""KMS StartKey MOL S19 generator.

Stage 1 (always):
  Reads aptiv_startkeys_<v>.s19 + ceer_startkeys_<v>.s19
  Writes mol_startkeys_<v>.s19  (Aptiv+CEER, no XCP/JTAG)
  Deletes the intermediate aptiv_*/ceer_* files.

Stage 2 (optional, requires --auth <Mol_AuthXcp_Pwds.s19>):
  Merges the auth file (0x720000-0x7200FF) with the startkeys
  Writes mol_merged_<v>.s19  (XCP/JTAG + Aptiv + CEER)

Usage:
    python gen_mol_startkeys.py                         # both variants, startkeys only
    python gen_mol_startkeys.py srr                     # SRR only, startkeys only
    python gen_mol_startkeys.py flr                     # FLR only, startkeys only
    python gen_mol_startkeys.py --auth Mol_AuthXcp_Pwds.s19           # both + merge
    python gen_mol_startkeys.py srr --auth Mol_AuthXcp_Pwds.s19       # SRR + merge
    python gen_mol_startkeys.py --auth /path/to/Mol_AuthXcp_Pwds.s19  # explicit path
"""

import sys
from pathlib import Path


# ---------------------------------------------------------------------------
# Intel HEX parser (handles record types 00, 01, 04)
# ---------------------------------------------------------------------------


def parse_ihex(path: Path, crop_start: int, crop_end: int) -> dict:
    """Parse an Intel HEX file; return {addr: byte} for data bytes in [crop_start, crop_end)."""  # noqa: E501
    data = {}
    lin_ext = 0

    for raw in path.read_text(encoding="ascii").splitlines():
        raw = raw.strip()
        if not raw.startswith(":"):
            continue
        rec = bytes.fromhex(raw[1:])
        count = rec[0]
        addr16 = (rec[1] << 8) | rec[2]
        rtype = rec[3]
        payload = rec[4 : 4 + count]

        if rtype == 0x00:  # data
            base = (lin_ext << 16) | addr16
            for i, b in enumerate(payload):
                abs_addr = base + i
                if crop_start <= abs_addr < crop_end:
                    data[abs_addr] = b
        elif rtype == 0x04:  # extended linear address
            lin_ext = (payload[0] << 8) | payload[1]
        elif rtype == 0x01:  # EOF
            break

    return data


# ---------------------------------------------------------------------------
# Motorola S19 parser (S3 records)
# ---------------------------------------------------------------------------


def parse_s19(path: Path) -> dict:
    """Parse a Motorola S19 file; return {addr: byte} for all S3 data records."""
    data = {}
    for raw in path.read_text(encoding="ascii").splitlines():
        raw = raw.strip()
        if not raw.startswith("S3"):
            continue
        rec = bytes.fromhex(raw[2:])
        count = rec[0]
        addr = (rec[1] << 24) | (rec[2] << 16) | (rec[3] << 8) | rec[4]
        payload = rec[5:count]  # excludes trailing checksum byte
        for i, b in enumerate(payload):
            data[addr + i] = b
    return data


# ---------------------------------------------------------------------------
# Motorola S19 emitter helpers
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
    """Return S3 records (32-bit address) for data starting at base_addr."""
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
# Writer
# ---------------------------------------------------------------------------


def write_s19(out_path: Path, merged: dict, description: str) -> None:
    """Write {addr: byte} dict as a sorted Motorola S19 file."""
    if not merged:
        print(f"  WARNING: no data to write for {out_path.name}", file=sys.stderr)
        return

    sorted_addrs = sorted(merged.keys())
    records = [s0_record(description[:60])]

    chunk_start = sorted_addrs[0]
    chunk_bytes = [merged[sorted_addrs[0]]]

    for addr in sorted_addrs[1:]:
        if addr == chunk_start + len(chunk_bytes):
            chunk_bytes.append(merged[addr])
        else:
            records += s3_records(chunk_start, bytes(chunk_bytes))
            chunk_start = addr
            chunk_bytes = [merged[addr]]

    records += s3_records(chunk_start, bytes(chunk_bytes))
    records.append(s7_record())

    out_path.write_text("\n".join(records) + "\n", encoding="ascii")
    print(f"  Written {len(records) - 2} S3 records -> {out_path.name}  ({len(merged)} bytes)")


# ---------------------------------------------------------------------------
# Per-variant builder
# ---------------------------------------------------------------------------


def build_variant(here: Path, variant: str, auth_data: dict | None) -> None:
    """Build mol_startkeys and optionally mol_merged S19 files for one variant.

    Stage 1 always runs: merges aptiv + ceer into mol_startkeys_<variant>.s19.
    Stage 2 runs when auth_data is provided: writes mol_merged_<variant>.s19.
    Intermediate aptiv_*/ceer_* files are deleted after use.
    """
    tag = variant.upper()

    aptiv_path = here / f"aptiv_startkeys_{variant}.s19"
    ceer_path = here / f"ceer_startkeys_{variant}.s19"

    for p in (aptiv_path, ceer_path):
        if not p.exists():
            print(f"ERROR: Intermediate file not found: {p.name}", file=sys.stderr)
            print("  Run gen_startkey_s19.py first to generate it.", file=sys.stderr)
            sys.exit(1)

    print(f"\n[{tag}] Parsing {aptiv_path.name}...")
    aptiv_data = parse_s19(aptiv_path)
    print(f"  {len(aptiv_data)} bytes  (@ 0x720100)")

    print(f"[{tag}] Parsing {ceer_path.name}...")
    ceer_data = parse_s19(ceer_path)
    print(f"  {len(ceer_data)} bytes  (@ 0x720200)")

    # ------------------------------------------------------------------
    # Stage 1: mol_startkeys_<variant>.s19 -- Aptiv + CEER, no XCP/JTAG
    # ------------------------------------------------------------------
    startkeys_only: dict = {}
    startkeys_only.update(aptiv_data)
    startkeys_only.update(ceer_data)

    overlap = len(aptiv_data) + len(ceer_data) - len(startkeys_only)
    if overlap:
        print(
            f"  WARNING [{tag}] startkeys overlap: {overlap} bytes overwritten.", file=sys.stderr
        )

    out_startkeys = here / f"mol_startkeys_{variant}.s19"
    print(f"[{tag}] Writing mol_startkeys_{variant}.s19  (Aptiv+CEER, no XCP/JTAG)...")
    write_s19(
        out_startkeys, startkeys_only, f"KMS StartKeys {tag} Aptiv+CEER gen_mol_startkeys.py"
    )

    # ------------------------------------------------------------------
    # Delete intermediates -- key material no longer needed in plain files
    # ------------------------------------------------------------------
    for p in (aptiv_path, ceer_path):
        p.unlink()
        print(f"  Deleted intermediate: {p.name}")

    # ------------------------------------------------------------------
    # Stage 2 (optional): mol_merged_<variant>.s19 -- full MOL
    # ------------------------------------------------------------------
    if auth_data is not None:
        merged_full: dict = {}
        merged_full.update(auth_data)
        merged_full.update(startkeys_only)

        overlap_full = len(auth_data) + len(startkeys_only) - len(merged_full)
        if overlap_full:
            print(
                f"  WARNING [{tag}] full merge overlap: {overlap_full} bytes overwritten.",
                file=sys.stderr,
            )

        out_merged = here / f"mol_merged_{variant}.s19"
        print(f"[{tag}] Writing mol_merged_{variant}.s19  (XCP/JTAG + Aptiv + CEER)...")
        write_s19(out_merged, merged_full, f"KMS MOL merged {tag} gen_mol_startkeys.py")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------


def main() -> None:
    """Parse CLI arguments and run the S19 generation pipeline."""
    here = Path(__file__).resolve().parent

    # Parse args: optional variant (srr/flr) and optional --auth <path>
    args = sys.argv[1:]
    variant = ""
    auth_path = None

    i = 0
    while i < len(args):
        a = args[i]
        if a == "--auth":
            if i + 1 >= len(args):
                print("ERROR: --auth requires a file path argument.", file=sys.stderr)
                sys.exit(1)
            auth_path = Path(args[i + 1])
            if not auth_path.is_absolute():
                auth_path = here / auth_path
            i += 2
        elif a in ("srr", "flr"):
            variant = a
            i += 1
        else:
            print(f'ERROR: Unknown argument "{a}".', file=sys.stderr)
            print(
                "Usage: gen_mol_startkeys.py [srr|flr] [--auth <Mol_AuthXcp_Pwds.s19>]",
                file=sys.stderr,
            )
            sys.exit(1)

    build_srr = variant in ("", "srr")
    build_flr = variant in ("", "flr")

    # Validate auth file if provided
    auth_data = None
    if auth_path is not None:
        if not auth_path.exists():
            print(f"ERROR: Auth file not found: {auth_path}", file=sys.stderr)
            sys.exit(1)
        print(f"\nParsing auth file: {auth_path.name}  [Intel HEX, crop 0x720000-0x7200FF]...")
        auth_data = parse_ihex(auth_path, crop_start=0x720000, crop_end=0x720100)
        print(f"  {len(auth_data)} bytes in XCP/JTAG auth range")

    print("============================================================")
    print(" gen_mol_startkeys.py")
    print(f" Variant : {'SRR+FLR' if not variant else variant.upper()}")
    print(" Stage 1 : mol_startkeys_<v>.s19  (Aptiv+CEER, no XCP/JTAG)")
    if auth_data is not None:
        print(" Stage 2 : mol_merged_<v>.s19     (XCP/JTAG + Aptiv + CEER)")
        print(f" Auth    : {auth_path.name}")
    else:
        print(" Stage 2 : SKIPPED  (pass --auth <file> to enable)")
    print("============================================================")

    if build_srr:
        build_variant(here, "srr", auth_data)
    if build_flr:
        build_variant(here, "flr", auth_data)

    print("\n============================================================")
    print(" Generated files:")
    if build_srr:
        print("   mol_startkeys_srr.s19")
        if auth_data is not None:
            print("   mol_merged_srr.s19")
    if build_flr:
        print("   mol_startkeys_flr.s19")
        if auth_data is not None:
            print("   mol_merged_flr.s19")
    print(" Intermediates deleted:")
    if build_srr:
        print("   aptiv_startkeys_srr.s19  ceer_startkeys_srr.s19")
    if build_flr:
        print("   aptiv_startkeys_flr.s19  ceer_startkeys_flr.s19")
    print("============================================================")
    print(" Done.")
    print("============================================================")


if __name__ == "__main__":
    main()
