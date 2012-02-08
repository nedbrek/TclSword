.PHONY: all run

INC :=-I/usr/include/sword

all: test.exe tclsword.dll

tclsword.dll: tclmain.cpp
	@g++ -fPIC -shared -o$@ $(INC) -I/usr/include/tcl8.5 -Wall $< -ltcl8.5 -lsword

test.exe: test.cpp
	@g++ -o$@ $(INC) -Wall $< -lsword

run: test.exe
	@./test.exe

