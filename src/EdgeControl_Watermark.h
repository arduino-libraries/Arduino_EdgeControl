/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
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