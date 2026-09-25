"""Gunicorn configuration for the HPCC login-node UI."""
import os
import multiprocessing

# Server socket
bind = f"{os.environ.get('HOST', '0.0.0.0')}:{os.environ.get('PORT', '5002')}"
backlog = int(os.environ.get('BACKLOG', '1024'))

# Fewer worker processes keep RSS bounded on the shared 15 GB login node.
default_workers = min(max(multiprocessing.cpu_count() // 2, 2), 3)
workers = int(os.environ.get('WORKERS', str(default_workers)))

# Use threaded workers for long-lived Hyperlink/video requests without forking too
# many copies of the Flask app.
worker_class = 'gthread'
threads = int(os.environ.get('THREADS', '8'))

timeout = int(os.environ.get('TIMEOUT', '240'))
graceful_timeout = int(os.environ.get('GRACEFUL_TIMEOUT', '30'))
keepalive = int(os.environ.get('KEEPALIVE', '120'))

# Recycle workers more aggressively so long-running shared sessions do not keep
# growing memory indefinitely.
max_requests = int(os.environ.get('MAX_REQUESTS', '400'))
max_requests_jitter = int(os.environ.get('MAX_REQUESTS_JITTER', '40'))

# Logging
accesslog = '-'
errorlog = '-'
loglevel = os.environ.get('LOG_LEVEL', 'info')

# Security
limit_request_line = 8190
limit_request_fields = 100

# Server mechanics
daemon = False
pidfile = None
tmp_upload_dir = None
worker_tmp_dir = '/dev/shm' if os.path.isdir('/dev/shm') else None

# Handle SIGTERM gracefully
def on_exit(server):
    """Clean up resources on server exit."""
    pass

# Worker lifecycle hooks
def worker_exit(server, worker):
    """Called when a worker exits - cleanup any lingering resources."""
    pass

def pre_fork(server, worker):
    """Called just before a worker is forked."""
    pass

def post_fork(server, worker):
    """Called just after a worker has been forked."""
    pass
