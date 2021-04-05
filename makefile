run: all
	main.exe transpLine.wvc
	main.exe redLine.wvc
	main.exe invalidExtensionTest.wvc

all:
	g++ main.cpp include/lodepng.cpp -o main.exe -std=c++17 -I.
