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
#include <I2C.h>

#include "lib/ioexpander/TCA6424A.h"

enum {
    EXP_LED1 =              TCA6424A_P00,
    EXP_FAULT_SOLAR_PANEL = TCA6424A_P01,
    EXP_FAULT_5V =          TCA6424A_P02,
    EXP_ON_LCD =            TCA6424A_P03,
    EXP_DEMUX_SEL0 =        TCA6424A_P04,
    EXP_DEMUX_SEL1 =        TCA6424A_P05,
    EXP_DEMUX_SEL2 =        TCA6424A_P06,
    EXP_DEMUX_SEL3 =        TCA6424A_P07,

    EXP_WATERMARK_EN =      TCA6424A_P10,
    EXP_05V_INPUT_EN =      TCA6424A_P11,
    EXP_SENSOR_DISCHARGE =  TCA6424A_P12,
    EXP_SENSOR_HIGH_P =     TCA6424A_P13,
    EXP_PULSE_CNTL0 =       TCA6424A_P14,
    EXP_PULSE_CNTL1 =       TCA6424A_P15,
    EXP_PULSE_CNTL2 =       TCA6424A_P16,
    EXP_PULSE_CNTL3 =       TCA6424A_P17,

    EXP_LCD_BACKLIGHT =     TCA6424A_P20,
    EXP_LCD_D4 =            TCA6424A_P21,
    EXP_LCD_D5 =            TCA6424A_P22,
    EXP_LCD_D6 =            TCA6424A_P23,
    EXP_LCD_D7 =            TCA6424A_P24,
    EXP_LCD_RS =            TCA6424A_P25,
    EXP_LCD_EN =            TCA6424A_P26,
    EXP_LCD_RW =            TCA6424A_P27
};

class EdgeControl_IOExpanderClass {

public:
    EdgeControl_IOExpanderClass() = default;
    ~EdgeControl_IOExpanderClass() = default;

    bool begin();
    void end();

    operator bool();

    bool pinMode(pin_size_t pin, PinMode direction);

    bool digitalWrite(pin_size_t pin, PinStatus status);
    bool digitalWrite(pin_size_t pin, int status) { return digitalWrite(pin, (PinStatus)status); };

    PinStatus digitalRead(pin_size_t pin);

private:
    TCA6424A _tca {};

    bool _status3V3;
    bool _statusVBAT;
};

extern EdgeControl_IOExpanderClass Expander;