#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'EOF'
Usage: cleanup_memory.sh <user>

Environment flags:
  DRY_RUN=true|false            Print actions without killing (default: true)
  KEEP_LOGIN_SERVICES=true|false  Preserve main_html / gunicorn / broker (default: true)
  KILL_ALL=true|false           Kill all user processes except preserved services (default: false)
EOF
}

if [[ $# -lt 1 ]]; then
    usage >&2
    exit 1
fi

TARGET_USER="$1"
DRY_RUN="${DRY_RUN:-true}"
KEEP_LOGIN_SERVICES="${KEEP_LOGIN_SERVICES:-true}"
KILL_ALL="${KILL_ALL:-false}"

is_truthy() {
    case "${1,,}" in
        1|true|yes|y|on) return 0 ;;
        *) return 1 ;;
    esac
}

preserve_command() {
    local cmd="$1"
    if ! is_truthy "$KEEP_LOGIN_SERVICES"; then
        return 1
    fi
    [[ "$cmd" =~ sshd ]] && return 0
    [[ "$cmd" =~ gunicorn ]] && return 0
    [[ "$cmd" =~ main_html\.simg ]] && return 0
    [[ "$cmd" =~ main_hpcc\.sh ]] && return 0
    [[ "$cmd" =~ hpcc_main\.pyz ]] && return 0
    [[ "$cmd" =~ main_html/app\.py ]] && return 0
    return 1
}

killable_command() {
    local cmd="$1"
    if is_truthy "$KILL_ALL"; then
        preserve_command "$cmd" && return 1
        return 0
    fi
    [[ "$cmd" =~ apptainer.*(udp_kpi|can_kpi|interactiveplot|rag|main_html)\.simg ]] && return 0
    [[ "$cmd" =~ singularity.*(udp_kpi|can_kpi|interactiveplot|rag|main_html)\.simg ]] && return 0
    [[ "$cmd" =~ UDP_KPI\.kpi_server ]] && return 0
    [[ "$cmd" =~ ResimHTMLReport\.py ]] && return 0
    [[ "$cmd" =~ kpi_runtime_launcher\.sh ]] && return 0
    [[ "$cmd" =~ run_(udp|can|intplot|rag)\.sh ]] && return 0
    [[ "$cmd" =~ inplot_(udp|can)\.sh ]] && return 0
    [[ "$cmd" =~ tmux ]] && return 0
    return 1
}

current_pid="$$"
parent_pid="${PPID:-0}"

echo "cleanup_memory user=$TARGET_USER dry_run=$DRY_RUN keep_login_services=$KEEP_LOGIN_SERVICES kill_all=$KILL_ALL"

while read -r pid cmd; do
    [[ -n "$pid" ]] || continue
    if [[ "$pid" == "$current_pid" || "$pid" == "$parent_pid" ]]; then
        continue
    fi
    if ! killable_command "$cmd"; then
        continue
    fi
    echo "candidate pid=$pid cmd=$cmd"
    if is_truthy "$DRY_RUN"; then
        continue
    fi
    kill "$pid" >/dev/null 2>&1 || true
done < <(ps -u "$TARGET_USER" -o pid=,args= 2>/dev/null)

if ! is_truthy "$DRY_RUN"; then
    sleep 2
    while read -r pid cmd; do
        [[ -n "$pid" ]] || continue
        if [[ "$pid" == "$current_pid" || "$pid" == "$parent_pid" ]]; then
            continue
        fi
        if ! killable_command "$cmd"; then
            continue
        fi
        kill -9 "$pid" >/dev/null 2>&1 || true
    done < <(ps -u "$TARGET_USER" -o pid=,args= 2>/dev/null)
fi

if command -v tmux >/dev/null 2>&1 && [[ "$(id -un 2>/dev/null || true)" == "$TARGET_USER" ]]; then
    while read -r session_name _; do
        [[ -n "$session_name" ]] || continue
        echo "tmux session=$session_name"
        if ! is_truthy "$DRY_RUN"; then
            tmux kill-session -t "$session_name" >/dev/null 2>&1 || true
        fi
    done < <(tmux list-sessions 2>/dev/null || true)
fi