import os
from pathlib import Path
from typing import List


def _find_kpi_files(base_output: Path, base_name: str) -> List[Path]:
    candidates: List[Path] = []
    root = base_output / base_name if (base_output / base_name).exists() else base_output
    if not root.exists():
        return []
    for sensor_dir in root.iterdir():
        if sensor_dir.is_dir():
            for html in sensor_dir.glob(f"{base_name}_*_kpi.html"):
                candidates.append(html)
    return sorted(candidates)


def generate_kpi_index(output_dir: str, base_name: str) -> str:
    out_path = Path(output_dir)
    files = _find_kpi_files(out_path, base_name)

    index_root = out_path / base_name if (out_path / base_name).exists() else out_path
    index_root.mkdir(parents=True, exist_ok=True)
    index_file = index_root / f"{base_name}_kpi.html"

    buttons = []
    for f in files:
        sensor = f.parent.name
        label = f"{sensor}: {f.stem.replace(base_name + '_', '')}"
        rel = os.path.relpath(f, index_root)
        buttons.append(f'<a class="btn" href="{rel}" target="_blank">{label}</a>')

    html = f"""
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>{base_name} KPI Index</title>
  <style>
    body {{ font-family: Arial, sans-serif; margin: 24px; background: #f7f9fc; }}
    h1 {{ color: #2c3e50; }}
    .grid {{ display: grid; grid-template-columns: repeat(auto-fill, minmax(280px, 1fr)); gap: 16px; margin-top: 20px; }}
    .btn {{
      display: inline-block; padding: 14px 16px; background: #4f46e5; color: #fff; text-decoration: none;
      border-radius: 8px; box-shadow: 0 4px 10px rgba(0,0,0,0.08); font-weight: 600; letter-spacing: .2px;
      transition: transform .05s ease, box-shadow .2s ease, background .2s ease;
    }}
    .btn:hover {{ transform: translateY(-1px); background: #4338ca; box-shadow: 0 6px 14px rgba(0,0,0,0.12); }}
    .empty {{ color: #6b7280; margin-top: 12px; }}
  </style>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;600&display=swap" rel="stylesheet">
  <style> body {{ font-family: 'Inter', Arial, sans-serif; }} </style>
</head>
<body>
  <h1>{base_name} KPI Reports</h1>
  <div class="grid">
    {''.join(buttons) if buttons else '<div class="empty">No KPI HTML files found.</div>'}
  </div>
</body>
</html>
"""

    with open(index_file, "w", encoding="utf-8") as fp:
        fp.write(html)

    return str(index_file)


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Aggregate KPI HTML links into an index page")
    parser.add_argument("--output-dir", required=True, help="Output directory root")
    parser.add_argument("--base-name", required=True, help="Base name for this run")
    args = parser.parse_args()

    path = generate_kpi_index(args.output_dir, args.base_name)
    print(path)
