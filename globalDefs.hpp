#include <deque>
#include <map>
#include "classes_structs.hpp"

#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED

std::deque<unsigned char> outImage;
fileInfo image;

unsigned int genInt(unsigned char a, unsigned char b, unsigned char c, unsigned char d){
    return (unsigned int)a << 24 | (unsigned int)b << 16 | (unsigned int)c << 8 | (unsigned int)d;
}

void setPixel(unsigned int x, unsigned int y, unsigned int width, std::deque<unsigned char>& image, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
    image[4 * width * y + 4 * x + 0] = r;
    image[4 * width * y + 4 * x + 1] = g;
    image[4 * width * y + 4 * x + 2] = b;
    image[4 * width * y + 4 * x + 3] = a;
}

unsigned int getNextInt(std::deque<unsigned char>& file){
    int tmp = genInt(file[0], file[1], file[2], file[3]);
    for (int i = 0; i < 4; i++) file.pop_front();
    return tmp;
}

void registerCommand(std::map<unsigned int, void (*)(std::deque<unsigned char>&, fileInfo&)>& instructionParsers, unsigned int id, void (*ext)(std::deque<unsigned char>&, fileInfo&)){
    instructionParsers.insert({id, ext});
}

#endif
