/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "EdgeControl_Expander.h"
#include "lib/solidstaterelay/TS13102.h"
#include <Arduino.h>
#include <SPI.h>

enum : pin_size_t {
    RELAY_CH01 = 0,
    RELAY_CH02 = 4,
    RELAY_CH03 = 1,
    RELAY_CH04 = 5,
};

class EdgeControl_SolidStateRelayClass {
public:
    EdgeControl_SolidStateRelayClass();

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

extern EdgeControl_SolidStateRelayClass Relay;