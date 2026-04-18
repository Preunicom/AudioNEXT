# Created with template for Xilinx projects.

This template provides a simple project structure for projects combining Vivado and Vitis.
It can also be used for Vivado only projects by deleting the `sw/` folder.

## Vivado project structure
- Vivado project created by the given script in `xilinx/vivado`
- Add sources/sim/constraints as external to Vivado from the corresponding folder in `hw/`.
- Create block designs in `hw/bd/`
- The default IP location is `hw/ip` and automatically set when creating the project by the script.

## Vitis project structure
- Vitis workspace has to be created in `sw/`.
- No external sources are required in vitis.

## Vivado testbenches format

To use the CI workflow and the test script all testbenches must follow the same guidelines.
- Testbench files have to use the format `TB_*.vhd`.
- Use self-checking testbenches.
- Use a boolean `tb_test_passed` signal which is `false` by default and gets set to `true` after the "TEST PASSED" report at the end of the test. 
- One rising edge after setting `tb_test_passed` to `true`, `finish` has to be called.

An example testbench is given in `examples/TB_Example.vhd`.