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

class OutdoorCarrier_LatchingClass {
public:
    OutdoorCarrier_LatchingClass() = default;
    ~OutdoorCarrier_LatchingClass() = default;

    void begin();
    void end();

    void enable();
    void disable();
    void strobe(unsigned int duration);

    void digitalWrite(pin_size_t pin, PinStatus val);
    void pulse(pin_size_t channel, PulseDirection dir);

private:
    bool selectSensor(pin_size_t channel);

    pin_size_t _channel;
};

extern OutdoorCarrier_LatchingClass Latching;