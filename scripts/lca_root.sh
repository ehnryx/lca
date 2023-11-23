#!/bin/bash

scripts_dir=$(dirname "$(readlink -f "$0")")
LCA_ROOT=$(realpath "${scripts_dir}/../")
export LCA_ROOT
echo "${LCA_ROOT}"
