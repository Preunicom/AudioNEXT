# @author Markus Remy

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
set project_data_dir [file normalize "$script_dir/.."]

set proj_name "Audio"
#set proj_top_module "TODO_SET"
set proj_part "xc7a100tcsg324-1"
set proj_dir "[file normalize "$project_data_dir/../../xilinx/vivado/$proj_name"]"
set proj_IP_dir "[file normalize "$project_data_dir/ip"]"
set general_files_dir "[file normalize "$project_data_dir/../0_General"]"
set ip_repo_path "[file normalize "$project_data_dir/../ip_repo"]"

# ================ PROJECT ================
create_project $proj_name "$proj_dir" -part $proj_part

set obj [current_project]
set_property -name "customized_default_ip_location" -value $proj_IP_dir -objects $obj
set_property -name "enable_vhdl_2008" -value "1" -objects $obj
set_property -name "part" -value $proj_part -objects $obj
set_property -name "simulator_language" -value "Mixed" -objects $obj
set_property -name "target_language" -value "VHDL" -objects $obj
set_property -name "source_mgmt_mode" -value "All" -objects $obj

# ================ FILES ================
set ip_files [find_files $proj_IP_dir "*.xci"]
set rtl_files [concat \
    [find_files $project_data_dir/rtl "*.vhd"] \
    [find_files $project_data_dir/rtl "*.v"] \
    [find_files $project_data_dir/rtl "*.sv"] \
    [find_files $general_files_dir/rtl "*.vhd"] \
    [find_files $general_files_dir/rtl "*.v"] \
    [find_files $general_files_dir/rtl "*.sv"] \
]
set bd_files [find_files $project_data_dir/bd "*.bd"]
set rtl_and_ip_files [concat $rtl_files $ip_files $bd_files]
set sim_files [concat \
    [find_files $project_data_dir/sim "*.vhd"] \
    [find_files $project_data_dir/sim "*.v"] \
    [find_files $project_data_dir/sim "*.sv"] \
    [find_files $general_files_dir/sim "*.vhd"] \
    [find_files $general_files_dir/sim "*.v"] \
    [find_files $general_files_dir/sim "*.sv"] \
]

# ================ SOURCES ================
if {[string equal [get_filesets -quiet sources_1] ""]} {
  create_fileset -srcset sources_1
}

set obj [get_filesets sources_1]
if {[llength $rtl_and_ip_files] > 0} {
    add_files -norecurse -fileset $obj $rtl_and_ip_files
}

foreach file $rtl_files {
    set file_obj [get_files $file]
    if {[string match "*.vhd" $file]} {
        set_property -name "file_type" -value "VHDL" -objects $file_obj
    } 
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
if {[info exists proj_top_module]} {
    set_property -name "top" -value $proj_top_module -objects $obj
}
set_property -name "top_auto_set" -value "0" -objects $obj

# ================ SIMULATION ================
if {[string equal [get_filesets -quiet sim_1] ""]} {
  create_fileset -simset sim_1
}

set obj [get_filesets sim_1]

if {[llength $sim_files] > 0} {
    add_files -norecurse -fileset $obj $sim_files
}

foreach file $sim_files {
    set file_obj [get_files $file]
    if {[string match "*.vhd" $file]} {
        set_property -name "file_type" -value "VHDL 2008" -objects $file_obj
    }
}

set_property -name "xsim.simulate.runtime" -value "0ns" -objects $obj

# ================ IP REPO ================
set_property ip_repo_paths [list $ip_repo_path] [current_project]
update_ip_catalog