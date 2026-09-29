"""Hyperlink_tool launcher.

This project is maintained for cloud usage in "online" mode.

Usage:
  python main.py [html_root] [video_root]

Optional env vars:
  LOGVIEW_HOST=0.0.0.0
  LOGVIEW_PORT=5000
  LOGVIEW_NO_BROWSER=1
  HYPERLINK_DATABASE_URL=postgresql://...
"""

import os
import sys

BASE_DIR = os.path.dirname(os.path.abspath(__file__))


def main() -> None:
    sys.path.insert(0, os.path.join(BASE_DIR, 'html_online'))
    from html_online.main import main as online_main

    # Keep supporting the legacy flag; online mode is always used.
    if '--serve' in sys.argv:
        sys.argv.remove('--serve')

    online_main()


if __name__ == "__main__":
    main()
