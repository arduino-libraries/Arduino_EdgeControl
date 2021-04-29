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
    INPUT_05V_CH01 = 0,
    INPUT_05V_CH02,
    INPUT_05V_CH03,
    INPUT_05V_CH04,
    INPUT_05V_CH05,
    INPUT_05V_CH06,
    INPUT_05V_CH07,
    INPUT_05V_CH08,

    INPUT_420mA_CH01 = 8,
    INPUT_420mA_CH02,
    INPUT_420mA_CH03,
    INPUT_420mA_CH04,

    INPUT_19V_REF = 15,
};

class EdgeControl_InputClass {
public:
    EdgeControl_InputClass() = default;
    ~EdgeControl_InputClass() = default;

    void begin();
    void end();

    void enable();
    void disable();

    PinStatus digitalRead(pin_size_t sensor);
    int analogRead(pin_size_t sensor);

private:
    bool selectSensor(pin_size_t channel);

    pin_size_t _channel;
};

extern EdgeControl_InputClass Input;