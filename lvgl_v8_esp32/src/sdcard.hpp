#ifndef SDCARD_H
#define SDCARD_H


#include <SD.h>
#include <FS.h>
#include <SPI.h>


// void pin_init(void);
void sdcard_setup(void);
void readFile(fs::FS &fs, const char * path);

#ifdef __cplusplus

extern "C" {
    
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif