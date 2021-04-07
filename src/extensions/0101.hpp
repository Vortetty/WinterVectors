#include "classes_structs.hpp"
#include "globalDefs.hpp"
#include "palettes/palettes.h"
#include <iostream>
#include <deque>
#include <map>
#include "ConColor.h"

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

unsigned int ext0101_raster4_id = 0x01010002;
void ext0101_raster4_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    unsigned int x = getNextInt(fi.instructions),
                 y = getNextInt(fi.instructions),
                 width = getNextInt(fi.instructions),
                 height = getNextInt(fi.instructions);
    color tmp;

    for (int cy = x; cy < x+width; cy++){
        for (int cx = y; cx < y+height; cx++){
            tmp = getNextColor(fi.instructions);
            setPixel(cx, cy, fi.width, img, tmp);
            if (fi.debugEnabled) std::cout << concolor_fg_rgb(tmp.r, tmp.g, tmp.b) <<  "(" << cx << ", " << cy << ")\n" << concolor_reset();
        }
    }
}
unsigned int ext0101_raster3_id = 0x01010003;
void ext0101_raster3_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    unsigned int x = getNextInt(fi.instructions),
                 y = getNextInt(fi.instructions),
                 width = getNextInt(fi.instructions),
                 height = getNextInt(fi.instructions);
    color tmp;

    for (int cy = x; cy < x+width; cy++){
        for (int cx = y; cx < y+height; cx++){
            tmp = getNextColor3(fi.instructions);
            setPixel(cx, cy, fi.width, img, tmp);
            if (fi.debugEnabled) std::cout << concolor_fg_rgb(tmp.r, tmp.g, tmp.b) <<  "(" << cx << ", " << cy << ")\n" << concolor_reset();
        }
    }
}

unsigned int ext0101_raster_set_palette_id = 0x01010004;
void ext0101_raster_set_palette_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    palette = getNextInt8(fi.instructions);
    if (fi.debugEnabled) std::cout << "Set palette to \"" << paletteNames[palette] << "\""; 
}
unsigned int ext0101_raster_palette_id = 0x01010005;
void ext0101_raster_palette_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    unsigned int x = getNextInt(fi.instructions),
                 y = getNextInt(fi.instructions),
                 width = getNextInt(fi.instructions),
                 height = getNextInt(fi.instructions);
    color tmp;

    for (int cy = x; cy < x+width; cy++){
        for (int cx = y; cx < y+height; cx++){
            tmp = read_palette_color(getNextInt8(fi.instructions));
            setPixel(cx, cy, fi.width, img, tmp);
            if (fi.debugEnabled) std::cout << concolor_fg_rgb(tmp.r, tmp.g, tmp.b) <<  "(" << cx << ", " << cy << ")\n" << concolor_reset();
        }
    }
}

void ext0101_registerAll(std::map<unsigned int, void (*)(std::deque<uint8_t>&, fileInfo&)>& instructionParsers) {
    instructionParsers[ext0101_noop_id] = ext0101_noop_inst;
    instructionParsers[ext0101_noopex_id] = ext0101_noopex_inst;
    instructionParsers[ext0101_setcolor_id] = ext0101_setcolor_inst;
    instructionParsers[ext0101_setdebug_id] = ext0101_setdebug_inst;
    instructionParsers[ext0101_raster_set_palette_id] = ext0101_raster_set_palette_inst;
    instructionParsers[ext0101_raster_palette_id] = ext0101_raster_palette_inst;
}