////////////////////////////////////////////////////////
//
// 
//
////////////////////////////////////////////////////////

`timescale 1ns / 1ps
 
import axi_vip_pkg::*;
import vip_db_axi_vip_0_0_pkg::*; // check instance name in block design <blockdesign-name>_<vip-inst-name>_0_pkg)


module vis_tb();

  //CONSTANT DEFINITIONS
  parameter integer C_S_AXI_CONTROL_ADDR_WIDTH = 12;
  parameter integer C_S_AXI_CONTROL_DATA_WIDTH = 32;
    
  //REGISTER DEFINITIONS
  // EDIT CODE BEGIN Markus Remy
  //GSCR General/Global Control and Status Register slv_reg00 0x00 
  parameter GSCR_ADDR              = 6'h000;
  //GIER slv_reg01 0x04  
  parameter GIER_ADDR              = 6'h004;
  parameter GIER_GIE_MASK          = 32'h00000001;    
  //IPIER slv_reg02 0x08  
  parameter IPIER_ADDR             = 6'h008;
  parameter IPIER_FIE_MASK         = 32'h00000001;  
  //IPISR slv_reg03 0x0C  
  parameter IPISR_ADDR             = 6'h00C;
  parameter IPISR_FDP_MASK         = 32'h00000001;  
  //IDR ID Register slv_reg04 0x10  --const axi r ip rw (part3)
  parameter IDR_ADDR               = 6'h010;
  //VERR Version Register slv_reg05 0x14 --const axi r ip rw (part3)
  parameter VERR_ADDR              = 6'h014;
  //CTRL Control Register slv_reg06 0x18 
  parameter CTRL_ADDR              = 6'h018;
  parameter CTRL_VEN_MASK          = 32'h00000001;
  parameter CTRL_WD_MASK           = 32'h00000100;
  //STATUS Status Register slv_reg07 0x1C 
  parameter STATUS_ADDR            = 6'h01C;
  parameter STATUS_FDP_MASK        = 32'h00000001;
  //ADDRR Address Register slv_reg08 0x20 
  parameter ADDRR_ADDR             = 6'h020;
  parameter ADDRR_XA_MASK          = 32'h000000EF;
  parameter ADDRR_YA_MASK          = 32'h00001F00;
  //VDATR Visualization Data Register slv_reg09 0x24
  parameter VDATR_ADDR             = 6'h024;
  parameter VDATR_CHAR_MASK        = 32'h000000EF;
  //COLR Color Register slv_reg10 0x28
  parameter COLR_ADDR              = 6'h028;
  parameter COLR_CR_MASK           = 32'h0000000F;
  parameter COLR_CG_MASK           = 32'h00000F00;
  parameter COLR_CB_MASK           = 32'h000F0000;
  // EDIT CODE END Markus Remy
  
  //BUFFER DEFINTIONS (not used so far)

  //FURTHER DEFINITIONS
  vip_db_axi_vip_0_0_mst_t  mst_ctrl_agent; // check instance name in block design (<blockdesign-name>_<vip-inst-name>_0_mst_t)
  //clock frequency definition
  parameter real CLK_PERIOD = 1; // for ease of use in the waveform diagram the clock period is selected as 1ns
  bit error_found = 0;


  //--------------------------------------------------------------------------------------
  //SYSTEM DEFINTION (system instances and signals)
  //CLK
  logic ap_clk = 0;
  initial begin: AP_CLK
    forever begin
      ap_clk = #(CLK_PERIOD/2) ~ap_clk;
    end
  end

  //RESET
  logic ap_rst_n = 0;
  initial begin: AP_RST
    ap_rst_n_sequence(16);
  end

  //SYSTEM DESIGN WRAPPER instance
  // EDIT CODE BEGIN Markus Remy
  logic i_pixel_clk;
  logic o_hsync;
  logic o_vsync;
  logic [3:0] o_red;
  logic [3:0] o_green;
  logic [3:0] o_blue;
  logic o_interrupt;

  //check instance name in block design -> <blockdeisgn_name>_wrapper 
  vip_db_wrapper DUT( // TODO: Check signals!
   .aclk(ap_clk),
   .aresetn(ap_rst_n),
   .o_t0_out(o_t0_out),
   .interrupt_0(o_interrupt)
  );
  // EDIT CODE END Markus Remy
 
  //-------------------------------------------------------------------------------------
  //FUNCTIONS AND TASKS
  /////////////////////////////////////////////////////////////
  // Reusing and adapting AXI VIP functions out of Xilinx Tutorial
  //
  //https://github.com/Xilinx/Vitis-Tutorials/blob/2023.2/Hardware_Acceleration/Feature_Tutorials/01-rtl_kernel_workflow/reference-files/src/testbench/Vadd_A_B_tb.sv   
  //
  ////////////////////////////////////////////////////////////  
  
  //`include "vis_tb.vh"
  
  //FUNCTIONS AND TASKS
  task automatic ap_rst_n_sequence(input integer unsigned width = 20);
    @(posedge ap_clk);
    #1ns;
    ap_rst_n = 0;
    repeat (width) @(posedge ap_clk);
    #1ns;
    ap_rst_n = 1;
  endtask

  task automatic start_vips();
    $display("//////////////////////////////////////////////////////////////");
    $display("Start Axi Control Master");    
    mst_ctrl_agent = new("master_ctrl_agent", DUT.vip_db_i.axi_vip_0.inst.IF); // check instance name in block design DUT.<blockdesign-name>_<vip-inst-name>.inst.IF      
    mst_ctrl_agent.start_master();
  endtask

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Control interface blocking write
  // The task will return when the BRESP has been returned from the kernel.
  task automatic blocking_write_register (input bit [31:0] addr_in, input bit [31:0] data);
    axi_transaction   wr_xfer;
    axi_transaction   wr_rsp;
    wr_xfer = mst_ctrl_agent.wr_driver.create_transaction("wr_xfer");
    wr_xfer.set_driver_return_item_policy(XIL_AXI_PAYLOAD_RETURN);
    assert(wr_xfer.randomize() with {addr == addr_in;});
    wr_xfer.set_data_beat(0, data);
    mst_ctrl_agent.wr_driver.send(wr_xfer);
    mst_ctrl_agent.wr_driver.wait_rsp(wr_rsp);
  endtask

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Control interface blocking read
  // The task will return when the BRESP has been returned from the kernel.
  task automatic read_register (input bit [31:0] addr, output bit [31:0] rddata);
    axi_transaction   rd_xfer;
    axi_transaction   rd_rsp;
    bit [31:0] rd_value;
    rd_xfer = mst_ctrl_agent.rd_driver.create_transaction("rd_xfer");
    rd_xfer.set_addr(addr);
    rd_xfer.set_driver_return_item_policy(XIL_AXI_PAYLOAD_RETURN);
    mst_ctrl_agent.rd_driver.send(rd_xfer);
    mst_ctrl_agent.rd_driver.wait_rsp(rd_rsp);
    rd_value = rd_rsp.get_data_beat(0);
    rddata = rd_value;
  endtask

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Poll the Control interface status register.
  // This will poll until the DONE flag in the status register is asserted.
  task automatic poll_done_register ();
    bit [31:0] rd_value;
    do begin
      read_register(GSCR_ADDR, rd_value);
    end while ((rd_value & GSCR_DONE_MASK) == 0);
  endtask
 
  task automatic poll_idle_register ();
    bit [31:0] rd_value;
    do begin
      read_register(GSCR_ADDR, rd_value);
    end while ((rd_value & GSCR_IDLE_MASK) == 0);
  endtask

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //check if only the implemented bit can be written and read back 
  //(unimplemented reserved bits should ignore writes and return zeros if read)
  task automatic check_32bitregister_value_with_gaps(input bit [31:0] addr_in, input bit [31:0] expectedreadregvalwriteff, input bit [31:0] expectedreadregvalwrite00, output bit error_found);
    bit [31:0] rddata;
    error_found = 0;

    blocking_write_register(addr_in, 32'hffffffff);
    read_register(addr_in, rddata);
    if (rddata != expectedreadregvalwriteff) begin
      $error("Value mismatch expectedreadregvalwriteff: A:0x%0x : Expected 0x%x -> Got 0x%x", addr_in, expectedreadregvalwriteff, rddata);
      error_found = 1;
    end
    blocking_write_register(addr_in, 32'h00000000);
    read_register(addr_in, rddata);
    if (rddata != expectedreadregvalwrite00 ) begin
      $error("Value mismatch expectedreadregvalwrite00: A:0x%0x : Expected 0x%x -> Got 0x%x", addr_in, expectedreadregvalwrite00, rddata);
      error_found = 1;
    end    
  endtask
  
  /////////////////////////////////////////////////////////////////////////////////////////////////
  // For each of the scalar registers, check:
  //  correct number bits set on a write 
  task automatic check_scalar_registers(output bit error_found);
    bit [31:0] expectedval = 32'h00000000;
    bit tmp_error_found = 0;
    
    error_found = 0;    
    $display("%t : Checking scalar registers", $time);

    // EDIT CODE BEGIN Markus Remy
    check_32bitregister_value_with_gaps (GSCR_ADDR, 32'h00000000, 32'h00000000, tmp_error_found);
    expectedval=0;
    expectedval=GIER_GIE_MASK;
    check_32bitregister_value_with_gaps (GIER_ADDR, expectedval, 32'h00000000, tmp_error_found);
    error_found |= tmp_error_found;
    expectedval=0;
    expectedval=IPIER_FIE_MASK;
    check_32bitregister_value_with_gaps (IPIER_ADDR, expectedval, 32'h00000000, tmp_error_found);
    error_found |= tmp_error_found;
    expectedval=0;
    expectedval=IPISR_FDP_MASK;
    check_32bitregister_value_with_gaps (IPISR_ADDR, expectedval, expectedval, tmp_error_found); //write 1 to toggle!
    check_32bitregister_value_with_gaps (IPISR_ADDR, 32'h00000000, 32'h00000000, tmp_error_found); //write 1 to toggle!
    error_found |= tmp_error_found;
    check_32bitregister_value_with_gaps (IDR_ADDR, 32'h0000D15C, 32'h0000D15C, tmp_error_found);
    error_found |= tmp_error_found;
    check_32bitregister_value_with_gaps (VERR_ADDR, 32'h00000001, 32'h00000001, tmp_error_found);
    error_found |= tmp_error_found;
    // EDIT CODE END Markus Remy
    // USER CODE BEGIN Markus Remy
    expectedval=0;
    expectedval=CTRL_WD_MASK | CTRL_WD_MASK;
    check_32bitregister_value_with_gaps (CTRL_ADDR, expectedval, 32'h00000000, tmp_error_found); // WD could be 1 or 0 depending on the current blanking status
    error_found |= tmp_error_found;
    check_32bitregister_value_with_gaps (STATUS_ADDR, 32'h00000000, 32'h00000000, tmp_error_found); // FDP could be 0 or 1 depending on the current frame status
    error_found |= tmp_error_found;
    expectedval=0;
    expectedval=ADDRR_XA_MASK | ADDRR_YA_MASK;
    check_32bitregister_value_with_gaps (ADDRR, expectedval, 32'h00000000, tmp_error_found);
    error_found |= tmp_error_found;
    expectedval=0;
    expectedval=VDATR_CHAR_MASK;
    check_32bitregister_value_with_gaps (VDATR, expectedval, 32'h00000000, tmp_error_found);
    error_found |= tmp_error_found;
    expectedval=0;
    expectedval=COLR_CR_MASK | COLR_CG_MASK | COLR_CB_MASK;
    check_32bitregister_value_with_gaps (COLR_ADDR, expectedval, 32'h00000000, tmp_error_found);
    error_found |= tmp_error_found;  
    // USER CODE END Markus Remy
  endtask
 
  task automatic wait_for_interrupt;
    wait(o_interrupt==1'b1);
  endtask
  
  
  //---------------------------------------------------------------------------------- 
  //TEST TASKS 
  
  task automatic CHECK_REGISTERS;  
    $display("---------------------------------------------------------------");
    $display("START TEST Check Registers");
    $display("---------------------------------------------------------------"); 
    check_scalar_registers(error_found);
    if(error_found == 1) begin
      $display( "Test Failed at Check Registers!");
      $finish();
    end else begin
      $display( "Test Check Registers ... OK");      
    end 
  endtask
  
  
  task automatic TEST_UP;  
    //const bit [31:0] c_cnt_limit = 32'h00000004;
    const bit [31:0] c_cnt_limit = 32'h00000100;
    bit [31:0] CR0_is =0;
    bit [31:0] GSCR_is =0;
  
    $display("---------------------------------------------------------------");
    $display(" START TEST_UP");
    $display("---------------------------------------------------------------");
  
    blocking_write_register(LR0_ADDR, c_cnt_limit);
    
    //TB_ud0 <='1'; (5)
    //TB_load0 <='0'; (4)
    //TB_ent0_out <='1'; (2)    
    blocking_write_register(SCSR_ADDR, 32'h00000024);
        
    //TB_auto_restart<='0'; (7)
    //TB_ap_start <='1'; (0)
    blocking_write_register(GSCR_ADDR, GSCR_START_MASK);    
   
   
    poll_done_register();
   
    
    #10
    read_register(GSCR_ADDR, GSCR_is); //reset ap_done (superflous since already accomplished in poll_done_register(), 
                                       //here just for check result)
    
    //RESULT CHECK (not sufficent)
    //read_register(CR0_ADDR, CR0_is);
    //if(CR0_is == c_cnt_limit) begin 
    //  $display(" RESULT DATA CHECK SUCCEED!!");
    //end else begin
    //  $display(" RESULT DATA CHECK FAILED!!");
    //  $display("  -- MISMATCH at  exp = %x, act = %x", c_cnt_limit, CR0_is);
    //end 
    
    //MANUAL CHECK IN WAVEFORM 
    //CR0_reg should reach value of LR0_reg (limit value);
    //ap_done(GSCR_reg(1)) should be set
    //ap_start(GSCR_reg(0)) should be reset by ap_done(GSCR_reg(1)) 
    //o_t0_out should be toggled by ap_done(GSCR_reg(1))
    //ap_done(GSCR_reg(1)) should be reset after done_ack (read of GSCR_reg)
        
    #10;     
  endtask

 
  task automatic TEST_UP_AUTORESTART;  
    //const bit [31:0] c_cnt_limit = 32'h00000004;
    //const bit [31:0] c_cnt_limit = 32'h00000100;
    const bit [31:0] c_cnt_limit = 32'h0000000A;
    bit [31:0] CR_is =0;
  
    $display("---------------------------------------------------------------");
    $display(" START TEST_UP_AUTORESTART");
    $display("---------------------------------------------------------------");
  
    blocking_write_register(LR0_ADDR, c_cnt_limit);
    
    //TB_ud0 <='1'; (5)
    //TB_load0 <='0'; (4)
    //TB_ent0_out <='1'; (2)    
    blocking_write_register(SCSR_ADDR, 32'h00000024);
        
    //TB_auto_restart<='1'; (7)    
    //TB_ap_start <='1'; (0)
    blocking_write_register(GSCR_ADDR, 32'h00000081);    
   
    #100;
   
   
    poll_done_register();


    //RESULT CHECK (not sufficent)
    //read_register(CR0_ADDR, CR_is);
    //if(CR_is == c_cnt_limit) begin 
    //  $display(" RESULT DATA CHECK SUCCEED!!");
    //end else begin
    //  $display(" RESULT DATA CHECK FAILED!!");
    //  $display("  -- MISMATCH at  exp = %x, act = %x", c_cnt_limit, CR_is);
    //end 
    
    //MANUAL RESULT CHECK IN WAVEFORM 
    //CR0_reg should reach value of LR0_reg (limit value);
    //ap_done(GSCR_reg(1)) should be set
    //ap_start(GSCR_reg(0)) should be reset by ap_done(GSCR_reg(1)) 
    //o_t0_out should be toggled by ap_done(GSCR_reg(1))
    //ap_done(GSCR_reg(1)) should be reset after done_ack (read of GSCR_reg)
    //repeating:       
      //CR0_reg should reach value of LR0_reg (limit value);
      //ap_done(GSCR_reg(1)) should be set
      //ap_start(GSCR_reg(0)) should be reset by ap_done(GSCR_reg(1)) 
      //o_t0_out should be toggled by ap_done(GSCR_reg(1)) (oscillating with LR0_reg amount clks)
      //ap_done(GSCR_reg(1)) should be reset after done_ack (read of GSCR_reg)   
          
    #10;   
  endtask
  
  
  task automatic TEST_UP_INT;  
  //##INSERT YOUR CODE HERE 
    //const bit [31:0] c_cnt_limit = 32'h00000004;
    const bit [31:0] c_cnt_limit = 32'h00000100;
    bit [31:0] CR0_is =0;
    bit [31:0] GSCR_is =0;
  
    $display("---------------------------------------------------------------");
    $display(" START TEST_UP_INT");
    $display("---------------------------------------------------------------");
  
    blocking_write_register(LR0_ADDR, c_cnt_limit);
    
    //TB_ud0 <='1'; (5)
    //TB_load0 <='0'; (4)
    //TB_ent0_out <='1'; (2)       
    blocking_write_register(SCSR_ADDR, 32'h00000024);
    
    blocking_write_register(IPIER_ADDR, IPIER_IPIE_MASK);
    
    blocking_write_register(GIER_ADDR, GIER_GIE_MASK);
           
    //TB_auto_restart<='0'; (7)    
    //TB_ap_start <='1'; (0)
    blocking_write_register(GSCR_ADDR, GSCR_START_MASK);    
   

    //poll_done_register();
    //wait(interrupt==1'b1);
    wait_for_interrupt();
     
    
    #10
    
    read_register(GSCR_ADDR, GSCR_is); //reset done (required here!)
      
    //clear interrupt
    blocking_write_register(IPISR_ADDR, IPISR_IPIS_MASK); 
      
        
    //CHECK RESULT (not sufficent)
    //read_register(CR0_ADDR, CR0_is);
    //if(CR0_is == c_cnt_limit) begin 
    //  $display(" RESULT DATA CHECK SUCCEED!!");
    //end else begin
    //  $display(" RESULT DATA CHECK FAILED!!");
    //  $display("  -- MISMATCH at  exp = %x, act = %x", c_cnt_limit, CR0_is);
    //end 
    
    //MANUAL RESULT CHECK IN WAVEFORM (timings taken with clkperiod=1ns)
    //CR0_reg should reach value of LR0_reg (limit value); //~338.5ns 
    //ap_done(GSCR_reg(1)) should be set //~338.5ns 
    //ap_start(GSCR_reg(0)) should be reset by ap_done(GSCR_reg(1)) //~338.5ns
    //o_t0_out should be toggled by ap_done(GSCR_reg(1))  //~338.5ns
    //interrupt should be set by ap_done(GSCR_reg(1))  //~338.5ns
    //ap_done(GSCR_reg(1)) should be reset after done_ack (read of GSCR_reg) //~350.5ns
    //interrupt should be reset after writing '1' to IPISR_reg (W1C!) //~372.5ns resp. ~373.5ns
        
    #10;   
  //##INSERT YOUR CODE HERE END    
  endtask
  
  
  task automatic TEST_UP_AUTORESTART_INT;  
    //const bit [31:0] c_cnt_limit = 32'h00000004;
    //const bit [31:0] c_cnt_limit = 32'h00000100;
    //const bit [31:0] c_cnt_limit = 32'h0000000A;
    const bit [31:0] c_cnt_limit = 32'h000008F;
    
    bit [31:0] CR0_is =0;
    bit [31:0] GSCR_is =0;
  
    $display("---------------------------------------------------------------");
    $display(" START TEST_UP_AUTORESTART WITH INT");
    $display("---------------------------------------------------------------");
  
   
    blocking_write_register(LR0_ADDR, c_cnt_limit);
    
    //TB_ud0 <='1'; (5)
    //TB_load0 <='0'; (4)
    //TB_ent0_out <='1'; (2)       
    blocking_write_register(SCSR_ADDR, 32'h00000024);
    
    blocking_write_register(IPIER_ADDR, IPIER_IPIE_MASK);
    
    blocking_write_register(GIER_ADDR, GIER_GIE_MASK);
            
    //TB_auto_restart<='1'; (7)    
    //TB_ap_start <='1'; (0)
    blocking_write_register(GSCR_ADDR, 32'h00000081);    
   
    #300;
 
   
    //poll_done_register();
    //wait(interrupt==1'b1);
    wait_for_interrupt();
 
    
    #10
    read_register(GSCR_ADDR, GSCR_is); //reset done
    
    //clear interrupt
    blocking_write_register(IPISR_ADDR, IPISR_IPIS_MASK); 
        
    //RESULT CHECK (not sufficent)
    //read_register(CR0_ADDR, CR0_is);
    //if(CR_is == c_cnt_limit) begin 
    //  $display(" RESULT DATA CHECK SUCCEED!!");
    //end else begin
    //  $display(" RESULT DATA CHECK FAILED!!");
    //  $display("  -- MISMATCH at  exp = %x, act = %x", c_cnt_limit, CR_is);
    //end 
    
    //MANUAL RESULT CHECK
    //repeating:       
      //CR0_reg should reach value of LR0_reg (limit value);
      //ap_done(GSCR_reg(1)) should be set
      //(ap_start(GSCR_reg(0)) should be reset by ap_done(GSCR_reg(1))) (overwritten by auto restart)
      //ap_start(GSCR_reg(0)) should be set again by auto_restart 
      //o_t0_out should be toggled by ap_done(GSCR_reg(1)) (oscillating with LR0_reg amount clks)
      //interrupt should be set by ap_done(GSCR_reg(1))
      //(ap_done(GSCR_reg(1)) should be reset after done_ack (read of GSCR_reg)) (not used here since free running)   
      //(interrupt should be reset after writing '1' to IPISR_ADDR (W1C)) (not used here since free running)    
    //ap_done(GSCR_reg(1)) should be reset after done_ack (read of GSCR_reg)
    //interrupt should be reset after writing '1' to IPISR_reg (W1C!)
    
    #10;   
  endtask
  
  //------------------------------------------------------------------------------------------------------
  //ACTUCAL TEST MAIN  
  initial begin : test_routine 
    //#2000
    //start_vips();
    //#1000
    #10
    start_vips();
    #10
 
    //##INSERT YOUR CODE HERE 
    CHECK_REGISTERS();
    //TEST_UP();
    //TEST_UP_AUTORESTART();
    //TEST_UP_INT();
    //TEST_UP_AUTORESTART_INT();
    //##INSERT YOUR CODE HERE END
 
    $finish(); 
            
  end 
  
endmodule
