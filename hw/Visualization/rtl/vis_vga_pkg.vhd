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
---- Sandro Amato, sdroamt@netscape.net                                     ----
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

package vis_vga_pkg is

-- Declare constants

  -- chars address and data bus size

  -- USER CODE BEGIN Markus Remy
  constant c_CHR_ADDR_BUS_W_X : integer := 7;
  constant c_CHR_ADDR_BUS_W_Y : integer := 5;
  constant c_CHR_ASCII_DATA_BUS_W : integer := 7;

  constant c_CHR_COLOR_BIT_DEPTH_W : integer := 4;
  constant c_CHR_COLOR_DATA_BUS_W : integer := 3 * c_CHR_COLOR_BIT_DEPTH_W;
  -- USER CODE END Markus Remy 

  
  -- internal used chars address and data bus size 
  -- 800x521 pixel (with non visible pixels) --> ceil(ln2(c_H_PERIODpx / 8)) + ceil(ln2(c_V_PERIODln / 16)) = 7 + 6 = 13
  constant c_INTCHR_ADDR_BUS_W : integer := 13;
  constant c_INTCHR_DATA_BUS_W : integer := 8;

  -- CODE EDIT BEGIN Markus Remy
  -- internal ROM chmaps address and data bus
  constant c_INTCHMAP_ADDR_BUS_W : integer := c_CHR_ASCII_DATA_BUS_W + 4; -- 8x16 chars --> Additional 4 bits for the line
  -- CODE EDIT END Markus Remy
  constant c_INTCHMAP_DATA_BUS_W : integer := 8;

  --
  -- horizontal timing signals (in pixels count )
  constant c_H_DISPLAYpx    : integer := 640; 
  constant c_H_BACKPORCHpx  : integer := 48; 
  constant c_H_SYNCTIMEpx   : integer := 96;
  constant c_H_FRONTPORCHpx : integer := 16;
  constant c_H_PERIODpx     : integer := c_H_DISPLAYpx +
                                         c_H_BACKPORCHpx +
                                         c_H_SYNCTIMEpx +
                                         c_H_FRONTPORCHpx; -- 800
  -- CODE EDIT BEGIN Markus Remy
  constant c_H_COUNT_W : integer := 10;       -- = ceil(ln2(c_H_PERIODpx)) = 10
  -- CODE EDIT END Markus Remy

  --
  -- vertical timing signals (in lines count)
  constant c_V_DISPLAYln    : integer := 480;
  constant c_V_BACKPORCHln  : integer := 10;
  constant c_V_SYNCTIMEln   : integer := 2;
  constant c_V_FRONTPORCHln : integer := 29;
  constant c_V_PERIODln     : integer := c_V_DISPLAYln +
                                         c_V_BACKPORCHln +
                                         c_V_SYNCTIMEln +
                                         c_V_FRONTPORCHln; -- 521
  constant c_V_COUNT_W : integer := 10;  -- = ceil(ln2(c_V_PERIODln)) = 10

--  constant c_CHARS_WIDTH: std_logic_vector(2 downto 0) := "111";
--  constant c_CHARS_HEIGHT: std_logic_vector(3 downto 0) := "1111";
--  constant c_CHARS_COLS: std_logic_vector(6 downto 0) := "1100011";
--  constant c_CHARS_ROWS: std_logic_vector(5 downto 0) := "100100";
 
end vis_vga_pkg;


package body vis_vga_pkg is

end vis_vga_pkg;
