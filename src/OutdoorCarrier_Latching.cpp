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

#include "OutdoorCarrier_Latching.h"

void OutdoorCarrier_LatchingClass::begin()
{
    ::pinMode(PULSE_DIRECTION, OUTPUT);
    ::pinMode(PULSE_STROBE, OUTPUT);

    Expander.pinMode(EXP_PULSE_CNTL0, OUTPUT);
    Expander.pinMode(EXP_PULSE_CNTL1, OUTPUT);
    Expander.pinMode(EXP_PULSE_CNTL2, OUTPUT);
    Expander.pinMode(EXP_PULSE_CNTL3, OUTPUT);
}

void OutdoorCarrier_LatchingClass::enable()
{
    ::digitalWrite(PULSE_STROBE, HIGH);
}

void OutdoorCarrier_LatchingClass::disable()
{
    ::digitalWrite(PULSE_STROBE, LOW);
}

void OutdoorCarrier_LatchingClass::strobe(unsigned int duration)
{
    enable();
    delay(duration);
    disable();
}


bool OutdoorCarrier_LatchingClass::selectSensor(pin_size_t channel)
{
    if (0 > channel > 15)
        return false;

    if (_channel == channel)
        return true;

    _channel = channel;

    Expander.digitalWrite(EXP_PULSE_CNTL0, (_channel >> 0) & 1 ? HIGH : LOW);
    Expander.digitalWrite(EXP_PULSE_CNTL1, (_channel >> 1) & 1 ? HIGH : LOW);
    Expander.digitalWrite(EXP_PULSE_CNTL2, (_channel >> 2) & 1 ? HIGH : LOW);
    Expander.digitalWrite(EXP_PULSE_CNTL3, (_channel >> 3) & 1 ? HIGH : LOW);

    return true;
}

void OutdoorCarrier_LatchingClass::digitalWrite(pin_size_t pin, PinStatus val)
{
    selectSensor(pin);

    ::digitalWrite(PULSE_DIRECTION, val);
}

void OutdoorCarrier_LatchingClass::pulse(pin_size_t channel, PulseDirection direction)
{
    PinStatus status;

    switch(direction) {
        case POSITIVE: status = LOW; break;
        case NEGATIVE: status = HIGH; break;
        default: break;
    }

    digitalWrite(channel, status);
}

OutdoorCarrier_LatchingClass Latching {};