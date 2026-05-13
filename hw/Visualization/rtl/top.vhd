-- USER CODE BEGIN Markus Remy
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity TOP is
  generic (
    g_DELAY : in natural := 120
  );
  port(
    i_sys_clk   : in std_logic;
    i_reset     : in std_logic;
    o_hsync     : out std_logic;
    o_vsync     : out std_logic;
    o_red       : out std_logic_vector (3 downto 0);
    o_green     : out std_logic_vector (3 downto 0);
    o_blue      : out std_logic_vector (3 downto 0)   
  );
        
end TOP;

architecture BEHAV of TOP is

  component vis_core is
    port (
          -- Frame buffer signals
          i_clk                       : in std_logic;
          i_rst                       : in std_logic;
          i_buf_valid                 : in std_logic;
          i_buf_addr_x                : in std_logic_vector(6 downto 0);
          i_buf_addr_y                : in std_logic_vector(4 downto 0);
          i_buf_char_ascii            : in std_logic_vector(6 downto 0);
          i_buf_color_red             : in std_logic_vector(3 downto 0);
          i_buf_color_green           : in std_logic_vector(3 downto 0);
          i_buf_color_blue            : in std_logic_vector(3 downto 0);
          o_buf_ready                 : out std_logic;
          -- VGA signals
          i_vga_enable                : in std_logic;
          o_visible_frame_done_pulse  : out std_logic;
          o_hsync                     : out std_logic;
          o_vsync                     : out std_logic;
          o_red                       : out std_logic_vector (3 downto 0);
          o_green                     : out std_logic_vector (3 downto 0);
          o_blue                      : out std_logic_vector (3 downto 0)   
          );
  end component;

  signal r_char_counter : natural;
  signal r_frame_counter : natural;

  type state_t is (s_TEXT1, s_BLANK1, s_TEXT2, s_BLANK2);
  signal r_current_state, r_next_state: state_t := s_TEXT1;

  type text_array_t is array (natural range <>) of character;
  constant c_TEXT1 : text_array_t := (' ', 'H', 'A', 'L', 'L', 'O', ' ', 'W', 'E', 'L', 'T', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  constant c_TEXT1_len : natural := 20;
  constant c_TEXT2 : text_array_t := (' ', 'I', 'C', 'H', ' ', 'W', 'A', 'R', ' ', '2', '0', '2', '6', ' ', 'H', 'I', 'E', 'R', '!', ' ');
  constant c_TEXT2_len : natural := 20;

  signal w_char_valid : std_logic;
  signal w_char_ascii : std_logic_vector(6 downto 0);
  signal w_char_addr_x : std_logic_vector(6 downto 0);
  signal w_char_addr_y : std_logic_vector(4 downto 0);
  signal w_char_col_r : std_logic_vector(3 downto 0);
  signal w_char_col_g : std_logic_vector(3 downto 0);
  signal w_char_col_b : std_logic_vector(3 downto 0);

  signal w_visible_frame_done_pulse : std_logic;

  signal w_buf_ready: std_logic;

  signal r_last_vis_frame_done : std_logic;

  signal r_char_counter_reset : std_logic;
  signal r_frame_counter_reset : std_logic;
begin

  VGA_INST: vis_core
  port map (
    i_clk                       => i_sys_clk,
    i_rst                       => i_reset,
    i_buf_valid                 => w_char_valid,
    i_buf_addr_x                => w_char_addr_x,
    i_buf_addr_y                => w_char_addr_y,
    i_buf_char_ascii            => w_char_ascii,
    i_buf_color_red             => w_char_col_r,
    i_buf_color_green           => w_char_col_g,
    i_buf_color_blue            => w_char_col_b,
    o_buf_ready                 => w_buf_ready,
    i_vga_enable                => '1',
    o_visible_frame_done_pulse  => w_visible_frame_done_pulse,
    o_hsync                     => o_hsync,
    o_vsync                     => o_vsync,
    o_red                       => o_red,
    o_green                     => o_green,
    o_blue                      => o_blue
  );

  process(i_sys_clk)
  begin
    if rising_edge(i_sys_clk) then
      if i_reset = '1' then
        r_current_state <= s_TEXT1;
      else
        r_current_state <= r_next_state;
      end if;
    end if;
  end process;

  process(r_current_state, r_frame_counter, r_char_counter)
  begin
    r_next_state <= r_current_state;
    w_char_valid <= '0';
    w_char_addr_x <= (others => '0');
    w_char_addr_y <= (others => '0');
    w_char_ascii <= (others => '0');
    w_char_col_r <= (others => '0');
    w_char_col_g <= (others => '0');
    w_char_col_b <= (others => '0');
    r_char_counter_reset <= '0';
    r_frame_counter_reset <= '0';
    case r_current_state is
      when s_TEXT1 => 
        if r_char_counter < c_TEXT1_len then
          w_char_valid <= '1';
          w_char_addr_x <= std_logic_vector(to_unsigned(r_char_counter, 7));
          w_char_addr_y <= "00001";
          w_char_ascii <= std_logic_vector(
                   to_unsigned(
                     character'pos(c_TEXT1(r_char_counter)),
                     7
                   )
                 );
          w_char_col_r <= "1111";
          w_char_col_g <= "1111";
          w_char_col_b <= "1111";
        else
          r_frame_counter_reset <= '1';
          r_next_state <= s_BLANK1;
        end if;
      when s_BLANK1 => 
        if r_frame_counter >= g_DELAY then
          r_next_state <= s_TEXT2;
          r_char_counter_reset <= '1';
        end if;
      when s_TEXT2 => 
        if r_char_counter < c_TEXT2_len then
          w_char_valid <= '1';
          w_char_addr_x <= std_logic_vector(to_unsigned(r_char_counter, 7));
          w_char_addr_y <= "00001";
          w_char_ascii <= std_logic_vector(
                   to_unsigned(
                     character'pos(c_TEXT2(r_char_counter)),
                     7
                   )
                 );
          w_char_col_r <= "0000";
          w_char_col_g <= "0000";
          w_char_col_b <= "1111";
        else
          r_frame_counter_reset <= '1';
          r_next_state <= s_BLANK2;
        end if;
      when s_BLANK2 => 
         if r_frame_counter >= g_DELAY then
          r_next_state <= s_TEXT1;
          r_char_counter_reset <= '1';
        end if;
    end case;

  end process;

  CHAR_COUNTER: process(i_sys_clk)
  begin
    if rising_edge(i_sys_clk) then
      if i_reset = '1' then
        r_char_counter <= 0;
      else
        if r_char_counter_reset = '0' then
          if w_char_valid = '1' and w_buf_ready = '1' then
            r_char_counter <= r_char_counter + 1;
          end if;
        else
          r_char_counter <= 0;
        end if;
      end if;
    end if;
  end process;

  FRAME_COUNTER: process(i_sys_clk)
  begin
    if rising_edge(i_sys_clk) then
      if i_reset = '1' then
        r_frame_counter <= 0;
        r_last_vis_frame_done <= '0';
      else
        r_last_vis_frame_done <= w_visible_frame_done_pulse;
        if r_frame_counter_reset = '0' then
          if r_last_vis_frame_done = '0' and w_visible_frame_done_pulse = '1' then
            r_frame_counter <= r_frame_counter + 1;
          end if;
        else
          r_frame_counter <= 0;
        end if;
      end if;
    end if;
  end process;

end BEHAV;


-- TODO Delete this file after testing