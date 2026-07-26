#!/usr/bin/env python3
"""
Entry point for HPC Tools Platform

This is a simple entry point that imports and runs the main application
from the main_html package. All application code is in main_html/app.py.

Usage:
  python app.py                    # Run with default settings
  FLASK_ENV=production python app.py  # Run in production mode
  
For development with auto-reload:
  flask run --debug
"""
import sys
import os
from pathlib import Path

# Add main_html to path
MAIN_HTML_PATH = Path(__file__).parent / "main_html"
sys.path.insert(0, str(MAIN_HTML_PATH))

# Import environment utilities first
try:
    from env_utils import get_env, get_cache_dir
    env = get_env()
except ImportError:
    env = None

# Import the Flask app from main_html
try:
    from app import app, db, init_db
except ImportError as e:
    print(f"ERROR: Could not import app from main_html: {e}")
    print(f"Make sure main_html/app.py exists and has required dependencies installed.")
    print(f"Run: pip install -r main_html/requirements.txt")
    sys.exit(1)


def main():
    """Run the application."""
    # Initialize database
    with app.app_context():
        try:
            init_db()
        except Exception as e:
            print(f"Warning: Could not initialize database: {e}")
    
    # Get configuration from environment
    host = os.environ.get('HOST', '0.0.0.0')
    port = int(os.environ.get('PORT', 5002))
    debug = os.environ.get('FLASK_DEBUG', '0') == '1' or os.environ.get('FLASK_ENV') == 'development'
    
    print("=" * 60)
    print("  HPC Tools Platform")
    print("=" * 60)
    if env:
        print(f"Env:   {env.name}")
        print(f"Cache: {env.cache_dir}")
        print(f"DB:    {env.db_path}")
    print(f"Host:  {host}")
    print(f"Port:  {port}")
    print(f"Debug: {debug}")
    print(f"URL:   http://{host}:{port}/html")
    print(f"Hyperlink: http://{host}:{port}/hyperlink/")
    print("=" * 60)
    
    # Run the Flask application
    app.run(host=host, port=port, debug=debug)


if __name__ == '__main__':
    main()

