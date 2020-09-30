/*
  This file is part of the Arduino Outdoor Carrier library.
  Copyright (C) 2017  Arduino AG (http://www.arduino.cc/)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <Arduino.h>
#include "OutdoorCarrier_Expander.h"

enum : pin_size_t {
    WATERMARKS_CH01 = 0,
    WATERMARKS_CH02,
    WATERMARKS_CH03,
    WATERMARKS_CH04,
    WATERMARKS_CH05,
    WATERMARKS_CH06,
    WATERMARKS_CH07,
    WATERMARKS_CH08,
    WATERMARKS_CH09,
    WATERMARKS_CH10,
    WATERMARKS_CH11,
    WATERMARKS_CH12,
    WATERMARKS_CH13,
    WATERMARKS_CH14,
    WATERMARKS_CH15,
    WATERMARKS_CH16,
};

class OutdoorCarrier_WatermarksClass {
public:
    OutdoorCarrier_WatermarksClass() = default;
    ~OutdoorCarrier_WatermarksClass() = default;

    void begin();
    void end();

    void enable();
    void disable();

    void commonWrite(PinStatus status);
    void commonMode(PinMode mode);

    void calibrationWrite(PinStatus status);
    void calibrationMode(PinMode mode);

    void fastDischarge(size_t duration);
    void setHighPrecision(bool precision);

    PinStatus digitalRead(pin_size_t sensor);
    int analogRead(pin_size_t sensor);

    void digitalWrite(pin_size_t sensor, PinStatus status);
    void analogWrite(pin_size_t sensor, int val);

private:    
    bool selectSensor(pin_size_t channel);

    void highPrecisionWrite(PinStatus status);
    void highPrecisionMode(PinMode mode);

    void fastDischargeWrite(PinStatus status);
    void fastDischargeMode(PinMode mode);

    bool _highPrecision { false } ;
    pin_size_t _channel;
};

extern OutdoorCarrier_WatermarksClass Watermarks;