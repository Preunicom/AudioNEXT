library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity async_fifo is
  generic (
    G_DATA_WIDTH : integer := 24;
    G_ADDR_WIDTH : integer := 4  -- 2^4 = 16 Speicherplätze reichen völlig aus
  );
  port (
    -- Schreibseite (Audio Clock Domain)
    i_wr_clk   : in  std_logic;
    i_wr_rst   : in  std_logic;
    i_wr_en    : in  std_logic;
    i_din      : in  std_logic_vector(G_DATA_WIDTH-1 downto 0);
    o_full     : out std_logic;
    o_overflow : out std_logic;

    -- Leseseite (AXI Clock Domain)
    i_rd_clk   : in  std_logic;
    i_rd_rst   : in  std_logic;
    i_rd_en    : in  std_logic;
    o_dout     : out std_logic_vector(G_DATA_WIDTH-1 downto 0);
    o_empty    : out std_logic
  );
end async_fifo;