library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use std.env.finish;
use work.vis_vga_pkg.all;

entity TB_vis_vga_ctrl is
end TB_vis_vga_ctrl;

architecture Testbench of TB_vis_vga_ctrl is

    signal tb_vga_clk        : std_logic := '0';
    signal tb_reset          : std_logic := '1';
    signal tb_enable         : std_logic := '0';
    signal tb_h_sync         : std_logic;
    signal tb_v_sync         : std_logic;
    signal tb_blank          : std_logic;
    signal tb_fdp            : std_logic;
    signal tb_red            : std_logic_vector(3 downto 0);
    signal tb_green          : std_logic_vector(3 downto 0);
    signal tb_blue           : std_logic_vector(3 downto 0);
    signal tb_char_clk       : std_logic := '0';
    signal tb_char_write_en  : std_logic := '0';
    signal tb_char_address_x : std_logic_vector(c_CHR_ADDR_BUS_W_X - 1 downto 0) := (others => '0');
    signal tb_char_address_y : std_logic_vector(c_CHR_ADDR_BUS_W_Y - 1 downto 0) := (others => '0');
    signal tb_char_data      : std_logic_vector(c_CHR_ASCII_DATA_BUS_W - 1 downto 0) := (others => '0');
    signal tb_char_col_red   : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0) := (others => '0');
    signal tb_char_col_green : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0) := (others => '0');
    signal tb_char_col_blue  : std_logic_vector(c_CHR_COLOR_BIT_DEPTH_W - 1 downto 0) := (others => '0');

    signal tb_test_passed : boolean := false;

begin

    DUT: entity work.vis_vga_ctrl
        port map (
            i_vga_clk                  => tb_vga_clk,
            i_reset                    => tb_reset,
            i_enable                   => tb_enable,
            o_h_sync                   => tb_h_sync,
            o_v_sync                   => tb_v_sync,
            o_blank                    => tb_blank,
            o_visible_frame_done_pulse => tb_fdp,
            o_red                      => tb_red,
            o_green                    => tb_green,
            o_blue                     => tb_blue,
            i_char_clk                 => tb_char_clk,
            i_char_write_en            => tb_char_write_en,
            i_char_address_x           => tb_char_address_x,
            i_char_address_y           => tb_char_address_y,
            i_char_data                => tb_char_data,
            i_char_col_red             => tb_char_col_red,
            i_char_col_green           => tb_char_col_green,
            i_char_col_blue            => tb_char_col_blue
        );

    -- 25 MHz pixel clock (40 ns period)
    tb_vga_clk  <= not tb_vga_clk  after 20 ns;
    tb_char_clk <= not tb_char_clk after 20 ns;

    STIMULUS: process
        variable v_count     : integer;
        variable v_fdp_count : integer;
        variable v_cyc       : integer;
    begin

        -- ----------------------------------------------------------------
        -- T1: All outputs must be in safe reset state while reset is high
        -- ----------------------------------------------------------------
        tb_reset  <= '1';
        tb_enable <= '0';

        wait until rising_edge(tb_vga_clk); -- allow DUT to register reset state

        for i in 1 to 5 loop
            wait until falling_edge(tb_vga_clk); -- sample mid-cycle
            assert tb_h_sync = '1'
                report "T1 FAIL: h_sync not '1' during reset" severity failure;
            assert tb_v_sync = '1'
                report "T1 FAIL: v_sync not '1' during reset" severity failure;
            assert tb_blank = '1'
                report "T1 FAIL: blank not '1' during reset" severity failure;
            assert tb_red = "0000" and tb_green = "0000" and tb_blue = "0000"
                report "T1 FAIL: RGB not zero during reset" severity failure;
            assert tb_fdp = '0'
                report "T1 FAIL: FDP not '0' during reset" severity failure;
        end loop;
        report "T1 PASSED: reset state correct" severity note;

        -- ----------------------------------------------------------------
        -- T2: blank goes low at display area start; RGB=0 with enable='0'
        -- ----------------------------------------------------------------
        tb_reset <= '0';
        -- enable remains '0' for this test

        wait until rising_edge(tb_vga_clk); -- cycle 0: blank still '1' (from reset register)
        wait until rising_edge(tb_vga_clk); -- cycle 1: blank registered from display area (h=0, v=0)

        assert tb_blank = '0'
            report "T2 FAIL: blank not '0' at start of display area after reset" severity failure;
        assert tb_red = "0000" and tb_green = "0000" and tb_blue = "0000"
            report "T2 FAIL: RGB not zero in display area with enable='0'" severity failure;
        report "T2 PASSED: blank low and RGB=0 in display area with enable off" severity note;

        -- ----------------------------------------------------------------
        -- T3: h_sync timing — transition, pulse width, blank and RGB during pulse
        -- ----------------------------------------------------------------
        tb_enable <= '1';

        -- Wait for h_sync to go low; must happen within one H period
        v_count := 0;
        while tb_h_sync = '1' loop
            wait until rising_edge(tb_vga_clk);
            v_count := v_count + 1;
            assert v_count <= c_H_PERIODpx
                report "T3 FAIL: h_sync did not go low within one H period" severity failure;
        end loop;
        report "T3: h_sync went low after " & integer'image(v_count) & " cycles" severity note;

        -- Count pulse width; check blank='1' and RGB=0 on every cycle of the active pulse
        v_count := 0;
        while tb_h_sync = '0' loop
            assert tb_blank = '1'
                report "T3 FAIL: blank not '1' during h_sync active" severity failure;
            assert tb_red = "0000" and tb_green = "0000" and tb_blue = "0000"
                report "T3 FAIL: RGB not zero during blank (h_sync active)" severity failure;
            v_count := v_count + 1;
            assert v_count <= c_H_SYNCTIMEpx
                report "T3 FAIL: h_sync pulse longer than " & integer'image(c_H_SYNCTIMEpx) & " cycles" severity failure;
            wait until rising_edge(tb_vga_clk);
        end loop;

        assert v_count = c_H_SYNCTIMEpx
            report "T3 FAIL: h_sync pulse width = " & integer'image(v_count)
                   & ", expected " & integer'image(c_H_SYNCTIMEpx) severity failure;
        report "T3 PASSED: h_sync pulse = " & integer'image(v_count)
               & " cycles, blank='1' and RGB=0 throughout" severity note;

        -- ----------------------------------------------------------------
        -- T4: Frame-done pulse fires at least twice over 2 full frames,
        --     and each pulse is exactly 1 clock wide
        -- ----------------------------------------------------------------
        v_fdp_count := 0;
        v_cyc       := 0;

        while v_cyc <= 2 * c_H_PERIODpx * c_V_PERIODln loop
            wait until rising_edge(tb_vga_clk);
            v_cyc := v_cyc + 1;

            if tb_fdp = '1' then
                v_fdp_count := v_fdp_count + 1;
                -- FDP must drop back to '0' on the very next clock
                wait until rising_edge(tb_vga_clk);
                v_cyc := v_cyc + 1;
                assert tb_fdp = '0'
                    report "T4 FAIL: FDP pulse wider than 1 clock (pulse #"
                           & integer'image(v_fdp_count) & ")" severity failure;
            end if;
        end loop;

        assert v_fdp_count >= 2
            report "T4 FAIL: expected >= 2 FDP pulses in 2 frames, got "
                   & integer'image(v_fdp_count) severity failure;
        report "T4 PASSED: FDP fired " & integer'image(v_fdp_count)
               & " times, each 1 clock wide" severity note;

        -- ----------------------------------------------------------------
        -- All tests passed
        -- ----------------------------------------------------------------
        report "TEST PASSED!" severity note;
        tb_test_passed <= true;
        wait until rising_edge(tb_vga_clk);
        finish;

    end process;

end Testbench;
