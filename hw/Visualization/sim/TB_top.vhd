library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity TB_top is 
end TB_top;

architecture BEHAV of TB_top is
    signal tb_clk : std_logic := '0';
    signal tb_rst : std_logic;

    signal tb_hsync : std_logic;
    signal tb_vsync : std_logic;
    signal tb_red : std_logic_vector(3 downto 0);
    signal tb_green : std_logic_vector(3 downto 0);
    signal tb_blue : std_logic_vector(3 downto 0);

begin

    top_inst: entity work.TOP
    generic map (
        g_DELAY => 1
    )
    port map (
        i_sys_clk   => tb_clk,
        i_reset => tb_rst,
        o_hsync => tb_hsync,
        o_vsync => tb_vsync,
        o_red   => tb_red,
        o_green => tb_green,
        o_blue  => tb_blue
    );

    tb_clk <= not tb_clk after 0.5 ns;
    tb_rst <= '1', '0' after 1 ns;

end BEHAV;

