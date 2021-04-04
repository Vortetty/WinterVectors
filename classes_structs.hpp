#include <deque>
#include <map>

#ifndef CLASSES_STRUCTS
#define CLASSES_STRUCTS
struct color {
    unsigned char r, g, b, a;
};

struct extension {
    unsigned char extType, extID;
};

class fileInfo {
    public:
        unsigned int version, width, height;
        std::deque<unsigned char> meta;
        color bg, currentColor={0, 0, 0, 0};
        unsigned char colorMode;
        std::deque<extension> extensions;
        std::deque<unsigned char> instructions;
};

class instructionDef {
    public:
        void inst(std::deque<unsigned char>& img, fileInfo& fi);
        unsigned int id;
};
#endif
