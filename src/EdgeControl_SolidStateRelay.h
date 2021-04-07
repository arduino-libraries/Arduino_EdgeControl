/*
  This file is part of the Arduino Edge Control library.
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

#include "EdgeControl_Expander.h"
#include "lib/solidstaterelay/TS13102.h"
#include <Arduino.h>
#include <SPI.h>

enum : pin_size_t {
    RELAYS_CH01 = 0,
    RELAYS_CH02 = 4,
    RELAYS_CH03 = 1,
    RELAYS_CH04 = 5,
};

class EdgeControl_SolidStateRelaysClass {
public:
    EdgeControl_SolidStateRelaysClass();

    void begin();
    void end();

    bool on(pin_size_t channel);
    bool off(pin_size_t channel);
    bool poll(pin_size_t channel);

private:
    TS13102Packet _relayPkt;

    // Use a fake SPI to communicate with the Solid State Relays
    // We need just MOSI. Point MISO and CLK to unused pins
    MbedSPI _relaySPI;

    bool doRelayCommand(pin_size_t address, TS13102_COMMANDS command);
    void doSPITransfer();
};

extern EdgeControl_SolidStateRelaysClass Relays;