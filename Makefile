all: test.exe

test.exe: test.cpp
	@g++ -o$@ -I/usr/include/sword -Wall $< -lsword

