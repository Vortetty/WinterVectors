run: all
	main.exe test.wvc

all:
	clang main.cpp include/lodepng.cpp -o main.exe -std=c++17 -I.
