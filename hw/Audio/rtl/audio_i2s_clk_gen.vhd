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
entity audio_i2s_clk_gen is
  port ( 
    i_mclk : in std_logic;
    i_reset : in std_logic;
    i_sen : in std_logic;
    
    o_lrck : out std_logic;
    o_sclk : out std_logic

  );
end audio_i2s_clk_gen;


architecture Behavioral of audio_i2s_clk_gen is
  signal r_cntsclk  : std_logic;
  signal r_cntlrck : unsigned(6 downto 0);
  signal r_sclk     : std_logic;
  signal r_lrck     : std_logic;

begin

ps_sclk: process (i_mclk) is
begin
  if (i_mclk='1' and i_mclk'event) then
    --reset
    if i_reset ='1' then
      r_cntsclk <= '0';
      r_sclk <= '0';
    else      
      --sampling enabled
      if i_sen='1' then
       --toggle on threshold  
        if r_cntsclk='1' then             
          r_sclk <= not r_sclk;  
          r_cntsclk <= '0'; 
        --increment counter
        else
          r_cntsclk <= not r_cntsclk;
        end if;
        --sampling disabled
      else
        --do nothing (idle, halt)                     
      end if;
    end if;      
  end if;
end process;

ps_lrck: process (i_mclk) is
begin
  if (i_mclk='1' and i_mclk'event) then
    --reset
    if i_reset ='1' then
      r_cntlrck <= (others=>'0');
      r_lrck <= '0';
    else      
      --sampling enabled
      if i_sen='1' then
          --toggle on threshold  
          if r_cntlrck="1111111" then
            r_lrck <= not r_lrck;
            r_cntlrck <= (others=>'0'); 
            --increment counter
          else
            r_cntlrck <= r_cntlrck+1;
          end if;
        --sampling disabled
       else
         --do nothing (idle, halt)
       end if;
    end if;      
  end if;
end process;

      
o_sclk <= r_sclk;
o_lrck <= r_lrck;
--end Maximilian Hafeneder
      
end Behavioral;