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

entity aud_i2s_sync is
  port ( 
    i_mclk    : in std_logic;
    i_resetn  : in std_logic;
    --
    i_data    : in std_logic;
    --
    o_data_sync : out std_logic
  );
end aud_i2s_sync;

architecture Behavioral of aud_i2s_sync is
  signal data_meta : std_logic;
  signal data_sync : std_logic;

begin 
  
ps_sync: process (i_mclk) is
begin
  if rising_edge(i_mclk) then
   --reset
    if i_resetn ='0' then
      data_meta <= '0';
      data_sync <= '0';
    else
      data_meta <= i_data;
      data_sync <= data_meta;
    end if;
  end if;
end process;    

o_data_sync <= data_sync;
--end Maximilian Hafeneder

end Behavioral;