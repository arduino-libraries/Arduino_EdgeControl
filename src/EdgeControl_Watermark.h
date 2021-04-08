/*
  This file is part of the Arduino Edge Control library.
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
#include "EdgeControl_Expander.h"

enum : pin_size_t {
    WATERMARK_CH01 = 0,
    WATERMARK_CH02,
    WATERMARK_CH03,
    WATERMARK_CH04,
    WATERMARK_CH05,
    WATERMARK_CH06,
    WATERMARK_CH07,
    WATERMARK_CH08,
    WATERMARK_CH09,
    WATERMARK_CH10,
    WATERMARK_CH11,
    WATERMARK_CH12,
    WATERMARK_CH13,
    WATERMARK_CH14,
    WATERMARK_CH15,
    WATERMARK_CH16,
};

class EdgeControl_WatermarkClass {
public:
    EdgeControl_WatermarkClass() = default;
    ~EdgeControl_WatermarkClass() = default;

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

extern EdgeControl_WatermarkClass Watermark;