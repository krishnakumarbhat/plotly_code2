import paramiko
import os
import threading
from typing import Optional, Tuple, Dict, Any, Callable

SERVERS = {
    "southfield": "10.192.224.131",
    "krakow": "10.214.45.45"  # Krakow cluster
}

# Global progress tracking
download_progress: Dict[str, Any] = {
    "active": False,
    "total_files": 0,
    "downloaded_files": 0,
    "current_file": "",
    "phase": "",  # "counting", "downloading", "complete", "error"
    "message": "",
    "errors": []
}

def reset_progress():
    global download_progress
    download_progress = {
        "active": False,
        "total_files": 0,
        "downloaded_files": 0,
        "current_file": "",
        "phase": "",
        "message": "",
        "errors": []
    }

def update_progress(phase: str = None, total: int = None, downloaded: int = None, 
                   current_file: str = None, message: str = None, error: str = None):
    global download_progress
    if phase is not None:
        download_progress["phase"] = phase
    if total is not None:
        download_progress["total_files"] = total
    if downloaded is not None:
        download_progress["downloaded_files"] = downloaded
    if current_file is not None:
        download_progress["current_file"] = current_file
    if message is not None:
        download_progress["message"] = message
    if error is not None:
        download_progress["errors"].append(error)

class ClusterConnection:
    def __init__(self):
        self.transport: Optional[paramiko.Transport] = None
        self.sftp: Optional[paramiko.SFTPClient] = None
        self.connected = False
        self.server_name = ""
        self.username = ""
    
    def connect(self, server: str, username: str, password: str) -> Tuple[bool, str]:
        """Connect to cluster via SFTP."""
        try:
            host = SERVERS.get(server.lower())
            if not host:
                return False, f"Unknown server: {server}"
            
            self.transport = paramiko.Transport((host, 22))
            self.transport.connect(username=username, password=password)
            self.sftp = paramiko.SFTPClient.from_transport(self.transport)
            self.connected = True
            self.server_name = server
            self.username = username
            return True, f"Connected to {server} ({host})"
        except Exception as e:
            self.connected = False
            self.username = ""
            return False, str(e)
    
    def disconnect(self):
        """Close SFTP connection."""
        try:
            if self.sftp:
                self.sftp.close()
            if self.transport:
                self.transport.close()
        except Exception:
            pass
        finally:
            self.connected = False
            self.username = ""
            self.sftp = None
            self.transport = None
    
    def list_dir(self, remote_path: str) -> Tuple[bool, list]:
        """List directory contents."""
        if not self.connected or not self.sftp:
            return False, ["Not connected"]
        try:
            items = self.sftp.listdir_attr(remote_path)
            result = []
            for item in items:
                is_dir = item.st_mode and (item.st_mode & 0o40000)
                result.append({
                    "name": item.filename,
                    "is_dir": bool(is_dir),
                    "size": item.st_size
                })
            return True, sorted(result, key=lambda x: (not x['is_dir'], x['name'].lower()))
        except Exception as e:
            return False, [str(e)]
    
    def download_file(self, remote_path: str, local_path: str) -> Tuple[bool, str]:
        """Download a single file."""
        if not self.connected or not self.sftp:
            return False, "Not connected"
        try:
            os.makedirs(os.path.dirname(local_path), exist_ok=True)
            self.sftp.get(remote_path, local_path)
            return True, f"Downloaded: {remote_path}"
        except Exception as e:
            return False, str(e)
    
    def download_directory(self, remote_path: str, local_path: str, callback=None) -> Tuple[bool, str]:
        """Recursively download a directory with progress tracking."""
        if not self.connected or not self.sftp:
            return False, "Not connected"
        
        downloaded = 0
        errors = []
        
        # First, count all files
        update_progress(phase="counting", message=f"Scanning {remote_path}...")
        total_files = self._count_files(remote_path)
        update_progress(total=total_files, phase="downloading", message="Starting download...")
        
        def _download_recursive(remote_dir: str, local_dir: str):
            nonlocal downloaded, errors
            try:
                os.makedirs(local_dir, exist_ok=True)
                items = self.sftp.listdir_attr(remote_dir)
                
                for item in items:
                    remote_item = f"{remote_dir}/{item.filename}"
                    local_item = os.path.join(local_dir, item.filename)
                    
                    is_dir = item.st_mode and (item.st_mode & 0o40000)
                    
                    if is_dir:
                        _download_recursive(remote_item, local_item)
                    else:
                        try:
                            update_progress(current_file=item.filename, 
                                          message=f"Downloading: {item.filename}")
                            self.sftp.get(remote_item, local_item)
                            downloaded += 1
                            update_progress(downloaded=downloaded)
                            if callback:
                                callback(downloaded, item.filename)
                        except Exception as e:
                            error_msg = f"{item.filename}: {e}"
                            errors.append(error_msg)
                            update_progress(error=error_msg)
            except Exception as e:
                error_msg = f"Path error: {remote_dir} - {e}"
                errors.append(error_msg)
                update_progress(error=error_msg)
        
        try:
            _download_recursive(remote_path, local_path)
            if downloaded == 0 and errors:
                update_progress(phase="error", message="; ".join(errors[:3]))
                return False, "; ".join(errors[:3])
            msg = f"Downloaded {downloaded} files"
            if errors:
                msg += f" (with {len(errors)} errors)"
            update_progress(phase="complete", message=msg)
            return True, msg
        except Exception as e:
            update_progress(phase="error", message=str(e))
            return False, str(e)
    
    def _count_files(self, remote_path: str) -> int:
        """Count total files in a remote directory recursively."""
        if not self.connected or not self.sftp:
            return 0
        
        count = 0
        try:
            items = self.sftp.listdir_attr(remote_path)
            for item in items:
                is_dir = item.st_mode and (item.st_mode & 0o40000)
                if is_dir:
                    count += self._count_files(f"{remote_path}/{item.filename}")
                else:
                    count += 1
        except Exception:
            pass
        return count


# Global connection instance
cluster = ClusterConnection()
