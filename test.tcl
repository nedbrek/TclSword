load ./tclsword.dll
set mods [sword::modules]
foreach d $mods {
	set name [dict get $d Name]
	puts "$name [dict get $d Type]"

	if {$name eq "ESV"} {
		set l [sword::get $name "John 1:1-4"]
		foreach d $l {
			puts "[dict get $d Ref] [dict get $d Text]"
		}
	}
}

