# Legible Code Archive

broken

## Setup

1. clone
2. add `lca/scripts` to `$PATH`
3. `lca_root.sh` should give directory of the cloned repo. If it doesn't work, hard-code in `lca/scripts/lca_root.sh`

## How to use

### Include a header file

`#include "header_file_relative_to_code"`

`%:include "header_file_relative_to_lca_root"`

### Replace includes with code

`lca_insert.sh code.cpp` writes the ready-to-submit code to `to_submit.cpp` by default.

`lca_insert.sh code.cpp -o result.cpp`

`lca_insert.sh . --help`

### Find code

`lca_contents.sh` prints all `*.h` files

`lca_find.py pattern` does a fuzzy search for pattern, where similarity ratio is taken from python's `difflib.SequenceMatcher`. Default tolerance is `0.8`.

`lca_find.py --help`
