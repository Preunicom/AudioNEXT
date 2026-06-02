library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- EDIT CODE BEGIN Richard Tuch

entity aud is

-- EDIT CODE END Richard Tuch

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
    -- dm begin

    -- EDIT CODE BEGIN RICHARD TUCH

    -- Audio I/O ports
    i_aud_clk     : in std_logic;
    o_mclk          : out std_logic;
    o_lrck          : out std_logic;
    o_sclk          : out std_logic;  
    i_data          : in std_logic;
    -- Interrupts
    o_interrupt_sla : out std_logic;
    o_interrupt_sra : out std_logic;

    -- EDIT CODE END RICHARD TUCH

    -- dm end
    
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

-- EDIT CODE BEGIN Richard Tuch

end aud;

-- EDIT CODE END Richard Tuch


-- EDIT CODE BEGIN Richard Tuch

architecture arch_imp of aud is

  -- component declaration
  component aud_S00_AXI is

  -- EDIT CODE END Richard Tuch

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

    -- EDIT CODE BEGIN RICHARD TUCH

    o_sampling_en : out std_logic;
    i_sla           : in  std_logic;
    i_sra           : in  std_logic;
    i_dol           : in  std_logic;
    i_dor           : in  std_logic;
    i_data_left     : in  std_logic_vector(23 downto 0);
    i_data_right    : in  std_logic_vector(23 downto 0);
    o_ack_left      : out std_logic;
    o_ack_right     : out std_logic;
    --
    o_interrupt_sla : out std_logic;
    o_interrupt_sra : out std_logic;
    --
    dummylast : in std_logic
    --dm end
    );
  end component aud_S00_AXI;

-- EDIT CODE END Richard Tuch


-- EDIT CODE BEGIN Richard Tuch

--dm begin
component aud_core is
  port ( 
    i_clk         : in  std_logic;
    i_resetn       : in  std_logic;
    i_aud_clk   : in  std_logic;
    -- Control
    i_sampling_en : in  std_logic;
    -- Status
    o_sla         : out std_logic;
    o_sra         : out std_logic;
    o_dol         : out std_logic;
    o_dor         : out std_logic;
    -- Audio Data
    o_data_left   : out std_logic_vector(23 downto 0);
    o_data_right  : out std_logic_vector(23 downto 0);
    -- Acknowledge
    i_ack_left    : in  std_logic;
    i_ack_right   : in  std_logic;
    -- Pmod I2S2 Signals
    o_mclk        : out std_logic;
    o_lrck        : out std_logic;
    o_sclk        : out std_logic;
    i_data        : in  std_logic;
    --
    dummylast     : in  std_logic 
  );
end component;

-- EDIT CODE END Richard Tuch


-- EDIT CODE BEGIN Richard Tuch
 
  signal w_resetn : std_logic;

  -- Internal signals between AXI slave and aud_core
  signal w_sampling_en : std_logic;
  signal w_sla         : std_logic;
  signal w_sra         : std_logic;
  signal w_dol         : std_logic;
  signal w_dor         : std_logic;
  signal w_data_left   : std_logic_vector(23 downto 0);
  signal w_data_right  : std_logic_vector(23 downto 0);
  signal w_ack_left    : std_logic;
  signal w_ack_right   : std_logic;
  signal w_interrupt_sla : std_logic;
  signal w_interrupt_sra : std_logic;
  --dm end

-- EDIT CODE END Richard Tuch


-- EDIT CODE BEGIN Richard Tuch
 
begin
-- Instantiation of Axi Bus Interface S00_AXI
aud_S00_AXI_inst : aud_S00_AXI

-- EDIT CODE END Richard Tuch

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

    -- EDIT CODE BEGIN RICHARD TUCH

    o_sampling_en   => w_sampling_en,
    i_sla           => w_sla,
    i_sra           => w_sra,
    i_dol           => w_dol,
    i_dor           => w_dor,
    i_data_left     => w_data_left,
    i_data_right    => w_data_right,
    o_ack_left      => w_ack_left,
    o_ack_right     => w_ack_right,
    o_interrupt_sla => w_interrupt_sla,
    o_interrupt_sra => w_interrupt_sra,
    --
    dummylast => '0'

    -- EDIT CODE END RICHARD TUCH

    --dm end
  );


  -- Add user logic here
  --dm begin
  w_resetn <= not s00_axi_aresetn;

-- EDIT CODE BEGIN Richard Tuch

  aud_core_inst: aud_core
    port map(
      i_clk => s00_axi_aclk,
      i_resetn => w_resetn,
      i_aud_clk => i_aud_clk,
      -- Control
      i_sampling_en => w_sampling_en,
      -- Status
      o_sla => w_sla,
      o_sra => w_sra,
      o_dol => w_dol,
      o_dor => w_dor,
      -- Audio Data
      o_data_left => w_data_left,
      o_data_right => w_data_right,
      -- Acknowledge
      i_ack_left => w_ack_left,
      i_ack_right => w_ack_right,
      -- Pmod I2S2 Signals
      o_mclk => o_mclk,
      o_lrck => o_lrck,
      o_sclk => o_sclk,
      i_data => i_data,
      --   
      dummylast => '0'      
    );

  o_interrupt_sla <= w_interrupt_sla;
  o_interrupt_sra <= w_interrupt_sra;
  
  --EDIT CODE END Richard Tuch

  --dm end
  -- User logic ends

end arch_imp;