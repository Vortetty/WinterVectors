#include <iostream>
#include <iterator>
#include <fstream>
#include <deque>
#include <vector>
#include <map>
#include <algorithm>
#include "classes_structs.hpp"
#include "ConColor.h"

#include "globalDefs.hpp"
#include "extensions/0101.hpp"
#include "extensions/2D/0102.hpp"

#define LODEPNG_NO_COMPILE_ZLIB
#define LODEPNG_NO_COMPILE_DECODER
//#define LODEPNG_NO_COMPILE_ENCODER
#define LODEPNG_COMPILE_CPP
#include "include/lodepng.h"

// Magic numbers are bad so i put stuff here (for #defines)
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

int main(int argc, char* argv[]){
    // Magic numbers are bad so i put stuff here
    // ------------------------------------------------------------------------
    std::string filename = "";
    if (argc > 1) filename = argv[1];

    std::deque<uint8_t> file;
    fileInfo image;

    std::string colorModes[] = {"RGBA", "HSVA", "RGB", "HSV"};

    std::map<unsigned int, void (*)(std::deque<uint8_t>&, fileInfo&)> instructionParsers;
    std::deque<uint8_t> outImage;
    unsigned int currentInstruction;

    bool unsupportedExtension = false;
    
    concolor_setup();
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
            image.extensions[0].extHigh = file[0];
            image.extensions[0].extLow = file[1];
            file.pop_front();
            file.pop_front();
        }
        file.pop_front();

        image.instructions = std::deque<uint8_t>(file.begin(), file.end());
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
        
        std::cout << "  Extensions Needed: { ";
        for (auto i = image.extensions.begin(); i != image.extensions.end(); ++i){
            std::cout << std::hex << (checkInstructionSetSupported((*i).extHigh, (*i).extLow) ? concolor_fg_rgb(119, 221, 119) : concolor_fg_rgb(221, 119, 119)) << ((uint16_t)(*i).extHigh << 8 | (uint16_t)(*i).extLow) << "\x1b[0m";
            if (image.extensions.end()-1 != i) std::cout << ", ";
            else std::cout << " }\n";

            if (!checkInstructionSetSupported((*i).extHigh, (*i).extLow)) unsupportedExtension = true;
        }

        if (unsupportedExtension) {
            std::cout << "Image uses one or more unsupported extensions, exiting.\n";
            return 0;
        }

        // ------------------------------------------------------------------------
    } else {
        std::cout << "Proper usage: " << argv[0] << " <wvc file>";

        return 0;
    }



    // Extension loader
    // ------------------------------------------------------------------------
    ext0101_registerAll(instructionParsers);
    ext0102_registerAll(instructionParsers);
    // ------------------------------------------------------------------------
    
    // Setup image
    // ------------------------------------------------------------------------
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
    // ------------------------------------------------------------------------

    
    // Run instructions and save image
    // ------------------------------------------------------------------------
    while (image.instructions.size() > 0){
        currentInstruction = getNextInt(image.instructions);
        std::cout << std::hex << "Instruction: " << currentInstruction << "\n";
        instructionParsers[currentInstruction](outImage, image);
    }

    saveImage((filename + ".png").c_str(), &outImage, image.width, image.height);
    // ------------------------------------------------------------------------

    return 0;
}