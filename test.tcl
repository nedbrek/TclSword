load ./tclsword.dll
set mods [sword::modules]
foreach d $mods {
	puts "[dict get $d Name] [dict get $d Type]"
}

