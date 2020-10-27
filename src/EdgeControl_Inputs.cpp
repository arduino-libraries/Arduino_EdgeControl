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

#include "EdgeControl_Inputs.h"

void EdgeControl_InputsClass::begin()
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

void EdgeControl_InputsClass::end()
{
    _channel = 16;
}

void EdgeControl_InputsClass::enable()
{
    Expander.digitalWrite(EXP_WATERMARK_EN, HIGH);
    Expander.digitalWrite(EXP_05V_INPUT_EN, LOW);
}

void EdgeControl_InputsClass::disable()
{
    Expander.digitalWrite(EXP_05V_INPUT_EN, HIGH);
}

bool EdgeControl_InputsClass::selectSensor(pin_size_t channel)
{
    if (0 > channel > 15)
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

PinStatus EdgeControl_InputsClass::digitalRead(pin_size_t pin)
{
    selectSensor(pin);

    return ::digitalRead(SENSOR_INPUT_ADC);
}

int EdgeControl_InputsClass::analogRead(pin_size_t pin)
{
    selectSensor(pin);

    return ::analogRead(SENSOR_INPUT_ADC);
}

EdgeControl_InputsClass Inputs {};
