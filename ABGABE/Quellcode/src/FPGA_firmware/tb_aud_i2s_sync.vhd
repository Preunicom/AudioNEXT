library ieee;
use ieee.std_logic_1164.all;
use std.env.finish;

-- BEGIN NEW Felix Knoll
entity tb_aud_i2s_sync is
end tb_aud_i2s_sync;

architecture Testbench of tb_aud_i2s_sync is

    component aud_i2s_sync is
        port(
            i_mclk      : in  std_logic;
            i_resetn    : in  std_logic;
            i_data      : in  std_logic;
            o_data_sync : out std_logic
        );
    end component;

    constant MCLK_PERIOD : time := 10 ns;

    signal mclk     : std_logic := '0';
    signal resetn   : std_logic := '0';
    signal data_in  : std_logic := '0';
    signal data_out : std_logic;

    signal tb_test_passed : boolean := false;

begin

    UUT: aud_i2s_sync port map(
        i_mclk      => mclk,
        i_resetn    => resetn,
        i_data      => data_in,
        o_data_sync => data_out
    );

    mclk <= not mclk after MCLK_PERIOD / 2;

    STIMULUS: process
    begin
        -- reset: ausgang muss 0 sein
        wait until rising_edge(mclk);
        wait until rising_edge(mclk);
        assert data_out = '0'
            report "Reset check failed: o_data_sync not '0'"
            severity failure;

        -- '1' anlegen, 2 FFs durchlaufen
        resetn  <= '1';
        data_in <= '1';
        wait until rising_edge(mclk);
        wait until rising_edge(mclk);
        wait until rising_edge(mclk);
        assert data_out = '1'
            report "Sync zu '1' fehlgeschlagen"
            severity failure;

        -- zurueck auf '0'
        data_in <= '0';
        wait until rising_edge(mclk);
        wait until rising_edge(mclk);
        wait until rising_edge(mclk);
        assert data_out = '0'
            report "Sync zu '0' fehlgeschlagen"
            severity failure;

        -- reset im betrieb: ausgang wieder 0
        resetn  <= '0';
        data_in <= '1';
        wait until rising_edge(mclk);
        wait until rising_edge(mclk);
        assert data_out = '0'
            report "Reset ueberschreibt nicht: erwartet '0'"
            severity failure;

        report "TEST PASSED!" severity note;
        tb_test_passed <= true;
        wait until rising_edge(mclk);
        finish;
    end process;

end Testbench;
-- END NEW Felix Knoll
