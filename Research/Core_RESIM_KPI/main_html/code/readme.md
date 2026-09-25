# Log Viewer - Build Instructions

## Prerequisites
Install PyInstaller:
```bash
pip install pyinstaller
```

## Building

### Windows
```bash
pyinstaller log_viewer_windows.spec
```
The executable will be in `dist/log_viewer.exe`

### Linux
```bash
pyinstaller log_viewer_linux.spec
```
The executable will be in `dist/log_viewer`

## Usage
```bash
# Windows
log_viewer.exe <html_root> <video_root> [output_html]

# Linux
./log_viewer <html_root> <video_root> [output_html]

# Example
log_viewer db/html db/video out/viewer.html
```

## Online (Web Server) Mode

Run the viewer as a web service (intended for LAN / cluster use):

```bash
cd main_html/code
python main.py --serve [html_root] [video_root] --host 0.0.0.0 --port 5000 --no-browser
```

- `--host` / `--port` control the bind address and port.
- `--no-browser` prevents auto-opening a local browser (useful for servers).
- If `html_root` / `video_root` are omitted, the server uses a per-user cache folder:
	- Linux: `~/.cache_html/html` and `~/.cache_html/video`
	- Windows: `C:\\.cache_html\\html` and `C:\\.cache_html\\video`

## Singularity (Cluster) Deployment

This repo includes a minimal Singularity definition that runs the online viewer.

### Building the .simg image

**On Windows (via WSL):**
```batch
main_html\code\singularity\build_image.bat
```

**On Linux / inside WSL:**
```bash
bash main_html/code/singularity/build_image.sh
```

The output is `main_html/code/singularity/log_viewer.simg`. Copy it to your cluster.

### Running with Slurm

Transfer `log_viewer.simg` and `slurm_log_viewer.sh` to the cluster, then:

```bash
# Submit the job (runs indefinitely until cancelled or time limit)
sbatch slurm_log_viewer.sh

# Check the job
squeue -u $USER

# The job log (log_viewer_<jobid>.out) prints the access URL:
#   http://<node-ip>:5000/
```

Customise port or partition:
```bash
PORT=8080 sbatch --partition=long slurm_log_viewer.sh
```

### Running without Slurm (interactive)

```bash
PORT=5000 bash singularity/run_log_viewer.sh ./log_viewer.simg
```

### Networking notes

- Singularity uses **host networking**, so the Flask server binds directly to the node's IP.
- Users on the company LAN (10.x.x.x) can access `http://<node-ip>:<port>/`.
- For a friendly URL like `http://10.x.x.x/html`, place an Nginx/Apache reverse proxy in front.
- Ensure firewall rules allow inbound traffic on the chosen port.

## Notes
- Both spec files create single-file executables (--onefile mode)
- The `html_build` module is included as a hidden import
- Windows version keeps console window open for output
- Linux version uses strip=True for smaller binary size
- UPX compression is enabled for both platforms
