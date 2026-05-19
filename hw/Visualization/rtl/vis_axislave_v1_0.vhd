library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity at is
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
    o_interrupt : out std_logic; 
    o_ven: out std_logic; 				-- Visualisation Enable
    o_write_data: out std_logic; 			-- Write Data Strobe (WD)
    o_char: out std_logic_vector(6 downto 0); 		-- ASCII Character (VDATR)
    o_x_addr: out std_logic_vector(6 downto 0); 	-- X-Address (ADDRR bits 6:0)
    o_y_addr: out std_logic_vector(4 downto 0); 	-- Y-Address (ADDRR bits 12:8)
    o_color : out std_logic_vector(11 downto 0); 	-- Color (COLR bits 11:0)
    i_fdp: in std_logic;			 	-- Frame Data Processed (FDP) from Core
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
end at;

architecture arch_imp of at is

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
    o_wd : out std_logic;
    o_char : out std_logic_vector(6 downto 0);
    o_x_addr : out std_logic_vector(6 downto 0);
    o_y_addr : out std_logic_vector(4 downto 0);
    o_color : out std_logic_vector(11 downto 0);
    i_fdp : in std_logic;
    interrupt : out std_logic
    -- End user code (Nicolas Lonthoff)
    --dm end
    );
  end component at_S00_AXI;

--dm begin
component at_visualization_core is
  port ( 
    i_clk : in std_logic;
    i_reset : in std_logic;
    --
    -- Begin user code (Nicolas Lonthoff)
    i_ven : in std_logic;
    i_wd : in std_logic;
    i_char : in std_logic_vector(6 downto 0);
    i_x_addr: in std_logic_vector(6 downto 0);
    i_y_addr: in std_logic_vector(4 downto 0);
    i_color: in std_logic_vector(11 downto 0);
    --
    o_fdp: out std_logic   
    -- End user code (Nicolas Lonthoff)
  );
end component;   
 -- Begin user code (Nicolas Lonthoff)
 signal w_reset : std_logic;
 -- End user code (Nicolas Lonthoff)
 signal w_ven : std_logic;
 signal w_wd: std_logic;
 signal w_char: std_logic_vector(6 downto 0);
 --
 signal w_x_addr: std_logic_vector(6 downto 0);
 signal w_y_addr: std_logic_vector(4 downto 0);
 --
 signal w_color: std_logic_vector(11 downto 0);
 --
 signal w_fdp: std_logic;
 
 signal w_interrupt: std_logic;
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
      o_wd         => w_wd,
      o_char       => w_char,
      o_x_addr     => w_x_addr,
      o_y_addr     => w_y_addr,
      o_color      => w_color,
      i_fdp        => w_fdp,
      interrupt    => w_interrupt
    -- End user code (Nicolas Lonthoff)
    --dm end
  );

  -- Add user logic here
  --dm begin
  -- Begin user code (Nicolas Lonthoff)
  w_reset <= not s00_axi_aresetn;

  visualization_core_inst: visualization_core
    port map(
      i_clk => s00_axi_aclk,
      i_reset => w_reset,
      --
      i_ven => w_ven,
      i_wd => w_wd,
      i_char => w_char,
      i_x_addr => w_x_addr,
      i_y_addr => w_y_addr,
      i_color => w_color,
      --
      o_fdp => w_fdp     
    );

  o_interrupt <= w_interrupt;
  -- End user code (Nicolas Lonthoff)
  --dm end
  -- User logic ends

end arch_imp;
