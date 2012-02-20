package require Tk

load ./tclsword.dll

###
set module "StrongsGreek"

set greekDict {
	α a
	β b
	γ g
	δ d
	ε e
	ζ z
	η _ee
	θ _th
	ι i
	κ k
	λ l
	μ m
	ν n
	ξ x
	ο o
	π p
	ρ r
	σ s
	ς _s
	τ t
	υ u
	φ _ph
	ψ _ps
	ω _oe
}
foreach {g e} $greekDict {
	dict lappend revGreekDict $e $g
}

proc revGreek {s} {
	set cur ""
	foreach e [split $s ""] {
		if {$cur eq ""} {
			if {$e ne "_"} {
				append ret [dict get $::revGreekDict $e]
				continue
			}
			append cur $e
			continue
		}

		if {$e eq "s"} {
			append ret [dict get $::revGreekDict "_s"]
			set cur ""
			continue
		}
		append cur $e
		if {[string length $cur] == 3} {
			append ret [dict get $::revGreekDict $cur]
			set cur ""
			continue
		}
	}

	return $ret
}

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
.tModEdit.tT insert end [sword::dict $::module 1]

toplevel .tTranslate
pack [entry .tTranslate.e]
bind .tTranslate.e <Return> {
	set t [revGreek [.tTranslate.e get]]
	.tTranslate.e delete 0 end
	.tTranslate.e insert end $t
}

