#ifndef UTIL_H
#define UTIL_H

#pragma once

// #define DEBUG_LOGGING 1
enum LogType {
    log_debug,
    log_always
};

// Print
void printByte(const char * message, unsigned char byte);
void printByte_debug(const char * message, unsigned char byte);

// File read / write
unsigned char * getROM(const char* filepath, int &romsize);
void createConvertedROM(const char * nameROM, unsigned char *ROM, int size);
    
#endif