lib/json.o: code/main.cpp include/json.h
	g++ -c code/main.cpp -o json.o -O3 -static -std=c++17

lib/libjson.lib: include/json.h
	ar rcs lib/libjson.lib lib/json.o
