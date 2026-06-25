library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use std.env.finish;
use work.vis_vga_pkg.all;

entity TB_vis_charmaps_ROM is
end TB_vis_charmaps_ROM;

architecture Testbench of TB_vis_charmaps_ROM is

    signal tb_en    : std_logic := '0';
    signal tb_clock : std_logic := '0';
    signal tb_addr  : std_logic_vector(c_INTCHMAP_ADDR_BUS_W - 1 downto 0) := (others => '0');
    signal tb_data  : std_logic_vector(c_INTCHMAP_DATA_BUS_W - 1 downto 0);

    signal tb_test_passed : boolean := false;

    -- ROM address encoding: addr = ascii[6:0] & row[3:0]
    -- Character 'A' = ASCII 0x41 = 65 → base address 65*16 = 1040
    constant c_CHAR_A_BASE : integer := 65 * 16;

    -- Reference bitmap for 'A' rows 0-15 (from ROM constant)
    type t_char_rows is array (0 to 15) of std_logic_vector(7 downto 0);
    constant c_CHAR_A_ROWS : t_char_rows := (
        X"00", X"00", X"10", X"28", X"28", X"28", X"44", X"7C",
        X"44", X"44", X"82", X"82", X"82", X"00", X"00", X"00"
    );

begin

    DUT: entity work.vis_charmaps_ROM
        port map (
            i_EN    => tb_en,
            i_clock => tb_clock,
            i_ADDR  => tb_addr,
            o_DO    => tb_data
        );

    tb_clock <= not tb_clock after 20 ns;

    STIMULUS: process
    begin
        -- ROM is synchronous: output is registered on rising edge when EN='1'.
        -- Read pattern: present address, wait one rising edge (ROM latches it),
        -- wait a second rising edge (output now stable), then assert.

        -- ----------------------------------------------------------------
        -- T1: Basic read with EN='1' - address 0 (NUL char, row 0) → 0x00
        -- ----------------------------------------------------------------
        tb_en   <= '1';
        tb_addr <= (others => '0');
        wait until rising_edge(tb_clock);  -- ROM latches addr=0
        wait until rising_edge(tb_clock);  -- output stable
        assert tb_data = X"00"
            report "T1 FAIL: ROM[0] expected 0x00, got 0x"
                   & integer'image(conv_integer(tb_data)) severity failure;
        report "T1 PASSED: ROM[0] = 0x00 (NUL char row 0)" severity note;

        -- ----------------------------------------------------------------
        -- T2: EN='0' - output must freeze when enable is de-asserted
        -- ----------------------------------------------------------------
        tb_en   <= '0';
        tb_addr <= conv_std_logic_vector(c_CHAR_A_BASE + 7, c_INTCHMAP_ADDR_BUS_W);
        wait until rising_edge(tb_clock);  -- EN='0': ROM must not update
        wait until rising_edge(tb_clock);
        assert tb_data = X"00"
            report "T2 FAIL: output changed while EN='0', got 0x"
                   & integer'image(conv_integer(tb_data)) severity failure;
        report "T2 PASSED: output frozen at 0x00 while EN='0'" severity note;

        -- ----------------------------------------------------------------
        -- T3: Re-enable - output must update to pending address (row 7 of 'A')
        -- ----------------------------------------------------------------
        tb_en <= '1';
        -- tb_addr still = c_CHAR_A_BASE + 7 (row 7 of 'A')
        wait until rising_edge(tb_clock);  -- ROM latches address
        wait until rising_edge(tb_clock);  -- output stable
        assert tb_data = X"7C"
            report "T3 FAIL: ROM[1047] expected 0x7C ('A' row 7), got 0x"
                   & integer'image(conv_integer(tb_data)) severity failure;
        report "T3 PASSED: output updated to 0x7C after re-enable ('A' row 7)" severity note;

        -- ----------------------------------------------------------------
        -- T3b: Last valid address (2047) → 0x00
        -- ----------------------------------------------------------------
        tb_addr <= conv_std_logic_vector(2047, c_INTCHMAP_ADDR_BUS_W);
        wait until rising_edge(tb_clock);
        wait until rising_edge(tb_clock);
        assert tb_data = X"00"
            report "T3b FAIL: ROM[2047] expected 0x00, got 0x"
                   & integer'image(conv_integer(tb_data)) severity failure;
        report "T3b PASSED: ROM[2047] = 0x00 (boundary address)" severity note;

        -- ----------------------------------------------------------------
        -- T4: Sequential read of all 16 rows of character 'A'
        --     Exploits the 1-cycle pipeline: check row N at the edge that
        --     latches row N+1 (both processes see pre-edge signal values).
        -- ----------------------------------------------------------------
        for row in 0 to 15 loop
            tb_addr <= conv_std_logic_vector(c_CHAR_A_BASE + row, c_INTCHMAP_ADDR_BUS_W);
            wait until rising_edge(tb_clock);
            if row > 0 then
                assert tb_data = c_CHAR_A_ROWS(row - 1)
                    report "T4 FAIL: 'A' row " & integer'image(row - 1)
                           & " expected 0x" & integer'image(conv_integer(c_CHAR_A_ROWS(row - 1)))
                           & " got 0x" & integer'image(conv_integer(tb_data)) severity failure;
            end if;
        end loop;
        -- Drain pipeline: check final row (row 15)
        wait until rising_edge(tb_clock);
        assert tb_data = c_CHAR_A_ROWS(15)
            report "T4 FAIL: 'A' row 15 expected 0x00, got 0x"
                   & integer'image(conv_integer(tb_data)) severity failure;
        report "T4 PASSED: all 16 rows of 'A' match reference bitmap" severity note;

        -- ----------------------------------------------------------------
        -- All tests passed
        -- ----------------------------------------------------------------
        report "TEST PASSED!" severity note;
        tb_test_passed <= true;
        wait until rising_edge(tb_clock);
        finish;

    end process;

end Testbench;
