#include <string>

#ifndef CONCOLOR_H
#define CONCOLOR_H

#if defined(__unix__) || defined(__unix) || defined(__linux__)
#define OS_LINUX
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define OS_WIN
#elif defined(__APPLE__) || defined(__MACH__)
#define OS_MAC
#else
#error Unknown Platform
#endif

#if defined(OS_LINUX) || defined(OS_MAC)
#include <unistd.h>

void concolor_setup(){};

#elif defined(OS_WIN)
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void concolor_setup(){
    DWORD dwMode = 0;
    bool _ = GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    _ = SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), dwMode);
}

#endif

std::string concolor_fg_rgb(uint8_t r, uint8_t g, uint8_t b){
    return "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

std::string concolor_reset(){
    return "\x1b[0m";
}

#endif // CONCOLOR_H