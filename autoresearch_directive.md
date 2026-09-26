# Binding iteration directive (overrides default scope until deleted)

1. WORKER MODEL: you may use up to 2 parallel subagents for literature/noveltysearch ONLY (Run 3 proved this works). Solo for implementation + runs.
2. Expand exactly ONE frontier node in order: N4 (T3) → N5 (T4) → N6 (T5) → N7 (F) → N8 (H) → N10 → N12. Never repeat an explored node.
3. Work order: (a) write the minimal prototype in resim_research/ + run it and capture numbers; (b) novelty check, max 3 fast queries (websearch cache-first; do NOT retry blocked domains); (c) checkpoint FIRST: append JSONL run line + strategy-graph node/edge + equations.md row + worklog entry; (d) git commit; (e) exit.
4. SKIP this iteration: papers/notes/*, graphify build, dashboard regen (batch those every 5 runs), LaTeX paper (only at novelty>=70 keep).
5. Timebox: if 40 minutes pass without reaching (d), checkpoint whatever exists as status unvalidated (max 1 per segment) or discard with reason, commit, exit. A small checkpoint beats a watchdog kill. Never end an iteration with zero state change.
