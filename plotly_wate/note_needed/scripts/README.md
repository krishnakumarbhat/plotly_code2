cleanup_memory.sh - safe nightly cleanup helper

Usage:

  DRY_RUN=true ./cleanup_memory.sh ouymc2

To schedule in cron (example - runs nightly at midnight and performs kills):

  0 0 * * * DRY_RUN=false /abs/path/to/all_services/scripts/cleanup_memory.sh ouymc2 >> /var/log/cleanup_memory_ouymc2.log 2>&1

The script is conservative: it keeps processes matching common website/server patterns (gunicorn/nginx/sshd) and only kills others owned by the specified user.
