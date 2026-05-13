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

entity vis_core is
  port (
        -- USER CODE BEGIN Markus Remy
        i_clk                       : in std_logic;
        i_rst                       : in std_logic;
        -- Frame buffer signals
        i_buf_valid                 : in std_logic;
        i_buf_addr_x                : in std_logic_vector(c_CHR_ADDR_BUS_W_X - 1 downto 0);
        i_buf_addr_y                : in std_logic_vector(c_CHR_ADDR_BUS_W_Y - 1 downto 0);
        i_buf_char_ascii            : in std_logic_vector(c_CHR_ASCII_DATA_BUS_W - 1 downto 0);
        i_buf_color_red             : in std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
        i_buf_color_green           : in std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
        i_buf_color_blue            : in std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
        o_buf_ready                 : out std_logic;
        -- VGA signals
        i_vga_enable                : in std_logic;
        o_visible_frame_done_pulse  : out std_logic;
        o_hsync                     : out std_logic;
        o_vsync                     : out std_logic;
        o_red                       : out std_logic_vector (c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
        o_green                     : out std_logic_vector (c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
        o_blue                      : out std_logic_vector (c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0)   
        -- USER CODE END Markus Remy
        );
end vis_core;

architecture Behavioral of vis_core is
	-- USER CODE END Markus Remy
  component VGA_CLK
    port (
      i_clk     : in  std_logic;
      o_vga_clk : out std_logic;
      reset     : in  std_logic;
      locked    : out std_logic
    );
  end component;
  -- USER CODE END Markus Remy
  -- CODE EDIT BEGIN Markus Remy
  component vis_vga_ctrl is
    port (
      i_vga_clk                   : in  std_logic;
      i_reset                     : in  std_logic;
      i_enable                    : in std_logic;
      o_h_sync                    : out std_logic;
      o_v_sync                    : out std_logic;
      o_blank                     : out std_logic;
      o_visible_frame_done_pulse  : out std_logic;
      o_red                       : out std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
      o_green                     : out std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
      o_blue                      : out std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
      i_char_clk		              : in  std_logic;
      i_char_reset	              : in  std_logic;
      i_char_write_en             : in  std_logic;
      i_char_address_x            : in  std_logic_vector(c_CHR_ADDR_BUS_W_X - 1 downto 0);
      i_char_address_y            : in  std_logic_vector(c_CHR_ADDR_BUS_W_Y - 1  downto 0);
      i_char_data                 : in  std_logic_vector(c_CHR_ASCII_DATA_BUS_W - 1 downto 0);
      i_char_col_red              : in  std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
      i_char_col_green            : in  std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
      i_char_col_blue             : in  std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0)
    );
  end component;

  signal s_h_sync : std_logic;
  signal s_v_sync : std_logic;
  signal s_red : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
  signal s_green : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
  signal s_blue : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
  
  signal s_buf_ready : std_logic;
  signal s_buf_write_en : std_logic;
  -- CODE EDIT END Markus Remy
  -- USER CODE BEGIN Markus Remy
  -- Clock Generation
  signal s_vga_clk : std_logic;
  -- CDC
  -- IN 2 VGA
  signal s_cdc_vga_rst : std_logic;
  signal s_vga_rst : std_logic;
  signal s_cdc_vga_en : std_logic;
  signal s_vga_en : std_logic;
  -- VGA 2 IN
  signal s_cdc_blank : std_logic;
  signal s_blank : std_logic;
  signal s_visible_frame_done_pulse_vga_clk : std_logic;
  signal s_cdc_visible_frame_done_pulse : std_logic;
  signal s_visible_frame_done_pulse : std_logic;
  -- USER CODE END Markus Remy
begin
  -- USER CODE BEGIN Markus Remy
  VGA_CLK_INST: VGA_CLK
  port map (
    i_clk     => i_clk,
    o_vga_clk => s_vga_clk,
    reset     => i_rst,
    locked    => open
  );
  -- USER CODE END Markus Remy

  o_hsync <= s_h_sync;
  o_vsync <= s_v_sync;
  -- EDIT CODE BEGIN Markus Remy
  o_red <= s_red;
  o_green <= s_green;
  o_blue <= s_blue;

  u1_vga_ctrl: vis_vga_ctrl
  port map (
    i_vga_clk                   => s_vga_clk,
    i_reset                     => s_vga_rst,
    i_enable                    => s_vga_en,
    o_h_sync                    => s_h_sync,
    o_v_sync                    => s_v_sync,
    o_blank                     => s_blank,
    o_red                       => s_red,
    o_green                     => s_green,
    o_blue                      => s_blue,
    o_visible_frame_done_pulse  => s_visible_frame_done_pulse_vga_clk,
    i_char_clk                  => i_clk,
    i_char_reset                => i_rst,
    i_char_write_en             => s_buf_write_en,
    i_char_address_x            => i_buf_addr_x,
    i_char_address_y            => i_buf_addr_y,
    i_char_data                 => i_buf_char_ascii,
    i_char_col_red              => i_buf_color_red,
    i_char_col_green            => i_buf_color_green,
    i_char_col_blue             => i_buf_color_blue
  );  
  -- EDIT CODE END Markus Remy
  -- USER CODE BEGIN Markus Remy

  -- Clock domain crossing to sync outputs to char clk
	CDC_VGA_2_IN: process(i_clk)
	begin
		if rising_edge(i_clk) then
      s_cdc_blank <= s_blank;
      s_buf_ready <= s_cdc_blank;
      s_cdc_visible_frame_done_pulse <= s_visible_frame_done_pulse_vga_clk;
      s_visible_frame_done_pulse <= s_cdc_visible_frame_done_pulse;
		end if;
	end process;

  CDC_IN_2_VGA: process(s_vga_clk)
	begin
		if rising_edge(s_vga_clk) then
      s_cdc_vga_rst <= i_rst;
      s_vga_rst <= s_cdc_vga_rst;
      s_cdc_vga_en <= i_vga_enable;
      s_vga_en <= s_cdc_vga_en;
		end if;
	end process;

  o_buf_ready <= s_buf_ready;
  s_buf_write_en <= i_buf_valid and s_buf_ready;
  o_visible_frame_done_pulse <= s_visible_frame_done_pulse;
  -- USER CODE END Markus Remy

end Behavioral;