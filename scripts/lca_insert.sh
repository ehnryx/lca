#!/bin/bash

lca_root=$(lca_root.sh)
echo "Using PATH_TO_LCA='${lca_root}'"
python3 "${lca_root}/scripts/lca_insert.py" "${1/.cpp}.cpp" --lca "${lca_root}" ${@:2}
