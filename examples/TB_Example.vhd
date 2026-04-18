library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use std.env.finish;

-- This is an example Testbench for getting used to the testbench format of the template.
entity TB_Example is
end TB_Example;

architecture Testbench of TB_Example is
    component Example is
        port (
            i_clk : in std_logic;
            i_a : in std_logic;
            o_b : out std_logic
        );
    end component;
    signal clk : std_logic := '0';
    signal a : std_logic;
    signal b : std_logic;

    -- The tb_test_passed signal as boolean (Must exist)
    signal tb_test_passed : boolean := false;    
begin
    -- Your test implemented as needed
    UUT: Example
    port map(
        i_clk => clk,
        i_a => a,
        o_b => b
    );

    clk <= not clk after 10 ns;

    STIMULUS: process
    begin
        a <= '1';
        wait until rising_edge(clk);
        a <= '0';
        wait;
    end process;

    CHECK: process
    begin
        wait until rising_edge(clk);
        -- Assert statements with severity failure to abort the testbench in case of an error (to have a self checking testbench which can be run by TCL and evaluated via the terminal)
        assert b = '1'
            report "Wrong signal value!"
                    & " Exp.: " & std_logic'image('1')
                    & " Got: " & std_logic'image(a)
            severity failure;
        wait until rising_edge(clk);
        assert b = '0'
            report "Wrong signal value!"
                    & " Exp.: " & std_logic'image('0')
                    & " Got: " & std_logic'image(a)
            severity failure;
        wait until rising_edge(clk);
        -- Test passed output, tb_test_passed signal set to true and finish call at the end of the testbench (Must exist)
        -- Warning: Make sure a finish call or an assert failure is terminating the testbench in all cases! Otherwise, the test runs forever!
        report "TEST PASSED!" severity note;
        tb_test_passed <= true;
        wait until rising_edge(clk);
        finish;
    end process;

end Testbench;