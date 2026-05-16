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
end aud_i2s_shreg;

architecture Behavioral of aud_i2s_shreg is
  signal sclk_prev  : std_logic;
  signal lrck_prev  : std_logic;
  signal r_shreg    : std_logic_vector(23 downto 0);
  signal r_bitcnt   : unsigned(4 downto 0);
  signal r_data_l   : std_logic_vector(23 downto 0);
  signal r_data_r   : std_logic_vector(23 downto 0);
  signal r_valid_l  : std_logic;
  signal r_valid_r  : std_logic;
  signal r_skip     : std_logic;

begin 
  
ps_shreg: process (i_mclk) is
begin
  if rising_edge(i_mclk) then
   --reset
    if i_resetn ='0' then
      sclk_prev <= '0';
      lrck_prev <= '0';
      r_shreg <= (others => '0');
      r_bitcnt <= (others => '0');
      r_data_l <= (others => '0');
      r_data_r <= (others => '0');
      r_valid_l <= '0';
      r_valid_r <= '0';
      r_skip <= '0';
    else
      sclk_prev <= i_sclk;
      lrck_prev <= i_lrck;

      r_valid_l <= '0';
      r_valid_r <= '0';

      if (i_lrck = '0' and lrck_prev = '1') or
        (i_lrck = '1' and lrck_prev = '0') then
        r_bitcnt <= (others => '0');
        r_skip <= '1';
      end if;
        
      if i_sclk = '0' and sclk_prev = '1' then
        if r_skip = '1' then
          r_skip <= '0';
        elsif i_sen = '1' then
          --Bit einlesen
          r_shreg <= r_shreg(22 downto 0) & i_data_sync;
          r_bitcnt <= r_bitcnt + 1;
        
          --Register voll
          if r_bitcnt = to_unsigned(23, r_bitcnt'length) then
            if lrck_prev = '0' then
              r_data_l <= r_shreg(22 downto 0) & i_data_sync;
              r_valid_l <= '1';
            else
              r_data_r <= r_shreg(22 downto 0) & i_data_sync;
              r_valid_r <= '1';
            end if;
            r_bitcnt <= (others => '0');
          end if;
        end if;
      end if;
    end if;
  end if;
end process;    

o_data_l <= r_data_l;
o_data_r <= r_data_r;
o_valid_l <= r_valid_l;
o_valid_r <= r_valid_r;
--end Maximilian Hafeneder

end Behavioral;