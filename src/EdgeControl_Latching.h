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
    LATCHING_OUT_1 = 0,
    LATCHING_OUT_2,
    LATCHING_OUT_3,
    LATCHING_OUT_4,
    LATCHING_OUT_5,
    LATCHING_OUT_6,
    LATCHING_OUT_7,
    LATCHING_OUT_8,

    LATCHING_CMD_1 = 8,
    LATCHING_CMD_2,
    LATCHING_CMD_3,
    LATCHING_CMD_4,
    LATCHING_CMD_5,
    LATCHING_CMD_6,
    LATCHING_CMD_7,
    LATCHING_CMD_8,
};

enum PulseDirection {
    POSITIVE,
    NEGATIVE
};

class EdgeControl_LatchingClass {
public:
    EdgeControl_LatchingClass() = default;
    ~EdgeControl_LatchingClass() = default;

    void begin();
    void end();

    void channelDirection(pin_size_t channel, PulseDirection dir);
    void latch();
    void release();
    void strobe(unsigned int duration);

private:
    bool selectSensor(pin_size_t channel);
    void digitalWrite(pin_size_t pin, PinStatus val);

    pin_size_t _channel;
};

extern EdgeControl_LatchingClass Latching;