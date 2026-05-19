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
    o_t0_out: out std_logic;
    --
    interrupt: out std_logic;

    -- Begin user code (Nicolas Lonthoff)
    -- Interface zum Visualization-Core
    0_interrupt : out std_logic; 
    o_ven: out std_logic; -- Visualisation Enable
    o_write_data: out std_logic; -- Write Data Strobe (WD)
    o_char: out std_logic_vector(6 downto 0); -- ASCII Character (VDATR)
    o_x_addr: out std_logic_vector(6 downto 0); -- X-Address (ADDRR bits 6:0)
    o_y_addr: out std_logic_vector(4 downto 0); -- Y-Address (ADDRR bits 12:8)
    o_color : out std_logic_vector(11 downto 0); -- Color (COLR bits 11:0)
    i_fdp: in std_logic -- Frame Data Processed (FDP) from Core
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
      o_ap_start : out std_logic;
      i_ap_idle : in std_logic;
      i_ap_done : in std_logic;      
      o_auto_restart : out std_logic;
      --
      o_ent0_out : out std_logic;
      o_load0 : out std_logic;
      o_ud0 : out std_logic;
      o_reset_ip: out std_logic;
      o_freeze_ip: out std_logic;
      --
      o_LR0: out std_logic_vector(31 downto 0);
      --
      i_CR0: in std_logic_vector(31 downto 0);
      --
      interrupt: out std_logic;
      --
      dummylast : in std_logic
    --dm end
    );
  end component at_S00_AXI;

--dm begin
component at_visualization_core is
  port ( 
    i_clk : in std_logic;
    i_reset : in std_logic;
    --
    i_ap_start : in std_logic;
    o_ap_idle : out std_logic;
    o_ap_done : out std_logic;        
    i_auto_restart : in std_logic;
    --
    i_ent0_out : in std_logic;
    i_load0 : in std_logic;
    i_ud0 : in std_logic;
    i_reset_ip: in std_logic;
    i_freeze_ip: in std_logic;
    --
    i_LR0: in std_logic_vector(31 downto 0);
    --
    o_CR0: out std_logic_vector(31 downto 0);
    --
    o_t0_out: out std_logic;   
    --
    dummylast : in std_logic  
  );
end component;   
 
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
      o_ap_start => w_ap_start, 
      i_ap_idle => w_ap_idle,
      i_ap_done => w_ap_done,      
      o_auto_restart => w_auto_restart,
      --
      o_ent0_out => w_ent0_out,
      o_load0 => w_load0,
      o_ud0 => w_ud0,
      o_reset_ip => w_reset_ip,
      o_freeze_ip => w_freeze_ip,
      --
      o_LR0 => w_LR0,
      --
      i_CR0 => w_CR0,
      --
      interrupt=>w_interrupt,
      --
      dummylast => '0'
    --dm end
  );

  -- Add user logic here
  --dm begin
  w_reset <= not s00_axi_aresetn;

  at_core_inst: at_core
    port map(
      i_clk => s00_axi_aclk,
      i_reset => w_reset,
      --
      i_ap_start => w_ap_start,
      o_ap_idle =>  w_ap_idle,
      o_ap_done =>  w_ap_done,      
      i_auto_restart => w_auto_restart,
      --
      i_ent0_out => w_ent0_out,
      i_load0 => w_load0,
      i_ud0 => w_ud0,
      i_reset_ip => w_reset_ip,
      i_freeze_ip => w_freeze_ip,
      --
      i_LR0 => w_LR0,
      --
      o_CR0 => w_CR0,
      --
      o_t0_out => w_t0_out,
      --   
      dummylast => '0'      
    );

  o_t0_out <= w_t0_out;

  interrupt <= w_interrupt;

  --dm end
  -- User logic ends

end arch_imp;
