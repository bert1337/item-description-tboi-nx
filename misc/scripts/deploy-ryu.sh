#!/bin/bash
set -e

# Verify the path is set.
if [ -z "${RYU_PATH}" ]; then
    echo "RYU_PATH appears to not be set! Check your exlaunch.sh?"
fi

# Copy over files.
cp ${OUT}/* /Users/marcobertolaso/Library/Application\ Support/Ryujinx/mods/contents/010021c000b6a000/exefs