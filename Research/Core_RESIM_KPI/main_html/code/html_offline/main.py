"""
Log Viewer - Offline Mode
Generates a standalone HTML file with embedded CSS and JavaScript.
"""
import os
import sys
import platform
from typing import Any, Dict, List, Optional

# Base directories
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
OUT_DIR = os.path.join(BASE_DIR, "out")
CWD = os.getcwd()


def is_windows() -> bool:
    """Check if running on Windows"""
    return platform.system().lower() == 'windows'


def resolve_path(path: str, base_dir: str = None) -> str:
    """Resolve a path appropriately for the current platform"""
    if os.path.isabs(path):
        return os.path.abspath(path)
    if base_dir:
        return os.path.abspath(os.path.join(base_dir, path))
    return os.path.abspath(os.path.join(CWD, path))


class LogViewerApp:
    """Core log viewer application - scans and processes log data for offline mode"""
    
    def __init__(self, html_root: str, video_root: str, output_dir: str = None) -> None:
        self.html_root = os.path.abspath(html_root)
        self.video_root = os.path.abspath(video_root)
        self.output_dir = os.path.abspath(output_dir) if output_dir else OUT_DIR

    @staticmethod
    def _key(name: str) -> Optional[str]:
        """Extract key from filename"""
        base = os.path.splitext(os.path.basename(name))[0]
        for suffix in ["_web"]:
            if base.endswith(suffix):
                base = base[:-len(suffix)]
                break
        parts = base.split("_")
        return "_".join(parts[-2:]) if len(parts) >= 2 else None

    def _scan_html(self) -> Dict[str, List[str]]:
        """Scan HTML folders"""
        if not os.path.isdir(self.html_root):
            return {}
        m: Dict[str, List[str]] = {}
        for entry in os.scandir(self.html_root):
            if not entry.is_dir():
                continue
            k = self._key(entry.name)
            if not k:
                continue
            files = []
            for root, _, fnames in os.walk(entry.path):
                for f in fnames:
                    if os.path.splitext(f)[1].lower() in {".html", ".htm"}:
                        abs_path = os.path.join(root, f)
                        # Relative path for offline mode
                        files.append(os.path.relpath(abs_path, self.output_dir).replace("\\", "/"))
            if files:
                m[k] = sorted(files)
        return m

    def _scan_images(self) -> Dict[str, Dict[str, Dict[str, str]]]:
        """Scan image files"""
        if not os.path.isdir(self.html_root):
            return {}
        
        sensor_names = {"range", "velocity", "rangerate", "doppler", "azimuth", "elevation", "snr", "rcs"}
        sensor_positions = {"FL", "FR", "RL", "RR", "FC"}
        img_map: Dict[str, Dict[str, Dict[str, str]]] = {}
        
        for entry in os.scandir(self.html_root):
            if not entry.is_dir():
                continue
            k = self._key(entry.name)
            if not k:
                continue
            
            imgs: Dict[str, Dict[str, str]] = {}
            for root, _, fnames in os.walk(entry.path):
                folder_name = os.path.basename(root).upper()
                for f in fnames:
                    if os.path.splitext(f)[1].lower() != ".png":
                        continue
                    
                    file_base = os.path.splitext(f)[0].lower()
                    sensor_pos = next((pos for pos in sensor_positions if folder_name == pos or pos.lower() in file_base), None)
                    if not sensor_pos:
                        continue
                    
                    sensor_name = next((sn for sn in sensor_names if sn in file_base), "other")
                    if sensor_name not in imgs:
                        imgs[sensor_name] = {}
                    
                    abs_path = os.path.join(root, f)
                    imgs[sensor_name][sensor_pos] = os.path.relpath(abs_path, self.output_dir).replace("\\", "/")
            
            if imgs:
                img_map[k] = imgs
        
        return img_map

    def _scan_text(self) -> Dict[str, Dict[str, str]]:
        """Scan text files"""
        text_dir = os.path.join(self.video_root, "log_txt")
        if not os.path.isdir(text_dir):
            return {}
        
        m: Dict[str, Dict[str, str]] = {}
        for entry in os.scandir(text_dir):
            if not entry.is_file() or os.path.splitext(entry.name)[1].lower() != ".txt":
                continue
            k = self._key(entry.name)
            if not k:
                continue
            try:
                with open(entry.path, "r", encoding="utf-8") as f:
                    content = f.read()
            except Exception:
                content = ""
            
            path = os.path.relpath(entry.path, self.output_dir).replace("\\", "/")
            m[k] = {"path": path, "content": content}
        return m

    def _scan_video(self) -> Dict[str, str]:
        """Scan video files"""
        if not os.path.isdir(self.video_root):
            return {}
        m: Dict[str, str] = {}
        for entry in os.scandir(self.video_root):
            if not entry.is_file() or os.path.splitext(entry.name)[1].lower() not in {".mp4", ".avi", ".mov", ".mkv"}:
                continue
            k = self._key(entry.name)
            if k:
                # File URL for offline mode
                m[k] = f"file:///{os.path.abspath(entry.path).replace(chr(92), '/')}"
        return m

    def _get_html_folder_name(self, key: str) -> str:
        """Get HTML folder name for a key"""
        for entry in os.scandir(self.html_root):
            if entry.is_dir() and self._key(entry.name) == key:
                return entry.name
        return key

    def build_mapping(self) -> Dict[str, Dict[str, Any]]:
        """Build complete mapping of all data for offline mode"""
        html_map = self._scan_html()
        img_map = self._scan_images()
        video_map = self._scan_video()
        text_map = self._scan_text()

        html_keys = set(html_map)
        video_keys = set(video_map)
        matched = sorted(html_keys & video_keys)

        for k in sorted(html_keys - video_keys):
            print(f"No video found for key: {k}")
        for k in sorted(video_keys - html_keys):
            print(f"No HTML folder found for key: {k}")

        mapping: Dict[str, Dict[str, Any]] = {}
        print("Matched keys:")
        for k in matched:
            video_url = video_map[k]
            video_path = video_url.replace("file:///", "")
            video_name = os.path.basename(video_path)
            
            text_info = text_map.get(k)
            if text_info:
                comment_path = text_info["path"]
                comment_content = text_info["content"]
            else:
                base_name = os.path.splitext(video_name)[0]
                comment_path = f"db/video/log_txt/{base_name}.txt"
                comment_content = ""
            
            mapping[k] = {
                "video": video_url,
                "video_name": video_name,
                "html_files": html_map[k],
                "html_folder": self._get_html_folder_name(k),
                "images": img_map.get(k, {}),
                "comment_path": comment_path,
                "comment_content": comment_content,
            }
            print(f"  {k} -> {len(html_map[k])} HTML file(s), video: {video_url}")

        return mapping


def main_offline(html_root: str, video_root: str, output_html: str = None) -> None:
    """Run in offline mode - generates standalone HTML file"""
    from html_build import build_html
    
    html_root = resolve_path(html_root, CWD)
    video_root = resolve_path(video_root, CWD)
    output_html = output_html or os.path.join(CWD, 'out', 'viewer.html')
    output_html = resolve_path(output_html, CWD)
    output_dir = os.path.dirname(output_html)
    
    os.makedirs(output_dir, exist_ok=True)
    
    viewer = LogViewerApp(html_root, video_root, output_dir)
    mapping = viewer.build_mapping()
    
    if not mapping:
        print("No matching HTML/video pairs found.")
        return
    
    build_html(output_html, mapping, serve_mode=False)
    print(f"\nViewer generated. Open {output_html} in a browser.")


def main() -> None:
    """Main entry point for offline mode"""
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    
    if len(args) < 2:
        print("="*60, file=sys.stderr)
        print("Log Viewer - Offline Mode", file=sys.stderr)
        print("="*60, file=sys.stderr)
        print("", file=sys.stderr)
        print("Usage:", file=sys.stderr)
        print("  python main.py <html_root> <video_root> [output_html]", file=sys.stderr)
        print("", file=sys.stderr)
        print("Arguments:", file=sys.stderr)
        print("  html_root    Path to folder containing HTML subfolders", file=sys.stderr)
        print("  video_root   Path to folder containing video files", file=sys.stderr)
        print("  output_html  (Optional) Path for output HTML file", file=sys.stderr)
        print("", file=sys.stderr)
        print("Example:", file=sys.stderr)
        print("  python main.py ../db/html ../db/video output/viewer.html", file=sys.stderr)
        print("", file=sys.stderr)
        print("Note: For online/server mode, use html_online/main.py instead", file=sys.stderr)
        sys.exit(1)
    
    html_root = args[0]
    video_root = args[1]
    output_html = args[2] if len(args) >= 3 else None
    main_offline(html_root, video_root, output_html)


if __name__ == "__main__":
    main()
