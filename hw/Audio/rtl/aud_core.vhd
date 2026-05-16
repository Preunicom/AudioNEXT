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

entity aud_core is
  port ( 
    i_mclk: in std_logic;
    i_resetn: in std_logic;
    --
    i_sen: in std_logic;
    i_data: in std_logic;
    --
    o_sclk: out std_logic;
    o_lrck: out std_logic;
    --
    o_valid_l: out std_logic;
    o_valid_r: out std_logic;
    --
    o_data_l: out std_logic_vector(23 downto 0);
    o_data_r: out std_logic_vector(23 downto 0)
  );
end aud_core;

architecture Behavioral of aud_core is

  component aud_i2s_clk_gen is
    port(
      i_mclk : in std_logic;
      i_resetn : in std_logic;
      i_sen : in std_logic;
    
      o_lrck : out std_logic;
      o_sclk : out std_logic
    );
  end component;
  
  component aud_i2s_sampling is
    port(
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
  end component;  
  
  signal w_lrck : std_logic;
  signal w_sclk : std_logic;

begin
  
  aud_i2s_clk_gen_inst: aud_i2s_clk_gen port map(
    i_mclk => i_mclk,
    i_resetn => i_resetn,
    i_sen => i_sen,
    o_lrck => w_lrck,
    o_sclk => w_sclk
  );
    
  aud_i2s_sampling_inst: aud_i2s_sampling port map(
    i_mclk => i_mclk,
    i_resetn => i_resetn,
    i_sclk => w_sclk,
    i_lrck => w_lrck,
    i_sen => i_sen,
    i_data => i_data,
    o_valid_l => o_valid_l,
    o_valid_r => o_valid_r,
    o_data_l => o_data_l,
    o_data_r => o_data_r
  );
    
  o_sclk <= w_sclk;
  o_lrck <= w_lrck;
--end Maximilian Hafeneder

end Behavioral;