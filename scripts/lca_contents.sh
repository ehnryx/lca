#!/bin/bash

lca_root="$(lca_root.sh)"

echo "contents"
echo "--------"
find "${lca_root}" -name "*.h" -exec \
  realpath -s --relative-to "${lca_root}" {} \;
