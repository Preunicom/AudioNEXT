proc find_files {dir pattern} {
    set result {}
    foreach f [glob -nocomplain -directory $dir -types f $pattern] {
        lappend result [file normalize $f]
    }
    foreach d [glob -nocomplain -directory $dir -types d *] {
        set result [concat $result [find_files $d $pattern]]
    }
    return $result
}


# ================ VALUES ================
set script_dir [file dirname [info script]]
set origin_dir "$script_dir/.."

set proj_name "TODO_SET_PROJECT_NAME"
set proj_top_module "TODO_SET_PROJECT_TOP_MODULE"
set proj_part "TODO_SET_PROJECT_PART"
set proj_dir "[file normalize "$origin_dir/xilinx/vivado"]"
set proj_IP_dir "[file normalize "$origin_dir/hw/ip"]"

# ================ PROJECT ================
create_project $proj_name "$proj_dir" -part $proj_part

set obj [current_project]
set_property -name "customized_default_ip_location" -value $proj_IP_dir -objects $obj
set_property -name "enable_vhdl_2008" -value "1" -objects $obj
set_property -name "part" -value $proj_part -objects $obj
set_property -name "simulator_language" -value "Mixed" -objects $obj
set_property -name "target_language" -value "VHDL" -objects $obj

# ================ FILES ================
set ip_files [find_files $proj_IP_dir "*.xci"]
set rtl_files [find_files $origin_dir/hw/rtl "*.vhd"]
set rtl_and_ip_files [concat $src_files $ip_files]
set sim_files [find_files $origin_dir/hw/sim "*.vhd"]
set constr_file [lindex [find_files $origin_dir/hw/constraints "*.xdc"] 0]

# ================ SOURCES ================
if {[string equal [get_filesets -quiet sources_1] ""]} {
  create_fileset -srcset sources_1
}

set obj [get_filesets sources_1]
add_files -norecurse -fileset $obj $rtl_and_ip_files

foreach file $rtl_files {
    set file_obj [get_files $file]
    set_property -name "file_type" -value "VHDL" -objects $file_obj
}

foreach file $ip_files {
    set file_obj [get_files $file]
    set_property -name "generate_files_for_reference" -value "0" -objects $file_obj
    set_property -name "registered_with_manager" -value "1" -objects $file_obj
    if { ![get_property "is_locked" $file_obj] } {
        set_property -name "synth_checkpoint_mode" -value "Singular" -objects $file_obj
    }
}

set obj [get_filesets sources_1]
set_property -name "top" -value $proj_top_module -objects $obj
set_property -name "top_auto_set" -value "0" -objects $obj

# ================ CONSTRAINTS ================
if {[string equal [get_filesets -quiet constrs_1] ""]} {
  create_fileset -constrset constrs_1
}
set obj [get_filesets constrs_1]

set file "$constr_file"
set file_added [add_files -norecurse -fileset $obj [list $file]]
set file_obj [get_files $file]
set_property -name "file_type" -value "XDC" -objects $file_obj

set obj [get_filesets constrs_1]
set_property -name "target_part" -value $proj_part -objects $obj

# ================ SIMULATION ================
if {[string equal [get_filesets -quiet sim_1] ""]} {
  create_fileset -simset sim_1
}

set obj [get_filesets sim_1]
add_files -norecurse -fileset $obj $sim_files

foreach file $sim_files {
    set file_obj [get_files $file]
    set_property -name "file_type" -value "VHDL 2008" -objects $file_obj
}

set obj [get_filesets sim_1]
set_property -name "xsim.simulate.runtime" -value "0ns" -objects $obj
