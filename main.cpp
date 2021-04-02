#include <iostream>
#include <fstream>
//#include "include/cli11/CLI11.hpp"
#include "classes.hpp"
// Magic numbers are bad so i put stuff here (for #defines)
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

template<class T>
void print(T str){
    std::cout << str << "\n";
}

int main(int argc, char* argv[]){
    // Magic numbers are bad so i put stuff here
    // ------------------------------------------------------------------------
    std::string filename = "test.wvc";
    fileInfo image;
    char temp;
    // ------------------------------------------------------------------------

    // Parse parameters
    // ------------------------------------------------------------------------
    //CLI::App app{"Decoder for the Winter Vector format, with implementations of all official extensions."};

    //app.add_option("file", filename, "File to decode");

    //CLI11_PARSE(app, argc, argv);
    // ------------------------------------------------------------------------

    
    // Parse file out
    // ------------------------------------------------------------------------
    std::ifstream file(filename, std::ios::binary);

    file >> image.version; // read version
    file >> image.width;   // read width
    file >> image.height;  // read height

    file >> temp;          // read 0 byte
    file >> temp;          // read next byte
    while (temp != 0x00){ // read until termination char
        image.meta.push_back(temp);
        file >> temp;
    }

    file >> image.bg.r; // read bg color from file
    file >> image.bg.g;
    file >> image.bg.b;
    file >> image.bg.a;
    file >> image.colorMode; // Read color mode

    file >> temp;          // read 0 byte
    file >> temp;          // read next byte
    while (temp != 0x00){ // read until termination char
        extension e;
        e.extID = temp;
        file >> e.extType;
        image.extensions.push_back(e);
        file >> temp;
    }

    file >> temp; // read in next byte
    while (!file.eof()) // read bytes until eof
    {
        image.instructions.push_back(temp);
        file >> temp;
    }

    print(image.version);
    print(image.width);
    print(image.height);
    for (std::deque<char>::const_iterator i = image.meta.begin(); i != image.meta.end(); ++i)
        std::cout << *i;
    print(image.bg.r);
    print(image.bg.g);
    print(image.bg.b);
    print(image.bg.a);
    print(image.colorMode);
    // ------------------------------------------------------------------------




    return 0;
}