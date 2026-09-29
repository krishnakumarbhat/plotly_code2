# HPCC Runtime Console - Deployment

## Structure
```
deploy_root/
├── run_hpcc.sh            ← Launcher (replaces main_hpcc.sh + run_hpcc_stack.sh)
├── hpcc_main.pyz          ← Broker (TCP 9100)
├── main_html.simg         ← Flask web app (port 5002)
├── rag.simg               ← RAG service with llama.cpp (port 5100)
├── kpi/
│   ├── can/can_kpi.simg
│   ├── udp/udp_kpi.simg
│   └── int_plot/intplot_kpi.simg
├── bundle_src/            ← Live source (bind-mounted)
│   ├── main_html/
│   ├── Hyperlink_tool/
│   ├── KPI/
│   ├── rag/
│   └── jira/
├── store/                 ← Runtime data
│   ├── db/hpc_tools_dev.db
│   ├── logs/
│   └── rag/vector_store/
└── README_deploy.md
```

## Commands

| Component | Without SIMG | With SIMG |
|---|---|---|
| Flask app | `gunicorn main_html.app:app` | `singularity run main_html.simg` |
| Broker | `python3 hpcc_main.pyz` | `python3 hpcc_main.pyz` |
| RAG | `python3 rag/run.py --talk` | `singularity run rag.simg --talk` |
| CAN KPI | `python3 KPI/can_kpi/kpi_main.py json <in> <out>` | `singularity run can_kpi.simg json <in> <out>` |
| UDP KPI | `python3 KPI/UDP_KPI/kpi_server.py json <in> <out>` | `singularity run udp_kpi.simg json <in> <out>` |
| Int Plot | `python3 KPI/intplot_kpi/ResimHTMLReport.py <xml> <in> <out>` | `singularity run intplot_kpi.simg <xml> <in> <out>` |

## Deployment Steps

1. Build images: `python3 generate_upload.py build`
2. Build broker: `python3 generate_upload.py pyz`
3. Copy to cluster:
   - Krakow: `/net/8k3/e0fs01/irods/PLKRA-PROJECTS/RNA-SDV-SRR7/4-Checkout/all_services_3`
   - Southfield: `/mnt/usmidet/projects/RADARCORE/2-Sim/all_services_3`
4. Run: `cd deploy_root && bash run_hpcc.sh`
5. Open: `http://<cluster-ip>:5002/html`

## Resource Allocation
Edit `resources.py` to change default partitions, accounts, memory, etc.
- Krakow: partition=plcyf-com, account=RNA-SDV-SRR7
- Southfield: partition=defq, account=radarcore

## RAG Service
- llama.cpp is bundled INSIDE rag.simg
- GGUF model must be bind-mounted at runtime
  - Krakow: `/net/8k3/.../rag/model/`
  - Southfield: `/mnt/usmidet/.../rag/model/`
- Default: Qwen3.5-2B-Q5_K_S ( or any other model), 64GB RAM, 1 GPU or 1 cpu
- Runs on port 5100, auto-starts with main_html

## Jira Integration
- Configure `jira/jira_config.json` or set env vars
- Default project: HZP, Board: 14936
- Auto-creates tickets for KPI accuracy < 50%
- Assignee: configured in jira_config.json

## Cleanup
```bash
bash clean_temp.sh /local/hpc_tools /path/to/permanent/store
```
Results are saved to permanent storage, then /local and /tmp are purged.

# HPCC Runtime Console - Deployment

## Structure
```
deploy_root/
├── run_hpcc.sh            <- Launcher
├── hpcc_main.pyz          <- Broker (TCP 9100)
├── main_html.simg         <- Flask web app (port 5005, auto-increments)
├── rag.simg               <- RAG + VLM service (port 5100)
├── kpi/
│   ├── can/can_kpi.simg
│   ├── udp/udp_kpi.simg
│   └── int_plot/intplot_kpi.simg
├── bundle_src/            <- Live source (bind-mounted)
├── store/                 <- Runtime data
├── resources.py           <- Resource allocation
├── jira/                  <- Jira integration
└── README_deploy.md
```

## Usage
1. `python generate_upload.py deploy` on machine with apptainer
2. Copy `generate_upload/` to cluster
3. `cd generate_upload && bash run_hpcc.sh`
4. Open `http://<cluster-ip>:5005/html`

## Resource Allocation
Edit `resources.py` to change partitions, accounts.
- Krakow: partition=plcyf-com, account=RNA-SDV-SRR7
- Southfield: partition=defq, account=radarcore

## RAG Service
- llama.cpp bundled inside rag.simg, GGUF model bind-mounted at runtime
- Port 5100, auto-starts with main_html

## VLM (Vision Language Model) — Gemma-4 via Slurm
When user clicks "Generate Summary" in Hyperlink viewer:
1. Backend submits Slurm batch job: `srun --partition=<video_partition> --mem=64G --nodes=1`
2. Runs `rag.simg` with `vlm_process.py` inside
3. `vlm_process.py` starts llama-server with Gemma-4 GGUF, extracts video frames, sends to VLM
4. Writes description to `.txt` file next to video on the cluster
5. UI polls for completion, downloads result, shows summary

### Required env vars (set in `hpcc_runtime.env`):
```
VLM_RAG_SIMG_PATH=/path/to/rag.simg          # On cluster filesystem
GEMMA_GGUF_PATH=/path/to/gemma-4-vl.gguf     # Bind-mounted into container at /models
VLM_PARTITION=compute                        # Slurm partition (defaults to compute)
VLM_ACCOUNT=your-project                     # Optional Slurm account
VLM_MEMORY=64G                               # Memory per VLM job
VLM_POLL_TIMEOUT=300                         # Max wait seconds for job completion
```
- `rag.simg` must be built with `vlm_process.py` and `opencv-python-headless` (handled by `generate_upload.py deploy`)
- Gemma-4 GGUF model must be accessible on the cluster (not inside the container)
