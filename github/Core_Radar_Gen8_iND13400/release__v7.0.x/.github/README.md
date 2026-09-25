# 🤖 .github/ — AI Assistance System

## 📖 Overview

This directory is the **single source of truth** for all AI/Copilot rules and knowledge in this repo. It's designed for **⚡ low-token efficiency** — structured so Copilot can solve problems with minimal context loading.

## 🏗️ Architecture

```
.github/
├── copilot-instructions.md          ← Global rules (auto-loaded every session)
├── instructions/                    ← Auto-triggered by file patterns
│   ├── swe5-core.instructions.md   ← Triggers on: software/**/integration_test/**
│   ├── swe5-build.instructions.md  ← Triggers on: **/BUILD, **/.bazelrc, **/MODULE.bazel
│   ├── swe5-memory.instructions.md ← Triggers on: software/bbe32/**, software/common/linker/**
│   └── bazel-conventions.instructions.md ← Triggers on: **/BUILD
├── skills/                          ← On-demand expert knowledge (loaded when invoked)
│   ├── swe5/SKILL.md              ← SWE5 Integration Testing (decision tree + error map)
│   ├── bazel-build/SKILL.md       ← Bazel patterns, flags, cc_library reference
│   ├── coverage/SKILL.md          ← Unit test coverage reports & thresholds
│   ├── ipc-streams/SKILL.md       ← R52↔BBE32 IPC and stream system
│   ├── debug-wrsd-failure/SKILL.md ← CI pipeline debugging
│   └── gerrit-api/SKILL.md        ← Gerrit review API
├── prompts/                         ← Agent-mode workflows (step-by-step)
│   ├── add-interface.prompt.md    ← Add SWE5 test interface
│   ├── debug-build.prompt.md      ← Fix SWE5 build failures
│   ├── fix-memory.prompt.md       ← Fix BBE32 memory overflow
│   ├── run-tests.prompt.md        ← Run unit tests
│   ├── debug-coverity.prompt.md   ← Fix Coverity static analysis defects
│   └── add-build-target.prompt.md ← Add cc_library/cc_test/bool_flag
├── agents/
│   └── swe5.agent.md              ← Autonomous SWE5 agent behavior
└── workflows/
    └── pr-checks.yml
```

## 🚀 How to Use

### 👩‍💻 For Developers (in VS Code with Copilot)

**🧠 Skills are invoked automatically** when you ask Copilot questions in the relevant domain:
- 💬 Ask "how do I add an interface?" → Copilot loads `swe5/SKILL.md`
- 💬 Ask "why is my build failing?" → Copilot loads `bazel-build/SKILL.md`
- 💬 Ask "fix coverage" → Copilot loads `coverage/SKILL.md`

**⚙️ Instructions auto-trigger** when you edit matching files:
- ✏️ Edit a `BUILD` file → `bazel-conventions.instructions.md` + `swe5-build.instructions.md` load
- ✏️ Edit `software/bbe32/integration_test/*` → `swe5-core.instructions.md` + `swe5-memory.instructions.md` load

**📋 Prompts are explicit workflows** — invoke them by name:
- 🔧 `/add-interface` → Step-by-step SWE5 interface addition
- 🧪 `/run-tests` → Find and run the right test target
- 🛡️ `/debug-coverity` → Fix Coverity defects with lookup tables

### 🤖 For the SWE5 Agent
Use `@swe5` in Copilot Chat to invoke the autonomous SWE5 agent that:
1. 📚 Reads instruction files automatically
2. 🌳 Uses decision tree from `swe5/SKILL.md`
3. ✍️ Modifies code following exact repo patterns
4. ✅ Builds both configs to verify

### 🔌 MCP Scanner (Optional Data Source)

The `.mcp/server.py` provides **19 repo-scanning tools** for quick data extraction without file searching:

#### 🧪 SWE5 Integration Testing tools

| Tool | What it returns | Tokens saved |
|------|-----------------|-------------|
| `scan_interfaces` | All DCS_X enum entries (BBE32 + R52) | ~420 lines |
| `scan_hooks` | All SIT_ macro names (BBE32 + R52) | ~200 lines |
| `scan_memory` | SRAM1 usage stats (vars with/without attrs) | ~300 lines |
| `scan_build` | IT BUILD targets (alwayslink, select usage) | ~150 lines |

#### 🏗️ Build System & Architecture tools

| Tool | What it returns | Tokens saved |
|------|-----------------|-------------|
| `scan_flags` | All bool_flags + config_settings from root BUILD | ~200 lines |
| `scan_streams` | 14 stream headers + source IDs | ~500 lines |
| `scan_test_targets` | 55 R52 + 10 BBE test targets | ~200 lines |
| `scan_modules` | 39 modules with BUILD/test presence | ~100 lines |
| `scan_external_deps` | All bazel_dep + local_repo + http_archive | ~322 lines |
| `scan_coverage_thresholds` | 57 coverage reports with min thresholds | **~1054 lines** |
| `scan_bazelrc` | 26 flag aliases + 10 config definitions | ~115 lines |
| `scan_swcs` | 27 AUTOSAR SWCs with targets + test info | ~500 lines |

#### 📏 Coding Standards tools

| Tool | Parameters | What it returns | Tokens saved |
|------|-----------|-----------------|-------------|
| `scan_precommit` | `file_path` (optional) | Pre-commit hooks + formatting rules for a file. Returns: which hooks apply, formatter config (clang-format/black/buildifier), exclude patterns | ~150 lines |

**Example:** `scan_precommit(file_path="software/bbe32/src/ipc_dsp.c")` →
```json
{"formatter": "clang-format v13.0.1", "key_rules": {"indent": "3", "column_limit": "131", "braces": "Allman", "tabs": "Never"}}
```

#### 🐛 Debugging & Investigation tools

| Tool | Parameters | What it returns | Tokens saved |
|------|-----------|-----------------|-------------|
| `scan_includes` | `file_path` | All #include directives from a source file | **100-1000 lines** per call |
| `scan_deps` | `build_path`, `target` | deps/srcs/hdrs for a specific BUILD target | **50-400 lines** per call |
| `scan_functions` | `file_path` | Function signatures (name, return type, params) — quick API overview | **100-800 lines** per call |
| `scan_defines` | `file_path` | All #define macros from a header (name, value, params) | **50-500 lines** per call |
| `scan_git_changes` | — | Modified/staged/untracked files from git status | replaces git calls |
| `scan_memory_stats` | — | Memory section usage from build output | ~200 lines |

#### 💰 Token savings math

**Without MCP:** "What format rules apply to my .c file?" → read .pre-commit-config.yaml (150 lines) + .clang-format (45 lines) = 195 tokens
**With MCP:** `scan_precommit(file_path="my_file.c")` → returns just the rules = ~30 tokens

**Without MCP:** "What functions does ipc_dsp.c have?" → `read_file(software/bbe32/src/ipc_dsp.c)` = 400+ tokens
**With MCP:** `scan_functions(file_path="software/bbe32/src/ipc_dsp.c")` → returns 16 signatures = ~50 tokens

**Without MCP:** "What coverage threshold does ecusync have?" → `read_file(coverage/BUILD, 1, 200)` = 200 tokens minimum
**With MCP:** `scan_coverage_thresholds()` → returns just `{"name": "ecusync", "min_line": "90", ...}` = ~20 tokens

**💎 Estimated savings per typical session:** 3000-8000 tokens (5-12 file reads replaced by MCP calls)

> 💡 MCP is **data only** — no rules or logic. All intelligence lives in this `.github/` directory.

## 🎯 Design Principles (Token Efficiency)

| # | Principle | Why it matters |
|---|-----------|----------------|
| 1 | 🌳 **Decision trees, not prose** | AI jumps to the answer in one lookup |
| 2 | 📍 **Exact file paths** | No searching needed; verified paths |
| 3 | 🗺️ **Error→Fix maps** | Common errors mapped to fixes (lookup, not reasoning) |
| 4 | 📋 **Copy-paste patterns** | Code patterns are exact, from the repo |
| 5 | ⚡ **Auto-trigger instructions** | Context loads automatically on file edit |
| 6 | 🔌 **MCP for data** | Quick counts/lists without file reading |
| 7 | 🧅 **Layered loading** | Only load what's needed: instructions → skill → MCP |

## ➕ Adding New Skills

1. 📁 Create folder: `.github/skills/my-skill/`
2. 📝 Create `SKILL.md` inside with YAML frontmatter:
   ```yaml
   ---
   description: "One-line description for discovery"
   ---
   ```
3. 🏗️ Structure content as: Quick Rules → Decision Tree → File Paths → Error Map → Patterns
4. 📦 Keep it self-contained (AI shouldn't need to read other files)

## ➕ Adding New Prompts

1. 📝 Create `.github/prompts/my-workflow.prompt.md`
2. 📎 Add YAML frontmatter:
   ```yaml
   ---
   mode: agent
   description: "What this workflow does"
   ---
   ```
3. 🔄 Structure as: Input Required → Steps → Commands → Verification

## ➕ Adding New Instructions

1. 📝 Create `.github/instructions/my-context.instructions.md`
2. 🎯 Add `applyTo` pattern:
   ```yaml
   ---
   applyTo: "software/my_module/**"
   ---
   ```
3. ⚠️ Keep brief — these auto-load and consume tokens every time the file pattern matches
