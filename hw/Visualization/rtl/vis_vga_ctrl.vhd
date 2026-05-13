--------------------------------------------------------------------------------
----                                                                        ----
---- Copyright (c) 2009, Sandro Amato (adapted)                             ----
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
	use IEEE.STD_LOGIC_ARITH.all;
	use IEEE.STD_LOGIC_UNSIGNED.all;

	use work.vis_vga_pkg.all;

	---- Uncomment the following library declaration if instantiating
	---- any Xilinx primitives in this code.
	--library UNISIM;
	--use UNISIM.VComponents.all;

entity vis_vga_ctrl is
	port (
		-- EDIT CODE BEGIN Markus Remy
		i_vga_clk        : in  std_logic;
		-- EDIT CODE END Markus Remy
		i_reset          : in  std_logic;

		-- USER CODE BEGIN Markus Remy
		i_enable : in std_logic;
		-- USER CODE END Markus Remy

		-- vga horizontal and vertical sync
		o_h_sync         : out std_logic;
		o_v_sync         : out std_logic;

		-- CODE DELETED Markus Remy

		-- blank
		o_blank          : out std_logic;

		-- CODE DELETED Markus Remy
		-- USER CODE BEGIN Markus Remy

		o_visible_frame_done_pulse : out std_logic;

		-- vga R G B signals (4 bit for each component (8 colors))
		o_red            : out std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
		o_green          : out std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
		o_blue           : out std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);

		i_char_clk		 : in std_logic;
		i_char_reset	 : in std_logic;

		i_char_write_en  : in std_logic;
		i_char_address_x : in  std_logic_vector(c_CHR_ADDR_BUS_W_X - 1 downto 0); -- To access 80 chars in one row
		i_char_address_y : in  std_logic_vector(c_CHR_ADDR_BUS_W_Y - 1 downto 0); -- To access 30 chars in one column
		i_char_data      : in  std_logic_vector(c_CHR_ASCII_DATA_BUS_W -1 downto 0); -- Char encoded as ASCII
		i_char_col_red   : in  std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
		i_char_col_green : in  std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
		i_char_col_blue  : in  std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0)
		-- USER CODE END Markus Remy
	);
end entity;

-- vga timings used
--                     0                                    TOT
-- ...-----------------|=============== PERIOD ==============|--...
--                     |                                     |
-- ...__           ___________________________           _______...
--      \_________/                           \_________/
--                                                                 
--      |         |    |               |      |         |    |
--      |         |    |               |      |         |    |
-- ...--|----S----|-F--|=======D=======|==B===|====S====|=F==|--...
--           Y      R          I          A        Y      R
--           N      O          S          C        N      O
--           C      N          P          K        C      N
--           T      T          L          P        T      T
--           I      P          T          O        I      P
--           M      O          I          R        M      O
--           E      R          M          C        E      R
--                  C          E          H               C
--                  H                                     H
--      |         |    |               |      |         |    |
--   ...|---------|----|===============|======|=========|====|--...
-- HPx:      96     16         640        48       96     16    px (h PERIOD = 800 px)
-- VLn:      2      29         480        10       2      29    ln (v PERIOD = 521 ln)
--
-- and with 25Mhz dot clock (40ns dot time):
--      |         |    |               |      |         |    |
--   ...|---------|----|===============|======|=========|====|--...
--Htime:   3.84    0.64       25.6       1.92     3.84   0.64   usec  (h PERIOD = 32 usec) Hfreq 31250 Hz
--Vtime:   64      928       15360       320       64    928    usec  (v PERIOD = 16672 usec) Vfreq 59.98 Hz

architecture rtl of vis_vga_ctrl is

	--
	signal s_h_count      : std_logic_vector(c_H_COUNT_W - 1 downto 0); -- horizontal pixel counter
	signal s_v_count      : std_logic_vector(c_V_COUNT_W - 1 downto 0); -- vertical line counter
	signal s_v_count_d_4  : std_logic_vector(3 downto 0);               -- vertical line counter mod 16 (char height)
	signal s_h_sync       : std_logic;                                  -- horizontal sync trigger
	signal s_h_sync_pulse : std_logic;                                  -- 1-clock pulse on sync trigger

	-- USER CODE BEGIN Markus Remy
	signal s_visible_frame_done_pulse : std_logic;						-- 1-clock pulse after visible frame part
	-- USED CODE END Markus Remy

	--
	-- signals for the charmaps Block ROM component...
	signal s_charmaps_en   : std_logic;
	signal s_charmaps_ADDR : std_logic_vector(c_INTCHMAP_ADDR_BUS_W - 1 downto 0);
	signal s_charmaps_mask : std_logic_vector(c_INTCHMAP_DATA_BUS_W - 1 downto 0);

	--
	-- to manage the outside display region's blanking
	signal s_display : std_logic;
	--

	--
	-- to manage the chars  ram address and the ram ascii
	signal s_chars_ram_addr : std_logic_vector(c_INTCHR_ADDR_BUS_W - 1 downto 0);
	-- EDIT CODE BEGIN Markus Remy
	signal s_chars_ascii    : std_logic_vector(c_CHR_ASCII_DATA_BUS_W - 1 downto 0);
	-- EDIT CODE END Markus Remy
	-- USER CODE BEGIN Markus Remy
	signal s_chars_color 	: std_logic_vector(c_CHR_COLOR_DATA_BUS_W - 1 downto 0);
	signal r_charmaps_color	: std_logic_vector(c_CHR_COLOR_DATA_BUS_W - 1 downto 0); -- Delays signal (for 1 clock cycle to match rom data)
	-- USER CODE END Markus Remy
	signal s_chars_EN_r     : std_logic;

	-- charmaps
	-- |------| |-----------------|
	-- |   P  | | D D D D D D D D |
	-- |======| |=================|
	-- |   8  | | 7 6 5 4 3 2 1 0 |
	-- |======| |=================|
	-- | Free | | Row char pixels |
	-- |------| |-----------------|
	--

	-- USER CODE BEGIN Markus Remy
	signal s_char_address_yx 	: std_logic_vector(c_INTCHR_ADDR_BUS_W - 1 downto 0);
	signal s_char_col_rgb 		: std_logic_vector(c_CHR_COLOR_DATA_BUS_W - 1 downto 0);
	-- USER CODE END Markus Remy

	component vis_charmaps_ROM is
		port (
			i_EN    : in  std_logic;                                            -- RAM Enable Input
			i_clock : in  std_logic;                                            -- Clock
			i_ADDR  : in  std_logic_vector(c_INTCHMAP_ADDR_BUS_W - 1 downto 0); -- 11-bit Address Input
			o_DO    : out std_logic_vector(c_INTCHMAP_DATA_BUS_W - 1 downto 0)  -- 8-bit Data Output
		);
	end component;

	-- CODE EDIT BEGIN Markus Remy
	component vis_RAM is
		generic (
			g_DATA_WIDTH : natural;
			g_ADDR_WIDTH : natural
		);
		port (
			i_clock_w 		: in  std_logic;                                       	-- Write Clock
			i_EN_w		 	: in std_logic;									 		-- Write RAM Enable Input    
			i_write_en_w 	: in  std_logic;                                    	-- Write RAM Write Enable    
			i_ADDR_w  		: in  std_logic_vector(g_ADDR_WIDTH - 1 downto 0);     	-- Write Address Input
			i_DI_w    		: in  std_logic_vector(g_DATA_WIDTH - 1 downto 0);     	-- Write Data Input
	
			i_clock_r  		: in  std_logic;                                      	-- Read Clock
			i_EN_r     		: in  std_logic;										-- Read RAM Enable Input    
			i_ADDR_r   		: in  std_logic_vector(g_ADDR_WIDTH - 1 downto 0);    	-- Read Address Input
			o_DO_r     		: out std_logic_vector(g_DATA_WIDTH - 1 downto 0)     	-- Read Data Output
		);
	end component;

	-- to manage the background and cursor colors
	signal s_color_inverted : std_logic := '0';
	-- CODE EDIT END Markus Remy

begin

	-- enable the ram both
	--   - during the display time   
	s_chars_EN_r <= s_display;

	-- EDIT CODE BEGIN Markus Remy
	-- modify the chars_ram address
	s_chars_ram_addr <= s_v_count(c_V_COUNT_W - 1 downto 4) & s_h_count(c_H_COUNT_W - 1 downto 3);
	-- EDIT CODE END Markus Remy

	-- USER CODE BEGIN Markus Remy
	s_char_col_rgb(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0) <= i_char_col_red;
	s_char_col_rgb((2*c_CHR_COLOR_BIT_DEPTH_W) - 1 downto c_CHR_COLOR_BIT_DEPTH_W) <= i_char_col_green;
	s_char_col_rgb(c_CHR_COLOR_DATA_BUS_W - 1 downto 2*c_CHR_COLOR_BIT_DEPTH_W) <= i_char_col_blue;

	 -- Porch is in address, so we have 800 pixel --> Need one more bit but this is alway zero as we cannot write in the porch
	s_char_address_yx <= i_char_address_y & '0' & i_char_address_x;

	u0_chars_RAM: vis_RAM
		generic map (
			g_DATA_WIDTH => c_CHR_ASCII_DATA_BUS_W,
			g_ADDR_WIDTH => c_INTCHR_ADDR_BUS_W
		)
		port map (
			i_clock_w  		=> i_char_clk,
			i_EN_w     		=> i_char_write_en,
			i_write_en_w 	=> i_char_write_en,
			i_ADDR_w   		=> s_char_address_yx,
			i_DI_w     		=> i_char_data,
			i_clock_r  		=> not i_vga_clk,
			i_EN_r     		=> s_chars_EN_r,
			i_ADDR_r   		=> s_chars_ram_addr,
			o_DO_r     		=> s_chars_ascii
		);
	
	u1_color_RAM: vis_RAM
		generic map (
			g_DATA_WIDTH => c_CHR_COLOR_DATA_BUS_W,
			g_ADDR_WIDTH => c_INTCHR_ADDR_BUS_W
		)
		port map (
			i_clock_w  		=> i_char_clk,
			i_EN_w     		=> i_char_write_en,
			i_write_en_w 	=> i_char_write_en,
			i_ADDR_w   		=> s_char_address_yx,
			i_DI_w     		=> s_char_col_rgb,
			i_clock_r  		=> not i_vga_clk,
			i_EN_r     		=> s_chars_EN_r,
			i_ADDR_r   		=> s_chars_ram_addr,
			o_DO_r     		=> s_chars_color
		);
	-- USER CODE END Markus Remy

	-- modify the charmaps address (each 16 s_v_count - chars are 16 pixel tall)
	--                  v----- ascii code ------v    v-- vert px mod 16 --v (chars are 16 pixel tall)
	--s_charmaps_ADDR <= (s_chars_ascii(6 downto 0) & s_v_count(3 downto 0));
	s_charmaps_ADDR <= (s_chars_ascii & s_v_count_d_4);
	s_charmaps_en   <= '1' when s_h_count(2 downto 0) = "111" -- each 8 h_count (chars are 8 pixel wide)
							else '0';

	u2_charmaps_rom: vis_charmaps_ROM
		port map (
			i_en    => s_charmaps_en,
			i_clock => not i_vga_clk,
			i_ADDR  => s_charmaps_ADDR,
			o_DO    => s_charmaps_mask
		);

	-- USER CODE BEGIN Markus Remy
	-- Delay color to match the data from ROM
	p_delay_color: process(i_vga_clk)
	begin
		if falling_edge(i_vga_clk) then -- falling edge to match the negated clock of ROM
			if i_reset = '1' then
				r_charmaps_color <= (others => '0');
			else
				if s_charmaps_en = '1' then
					r_charmaps_color <= s_chars_color;
				end if;
			end if;
		end if;
	end process;
	-- USER CODE END Markus Remy


	-- generate a single clock pulse on hsync falling -- hsync trigger for p_V_LN_COUNT (tick signal)
	p_pulse_on_hsync_falling: process (i_vga_clk)
		variable v_h_sync1 : std_logic;
	begin
		if rising_edge(i_vga_clk) then
			s_h_sync_pulse <= not s_h_sync and v_h_sync1;
			v_h_sync1 := s_h_sync;
		end if;
	end process;

	-- control the reset, increment and overflow of the horizontal pixel count
	p_H_PX_COUNT: process (i_vga_clk)
	begin
		if rising_edge(i_vga_clk) then
			if i_reset = '1' or s_h_count = c_H_PERIODpx then -- sync reset
				s_h_count <= (others => '0');
			else
				s_h_count <= s_h_count + 1;
			end if;
		end if;
	end process;

	-- control the reset, increment and overflow of the vertical pixel count
	p_V_LN_COUNT: process (i_vga_clk)
	begin
		if rising_edge(i_vga_clk) then
			if i_reset = '1' or s_v_count = c_V_PERIODln then -- sync reset
				s_v_count <= (others => '0');
				s_v_count_d_4 <= s_v_count(3 downto 0);
			elsif s_h_sync_pulse = '1' then
				s_v_count <= s_v_count + 1;
				s_v_count_d_4 <= s_v_count(3 downto 0);
			end if;
		end if;
	end process;

	-- set the horizontal sync high time and low time according to the constants
	p_MGM_H_SYNC: process (i_vga_clk)
	begin
		if rising_edge(i_vga_clk) then
			if (s_h_count = c_H_DISPLAYpx + c_H_BACKPORCHpx) then
				s_h_sync <= '0';
			elsif (s_h_count = c_H_PERIODpx - c_H_FRONTPORCHpx) then
				s_h_sync <= '1';
			end if;
		end if;
	end process;
	-- CODE EDIT BEGIN Markus Remy
	o_h_sync <= s_h_sync;
	-- CODE EDIT END Markus Remy

	-- set the vertical sync high time and low time according to the constants
	p_MGM_V_SYNC: process (i_vga_clk)
	begin
		--if falling_edge(i_vga_clk) then
		if rising_edge(i_vga_clk) then
			-- CODE EDIT BEGIN Markus Remy
			if (s_v_count = (c_V_DISPLAYln + c_V_BACKPORCHln)) then
			-- CODE EDIT END Markus Remy
				o_v_sync <= '0';
			elsif (s_v_count = (c_V_PERIODln - c_V_FRONTPORCHln)) then --and (s_h_sync_pulse = '1') then
				o_v_sync <= '1';
			end if;
		end if;
	end process;

	-- asserts the blanking signal (active low)
	p_MGM_BLANK: process (i_vga_clk)
	begin
		if rising_edge(i_vga_clk) then
			-- if we are outside the visible range on the screen then tell the RAMDAC to blank
			-- in this section by putting s_display low
			if not (s_h_count < c_H_DISPLAYpx and s_v_count < c_V_DISPLAYln) then
				s_display <= '0';
			else
				s_display <= '1';
			end if;
		end if;
	end process;

	-- USED CODE BEGIN Markus Remy
	p_MGM_FRAME_END: process(i_vga_clk)
	begin
		if rising_edge(i_vga_clk) then
			-- First pixel after the last visible pixel of the frame -> visible frame done
			if s_h_count = c_H_DISPLAYpx and s_v_count = c_V_DISPLAYln - 1 then
				s_visible_frame_done_pulse <= '1';
			else
				s_visible_frame_done_pulse <= '0';
			end if;
		end if;
	end process;
	-- USER CODE END Markus Remy

	-- generates the r g b signals showing chars
	p_MGM_RGB: process (i_vga_clk)
	begin
		if rising_edge(i_vga_clk) then -- not async reset
			if i_reset = '1' then -- sync reset
				-- CODE EDIT BEGIN Markus Remy
				o_red <= (others => '0');
				o_green <= (others => '0');
				o_blue <= (others => '0');
				-- CODE EDIT END Markus Remy
			else
				if s_display = '1' and i_enable = '1' then -- display zone and enabled
					-- USER CODE BEGIN Markus Remy
					-- Map 1 in ascii pixel map to foreground color and 0 to background color
					if s_charmaps_mask(conv_integer(not s_h_count(2 downto 0))) = '1' then
						if s_color_inverted = '0' then
							o_red <= r_charmaps_color(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
							o_green <= r_charmaps_color((2*c_CHR_COLOR_BIT_DEPTH_W) - 1 downto c_CHR_COLOR_BIT_DEPTH_W);
							o_blue <= r_charmaps_color(c_CHR_COLOR_DATA_BUS_W - 1 downto 2*c_CHR_COLOR_BIT_DEPTH_W);
						else
							o_red <= not r_charmaps_color(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
							o_green <= not r_charmaps_color((2*c_CHR_COLOR_BIT_DEPTH_W) - 1 downto c_CHR_COLOR_BIT_DEPTH_W);
							o_blue <= not r_charmaps_color(c_CHR_COLOR_DATA_BUS_W - 1 downto 2*c_CHR_COLOR_BIT_DEPTH_W);
						end if;
					else
						if s_color_inverted = '0' then
							o_red <= (others => '0');
							o_green <= (others => '0');
							o_blue <= (others => '0');
						else
							o_red <= (others => '1');
							o_green <= (others => '1');
							o_blue <= (others => '1');
						end if;
					end if;
					-- USER CODE END Markus Remy
				else -- blank zone
				-- CODE EDIT BEGIN Markus Remy
					-- the blanking zone or disabled output
					o_red <= (others => '0');
					o_green <= (others => '0');
					o_blue <= (others => '0');
				-- CODE EDIT END Markus Remy
				end if; -- if s_display        
			end if; -- if i_reset
		end if; -- if rising_edge(i_vga_clk)
	end process;

	o_blank <= not s_display;
	o_visible_frame_done_pulse <= s_visible_frame_done_pulse;

end architecture;