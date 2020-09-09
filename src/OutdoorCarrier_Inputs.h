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
    INPUTS_05V_CH01 = 0,
    INPUTS_05V_CH02,
    INPUTS_05V_CH03,
    INPUTS_05V_CH04,
    INPUTS_05V_CH05,
    INPUTS_05V_CH06,
    INPUTS_05V_CH07,
    INPUTS_05V_CH08,

    INPUTS_420mA_CH01 = 8,
    INPUTS_420mA_CH02,
    INPUTS_420mA_CH03,
    INPUTS_420mA_CH04,

    INPUTS_19V_REF = 15,
};

class OutdoorCarrier_InputsClass {
public:
    OutdoorCarrier_InputsClass() = default;
    ~OutdoorCarrier_InputsClass() = default;

    void begin();

    void enable();
    void disable();

    PinStatus digitalRead(pin_size_t sensor);
    int analogRead(pin_size_t sensor);

private:
    bool selectSensor(pin_size_t channel);

    pin_size_t _channel { 0 };
};

extern OutdoorCarrier_InputsClass Inputs;