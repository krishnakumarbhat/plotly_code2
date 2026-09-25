"""
main.py
=======
Entry point for the TRATON Radar Plan View GUI.

Run from the project root:
    .\\venv\\Scripts\\python gui\\main.py
"""

import sys
from pathlib import Path

# Ensure gui/ is importable when running directly.
_GUI = Path(__file__).resolve().parent
if str(_GUI) not in sys.path:
    sys.path.insert(0, str(_GUI))

from PyQt6.QtWidgets import QApplication
from PyQt6.QtGui import QIcon

from main_window import MainWindow


def main() -> None:
    app = QApplication(sys.argv)
    app.setApplicationName("TRATON Radar Viewer")
    app.setOrganizationName("TRATON")

    window = MainWindow()
    window.show()

    # Optional: open a file passed as a command-line argument.
    cli_args = [a for a in sys.argv[1:] if not a.startswith("-")]
    if cli_args and Path(cli_args[0]).suffix.lower() == ".mf4":
        # Trigger open via the internal method so the loading thread runs.
        window._open_file_path(cli_args[0])

    sys.exit(app.exec())


if __name__ == "__main__":
    main()
