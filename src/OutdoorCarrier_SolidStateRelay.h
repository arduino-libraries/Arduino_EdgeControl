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

#include "OutdoorCarrier_Expander.h"
#include "lib/solidstaterelay/TS13102.h"
#include <Arduino.h>

class OutdoorCarrier_SolidStateRelayClass {
public:
    OutdoorCarrier_SolidStateRelayClass() = default;
    ~OutdoorCarrier_SolidStateRelayClass() = default;

    bool begin();

    void on();
    void off();

private:
    TS13102Packet _pck {};
}

extern OutdoorCarrier_SolidStateRelayClass Relay;