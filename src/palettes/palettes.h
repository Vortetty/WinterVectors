#include <deque>
#include <map>
#include <string>
#include "atari_8_bit_family_gtia.h"
#include "aurora.h"
#include "duel.h"
#include "hocus_pocus.h"
#include "sonic_robo_blast_2_v22.h"
#include "srb2.h"
#include "uzebox.h"
#include "windows_95_256_colour.h"
#include "globalDefs.hpp"

#ifndef PALLETES_H
#define PALLETES_H

std::deque<const unsigned char (*)[2048]> palettes;
std::string paletteNames[] = {
    "Atari 8bit (256)",
    "Aurora (256)",
    "Duel (256)",
    "Hocus Pocus (256)",
    "Sonic Robo Blast 2 (256)",
    "srb2 (256)",
    "uzebox (256)",
    "Windows 95 (256)"
};
uint8_t palette = 0x01;

std::map<uint8_t, uint8_t> chr2nib;

void palettes_setup(){
    palettes.push_back(&atari_8_bit_family_gtia_hex);
    palettes.push_back(&aurora_hex);
    palettes.push_back(&duel_hex);
    palettes.push_back(&hocus_pocus_hex);
    palettes.push_back(&sonic_robo_blast_2_v22_hex);
    palettes.push_back(&srb2_hex);
    palettes.push_back(&uzebox_hex);
    palettes.push_back(&windows_95_256_colours_hex);

    chr2nib['0'] = 0x0;
    chr2nib['1'] = 0x1;
    chr2nib['2'] = 0x2;
    chr2nib['3'] = 0x3;
    chr2nib['4'] = 0x4;
    chr2nib['5'] = 0x5;
    chr2nib['6'] = 0x6;
    chr2nib['7'] = 0x7;
    chr2nib['8'] = 0x8;
    chr2nib['9'] = 0x9;

    chr2nib['A'] = 0xa;
    chr2nib['B'] = 0xb;
    chr2nib['C'] = 0xc;
    chr2nib['D'] = 0xd;
    chr2nib['E'] = 0xe;
    chr2nib['F'] = 0xf;
    
    chr2nib['a'] = 0xa;
    chr2nib['b'] = 0xb;
    chr2nib['c'] = 0xc;
    chr2nib['d'] = 0xd;
    chr2nib['e'] = 0xe;
    chr2nib['f'] = 0xf;
}

color read_palette_color(uint8_t idx){
    try {
        return {
            (uint8_t)(chr2nib[(*palettes[palette])[idx * 8 + 0]] << 4 | chr2nib[(*palettes[palette])[idx * 8 + 1]]),
            (uint8_t)(chr2nib[(*palettes[palette])[idx * 8 + 2]] << 4 | chr2nib[(*palettes[palette])[idx * 8 + 3]]),
            (uint8_t)(chr2nib[(*palettes[palette])[idx * 8 + 4]] << 4 | chr2nib[(*palettes[palette])[idx * 8 + 5]]),
            255
        };
    } catch (std::out_of_range){
        std::cout << "Index out of range for palette";
        exit(1);
    }
}
#endif // PALLETES_H