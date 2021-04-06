run: all
ifeq ($(OS),Windows_NT)
	bin/main.exe tests/2Dtest.wvc
	bin/main.exe tests/invalidExtensionTest.wvc
else
	bin/main tests/2Dtest.wvc
	bin/main tests/invalidExtensionTest.wvc
endif

asm:
	cd src && clang main.cpp -std=c++17 -I../src -I../include -I.. -O3 -s -S -masm=intel
	cd include && clang lodepng.cpp -std=c++17 -I../src -I../include -I.. -O3 -s -S -masm=intel

github: all asm
	git add *
	git commit
	git push

git: github
push: github
update: github

all:
ifeq ($(OS),Windows_NT)
	g++ src/main.cpp include/lodepng.cpp -o bin/main.exe -std=c++17 -I./src -I./include -I. -O9999 -s
else
	g++ src/main.cpp include/lodepng.cpp -o bin/main -std=c++17 -I./src -I./include -I. -O9999 -s
	chmod +x bin/main
endif
