#pragma once

#include <Arduino_EdgeControl.h>
#include <TimeAlarms.h>
#include "TimeHelpers.h"
#include "SensorsData.h"

#include <map>
#include <list>

void setSystemClock(String date = __DATE__, String time = __TIME__);
void statusLCD();
void backlightOff(bool powerDown);
int getAverageInputsRead(int pin, size_t loops = 20);
float getAverage05VRead(int pin);
int getMoisturePerc(int pin);