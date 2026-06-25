library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use std.env.finish;

-- BEGIN NEW Felix Knoll
entity tb_aud_i2s_shreg is
end tb_aud_i2s_shreg;

architecture Testbench of tb_aud_i2s_shreg is

    component aud_i2s_shreg is
        port(
            i_mclk      : in  std_logic;
            i_resetn    : in  std_logic;
            i_data_sync : in  std_logic;
            i_sclk      : in  std_logic;
            i_lrck      : in  std_logic;
            i_sen       : in  std_logic;
            o_valid_l   : out std_logic;
            o_valid_r   : out std_logic;
            o_data_l    : out std_logic_vector(23 downto 0);
            o_data_r    : out std_logic_vector(23 downto 0)
        );
    end component;

    constant MCLK_PERIOD : time := 10 ns;

    signal mclk      : std_logic := '0';
    signal resetn    : std_logic := '0';
    signal data_sync : std_logic := '0';
    signal sclk      : std_logic := '0';
    signal lrck      : std_logic := '1';
    signal sen       : std_logic := '0';

    signal valid_l   : std_logic;
    signal valid_r   : std_logic;
    signal data_l    : std_logic_vector(23 downto 0);
    signal data_r    : std_logic_vector(23 downto 0);

    signal tb_test_passed : boolean := false;

    -- schickt einen I2S frame: erst links, dann rechts
    procedure send_frame(
        constant c_data_l : in  std_logic_vector(23 downto 0);
        constant c_data_r : in  std_logic_vector(23 downto 0);
        signal   s_mclk   : in  std_logic;
        signal   s_sclk   : out std_logic;
        signal   s_lrck   : out std_logic;
        signal   s_data   : out std_logic
    ) is
    begin
        -- links: LRCK fallende flanke
        s_lrck <= '0';
        wait until rising_edge(s_mclk);
        -- erstes SCLK wird uebersprungen (I2S delay bit)
        s_sclk <= '1';
        wait until rising_edge(s_mclk);
        s_sclk <= '0';
        wait until rising_edge(s_mclk);

        -- 24 bits, MSB zuerst
        for i in 23 downto 0 loop
            s_data <= c_data_l(i);
            wait until rising_edge(s_mclk);
            s_sclk <= '1';
            wait until rising_edge(s_mclk); -- DUT sampelt hier
            s_sclk <= '0';
            wait until rising_edge(s_mclk);
        end loop;

        -- rechts: LRCK steigende flanke
        s_lrck <= '1';
        wait until rising_edge(s_mclk);
        s_sclk <= '1';
        wait until rising_edge(s_mclk);
        s_sclk <= '0';
        wait until rising_edge(s_mclk);

        for i in 23 downto 0 loop
            s_data <= c_data_r(i);
            wait until rising_edge(s_mclk);
            s_sclk <= '1';
            wait until rising_edge(s_mclk);
            s_sclk <= '0';
            wait until rising_edge(s_mclk);
        end loop;
    end procedure;

begin

    UUT: aud_i2s_shreg port map(
        i_mclk      => mclk,
        i_resetn    => resetn,
        i_data_sync => data_sync,
        i_sclk      => sclk,
        i_lrck      => lrck,
        i_sen       => sen,
        o_valid_l   => valid_l,
        o_valid_r   => valid_r,
        o_data_l    => data_l,
        o_data_r    => data_r
    );

    mclk <= not mclk after MCLK_PERIOD / 2;

    STIMULUS: process
    begin
        -- reset, lrck auf 1 damit lrck_prev sauber startet
        resetn <= '0';
        lrck   <= '1';
        wait until rising_edge(mclk);
        wait until rising_edge(mclk);
        resetn <= '1';
        sen    <= '1';
        wait until rising_edge(mclk);

        -- frame 1: links 0, rechts alles 1
        send_frame(X"000000", X"FFFFFF", mclk, sclk, lrck, data_sync);
        -- frame 2: nur MSB / nur LSB
        send_frame(X"800000", X"000001", mclk, sclk, lrck, data_sync);

        wait;
    end process;

    CHECK: process
    begin
        wait until valid_l = '1';
        assert data_l = X"000000"
            report "Frame 1 links: daten falsch"
            severity failure;

        wait until valid_r = '1';
        assert data_r = X"FFFFFF"
            report "Frame 1 rechts: daten falsch"
            severity failure;

        wait until valid_l = '1';
        assert data_l = X"800000"
            report "Frame 2 links: MSB-only fehlgeschlagen"
            severity failure;

        wait until valid_r = '1';
        assert data_r = X"000001"
            report "Frame 2 rechts: LSB-only fehlgeschlagen"
            severity failure;

        report "TEST PASSED!" severity note;
        tb_test_passed <= true;
        wait until rising_edge(mclk);
        finish;
    end process;

end Testbench;
-- END NEW Felix Knoll
