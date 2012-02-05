all: test.exe tclsword.dll

tclsword.dll: tclmain.cpp
	@g++ -fPIC -shared -o$@ -I/usr/include/tcl8.5 -Wall $< -ltcl8.5

test.exe: test.cpp
	@g++ -o$@ -I/usr/include/sword -Wall $< -lsword

