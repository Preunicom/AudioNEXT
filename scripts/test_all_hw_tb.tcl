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

puts "============================================================"
puts " Testing Project..."
puts " Searching for sub-test-scripts..."
puts "============================================================"

set script_dir [file dirname [info script]]
set project_dir [file normalize "$script_dir/.."]

set test_scripts [find_files $project_dir/hw "test_all_tb.tcl"]

puts "Script directory: $script_dir"
puts "Project directory: $project_dir"
puts "Found subscripts: $test_scripts"

set exit_code 0

foreach tb_script $test_scripts {

    puts "------------------------------------------------------------"
    puts "Running sub test script: $tb_script"
    puts "------------------------------------------------------------"

    cd [file dirname $tb_script]

    catch {source $tb_script} tcl_error_code

    if {$exit_code_result != 0 || $tcl_error_code != 0 } {
        puts "ERROR: Sub test script failed: $tb_script"
        set exit_code 1
    } else {
        puts "INFO: Sub test script finished successfully"
    }
}

puts "------------------------------------------------------------"
puts "All sub test scripts completed!"
puts "------------------------------------------------------------"

if {$exit_code != 0} {
    puts "Exited with error!"
    exit 1
} else {
    puts "Exited successfully!"
    exit 0
}