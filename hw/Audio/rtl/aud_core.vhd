library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

--begin Maximilian Hafeneder

entity aud_core is
  port ( 
    i_audio_clk: in std_logic;
    i_clk: in std_logic;
    i_resetn: in std_logic;
    --
    i_sampling_en: in std_logic;
    i_data: in std_logic;
    i_ack_left: in std_logic;
    i_ack_right: in std_logic;
    --
    o_mclk: out std_logic;
    o_sclk: out std_logic;
    o_lrck: out std_logic;
    --
    o_sla: out std_logic;
    o_sra: out std_logic;
    o_dol: out std_logic;
    o_dor: out std_logic;
    --
    o_data_left: out std_logic_vector(23 downto 0);
    o_data_right: out std_logic_vector(23 downto 0);
    --
    dummylast: in std_logic
  );
end aud_core;

architecture Behavioral of aud_core is

  component aud_i2s_clk_gen is
    port(
      i_mclk : in std_logic;
      i_resetn : in std_logic;
      i_sen : in std_logic;
    
      o_lrck : out std_logic;
      o_sclk : out std_logic
    );
  end component;
  
  component aud_i2s_sampling is
    port(
      i_mclk: in std_logic;
      i_sclk: in std_logic;
      i_lrck: in std_logic;
      i_resetn: in std_logic;
      --
      i_sen: in std_logic;
      i_data: in std_logic;
      --
      o_valid_l: out std_logic;
      o_valid_r: out std_logic;
      --
      o_data_l: out std_logic_vector(23 downto 0);
      o_data_r: out std_logic_vector(23 downto 0)
    );
  end component;  
  
  component aud_channel_buffer is
    Port ( 
      wr_rst_busy : out STD_LOGIC;
      rd_rst_busy : out STD_LOGIC;
      m_aclk : in STD_LOGIC;
      s_aclk : in STD_LOGIC;
      s_aresetn : in STD_LOGIC;
      s_axis_tvalid : in STD_LOGIC;
      s_axis_tready : out STD_LOGIC;
      s_axis_tdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
      m_axis_tvalid : out STD_LOGIC;
      m_axis_tready : in STD_LOGIC;
      m_axis_tdata : out STD_LOGIC_VECTOR ( 31 downto 0 )
    );

  end component;
  
  signal w_lrck : std_logic;
  signal w_sclk : std_logic;

  signal w_valid_l: std_logic;
  signal w_valid_r: std_logic;
  signal w_data_l: std_logic_vector(23 downto 0);
  signal w_data_r: std_logic_vector(23 downto 0);

  signal w_fifo_data_l: std_logic_vector(31 downto 0);
  signal w_fifo_data_r: std_logic_vector(31 downto 0);
  signal w_tready_l: std_logic;
  signal w_tready_r: std_logic;

begin
  
  aud_i2s_clk_gen_inst: aud_i2s_clk_gen port map(
    i_mclk => i_audio_clk,
    i_resetn => i_resetn,
    i_sen => i_sampling_en,
    o_lrck => w_lrck,
    o_sclk => w_sclk
  );
    
  aud_i2s_sampling_inst: aud_i2s_sampling port map(
    i_mclk => i_audio_clk,
    i_resetn => i_resetn,
    i_sclk => w_sclk,
    i_lrck => w_lrck,
    i_sen => i_sampling_en,
    i_data => i_data,
    o_valid_l => w_valid_l,
    o_valid_r => w_valid_r,
    o_data_l => w_data_l,
    o_data_r => w_data_r
  );
    
  aud_channel_buffer_left: aud_channel_buffer port map(
    wr_rst_busy => open,
    rd_rst_busy => open,
    s_aclk => i_audio_clk,
    m_aclk => i_clk,
    s_aresetn => i_resetn,
    s_axis_tvalid => w_valid_l,
    s_axis_tdata => "00000000" & w_data_l,
    s_axis_tready => w_tready_l,
    m_axis_tvalid => o_sla,
    m_axis_tready => i_ack_left,
    m_axis_tdata => w_fifo_data_l
  );

  aud_channel_buffer_right: aud_channel_buffer port map(
    wr_rst_busy => open,
    rd_rst_busy => open,
    s_aclk => i_audio_clk,
    m_aclk => i_clk,
    s_aresetn => i_resetn,
    s_axis_tvalid => w_valid_r,
    s_axis_tdata => "00000000" & w_data_r,
    s_axis_tready => w_tready_r,
    m_axis_tvalid => o_sra,
    m_axis_tready => i_ack_right,
    m_axis_tdata => w_fifo_data_r
  );
    
  o_mclk <= i_audio_clk;
  o_sclk <= w_sclk;
  o_lrck <= w_lrck;

  o_dol <= not w_tready_l;
  o_dor <= not w_tready_r;

  o_data_left <= w_fifo_data_l(23 downto 0);
  o_data_right <= w_fifo_data_r(23 downto 0);
--end Maximilian Hafeneder

end Behavioral;