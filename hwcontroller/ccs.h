#ifndef H_CCS
#define H_CCS

#include <Arduino.h>

extern bool advertiseCCS;

void setupCCS(uint16_t *fhtLeft, uint16_t *fhtFront, uint16_t *fhtRight);
void handleCCS();

#endif // H_CCS
