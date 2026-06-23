#!/bin/bash

# @author Markus Remy

#export VITIS_SETTINGS=/home/user/Xilinx/Vitis/2023.2/settings64.sh
#
#export VITIS_XSA=/home/user/1work/AudioNEXT/xilinx/vivado/AudioNEXT/AudioNEXT.xsa
#./scripts/create_all_vitis_platforms.sh


# Fail if errors or undefined variables
set -euo pipefail

script_dir="$(dirname "$(realpath "$0")")"

fail=0
# Checks if Vitis environment is set
if ! command -v vitis >/dev/null 2>&1; then
    echo "ERROR: vitis not found on PATH after sourcing settings64.sh." >&2
    exit 2
fi

echo "Vitis found!"

# Searches all python vitis platform creation scripts in scripts and runs them
while IFS= read -r -d '' s; do
    echo "Running $s"
    vitis -s "$s" || { echo "FAILED: $s"; fail=1; }
done < <(find "$script_dir" -name create_vitis_project.py -print0)

exit $fail