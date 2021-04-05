run: all
ifeq ($(OS),Windows_NT)
	bin/main.exe 2Dtest.wvc
	bin/main.exe invalidExtensionTest.wvc
else
	bin/main 2Dtest.wvc
	bin/main invalidExtensionTest.wvc
endif

github: all
	git add *
	git commit
	git push

all:
ifeq ($(OS),Windows_NT)
	g++ main.cpp include/lodepng.cpp -o bin/main.exe -std=c++17 -I.
else
	g++ main.cpp include/lodepng.cpp -o bin/main -std=c++17 -I.
	chmod +x bin/main
endif
