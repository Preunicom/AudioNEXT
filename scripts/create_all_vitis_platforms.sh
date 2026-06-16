#!/bin/bash
#export VITIS_SETTINGS=/home/user/Xilinx/Vitis/2023.2/settings64.sh
#
#export VITIS_XSA=/home/user/1work/AudioNEXT/xilinx/vivado/AudioNEXT/AudioNEXT.xsa
#./scripts/create_all_vitis_platforms.sh

set -euo pipefail

DIR="$(dirname "$(realpath "$0")")"
fail=0

if [[ -n "${VITIS_SETTINGS:-}" ]]; then
    # shellcheck disable=SC1090
    source "$VITIS_SETTINGS"
elif [[ -n "${VITIS_HOME:-}" ]]; then
    # shellcheck disable=SC1090
    source "$VITIS_HOME/settings64.sh"
elif [[ -n "${XILINX_VITIS:-}" ]]; then
    # shellcheck disable=SC1090
    source "$XILINX_VITIS/settings64.sh"
else
    echo "ERROR: Set VITIS_SETTINGS (path to settings64.sh), VITIS_HOME, or XILINX_VITIS." >&2
    exit 2
fi

if ! command -v vitis >/dev/null 2>&1; then
    echo "ERROR: vitis not found on PATH after sourcing settings64.sh." >&2
    exit 2
fi

while IFS= read -r -d '' s; do
    echo "Running $s"
    vitis -s "$s" || { echo "FAILED: $s"; fail=1; }
done < <(find "$DIR/../hw" -name create_vitis_platform.py -print0)

exit $fail
