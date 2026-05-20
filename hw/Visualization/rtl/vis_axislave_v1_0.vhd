library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.vis_vga_pkg.all;

entity vis is
  generic (
    -- Users to add parameters here

    -- User parameters ends
    -- Do not modify the parameters beyond this line


    -- Parameters of Axi Slave Bus Interface S00_AXI
    C_S00_AXI_DATA_WIDTH  : integer := 32;
    C_S00_AXI_ADDR_WIDTH  : integer := 6
  );
  port (
    -- Users to add ports here
    --dm begin
    -- Begin user code (Nicolas Lonthoff)
    -- Interface zum Visualization-Core
    i_pixel_clk : in std_logic;   -- Pixel clock signal (25,175 MHz)
    --
    o_interrupt : out std_logic;  -- 
    o_red: out std_logic_vector(3 downto 0); 	-- Red color channel
    o_green: out std_logic_vector(3 downto 0); 	-- Green color channel
    o_blue : out std_logic_vector(3 downto 0); 	-- Blue color channel
    o_hsync : out std_logic;			 	-- Horizontal synchronisation signal
    o_vsync : out std_logic;        -- Vertical synchroniston signal
    -- End user code (Nicolas Lonthoff)
    --dm end
    
    -- User ports ends
    -- Do not modify the ports beyond this line

    -- Ports of Axi Slave Bus Interface S00_AXI
    s00_axi_aclk  : in std_logic;
    s00_axi_aresetn : in std_logic;
    s00_axi_awaddr  : in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
    s00_axi_awprot  : in std_logic_vector(2 downto 0);
    s00_axi_awvalid : in std_logic;
    s00_axi_awready : out std_logic;
    s00_axi_wdata : in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
    s00_axi_wstrb : in std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0);
    s00_axi_wvalid  : in std_logic;
    s00_axi_wready  : out std_logic;
    s00_axi_bresp : out std_logic_vector(1 downto 0);
    s00_axi_bvalid  : out std_logic;
    s00_axi_bready  : in std_logic;
    s00_axi_araddr  : in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
    s00_axi_arprot  : in std_logic_vector(2 downto 0);
    s00_axi_arvalid : in std_logic;
    s00_axi_arready : out std_logic;
    s00_axi_rdata : out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
    s00_axi_rresp : out std_logic_vector(1 downto 0);
    s00_axi_rvalid  : out std_logic;
    s00_axi_rready  : in std_logic    
  );
end vis;

architecture arch_imp of vis is

  -- component declaration
  component at_S00_AXI is
    generic (
    C_S_AXI_DATA_WIDTH  : integer := 32;
    C_S_AXI_ADDR_WIDTH  : integer := 6
    );
    port (
    S_AXI_ACLK  : in std_logic;
    S_AXI_ARESETN : in std_logic;
    S_AXI_AWADDR  : in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
    S_AXI_AWPROT  : in std_logic_vector(2 downto 0);
    S_AXI_AWVALID : in std_logic;
    S_AXI_AWREADY : out std_logic;
    S_AXI_WDATA : in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    S_AXI_WSTRB : in std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
    S_AXI_WVALID  : in std_logic;
    S_AXI_WREADY  : out std_logic;
    S_AXI_BRESP : out std_logic_vector(1 downto 0);
    S_AXI_BVALID  : out std_logic;
    S_AXI_BREADY  : in std_logic;
    S_AXI_ARADDR  : in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
    S_AXI_ARPROT  : in std_logic_vector(2 downto 0);
    S_AXI_ARVALID : in std_logic;
    S_AXI_ARREADY : out std_logic;
    S_AXI_RDATA : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    S_AXI_RRESP : out std_logic_vector(1 downto 0);
    S_AXI_RVALID  : out std_logic;
    S_AXI_RREADY  : in std_logic;
    --
    --dm begin
    -- Begin user code (Nicolas Lonthoff)
    o_ven : out std_logic;
    o_vol : out std_logic_vector(8 downto 0);
    o_rt : out std_logic_vector(8 downto 0);
    o_yt : out std_logic_vector(8 downto 0);
    i_fdp : in std_logic;
    interrupt : out std_logic
    -- End user code (Nicolas Lonthoff)
    --dm end
    );
  end component at_S00_AXI;

--dm begin
component vis_core is
  port ( 
   -- USER CODE BEGIN Markus Remy (copied by Nicolas Lonthoff)
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
    -- All following outputs are in the clock domain of i_pixel_clk
    i_pixel_clk                 : in std_logic;
    o_hsync                     : out std_logic;
    o_vsync                     : out std_logic;
    o_red                       : out std_logic_vector (c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
    o_green                     : out std_logic_vector (c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
    o_blue                      : out std_logic_vector (c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0)
   -- USER CODE END Markus Remy
    );
end component;
 -- Begin user code (Nicolas Lonthoff)
  signal w_reset         : std_logic;
  signal w_ven           : std_logic;
  signal w_vol           : std_logic_vector(8 downto 0);
  signal w_rt            : std_logic_vector(8 downto 0);
  signal w_yt            : std_logic_vector(8 downto 0);
  -- frame buffer interface driven by conversion state machine
  signal w_buf_valid     : std_logic;
  signal w_buf_addr_x    : std_logic_vector(c_CHR_ADDR_BUS_W_X - 1 downto 0);
  signal w_buf_addr_y    : std_logic_vector(c_CHR_ADDR_BUS_W_Y - 1 downto 0);
  signal w_buf_char_ascii: std_logic_vector(c_CHR_ASCII_DATA_BUS_W - 1 downto 0);
  signal w_buf_color_red : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
  signal w_buf_color_green: std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
  signal w_buf_color_blue: std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
  signal w_buf_ready     : std_logic;
  signal w_fdp           : std_logic;
  signal w_interrupt     : std_logic;
  type t_conv_state is (s_IDLE, s_WRITE, s_DONE);
  signal r_state         : t_conv_state := s_IDLE;
  signal r_col           : unsigned(6 downto 0) := (others => '0');
  signal w_bar_width     : unsigned(5 downto 0);
 -- End user code (Nicolas Lonthoff)
--dm end
 
begin

-- Instantiation of Axi Bus Interface S00_AXI
at_S00_AXI_inst : at_S00_AXI
  generic map (
    C_S_AXI_DATA_WIDTH  => C_S00_AXI_DATA_WIDTH,
    C_S_AXI_ADDR_WIDTH  => C_S00_AXI_ADDR_WIDTH
  )
  port map (
    S_AXI_ACLK  => s00_axi_aclk,
    S_AXI_ARESETN => s00_axi_aresetn,
    S_AXI_AWADDR  => s00_axi_awaddr,
    S_AXI_AWPROT  => s00_axi_awprot,
    S_AXI_AWVALID => s00_axi_awvalid,
    S_AXI_AWREADY => s00_axi_awready,
    S_AXI_WDATA => s00_axi_wdata,
    S_AXI_WSTRB => s00_axi_wstrb,
    S_AXI_WVALID  => s00_axi_wvalid,
    S_AXI_WREADY  => s00_axi_wready,
    S_AXI_BRESP => s00_axi_bresp,
    S_AXI_BVALID  => s00_axi_bvalid,
    S_AXI_BREADY  => s00_axi_bready,
    S_AXI_ARADDR  => s00_axi_araddr,
    S_AXI_ARPROT  => s00_axi_arprot,
    S_AXI_ARVALID => s00_axi_arvalid,
    S_AXI_ARREADY => s00_axi_arready,
    S_AXI_RDATA => s00_axi_rdata,
    S_AXI_RRESP => s00_axi_rresp,
    S_AXI_RVALID  => s00_axi_rvalid,
    S_AXI_RREADY  => s00_axi_rready,
    --dm begin
    -- Begin user code (Nicolas Lonthoff)
      o_ven        => w_ven,
      o_vol        => w_vol,
      o_rt         => w_rt,
      o_yt         => w_yt,
      i_fdp        => w_fdp,
      interrupt    => w_interrupt
    -- End user code (Nicolas Lonthoff)
    --dm end
  );

  -- Add user logic here
  --dm begin
  -- Begin user code (Nicolas Lonthoff)
  w_reset <= not s00_axi_aresetn;

  -- copied from top.vhd and adjusted (by Nicolas Lonthoff)
  VGA_INST: vis_core
  port map (
    i_clk                       => s00_axi_aclk,
    i_rst                       => w_reset,
    i_buf_valid                 => w_buf_valid,
    i_buf_addr_x                => w_buf_addr_x,
    i_buf_addr_y                => w_buf_addr_y,
    i_buf_char_ascii            => w_buf_char_ascii,
    i_buf_color_red             => w_buf_color_red,
    i_buf_color_green           => w_buf_color_green,
    i_buf_color_blue            => w_buf_color_blue,
    o_buf_ready                 => w_buf_ready,
    o_visible_frame_done_pulse  => open,
    --
    i_vga_enable                => w_ven,
    i_pixel_clk                 => i_pixel_clk,
    o_hsync                     => o_hsync,
    o_vsync                     => o_vsync,
    o_red                       => o_red,
    o_green                     => o_green,
    o_blue                      => o_blue
  );

  o_interrupt <= w_interrupt;

  -- Bar width = integer part of volume / 2 (w_vol bits 8:3 = upper 6 bits of
  -- the 7.2 fixed-point value), giving a range of 0-63 columns out of 80.
  w_bar_width <= unsigned(w_vol(8 downto 3));

  -- Converts the 9-bit volume value into a one-row horizontal bar written to
  -- vis_core's frame buffer one character at a time.  Runs continuously: each
  -- time vis_core enters blanking (w_buf_ready = '1') a fresh frame is written
  -- so the display always reflects the latest volume.
  CONV_SM: process(s00_axi_aclk)
  begin
    if rising_edge(s00_axi_aclk) then
      -- Pulse-only defaults; overridden inside the relevant states.
      w_fdp       <= '0';
      w_buf_valid <= '0';
      if w_reset = '1' then
        r_state <= s_IDLE;
        r_col   <= (others => '0');
      else
        case r_state is

          -- Wait for blanking period before starting a new frame write.
          -- w_buf_ready is only high while vis_core is in blanking, so writes
          -- are safe from display tearing.
          when s_IDLE =>
            if w_buf_ready = '1' then
              r_col   <= (others => '0');
              r_state <= s_WRITE;
            end if;

          -- Walk columns 0-79 on row 0, one character per clock.
          -- Columns inside the bar receive '*' (ASCII 42), outside ' ' (32).
          -- The bar is colored uniformly based on volume vs. thresholds:
          --   vol > RT → red,  vol > YT → yellow,  otherwise → green.
          when s_WRITE =>
            if w_buf_ready = '1' then
              w_buf_valid  <= '1';
              w_buf_addr_x <= std_logic_vector(r_col);
              w_buf_addr_y <= (others => '0');  -- row 0
              if r_col < w_bar_width then
                w_buf_char_ascii <= std_logic_vector(to_unsigned(42, c_CHR_ASCII_DATA_BUS_W)); -- '*'
              else
                w_buf_char_ascii <= std_logic_vector(to_unsigned(32, c_CHR_ASCII_DATA_BUS_W)); -- ' '
              end if;
              -- Red threshold is checked first so it takes priority over yellow
              if unsigned(w_vol) > unsigned(w_rt) then
                w_buf_color_red   <= (others => '1');
                w_buf_color_green <= (others => '0');
                w_buf_color_blue  <= (others => '0');
              elsif unsigned(w_vol) > unsigned(w_yt) then
                w_buf_color_red   <= (others => '1');
                w_buf_color_green <= (others => '1');
                w_buf_color_blue  <= (others => '0');
              else
                w_buf_color_red   <= (others => '0');
                w_buf_color_green <= (others => '1');
                w_buf_color_blue  <= (others => '0');
              end if;
              if r_col = to_unsigned(79, 7) then
                r_state <= s_DONE;  -- last column written
              else
                r_col <= r_col + 1;
              end if;
            end if;

          -- Assert w_fdp for exactly one cycle to tell the AXI slave the
          -- frame buffer update is complete (sets IPISR.FDP → interrupt).
          when s_DONE =>
            w_fdp   <= '1';
            r_state <= s_IDLE;

        end case;
      end if;
    end if;
  end process CONV_SM;
  -- End user code (Nicolas Lonthoff)
  --dm end
  -- User logic ends

end arch_imp;
