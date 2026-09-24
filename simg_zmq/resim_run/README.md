# ReSim Run Architecture Snapshot

This folder is an isolated architecture snapshot of the Southfield `all_services_7` ReSim launcher and the connected ReSimAutoMng source tree.

## Scope

Snapshot date: 2026-09-18

Remote deployment root:

```text
/mnt/usmidet/projects/RADARCORE/2-Sim/all_services_7
```

Remote ReSim source root:

```text
/mnt/usmidet/projects/GPO-IFV7XX/4-Checkout/ReSimAutoMng
```

Downloaded content:

- `remote_southfield/all_services_7/`: deployed launcher and runtime scripts
- `remote_southfield/ReSimAutoMng/`: connected Python, shell, configuration, and documentation files
- `remote_southfield/DOWNLOAD_MANIFEST.tsv`: source path, snapshot size, and remote timestamp
- `RESIM_ARCHITECTURE.md`: current execution architecture and call chain
- `RESIM_CICD_PLAN.md`: maintenance, CI/CD, observability, and optimization plan
- `resim_architecture.drawio`: editable architecture diagram

Large or runtime-only artifacts were deliberately excluded:

- `.simg`/container images
- virtual environments
- Python bytecode and caches
- logs, data, job staging, and generated result folders
- old backup trees where the same active source was available elsewhere

## Important Finding

The current ReSim chain does not invoke the HPCC MUDP or KPI tools directly. The ReSim child script currently states that HTML, BORDNET, MUDP, and UDP KPI stages were removed from this pipeline. Those tools are separate broker/runtime tools in the surrounding `simg_zmq` platform and must be modeled as a downstream or parallel workflow, not as a direct child of `trig_helios.sh` (renamed from `rResim_Gen7.sh`).

## Current Southfield Call Shape

```text
trig_helios.sh <input.txt> <resim.simg> highPrio <b02|b04>
```

The wrapper activates the Southfield Gen7 environment and calls `resim_main.py`. The Python entry point creates input manifests and generated Slurm scripts, then invokes `rResim_main.sh` or `rResim_main_highPrio.sh`. Those scripts submit the ReSim array and dependent mining/statistics jobs.

This snapshot is documentation and source evidence only. It does not change the existing application, launcher, deployment bundle, or remote cluster.
