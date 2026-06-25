library ieee;
use ieee.std_logic_1164.all;
use std.env.finish;

-- BEGIN NEW Felix Knoll
entity tb_aud_i2s_clk_gen is
end tb_aud_i2s_clk_gen;

architecture Testbench of tb_aud_i2s_clk_gen is

    component aud_i2s_clk_gen is
        port(
            i_mclk   : in  std_logic;
            i_resetn : in  std_logic;
            i_sen    : in  std_logic;
            o_lrck   : out std_logic;
            o_sclk   : out std_logic
        );
    end component;

    constant MCLK_PERIOD : time := 10 ns;

    signal mclk   : std_logic := '0';
    signal resetn : std_logic := '0';
    signal sen    : std_logic := '0';
    signal lrck   : std_logic;
    signal sclk   : std_logic;

    signal tb_test_passed : boolean := false;

begin

    UUT: aud_i2s_clk_gen port map(
        i_mclk   => mclk,
        i_resetn => resetn,
        i_sen    => sen,
        o_lrck   => lrck,
        o_sclk   => sclk
    );

    mclk <= not mclk after MCLK_PERIOD / 2;

    STIMULUS: process
        variable t0 : time;
    begin
        -- reset: beide clocks 0 (2 flanken warten bis reset wirkt)
        wait until rising_edge(mclk);
        wait until rising_edge(mclk);
        assert sclk = '0' and lrck = '0'
            report "Reset check: sclk oder lrck nicht 0"
            severity failure;

        -- sen=0: nichts darf sich bewegen
        resetn <= '1';
        sen    <= '0';
        wait until rising_edge(mclk);
        assert sclk = '0' and lrck = '0'
            report "Hold check: clocks bewegen sich obwohl sen=0"
            severity failure;
        wait until rising_edge(mclk);
        assert sclk = '0' and lrck = '0'
            report "Hold check: clocks bewegen sich obwohl sen=0"
            severity failure;
        wait until rising_edge(mclk);
        assert sclk = '0' and lrck = '0'
            report "Hold check: clocks bewegen sich obwohl sen=0"
            severity failure;

        -- sen=1: SCLK periode = 4 MCLK
        sen <= '1';
        wait until rising_edge(sclk);
        t0 := now;
        wait until rising_edge(sclk);
        assert (now - t0) = 4 * MCLK_PERIOD
            report "SCLK periode falsch: erwartet 40ns"
            severity failure;

        -- LRCK periode = 256 MCLK
        wait until rising_edge(lrck);
        t0 := now;
        wait until rising_edge(lrck);
        assert (now - t0) = 256 * MCLK_PERIOD
            report "LRCK periode falsch: erwartet 2560ns"
            severity failure;

        -- reset im betrieb
        resetn <= '0';
        wait until rising_edge(mclk);
        wait until rising_edge(mclk);
        assert sclk = '0' and lrck = '0'
            report "Reset im betrieb: ausgaenge nicht 0"
            severity failure;

        report "TEST PASSED!" severity note;
        tb_test_passed <= true;
        wait until rising_edge(mclk);
        finish;
    end process;

end Testbench;
-- END NEW Felix Knoll
