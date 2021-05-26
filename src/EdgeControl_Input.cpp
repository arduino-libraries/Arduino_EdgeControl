/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "EdgeControl_Input.h"

void EdgeControl_InputClass::begin()
{

    while (!Expander.begin())
        ;

    ::pinMode(SENSOR_INPUT_ADC, INPUT);

    Expander.pinMode(EXP_05V_INPUT_EN, OUTPUT);
    Expander.pinMode(EXP_WATERMARK_EN, OUTPUT);

    Expander.pinMode(EXP_DEMUX_SEL0, OUTPUT);
    Expander.pinMode(EXP_DEMUX_SEL1, OUTPUT);
    Expander.pinMode(EXP_DEMUX_SEL2, OUTPUT);
    Expander.pinMode(EXP_DEMUX_SEL3, OUTPUT);

    _channel = 16;
}

void EdgeControl_InputClass::end()
{
    _channel = 16;
}

void EdgeControl_InputClass::enable()
{
    Expander.digitalWrite(EXP_WATERMARK_EN, HIGH);
    Expander.digitalWrite(EXP_05V_INPUT_EN, LOW);
}

void EdgeControl_InputClass::disable()
{
    Expander.digitalWrite(EXP_05V_INPUT_EN, HIGH);
}

bool EdgeControl_InputClass::selectSensor(pin_size_t channel)
{
    if (channel > 15)
        return false;

    if (_channel == channel)
        return true;

    _channel = channel;

    Expander.digitalWrite(EXP_DEMUX_SEL0, (_channel >> 0) & 1 ? HIGH : LOW);
    Expander.digitalWrite(EXP_DEMUX_SEL1, (_channel >> 1) & 1 ? HIGH : LOW);
    Expander.digitalWrite(EXP_DEMUX_SEL2, (_channel >> 2) & 1 ? HIGH : LOW);
    Expander.digitalWrite(EXP_DEMUX_SEL3, (_channel >> 3) & 1 ? HIGH : LOW);

    return true;
}

PinStatus EdgeControl_InputClass::digitalRead(pin_size_t pin)
{
    selectSensor(pin);

    return ::digitalRead(SENSOR_INPUT_ADC);
}

int EdgeControl_InputClass::analogRead(pin_size_t pin)
{
    selectSensor(pin);

    return ::analogRead(SENSOR_INPUT_ADC);
}

EdgeControl_InputClass Input {};
