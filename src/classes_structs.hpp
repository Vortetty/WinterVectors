#include <deque>
#include <map>

#ifndef CLASSES_STRUCTS
#define CLASSES_STRUCTS
struct color {
    uint8_t r, g, b, a;
};

struct extension {
    uint8_t extHigh, extLow;
};

class fileInfo {
    public:
        unsigned int version, width, height;
        std::deque<uint8_t> meta;
        color bg, currentColor = {0, 0, 0, 0};
        uint8_t colorMode;
        std::deque<extension> extensions;
        std::deque<uint8_t> instructions;
        bool debugEnabled = false;
};

class instructionDef {
    public:
        void inst(std::deque<uint8_t>& img, fileInfo& fi);
        unsigned int id;
};
#endif
