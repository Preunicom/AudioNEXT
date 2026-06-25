library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use std.env.finish;

-- BEGIN NEW Felix Knoll
entity tb_aud_i2s_all is
end tb_aud_i2s_all;

architecture Testbench of tb_aud_i2s_all is

    component aud_i2s_sampling is
        port(
            i_mclk   : in  std_logic;
            i_sclk   : in  std_logic;
            i_lrck   : in  std_logic;
            i_resetn : in  std_logic;
            i_sen    : in  std_logic;
            i_data   : in  std_logic;
            o_valid_l : out std_logic;
            o_valid_r : out std_logic;
            o_data_l  : out std_logic_vector(23 downto 0);
            o_data_r  : out std_logic_vector(23 downto 0)
        );
    end component;

    component aud_i2s_clk_gen is
        port ( 
        i_mclk : in std_logic;
        i_resetn : in std_logic;
        i_sen : in std_logic;

        o_lrck : out std_logic;
        o_sclk : out std_logic

        );
    end component;

    constant MCLK_PERIOD : time := 10 ns;

    signal mclk    : std_logic := '0';
    signal resetn  : std_logic := '0';
    signal data_in : std_logic := '0';
    signal sclk    : std_logic;
    signal lrck    : std_logic;
    signal sen     : std_logic := '0';

    signal valid_l : std_logic;
    signal valid_r : std_logic;
    signal data_l  : std_logic_vector(23 downto 0);
    signal data_r  : std_logic_vector(23 downto 0);

    signal tb_test_passed : boolean := false;

    -- wie shreg, aber i_data laeuft durch 2-FF sync
    -- darum datum 2 takte vor der SCLK-flanke anlegen
    procedure send_frame(
        constant c_data_l : in  std_logic_vector(23 downto 0);
        constant c_data_r : in  std_logic_vector(23 downto 0);
        signal   s_data   : out std_logic
    ) is
    begin
        wait until falling_edge(sclk);
        for i in 23 downto 0 loop
            s_data <= c_data_l(i);
            wait until falling_edge(sclk);
        end loop;
        for i in 7 downto 0 loop
            s_data <= '0';
            wait until falling_edge(sclk);
        end loop;

        -- rechter kanal

        for i in 23 downto 0 loop
            s_data <= c_data_r(i);
            wait until falling_edge(sclk);
        end loop;
        for i in 7 downto 0 loop
            s_data <= '0';
            if i /= 0 then
                wait until falling_edge(sclk);
            end if;
        end loop;
    end procedure;

begin

    HELPER: aud_i2s_clk_gen port map(
        i_mclk => mclk,
        i_resetn => resetn,
        i_sen => sen,


        o_lrck => lrck,
        o_sclk => sclk
    );

    UUT: aud_i2s_sampling port map(
        i_mclk    => mclk,
        i_sclk    => sclk,
        i_lrck    => lrck,
        i_resetn  => resetn,
        i_sen     => sen,
        i_data    => data_in,
        o_valid_l => valid_l,
        o_valid_r => valid_r,
        o_data_l  => data_l,
        o_data_r  => data_r
    );

    mclk <= not mclk after MCLK_PERIOD / 2;

    STIMULUS: process
    begin
        resetn <= '0';
        wait until rising_edge(mclk);
        resetn <= '1';
        sen    <= '1';

        wait until rising_edge(lrck);
        wait until rising_edge(sclk);

        -- frame 1: rechts 0, links alles 1
        send_frame(X"000000", X"FFFFFF", data_in);
        -- frame 2: nur MSB / nur LSB
        send_frame(X"800000", X"000001", data_in);

        wait;
    end process;

    CHECK: process
    begin
        wait until valid_r = '1';
        assert data_r = X"000000"
            report "Frame 1 rechts: daten falsch"
            severity failure;

        wait until valid_l = '1';
        assert data_l = X"FFFFFF"
            report "Frame 1 links: daten falsch"
            severity failure;

        wait until valid_r = '1';
        assert data_r = X"800000"
            report "Frame 2 rechts: MSB-only fehlgeschlagen"
            severity failure;

        wait until valid_l = '1';
        assert data_l = X"000001"
            report "Frame 2 links: LSB-only fehlgeschlagen"
            severity failure;

        report "TEST PASSED!" severity note;
        tb_test_passed <= true;
        wait until rising_edge(mclk);
        finish;
    end process;

end Testbench;
-- END NEW Felix Knoll
