#include <deque>

struct color {
    char r, g, b, a;
};

struct extension {
    char extType, extID;
};

class fileInfo {
    public:
        int version, width, height;
        std::deque<char> meta;
        color bg;
        char colorMode;
        std::deque<extension> extensions;
        std::deque<int> instructions;
};
