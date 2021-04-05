#include "classes_structs.hpp"
#include "globalDefs.hpp"
#include <deque>
#include <map>

unsigned int ext0101_setcolor_id = 0x01010000;
void ext0101_setcolor_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    fi.currentColor = getNextColor(fi.instructions);
}

void ext0101_registerAll(std::map<unsigned int, void (*)(std::deque<uint8_t>&, fileInfo&)>& instructionParsers) {
    instructionParsers[ext0101_setcolor_id] = ext0101_setcolor_inst;
}