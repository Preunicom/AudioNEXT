library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;

entity Example is
    port (
        i_clk : in std_logic;
        i_a : in std_logic;
        o_b : out std_logic
    );
end entity;

architecture Behavioral of Example is
begin

    process(i_clk) is
    begin
        if rising_edge(i_clk) then
            o_b <= i_a;
        end if;
    end process;

end architecture;
