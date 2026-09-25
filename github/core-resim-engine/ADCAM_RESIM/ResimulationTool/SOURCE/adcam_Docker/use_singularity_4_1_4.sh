#!/bin/sh

# Source this file in a shell session to prefer the local SingularityCE 4.1.4
# install that was placed under $HOME/.local.

SINGULARITYCE_4_1_4_ROOT="$HOME/.local/singularityce-4.1.4"

if [ -d "$SINGULARITYCE_4_1_4_ROOT/bin" ]; then
    export PATH="$SINGULARITYCE_4_1_4_ROOT/bin:$PATH"
fi
