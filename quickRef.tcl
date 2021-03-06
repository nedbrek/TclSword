package require Tk
package require tdom
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

set xmlOut ""
proc xmlTextCB {txt} {
	set lastChar [string index $::xmlOut end]
	set firstChar [string index $txt 0]
	if {$lastChar ne " " && ![string is punct $firstChar]} {
		append ::xmlOut " "
	}
	append ::xmlOut $txt
}

proc stripHtml {t} {
	if {[string first "<" $t] == -1} {
		return $t
	}

	xml::parser p -characterdatacommand xmlTextCB

	set ::xmlOut ""
	p parse "<body>$t</body>"

	p free
	return $::xmlOut
}

### gui
wm title . "Quick Reference"
grid [ttk::combobox .cbVersions -state readonly -values [getModNames "Biblical Texts"]] -row 0

grid [entry .eRef] -row 1

scrollbar .scrollV -orient vert -command ".tResults yview"
text .tResults -wrap word -yscrollcommand ".scrollV set"

grid .tResults -sticky nswe -row 2 -column 0
grid .scrollV -sticky ns -row 2 -column 1

grid rowconfigure . .tResults -weight 1
grid columnconfigure . .tResults -weight 1

bind .eRef <Return> {
	set ver [.cbVersions get]
	if {$ver eq ""} { return }

	set ref [.eRef get]
	if {$ref eq ""} { return }

	set res [sword::get $ver $ref]
	foreach r $res {
		set v [dict get $r Ref]
		set t [stripHtml [dict get $r Text]]
		.tResults insert end "$ver $v $t\n"
	}
}

