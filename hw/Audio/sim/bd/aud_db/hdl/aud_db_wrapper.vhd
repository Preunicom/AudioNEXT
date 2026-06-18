--Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
--Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2023.2 (lin64) Build 4029153 Fri Oct 13 20:13:54 MDT 2023
--Date        : Wed Jun 17 12:35:02 2026
--Host        : xuser-pc running 64-bit Ubuntu 22.04.3 LTS
--Command     : generate_target aud_db_wrapper.bd
--Design      : aud_db_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity aud_db_wrapper is
  port (
    ACLK : in STD_LOGIC;
    ARESETN : in STD_LOGIC;
    i_audio_clk : in STD_LOGIC;
    i_data : in STD_LOGIC;
    o_interrupt_sla : out STD_LOGIC;
    o_interrupt_sra : out STD_LOGIC;
    o_lrck : out STD_LOGIC;
    o_mclk : out STD_LOGIC;
    o_sclk : out STD_LOGIC
  );
end aud_db_wrapper;

architecture STRUCTURE of aud_db_wrapper is
  component aud_db is
  port (
    o_mclk : out STD_LOGIC;
    o_lrck : out STD_LOGIC;
    o_sclk : out STD_LOGIC;
    ARESETN : in STD_LOGIC;
    i_data : in STD_LOGIC;
    ACLK : in STD_LOGIC;
    o_interrupt_sla : out STD_LOGIC;
    o_interrupt_sra : out STD_LOGIC;
    i_audio_clk : in STD_LOGIC
  );
  end component aud_db;
begin
aud_db_i: component aud_db
     port map (
      ACLK => ACLK,
      ARESETN => ARESETN,
      i_audio_clk => i_audio_clk,
      i_data => i_data,
      o_interrupt_sla => o_interrupt_sla,
      o_interrupt_sra => o_interrupt_sra,
      o_lrck => o_lrck,
      o_mclk => o_mclk,
      o_sclk => o_sclk
    );
end STRUCTURE;
