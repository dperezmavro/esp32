#ifndef _SETUP_H
#define _SETUP_H

#include "./stratagem-manager.h"

extern StratagemManager* sm;

void setup_sd_card();
void setuip_esp_now();
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);

#endif