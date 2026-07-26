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
- Default: Qwen3.5-2B-Q5_K_S, 64GB RAM, 1 GPU
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
