#ifndef API_H
#define API_H

#include <Arduino.h>

void fetchEnergyPrices();
void parseAPIResponse(String response);

#endif

