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
    i_sclk    : in std_logic;
    i_lrck    : in std_logic;
    i_data    : in std_logic;
    --
    o_sclk_sync : out std_logic;
    o_lrck_sync : out std_logic;
    o_data_sync : out std_logic
  );
end aud_i2s_sync;

architecture Behavioral of aud_i2s_sync is
  signal sclk_meta : std_logic;
  signal sclk_sync : std_logic;
  signal lrck_meta : std_logic;
  signal lrck_sync : std_logic;
  signal data_meta : std_logic;
  signal data_sync : std_logic;

begin 
  
ps_sync: process (i_mclk) is
begin
  if rising_edge(i_mclk) then
   --reset
    if i_resetn ='0' then
      sclk_meta <= '0';
      sclk_sync <= '0';
      lrck_meta <= '0';
      lrck_sync <= '0';
      data_meta <= '0';
      data_sync <= '0';
    else
      sclk_meta <= i_sclk;
      sclk_sync <= sclk_meta;
      lrck_meta <= i_lrck;
      lrck_sync <= lrck_meta;
      data_meta <= i_data;
      data_sync <= data_meta;
    end if;
  end if;
end process;    

o_sclk_sync <= sclk_sync;
o_lrck_sync <= lrck_sync;
o_data_sync <= data_sync;
--end Maximilian Hafeneder

end Behavioral;