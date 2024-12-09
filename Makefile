all: uaf attackcode

uaf: UAF.cpp
	g++ -Wall -o uaf UAF.cpp

attackcode: attackcode.cpp
	g++ -Wall -o attackcode attackcode.cpp
	