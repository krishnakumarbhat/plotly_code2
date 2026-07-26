from __future__ import annotations

import argparse
import warnings
from typing import Optional

from app.main import Application

warnings.filterwarnings(
    "ignore",
    message=r"The 'validate_default' attribute with value True was provided to the `Field\(\)` function.*",
)


def main(argv: Optional[list[str]] = None) -> None:
    parser = argparse.ArgumentParser(description="RAG HTML ingestor and Flask talk server")
    parser.add_argument("--scrap", "-s", dest="scrap_path", help="Path to HTML root to scrape")
    parser.add_argument("--talk", "-t", action="store_true", help="Run Flask talk server (LLM RAG)")
    parser.add_argument("--embed", "-e", action="store_true", help="When scraping, also create embeddings/vectors using local models")
    parser.add_argument(
        "--reset-index",
        action="store_true",
        help="Reset vector index + ingestion/query logs before scraping",
    )
    args = parser.parse_args(argv)

    if not args.scrap_path and not args.talk:
        parser.print_help()
        return

    # If scrap provided, run ingestion and exit (unless --talk also set)
    app = Application(html_root_override=args.scrap_path) if args.scrap_path else Application()

    if args.scrap_path:
        if args.reset_index:
            app.rag_engine.reset_vector_store()
            app.sqlite_store.clear_ingestions()
            app.sqlite_store.clear_queries()
            print({"status": "index reset completed"})
        result = app.ingestor.ingest(run_vector=args.embed)
        print(result)

    if args.talk:
        print(f"Starting Flask server on {app.config.host}:{app.config.port} (talk mode)")
        app.run()


if __name__ == "__main__":
    main()
