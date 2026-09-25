"""
Log Viewer Application Launcher
Routes to the appropriate mode (offline or online).

Usage:
  Online mode:  python main.py --serve [html_root] [video_root]
  Offline mode: python main.py <html_root> <video_root> [output_html]
  VLM mode:     python main.py --vlm [video_root] [--force]

For standalone usage:
  - Offline: cd html_offline && python main.py <html_root> <video_root>
  - Online:  cd html_online && python main.py [html_root] [video_root]
"""
import os
import sys

BASE_DIR = os.path.dirname(os.path.abspath(__file__))


def main() -> None:
    """Main entry point - routes to offline or online mode"""
    serve_mode = '--serve' in sys.argv
    vlm_mode = '--vlm' in sys.argv
    
    if serve_mode or vlm_mode:
        # Online mode or VLM mode - use html_online module
        sys.path.insert(0, os.path.join(BASE_DIR, 'html_online'))
        from html_online.main import main as online_main
        
        # Remove --serve flag for online main (it's the default there)
        if '--serve' in sys.argv:
            sys.argv.remove('--serve')
        
        online_main()
    else:
        # Offline mode - use html_offline module
        sys.path.insert(0, os.path.join(BASE_DIR, 'html_offline'))
        from html_offline.main import main as offline_main
        offline_main()


if __name__ == "__main__":
    main()
