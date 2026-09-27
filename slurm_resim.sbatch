#SBATCH --job-name=apprism-resim
#SBATCH --nodes=1
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=4G
#SBATCH --time=12:00:00
#SBATCH --output=resim_out/slurm_%A_%a.out
#SBATCH --array=0-19
# Slurm replay worker template (HPCC plan, Appendix B of research.md).
# Each task replays its halo slice; alignment headers injected per
# Solution A (converged calibration) with Solution B (fast-lock DA) fallback.
# Environment needed on cluster: JFROG_ACCESS_TOKEN, SIL_EXE, input_path.xml.
set -euo pipefail
echo "[slurm] task $SLURM_ARRAY_TASK_ID of $SLURM_ARRAY_TASK_COUNT"
echo "[slurm] TODO(cluster): srun $SIL_EXE --input $MANIFEST --slice $SLURM_ARRAY_TASK_ID"
echo "[slurm] template only — no local execution."
