run: all
ifeq ($(OS),Windows_NT)
	bin/main.exe tests/2Dtest.wvc
	bin/main.exe tests/paletteRaster.wvc
	bin/main.exe tests/invalidExtensionTest.wvc
else
	bin/main tests/2Dtest.wvc
	bin/main tests/paletteRaster.wvc
	bin/main tests/invalidExtensionTest.wvc
endif

github: all
	git add *
	git commit
	git push

git: github
push: github
update: github

all:
ifeq ($(OS),Windows_NT)
	clang src/main.cpp include/lodepng.cpp -o bin/main.exe -std=c++17 -I./src -I./include -I. -O3 -s -Ofast
	llvm-strip bin/main.exe
else
	clang src/main.cpp include/lodepng.cpp -o bin/main -std=c++17 -I./src -I./include -I. -O3 -s -Ofast
	llvm-strip bin/main
	chmod +x bin/main
endif
