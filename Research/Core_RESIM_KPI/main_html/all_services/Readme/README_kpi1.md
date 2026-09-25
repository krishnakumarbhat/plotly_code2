# KPI Direct HPCC Usage

This document is the operator guide for running CAN KPI, UDP KPI, and Interactive Plot directly on HPCC from the regenerated `simg_sh_hpcc` bundle.

The current deployment model is strict:

- only `main_html` and Hyperlink should stay active on the login node
- KPI work must run through Slurm from the UI or from direct bundle scripts on compute resources
- combined UDP + Interactive Plot runs should use tmux-backed wrappers so the job survives a laptop disconnect

## Bundle Layout

After `bash scripts/wsl_build_hpcc_bundle.sh`, the HPCC bundle layout is:

```text
simg_sh_hpcc/
  main_html.simg
  hpcc_main.pyz
  main_hpcc.sh
  kpi_runtime_launcher.sh
  kpi/
    can/
      can_kpi.simg
      run_can.sh
    udp/
      udp_kpi.simg
      run_udp.sh
    int_plot/
      intplot_kpi.simg
      run_intplot.sh
    inplot_udp.sh
    inplot_can.sh
  rag/
    rag.simg
    run_rag.sh
```

## Login Node Policy

Use the login node only for the web front end:

```bash
cd simg_sh_hpcc
PORT=5006 HPCC_BROKER_PORT=9106 ./main_hpcc.sh
```

By default this launcher exports:

- `HPCC_REQUIRE_SLURM_FOR_KPI=1`
- `HPCC_ALLOW_LOCAL_KPI=0`
- `HPCC_SLURM_IMMEDIATE_SECONDS=60`

That means KPI requests fail fast if Slurm resources are not available instead of silently running on the login node.

## Direct Terminal Usage

All commands below are intended to be run from inside `simg_sh_hpcc` on HPCC.

### CAN KPI

JSON mode:

```bash
./kpi/can/run_can.sh /path/to/kpi.json /path/to/output/can_run
```

Compatibility form with XML in front. The XML argument is accepted but ignored by the CAN wrapper:

```bash
./kpi/can/run_can.sh /path/to/ConfigInteractivePlots.xml /path/to/kpi.json /path/to/output/can_run
```

HDF mode:

```bash
./kpi/can/run_can.sh --hdf /path/to/input.h5 /path/to/output.h5 /path/to/output/can_run
```

### UDP KPI

JSON mode:

```bash
./kpi/udp/run_udp.sh /path/to/kpi.json /path/to/output/udp_run
```

HDF mode:

```bash
./kpi/udp/run_udp.sh --hdf /path/to/input.h5 /path/to/output.h5 /path/to/output/udp_run
```

ZMQ server mode only:

```bash
./kpi/udp/run_udp.sh zmq 5560
```

### Interactive Plot Only

```bash
./kpi/int_plot/run_intplot.sh /path/to/ConfigInteractivePlots.xml /path/to/InputsInteractivePlot.json /path/to/output/intplot_run
```

With optional plot config:

```bash
./kpi/int_plot/run_intplot.sh /path/to/ConfigInteractivePlots.xml /path/to/InputsInteractivePlot.json /path/to/output/intplot_run /path/to/config.json
```

## Combined Flows

### UDP KPI + Interactive Plot

This is the wrapper to use when UDP + KPI should not get stuck behind a single terminal. It starts two tmux windows:

1. UDP KPI server in ZMQ mode
2. Interactive Plot client pointed at that ZMQ server

Detached tmux launch:

```bash
./kpi/inplot_udp.sh /path/to/ConfigInteractivePlots.xml /path/to/InputsInteractivePlot.json /path/to/output/udp_intplot 5560 /path/to/config.json
```

Wait for completion in the current shell:

```bash
./kpi/inplot_udp.sh --wait /path/to/ConfigInteractivePlots.xml /path/to/InputsInteractivePlot.json /path/to/output/udp_intplot 5560
```

The script prints the tmux session name, run directory, and log paths.

### CAN KPI + Interactive Plot

CAN does not use the UDP ZMQ server path, so this wrapper runs the combined flow through one tmux session with a checked-in launcher:

```bash
./kpi/inplot_can.sh /path/to/ConfigInteractivePlots.xml /path/to/InputsInteractivePlot.json /path/to/output/can_intplot /path/to/config.json
```

Blocking mode:

```bash
./kpi/inplot_can.sh --wait /path/to/ConfigInteractivePlots.xml /path/to/InputsInteractivePlot.json /path/to/output/can_intplot
```

## Generic Launcher

`kpi_runtime_launcher.sh` is the common entrypoint used by the bundle wrappers and the broker.

Examples:

UDP KPI only:

```bash
./kpi_runtime_launcher.sh --target udp_kpi --input-mode json --json-path /path/to/kpi.json --output-dir /path/to/output/udp_run
```

Interactive Plot only:

```bash
./kpi_runtime_launcher.sh --target interactive_plot --input-mode json --json-path /path/to/InputsInteractivePlot.json --config-xml /path/to/ConfigInteractivePlots.xml --output-dir /path/to/output/intplot_run
```

UDP KPI + Interactive Plot in detached tmux mode:

```bash
./kpi_runtime_launcher.sh --target udp_kpi --source-target udp_kpi --interactive-mode enabled --input-mode json --json-path /path/to/InputsInteractivePlot.json --config-xml /path/to/ConfigInteractivePlots.xml --output-dir /path/to/output/udp_intplot --port 5560 --detached
```

CAN KPI + Interactive Plot in detached tmux mode:

```bash
./kpi_runtime_launcher.sh --target can_kpi --source-target can_kpi --interactive-mode enabled --input-mode json --json-path /path/to/InputsInteractivePlot.json --config-xml /path/to/ConfigInteractivePlots.xml --output-dir /path/to/output/can_intplot --detached
```

## RAG Helper

Start the bundled RAG service manually:

```bash
./rag/run_rag.sh
```

Scrape an HTML root into the RAG pipeline:

```bash
./rag/run_rag.sh --scrap /path/to/html_root
```

## Logs And Output

- `logs/main_html.log` and `logs/hpcc_broker.log` contain login-node service logs
- `runs/<user>/...` contains per-launch tmux logs and exit codes for the detached wrappers
- direct CAN, UDP, and Interactive Plot outputs go to the output directory passed to the wrapper

## RETURN_CODE -9

`RETURN_CODE: -9` means the process was killed by signal 9. On HPCC that usually means one of these:

- Slurm killed the step because memory was exceeded
- an admin or cleanup script killed the process
- the process died during node pressure and the scheduler reaped it

Check these in order:

1. the per-run log under `runs/<user>/...`
2. the generated job log path recorded by the broker
3. `sacct -j <job_id> --format=JobID,State,ExitCode,Elapsed,MaxRSS,NodeList`
4. the `cgroup memory.max` and `memory.current` lines written into the local execution context

## Internal Notes

The runtime scripts call the same packaged container entrypoints used by the broker:

- CAN KPI supports `json` and `hdf`
- UDP KPI supports `json`, `hdf`, and `zmq`
- Interactive Plot expects `<config.xml> <inputs.json> [output_dir] [plot_config.json]`

That is why the documented commands above should be treated as the source of truth for direct HPCC usage.