#!/bin/bash

scripts_dir=$(dirname "$(readlink -f "$0")")
export LCA_ROOT=$(realpath "${scripts_dir}/../")
echo "${LCA_ROOT}"
