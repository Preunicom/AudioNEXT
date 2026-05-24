library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use std.env.finish;

entity TB_vis_RAM is
end TB_vis_RAM;

architecture Testbench of TB_vis_RAM is

    -- Small instance sufficient to test all behaviours (16 entries × 8 bit)
    constant c_DATA_W : natural := 8;
    constant c_ADDR_W : natural := 4;

    -- Write clock and read clock are 180° phase-offset, matching the usage
    -- in vis_vga_ctrl where i_clock_r => not i_vga_clk
    signal tb_clk_w  : std_logic := '0';
    signal tb_clk_r  : std_logic := '1';  -- starts opposite to tb_clk_w

    signal tb_en_w   : std_logic := '0';
    signal tb_wr_en  : std_logic := '0';
    signal tb_addr_w : std_logic_vector(c_ADDR_W - 1 downto 0) := (others => '0');
    signal tb_data_w : std_logic_vector(c_DATA_W - 1 downto 0) := (others => '0');

    signal tb_en_r   : std_logic := '0';
    signal tb_addr_r : std_logic_vector(c_ADDR_W - 1 downto 0) := (others => '0');
    signal tb_data_r : std_logic_vector(c_DATA_W - 1 downto 0);

    signal tb_test_passed : boolean := false;

begin

    DUT: entity work.vis_RAM
        generic map (
            g_DATA_WIDTH => c_DATA_W,
            g_ADDR_WIDTH => c_ADDR_W
        )
        port map (
            i_clock_w    => tb_clk_w,
            i_EN_w       => tb_en_w,
            i_write_en_w => tb_wr_en,
            i_ADDR_w     => tb_addr_w,
            i_DI_w       => tb_data_w,
            i_clock_r    => tb_clk_r,
            i_EN_r       => tb_en_r,
            i_ADDR_r     => tb_addr_r,
            o_DO_r       => tb_data_r
        );

    tb_clk_w <= not tb_clk_w after 20 ns;
    tb_clk_r <= not tb_clk_r after 20 ns;

    STIMULUS: process
    begin
        -- Read pattern: present address, wait one read-clock rising edge (port latches),
        -- wait a second (output now stable), then assert.
        -- Write pattern: present address + data + enables, wait one write-clock rising
        -- edge (shared variable updated immediately).

        -- ----------------------------------------------------------------
        -- T1: Basic write/read - write 0xAB to addr 0, read back
        -- ----------------------------------------------------------------
        tb_addr_w <= X"0";
        tb_data_w <= X"AB";
        tb_en_w   <= '1';
        tb_wr_en  <= '1';
        wait until rising_edge(tb_clk_w);   -- shared variable updated
        tb_wr_en  <= '0';

        tb_addr_r <= X"0";
        tb_en_r   <= '1';
        wait until rising_edge(tb_clk_r);   -- read port latches addr=0
        wait until rising_edge(tb_clk_r);   -- output stable
        assert tb_data_r = X"AB"
            report "T1 FAIL: expected 0xAB, got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;
        report "T1 PASSED: wrote 0xAB to addr 0, read back correctly" severity note;

        -- ----------------------------------------------------------------
        -- T2: i_write_en_w='0' must block the write
        -- ----------------------------------------------------------------
        tb_addr_w <= X"0";
        tb_data_w <= X"CD";
        tb_en_w   <= '1';
        tb_wr_en  <= '0';               -- write enable blocked
        wait until rising_edge(tb_clk_w);

        tb_addr_r <= X"0";
        wait until rising_edge(tb_clk_r);
        wait until rising_edge(tb_clk_r);
        assert tb_data_r = X"AB"
            report "T2 FAIL: i_write_en_w='0' did not block write; got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;
        report "T2 PASSED: i_write_en_w='0' blocks write, value preserved" severity note;

        -- ----------------------------------------------------------------
        -- T3: i_EN_w='0' must block the write
        -- ----------------------------------------------------------------
        tb_addr_w <= X"0";
        tb_data_w <= X"EF";
        tb_en_w   <= '0';               -- EN_w blocked
        tb_wr_en  <= '1';
        wait until rising_edge(tb_clk_w);
        tb_en_w   <= '1';

        tb_addr_r <= X"0";
        wait until rising_edge(tb_clk_r);
        wait until rising_edge(tb_clk_r);
        assert tb_data_r = X"AB"
            report "T3 FAIL: i_EN_w='0' did not block write; got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;
        report "T3 PASSED: i_EN_w='0' blocks write, value preserved" severity note;

        -- ----------------------------------------------------------------
        -- T4: i_EN_r='0' must freeze the read output
        -- ----------------------------------------------------------------
        -- output is currently 0xAB from T3; disable read and point to addr 5
        tb_en_r   <= '0';
        tb_addr_r <= X"5";              -- addr 5: never written, would give 0x00
        wait until rising_edge(tb_clk_r);   -- EN_r='0': no update
        wait until rising_edge(tb_clk_r);
        assert tb_data_r = X"AB"
            report "T4 FAIL: output changed while EN_r='0'; got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;

        -- re-enable and verify the pending address now takes effect
        tb_en_r <= '1';
        wait until rising_edge(tb_clk_r);   -- latch addr=5 with EN_r='1'
        wait until rising_edge(tb_clk_r);   -- output = v_ram0(5) = 0x00
        assert tb_data_r = X"00"
            report "T4 FAIL: output did not update after re-enable; got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;
        report "T4 PASSED: EN_r gate freezes output; resumes on re-enable" severity note;

        -- ----------------------------------------------------------------
        -- T5: Independent writes to multiple addresses, read each back
        -- ----------------------------------------------------------------
        tb_wr_en <= '1';

        tb_addr_w <= X"1"; tb_data_w <= X"11";
        wait until rising_edge(tb_clk_w);
        tb_addr_w <= X"2"; tb_data_w <= X"22";
        wait until rising_edge(tb_clk_w);
        tb_addr_w <= X"F"; tb_data_w <= X"FF";
        wait until rising_edge(tb_clk_w);
        tb_wr_en <= '0';

        tb_addr_r <= X"1";
        wait until rising_edge(tb_clk_r);
        wait until rising_edge(tb_clk_r);
        assert tb_data_r = X"11"
            report "T5 FAIL: addr 1 expected 0x11, got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;

        tb_addr_r <= X"2";
        wait until rising_edge(tb_clk_r);
        wait until rising_edge(tb_clk_r);
        assert tb_data_r = X"22"
            report "T5 FAIL: addr 2 expected 0x22, got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;

        tb_addr_r <= X"F";
        wait until rising_edge(tb_clk_r);
        wait until rising_edge(tb_clk_r);
        assert tb_data_r = X"FF"
            report "T5 FAIL: addr 15 expected 0xFF, got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;

        -- Verify addr 0 was not corrupted by writes to other addresses
        tb_addr_r <= X"0";
        wait until rising_edge(tb_clk_r);
        wait until rising_edge(tb_clk_r);
        assert tb_data_r = X"AB"
            report "T5 FAIL: addr 0 corrupted, expected 0xAB, got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;
        report "T5 PASSED: multi-address writes independent; no cross-address corruption" severity note;

        -- ----------------------------------------------------------------
        -- T6: Overwrite - second write to the same address takes precedence
        -- ----------------------------------------------------------------
        tb_addr_w <= X"0";
        tb_data_w <= X"55";
        tb_en_w   <= '1';
        tb_wr_en  <= '1';
        wait until rising_edge(tb_clk_w);
        tb_wr_en  <= '0';

        tb_addr_r <= X"0";
        wait until rising_edge(tb_clk_r);
        wait until rising_edge(tb_clk_r);
        assert tb_data_r = X"55"
            report "T6 FAIL: overwrite failed; expected 0x55, got 0x"
                   & integer'image(conv_integer(tb_data_r)) severity failure;
        report "T6 PASSED: overwrite of addr 0 gives 0x55" severity note;

        -- ----------------------------------------------------------------
        -- All tests passed
        -- ----------------------------------------------------------------
        report "TEST PASSED!" severity note;
        tb_test_passed <= true;
        wait until rising_edge(tb_clk_w);
        finish;

    end process;

end Testbench;
