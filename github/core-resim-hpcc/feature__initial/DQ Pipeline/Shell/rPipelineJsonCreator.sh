#!/usr/bin/env bash
echo "[INFO] : Starting pipeline json Creator Script Execution"
if [[ $1 == *'/projects/'* ]]; then
    $(xxd -p -r <<<"70 79 74 68 6f 6e 20 2f 6d 6e 74 2f 75 73 6d 69 64 65 74 2f 70 72 6f 6a 65 63 74 73 2f 53 54 4c 41 2d 54 48 55 4e 44 45 52 2f 37 2d 54 6f 6f 6c 73 2f 52 65 53 69 6d 41 75 74 6f 4d 6e 67 2f 53 75 70 70 6f 72 74 2f 50 6f 73 74 50 72 6f 63 65 73 73 69 6e 67 2f 70 69 70 65 6c 69 6e 65 5f 6a 73 6f 6e 43 72 65 61 74 6f 72 2e 70 79") $@
else
    $(xxd -p -r <<<"70 79 74 68 6f 6e 20 2f 6e 65 74 2f 38 6b 33 2f 65 30 66 73 30 31 2f 69 72 6f 64 73 2f 50 4c 4b 52 41 2d 50 52 4f 4a 45 43 54 53 2f 52 4e 41 2d 53 44 56 2d 53 52 52 37 2f 37 2d 54 6f 6f 6c 73 2f 52 65 53 69 6d 41 75 74 6f 4d 6e 67 2f 53 75 70 70 6f 72 74 2f 50 6f 73 74 50 72 6f 63 65 73 73 69 6e 67 2f 70 69 70 65 6c 69 6e 65 5f 6a 73 6f 6e 43 72 65 61 74 6f 72 2e 70 79") $@
fi
echo "[INFO] : Completed pipeline json Creator Script Execution"