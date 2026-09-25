#!/bin/bash
# GDB wrapper for VS Code cppdbg.
# Disables debuginfod before GDB starts its MI session so the
# interactive "Enable debuginfod? (y/n)" prompt never appears and
# never breaks the MI pipe.
export DEBUGINFOD_URLS=""
exec /usr/bin/gdb -iex "set debuginfod enabled off" "$@"
