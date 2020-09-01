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
#include <Wire.h>

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

class OutdoorCarrier_IOExpanderClass {

public:
    OutdoorCarrier_IOExpanderClass() = default;
    ~OutdoorCarrier_IOExpanderClass() = default;

    bool begin();

    bool pinMode(pin_size_t pin, PinMode direction);
    bool digitalWrite(pin_size_t pin, PinStatus status);
    bool digitalWrite(pin_size_t pin, int status) { return digitalWrite(pin, (PinStatus)status); };
    PinStatus digitalRead(pin_size_t pin);

private:
    TCA6424A _tca {};
};

extern OutdoorCarrier_IOExpanderClass Expander;