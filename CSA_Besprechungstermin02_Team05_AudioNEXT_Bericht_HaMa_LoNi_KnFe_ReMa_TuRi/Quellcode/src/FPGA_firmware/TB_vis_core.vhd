library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use std.env.finish;
use work.vis_vga_pkg.all;

entity TB_vis_core is
end TB_vis_core;

architecture Testbench of TB_vis_core is

    -- i_clk: 100 MHz (AXI/char domain); i_pixel_clk: 25 MHz (VGA domain)
    signal tb_clk       : std_logic := '0';
    signal tb_pixel_clk : std_logic := '0';

    signal tb_rst       : std_logic := '1';
    signal tb_vga_en    : std_logic := '0';
    signal tb_buf_valid : std_logic := '0';
    signal tb_addr_x    : std_logic_vector(c_CHR_ADDR_BUS_W_X     - 1 downto 0) := (others => '0');
    signal tb_addr_y    : std_logic_vector(c_CHR_ADDR_BUS_W_Y     - 1 downto 0) := (others => '0');
    signal tb_ascii     : std_logic_vector(c_CHR_ASCII_DATA_BUS_W - 1 downto 0) := (others => '0');
    signal tb_col_r     : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0) := (others => '0');
    signal tb_col_g     : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0) := (others => '0');
    signal tb_col_b     : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0) := (others => '0');

    signal tb_buf_ready : std_logic;
    signal tb_fdp       : std_logic;
    signal tb_h_sync    : std_logic;
    signal tb_v_sync    : std_logic;
    signal tb_red       : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
    signal tb_green     : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);
    signal tb_blue      : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0);

    signal tb_test_passed : boolean := false;

begin

    DUT: entity work.vis_core
        port map (
            i_clk                      => tb_clk,
            i_rst                      => tb_rst,
            i_buf_valid                => tb_buf_valid,
            i_buf_addr_x               => tb_addr_x,
            i_buf_addr_y               => tb_addr_y,
            i_buf_char_ascii           => tb_ascii,
            i_buf_color_red            => tb_col_r,
            i_buf_color_green          => tb_col_g,
            i_buf_color_blue           => tb_col_b,
            o_buf_ready                => tb_buf_ready,
            i_vga_enable               => tb_vga_en,
            o_visible_frame_done_pulse => tb_fdp,
            i_pixel_clk                => tb_pixel_clk,
            o_hsync                    => tb_h_sync,
            o_vsync                    => tb_v_sync,
            o_red                      => tb_red,
            o_green                    => tb_green,
            o_blue                     => tb_blue
        );

    tb_clk       <= not tb_clk       after 5 ns;   -- 100 MHz
    tb_pixel_clk <= not tb_pixel_clk after 20 ns;  -- 25 MHz

    STIMULUS: process
        variable v_count : integer;
        variable v_pulse : integer;
    begin
        -- ----------------------------------------------------------------
        -- T1: Reset propagates via CDC IN→VGA (2 pixel-clock stages)
        --     After > 2 pixel-clock cycles, vis_vga_ctrl must be in reset:
        --     h_sync = '1', v_sync = '1'
        -- ----------------------------------------------------------------
        tb_rst    <= '1';
        tb_vga_en <= '0';

        -- Three pixel-clock edges guarantee CDC has asserted s_vga_rst
        wait until rising_edge(tb_pixel_clk);
        wait until rising_edge(tb_pixel_clk);
        wait until rising_edge(tb_pixel_clk);

        wait until falling_edge(tb_pixel_clk);  -- sample mid-cycle: outputs stable after last rising edge

        assert tb_h_sync = '1'
            report "T1 FAIL: h_sync not '1' after CDC reset propagation" severity failure;
        assert tb_v_sync = '1'
            report "T1 FAIL: v_sync not '1' after CDC reset propagation" severity failure;
        report "T1 PASSED: reset propagates via CDC to VGA domain" severity note;

        -- ----------------------------------------------------------------
        -- T2: buf_ready goes low once VGA leaves reset and enters display area
        --     CDC path: i_rst=0 -> s_vga_rst=0 (2 px clks) -> blank=0 (1 px clk)
        --                       -> s_cdc_blank=0 (1 i_clk) -> buf_ready=0 (1 i_clk)
        --     Worst-case: 3 px clks x 40 ns + 2 i_clks x 10 ns = 140 ns.
        --     Waiting 30 i_clk cycles = 300 ns >> 140 ns.
        -- ----------------------------------------------------------------
        tb_rst    <= '0';
        tb_vga_en <= '1';

        for i in 1 to 30 loop
            wait until rising_edge(tb_clk);
        end loop;

        assert tb_buf_ready = '0'
            report "T2 FAIL: buf_ready not '0' after CDC settling in display area" severity failure;
        report "T2 PASSED: buf_ready='0' in display area (CDC VGA->IN path)" severity note;

        -- ----------------------------------------------------------------
        -- T3: h_sync timing - confirms CDC enable (IN->VGA) path works
        --     Pulse width must equal c_H_SYNCTIMEpx = 96 pixel clocks.
        -- ----------------------------------------------------------------
        v_count := 0;
        while tb_h_sync = '1' loop
            wait until rising_edge(tb_pixel_clk);
            v_count := v_count + 1;
            assert v_count <= c_H_PERIODpx
                report "T3 FAIL: h_sync did not go low within one H period" severity failure;
        end loop;
        report "T3: h_sync went low after " & integer'image(v_count) & " pixel clocks" severity note;

        v_count := 0;
        while tb_h_sync = '0' loop
            v_count := v_count + 1;
            assert v_count <= c_H_SYNCTIMEpx
                report "T3 FAIL: h_sync pulse too long" severity failure;
            wait until rising_edge(tb_pixel_clk);
        end loop;
        assert v_count = c_H_SYNCTIMEpx
            report "T3 FAIL: h_sync pulse = " & integer'image(v_count)
                   & " px clks, expected " & integer'image(c_H_SYNCTIMEpx) severity failure;
        report "T3 PASSED: h_sync pulse = " & integer'image(v_count)
               & " pixel clocks (CDC enable path confirmed)" severity note;

        -- ----------------------------------------------------------------
        -- T4: buf_ready='1' during porch region (just exited h_sync active)
        --     blank='1' since back-porch start (~49 px clks ago); CDC delay
        --     (2 x 10 ns = 20 ns) has long elapsed.  A 3-cycle i_clk guard
        --     covers any remaining metastability settling.
        -- ----------------------------------------------------------------
        wait until rising_edge(tb_clk);
        wait until rising_edge(tb_clk);
        wait until rising_edge(tb_clk);

        assert tb_buf_ready = '1'
            report "T4 FAIL: buf_ready not '1' in porch region" severity failure;
        report "T4 PASSED: buf_ready='1' in porch region (CDC blank path)" severity note;

        -- ----------------------------------------------------------------
        -- T5: o_visible_frame_done_pulse appears on i_clk domain
        --     FDP fires at pixel (639, 479) ≈ 383 840 px clks from frame
        --     start (~15.4 ms).  CDC stretches the raw 1-px-clk pulse by
        --     at most 4 i_clk cycles (1 px clk = 4 i_clk at 100/25 MHz).
        -- ----------------------------------------------------------------
        -- Wait for first FDP (up to 20 ms covers the first frame comfortably)
        wait until tb_fdp = '1' for 20 ms;
        assert tb_fdp = '1'
            report "T5 FAIL: FDP did not appear on i_clk domain within 20 ms" severity failure;

        -- Verify the pulse clears within a bounded number of i_clk cycles
        v_pulse := 0;
        while tb_fdp = '1' loop
            wait until rising_edge(tb_clk);
            v_pulse := v_pulse + 1;
            assert v_pulse <= 8
                report "T5 FAIL: FDP stuck high for more than 8 i_clk cycles" severity failure;
        end loop;
        report "T5: FDP pulse width = " & integer'image(v_pulse) & " i_clk cycles" severity note;

        -- Wait for second FDP (one full frame later, ≈ 16.8 ms < 20 ms timeout)
        wait until tb_fdp = '1' for 20 ms;
        assert tb_fdp = '1'
            report "T5 FAIL: second FDP did not appear within 20 ms" severity failure;
        report "T5 PASSED: FDP fires and propagates to i_clk domain" severity note;

        -- ----------------------------------------------------------------
        -- All tests passed
        -- ----------------------------------------------------------------
        report "TEST PASSED!" severity note;
        tb_test_passed <= true;
        wait until rising_edge(tb_clk);
        finish;

    end process;

end Testbench;
