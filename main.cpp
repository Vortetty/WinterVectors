#include <iostream>
#include <iterator>
#include <fstream>
#include <deque>
#include <vector>
#include <map>
#include <algorithm>
#include "classes_structs.hpp"

#include "globalDefs.hpp"
#include "extensions/2D/01.hpp"

#define LODEPNG_NO_COMPILE_ZLIB
#define LODEPNG_NO_COMPILE_DECODER
//#define LODEPNG_NO_COMPILE_ENCODER
#define LODEPNG_COMPILE_CPP
#include "include/lodepng.h"

// Magic numbers are bad so i put stuff here (for #defines)
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

std::deque<unsigned char> readFile(const char* filename)
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
    std::deque<unsigned char> vec;
    //vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<unsigned char>(file),
               std::istream_iterator<unsigned char>());

    return vec;
}

void setPixel(unsigned int x, unsigned int y, unsigned int width, std::deque<unsigned char>* image, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
    (*image)[4 * width * y + 4 * x + 0] = r;
    (*image)[4 * width * y + 4 * x + 1] = g;
    (*image)[4 * width * y + 4 * x + 2] = b;
    (*image)[4 * width * y + 4 * x + 3] = a;
}

void saveImage(const char* filename, std::deque<unsigned char>* image, unsigned int width, unsigned int height){
    std::vector<unsigned char> png;
    std::vector<unsigned char> inpng((*image).begin(), (*image).end());

    unsigned error = lodepng::encode(png, inpng, width, height);
    if(!error) lodepng::save_file(png, filename);
    else std::cout << lodepng_error_text(error) << " " << inpng.size() << " " << (*image).size() << " " << width*height;
}

int main(int argc, char* argv[]){
    // Magic numbers are bad so i put stuff here
    // ------------------------------------------------------------------------
    std::string filename = "";
    if (argc > 1) filename = argv[1];
    std::deque<unsigned char> file;
    fileInfo image;
    std::string colorModes[] = {"RGBA", "HSVA", "RGB", "HSV"};
    // ------------------------------------------------------------------------

    // Parse parameters
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------


    if (filename != "") {
        // Parse file out
        // ------------------------------------------------------------------------
        file = readFile(filename.c_str());
    
        image.version = genInt(file[0], file[1], file[2], file[3]);
        for (int i = 0; i < 4; i++) file.pop_front();
        
        image.width = genInt(file[0], file[1], file[2], file[3]);
        for (int i = 0; i < 4; i++) file.pop_front();
        
        image.height = genInt(file[0], file[1], file[2], file[3]);
        for (int i = 0; i < 4; i++) file.pop_front();

        file.pop_front();
        while (file[0] != 0){
            image.meta.push_back(file[0]);
            file.pop_front();
        }
        file.pop_front();

        image.bg.r = file[0];
        image.bg.g = file[1];
        image.bg.b = file[2];
        image.bg.a = file[3];
        for (int i = 0; i < 4; i++) file.pop_front();

        image.colorMode = file[0];
        file.pop_front();

        file.pop_front();
        while (file[0] != 0){
            extension e;
            image.extensions.push_front(
                e
            );
            image.extensions[0].extType = file[0];
            image.extensions[0].extID = file[1];
            file.pop_front();
            file.pop_front();
        }
        file.pop_front();

        image.instructions = std::deque<unsigned char>(file.begin(), file.end());

        delete &file;
        // ------------------------------------------------------------------------


        // Show info about file
        // ------------------------------------------------------------------------
        std::cout << "Image info:\n"; 
        std::cout << "  Version: " << image.version << "\n";
        std::cout << "  Width: " << image.width << "\n";
        std::cout << "  Height: " << image.height << "\n";

        std::cout << "  Metadata: ";
        for (auto i = image.meta.begin(); i != image.meta.end(); ++i)
            std::cout << *i;
        std::cout << "\n";

        std::cout << "  Background Color: #" << std::hex << (int)image.bg.r << (int)image.bg.g << (int)image.bg.b << (int)image.bg.a << std::dec << "\n";
        std::cout << "  Color Mode: " << colorModes[image.colorMode] << "\n";
        
        std::cout << "  Extensions: { ";
        for (auto i = image.extensions.begin(); i != image.extensions.end(); ++i){
            std::cout << std::hex << (unsigned int)((extension)*i).extType << "::" << (unsigned int)((extension)*i).extID << " ";
            if (image.extensions.end()-1 != i) std::cout << ", ";
            else std::cout << "}\n";
        }
        // ------------------------------------------------------------------------
    } else {
        std::cout << "Proper usage: " << argv[0] << " <wvc file>";

        return 0;
    }



    // Extension loader
    // ------------------------------------------------------------------------
    std::map<unsigned int, void (*)(std::deque<unsigned char>&, fileInfo&)> instructionParsers;
    std::deque<unsigned char> outImage;

    for (unsigned int x = 0; x < image.width; x++){
        for (unsigned int y = 0; y < image.height; y++){
            outImage.push_back(0);
            outImage.push_back(0);
            outImage.push_back(0);
            outImage.push_back(0);
        }
    }
    for (unsigned int x = 0; x < image.width; x++){
        for (unsigned int y = 0; y < image.height; y++){
            outImage[4 * image.width * y + 4 * x + 0] = image.bg.r;
            outImage[4 * image.width * y + 4 * x + 1] = image.bg.g;
            outImage[4 * image.width * y + 4 * x + 2] = image.bg.b;
            outImage[4 * image.width * y + 4 * x + 3] = image.bg.a;
        }
    }

    ext01_registerAll(instructionParsers);

    while (image.instructions.size() > 0){
        instructionParsers[getNextInt(image.instructions)](outImage, image);
    }

    saveImage("test.png", &outImage, image.width, image.height);
    // ------------------------------------------------------------------------

    return 0;
}