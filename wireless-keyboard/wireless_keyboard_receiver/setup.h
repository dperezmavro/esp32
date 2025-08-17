#ifndef _SETUP_H
#define _SETUP_H

#include "./stratagem-manager.h"

extern StratagemManager* sm;

const char* read_file(fs::FS& fs, const char* path);
void setuip_esp_now();
void setup_sd_card();
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);

#endif