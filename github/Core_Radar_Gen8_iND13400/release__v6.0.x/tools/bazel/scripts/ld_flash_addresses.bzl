"""Repository rule and module extension for extracting FLASH addresses from linker scripts.

Reads a preprocessed GNU ld linker script and extracts FLASH address symbol
definitions (e.g. `__APP_FLASH_START_ADDRESS = 0x00180000;`) into a generated
`.bzl` file that can be loaded from any BUILD or .bzl file in the workspace.

Usage in MODULE.bazel:
    flash_addr = use_extension(
        "//tools/bazel/scripts:ld_flash_addresses.bzl",
        "flash_addresses_extension",
    )
    flash_addr.from_ld(
        name = "flash_addresses",
        src = "//software/common/linker:common_ld",
    )
    use_repo(flash_addr, "flash_addresses")

Usage in BUILD or .bzl files:
    load("@flash_addresses//:flash_addresses.bzl", "FLASH_ADDRESSES")
    # or specific constants:
    load("@flash_addresses//:flash_addresses.bzl", "APP_FLASH_START_ADDRESS", "APP_FLASH_BLOCK_SIZE")
"""

def _parse_flash_addresses(content):
    """Parse FLASH address symbol definitions from GNU ld linker script content.

    Recognises lines of the form:
        __<NAME>_FLASH_<SUFFIX> = 0x<HEXVALUE>;

    Correctly skips:
    - Multi-line block comments (/* ... */)
    - Inline block comments on the same line
    - Line comments starting with // or *

    Args:
        content: String contents of the .ld file.

    Returns:
        dict[str, int]: Mapping of symbol name to its integer value.
    """
    addresses = {}
    in_block_comment = False

    for line in content.split("\n"):
        stripped = line.strip()

        # Handle block comment state first
        if in_block_comment:
            if "*/" in stripped:
                in_block_comment = False
            continue

        # Detect start of a block comment on this line
        if "/*" in stripped:
            comment_start = stripped.find("/*")
            after_open = stripped[comment_start + 2:]
            if "*/" in after_open:
                # Inline block comment: strip it out, keep processing the rest
                stripped = stripped[:comment_start].strip()
            else:
                in_block_comment = True
                continue

        # Skip line comments and continuation lines inside block comments
        if stripped.startswith("//") or stripped.startswith("*"):
            continue

        # Match FLASH symbol assignments:  __<NAME>_FLASH_<SUFFIX> = 0x<HEX>;
        if "FLASH" in stripped and "=" in stripped and stripped.endswith(";"):
            eq_idx = stripped.find("=")
            name = stripped[:eq_idx].strip()
            value_part = stripped[eq_idx + 1:].strip().rstrip(";").strip()

            if (name.startswith("__") and "FLASH" in name and (value_part.startswith("0x") or value_part.startswith("0X"))):
                # int(x, 0) auto-detects base from the 0x prefix
                addresses[name] = int(value_part, 0)

    return addresses

def _ld_flash_addresses_impl(repository_ctx):
    src_path = repository_ctx.path(repository_ctx.attr.src)

    # Watch the file so Bazel re-fetches this repository when the preprocessed
    # linker script changes.
    repository_ctx.watch(src_path)

    content = repository_ctx.read(src_path)
    addresses = _parse_flash_addresses(content)

    if not addresses:
        fail("ld_flash_addresses: no FLASH address symbols found in '{}'".format(
            str(repository_ctx.attr.src),
        ))

    # --- Build flash_addresses.bzl ---
    bzl_lines = [
        '"""Auto-generated FLASH address constants from {}.'.format(repository_ctx.attr.src),
        "",
        "Extracted by the ld_flash_addresses repository rule.",
        'Do not edit manually."""',
        "",
        "# All FLASH symbols as a dictionary {symbol_name: int_value}.",
        "FLASH_ADDRESSES = {",
    ]

    for name in sorted(addresses.keys()):
        bzl_lines.append('    "{}": {},'.format(name, addresses[name]))

    bzl_lines += [
        "}",
        "",
        "# Individual constants with leading underscores stripped so they are",
        "# valid top-level Starlark identifiers (e.g. APP_FLASH_START_ADDRESS).",
    ]

    for name in sorted(addresses.keys()):
        starlark_name = name.lstrip("_")
        bzl_lines.append("{} = {}".format(starlark_name, addresses[name]))

    bzl_lines.append("")  # trailing newline

    repository_ctx.file("flash_addresses.bzl", "\n".join(bzl_lines))
    repository_ctx.file("BUILD", 'exports_files(["flash_addresses.bzl"])\n')

_ld_flash_addresses = repository_rule(
    implementation = _ld_flash_addresses_impl,
    attrs = {
        "src": attr.label(
            mandatory = True,
            allow_single_file = True,
            doc = "Label for the preprocessed .ld file to parse " +
                  "(e.g. '//software/common/linker:common_ld').",
        ),
    },
    doc = """\\\r
Reads a preprocessed GNU ld linker script and extracts FLASH address symbol definitions\r
into a generated `flash_addresses.bzl` that can be loaded from BUILD files.\r
\r
The generated file exposes:\r
  - `FLASH_ADDRESSES`  \342\200\223 dict of {symbol_name: int_value} for all extracted symbols.\r
  - Individual constants \342\200\223 one Starlark name per symbol, leading underscores stripped.\r
""",
)

# ---------------------------------------------------------------------------
# Module extension (bzlmod)
# ---------------------------------------------------------------------------

def _flash_addresses_extension_impl(module_ctx):
    for mod in module_ctx.modules:
        for tag in mod.tags.from_ld:
            _ld_flash_addresses(
                name = tag.name,
                src = tag.src,
            )

_from_ld_tag = tag_class(
    attrs = {
        "name": attr.string(
            mandatory = True,
            doc = "Name for the generated repository, used in load() statements.",
        ),
        "src": attr.label(
            mandatory = True,
            doc = "Label for the preprocessed .ld file to parse.",
        ),
    },
)

flash_addresses_extension = module_extension(
    implementation = _flash_addresses_extension_impl,
    tag_classes = {"from_ld": _from_ld_tag},
    doc = """\\\r
Module extension that creates a FLASH-address repository from a GNU ld linker script.\r
\r
Each `from_ld` tag generates one repository containing `flash_addresses.bzl`.\r
""",
)
