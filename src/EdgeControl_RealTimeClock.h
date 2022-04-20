/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2022 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0
  
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <mbed.h>
#include <mbed_mktime.h>
#include <time.h>

#define IRQ_RTC IRQ_CH6

class EdgeControl_RealTimeClockClass {

public:
    EdgeControl_RealTimeClockClass();
    bool begin();
    void setYears(uint8_t years);
    void setMonths(uint8_t months);
    void setDays(uint8_t days);
    void setHours(uint8_t hours);
    void setMinutes(uint8_t minutes);
    void setSeconds(uint8_t seconds);

    uint8_t getYears();
    uint8_t getMonths();
    uint8_t getDays();
    uint8_t getHours();
    uint8_t getMinutes();
    uint8_t getSeconds();

    void setEpoch();
    void setEpoch(uint8_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
    void setEpoch(time_t seconds);
    time_t getEpoch();

    void enableAlarm();
    void disableAlarm();
    void clearAlarm();
    void setMinuteAlarm(uint8_t minutes);
    void disableMinuteAlarm();
    void setHourAlarm(uint8_t hours);
    void disableHourAlarm();
    void setDayAlarm(uint8_t days);
    void disableDayAlarm();

private:
    void writeByte(uint8_t regAddres, uint8_t data);
    uint8_t readByte(uint8_t regAddres);
};

extern EdgeControl_RealTimeClockClass RealTimeClock;
