package require Tk
load ./tclsword.dll

proc getModNames {type} {
	set ret ""

	foreach d [sword::modules] {
		if {$type eq "" || $type eq [dict get $d Type]} {
			lappend ret [dict get $d Name]
		}
	}

	return $ret
}

### gui
pack [ttk::combobox .cbVersions -state readonly -values [getModNames "Biblical Texts"]] -side top

pack [entry .eRef] -side top

pack [text .tResults] -side top

bind .eRef <Return> {
	set ver [.cbVersions get]
	if {$ver eq ""} { return }

	set ref [.eRef get]
	if {$ref eq ""} { return }

	set res [sword::get $ver $ref]
	foreach r $res {
		set v [dict get $r Ref]
		set t [dict get $r Text]
		.tResults insert end "$ver $v $t\n"
	}
}

