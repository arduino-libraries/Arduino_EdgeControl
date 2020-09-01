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

#include "OutdoorCarrier_Expander.h"
#include "OutdoorCarrier_Power.h"

bool OutdoorCarrier_IOExpanderClass::begin()
{
    _tca.initialize();
    return _tca.testConnection();
}

bool OutdoorCarrier_IOExpanderClass::pinMode(pin_size_t pin, PinMode direction)
{
    if (direction > OUTPUT)
        return false;

    _tca.setPinDirection(pin, direction == INPUT ? TCA6424A_INPUT : TCA6424A_OUTPUT);
    return true;
}

bool OutdoorCarrier_IOExpanderClass::digitalWrite(pin_size_t pin, PinStatus status)
{
    if (status > HIGH)
        return false;

    _tca.writePin(pin, status == HIGH ? TCA6424A_HIGH : TCA6424A_LOW);
    return true;
}

PinStatus OutdoorCarrier_IOExpanderClass::digitalRead(pin_size_t pin)
{
    return _tca.readPin(pin) == true ? HIGH : LOW;
}

OutdoorCarrier_IOExpanderClass Expander {};
