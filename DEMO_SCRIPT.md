# Demo Script — Resim Run + KPI Platform (30 min)

Follow the HTML deck (`DEMO_PRESENTATION.html`) slide by slide. Times include demo action items.
Golden rule the audience should remember: **CAN KPI → CAN Interactive Plot · UDP KPI → Interactive Plot**.

---

## 0–3 min · Context

> "This console is the single door to three things: Resim Run, KPI analysis, and interactive plotting.
> Today, doing any of these means SSH-ing to a cluster, sourcing environments, and watching a terminal.
> Here, everything is a web form; the broker (port 9100) queues the job, and every run is recorded in SQLite — owner, node, exit code, log, artifacts."

**Demo:** open the dashboard `/html`, click Runtime Map, show the topology graph (main_html → can_kpi / udp_kpi / interactive_plot / rag / hyperlink).

---

## 3–10 min · Resim Run — the focus

> "Resim Run launches a customer resimulation. The user gives two paths: the scenario `input.txt` and the app `.simg`. The console validates they are on the same cluster partition, detects the project and account from the path, then runs the vendor script `rResim_Gen7.sh` as the logged-in user over SSH, under Slurm."

**Demo steps:**
1. Runtime Map → "Resim Run" form → fill `input.txt` + `.simg` → click **Detect** (shows cluster / project / partition).
2. Optionally tick "Create JIRA ticket".
3. Click **Run Resim** → you land on the Runtime Console.
4. Show the live tail (polls every 3 s), the status chip, the stored log path.

> "Three things make this robust: credentials are handled via askpass (no typing on the server), vendor prompts are auto-answered, and the run is tmux-backed so it survives a laptop disconnect. If it fails, the console sniffs the log and tells us *why* — permission denied, missing file, traceback. Note: this vendor script always exits 0, so the console checks failure markers, not just the exit code."

---

## 10–14 min · KPI 101 + the form

> "KPI means Key Performance Indicators: how well the recorded sensor data matches the reference. It compares an input HDF against an output HDF and produces an HTML report with precision, recall, F1, accuracy and overall match % per sensor."

**Demo:** open KPI Analysis page. Show:
- Execution target radio (CAN KPI / UDP KPI)
- The two toggles: **Enable Interactive Plot** (UDP only) / **Enable CAN Interactive Plot** (CAN only)
- Input mode (JSON batch / HDF pair)
- Config files panel — view / validate / save / upload (validated before saving, auto backup)
- Resource presets (partition, customer account, memory, CPUs, time)

> "People often ask *how do I even run KPI?* Answer: it's one form. Pick the target, toggle the plot you want, fill paths, press Queue through broker. The broker records the request and spawns the container on a Slurm compute node."

---

## 14–18 min · CAN path

> "For CAN: enable CAN KPI and CAN Interactive Plot. One broker request produces both outputs together."

**Demo (optional live, else show a completed run):**
- CAN KPI report: per-sensor tabs, Overview, KPI summary table, radar/match plot, `index.html` across logs
- CAN Interactive Plot next to it

> "Note the generic Interactive Plot is *not available* for CAN — the CAN pipeline has its own plot path. The UI enforces this; you can't combine the wrong pair."

---

## 18–21 min · UDP path

> "For UDP: UDP KPI + Interactive Plot. The KPI server runs in ZMQ mode; the plot client subscribes and renders the live, connected view — boxes for sensors linked like a neural network."

**Demo:** show the combined flow (tmux wrapper, two panes: ZMQ server + plot client), then the final HTML report.

---

## 21–24 min · Plots as a debug tool

> "Why do we need the plots at all? Because a KPI number alone tells you *that* something is wrong, not *what*. The CAN Interactive Plot shows raw signal traces and their distribution — you can see flat lines (sensor not parsed), drift between input and output, or outliers that skew averages. The UDP connected view shows you the topology and where data stops flowing."

**Demo:** point at traces, histogram/distribution, input-vs-output overlay in a real report.

---

## 24–26 min · How the KPI averages

> "How is the average taken? First, scans are aligned by scanindex — only common scans compare. Detections are matched scan-by-scan. Each scan gets precision, recall, F1, accuracy. Then `nanmean` averages those per-sensor arrays into one number per metric, and sensor averages are averaged into the Overall Accuracy / Overall F1 on the index page. If accuracy drops below 60%, an alert can auto-create a JIRA ticket."

---

## 26–28 min · Runtime console & reuse

> "The output is shown in the Runtime Console — a live terminal, polled every 3 seconds, with the stored log path and generated artifacts. The owner can send input or Ctrl+C while it runs; teammates watch read-only."

> "And the best part — **you don't need to trigger it again and again.** Every request is fingerprinted. Submit the same scenario, and the console asks: *this run already exists, open the stored result instead?* Cancel reuses it — zero compute. OK reruns it deliberately. History keeps a link to the original run."

**Demo:** submit the same scenario again → reuse dialog → open stored console.

---

## 28–30 min · Recap + Q&A

> "Resim Run = submit a customer resim from a form, watch it live. CAN KPI pairs with CAN Interactive Plot; UDP KPI pairs with Interactive Plot. Plots show raw data for debugging. Averages = aligned scans → per-scan metrics → nanmean → overall. Every run lives in the console and is reusable — never trigger twice. Thank you, questions?"

---

## Fallback plan (if Slurm is slow)
Show the reuse dialog on a completed job and the History tab first — instant, and proves the platform without waiting for a queue.