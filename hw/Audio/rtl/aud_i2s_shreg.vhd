library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

--begin Maximilian Hafeneder

entity aud_i2s_shreg is
  port ( 
    i_mclk: in std_logic;
    i_resetn: in std_logic;
    --
    i_data_sync: in std_logic;
    i_sclk_sync: in std_logic;
    i_lrck_sync: in std_logic;
    i_sen: in std_logic;
    --
    o_valid_l: out std_logic;
    o_valid_r: out std_logic;
    --
    o_data_l: out std_logic_vector(23 downto 0);
    o_data_r: out std_logic_vector(23 downto 0)
  );
end aud_i2s_shreg;

architecture Behavioral of aud_i2s_shreg is
  signal r_sync : std_logic_vector(2 downto 0);

begin 
  
ps_shreg: process (i_mclk) is
begin
  if rising_edge(i_mclk) then
   --reset
    if i_reset ='1' then
      r_sync <= (others => '0');
    else
      r_sync <= r_sync(1 downto 0) & i_data;
    end if;
  end if;
end process;    


o_data_sync <= r_sync(2);
--end Maximilian Hafeneder

end Behavioral;