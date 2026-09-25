"""Gen8 MCP Server - Minimal repo scanner.

Rules/logic live in .github/ (instructions, skills, prompts).
This server ONLY scans files and returns data. No rules. No validation logic.
stdlib only.
"""

import inspect
import json
import re
import sys
from pathlib import Path
from typing import Any, Dict, Optional

REPO = Path(__file__).resolve().parent.parent


def scan_interfaces() -> Dict[str, Any]:
    """Return all DCS_X enum entries from IT headers."""
    results = {}
    for core, header in [
        ("bbe32", REPO / "software/bbe32/integration_test/dsp_integration_test.h"),
        ("r52", REPO / "software/r52/integration_test/master_integration_test.h"),
    ]:
        if not header.exists():
            results[core] = []
            continue
        text = header.read_text(encoding="utf-8", errors="replace")
        results[core] = [
            {"name": m[0], "id": int(m[1])}
            for m in re.findall(r"DCS_X\(\s*(\w+)\s*,\s*(\d+)U?\)", text)
        ]
    return results


def scan_hooks() -> Dict[str, Any]:
    """Return SIT_ macro names from hook headers."""
    results = {}
    for core, header in [
        ("bbe32", REPO / "software/bbe32/integration_test/dsp_it_macros_test.h"),
        ("r52", REPO / "software/r52/integration_test/master_it_macros_test.h"),
    ]:
        if not header.exists():
            results[core] = []
            continue
        text = header.read_text(encoding="utf-8", errors="replace")
        results[core] = re.findall(r"#define\s+(SIT_\w+)\(", text)
    return results


def scan_memory() -> Dict[str, Any]:
    """Return SRAM1 usage stats from dsp_integration_test.c."""
    src = REPO / "software/bbe32/integration_test/dsp_integration_test.c"
    if not src.exists():
        return {"error": "file not found"}
    text = src.read_text(encoding="utf-8", errors="replace")
    lines = text.split("\n")
    return {
        "sram1_bss": sum(1 for ln in lines if "SRAM1_BSS" in ln and "#define" not in ln),
        "sram1_data": sum(1 for ln in lines if "SRAM1_DATA" in ln and "#define" not in ln),
        "sram1_text": sum(1 for ln in lines if "SRAM1_TEXT" in ln and "#define" not in ln),
        "statics_without_sram1": sum(
            1
            for ln in lines
            if ln.startswith("static ")
            and "SRAM1" not in ln
            and "inline" not in ln
            and "void" not in ln
        ),
    }


def scan_build() -> Dict[str, Any]:
    """Return IT-related BUILD target info."""
    results = {}
    for core, build in [
        ("bbe32", REPO / "software/bbe32/integration_test/BUILD"),
        ("r52", REPO / "software/r52/integration_test/BUILD"),
    ]:
        if not build.exists():
            results[core] = {"error": "BUILD not found"}
            continue
        text = build.read_text(encoding="utf-8", errors="replace")
        results[core] = {
            "has_alwayslink": "alwayslink" in text,
            "has_select": "select(" in text,
            "targets": re.findall(r'name\s*=\s*"([^"]+)"', text),
        }
    return results


def scan_flags() -> Dict[str, Any]:
    """Return all bool_flag/string_flag and config_setting from root BUILD."""
    build = REPO / "BUILD"
    if not build.exists():
        return {"error": "root BUILD not found"}
    text = build.read_text(encoding="utf-8", errors="replace")
    bool_flags = re.findall(r'bool_flag\(\s*name\s*=\s*"([^"]+)"', text)
    string_flags = re.findall(r'string_flag\(\s*name\s*=\s*"([^"]+)"', text)
    config_settings = re.findall(r'config_setting\(\s*name\s*=\s*"([^"]+)"', text)
    return {
        "bool_flags": bool_flags,
        "string_flags": string_flags,
        "config_settings": config_settings,
    }


def scan_streams() -> Dict[str, Any]:
    """Return stream header files and their source IDs from software/common/."""
    common = REPO / "software" / "common"
    if not common.exists():
        return {"error": "software/common not found"}
    streams = []
    for f in sorted(common.glob("*_stream.h")):
        text = f.read_text(encoding="utf-8", errors="replace")
        ids = re.findall(r"#define\s+(\w+_STREAM_ID)\s+\(?\s*(\w+)\s*\)?", text)
        streams.append(
            {"file": f.name, "source_ids": [{"name": m[0], "value": m[1]} for m in ids]}
        )
    return {"streams": streams, "count": len(streams)}


def scan_test_targets() -> Dict[str, Any]:
    """Return all test targets from root BUILD test_suite."""
    build = REPO / "BUILD"
    if not build.exists():
        return {"error": "root BUILD not found"}
    text = build.read_text(encoding="utf-8", errors="replace")
    # Extract all_unit_tests suite members
    all_ut_match = re.search(
        r'name\s*=\s*"all_unit_tests".*?tests\s*=\s*\[(.*?)\]', text, re.DOTALL
    )
    bbe_match = re.search(r'name\s*=\s*"tests_bbe".*?tests\s*=\s*\[(.*?)\]', text, re.DOTALL)

    def extract_targets(match):
        if not match:
            return []
        return re.findall(r'"([^"]+)"', match.group(1))

    return {
        "all_unit_tests": extract_targets(all_ut_match),
        "tests_bbe": extract_targets(bbe_match),
    }


def scan_modules() -> Dict[str, Any]:
    """Return top-level software modules with their BUILD file presence."""
    modules = {}
    for core_dir in ["software/r52", "software/bbe32", "software/common"]:
        core_path = REPO / core_dir
        if not core_path.exists():
            continue
        core_name = core_dir.split("/")[-1]
        subdirs = []
        for d in sorted(core_path.iterdir()):
            if d.is_dir() and not d.name.startswith("."):
                has_build = (d / "BUILD").exists()
                has_test = (d / "test").exists() or (d / "test" / "BUILD").exists()
                subdirs.append({"name": d.name, "has_build": has_build, "has_test": has_test})
        modules[core_name] = subdirs
    return modules


def scan_includes(file_path: str = "") -> Dict[str, Any]:
    """Return all #include directives from a source file. Saves reading entire files."""
    if not file_path:
        return {"error": "file_path required (relative to repo root)"}
    src = REPO / file_path
    if not src.exists():
        return {"error": f"file not found: {file_path}"}
    text = src.read_text(encoding="utf-8", errors="replace")
    includes = re.findall(r'#include\s+[<"]([^>"]+)[>"]', text)
    # Also extract conditional includes
    guards = re.findall(r'#if.*defined\((\w+)\)\s*\n\s*#include\s+[<"]([^>"]+)[>"]', text)
    return {
        "file": file_path,
        "includes": includes,
        "conditional_includes": [{"guard": g[0], "header": g[1]} for g in guards],
        "total": len(includes),
    }


def scan_deps(build_path: str = "", target: str = "") -> Dict[str, Any]:
    """Return deps/srcs/hdrs for a specific target in a BUILD file."""
    if not build_path:
        return {
            "error": "build_path required (relative to repo root, e.g. 'software/bbe32/rdd_proc/BUILD')"
        }
    build_file = REPO / build_path
    if not build_file.exists():
        return {"error": f"BUILD not found: {build_path}"}
    text = build_file.read_text(encoding="utf-8", errors="replace")

    # If no target specified, return all target names
    if not target:
        targets = re.findall(r'name\s*=\s*"([^"]+)"', text)
        return {"build_path": build_path, "targets": targets}

    # Find the specific target block
    # Match from 'name = "target"' to the next rule or end
    pattern = rf'(?:cc_library|cc_test|cc_binary|filegroup|test_suite|bool_flag|string_flag|config_setting)\(\s*(?:[^)]*?name\s*=\s*"{re.escape(target)}"[^)]*)\)'
    match = re.search(pattern, text, re.DOTALL)
    if not match:
        # Fallback: find block containing the target name
        lines = text.split("\n")
        in_target = False
        block = []
        paren_depth = 0
        for line in lines:
            if f'name = "{target}"' in line:
                in_target = True
                # Find the start of this rule (go back to find opening paren)
                block = []
                paren_depth = 1
                continue
            if in_target:
                paren_depth += line.count("(") - line.count(")")
                block.append(line)
                if paren_depth <= 0:
                    break
        block_text = "\n".join(block)
    else:
        block_text = match.group(0)

    deps = re.findall(r'"((?://|@|:)[^"]+)"', block_text)
    srcs = re.findall(r"srcs\s*=\s*\[(.*?)\]", block_text, re.DOTALL)
    hdrs = re.findall(r"hdrs\s*=\s*\[(.*?)\]", block_text, re.DOTALL)

    def extract_strings(match_list):
        if not match_list:
            return []
        return re.findall(r'"([^"]+)"', match_list[0])

    return {
        "build_path": build_path,
        "target": target,
        "deps": [d for d in deps if d not in extract_strings(srcs) + extract_strings(hdrs)],
        "srcs": extract_strings(srcs),
        "hdrs": extract_strings(hdrs),
        "has_select": "select(" in block_text,
        "has_alwayslink": "alwayslink" in block_text,
    }


def scan_external_deps() -> Dict[str, Any]:
    """Return all bazel_dep entries from MODULE.bazel and bb.MODULE.bazel."""
    results = {"bazel_deps": [], "local_repos": [], "http_archives": []}
    for module_file in ["MODULE.bazel", "bb.MODULE.bazel"]:
        path = REPO / module_file
        if not path.exists():
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        # bazel_dep entries
        for m in re.finditer(
            r'bazel_dep\(\s*name\s*=\s*"([^"]+)"(?:.*?version\s*=\s*"([^"]+)")?', text, re.DOTALL
        ):
            entry = {"name": m.group(1), "source": module_file}
            if m.group(2):
                entry["version"] = m.group(2)
            results["bazel_deps"].append(entry)
        # local_repository entries
        for m in re.finditer(
            r'local_repository\(\s*name\s*=\s*"([^"]+)".*?path\s*=\s*"([^"]+)"', text, re.DOTALL
        ):
            results["local_repos"].append(
                {"name": m.group(1), "path": m.group(2), "source": module_file}
            )
        # http_archive entries
        for m in re.finditer(r'http_archive\(\s*name\s*=\s*"([^"]+)"', text):
            results["http_archives"].append({"name": m.group(1), "source": module_file})
    return results


def scan_coverage_thresholds() -> Dict[str, Any]:
    """Return all coverage_report targets with their thresholds. Saves reading 1000+ line BUILD."""
    build = REPO / "coverage" / "BUILD"
    if not build.exists():
        return {"error": "coverage/BUILD not found"}
    text = build.read_text(encoding="utf-8", errors="replace")
    reports = []
    for m in re.finditer(r'coverage_report\(\s*name\s*=\s*"([^"]+)"(.*?)\)', text, re.DOTALL):
        name = m.group(1)
        block = m.group(2)
        report = {"name": name}
        for field in ["min_line", "min_branch", "min_decision", "min_function"]:
            val = re.search(rf'{field}\s*=\s*"([^"]+)"', block)
            if val:
                report[field] = val.group(1)
        # Extract test targets
        tests_match = re.search(r"tests\s*=\s*\[(.*?)\]", block, re.DOTALL)
        if tests_match:
            report["tests"] = re.findall(r'"([^"]+)"', tests_match.group(1))
        reports.append(report)
    return {"reports": reports, "count": len(reports)}


def scan_bazelrc() -> Dict[str, Any]:
    """Return flag aliases and config definitions from .bazelrc."""
    rc = REPO / ".bazelrc"
    if not rc.exists():
        return {"error": ".bazelrc not found"}
    text = rc.read_text(encoding="utf-8", errors="replace")
    # Flag aliases
    aliases = {}
    for m in re.finditer(r"--flag_alias=(\w+)=(.*)", text):
        aliases[m.group(1)] = m.group(2).strip()
    # Config definitions (build:name --flag)
    configs = {}
    for m in re.finditer(r"(?:build|common|test):(\w+)\s+(--\S+.*)", text):
        cfg_name = m.group(1)
        if cfg_name not in configs:
            configs[cfg_name] = []
        configs[cfg_name].append(m.group(2).strip())
    return {"flag_aliases": aliases, "configs": configs}


def scan_swcs() -> Dict[str, Any]:
    """Return all AUTOSAR SWC directories with their BUILD targets."""
    swc_dir = REPO / "software" / "r52" / "autosar" / "swc"
    if not swc_dir.exists():
        return {"error": "software/r52/autosar/swc not found"}
    swcs = []
    for d in sorted(swc_dir.iterdir()):
        if not d.is_dir() or d.name.startswith("."):
            continue
        entry = {"name": d.name, "has_build": (d / "BUILD").exists(), "has_test": False}
        # Check for test directory
        test_dir = d / "test"
        if test_dir.exists() and (test_dir / "BUILD").exists():
            entry["has_test"] = True
            # Extract test target names
            test_build = (test_dir / "BUILD").read_text(encoding="utf-8", errors="replace")
            entry["test_targets"] = re.findall(r'name\s*=\s*"([^"]+)"', test_build)
        # Get BUILD targets if BUILD exists
        if entry["has_build"]:
            build_text = (d / "BUILD").read_text(encoding="utf-8", errors="replace")
            entry["targets"] = re.findall(r'name\s*=\s*"([^"]+)"', build_text)
        swcs.append(entry)
    return {"swcs": swcs, "count": len(swcs)}


# --- Coding Standards Tools ---


def scan_precommit(file_path: str = "") -> Dict[str, Any]:
    """Return pre-commit hooks that apply to a file, or all hooks if no file given."""
    cfg = REPO / ".pre-commit-config.yaml"
    if not cfg.exists():
        return {"error": ".pre-commit-config.yaml not found"}
    text = cfg.read_text(encoding="utf-8", errors="replace")

    hooks = []
    # Simple YAML parsing: find id + name pairs and their exclude blocks
    lines = text.split("\n")
    i = 0
    current_hook = None
    in_exclude = False
    exclude_lines = []

    while i < len(lines):
        line = lines[i]
        # New hook entry
        id_match = re.match(r"\s*-\s+id:\s*(\S+)", line)
        if id_match:
            # Save previous hook
            if current_hook:
                if exclude_lines:
                    current_hook["excludes"] = exclude_lines
                hooks.append(current_hook)
            current_hook = {"id": id_match.group(1)}
            in_exclude = False
            exclude_lines = []
            i += 1
            continue

        if current_hook:
            name_match = re.match(r'\s*name:\s*"?([^"\n]+)"?', line)
            if name_match:
                current_hook["name"] = name_match.group(1).strip()

            # Start of exclude block
            if re.match(r"\s*exclude:\s*\|", line):
                in_exclude = True
                i += 1
                continue
            elif re.match(r"\s*exclude:\s*$", line):
                in_exclude = True
                i += 1
                continue

            # Collecting exclude patterns
            if in_exclude:
                stripped = line.strip()
                if stripped and not stripped.startswith(")") and not stripped.startswith("(?x"):
                    # Extract the actual path pattern
                    path_pat = stripped.rstrip("|").strip().rstrip("#").strip()
                    # Remove trailing comments
                    if "#" in path_pat:
                        path_pat = path_pat[: path_pat.index("#")].strip()
                    if path_pat and not path_pat.startswith("$"):
                        exclude_lines.append(path_pat)
                # End of exclude block: next key at same/lower indent
                if (
                    stripped
                    and re.match(r"\s{0,4}\w", line)
                    and ":" in line
                    and "exclude" not in line
                ):
                    in_exclude = False
        i += 1

    # Save last hook
    if current_hook:
        if exclude_lines:
            current_hook["excludes"] = exclude_lines
        hooks.append(current_hook)

    # If file_path given, determine which hooks apply
    if file_path:
        for hook in hooks:
            excluded = False
            for pat in hook.get("excludes", []):
                try:
                    if re.search(pat, file_path):
                        excluded = True
                        break
                except re.error:
                    # Pattern is not valid regex, try simple substring
                    if pat in file_path:
                        excluded = True
                        break
            hook["applies"] = not excluded

    result = {"hooks": hooks, "total_hooks": len(hooks)}
    if file_path:
        ext = Path(file_path).suffix
        result["file_path"] = file_path
        result["file_type"] = ext
        result["formatting_rules"] = _get_format_rules(ext)
        result["applicable_hooks"] = [h["id"] for h in hooks if h.get("applies", True)]
    return result


def _get_format_rules(ext: str) -> Dict[str, Any]:
    """Return formatting rules based on file extension."""
    if ext in [".c", ".h", ".cpp", ".hpp"]:
        # Parse .clang-format
        cf = REPO / ".clang-format"
        if cf.exists():
            text = cf.read_text(encoding="utf-8", errors="replace")
            rules = {}
            for line in text.split("\n"):
                if ":" in line and not line.startswith("---") and not line.startswith("..."):
                    key, val = line.split(":", 1)
                    rules[key.strip()] = val.strip().strip("'\"")
            return {
                "formatter": "clang-format v13.0.1",
                "key_rules": {
                    "style": rules.get("BasedOnStyle", "Microsoft"),
                    "indent": rules.get("IndentWidth", "3"),
                    "column_limit": rules.get("ColumnLimit", "131"),
                    "braces": rules.get("BreakBeforeBraces", "Allman"),
                    "tabs": rules.get("UseTab", "Never"),
                    "pointer": rules.get("PointerAlignment", "Right"),
                    "sort_includes": rules.get("SortIncludes", "true"),
                },
            }
        return {"formatter": "clang-format", "config": "not found"}
    elif ext in [".py"]:
        return {
            "formatter": "black",
            "key_rules": {
                "line_length": "99",
                "target_version": "py310",
                "config": "tools/preCommit/pyproject.toml",
            },
            "linter": "flake8",
            "linter_rules": {
                "max_line_length": "131",
                "ignored": "E203, E501, W503, D200, D105, D401",
                "config": "tools/preCommit/f8_python.ini",
            },
        }
    elif ext in [".bzl", ""]:  # BUILD files have no extension
        return {
            "formatter": "buildifier",
            "key_rules": {
                "tool": "buildifier (Bazel formatter)",
                "auto_applied": "on commit via pre-commit hook",
            },
        }
    elif ext in [".m", ".slx"]:
        return {
            "formatter": "mh_style (MISS HIT)",
            "key_rules": {
                "config": "miss_hit.cfg",
                "version": "0.9.44",
            },
        }
    elif ext in [".sh"]:
        return {
            "formatter": "shfmt",
            "key_rules": {
                "indent": "2 spaces",
                "binary_newline": True,
                "ci": True,
            },
        }
    return {"formatter": "none", "note": "No formatter configured for this extension"}


def scan_functions(file_path: str = "") -> Dict[str, Any]:
    """Return function signatures from a .c/.h file. Quick API overview without reading full source."""
    if not file_path:
        return {"error": "file_path required (relative to repo root)"}
    src = REPO / file_path
    if not src.exists():
        return {"error": f"file not found: {file_path}"}
    text = src.read_text(encoding="utf-8", errors="replace")

    # Match C function definitions/declarations
    # Pattern: return_type function_name(params) with optional qualifiers
    functions = []
    for m in re.finditer(
        r"^[ \t]*((?:static\s+|inline\s+|extern\s+|const\s+)*\w[\w\s\*]*?)\s+(\w+)\s*\(([^)]*)\)\s*[{;]",
        text,
        re.MULTILINE,
    ):
        ret_type = m.group(1).strip()
        name = m.group(2).strip()
        params = m.group(3).strip()
        # Skip macros, defines, and common false positives
        if (
            name.isupper()
            or name.startswith("_")
            or ret_type in ["if", "while", "for", "switch", "return"]
        ):
            continue
        functions.append({"name": name, "return_type": ret_type, "params": params})

    return {"file": file_path, "functions": functions, "count": len(functions)}


def scan_defines(file_path: str = "") -> Dict[str, Any]:
    """Return all #define macros from a header. Understanding config without reading full file."""
    if not file_path:
        return {"error": "file_path required (relative to repo root)"}
    src = REPO / file_path
    if not src.exists():
        return {"error": f"file not found: {file_path}"}
    text = src.read_text(encoding="utf-8", errors="replace")

    defines = []
    for m in re.finditer(
        r"#define\s+(\w+)(?:\(([^)]*)\))?\s*(.*?)(?:\\\s*\n.*?)*$", text, re.MULTILINE
    ):
        name = m.group(1)
        params = m.group(2)  # None if not a function-like macro
        value = m.group(3).strip().rstrip("\\").strip()
        # Skip include guards
        if name.endswith("_H") or name.endswith("_H_"):
            continue
        entry = {"name": name}
        if params is not None:
            entry["params"] = params
        if value and len(value) < 100:
            entry["value"] = value
        defines.append(entry)

    return {"file": file_path, "defines": defines, "count": len(defines)}


def scan_git_changes() -> Dict[str, Any]:
    """Return modified/staged/untracked files from git status."""
    import subprocess

    try:
        result = subprocess.run(
            ["git", "status", "--porcelain"],
            cwd=str(REPO),
            capture_output=True,
            text=True,
            timeout=10,
        )
        if result.returncode != 0:
            return {"error": result.stderr.strip()}
        changes = {"modified": [], "staged": [], "untracked": []}
        for line in result.stdout.strip().split("\n"):
            if not line.strip():
                continue
            status = line[:2]
            filepath = line[3:].strip()
            if status[0] in "MADRC":
                changes["staged"].append(filepath)
            if status[1] == "M":
                changes["modified"].append(filepath)
            elif status[1] == "?":
                changes["untracked"].append(filepath)
        return changes
    except Exception as e:
        return {"error": str(e)}


def scan_memory_stats() -> Dict[str, Any]:
    """Return memory usage stats from build output (if available)."""
    stats_file = REPO / "bazel-bin" / "temp" / "memoryStats.txt"
    if not stats_file.exists():
        return {"error": "bazel-bin/temp/memoryStats.txt not found. Run a build first."}
    text = stats_file.read_text(encoding="utf-8", errors="replace")
    lines = text.strip().split("\n")

    sections = {}
    for line in lines:
        # Parse lines like "section_name: used/total (percent%)"
        m = re.match(
            r"\s*(\w[\w\s]*\w)\s*:\s*([\d.]+)\s*[/|]\s*([\d.]+)\s*(?:\(([\d.]+)%?\))?", line
        )
        if m:
            sections[m.group(1).strip()] = {
                "used": m.group(2),
                "total": m.group(3),
                "percent": m.group(4) if m.group(4) else None,
            }

    # If no structured parsing worked, return raw summary (first 30 lines)
    if not sections:
        return {"raw_summary": lines[:30], "total_lines": len(lines)}
    return {"sections": sections}


# --- Token-Saving Power Tools ---


def scan_structs(file_path: str = "") -> Dict[str, Any]:
    """Extract struct/typedef/enum definitions from a header. Saves reading 200-1000 line headers."""
    if not file_path:
        return {"error": "file_path required (relative to repo root)"}
    src = REPO / file_path
    if not src.exists():
        return {"error": f"file not found: {file_path}"}
    text = src.read_text(encoding="utf-8", errors="replace")

    results = {"file": file_path, "structs": [], "enums": [], "typedefs": []}

    # Extract typedef struct { ... } Name_T;
    for m in re.finditer(
        r"typedef\s+struct\s*(?:\w+)?\s*\{([^}]*(?:\{[^}]*\}[^}]*)*)\}\s*(\w+)\s*;",
        text,
        re.DOTALL,
    ):
        body = m.group(1)
        name = m.group(2)
        fields = []
        for field in re.finditer(
            r"^\s+([\w\s\*]+?)\s+(\w+)(?:\[([^\]]+)\])?\s*;", body, re.MULTILINE
        ):
            entry = {"type": field.group(1).strip(), "name": field.group(2)}
            if field.group(3):
                entry["array"] = field.group(3)
            fields.append(entry)
        results["structs"].append({"name": name, "fields": fields, "field_count": len(fields)})

    # Extract plain struct (non-typedef)
    for m in re.finditer(
        r"(?<!typedef\s)struct\s+(\w+)\s*\{([^}]*(?:\{[^}]*\}[^}]*)*)\}\s*;", text, re.DOTALL
    ):
        name = m.group(1)
        body = m.group(2)
        fields = []
        for field in re.finditer(
            r"^\s+([\w\s\*]+?)\s+(\w+)(?:\[([^\]]+)\])?\s*;", body, re.MULTILINE
        ):
            entry = {"type": field.group(1).strip(), "name": field.group(2)}
            if field.group(3):
                entry["array"] = field.group(3)
            fields.append(entry)
        # Avoid duplicates from typedef struct
        if not any(s["name"] == name or s["name"] == name + "_T" for s in results["structs"]):
            results["structs"].append({"name": name, "fields": fields, "field_count": len(fields)})

    # Extract enums (typedef enum { ... } Name_T;)
    for m in re.finditer(r"typedef\s+enum\s*(?:\w+)?\s*\{([^}]+)\}\s*(\w+)\s*;", text, re.DOTALL):
        body = m.group(1)
        name = m.group(2)
        values = []
        for v in re.finditer(r"(\w+)\s*(?:=\s*([^,\n]+))?\s*[,\n]", body):
            entry = {"name": v.group(1)}
            if v.group(2):
                entry["value"] = v.group(2).strip()
            values.append(entry)
        results["enums"].append({"name": name, "values": values, "count": len(values)})

    # Extract simple typedefs (typedef existing_type New_Name_T;)
    for m in re.finditer(r"typedef\s+([\w\s\*]+?)\s+(\w+_[Tt])\s*;", text):
        type_val = m.group(1).strip()
        name = m.group(2)
        if "struct" not in type_val and "enum" not in type_val:
            results["typedefs"].append({"name": name, "base_type": type_val})

    results["total"] = len(results["structs"]) + len(results["enums"]) + len(results["typedefs"])
    return results


def scan_ipc_payload() -> Dict[str, Any]:
    """Extract IPC data structures (M2D/D2M payloads, SIT_Data_T) from ipc_data.h. Saves ~600 lines."""
    ipc_file = REPO / "software" / "common" / "ipc" / "ipc_data.h"
    if not ipc_file.exists():
        return {"error": "software/common/ipc/ipc_data.h not found"}
    text = ipc_file.read_text(encoding="utf-8", errors="replace")

    results = {"structs": [], "defines": [], "key_types": []}

    # Find all typedef structs with their fields
    for m in re.finditer(
        r"typedef\s+struct\s*(?:\w+)?\s*\{([^}]*(?:\{[^}]*\}[^}]*)*)\}\s*(\w+)\s*;",
        text,
        re.DOTALL,
    ):
        body = m.group(1)
        name = m.group(2)
        fields = []
        for field in re.finditer(
            r"^\s+([\w\s\*]+?)\s+(\w+)(?:\[([^\]]+)\])?\s*;", body, re.MULTILINE
        ):
            entry = {"type": field.group(1).strip(), "name": field.group(2)}
            if field.group(3):
                entry["array"] = field.group(3)
            fields.append(entry)
        results["structs"].append({"name": name, "fields": fields})

    # Find key IPC defines
    for m in re.finditer(r"#define\s+((?:M2D|D2M|IPC|SIT)\w+)\s+(.+?)$", text, re.MULTILINE):
        results["defines"].append({"name": m.group(1), "value": m.group(2).strip()})

    # Highlight key structs
    key_names = ["SIT_Data_T", "M2D_Data_T", "D2M_Data_T", "IPC_Data_T"]
    for s in results["structs"]:
        if s["name"] in key_names:
            results["key_types"].append(s["name"])

    results["total_structs"] = len(results["structs"])
    return results


def scan_ci_pipelines() -> Dict[str, Any]:
    """Parse all WRSD pipeline YAML files — stages, triggers, params. Saves reading 5-10 YAML files."""
    ci_dir = REPO / "tools" / "CI" / "WRSD"
    if not ci_dir.exists():
        return {"error": "tools/CI/WRSD not found"}

    pipelines = []
    for f in sorted(ci_dir.glob("*.yaml")):
        text = f.read_text(encoding="utf-8", errors="replace")
        pipeline = {"file": f.name, "params": [], "tasks": [], "triggers": []}

        # Extract pipeline name
        name_match = re.search(r"name:\s*(.+)", text)
        if name_match:
            pipeline["name"] = name_match.group(1).strip()

        # Extract params
        for m in re.finditer(
            r"-\s*name:\s*(\S+)\s*\n\s*(?:type:\s*(\S+))?\s*\n?\s*(?:default:\s*(.+))?", text
        ):
            param = {"name": m.group(1)}
            if m.group(2):
                param["type"] = m.group(2)
            if m.group(3):
                param["default"] = m.group(3).strip()
            pipeline["params"].append(param)

        # Extract task names
        for m in re.finditer(r"(?:^|\n)\s*-\s*name:\s*(.+)", text):
            task_name = m.group(1).strip().strip('"').strip("'")
            if task_name and task_name not in [p["name"] for p in pipeline["params"]]:
                pipeline["tasks"].append(task_name)

        # Extract triggers
        for m in re.finditer(r"trigger:\s*\n((?:\s+.+\n)*)", text):
            trigger_block = m.group(1)
            for t in re.finditer(r"-\s*(\S+)", trigger_block):
                pipeline["triggers"].append(t.group(1))

        pipelines.append(pipeline)

    return {"pipelines": pipelines, "count": len(pipelines)}


def scan_build_graph(build_path: str = "", target: str = "", depth: int = 2) -> Dict[str, Any]:
    """Recursively resolve deps for a BUILD target up to N levels. Saves multiple scan_deps calls."""
    if not build_path or not target:
        return {"error": "build_path and target required"}

    visited = set()
    graph = {}

    def resolve(bpath: str, tname: str, current_depth: int):
        key = f"{bpath}:{tname}"
        if key in visited or current_depth > depth:
            return
        visited.add(key)

        build_file = REPO / bpath
        if not build_file.exists():
            graph[key] = {"error": "BUILD not found"}
            return

        text = build_file.read_text(encoding="utf-8", errors="replace")
        # Find target block
        pattern = rf'name\s*=\s*"{re.escape(tname)}"'
        match = re.search(pattern, text)
        if not match:
            graph[key] = {"error": f"target '{tname}' not found"}
            return

        # Get surrounding block (from previous rule keyword to closing paren)
        start = text.rfind("\n", 0, match.start())
        # Find the rule block end
        paren_depth = 0
        end = match.end()
        for i in range(match.start(), len(text)):
            if text[i] == "(":
                paren_depth += 1
            elif text[i] == ")":
                paren_depth -= 1
                if paren_depth <= 0:
                    end = i + 1
                    break

        block = text[start:end]
        deps = re.findall(r'"((?://|@|:)[^"]+)"', block)

        # Filter to actual deps (not srcs/hdrs)
        srcs_match = re.search(r"srcs\s*=\s*\[(.*?)\]", block, re.DOTALL)
        hdrs_match = re.search(r"hdrs\s*=\s*\[(.*?)\]", block, re.DOTALL)
        non_deps = set()
        if srcs_match:
            non_deps.update(re.findall(r'"([^"]+)"', srcs_match.group(1)))
        if hdrs_match:
            non_deps.update(re.findall(r'"([^"]+)"', hdrs_match.group(1)))

        actual_deps = [d for d in deps if d not in non_deps]
        graph[key] = {"deps": actual_deps}

        # Recurse into local deps
        if current_depth < depth:
            for dep in actual_deps:
                if dep.startswith("//"):
                    parts = dep.lstrip("/").split(":")
                    if len(parts) == 2:
                        next_build = parts[0] + "/BUILD"
                        next_target = parts[1]
                        resolve(next_build, next_target, current_depth + 1)
                elif dep.startswith(":"):
                    resolve(bpath, dep[1:], current_depth + 1)

    resolve(build_path, target, 0)
    return {"root": f"{build_path}:{target}", "depth": depth, "graph": graph, "nodes": len(graph)}


def scan_diff(ref: str = "HEAD") -> Dict[str, Any]:
    """Structured git diff summary vs ref. Shows files changed, insertions, deletions. Saves raw diff reading."""
    import subprocess

    try:
        # Get diff stat
        stat = subprocess.run(
            ["git", "diff", "--stat", ref],
            cwd=str(REPO),
            capture_output=True,
            text=True,
            timeout=15,
        )
        # Get file list with change types
        namestatus = subprocess.run(
            ["git", "diff", "--name-status", ref],
            cwd=str(REPO),
            capture_output=True,
            text=True,
            timeout=15,
        )
        if stat.returncode != 0:
            return {"error": stat.stderr.strip()}

        files = []
        for line in namestatus.stdout.strip().split("\n"):
            if not line.strip():
                continue
            parts = line.split("\t", 2)
            if len(parts) >= 2:
                entry = {"status": parts[0], "file": parts[1]}
                if len(parts) == 3:
                    entry["renamed_to"] = parts[2]
                files.append(entry)

        # Get per-file line counts
        numstat = subprocess.run(
            ["git", "diff", "--numstat", ref],
            cwd=str(REPO),
            capture_output=True,
            text=True,
            timeout=15,
        )
        line_changes = {}
        for line in numstat.stdout.strip().split("\n"):
            parts = line.split("\t")
            if len(parts) == 3:
                line_changes[parts[2]] = {
                    "additions": parts[0],
                    "deletions": parts[1],
                }

        # Merge data
        for f in files:
            if f["file"] in line_changes:
                f.update(line_changes[f["file"]])

        # Summary
        total_add = sum(
            int(f.get("additions", 0)) for f in files if f.get("additions", "-") != "-"
        )
        total_del = sum(
            int(f.get("deletions", 0)) for f in files if f.get("deletions", "-") != "-"
        )

        return {
            "ref": ref,
            "files": files,
            "total_files": len(files),
            "total_additions": total_add,
            "total_deletions": total_del,
        }
    except Exception as e:
        return {"error": str(e)}


def scan_module_api(module_path: str = "") -> Dict[str, Any]:
    """Combined module overview: functions + defines + includes + structs in one call. Saves 3-4 separate calls."""
    if not module_path:
        return {"error": "module_path required (e.g. 'software/bbe32/rdd_proc')"}
    mod_dir = REPO / module_path
    if not mod_dir.exists():
        return {"error": f"directory not found: {module_path}"}

    result = {
        "module": module_path,
        "headers": [],
        "sources": [],
        "functions": [],
        "defines": [],
        "includes": [],
        "structs": [],
    }

    # Collect all .h and .c files
    h_files = list(mod_dir.rglob("*.h"))
    c_files = list(mod_dir.rglob("*.c"))
    result["headers"] = [str(f.relative_to(REPO)) for f in h_files]
    result["sources"] = [str(f.relative_to(REPO)) for f in c_files]

    # Parse headers for defines and structs
    for h in h_files[:10]:  # Limit to 10 headers
        text = h.read_text(encoding="utf-8", errors="replace")

        # Defines
        for m in re.finditer(r"#define\s+(\w+)(?:\(([^)]*)\))?\s+(.+?)$", text, re.MULTILINE):
            name = m.group(1)
            if not name.endswith("_H") and not name.endswith("_H_"):
                entry = {"name": name, "file": h.name}
                if m.group(2) is not None:
                    entry["params"] = m.group(2)
                result["defines"].append(entry)

        # Structs
        for m in re.finditer(r"typedef\s+struct\s*\w*\s*\{[^}]*\}\s*(\w+)\s*;", text, re.DOTALL):
            result["structs"].append({"name": m.group(1), "file": h.name})

    # Parse sources for functions and includes
    for c in c_files[:10]:  # Limit to 10 sources
        text = c.read_text(encoding="utf-8", errors="replace")

        # Functions
        for m in re.finditer(
            r"^[ \t]*((?:static\s+|inline\s+|extern\s+|const\s+)*\w[\w\s\*]*?)\s+(\w+)\s*\(([^)]*)\)\s*[{;]",
            text,
            re.MULTILINE,
        ):
            name = m.group(2).strip()
            if not name.isupper() and not name.startswith("_"):
                ret = m.group(1).strip()
                if ret not in ["if", "while", "for", "switch", "return"]:
                    result["functions"].append(
                        {
                            "name": name,
                            "return_type": ret,
                            "file": c.name,
                        }
                    )

        # Includes
        for m in re.finditer(r'#include\s+[<"]([^>"]+)[>"]', text):
            inc = m.group(1)
            if inc not in result["includes"]:
                result["includes"].append(inc)

    result["summary"] = {
        "header_count": len(h_files),
        "source_count": len(c_files),
        "function_count": len(result["functions"]),
        "define_count": len(result["defines"]),
        "struct_count": len(result["structs"]),
    }
    return result


def scan_todo_fixme(path: str = "", pattern: str = "TODO|FIXME|HACK|XXX") -> Dict[str, Any]:
    """Find all TODO/FIXME/HACK comments in a path. Quick tech debt overview."""
    if not path:
        return {"error": "path required (relative to repo root, e.g. 'software/bbe32/src')"}
    target = REPO / path
    if not target.exists():
        return {"error": f"path not found: {path}"}

    results = []
    files_to_scan = []
    if target.is_file():
        files_to_scan = [target]
    else:
        for ext in ["*.c", "*.h", "*.cc", "*.cpp", "*.py"]:
            files_to_scan.extend(target.rglob(ext))

    pat = re.compile(rf".*({pattern})\s*[:\-]?\s*(.*)", re.IGNORECASE)

    for f in files_to_scan[:50]:  # Limit file count
        try:
            text = f.read_text(encoding="utf-8", errors="replace")
            for i, line in enumerate(text.split("\n"), 1):
                m = pat.match(line.strip())
                if m and ("//" in line or "/*" in line or "#" in line):
                    results.append(
                        {
                            "file": str(f.relative_to(REPO)),
                            "line": i,
                            "type": m.group(1).upper(),
                            "text": m.group(2).strip()[:100],
                        }
                    )
        except (OSError, UnicodeDecodeError):
            continue

    return {"path": path, "items": results, "count": len(results)}


def scan_file_summary(file_path: str = "") -> Dict[str, Any]:
    """High-level summary of any source file: line count, functions, includes, guards. One call = full context."""
    if not file_path:
        return {"error": "file_path required (relative to repo root)"}
    src = REPO / file_path
    if not src.exists():
        return {"error": f"file not found: {file_path}"}
    text = src.read_text(encoding="utf-8", errors="replace")
    lines = text.split("\n")

    result = {
        "file": file_path,
        "total_lines": len(lines),
        "blank_lines": sum(1 for ln in lines if not ln.strip()),
        "comment_lines": sum(1 for ln in lines if ln.strip().startswith(("//", "/*", "*"))),
    }

    # Includes
    result["includes"] = re.findall(r'#include\s+[<"]([^>"]+)[>"]', text)

    # Functions (name + line number)
    funcs = []
    for m in re.finditer(
        r"^[ \t]*((?:static\s+|inline\s+|extern\s+|const\s+)*\w[\w\s\*]*?)\s+(\w+)\s*\([^)]*\)\s*\{",
        text,
        re.MULTILINE,
    ):
        name = m.group(2).strip()
        if not name.isupper() and name not in ["if", "while", "for", "switch", "return"]:
            line_num = text[: m.start()].count("\n") + 1
            funcs.append({"name": name, "line": line_num, "return_type": m.group(1).strip()})
    result["functions"] = funcs

    # Ifdef guards
    guards = re.findall(r"#if(?:def|ndef)?\s+(?:defined\()?(\w+)\)?", text)
    result["ifdef_guards"] = list(dict.fromkeys(guards))  # unique, preserve order

    # Global variables
    globals_list = []
    for m in re.finditer(
        r"^(?:static\s+|volatile\s+|extern\s+)*([\w\s\*]+?)\s+(\w+)\s*(?:=|;)", text, re.MULTILINE
    ):
        vtype = m.group(1).strip()
        vname = m.group(2)
        if vtype not in ["if", "while", "for", "switch", "return", "typedef", "#define"]:
            # Only file-scope (not inside a function)
            pos = m.start()
            preceding = text[:pos]
            # Rough check: no unmatched { before this
            open_braces = preceding.count("{") - preceding.count("}")
            if open_braces == 0:
                globals_list.append({"type": vtype, "name": vname})
    result["globals"] = globals_list[:30]  # Limit

    result["summary"] = (
        f"{len(lines)} lines, {len(funcs)} functions, "
        f"{len(result['includes'])} includes, {len(result['ifdef_guards'])} guards"
    )
    return result


# --- MCP Protocol (JSON-RPC over stdio) ---

TOOLS = {
    "scan_interfaces": {
        "fn": scan_interfaces,
        "desc": "List all DCS_X enum entries from IT headers.",
        "params": {},
    },
    "scan_hooks": {
        "fn": scan_hooks,
        "desc": "List SIT_ macro names from hook headers.",
        "params": {},
    },
    "scan_memory": {
        "fn": scan_memory,
        "desc": "SRAM1 usage stats from BBE32 IT source.",
        "params": {},
    },
    "scan_build": {
        "fn": scan_build,
        "desc": "IT BUILD target info (alwayslink, select, names).",
        "params": {},
    },
    "scan_flags": {
        "fn": scan_flags,
        "desc": "All bool_flag/string_flag/config_setting from root BUILD.",
        "params": {},
    },
    "scan_streams": {
        "fn": scan_streams,
        "desc": "Stream headers and source IDs from software/common/.",
        "params": {},
    },
    "scan_test_targets": {
        "fn": scan_test_targets,
        "desc": "All test targets from root BUILD test_suites.",
        "params": {},
    },
    "scan_modules": {
        "fn": scan_modules,
        "desc": "Software modules with BUILD/test presence.",
        "params": {},
    },
    "scan_includes": {
        "fn": scan_includes,
        "desc": "All #include directives from a source file. Pass file_path relative to repo root.",
        "params": {
            "file_path": {
                "type": "string",
                "description": "Relative path to .c/.h file (e.g. software/bbe32/src/dsp_main.c)",
            }
        },
    },
    "scan_deps": {
        "fn": scan_deps,
        "desc": "Deps/srcs/hdrs for a BUILD target. Pass build_path and optionally target name.",
        "params": {
            "build_path": {
                "type": "string",
                "description": "Relative path to BUILD file (e.g. software/bbe32/rdd_proc/BUILD)",
            },
            "target": {
                "type": "string",
                "description": "Target name (optional, lists all targets if omitted)",
            },
        },
    },
    "scan_external_deps": {
        "fn": scan_external_deps,
        "desc": "All bazel_dep/local_repository/http_archive from MODULE.bazel files.",
        "params": {},
    },
    "scan_coverage_thresholds": {
        "fn": scan_coverage_thresholds,
        "desc": "All coverage_report targets with min thresholds. Saves reading 1000+ line BUILD.",
        "params": {},
    },
    "scan_bazelrc": {
        "fn": scan_bazelrc,
        "desc": "Flag aliases and config definitions from .bazelrc.",
        "params": {},
    },
    "scan_swcs": {
        "fn": scan_swcs,
        "desc": "All AUTOSAR SWC directories with BUILD targets and test info.",
        "params": {},
    },
    "scan_precommit": {
        "fn": scan_precommit,
        "desc": "Pre-commit hooks + formatting rules for a file. Pass file_path to check what applies.",
        "params": {
            "file_path": {
                "type": "string",
                "description": "Relative path to check (e.g. software/bbe32/src/dsp_main.c). Optional.",
            }
        },
    },
    "scan_functions": {
        "fn": scan_functions,
        "desc": "Function signatures from a .c/.h file. Quick API overview.",
        "params": {
            "file_path": {
                "type": "string",
                "description": "Relative path to .c/.h file (e.g. software/bbe32/src/dsp_main.c)",
            }
        },
    },
    "scan_defines": {
        "fn": scan_defines,
        "desc": "All #define macros from a header. Config understanding without full read.",
        "params": {
            "file_path": {
                "type": "string",
                "description": "Relative path to .h file (e.g. software/bbe32/inc/dsp_config.h)",
            }
        },
    },
    "scan_git_changes": {
        "fn": scan_git_changes,
        "desc": "Modified/staged/untracked files from git status. Know what is being worked on.",
        "params": {},
    },
    "scan_memory_stats": {
        "fn": scan_memory_stats,
        "desc": "Memory usage stats from build output (bazel-bin/temp/memoryStats.txt).",
        "params": {},
    },
    "scan_structs": {
        "fn": scan_structs,
        "desc": "Extract struct/typedef/enum definitions from a header. Saves reading 200-1000 line headers.",
        "params": {
            "file_path": {
                "type": "string",
                "description": "Relative path to .h file (e.g. software/common/ipc/ipc_data.h)",
            }
        },
    },
    "scan_ipc_payload": {
        "fn": scan_ipc_payload,
        "desc": "IPC data structures (M2D/D2M payloads, SIT_Data_T) from ipc_data.h. Saves ~600 lines.",
        "params": {},
    },
    "scan_ci_pipelines": {
        "fn": scan_ci_pipelines,
        "desc": "All WRSD pipeline YAML summaries — stages, triggers, params. Saves reading 5-10 YAML files.",
        "params": {},
    },
    "scan_build_graph": {
        "fn": scan_build_graph,
        "desc": "Recursive dep graph for a BUILD target (up to N levels). Saves multiple scan_deps calls.",
        "params": {
            "build_path": {
                "type": "string",
                "description": "Relative path to BUILD file (e.g. software/bbe32/src/BUILD)",
            },
            "target": {
                "type": "string",
                "description": "Target name to resolve deps for",
            },
            "depth": {
                "type": "integer",
                "description": "Max recursion depth (default 2, max 4)",
            },
        },
    },
    "scan_diff": {
        "fn": scan_diff,
        "desc": "Structured git diff summary vs a ref (default HEAD). Files, additions, deletions.",
        "params": {
            "ref": {
                "type": "string",
                "description": "Git ref to diff against (default: HEAD). E.g. HEAD~3, origin/main, commit-sha",
            }
        },
    },
    "scan_module_api": {
        "fn": scan_module_api,
        "desc": "Combined module overview: functions + defines + includes + structs in one call. Saves 3-4 calls.",
        "params": {
            "module_path": {
                "type": "string",
                "description": "Relative path to module directory (e.g. software/bbe32/rdd_proc)",
            }
        },
    },
    "scan_todo_fixme": {
        "fn": scan_todo_fixme,
        "desc": "Find TODO/FIXME/HACK comments in a path. Quick tech debt overview.",
        "params": {
            "path": {
                "type": "string",
                "description": "Relative path to scan (file or directory, e.g. software/bbe32/src)",
            },
            "pattern": {
                "type": "string",
                "description": "Regex pattern to match (default: TODO|FIXME|HACK|XXX)",
            },
        },
    },
    "scan_file_summary": {
        "fn": scan_file_summary,
        "desc": "High-level file summary: line count, functions, includes, globals, guards. One call = full context.",
        "params": {
            "file_path": {
                "type": "string",
                "description": "Relative path to source file (e.g. software/bbe32/src/dsp_main.c)",
            }
        },
    },
}


def handle(req: Dict) -> Optional[Dict]:
    """Route incoming JSON-RPC request to the appropriate handler."""
    method = req.get("method", "")
    rid = req.get("id")

    if method == "initialize":
        return {
            "jsonrpc": "2.0",
            "id": rid,
            "result": {
                "protocolVersion": "2024-11-05",
                "capabilities": {"tools": {"listChanged": False}},
                "serverInfo": {"name": "gen8-scanner", "version": "1.0.0"},
            },
        }

    if method == "tools/list":
        return {
            "jsonrpc": "2.0",
            "id": rid,
            "result": {
                "tools": [
                    {
                        "name": n,
                        "description": t["desc"],
                        "inputSchema": {
                            "type": "object",
                            "properties": t.get("params", {}),
                        },
                    }
                    for n, t in TOOLS.items()
                ]
            },
        }

    if method == "tools/call":
        name = req.get("params", {}).get("name", "")
        args = req.get("params", {}).get("arguments", {})
        if name in TOOLS:
            try:
                fn = TOOLS[name]["fn"]
                # Pass arguments to functions that accept them
                sig = inspect.signature(fn)
                if sig.parameters:
                    result = fn(**{k: v for k, v in args.items() if k in sig.parameters})
                else:
                    result = fn()
                return {
                    "jsonrpc": "2.0",
                    "id": rid,
                    "result": {
                        "content": [{"type": "text", "text": json.dumps(result, indent=2)}]
                    },
                }
            except Exception as e:
                return {"jsonrpc": "2.0", "id": rid, "error": {"code": -32000, "message": str(e)}}
        return {
            "jsonrpc": "2.0",
            "id": rid,
            "error": {"code": -32601, "message": f"Unknown: {name}"},
        }

    if method == "notifications/initialized":
        return None
    return {
        "jsonrpc": "2.0",
        "id": rid,
        "error": {"code": -32601, "message": f"Unknown: {method}"},
    }


if __name__ == "__main__":
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        try:
            resp = handle(json.loads(line))
        except json.JSONDecodeError:
            continue
        if resp:
            sys.stdout.write(json.dumps(resp) + "\n")
            sys.stdout.flush()
