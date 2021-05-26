/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "EdgeControl_Latching.h"

void EdgeControl_LatchingClass::begin()
{
    ::pinMode(PULSE_DIRECTION, OUTPUT);
    ::pinMode(PULSE_STROBE, OUTPUT);

    while (!Expander.begin())
        ;

    Expander.pinMode(EXP_PULSE_CNTL0, OUTPUT);
    Expander.pinMode(EXP_PULSE_CNTL1, OUTPUT);
    Expander.pinMode(EXP_PULSE_CNTL2, OUTPUT);
    Expander.pinMode(EXP_PULSE_CNTL3, OUTPUT);

    _channel = 16;
}

void EdgeControl_LatchingClass::end()
{
    _channel = 16;
}

void EdgeControl_LatchingClass::latch()
{
    ::digitalWrite(PULSE_STROBE, HIGH);
}

void EdgeControl_LatchingClass::release()
{
    ::digitalWrite(PULSE_STROBE, LOW);
}

void EdgeControl_LatchingClass::strobe(unsigned int duration)
{
    latch();
    // delay(duration);
    for (auto go = millis() + duration; millis() <= go; yield());
    release();
}

bool EdgeControl_LatchingClass::selectSensor(pin_size_t channel)
{
    if (channel > 15)
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

void EdgeControl_LatchingClass::digitalWrite(pin_size_t pin, PinStatus val)
{
    selectSensor(pin);

    ::digitalWrite(PULSE_DIRECTION, val);
}

void EdgeControl_LatchingClass::channelDirection(pin_size_t channel, PulseDirection direction)
{
    PinStatus status { LOW };

    switch (direction) {
    case POSITIVE: {
        status = LOW;
        break;
    }
    case NEGATIVE: {
        status = HIGH;
        break;
    }
    default:
        break;
    }

    digitalWrite(channel, status);
}

EdgeControl_LatchingClass Latching {};
