#pragma once

#include <Arduino.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Arduino_EdgeControl.h>

#include <map>
#include <list>

#include "TimeHelpers.h"

void backlightOff(bool powerDown);
void openValve();
void closeValve();
void displayClock();
void blinkLed();
void getSensors();
void saveData();

/**
 * Define your task function and add it to the commandMap.
 * The key of the map is the name to be used in the alarmtab file.
 */
static std::map<String, OnTick_t> commandMap {
    { "OpenValve", openValve },
    { "CloseValve", closeValve },
    { "DisplayClock", displayClock },
    { "BlinkLed", blinkLed },
    { "GetSensors", getSensors },
    { "SaveData", saveData },
};