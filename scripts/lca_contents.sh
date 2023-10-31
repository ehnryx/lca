#!/bin/bash

lca_root="$(lca_root.sh)"

echo "contents"
echo "--------"
for f in $(find "${lca_root}" -name "*.h"); do
  echo $(realpath -s --relative-to "${lca_root}" "$f");
done
