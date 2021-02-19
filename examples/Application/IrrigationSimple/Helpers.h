#pragma once

#include "SensorsData.h"
#include "TimeHelpers.h"
#include <Arduino_EdgeControl.h>
#include <TimeAlarms.h>

#include <list>
#include <map>

void setSystemClock(String date = __DATE__, String time = __TIME__);
void statusPrint();
int getAverageInputsRead(int pin, size_t loops = 20);
float getAverage05VRead(int pin);
int getMoisturePerc(int pin);

extern std::list<AlarmID_t> alarmTabIDs;
extern std::list<AlarmID_t> alarmSketchIDs;
extern std::list<DataPoint> dataPoints;