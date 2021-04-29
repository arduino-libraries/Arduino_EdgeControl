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

enum TS13102_COMMANDS : uint8_t {
    TS13102_COMMAND_FIRST = 1,
    TS13102_COMMAND_OFF_IMMEDIATE = 1,
    TS13102_COMMAND_OFF_ZERO_CROSSING = 2,
    TS13102_COMMAND_ON_IMMEDIATE = 3,
    TS13102_COMMAND_ON_ZERO_CROSSING = 4,
    TS13102_COMMAND_ON_IMMEDIATE_DITHERING = 5,
    TS13102_COMMAND_ON_ZERO_CROSSING_DITHERING = 6,
    TS13102_COMMAND_HEARTBEAT = 7,
    TS13102_COMMAND_POLL = 15,
    TS13102_COMMAND_LAST
};

class TS13102Packet {
public:
    TS13102Packet();

    bool setAddress(const uint8_t address);
    bool setCommand(const uint8_t command);

    size_t getBytes(uint8_t* out);
    size_t getWords(uint16_t* out);
    size_t length();
    size_t lengthWords();

private:
    const uint16_t _reset[3];
    const uint16_t _preamble[3];
    uint16_t _address[3];
    uint16_t _command[4];
    const uint16_t _clock[5];
    const uint16_t _any[1];

    void toTSBits(const uint8_t n, uint16_t* out, const size_t len) const;
};
