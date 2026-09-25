#!/usr/bin/env python3
"""
Auto-generate rdd_sil.hpp and Sil_init.m from *_stream.h files.

This script parses multiple stream header files and generates:
1. rdd_sil.hpp - Consolidated C++ header for MATLAB MEX interface
2. Sil_init.m - MATLAB initialization function with all structures

Usage:
    python generate_sil_interface.py --variant srr7p --smc-path <path_to_smc.mat>
"""

import argparse
import re
from pathlib import Path
from typing import Dict, List
from dataclasses import dataclass, field

try:
    import scipy.io

    HAS_SCIPY = True
except ImportError:
    HAS_SCIPY = False
    print("WARNING: scipy not installed. Will use fallback for SMC parsing.")


@dataclass
class StructField:
    """Represents a single field in a struct."""

    name: str
    field_type: str
    array_dims: List[str] = field(default_factory=list)
    is_nested_struct: bool = False
    comment: str = ""


@dataclass
class StructDef:
    """Represents a complete structure definition."""

    name: str
    typedef_name: str
    fields: List[StructField]
    offset_comment: str = ""


class StreamHeaderParser:
    """Parse multiple *_stream.h files and extract structure definitions."""

    # C type to MATLAB type mapping
    C_TO_MATLAB_TYPE = {
        "uint8_t": "uint8",
        "uint16_t": "uint16",
        "uint32_t": "uint32",
        "uint64_t": "uint64",
        "int8_t": "int8",
        "int16_t": "int16",
        "int32_t": "int32",
        "int64_t": "int64",
        "float32_t": "single",
        "float": "single",
        "double": "double",
        "u16p16_T": "uint32",
        "s10p21_T": "int32",
        "u9p7_T": "uint16",
        "s7p8_T": "int16",
        "s8p23_T": "int32",
    }

    # Standard integer typedefs to include
    STANDARD_TYPEDEFS = [
        "int8_t",
        "int16_t",
        "int32_t",
        "int64_t",
        "uint8_t",
        "uint16_t",
        "uint32_t",
        "uint64_t",
        "int_least8_t",
        "int_least16_t",
        "int_least32_t",
        "int_least64_t",
        "uint_least8_t",
        "uint_least16_t",
        "uint_least32_t",
        "uint_least64_t",
        "int_fast8_t",
        "int_fast16_t",
        "int_fast32_t",
        "int_fast64_t",
        "uint_fast8_t",
        "uint_fast16_t",
        "uint_fast32_t",
        "uint_fast64_t",
        "intmax_t",
        "uintmax_t",
        "u16p16_T",
        "s10p21_T",
        "u9p7_T",
        "s7p8_T",
        "s8p23_T",
        "float32_t",
    ]

    def __init__(
        self,
        stream_header_dir: Path,
        smc_constants: Dict[str, int],
        target_endian: str = "auto",
    ):
        """Initialize the parser with stream header directory, SMC constants, and endian preference.

        Args:
            stream_header_dir: Directory containing *_stream.h files.
            smc_constants: Constants extracted from SMC or defaults.
            target_endian: "little", "big", or "auto" (default).
        """
        self.stream_dir = stream_header_dir
        self.smc_constants = smc_constants
        normalized_endian = (target_endian or "auto").strip().lower()
        self.target_endian = (
            normalized_endian if normalized_endian in {"auto", "little", "big"} else "auto"
        )
        self.global_endian = None
        self.structures: Dict[str, StructDef] = {}
        self.enums: Dict[str, List[str]] = {}
        self.all_constants: Dict[str, str] = {}

    def parse_headers(self, header_files: List):
        """Parse multiple stream headers."""
        print(f"Parsing {len(header_files)} header files...")

        # Resolve endianness once for the whole generation run.
        # Priority:
        #   1) explicit CLI argument (--endian little|big)
        #   2) stream_header.h macro definition
        #   3) default little-endian
        self.global_endian = self._detect_global_endian(header_files)
        print(f"Using endian mode: {self.global_endian}")

        for header_file in header_files:
            if isinstance(header_file, Path):
                filepath = header_file
            else:
                filepath = self.stream_dir / header_file

            if not filepath.exists():
                print(f"WARNING: {filepath.name} not found, skipping...")
                continue

            print(f"  - Parsing {filepath.name}")
            self._parse_single_header(filepath)

        print(f"Found {len(self.structures)} structures, {len(self.enums)} enums")

    def _detect_global_endian(self, header_files: List) -> str:
        """Detect endian once from config or stream_header.h."""
        if self.target_endian in {"little", "big"}:
            return self.target_endian

        # Prefer explicit macro from stream_header.h if available.
        stream_header_path = self.stream_dir / "stream_header.h"
        if stream_header_path.exists():
            try:
                content = stream_header_path.read_text(encoding="utf-8", errors="ignore")
                has_little_define = re.search(
                    r"^\s*#\s*define\s+LITTLE_ENDIAN_STRUCTURE\b", content, re.MULTILINE
                )
                has_big_define = re.search(
                    r"^\s*#\s*define\s+BIG_ENDIAN_STRUCTURE\b", content, re.MULTILINE
                )

                if has_little_define and not has_big_define:
                    return "little"
                if has_big_define and not has_little_define:
                    return "big"
            except Exception:
                pass

        # Fallback: scan provided headers for a clear local define.
        for header_file in header_files:
            filepath = (
                header_file if isinstance(header_file, Path) else (self.stream_dir / header_file)
            )
            if not filepath.exists():
                continue
            try:
                content = filepath.read_text(encoding="utf-8", errors="ignore")
            except Exception:
                continue

            has_little_define = re.search(
                r"^\s*#\s*define\s+LITTLE_ENDIAN_STRUCTURE\b", content, re.MULTILINE
            )
            has_big_define = re.search(
                r"^\s*#\s*define\s+BIG_ENDIAN_STRUCTURE\b", content, re.MULTILINE
            )

            if has_little_define and not has_big_define:
                return "little"
            if has_big_define and not has_little_define:
                return "big"

        return "little"

    def _parse_single_header(self, filepath: Path):
        """Parse a single header file."""
        content = filepath.read_text(encoding="utf-8", errors="ignore")

        # Extract #define constants (before big-endian extraction to get all defines)
        self._extract_defines(content)

        # Extract only the selected endian structure section while preserving other content
        content = self._extract_selected_endian_section(content)

        # Extract enums
        self._extract_enums(content)

        # Extract structures
        self._extract_structures(content)

    def _resolve_endian(self, content: str) -> str:
        """Resolve target endian for current parse call.

        Endianness is determined once globally and reused for all headers.
        """
        if self.global_endian in {"little", "big"}:
            return self.global_endian

        # Safety fallback, should rarely be used because parse_headers() sets global_endian.
        if self.target_endian in {"little", "big"}:
            return self.target_endian

        return "little"

    def _extract_selected_endian_section(self, content: str) -> str:
        """Normalize endian-guarded blocks to the selected branch.

        Supports both styles:
        1) New style:
           #if defined(LITTLE_ENDIAN_STRUCTURE)
             ...
           #elif defined(BIG_ENDIAN_STRUCTURE)
             ...
           #else
             ...
           #endif

        2) Legacy style:
           #ifndef LITTLE_ENDIAN_STRUCTURE
             ...
           #else
             ...
           #endif

        If no endian guard is found, returns content unchanged.
        """
        endian = self._resolve_endian(content)

        # New explicit endian style
        new_style = re.compile(
            r"#if\s+defined\s*\(\s*LITTLE_ENDIAN_STRUCTURE\s*\)\s*"
            r"(?P<little>.*?)"
            r"#elif\s+defined\s*\(\s*BIG_ENDIAN_STRUCTURE\s*\)\s*"
            r"(?P<big>.*?)"
            r"(?:#else\s*.*?\s*)?"
            r"#endif(?:\s*/\*[^*]*\*/)?",
            re.DOTALL,
        )

        match = new_style.search(content)
        if match:
            selected = match.group("little") if endian == "little" else match.group("big")
            return content[: match.start()] + selected + content[match.end() :]

        # Legacy style used by older generated headers.
        # NOTE: first branch historically contained little-endian fields in this project.
        legacy_style = re.compile(
            r"#ifndef\s+LITTLE_ENDIAN_STRUCTURE\s*"
            r"(?P<little>.*?)"
            r"#else\s*"
            r"(?P<big>.*?)"
            r"#endif(?:\s*/\*[^*]*\*/)?",
            re.DOTALL,
        )

        match = legacy_style.search(content)
        if match:
            selected = match.group("little") if endian == "little" else match.group("big")
            return content[: match.start()] + selected + content[match.end() :]

        return content

    def _extract_defines(self, content: str):
        """Extract #define constants."""
        define_pattern = r"#define\s+([A-Z_][A-Z0-9_]*)\s+\(?([0-9A-Z_]+)U?\)?"

        for match in re.finditer(define_pattern, content):
            const_name = match.group(1)
            const_value = match.group(2)

            # Skip include guards and non-numeric defines
            if const_name.endswith("_H") or const_name.endswith("_HPP"):
                continue

            # Remove C-style suffix (U, UL, etc.) for MATLAB compatibility
            const_value = const_value.rstrip("UL")

            # Store in all_constants (don't override SMC constants)
            if const_name not in self.smc_constants:
                self.all_constants[const_name] = const_value

    def _extract_enums(self, content: str):
        """Extract enum definitions."""
        enum_pattern = r"typedef\s+enum\s+(\w+)_Tag\s*{([^}]+)}\s*(\w+_T);"

        for match in re.finditer(enum_pattern, content, re.DOTALL):
            enum_body = match.group(2)
            enum_name = match.group(3)

            # Extract enum values
            values = []
            for line in enum_body.split(","):
                line = line.strip()
                if line and not line.startswith("//"):
                    value_name = line.split("=")[0].strip()
                    if value_name:
                        values.append(value_name)

            self.enums[enum_name] = values

    def _extract_structures(self, content: str):
        """Extract structure definitions."""
        struct_pattern = (
            r"typedef\s+struct\s+(\w+)_Tag\s*(?:/\*([^*]*)\*/)?\s*{([^}]+)}\s*(\w+_T);"
        )

        for match in re.finditer(struct_pattern, content, re.DOTALL):
            struct_tag = match.group(1)
            offset_comment = match.group(2).strip() if match.group(2) else ""
            struct_body = match.group(3)
            typedef_name = match.group(4)

            # Parse fields
            fields = self._parse_struct_fields(struct_body)

            struct_def = StructDef(
                name=struct_tag,
                typedef_name=typedef_name,
                fields=fields,
                offset_comment=offset_comment,
            )

            self.structures[typedef_name] = struct_def

    def _parse_struct_fields(self, struct_body: str) -> List[StructField]:
        """Parse individual fields from struct body."""
        fields = []
        field_pattern = r"(\w+(?:\s*\*)?)\s+(\w+)((?:\[[^\]]+\])*)\s*;\s*(?:/\*\s*(.*?)\s*\*/)?"

        for match in re.finditer(field_pattern, struct_body):
            field_type = match.group(1).strip()
            field_name = match.group(2).strip()
            array_part = match.group(3).strip()
            comment = match.group(4).strip() if match.group(4) else ""

            # Extract array dimensions
            array_dims = []
            if array_part:
                dim_matches = re.findall(r"\[([^\]]+)\]", array_part)
                array_dims = [dim.strip() for dim in dim_matches]

            # Check if it's a nested struct (ends with _T)
            is_nested = field_type.endswith("_T") and field_type not in self.STANDARD_TYPEDEFS

            field = StructField(
                name=field_name,
                field_type=field_type,
                array_dims=array_dims,
                is_nested_struct=is_nested,
                comment=comment,
            )

            fields.append(field)

        return fields

    def get_stream_struct_field_types(self) -> set:
        """Get all struct types that are direct fields of *_Stream_T structures."""
        stream_field_types = set()

        # Find all *_Stream_T structures
        for struct_name, struct_def in self.structures.items():
            if struct_name.endswith("_Stream_T"):
                # Extract field types from this stream struct
                for struct_field in struct_def.fields:
                    if (
                        struct_field.is_nested_struct
                        and struct_field.field_type in self.structures
                    ):
                        # This is a direct field type of a stream struct
                        stream_field_types.add(struct_field.field_type)

        return stream_field_types

    def resolve_constant(self, const_name: str) -> str:
        """Resolve a constant name to its value."""
        # First check SMC constants (priority)
        if const_name in self.smc_constants:
            return str(self.smc_constants[const_name])

        # Then check header #define constants
        if const_name in self.all_constants:
            return self.all_constants[const_name]

        # Return as-is if not found
        return const_name


class RddSilHppGenerator:
    """Generate rdd_sil.hpp file."""

    def __init__(
        self,
        parser: StreamHeaderParser,
        function_prototype: str,
        variant: str,
        stream_dir: Path,
        smc_version: str = None,
        stream_headers: List[str] = None,
    ):
        """Initialize the HPP generator with parser and function prototype."""
        self.parser = parser
        self.function_prototype = function_prototype
        self.variant = variant
        self.stream_dir = stream_dir
        self.smc_version = smc_version or "Unknown"
        self.stream_headers = stream_headers or []

    def generate(self, output_path: Path):
        """Generate the complete rdd_sil.hpp file."""
        print(f"Generating {output_path.name}...")

        lines = []

        # Header
        lines.extend(self._generate_header())

        # Constants from SMC (before typedefs to match reference)
        lines.extend(self._generate_constants())

        # Standard typedefs
        lines.extend(self._generate_typedefs())

        # Enums
        lines.extend(self._generate_enums())

        # Structures
        lines.extend(self._generate_structures())

        # Function prototype
        lines.extend(self._generate_function_prototype())

        # Footer
        lines.extend(self._generate_footer())

        # Write to file atomically to avoid corruption on failure
        temp_path = output_path.with_suffix(output_path.suffix + ".tmp")
        try:
            temp_path.write_text("\n".join(lines), encoding="utf-8")
            temp_path.replace(output_path)  # Atomic rename
        except Exception:
            # Clean up temp file if write/replace failed
            if temp_path.exists():
                temp_path.unlink()
            raise
        print(f"  [OK] Generated {output_path.name} ({len(lines)} lines)")

    def _generate_header(self) -> List[str]:
        """Generate file header."""
        from datetime import datetime

        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # Compute path relative to project root
        try:
            # Find project root by looking for .git or MODULE.bazel
            current = Path(__file__).parent
            project_root = None
            for parent in [current] + list(current.parents):
                if (parent / ".git").exists() or (parent / "MODULE.bazel").exists():
                    project_root = parent
                    break

            if project_root:
                rel_path = self.stream_dir.relative_to(project_root)
                stream_path_str = f"<prjRoot>/{str(rel_path).replace(chr(92), '/')}"
            else:
                # Fallback to relative from script
                script_dir = Path(__file__).parent
                rel_path = self.stream_dir.relative_to(script_dir)
                stream_path_str = str(rel_path).replace(chr(92), "/")
        except (ValueError, AttributeError):
            # If not relative, show as-is
            stream_path_str = str(self.stream_dir).replace(chr(92), "/")

        return (
            [
                "// rdd_sil.hpp",
                "//",
                "// ******************************************************************************",
                "// ** AUTO-GENERATED FILE - DO NOT MODIFY BY HAND                             **",
                "// ******************************************************************************",
                "//",
                "// Generated by: generate_sil_interface.py",
                f"// Variant:      {self.variant}",
                f"// Timestamp:    {timestamp}",
                f"// Stream dir:   {stream_path_str}",
                f"// SMC Version:  {self.smc_version}",
                "//",
                "// Stream headers used:",
            ]
            + [f"//   - {header}" for header in self.stream_headers]
            + [
                "//",
                "// To regenerate, run: generate_sil_files.m in MATLAB",
                "// ******************************************************************************",
                "",
                "#ifdef __cplusplus",
                'extern "C"',
                "{",
                "#endif",
                "",
                "// Force packed structures (no padding) to match C DLL layout",
                "#pragma pack(push, 2)",
                "",
            ]
        )

    def _generate_typedefs(self) -> List[str]:
        """Generate standard type definitions."""
        return [
            "   typedef signed char int8_t;",
            "   typedef short int16_t;",
            "   typedef int int32_t;",
            "   typedef long long int64_t;",
            "   typedef unsigned char uint8_t;",
            "   typedef unsigned short uint16_t;",
            "   typedef unsigned int uint32_t;",
            "   typedef unsigned long long uint64_t;",
            "",
            "   typedef signed char int_least8_t;",
            "   typedef short int_least16_t;",
            "   typedef int int_least32_t;",
            "   typedef long long int_least64_t;",
            "   typedef unsigned char uint_least8_t;",
            "   typedef unsigned short uint_least16_t;",
            "   typedef unsigned int uint_least32_t;",
            "   typedef unsigned long long uint_least64_t;",
            "",
            "   typedef signed char int_fast8_t;",
            "   typedef int int_fast16_t;",
            "   typedef int int_fast32_t;",
            "   typedef long long int_fast64_t;",
            "   typedef unsigned char uint_fast8_t;",
            "   typedef unsigned int uint_fast16_t;",
            "   typedef unsigned int uint_fast32_t;",
            "   typedef unsigned long long uint_fast64_t;",
            "",
            "   typedef long long intmax_t;",
            "   typedef unsigned long long uintmax_t;",
            "",
            "   typedef uint32_t u16p16_T;",
            "   typedef int32_t s10p21_T;",
            "   typedef uint16_t u9p7_T;",
            "   typedef int16_t s7p8_T;",
            "   typedef int32_t s8p23_T;",
            "   typedef float float32_t;",
            "",
        ]

    def _generate_constants(self) -> List[str]:
        """Generate constant definitions from SMC and extracted headers."""
        lines = []

        # Define order for constants (same as MATLAB for consistency)
        const_order = [
            "MAX_RANGE_BINS",
            "MAX_DETS_FIRST_PASS",
            "NUM_TX_CHANNELS",
            "NUM_RX_CHANNELS",
            "K_MAX",
            "AF_MAX_NUM_DET",
            "MAX_TARGET_REPORTS",
            "NO_OF_COEFF",
        ]

        # Calculate spacing for alignment
        max_name_len = max(len(name) for name in const_order)
        align_column = max_name_len + 9  # "#define " is 8 chars + 1 space

        for const_name in const_order:
            # Check SMC first, then extracted constants
            if const_name in self.parser.smc_constants:
                const_value = self.parser.smc_constants[const_name]
            elif const_name in self.parser.all_constants:
                const_value = self.parser.all_constants[const_name]
            else:
                continue

            # Calculate spacing to align values
            spaces_needed = align_column - len(const_name) - 8
            spaces = max(1, spaces_needed)
            lines.append(f"#define {const_name}{' ' * spaces}{const_value}")

        lines.append("")
        return lines

    def _generate_enums(self) -> List[str]:
        """Generate enum definitions."""
        if not self.parser.enums:
            return []

        lines = []

        for enum_name, values in self.parser.enums.items():
            # Extract base name without _T for the tag
            base_name = enum_name.replace("_T", "") if enum_name.endswith("_T") else enum_name
            lines.append(f"   typedef enum {base_name}_Tag")
            lines.append("   {")
            for i, value in enumerate(values):
                # Add blank line before last value (to match reference format)
                if i == len(values) - 1:
                    lines.append("")
                comma = "," if i < len(values) - 1 else ""
                lines.append(f"      {value}{comma}")
            lines.append(f"   }} {enum_name};")
            lines.append("")

        return lines

    def _generate_structures(self) -> List[str]:
        """Generate all structure definitions."""
        lines = []

        # Sort structures to ensure dependencies come first
        sorted_structs = self._topological_sort_structs()

        for struct_name in sorted_structs:
            struct_def = self.parser.structures[struct_name]
            lines.extend(self._generate_single_struct(struct_def))
            lines.append("")

        return lines

    def _topological_sort_structs(self) -> List[str]:
        # Build dependency graph
        dependencies = {}
        for struct_name, struct_def in self.parser.structures.items():
            deps = set()
            for struct_field in struct_def.fields:
                if (
                    struct_field.is_nested_struct
                    and struct_field.field_type in self.parser.structures
                ):
                    deps.add(struct_field.field_type)
            dependencies[struct_name] = deps

        # Topological sort (Kahn's algorithm)
        sorted_structs = []
        no_deps = [name for name, deps in dependencies.items() if not deps]
        remaining = set(dependencies.keys()) - set(no_deps)

        # Safety guard: algorithm should finish within number of nodes.
        max_iterations = len(dependencies) + 1
        iterations = 0

        while no_deps:
            iterations += 1
            if iterations > max_iterations:
                # Defensive fallback to avoid any unexpected non-terminating behavior.
                break

            no_deps.sort()  # Alphabetical for consistency
            node = no_deps.pop(0)
            sorted_structs.append(node)

            # Remove this node from all dependency lists of remaining structs
            for name in list(remaining):
                if node in dependencies[name]:
                    dependencies[name].remove(node)
                    if not dependencies[name]:
                        no_deps.append(name)
                        remaining.remove(name)

        # Handle any circular dependencies or remaining structs
        if remaining:
            sorted_structs.extend(sorted(remaining))

        return sorted_structs

    def _generate_single_struct(self, struct_def: StructDef) -> List[str]:
        """Generate a single structure definition."""
        lines = []

        # Comment with offset info - wrap long lines like the reference
        if struct_def.offset_comment:
            struct_line = (
                f"   typedef struct {struct_def.name}_Tag /*{struct_def.offset_comment}*/"
            )
            # If line is too long (>125 chars), wrap the comment
            if len(struct_line) > 125:
                # Find where "bytes*/" starts in the comment
                if "bytes*/" in struct_line:
                    # Split before "bytes*/"
                    base_part = struct_line[: struct_line.rfind(" bytes*/")]
                    lines.append(base_part)
                    # Add continuation line with proper indentation (56 spaces to align)
                    lines.append(" " * 56 + "bytes*/")
                else:
                    lines.append(struct_line)
            else:
                lines.append(struct_line)
        else:
            lines.append(f"   typedef struct {struct_def.name}_Tag")

        lines.append("   {")

        # Generate fields
        for struct_field in struct_def.fields:
            field_line = f"      {struct_field.field_type} {struct_field.name}"

            # Add array dimensions - keep as symbolic constants, don't resolve to numbers
            for dim in struct_field.array_dims:
                # Don't resolve - keep symbolic names like MAX_RANGE_BINS
                field_line += f"[{dim}]"

            field_line += ";"

            # Add inline comment if present
            if struct_field.comment:
                # Pad to align comments at column 31 (like the reference file)
                padding = " " * (31 - len(field_line))
                if len(padding) < 1:
                    padding = " "
                field_line += padding + f"/* {struct_field.comment} */"

            lines.append(field_line)

        lines.append(f"   }} {struct_def.typedef_name};")

        return lines

    def _format_function_prototype(self, prototype: str) -> List[str]:
        """Format a single-line function prototype into multi-line format."""
        # Remove the trailing semicolon temporarily
        proto = prototype.rstrip(";").strip()

        # Extract return type and function name with opening paren
        # Pattern: "bool Rdd_To_Detection_Configuration("
        match = re.match(r"(\w+)\s+(\w+)\s*\((.*)\)", proto)
        if not match:
            # Fallback - return as single line
            return [f"   {prototype}", ""]

        return_type = match.group(1)
        func_name = match.group(2)
        params_str = match.group(3).strip()

        # Split parameters by comma
        params = [p.strip() for p in params_str.split(",")]

        if not params:
            return [f"   {prototype}", ""]

        # Format first line with first parameter(s) - aim for ~120 chars
        first_line = f"   {return_type} {func_name}({params[0]}"
        param_idx = 1

        # Add more parameters to first line if they fit
        while param_idx < len(params):
            test_line = first_line + f", {params[param_idx]}"
            if len(test_line) > 120:
                break
            first_line = test_line
            param_idx += 1

        lines = []
        if param_idx < len(params):
            # Need continuation lines
            first_line += ","
            lines.append(first_line)

            # Continuation indent - align with opening paren
            indent_len = len(f"   {return_type} {func_name}(")
            indent = " " * indent_len

            # Add remaining parameters
            while param_idx < len(params):
                is_last = param_idx == len(params) - 1
                if is_last:
                    lines.append(f"{indent}{params[param_idx]});")
                    break  # Exit loop after last param
                else:
                    # Try to fit multiple params on continuation lines
                    cont_line = f"{indent}{params[param_idx]}"
                    param_idx += 1
                    while param_idx < len(params):
                        test_line = cont_line + f", {params[param_idx]}"
                        if len(test_line) > 120:
                            break
                        cont_line = test_line
                        param_idx += 1

                    is_last = param_idx >= len(params)
                    if is_last:
                        lines.append(f"{cont_line});")
                    else:
                        lines.append(f"{cont_line},")
        else:
            # All params fit on first line
            lines.append(first_line + ");")

        lines.append("")
        return lines

    def _generate_function_prototype(self) -> List[str]:
        """Generate the function prototype with line wrapping."""
        # Use the extracted function prototype from rdd_sil_api.hpp
        return self._format_function_prototype(self.function_prototype)

    def _generate_footer(self) -> List[str]:
        """Generate file footer."""
        return [
            "// Restore default packing",
            "#pragma pack(pop)",
            "",
            "#ifdef __cplusplus",
            "}",
            "#endif",
        ]


class SilInitMGenerator:
    """Generate Sil_init.m file."""

    def __init__(
        self,
        parser: StreamHeaderParser,
        function_params: List[Dict[str, str]] = None,
        variant: str = "",
        stream_dir: Path = None,
        smc_version: str = None,
        stream_headers: List[str] = None,
    ):
        """Initialize the MATLAB generator with parser and function parameters."""
        self.parser = parser
        self.function_params = function_params or []
        self.variant = variant
        self.stream_dir = stream_dir
        self.smc_version = smc_version or "Unknown"
        self.stream_headers = stream_headers or []

    def generate(self, output_path: Path):
        """Generate the complete Sil_init.m file."""
        print(f"Generating {output_path.name}...")

        lines = []

        # Header
        lines.extend(self._generate_header())

        # Constants section
        lines.extend(self._generate_constants())

        # Structure declarations
        lines.extend(self._generate_all_structures())

        # Build final output struct
        lines.extend(self._generate_output_struct())

        # Footer
        lines.extend(self._generate_footer())

        # Write to file atomically to avoid corruption on failure
        temp_path = output_path.with_suffix(output_path.suffix + ".tmp")
        try:
            temp_path.write_text("\n".join(lines), encoding="utf-8")
            temp_path.replace(output_path)  # Atomic rename
        except Exception:
            # Clean up temp file if write/replace failed
            if temp_path.exists():
                temp_path.unlink()
            raise
        print(f"  [OK] Generated {output_path.name} ({len(lines)} lines)")

    def _generate_header(self) -> List[str]:
        """Generate file header."""
        from datetime import datetime

        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # Compute path relative to project root
        try:
            # Find project root by looking for .git or MODULE.bazel
            current = Path(__file__).parent
            project_root = None
            for parent in [current] + list(current.parents):
                if (parent / ".git").exists() or (parent / "MODULE.bazel").exists():
                    project_root = parent
                    break

            if project_root and self.stream_dir:
                rel_path = self.stream_dir.relative_to(project_root)
                stream_path_str = f"<prjRoot>/{str(rel_path).replace(chr(92), '/')}"
            elif self.stream_dir:
                # Fallback to relative from script
                script_dir = Path(__file__).parent
                rel_path = self.stream_dir.relative_to(script_dir)
                stream_path_str = str(rel_path).replace(chr(92), "/")
            else:
                stream_path_str = "N/A"
        except (ValueError, AttributeError):
            # If not relative or stream_dir is None, show as-is
            stream_path_str = (
                str(self.stream_dir).replace(chr(92), "/") if self.stream_dir else "N/A"
            )

        return (
            [
                "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
                "%    Sil_init.m - SIL Input Initialization",
                "%",
                "% ****************************************************************************",
                "% ** AUTO-GENERATED FILE - DO NOT MODIFY BY HAND                           **",
                "% ****************************************************************************",
                "%",
                "% Generated by: generate_sil_interface.py",
                f"% Variant:      {self.variant}",
                f"% Timestamp:    {timestamp}",
                f"% Stream dir:   {stream_path_str}",
                f"% SMC Version:  {self.smc_version}",
                "%",
                "% Stream headers used:",
            ]
            + [f"%   - {header}" for header in self.stream_headers]
            + [
                "%",
                "% To regenerate, run: generate_sil_files.m in MATLAB",
                "% ****************************************************************************",
                "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
                "function Sil_Input = Sil_init(SMC)",
                "   %% Constants",
                "   % SMC.Configuration.BB_Configuration",
            ]
        )

    def _generate_constants(self) -> List[str]:
        """Generate constants from SMC."""
        lines = []

        # Define specific order and SMC path mapping
        const_order = [
            ("MAX_RANGE_BINS", "MAX_RANGE_BINS"),
            ("MAX_DETS_FIRST_PASS", "MAX_DETS_FIRST_PASS"),
            ("NUM_TX_CHANNELS", "NUM_TX_CHANNELS"),
            ("NUM_RX_CHANNELS", "NUM_RX_CHANNELS"),
            ("K_MAX", "MAX_DOPPLER_FFT_SIZE"),  # K_MAX comes from MAX_DOPPLER_FFT_SIZE
            ("AF_MAX_NUM_DET", "AF_MAX_NUM_DET"),
            ("MAX_TARGET_REPORTS", "AF_MAX_NUM_DET"),  # Also from AF_MAX_NUM_DET
        ]

        # Generate constants in specific order with proper SMC paths and alignment
        for const_name, smc_path in const_order:
            const_value = self.parser.smc_constants.get(const_name, "???")
            # Build the line: constant = SMC path
            line_start = f"   {const_name} = SMC.Configuration.BB_Configuration.{smc_path}.VALUE;"
            # Align comment at column 94 (match reference exactly)
            comment_col = 94
            spaces_needed = comment_col - len(line_start)
            if spaces_needed > 0:
                spacing = " " * spaces_needed
            else:
                spacing = "  "  # Minimum 2 spaces
            lines.append(f"{line_start}{spacing}% {const_value};")

        # Add NO_OF_COEFF if available from parsed headers
        if "NO_OF_COEFF" in self.parser.all_constants:
            no_of_coeff_value = self.parser.all_constants["NO_OF_COEFF"]
            lines.append(f"   NO_OF_COEFF = {no_of_coeff_value};")
        elif "NO_OF_COEFF" in self.parser.smc_constants:
            no_of_coeff_value = self.parser.smc_constants["NO_OF_COEFF"]
            lines.append(f"   NO_OF_COEFF = {no_of_coeff_value};")

        lines.extend(
            [
                "",
                "   %% Declare and initialize structs",
                "",
            ]
        )

        return lines

    def _generate_all_structures(self) -> List[str]:
        lines = []

        # Sort structures
        sorted_structs = self._topological_sort_structs()

        # Only generate top-level (referenceable) structures
        for struct_name in sorted_structs:
            if self._is_referenceable_struct(struct_name):
                struct_def = self.parser.structures[struct_name]
                lines.extend(self._generate_struct_init(struct_def))
                lines.append("")

        return lines

    def _topological_sort_structs(self) -> List[str]:
        # Build dependency graph
        dependencies = {}
        for struct_name, struct_def in self.parser.structures.items():
            deps = set()
            for struct_field in struct_def.fields:
                if (
                    struct_field.is_nested_struct
                    and struct_field.field_type in self.parser.structures
                ):
                    deps.add(struct_field.field_type)
            dependencies[struct_name] = deps

        # Topological sort (Kahn's algorithm)
        sorted_structs = []
        no_deps = [name for name, deps in dependencies.items() if not deps]
        remaining = set(dependencies.keys()) - set(no_deps)

        # Safety guard: algorithm should finish within number of nodes.
        max_iterations = len(dependencies) + 1
        iterations = 0

        while no_deps:
            iterations += 1
            if iterations > max_iterations:
                # Defensive fallback to avoid any unexpected non-terminating behavior.
                break

            no_deps.sort()  # Alphabetical for consistency
            node = no_deps.pop(0)
            sorted_structs.append(node)

            # Remove this node from all dependency lists of remaining structs
            for name in list(remaining):
                if node in dependencies[name]:
                    dependencies[name].remove(node)
                    if not dependencies[name]:
                        no_deps.append(name)
                        remaining.remove(name)

        # Handle any circular dependencies or remaining structs
        if remaining:
            sorted_structs.extend(sorted(remaining))

        return sorted_structs

    def _generate_struct_init(self, struct_def: StructDef) -> List[str]:
        """Generate MATLAB struct initialization matching reference format."""
        lines = [f"   % {struct_def.typedef_name}"]

        # Build field list
        field_strings = []
        for struct_field in struct_def.fields:
            field_init = self._generate_field_init(struct_field)
            field_strings.append(f"'{struct_field.name}', {field_init}")

        # Start the struct definition
        first_line = f"   {struct_def.typedef_name} = struct("
        current_line = first_line
        continuation_indent = " " * 25  # Continuation lines indent to column 25

        for i, field_str in enumerate(field_strings):
            is_last = i == len(field_strings) - 1

            # Try adding this field to current line
            if current_line == first_line or current_line == continuation_indent:
                # Starting new content line
                test_line = current_line + field_str
            else:
                # Adding to existing content
                test_line = current_line + ", " + field_str

            # Check if we need to wrap
            if is_last:
                # Last field - add closing
                test_line += ");"
                if len(test_line) <= 100:  # Reference seems to use ~100 char limit
                    lines.append(test_line)
                else:
                    # Field doesn't fit, wrap it
                    if current_line != first_line and current_line != continuation_indent:
                        lines.append(current_line + ", ...")
                    else:
                        lines.append(current_line + " ...")
                    lines.append(continuation_indent + field_str + ");")
            else:
                # Not last field
                if len(test_line) <= 100:
                    current_line = test_line
                else:
                    # Line too long, need to wrap
                    if current_line == first_line or current_line == continuation_indent:
                        # Nothing on this line yet, must fit the field
                        lines.append(current_line + field_str + ", ...")
                    else:
                        # Finish current line and start new one
                        lines.append(current_line + ", ...")
                        lines.append(continuation_indent + field_str + ", ...")
                    current_line = continuation_indent

        # If there's remaining content not yet added
        if (
            current_line != first_line
            and current_line != continuation_indent
            and not lines[-1].endswith(");")
        ):
            lines.append(current_line + ");")

        return lines

    def _generate_field_init(self, struct_field: StructField) -> str:
        """Generate initialization value for a field."""
        if struct_field.is_nested_struct:
            # Determine if struct should be referenced or inlined
            # Strategy: If a struct is ONLY used as a field in other structs and never standalone,
            # it should be inlined. Otherwise, it should be referenced.
            typedef_name = struct_field.field_type

            # Check if this struct is a top-level defined struct (should be referenced)
            # A struct is "top-level" if it appears in the topologically sorted list
            # AND is not just embedded in other structs
            referenceable = self._is_referenceable_struct(typedef_name)

            if referenceable:
                # Reference the already-defined struct
                return typedef_name
            else:
                # Generate inline anonymous struct
                if typedef_name in self.parser.structures:
                    return self._generate_inline_struct(self.parser.structures[typedef_name])
                else:
                    # Fallback - just reference it
                    return typedef_name

        # Get MATLAB type
        matlab_type = self.parser.C_TO_MATLAB_TYPE.get(struct_field.field_type, "double")

        if struct_field.array_dims:
            # Array field
            resolved_dims = []
            for dim in struct_field.array_dims:
                resolved_dim = self.parser.resolve_constant(dim)
                resolved_dims.append(resolved_dim)

            # MATLAB expects column vectors for 1D arrays: zeros(N, 1, 'type')
            # Multi-dimensional arrays should NOT have the extra dimension
            if len(resolved_dims) == 1:
                # 1D array - add ', 1' to make it a column vector
                dims_str = resolved_dims[0] + ", 1"
            else:
                # Multi-dimensional array - use dimensions as-is
                dims_str = ", ".join(resolved_dims)

            return f"zeros({dims_str}, '{matlab_type}')"
        else:
            # Scalar field
            return f"{matlab_type}(0)"

    def _is_referenceable_struct(self, struct_name: str) -> bool:
        """Determine if a struct should be referenced (not inlined)."""
        # Structs are INLINED if they're ONLY used as a nested field in exactly ONE parent
        # Structs are REFERENCED if they:
        #   1. End with _Stream_T (top-level stream structs)
        #   2. Are used by multiple parents (shared dependencies)
        #   3. Are not used by anyone (standalone definitions)
        #   4. Are direct fields of *_Stream_T structures (major data structures)
        if struct_name not in self.parser.structures:
            return False

        # Pattern 1: All *_Stream_T structures are top-level
        if struct_name.endswith("_Stream_T"):
            return True

        # Pattern 2: Stream_Hdr_T is shared by all streams, make it top-level
        if struct_name == "Stream_Hdr_T":
            return True

        # Pattern 3: Only direct fields of Rdd_Stream_T are top-level
        # (Look_Data_T, RDD_Data_T, Rfft_Data_T)
        if "Rdd_Stream_T" in self.parser.structures:
            rdd_stream = self.parser.structures["Rdd_Stream_T"]
            for struct_field in rdd_stream.fields:
                if struct_field.is_nested_struct and struct_field.field_type == struct_name:
                    return True

        # Pattern 4: Nested fields of RDD_Data_T are also top-level
        # (Exec_Spec_Version_T, Rdd_FP_Targets_Saturation_Data_T)
        if "RDD_Data_T" in self.parser.structures:
            rdd_data = self.parser.structures["RDD_Data_T"]
            for struct_field in rdd_data.fields:
                if struct_field.is_nested_struct and struct_field.field_type == struct_name:
                    return True

        # Pattern 5: Otherwise inline it
        return False

    def _generate_inline_struct(self, struct_def: StructDef) -> str:
        """Generate an inline anonymous struct definition."""
        field_inits = []
        for struct_field in struct_def.fields:
            field_init = self._generate_field_init(struct_field)
            field_inits.append(f"'{struct_field.name}', {field_init}")

        # Format as inline struct - all on one level of continuation
        return (
            "struct( ...\n"
            + " " * 57
            + (", ...\n" + " " * 57).join(field_inits)
            + " ...\n"
            + " " * 56
            + ")"
        )

    def _generate_output_struct(self) -> List[str]:
        """Generate final output structure."""
        lines = []

        # Extract struct types from function parameters (excluding basic types)
        basic_types = {
            "uint8_t",
            "uint16_t",
            "uint32_t",
            "uint64_t",
            "int8_t",
            "int16_t",
            "int32_t",
            "int64_t",
            "float",
            "double",
            "bool",
        }

        output_fields = []
        added_structs = set()  # Track which structs we've already added
        pointer_vars = []  # Track pointer variable declarations needed

        # Build output fields from function parameters
        for param in self.function_params:
            param_type = param["type"]
            param_name = param["name"]

            # Check if it's a struct type (ends with _T and not a basic type)
            if param_type.endswith("_T") and param_type not in basic_types:
                # Handle look_id specially (enum type)
                if param_name == "look_id":
                    output_fields.append("'look_id', uint32(0)")
                # Handle struct types
                elif param_type in self.parser.structures:
                    # Generate field name from struct type name, not parameter name
                    # E.g., Detection_Stream_T -> detection_stream
                    field_name = param_type.replace("_T", "").lower()

                    # Handle duplicates (e.g., Rdd_Stream_T appears twice as input and output)
                    if param_type not in added_structs:
                        output_fields.append(f"'{field_name}', {param_type}")
                        added_structs.add(param_type)
                    else:
                        # For second occurrence, add '_out' suffix
                        output_fields.append(f"'{field_name}_out', {param_type}")
            elif param_type in basic_types and param["is_pointer"]:
                # Handle basic type pointers (e.g., uint8_t *)
                # Use a simple naming based on the pointer purpose
                # The only uint8_t* in the function is radar position
                field_name = "radar_position"
                var_name = "radar_pos"

                # Determine MATLAB pointer type
                matlab_ptr_type = f"{param_type.replace('_t', '')}Ptr"  # e.g., uint8_t -> uint8Ptr

                # Store pointer variable info
                pointer_vars.append(
                    {
                        "var_name": var_name,
                        "ptr_type": matlab_ptr_type,
                        "base_type": param_type.replace("_t", ""),  # uint8_t -> uint8
                        "field_name": field_name,
                    }
                )

                # Add to output fields
                output_fields.append(f"'{field_name}', {var_name}")

        # Generate pointer declarations if any
        if pointer_vars:
            for ptr_var in pointer_vars:
                if lines:  # Add separator if not first item
                    lines.append("")
                # Format comment: "Radar position pointer" (capitalize first letter of each word)
                comment_text = ptr_var["field_name"].replace("_", " ").title()
                lines.append(f"   %% {comment_text} pointer")
                lines.append(
                    f"   {ptr_var['var_name']} = libpointer('{ptr_var['ptr_type']}', {ptr_var['base_type']}(0));"
                )

        if lines:  # Add separator before Sil_Input if we added pointer declarations
            lines.append("")
        lines.append("   % Sil_Input")

        lines.append("   Sil_Input = struct( ...")

        for i, output_field in enumerate(output_fields):
            is_last = i == len(output_fields) - 1
            if is_last:
                lines.append(f"                      {output_field});")
            else:
                lines.append(f"                      {output_field}, ...")

        return lines

    def _generate_footer(self) -> List[str]:
        """Generate file footer."""
        return [
            "",
            "end",
            "",
        ]


class SMCParser:
    """Parse SMC MATLAB file to extract constants."""

    @staticmethod
    def parse_smc_file(smc_path: Path, variant: str) -> Dict[str, int]:
        """Parse SMC .mat file and extract constants."""
        constants = {}

        if not smc_path.exists():
            print(f"WARNING: SMC file not found: {smc_path}")
            return SMCParser.get_default_constants(variant)

        if not HAS_SCIPY:
            print("WARNING: scipy not available, using default constants")
            return SMCParser.get_default_constants(variant)

        try:
            print(f"Loading SMC file: {smc_path}")
            smc = scipy.io.loadmat(str(smc_path), struct_as_record=False, squeeze_me=True)

            # Navigate SMC structure
            config = smc["SMC"].Configuration.BB_Configuration

            # Extract constants
            constants["MAX_RANGE_BINS"] = int(config.MAX_RANGE_BINS.VALUE)
            constants["MAX_DETS_FIRST_PASS"] = int(config.MAX_DETS_FIRST_PASS.VALUE)
            constants["NUM_TX_CHANNELS"] = int(config.NUM_TX_CHANNELS.VALUE)
            constants["NUM_RX_CHANNELS"] = int(config.NUM_RX_CHANNELS.VALUE)
            constants["K_MAX"] = int(config.MAX_DOPPLER_FFT_SIZE.VALUE)
            constants["AF_MAX_NUM_DET"] = int(config.AF_MAX_NUM_DET.VALUE)
            constants["MAX_TARGET_REPORTS"] = int(config.AF_MAX_NUM_DET.VALUE)
            # NO_OF_COEFF is parsed from header files (vse_stream.h)

            print(f"  [OK] Extracted {len(constants)} constants from SMC")

        except Exception as e:
            print(f"ERROR parsing SMC file: {e}")
            print("Using default constants instead")
            return SMCParser.get_default_constants(variant)

        return constants

    @staticmethod
    def get_default_constants(variant: str) -> Dict[str, int]:
        """Get default constants if SMC parsing fails."""
        return {
            "MAX_RANGE_BINS": 385,
            "MAX_DETS_FIRST_PASS": 512,
            "NUM_TX_CHANNELS": 4,
            "NUM_RX_CHANNELS": 4,
            "K_MAX": 256,
            "AF_MAX_NUM_DET": 768,
            "MAX_TARGET_REPORTS": 768,
        }


def extract_function_prototype_from_api(api_header_path: Path) -> str:
    """Extract function prototype from rdd_sil_api.hpp."""
    try:
        content = api_header_path.read_text()
        pattern = r"(DLL_EXPORT\s+)?bool\s+Rdd_To_Detection_Configuration\s*\([^;]+\);"
        match = re.search(pattern, content, re.DOTALL | re.MULTILINE)

        if match:
            func_decl = match.group(0)
            # Remove DLL_EXPORT prefix if present
            func_decl = re.sub(r"DLL_EXPORT\s+", "", func_decl)
            # Normalize whitespace
            func_decl = re.sub(r"\s+", " ", func_decl)
            # Remove extra spaces around punctuation
            func_decl = re.sub(r"\s*\(\s*", "(", func_decl)
            func_decl = re.sub(r"\s*\)\s*;", ");", func_decl)
            func_decl = re.sub(r"\s*,\s*", ", ", func_decl)
            return func_decl.strip()
        else:
            print(f"WARNING: Could not find function prototype in {api_header_path}")
            return None
    except Exception as e:
        print(f"ERROR reading API header {api_header_path}: {e}")
        return None


def get_default_function_prototype() -> str:
    """Return the default function prototype when API header is not available."""
    return (
        "bool Rdd_To_Detection_Configuration("
        "Radar_Look_T look_id, Rdd_Stream_T *p_rdd_data_in, Rdd_Stream_T *p_rdd_data_out, "
        "Detection_Stream_T *p_af_det_output, Down_selection_Stream_T *p_af_ds_det_output, "
        "Detection_Debug_Stream_T *p_af_det_debug_output, Vse_Stream_T *p_af_vse_stream_input, "
        "uint8_t *p_af_radar_position_input);"
    )


def extract_parameter_types_from_prototype(prototype: str) -> List[Dict[str, str]]:
    """Extract parameter types and names from function prototype."""
    proto = prototype.rstrip(";").strip()
    proto = re.sub(r"\s+", " ", proto)

    match = re.match(r"\w+\s+\w+\s*\((.*)\)", proto)
    if not match:
        return []

    params_str = match.group(1).strip()
    if not params_str:
        return []

    params = [p.strip() for p in params_str.split(",")]

    param_list = []
    for param in params:
        param = param.strip()
        param_match = re.match(r"([\w_]+)\s*\*?\s*([\w_]+)", param)
        if param_match:
            param_type = param_match.group(1)
            param_name = param_match.group(2)
            param_list.append({"type": param_type, "name": param_name, "is_pointer": "*" in param})

    return param_list


def main():
    """Main entry point for SIL interface file generation."""
    parser = argparse.ArgumentParser(
        description="Auto-generate rdd_sil.hpp and Sil_init.m from stream headers"
    )
    parser.add_argument(
        "--variant",
        type=str,
        required=True,
        choices=["srr7e", "srr7p", "flr7", "flr7v3"],
        help="Radar variant",
    )
    parser.add_argument("--smc-path", type=str, help="Path to SMC .mat file (optional)")
    parser.add_argument("--smc-version", type=str, help="SMC version/name (optional)")
    parser.add_argument(
        "--stream-dir",
        type=str,
        default="../../../../../../software/common",
        help="Path to stream headers directory",
    )
    parser.add_argument(
        "--output-dir", type=str, default=".", help="Output directory for generated files"
    )
    parser.add_argument(
        "--endian",
        type=str,
        default="auto",
        choices=["auto", "little", "big"],
        help="Endian branch to parse from guarded stream headers",
    )
    parser.add_argument("--max-range-bins", type=int, help="MAX_RANGE_BINS constant value")
    parser.add_argument(
        "--max-dets-first-pass", type=int, help="MAX_DETS_FIRST_PASS constant value"
    )
    parser.add_argument("--num-tx-channels", type=int, help="NUM_TX_CHANNELS constant value")
    parser.add_argument("--num-rx-channels", type=int, help="NUM_RX_CHANNELS constant value")
    parser.add_argument("--k-max", type=int, help="K_MAX constant value")
    parser.add_argument("--af-max-num-det", type=int, help="AF_MAX_NUM_DET constant value")
    parser.add_argument("--max-target-reports", type=int, help="MAX_TARGET_REPORTS constant value")

    args = parser.parse_args()

    # Resolve paths
    script_dir = Path(__file__).parent
    stream_dir = (script_dir / args.stream_dir).resolve()
    output_dir = Path(args.output_dir).resolve()

    # Output filenames
    hpp_filename = "rdd_sil.hpp"
    matlab_filename = "Sil_init.m"

    print("=" * 70)
    print("SIL Interface Generator")
    print("=" * 70)
    print(f"Variant: {args.variant}")
    print(f"Stream headers: {stream_dir}")
    print(f"Output directory: {output_dir}")
    print()

    # Get constants from command-line arguments or fallback to defaults
    smc_constants = {}

    if args.max_range_bins is not None:
        # Use constants from command-line (passed from MATLAB)
        print("Using SMC constants from MATLAB:")
        smc_constants["MAX_RANGE_BINS"] = args.max_range_bins
        smc_constants["MAX_DETS_FIRST_PASS"] = args.max_dets_first_pass
        smc_constants["NUM_TX_CHANNELS"] = args.num_tx_channels
        smc_constants["NUM_RX_CHANNELS"] = args.num_rx_channels
        smc_constants["K_MAX"] = args.k_max
        smc_constants["AF_MAX_NUM_DET"] = args.af_max_num_det
        smc_constants["MAX_TARGET_REPORTS"] = args.max_target_reports
        print(f"  MAX_RANGE_BINS: {smc_constants['MAX_RANGE_BINS']}")
        print(f"  MAX_DETS_FIRST_PASS: {smc_constants['MAX_DETS_FIRST_PASS']}")
        print(f"  NUM_TX_CHANNELS: {smc_constants['NUM_TX_CHANNELS']}")
        print(f"  NUM_RX_CHANNELS: {smc_constants['NUM_RX_CHANNELS']}")
        print(f"  K_MAX: {smc_constants['K_MAX']}")
        print(f"  AF_MAX_NUM_DET: {smc_constants['AF_MAX_NUM_DET']}")
        print(f"  MAX_TARGET_REPORTS: {smc_constants['MAX_TARGET_REPORTS']}")
    elif args.smc_path:
        # Fallback: Try to parse SMC file if path provided
        smc_path = Path(args.smc_path)
        print(f"Loading SMC file: {smc_path.name}")
        smc_constants = SMCParser.parse_smc_file(smc_path, args.variant)
        print(f"  [OK] Loaded {len(smc_constants)} constants from SMC file")
    else:
        # Last resort: use hardcoded defaults
        print("No SMC constants provided, using hardcoded defaults")
        smc_constants = SMCParser.get_default_constants(args.variant)

    print()

    # List of required header files to parse
    stream_headers = [
        "stream_header.h",
        "radar_look_types.h",
        "rdd_stream.h",
        "detection_stream.h",
        "detection_debug_stream.h",
        "down_selection_stream.h",
        "vse_stream.h",
    ]

    # Parse headers
    header_parser = StreamHeaderParser(stream_dir, smc_constants, target_endian=args.endian)
    header_parser.parse_headers(stream_headers)

    print()

    # Extract function prototype from rdd_sil_api.hpp
    api_header_path = (
        Path(__file__).parent.parent.parent.parent.parent.parent
        / "sil"
        / "rsp_sil"
        / "main"
        / "rsp_wrapper_interface"
        / "rdd_sil_interface"
        / "rdd_sil_api.hpp"
    )

    function_prototype = None
    if api_header_path.exists():
        print(f"Extracting function prototype from: {api_header_path.name}")
        function_prototype = extract_function_prototype_from_api(api_header_path)
        if function_prototype:
            print(f"  [OK] Found: {function_prototype[:80]}...")
        else:
            print("  ⚠ Extraction failed, using default prototype")
    else:
        print(f"WARNING: API header not found at: {api_header_path}")
        print("Using default function prototype")

    if not function_prototype:
        function_prototype = get_default_function_prototype()

    # Extract parameter types from function prototype
    function_params = extract_parameter_types_from_prototype(function_prototype)
    print(f"  [OK] Extracted {len(function_params)} parameters from function prototype")

    print()
    hpp_generator = RddSilHppGenerator(
        header_parser,
        function_prototype,
        args.variant,
        stream_dir,
        args.smc_version,
        stream_headers,
    )
    hpp_generator.generate(output_dir / hpp_filename)

    print()

    # Generate Sil_init.m
    matlab_generator = SilInitMGenerator(
        header_parser, function_params, args.variant, stream_dir, args.smc_version, stream_headers
    )
    matlab_generator.generate(output_dir / matlab_filename)

    print()
    print("=" * 70)
    print("[OK] Generation complete!")
    print("=" * 70)


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print()
        print("=" * 70)
        print("[ERROR] Generation failed!")
        print("=" * 70)
        print(f"Error: {e}")
        print()
        print("The existing output files (if any) remain unchanged.")
        print("Please fix the error and re-run the script.")
        print("=" * 70)
        import sys

        sys.exit(1)
