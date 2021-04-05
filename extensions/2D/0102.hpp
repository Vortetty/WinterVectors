#include "classes_structs.hpp"
#include "globalDefs.hpp"
#include <deque>
#include <map>

unsigned int ext0102_line_id = 0x01020003;
void ext0102_line_inst(std::deque<uint8_t>& img, fileInfo& fi) {
    unsigned int x1 = getNextInt(fi.instructions),
                 y1 = getNextInt(fi.instructions),
                 x2 = getNextInt(fi.instructions),
                 y2 = getNextInt(fi.instructions);
    unsigned int m_new = 2 * (y2 - y1);
    unsigned int slope_error_new = m_new - (x2 - x1);
    for (int x = x1, y = y1; x <= x2; x++)
    {
        setPixel(x, y, fi.width, img, fi.currentColor.r, fi.currentColor.g, fi.currentColor.b, fi.currentColor.a);
    
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

void ext0102_registerAll(std::map<unsigned int, void (*)(std::deque<uint8_t>&, fileInfo&)>& instructionParsers) {
    instructionParsers[ext0102_line_id] = ext0102_line_inst;
}