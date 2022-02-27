#!/bin/bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" >& /dev/null && pwd)
echo "searching in $SCRIPT_DIR for $1"
echo "------------"

grep -r --include "*\.h" $1 $SCRIPT_DIR \
  | grep ": *struct" \
  | sed 's/: *\([^ ]* *[^ ]*\).*/: \1/g'
