--------------------------------------------------------------------------------
----                                                                        ----
---- This file is part of the yaVGA project                                 ----
---- http://www.opencores.org/?do=project&who=yavga                         ----
----                                                                        ----
---- Description                                                            ----
---- Implementation of yaVGA IP core                                        ----
----                                                                        ----
---- To Do:                                                                 ----
----                                                                        ----
----                                                                        ----
---- Author(s):                                                             ----
---- Sandro Amato, sdroamt@netscape.net   (adapted)                                  ----
----                                                                        ----
--------------------------------------------------------------------------------
----                                                                        ----
---- Copyright (c) 2009, Sandro Amato                                       ----
---- All rights reserved.                                                   ----
----                                                                        ----
---- Redistribution  and  use in  source  and binary forms, with or without ----
---- modification,  are  permitted  provided that  the following conditions ----
---- are met:                                                               ----
----                                                                        ----
----     * Redistributions  of  source  code  must  retain the above        ----
----       copyright   notice,  this  list  of  conditions  and  the        ----
----       following disclaimer.                                            ----
----     * Redistributions  in  binary form must reproduce the above        ----
----       copyright   notice,  this  list  of  conditions  and  the        ----
----       following  disclaimer in  the documentation and/or  other        ----
----       materials provided with the distribution.                        ----
----     * Neither  the  name  of  SANDRO AMATO nor the names of its        ----
----       contributors may be used to  endorse or  promote products        ----
----       derived from this software without specific prior written        ----
----       permission.                                                      ----
----                                                                        ----
---- THIS SOFTWARE IS PROVIDED  BY THE COPYRIGHT  HOLDERS AND  CONTRIBUTORS ----
---- "AS IS"  AND  ANY EXPRESS OR  IMPLIED  WARRANTIES, INCLUDING,  BUT NOT ----
---- LIMITED  TO, THE  IMPLIED  WARRANTIES  OF MERCHANTABILITY  AND FITNESS ----
---- FOR  A PARTICULAR  PURPOSE  ARE  DISCLAIMED. IN  NO  EVENT  SHALL  THE ----
---- COPYRIGHT  OWNER  OR CONTRIBUTORS  BE LIABLE FOR ANY DIRECT, INDIRECT, ----
---- INCIDENTAL,  SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, ----
---- BUT  NOT LIMITED  TO,  PROCUREMENT OF  SUBSTITUTE  GOODS  OR SERVICES; ----
---- LOSS  OF  USE,  DATA,  OR PROFITS;  OR  BUSINESS INTERRUPTION) HOWEVER ----
---- CAUSED  AND  ON  ANY THEORY  OF LIABILITY, WHETHER IN CONTRACT, STRICT ----
---- LIABILITY,  OR  TORT  (INCLUDING  NEGLIGENCE  OR OTHERWISE) ARISING IN ----
---- ANY  WAY OUT  OF THE  USE  OF  THIS  SOFTWARE,  EVEN IF ADVISED OF THE ----
---- POSSIBILITY OF SUCH DAMAGE.                                            ----
--------------------------------------------------------------------------------

library IEEE;
	use IEEE.STD_LOGIC_1164.all;
	use IEEE.NUMERIC_STD.all;

	use work.vis_vga_pkg.all;

	-- Uncomment the following lines to use the declarations that are
	-- provided for instantiating Xilinx primitive components.
	--library UNISIM;
	--use UNISIM.VComponents.all;

entity vis_RAM is
	-- USER CODE BEGIN Markus Remy
  	generic (
		g_DATA_WIDTH : natural;
		g_ADDR_WIDTH : natural
	);
	-- USER CODE END Markus Remy
	-- CODE EDIT BEGIN Markus Remy
	port (
		i_clock_w : in  std_logic;                                       -- Write Clock
		i_EN_w    : in  std_logic;                                       -- Write RAM Enable Input    
		i_ADDR_w  : in  std_logic_vector(g_ADDR_WIDTH - 1 downto 0);     -- Write Address Input
		i_DI_w    : in  std_logic_vector(g_DATA_WIDTH - 1 downto 0);     -- Write Data Input

		i_sync_rst: in std_logic;								 	 	 -- Synchronous RAM Reset

		i_clock_r  : in  std_logic;                                      -- Read Clock
		i_EN_r     : in  std_logic;										 -- Read RAM address Enable Input    
		i_ADDR_r   : in  std_logic_vector(g_ADDR_WIDTH - 1 downto 0);    -- Read Address Input
		o_DO_r     : out std_logic_vector(g_DATA_WIDTH - 1 downto 0)     -- Read Data Output
	);
	-- CODE EDIT END Markus Remy
end entity;

architecture Behavioral of vis_RAM is

	constant c_ram_size : natural := 2 ** (g_ADDR_WIDTH);

	-- CODE EDIT BEGIN Markus Remy
	type t_ram is array (c_ram_size - 1 downto 0) of std_logic_vector(g_DATA_WIDTH - 1 downto 0);

	signal v_ram0 : t_ram := (others => (others=>'0'));
	signal s0_DO_r : std_logic_vector(g_DATA_WIDTH - 1 downto 0);

	-- Infer BRAM instead of distributed RAM
	attribute ram_style : string;
	attribute ram_style of v_ram0 : signal is "block";
	-- CODE EDIT END Markus Remy
begin

	-- CODE EDIT BEGIN Markus Remy
	p_w0_port: process (i_clock_w)
	begin
		if rising_edge(i_clock_w) then
			-- No reset as it would not infer BRAM in this case
			if i_EN_w = '1' then
				v_ram0(to_integer(unsigned((i_ADDR_w)))) <= i_DI_w;
			end if;
		end if;
	end process;

	p_ro0_port: process (i_clock_r)
	begin
		if rising_edge(i_clock_r) then
			if i_sync_rst = '1' then
				s0_DO_r <= (others => '0');
			elsif (i_EN_r = '1') then
				s0_DO_r <= v_ram0(to_integer(unsigned(i_ADDR_r)));
			end if;
		end if;
	end process;
	-- CODE EDIT END Markus Remy

	o_DO_r <= s0_DO_r;

end architecture;
