--Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
--Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2023.2 (lin64) Build 4029153 Fri Oct 13 20:13:54 MDT 2023
--Date        : Wed Jun 17 19:02:24 2026
--Host        : 0e66597dd674 running 64-bit Ubuntu 22.04.5 LTS
--Command     : generate_target vip_db_wrapper.bd
--Design      : vip_db_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity vip_db_wrapper is
  port (
    ACLK : in STD_LOGIC;
    ARESETN : in STD_LOGIC;
    i_pixel_clk_0 : in STD_LOGIC;
    o_blue_0 : out STD_LOGIC_VECTOR ( 3 downto 0 );
    o_green_0 : out STD_LOGIC_VECTOR ( 3 downto 0 );
    o_hsync_0 : out STD_LOGIC;
    o_interrupt_0 : out STD_LOGIC;
    o_red_0 : out STD_LOGIC_VECTOR ( 3 downto 0 );
    o_vsync_0 : out STD_LOGIC
  );
end vip_db_wrapper;

architecture STRUCTURE of vip_db_wrapper is
  component vip_db is
  port (
    ACLK : in STD_LOGIC;
    ARESETN : in STD_LOGIC;
    i_pixel_clk_0 : in STD_LOGIC;
    o_interrupt_0 : out STD_LOGIC;
    o_red_0 : out STD_LOGIC_VECTOR ( 3 downto 0 );
    o_green_0 : out STD_LOGIC_VECTOR ( 3 downto 0 );
    o_blue_0 : out STD_LOGIC_VECTOR ( 3 downto 0 );
    o_hsync_0 : out STD_LOGIC;
    o_vsync_0 : out STD_LOGIC
  );
  end component vip_db;
begin
vip_db_i: component vip_db
     port map (
      ACLK => ACLK,
      ARESETN => ARESETN,
      i_pixel_clk_0 => i_pixel_clk_0,
      o_blue_0(3 downto 0) => o_blue_0(3 downto 0),
      o_green_0(3 downto 0) => o_green_0(3 downto 0),
      o_hsync_0 => o_hsync_0,
      o_interrupt_0 => o_interrupt_0,
      o_red_0(3 downto 0) => o_red_0(3 downto 0),
      o_vsync_0 => o_vsync_0
    );
end STRUCTURE;
