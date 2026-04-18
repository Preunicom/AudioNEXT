#!/bin/bash

# @author Markus Remy

VIVADO="vivado"

SCRIPT_PATH="$(realpath "$0")"
SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"

failed_scripts=()
for script in $(find $SCRIPT_DIR/../hw -name "create_vivado_project.tcl"); do
    echo "Running $script"

    $VIVADO -mode batch -source "$script"
    
    if [ $? -ne 0 ]; then
        echo "FAILED: $script"
        failed_scripts+=("$script")
    else
        echo "Successfully created project with $script"
    fi
done
echo "----------------------------------------"
echo "All creation scripts completed!"
if [ ${#failed_scripts[@]} -ne 0 ]; then
    echo "----------------------------------------"
    echo "The following scripts failed:"
    for s in "${failed_scripts[@]}"; do
        echo " - $s"
    done
    echo "Please remove the created project folders of these project from \"../xilinx/vivado\"!"
    exit 1
fi