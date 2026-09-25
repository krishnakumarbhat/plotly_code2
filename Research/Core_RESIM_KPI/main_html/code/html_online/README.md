# Log Viewer (Online) - Build & Packaging

This folder contains the online Log Viewer application and PyInstaller spec files for building standalone binaries on Windows and Linux.

Requirements
- Python 3.8+ (match the interpreter used for your target platform)
- `pyinstaller` installed in the target environment
- Recommended packages listed in `requirements.txt` in the project root

Important: run `pip install -r requirements.txt` from the repository root (where `requirements.txt` lives). The README build commands below assume you run them from the repository root.

Windows build (on Windows)
```powershell
# From repository root
cd main_html\code\html_online
pip install -r requirements.txt
pyinstaller --clean --noconfirm log_viewer_online_windows.spec
```

Linux build (on Linux)
```bash
cd main_html/code/html_online
pip install -r requirements.txt
pyinstaller --clean --noconfirm log_viewer_online_linux.spec
```

Notes
- The spec files include static assets under `html_online/static` and sample `db/` content so the packaged binary can run without requiring the repo layout.
- Building Windows binaries should be done on Windows; building Linux binaries should be done on a compatible Linux environment (or use a container).
- The `.gitignore` at repository root ignores build artifacts, virtualenvs, and the `.cache_html` cache directory.

Troubleshooting
- If PyInstaller fails with a `NameError: name '__file__' is not defined` while executing the spec, the spec now falls back to using the current working directory. Ensure you run `pyinstaller` from the `main_html/code` directory (or from the repository root with the path to the spec file) so the spec can locate static assets correctly.

- If PyInstaller fails while inspecting `sqlalchemy` (errors referencing dialects or typing internals), the spec files in this folder exclude `sqlalchemy` by default because this project does not import SQLAlchemy directly. If your environment or another dependency requires SQLAlchemy at runtime, either:
	- Install SQLAlchemy in the build environment and remove it from the `excludes` list in the spec, or
	- Use the PyInstaller CLI flag to exclude it explicitly: `pyinstaller --exclude-module sqlalchemy ...`


Runtime
- After building, the produced executable will be under `dist/log_viewer_online/` (or similar name used in the spec).
- Run the exe and open `http://localhost:5000` in your browser to use the viewer.
