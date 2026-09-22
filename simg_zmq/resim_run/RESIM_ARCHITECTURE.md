# ReSim Gen7 Internal Architecture

## 1. Executive Summary

The current Southfield ReSim flow is a layered, generated-script pipeline rather than one executable program:

```text
Runtime Map / SSH
  -> all_services_7/rResim_Gen7.sh
  -> ReSimAutoMng/Support/Resim/resim_main.py
  -> resim_generic.py user input and validation
  -> generated jobout manifests and generated Slurm scripts
  -> ReSimAutoMng/Shell/rResim_main_highPrio.sh
  -> Slurm ReSim array
  -> generated .resim.sh task script
  -> resim_child.py generated task body
  -> splitter / JSON preparation / singularity execution
  -> temporary output copied into the result tree
  -> optional VIDEO conversion
  -> dependent mining and statistics jobs
```

The current HPCC wrapper passes only the input list, SIMG path, and `highPrio` mode. The XML/custom Docker choice is interactive inside `resim_main.py`; it is not a positional argument to `rResim_Gen7.sh`.

## 2. Entry Point

### `rResim_Gen7.sh`

The deployed wrapper is version `3.0`.

It performs three operations:

1. Detects Southfield when the first argument contains `/projects/`.
2. Sources the Southfield Gen7 virtual environment.
3. Executes the external `resim_main.py` with all original arguments.

Southfield paths embedded in the wrapper include:

```text
Environment:
/mnt/usmidet/projects/RADARCORE/2-Sim/USER_DATA/z5daa9/virtual_env/gen7v2/bin/activate

Python entry point:
/mnt/usmidet/projects/GPO-IFV7XX/4-Checkout/ReSimAutoMng/Support/Resim/resim_main.py
```

The wrapper does not validate XML, create output directories, submit Slurm jobs, or run KPI/MUDP. It is a routing and environment bootstrap script.

## 3. Python Control Plane

### `resim_main.py`

`resim_main.py` imports `resim_generic` and executes the control sequence:

1. `validate_argv()`
2. `childCreation()`
3. `collectsessions()`
4. `collectInputLogs()`
5. append bus-tag and zero-segment settings
6. invoke the selected shell pipeline through `os.system()`

The final command is assembled from the mutable global `input_parameter` list. This is a major maintenance risk because positional indexes carry meaning across Python and shell boundaries.

### `resim_generic.py`

This module owns the interactive setup and generated-job preparation.

It:

- validates `.txt` and `.simg` inputs
- detects Southfield from the current working path
- selects customer/project defaults
- parses optional `highPrio`, `b02`, `b04`, and `rm_zero` arguments
- asks whether the default Docker configuration should be used
- asks for a menu selection when the answer is not `Y`
- validates the custom XML path with `.xml` and `os.path.isfile`
- creates the result directory and jobout directory
- writes generated `.resim.sh`, `.rming.sh`, and `.sming.sh` files
- builds `SIL_Input_all.txt`, `SIL_input_session.txt`, JSON manifests, and cache lists
- invokes the high-priority or normal shell launcher

For the live vendor prompt, the external XML sequence is:

```text
N
1
/path/to/custom.xml
EOF
```

`N` selects non-default configuration, `1` selects `Customize Docker (CDC | RDD | DET | AF)`, and the XML path is then validated and stored in `input_parameter`.

## 4. Input Parameter Contract

The Python code passes a comma-separated serialized list into the shell launcher. The effective fields are:

| Position | Meaning |
|---|---|
| 0 | Shell launcher path, normally `Shell/rResim_main.sh` or high-priority variant |
| 1 | Input list path |
| 2 | SIMG path |
| 3 | Customer type, for example `Platform` |
| 4 | Slurm project/account, for example `GPO-IFV7XX` |
| 5 | Configuration mode, for example `Customize Docker` or `Default` |
| 6 | Custom XML path when customization is selected |
| 7 | Generated execution/result path |
| 8 | High-priority boolean |
| 9 | UPU flag |
| 10 | Bus tag, normally `b04` or `b02` |
| 11 | Zero-segment removal flag |

This positional protocol is the central coupling point between Python, shell, and Slurm. It should eventually be replaced with a structured manifest.

## 5. Manifest and Session Preparation

`resim_splitter.py` and the related functions in `resim_generic.py` prepare the workload before Slurm submission.

The preparation phase can:

- read the input list
- detect nested session directories
- collect session/log metadata
- create `SIL_Input_all.txt`
- create `SIL_input_session.txt`
- create per-batch JSON/TXT files
- create a session manifest
- cache files such as GPO-V2, MCIP, DGPS, bus, and b05 lists
- filter zero segments when `rm_zero` is enabled
- preserve session-level output structure

The caches are intended to avoid repeating expensive recursive file discovery in every Slurm task.

## 6. Generated Slurm Orchestration

### `Shell/rResim_main_highPrio.sh`

The high-priority launcher:

1. Parses the comma-separated `input_parameter` list.
2. Selects `-p highPrio`, or `-p 8k3` for the Helios account case.
3. Reads `jobout/SIL_Input_all.txt`.
4. Splits large workloads into Slurm arrays of at most 1000 tasks.
5. Submits the generated `.resim.sh` array script with `sbatch`.
6. Submits dependent ReSim mining and statistics jobs using `afterany`.
7. Writes `RESIM_details.txt` with paths, mode, config, task counts, and job IDs.
8. Polls Slurm status and prints a pipeline summary.

The normal `Shell/rResim_main.sh` follows the same model without the high-priority partition choice.

### Generated `.resim.sh`

The generated task script is produced from `resim_child_script_highPrio()` or `resim_child_script()`.

It requests resources such as:

- one node
- one task
- six CPUs
- 80 GB memory in the normal/high-priority task template
- a long wall-clock limit

Each array task:

1. Resolves its input session from `SIL_Input_all.txt`.
2. Creates a temporary directory in `/dev/shm`.
3. Sources the required Singularity module.
4. Runs the SIMG through `/RUN_RESIM.sh` with the prepared input JSON, temporary output, and custom XML config.
5. Copies temporary output to the result session directory.
6. Optionally runs VIDEO conversion.
7. Removes temporary files.

ADCAM/`ifv600_` images use a separate `singularity run` calling convention and do not use `/RUN_RESIM.sh`.

## 7. Resim Execution and Output

The SIMG is the execution boundary. For the standard path the generated task calls:

```text
singularity exec <simg> /RUN_RESIM.sh <input-json> <temp-output> <config-xml>
```

After the container exits:

- the task copies the temporary result into `$OUT/output/<session>`
- `.mining.txt` receives task log references
- `.SIL_Statistics.txt` receives XML output references
- VIDEO output is created under the session `VIDEO` directory when converter metadata exists
- non-video temporary converter files are removed

## 8. Video Phase

The task script extracts converter settings from `support_files.txt`:

- `converter_simg`
- `converter_config`
- `converter_config_dgps`

It prepares converter JSON files from the ReSim input data, invokes:

```text
singularity exec <converter.simg> /RUN_CONVERTER.sh <converter-config> <converter-json> <video-output>
```

It then counts video files, logs converter status, and deletes non-video artifacts from the VIDEO directory.

## 9. Mining and Statistics

The high-priority launcher submits dependent jobs after the ReSim array:

- `JB-min.py` through `.rming.sh`
- `stats_mining.py` through `.sming.sh`

These jobs consume output references written by the task scripts. They are not part of the main ReSim container invocation, but they are part of the user-visible pipeline completion.

## 10. MUDP and KPI Boundary

The current ReSim child template explicitly states that HTML, BORDNET, MUDP, and UDP KPI stages were removed from this pipeline. Therefore:

- `rResim_Gen7.sh` does not call MUDP.
- `rResim_Gen7.sh` does not call UDP KPI.
- `rResim_Gen7.sh` does not call Interactive Plot.
- Those tools are separate HPCC broker/runtime tools in `simg_zmq`.

A future combined workflow should make the relationship explicit:

```text
ReSim output
  -> optional MUDP/UDP KPI job
  -> optional Interactive Plot job
  -> optional report aggregation
```

It should not hide those stages inside the current positional ReSim contract.

## 11. Current Risks

1. Interactive input is required and historically easy to answer incorrectly.
2. The external XML path is passed through stdin, not as a typed CLI option.
3. The `input_parameter` list is a positional, comma-separated API.
4. `os.system()` obscures exit status and quoting failures.
5. Several paths are hard-coded to customer/project layouts.
6. Python generates shell code dynamically, making static validation difficult.
7. Slurm task and dependent-job errors can be separated across several logs.
8. The task script embeds resource directives and paths from source templates.
9. The current launcher has different Southfield and Krakow code paths.
10. The live deployment can contain stale launcher/source copies if only part of the bundle is updated.

## 12. Recommended Target Architecture

Use a versioned, non-interactive job contract:

```text
resim-run.yaml or resim-run.json
  input_list
  simg
  mode
  custom_config
  customer
  project/account
  partition
  bus_tag
  rm_zero
  output_root
```

Then implement a staged runner:

```text
validate
 -> normalize paths
 -> create run manifest
 -> create deterministic jobout
 -> submit ReSim array
 -> wait/collect task state
 -> submit mining/statistics dependencies
 -> optional MUDP/KPI/Interactive Plot stages
 -> publish artifacts and machine-readable summary
```

Every stage should write a status file and structured JSON event record.
