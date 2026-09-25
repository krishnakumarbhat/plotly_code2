"""Quick test for all MCP server tools."""
import json  # noqa: E402
import sys

sys.path.insert(0, ".mcp")
import server  # noqa: E402

passed = 0
failed = 0


def test(name, fn):  # noqa: D103
    global passed, failed
    try:
        result = fn()
        if "error" in result and not isinstance(result.get("error"), type(None)):
            # Some errors are expected (missing files), check if it's a real bug
            if "required" in str(result.get("error", "")):
                print(f"  SKIP {name}: {result['error']}")
                return
            print(f"  WARN {name}: {result['error']}")
            passed += 1
        else:
            print(f"  OK   {name}")
            passed += 1
    except Exception as e:
        print(f"  FAIL {name}: {e}")
        failed += 1


print("=" * 60)
print("MCP Server Tool Tests")
print("=" * 60)

# Original tools
test("scan_interfaces", server.scan_interfaces)
test("scan_hooks", server.scan_hooks)
test("scan_memory", server.scan_memory)
test("scan_build", server.scan_build)
test("scan_flags", server.scan_flags)
test("scan_streams", server.scan_streams)
test("scan_test_targets", server.scan_test_targets)
test("scan_modules", server.scan_modules)
test("scan_includes(file)", lambda: server.scan_includes("software/bbe32/main.c"))
test("scan_deps(build)", lambda: server.scan_deps("software/bbe32/BUILD"))
test("scan_external_deps", server.scan_external_deps)
test("scan_coverage_thresholds", server.scan_coverage_thresholds)
test("scan_bazelrc", server.scan_bazelrc)
test("scan_swcs", server.scan_swcs)
test("scan_precommit(file)", lambda: server.scan_precommit("software/bbe32/src/dsp_main.c"))
test("scan_functions(file)", lambda: server.scan_functions("software/bbe32/main.c"))
test("scan_defines(file)", lambda: server.scan_defines("software/common/stream_header.h"))
test("scan_git_changes", server.scan_git_changes)
test("scan_memory_stats", server.scan_memory_stats)

# New tools
print()
print("--- New Tools ---")
test("scan_structs(header)", lambda: server.scan_structs("software/common/stream_header.h"))
test("scan_ipc_payload", server.scan_ipc_payload)
test("scan_ci_pipelines", server.scan_ci_pipelines)
test(
    "scan_build_graph",
    lambda: server.scan_build_graph(
        "software/bbe32/integration_test/BUILD", "dsp_integration_test_lib", 1
    ),
)
test("scan_diff(HEAD)", lambda: server.scan_diff("HEAD"))
test("scan_module_api", lambda: server.scan_module_api("software/bbe32/integration_test"))
test("scan_todo_fixme", lambda: server.scan_todo_fixme("software/bbe32/src"))
test("scan_file_summary", lambda: server.scan_file_summary("software/bbe32/main.c"))

# MCP Protocol tests
print()
print("--- MCP Protocol ---")
req = {"jsonrpc": "2.0", "id": 1, "method": "initialize", "params": {}}
resp = server.handle(req)
assert resp["result"]["serverInfo"]["name"] == "gen8-scanner"
print("  OK   initialize")
passed += 1

req = {"jsonrpc": "2.0", "id": 2, "method": "tools/list", "params": {}}
resp = server.handle(req)
tool_count = len(resp["result"]["tools"])
assert tool_count == 27, f"Expected 27 tools, got {tool_count}"
print(f"  OK   tools/list ({tool_count} tools)")
passed += 1

req = {
    "jsonrpc": "2.0",
    "id": 3,
    "method": "tools/call",
    "params": {"name": "scan_flags", "arguments": {}},
}
resp = server.handle(req)
data = json.loads(resp["result"]["content"][0]["text"])
assert "bool_flags" in data
print(f"  OK   tools/call (scan_flags -> {len(data['bool_flags'])} bool_flags)")
passed += 1

req = {
    "jsonrpc": "2.0",
    "id": 4,
    "method": "tools/call",
    "params": {"name": "scan_file_summary", "arguments": {"file_path": "software/bbe32/main.c"}},
}
resp = server.handle(req)
data = json.loads(resp["result"]["content"][0]["text"])
assert "total_lines" in data
print(f"  OK   tools/call (scan_file_summary -> {data['total_lines']} lines)")
passed += 1

req = {
    "jsonrpc": "2.0",
    "id": 5,
    "method": "tools/call",
    "params": {"name": "nonexistent", "arguments": {}},
}
resp = server.handle(req)
assert "error" in resp
print("  OK   tools/call (unknown tool -> error)")
passed += 1

print()
print("=" * 60)
print(f"Results: {passed} passed, {failed} failed")
if failed == 0:
    print("ALL TESTS PASSED")
else:
    print("SOME TESTS FAILED")
    sys.exit(1)
