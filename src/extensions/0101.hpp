#include "classes_structs.hpp"
#include "globalDefs.hpp"
#include <deque>
#include <map>

unsigned int ext0101_noop_id = 0x00000000;
void ext0101_noop_inst(std::deque<uint8_t>& img, fileInfo& fi) {}

unsigned int ext0101_noopex_id = 0x00000001;
void ext0101_noopex_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    uint16_t readBytes = getNextInt16(fi.instructions);
    for (int i = 0; i < readBytes; i++){
        getNextInt8(fi.instructions);
    }
}

unsigned int ext0101_setcolor_id = 0x01010000;
void ext0101_setcolor_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    fi.currentColor = getNextColor(fi.instructions);
}

unsigned int ext0101_setdebug_id = 0x01010001;
void ext0101_setdebug_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    fi.debugEnabled = (bool)getNextInt(fi.instructions);
}

void ext0101_registerAll(std::map<unsigned int, void (*)(std::deque<uint8_t>&, fileInfo&)>& instructionParsers) {
    instructionParsers[ext0101_noop_id] = ext0101_noop_inst;
    instructionParsers[ext0101_noopex_id] = ext0101_noopex_inst;
    instructionParsers[ext0101_setcolor_id] = ext0101_setcolor_inst;
    instructionParsers[ext0101_setdebug_id] = ext0101_setdebug_inst;
}