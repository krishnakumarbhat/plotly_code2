"""
VLM job client — submits Slurm batch jobs to run Gemma-4 VLM analysis
inside rag.simg on a cluster compute node. Communicates via SSH.

Usage:
    client = VlmJobClient(ssh_client)
    result = client.process_video('/net/.../video.mp4', force=True)
    # Returns immediately with {success, status, job_id} or {success, status: 'skipped', description}
"""
import json
import os
import time
from typing import Optional

import paramiko

RAG_SIMG_PATH = os.environ.get('VLM_RAG_SIMG_PATH', '')
GEMMA_GGUF_PATH = os.environ.get('GEMMA_GGUF_PATH', '')

VLM_SCRIPT = '/app/rag/vlm_process.py'
DEFAULT_PARTITION = os.environ.get('VLM_PARTITION', 'compute')
DEFAULT_ACCOUNT = os.environ.get('VLM_ACCOUNT', '')
DEFAULT_MEMORY = os.environ.get('VLM_MEMORY', '64G')
DEFAULT_TIMEOUT = int(os.environ.get('VLM_POLL_TIMEOUT', '300'))


class VlmJobClient:
    def __init__(self, ssh_client: paramiko.SSHClient):
        self.ssh = ssh_client

    def process_video(self, video_path: str, force: bool = False,
                      partition: str = '', account: str = '',
                      memory: str = '', poll: bool = True) -> dict:
        if not RAG_SIMG_PATH:
            return {'success': False, 'error': 'VLM_RAG_SIMG_PATH not configured'}
        if not GEMMA_GGUF_PATH:
            return {'success': False, 'error': 'GEMMA_GGUF_PATH not configured'}

        txt_path = self._remote_txt_path(video_path)

        # Fast path: .txt already exists on cluster
        if not force:
            existing = self._read_remote_file(txt_path)
            if existing is not None:
                return {'success': True, 'status': 'skipped', 'description': existing, 'text_path': txt_path}

        # Submit sbatch job
        job_id = self._submit_sbatch(video_path, txt_path, partition or DEFAULT_PARTITION,
                                     account or DEFAULT_ACCOUNT, memory or DEFAULT_MEMORY, force)
        if job_id is None:
            return {'success': False, 'error': 'Failed to submit Slurm job'}

        if not poll:
            return {'success': True, 'status': 'submitted', 'job_id': job_id, 'text_path': txt_path}

        # Poll for completion
        deadline = time.time() + DEFAULT_TIMEOUT
        while time.time() < deadline:
            state = self._check_job(job_id)
            if state == 'completed':
                content = self._read_remote_file(txt_path)
                if content is not None:
                    return {'success': True, 'status': 'processed', 'description': content,
                            'text_path': txt_path, 'job_id': job_id}
                return {'success': False, 'error': 'Job completed but result file missing', 'job_id': job_id}
            if state in ('failed', 'cancelled', 'timeout'):
                error_msg = self._read_file_remote(f'/tmp/vlm_{job_id}.err')
                return {'success': False, 'error': f'Job {state}: {error_msg}', 'job_id': job_id}
            time.sleep(5)

        return {'success': True, 'status': 'submitted', 'job_id': job_id,
                'text_path': txt_path, 'error': 'Job still running, check back later'}

    def check_job(self, job_id: str) -> str:
        return self._check_job(job_id)

    def _submit_sbatch(self, video_path: str, txt_path: str,
                       partition: str, account: str, memory: str, force: bool) -> Optional[str]:
        account_flag = f'--account={account}' if account else ''
        job_name = f'vlm_{os.path.basename(video_path)[:30]}'
        force_flag = '--force' if force else ''
        model_dir = os.path.dirname(GEMMA_GGUF_PATH)

        script = f'''#!/bin/bash
#SBATCH --job-name={job_name}
#SBATCH --partition={partition}
#SBATCH --mem={memory}
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --time=00:30:00
#SBATCH --output=/tmp/vlm_{job_name}_%j.out
#SBATCH --error=/tmp/vlm_{job_name}_%j.err
{account_flag}

singularity exec --bind "{model_dir}:/models" "{RAG_SIMG_PATH}" \\
  python {VLM_SCRIPT} "{video_path}" \\
  --model "{GEMMA_GGUF_PATH}" {force_flag}
'''
        stdin, stdout, stderr = self.ssh.exec_command(
            f'cat << "VLMEOF" | sbatch\n{script}\nVLMEOF', timeout=30
        )
        output = stdout.read().decode().strip()
        error = stderr.read().decode().strip()
        parts = output.split()
        job_id = parts[-1] if parts else ''
        if job_id.isdigit():
            return job_id
        return None

    def _check_job(self, job_id: str) -> str:
        try:
            stdin, stdout, stderr = self.ssh.exec_command(
                f'sacct -j {job_id} --format=State --noheader --parsable2', timeout=15
            )
            states = [l.strip() for l in stdout.read().decode().splitlines() if l.strip()]
            if not states:
                return 'unknown'
            state = states[-1].split('|')[0] if '|' in states[-1] else states[-1].split()[0]
            if state in ('COMPLETED',):
                return 'completed'
            if state in ('FAILED',):
                return 'failed'
            if state in ('CANCELLED',):
                return 'cancelled'
            if state in ('TIMEOUT',):
                return 'timeout'
            return 'running'
        except Exception:
            return 'unknown'

    def _remote_txt_path(self, video_path: str) -> str:
        base = os.path.splitext(os.path.basename(video_path))[0]
        if base.endswith('_web'):
            base = base[:-4]
        return os.path.join(os.path.dirname(video_path), 'log_txt', f'{base}_web.txt')

    def _read_remote_file(self, path: str) -> Optional[str]:
        try:
            return self._read_file_remote(path)
        except Exception:
            return None

    def _read_file_remote(self, path: str) -> Optional[str]:
        stdin, stdout, stderr = self.ssh.exec_command(f'cat "{path}"', timeout=15)
        content = stdout.read().decode().strip()
        error = stderr.read().decode().strip()
        if error and not content:
            return None
        return content
