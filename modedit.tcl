package require Tk

load ./tclsword.dll

###
set module "StrongsGreek"

###
proc validIdx {newVal} {
	if {$newVal <= 0 || $newVal > 5624} {
		return 0
	}

	.tModEdit.tT delete 1.0 end
	.tModEdit.tT insert end [sword::dict $::module $newVal]

	return 1
}

### gui
wm withdraw .

toplevel .tModEdit
pack [spinbox .tModEdit.sIdx -from 1 -to 5624 -validatecommand {validIdx %P}] -side top
pack [text    .tModEdit.tT] -side top
.tModEdit.sIdx configure -validate key

