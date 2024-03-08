# Procedures updates fileset.
#
# Inputs:
#   src_dirs    source directories
#   fileset     fileset associated with source directory
#
proc update_fileset {src_dirs fileset} {

    # Get contents of source directory
    set src_files {}
    foreach src_dir $src_dirs {
        lappend src_files {*}[glob -nocomplain "${src_dir}/*"]
    }

    # Ensure all paths are absolute paths
    for {set i 0} {$i < [llength $src_files]} {incr i} {
        lset src_files $i [file normalize [lindex $src_files $i]]
    }

    # Get files in fileset
    set prj_files [get_files -of_objects [get_filesets $fileset] -quiet]

    # Ensure all paths are absolute paths
    for {set i 0} {$i < [llength $prj_files]} {incr i} {
        lset prj_files $i [file normalize [lindex $prj_files $i]]
    }

    # Add files not yet in fileset
    if {[llength $src_files]} {
        foreach src_file $src_files {
            set match 0
            if {[llength $prj_files]} {
                foreach prj_file $prj_files {
                    if {[string equal $prj_file $src_file]} {
                        set match 1
                    }
                }
            }
            if {$match == 0} {
                puts $src_file
                add_files -fileset $fileset $src_file
            }
        }
    }

    # Remove files no longer in source directory
    if {[llength $prj_files]} {
        foreach prj_file $prj_files {
            set match 0
            if {[llength $src_files]} {
                foreach src_file $src_files {
                    if {[string equal $prj_file $src_file]} {
                        set match 1
                    }
                }
            }
            if {$match == 0} {
                remove_files -fileset $fileset $prj_file
            }
        }
    }
}

# Add sources
update_fileset {./verilog_files/datapath ./verilog_files/reference ./verilog_files/autogen} sources_1

# Add testbenches
update_fileset {./verilog_files/tb} sim_1