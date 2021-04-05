#include "classes_structs.hpp"
#include "globalDefs.hpp"
#include <deque>
#include <map>

// Pixel command
// ------------------------------------------------------------------------
unsigned int ext0102_pixel_id = 0x01020000;
void ext0102_pixel_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    unsigned int x = getNextInt(fi.instructions),
                 y = getNextInt(fi.instructions);
    
    setPixel(x, y, fi.width, img, fi.currentColor);
    if (fi.debugEnabled) std::cout << "(" << x << ", " << y << ")";
}
// ------------------------------------------------------------------------

// Rectangle command
// ------------------------------------------------------------------------
unsigned int ext0102_rect_id = 0x01020001;
void ext0102_rect_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    unsigned int x = getNextInt(fi.instructions),
                 y = getNextInt(fi.instructions),
                 width = getNextInt(fi.instructions),
                 height = getNextInt(fi.instructions);
    
    for (unsigned int cx=x; cx < x+width; cx++){
        for (unsigned int cy=y; cy < y+height; cy++){
            setPixel(cx, cy, fi.width, img, fi.currentColor);
            if (fi.debugEnabled) std::cout << "(" << cx << ", " << cy << ")";
        }
        if (fi.debugEnabled) std::cout << "\n";
    }
}
// ------------------------------------------------------------------------

// Circle command, diameter will be radius*2+1
// ------------------------------------------------------------------------
unsigned int ext0102_circle_id = 0x01020002;
void ext0102_circle_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    unsigned int x = getNextInt(fi.instructions),
                 y = getNextInt(fi.instructions),
                 radius = getNextInt(fi.instructions);
    if (fi.debugEnabled) std::cout << std::dec << x << " " << y << " " << radius << "\n";
    if (fi.debugEnabled) std::cout << "(" << x-radius+1 << ", " << y-radius+1 << ") -> (" << x+radius << ", " << y+radius << ")\n";
    
    for (unsigned int cx=x-radius; cx < x+radius+1; cx++){
        for (unsigned int cy=y-radius; cy < y+radius+1; cy++){
            if (pointDistance(x, y, cx, cy) <= radius) setPixel(cx, cy, fi.width, img, fi.currentColor);
            if (fi.debugEnabled) std::cout << (pointDistance(x, y, cx, cy) <= radius ? concolor_fg_rgb(119, 221, 119) : concolor_fg_rgb(221, 119, 119)) << "(" << cx << ", " << cy << ")" << concolor_reset();
        }
        if (fi.debugEnabled) std::cout << "\n";
    }
}
// ------------------------------------------------------------------------

// Line command
// ------------------------------------------------------------------------
unsigned int ext0102_line_id = 0x01020003;
void ext0102_line_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    unsigned int x1 = getNextInt(fi.instructions),
                 y1 = getNextInt(fi.instructions),
                 x2 = getNextInt(fi.instructions),
                 y2 = getNextInt(fi.instructions);
    unsigned int m_new = 2 * (y2 - y1);
    unsigned int slope_error_new = m_new - (x2 - x1);
    for (unsigned int x = x1, y = y1; x <= x2; x++)
    {
        setPixel(x, y, fi.width, img, fi.currentColor);
        if (fi.debugEnabled) std::cout << "(" << x << ", " << y << ")";
    
        // Add slope to increment angle formed
        slope_error_new += m_new;
    
        // Slope error reached limit, time to
        // increment y and update slope error.
        if (slope_error_new >= 0)
        {
            y++;
            slope_error_new  -= 2 * (x2 - x1);
        }
    }
}
// ------------------------------------------------------------------------

void ext0102_registerAll(std::map<unsigned int, void (*)(std::deque<uint8_t>&, fileInfo&)>& instructionParsers) {
    instructionParsers[ext0102_pixel_id] = ext0102_pixel_inst;
    instructionParsers[ext0102_rect_id] = ext0102_rect_inst;
    instructionParsers[ext0102_circle_id] = ext0102_circle_inst;
    instructionParsers[ext0102_line_id] = ext0102_line_inst;
}