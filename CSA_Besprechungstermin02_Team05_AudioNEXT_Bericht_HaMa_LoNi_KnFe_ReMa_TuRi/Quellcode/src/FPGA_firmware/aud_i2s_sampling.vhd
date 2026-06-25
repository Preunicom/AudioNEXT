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

entity aud_i2s_sampling is
  port ( 
    i_mclk: in std_logic;
    i_sclk: in std_logic;
    i_lrck: in std_logic;
    i_resetn: in std_logic;
    --
    i_sen: in std_logic;
    i_data: in std_logic;
    --
    o_valid_l: out std_logic;
    o_valid_r: out std_logic;
    --
    o_data_l: out std_logic_vector(23 downto 0);
    o_data_r: out std_logic_vector(23 downto 0)
  );
end aud_i2s_sampling;

architecture Behavioral of aud_i2s_sampling is

  component aud_i2s_sync is
    port(
      i_mclk    : in std_logic;
      i_resetn  : in std_logic;
      --
      i_data    : in std_logic;
      --
      o_data_sync : out std_logic
     );
  end component;
  
  component aud_i2s_shreg is
    port(
      i_mclk: in std_logic;
      i_resetn: in std_logic;
      --
      i_data_sync: in std_logic;
      i_sclk: in std_logic;
      i_lrck: in std_logic;
      i_sen: in std_logic;
      --
      o_valid_l: out std_logic;
      o_valid_r: out std_logic;
      --
      o_data_l: out std_logic_vector(23 downto 0);
      o_data_r: out std_logic_vector(23 downto 0)
     );
  end component;  
  
  signal w_data_sync : std_logic;

begin
  
  aud_i2s_sync_inst: aud_i2s_sync port map(
    i_mclk => i_mclk,
    i_resetn => i_resetn,
    i_data => i_data,
    o_data_sync => w_data_sync
  );
    
  aud_i2s_shreg_inst: aud_i2s_shreg port map(
    i_mclk => i_mclk,
    i_resetn => i_resetn,
    i_data_sync => w_data_sync,
    i_sclk => i_sclk,
    i_lrck => i_lrck,
    i_sen => i_sen,
    o_valid_l => o_valid_l,
    o_valid_r => o_valid_r,
    o_data_l => o_data_l,
    o_data_r => o_data_r
  );
    
--end Maximilian Hafeneder

end Behavioral;