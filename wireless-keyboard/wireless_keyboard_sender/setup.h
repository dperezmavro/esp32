#ifndef _SETUP_H
#define _SETUP_H

#include "./common.h"

extern data_packet command;

int setuip_esp_now();
void send_command();

#endif