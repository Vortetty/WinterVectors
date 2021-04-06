#include <deque>
#include <map>
#include <cmath>
#include "classes_structs.hpp"

#define LODEPNG_NO_COMPILE_ZLIB
#define LODEPNG_NO_COMPILE_DECODER
//#define LODEPNG_NO_COMPILE_ENCODER
#define LODEPNG_COMPILE_CPP
#include "include/lodepng.h"

#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED

std::deque<uint16_t> supportedExtensions = {
    0x0101, // Default commands, all must support these
    0x0102  // Basic 2D
};

unsigned int genInt(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
    return (unsigned int)a << 24 | (unsigned int)b << 16 | (unsigned int)c << 8 | (unsigned int)d;
}

uint16_t genInt16(uint8_t a, uint8_t b){
    return (uint16_t)a << 8 | (uint16_t)b;
}

void setPixel(unsigned int x, unsigned int y, unsigned int width, std::deque<uint8_t>& image, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    image[4 * width * y + 4 * x + 0] = r;
    image[4 * width * y + 4 * x + 1] = g;
    image[4 * width * y + 4 * x + 2] = b;
    image[4 * width * y + 4 * x + 3] = a;
}

void setPixel(unsigned int x, unsigned int y, unsigned int width, std::deque<uint8_t>& image, color col){
    image[4 * width * y + 4 * x + 0] = col.r;
    image[4 * width * y + 4 * x + 1] = col.g;
    image[4 * width * y + 4 * x + 2] = col.b;
    image[4 * width * y + 4 * x + 3] = col.a;
}

double pointDistance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    // Calculating distance
    return sqrt(((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)));
}

unsigned int getNextInt(std::deque<uint8_t>& file){
    int tmp = genInt(file[0], file[1], file[2], file[3]);
    for (int i = 0; i < 4; i++) file.pop_front();
    return tmp;
}

unsigned int getNextInt16(std::deque<uint8_t>& file){
    int tmp = genInt16(file[0], file[1]);
    file.pop_front();
    file.pop_front();
    return tmp;
}

uint8_t getNextInt8(std::deque<uint8_t>& file){
    uint8_t tmp = file[0];
    file.pop_front();
    return tmp;
}

color getNextColor(std::deque<uint8_t>& file){
    color tmp = {file[0], file[1], file[2], file[3]};
    for (int i = 0; i < 4; i++) file.pop_front();
    return tmp;
}

bool checkInstructionSetSupported(uint8_t high, uint8_t low){
    return std::find(supportedExtensions.begin(), supportedExtensions.end(), (uint16_t)high << 8 | (uint16_t)low) != supportedExtensions.end();
}

std::deque<uint8_t> readFile(const char* filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::deque<uint8_t> vec;
    //vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());

    return vec;
}

void saveImage(const char* filename, std::deque<uint8_t>* image, unsigned int width, unsigned int height){
    std::vector<uint8_t> png;
    std::vector<uint8_t> inpng((*image).begin(), (*image).end());

    unsigned error = lodepng::encode(png, inpng, width, height);
    if(!error) lodepng::save_file(png, filename);
    else std::cout << std::dec << lodepng_error_text(error) << " | Given size: " << (*image).size()/4 << ", Expected Size: " << width*height;
}

void padImage(std::deque<uint8_t>& image, unsigned int width, unsigned int height){
    while (image.size()/4 < width*height){
        for (int i = 0; i < 4; i++) image.push_back(0);
    }
}

#endif
