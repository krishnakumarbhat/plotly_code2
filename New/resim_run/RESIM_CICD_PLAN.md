# ReSim Gen7 Maintenance and CI/CD Plan

This is a planning document only. It proposes a safe path to maintain and optimize the current pipeline without changing the existing implementation in this repository.

## Goals

- Make every run reproducible from a versioned manifest.
- Remove hidden interactive behavior from automation.
- Make failures attributable to one stage and one log.
- Keep customer/project routing explicit.
- Support controlled ReSim, video, mining, statistics, MUDP, and KPI stages.
- Reduce repeated filesystem scans and avoid unnecessary Slurm submissions.
- Preserve the current vendor-compatible execution path while migrating gradually.

## Phase 0: Freeze and Observe

Deliverables:

- Keep this `resim_run` snapshot as the baseline.
- Record the exact launcher version, source timestamps, Python version, Slurm version, Singularity version, and account/partition.
- Capture one representative run for each image family: Platform, MCIP, DGPS, ADCAM, and dSpace.
- Store `RESIM_details.txt`, generated job scripts, Slurm IDs, task logs, mining logs, statistics logs, and output manifests.

Acceptance criteria:

- A run can be reconstructed from input list, SIMG, custom XML, mode, account, and output root.
- Every generated file is retained as an artifact.

## Phase 1: Static Validation Without Runtime Changes

Add CI checks around the snapshot and future source repository:

- `bash -n` for every `.sh` file.
- Python syntax compilation for every `.py` file.
- Import graph check for the Resim support package.
- Search for missing referenced scripts and missing support files.
- Validate that all `#SBATCH` directives use approved partitions/accounts.
- Detect hard-coded paths outside an allowlist.
- Detect `os.system()` and unquoted shell interpolation for review.
- Validate XML files with an XML parser.
- Validate that `support_files.txt` points to readable files or documented runtime resources.

The static stage should not submit Slurm jobs or run containers.

## Phase 2: Replace Interactive Prompts With a Typed Contract

Current behavior:

```text
stdin: N, 1, custom.xml
```

Target behavior:

```text
resim-run --input input.txt --simg app.simg --config custom.xml \
  --mode customize --customer Platform --account GPO-IFV7XX \
  --partition highPrio --bus-tag b04 --rm-zero false
```

Migration steps:

1. Add a wrapper that validates and serializes a manifest.
2. Keep the vendor Python call unchanged initially.
3. Feed the vendor prompt only as a compatibility adapter.
4. Log the resolved mode and XML path before invoking the vendor code.
5. Reject a run if the external XML is missing or unreadable.
6. Later patch the vendor entry point, if permitted, to accept explicit options.

The compatibility adapter must be finite and prompt-aware. Never pipe an unbounded `yes` process into the vendor program.

## Phase 3: Separate Stages and State

Represent the run as explicit stages:

1. `validate_inputs`
2. `prepare_manifests`
3. `submit_resim_array`
4. `wait_resim_array`
5. `submit_video`
6. `submit_mining`
7. `submit_statistics`
8. `submit_mudp`
9. `submit_kpi`
10. `publish_results`

Each stage writes:

```text
stage.json
stage.log
stage.exit
stage.artifacts.json
```

The final run state should be derived from stage states, not only from the parent shell exit code.

## Phase 4: Slurm Hardening

- Replace `wc` output parsing with `wc -l` and numeric validation.
- Validate empty input manifests before calling `sbatch`.
- Use bounded array chunks based on live `MaxArraySize`.
- Capture `sbatch` output and validate a numeric job ID.
- Capture all dependent job IDs separately.
- Use `sacct` for final states, not only `squeue` polling.
- Add timeout and cancellation handling.
- Record account, partition, QoS, memory, CPUs, time, and node list in the run manifest.
- Keep `afterany` semantics explicit; decide whether downstream stages should use `afterok`.
- Avoid fixed customer paths in generated `#SBATCH -o` and `#SBATCH -e` directives.

## Phase 5: Container and Artifact Reproducibility

- Record SIMG SHA256 before submission.
- Record converter SIMG SHA256.
- Record the exact XML SHA256.
- Record the input list SHA256.
- Record the source snapshot commit/version.
- Prefer immutable image paths or a content-addressed image cache.
- Add a lightweight container smoke test for `/RUN_RESIM.sh`.
- Make the expected container contract explicit: input JSON, temp output, config XML.

## Phase 6: Performance Optimization

Measure before changing:

- input scan time
- session discovery time
- cache creation time
- Slurm queue wait
- container startup time
- per-task ReSim time
- video conversion time
- mining/statistics time
- output copy time

Safe optimizations:

- Reuse validated session manifests by input-directory fingerprint.
- Reuse cached file lists when source mtimes and configuration hashes match.
- Avoid repeated recursive scans inside every array task.
- Keep temporary files on node-local storage, but keep control files and final results on shared storage.
- Compress or rotate verbose logs after artifact publication.
- Use a bounded concurrency policy for converter jobs.
- Avoid launching MUDP/KPI until the ReSim stage has produced the required artifacts.
- Use a dependency graph so independent post-processing stages can run concurrently.

## Phase 7: MUDP/KPI Integration

The current ReSim chain does not call MUDP or KPI. Add them as separate stages only after defining their contracts:

```text
ReSim output directory
  -> MUDP input adapter
  -> UDP KPI input/output pair
  -> Interactive Plot input JSON + XML
  -> report aggregator
```

Required contract decisions:

- Which output files are authoritative?
- Is the XML the same Resim XML or a KPI/Interactive Plot XML?
- Which scan metric is displayed?
- Which stage owns output naming?
- What constitutes success if ReSim succeeds but KPI has partial sensor failures?

## Phase 8: CI/CD Pipeline

Recommended pipeline:

```text
commit
  -> lint and syntax checks
  -> dependency/reference graph check
  -> unit tests
  -> shell contract tests
  -> fixture-based manifest test
  -> mock Slurm submission test
  -> container smoke test
  -> staging dry-run
  -> staging run with small fixture
  -> artifact and provenance verification
  -> manual production approval
  -> production submission
```

Production deployment should:

- build a versioned bundle
- publish a manifest of all file hashes
- upload atomically
- verify sentinels and source copies
- restart only the intended service
- run `/health` and broker ping checks
- submit a small canary job
- retain the previous bundle for rollback

## Rollback Plan

Keep:

- previous bundle root
- previous launcher
- previous runtime configuration
- previous image hashes
- previous source manifest
- previous known-good job IDs

Rollback must restore the bundle and runtime config as one versioned unit. Do not mix a new `main_html.simg` with an old broker or old generated source tree.

## Immediate Next Actions

1. Review this snapshot with the Resim owner.
2. Confirm whether the `New` or `new_24aug` source tree is authoritative.
3. Confirm whether the external XML should map only to Customize Docker mode.
4. Define a manifest schema and stop relying on positional `input_parameter` indexes.
5. Add a non-interactive staging harness with fake `sbatch`, `squeue`, and `sacct` commands.
6. Add one small fixture input list and one deterministic expected output manifest.
7. Add explicit MUDP/KPI stages only after their input/output contracts are approved.
