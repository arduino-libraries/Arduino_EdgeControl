/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0
  
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "TS13102.h"

enum TS13102_BITS : uint16_t {
    TS13102_RESET = 0x0000,
    TS13102_ZERO = 0x0005,
    TS13102_ONE = 0x0055
};

TS13102Packet::TS13102Packet()
    : _reset { TS13102_RESET }
    , _preamble { TS13102_ONE, TS13102_ONE, TS13102_ZERO }
    , _address { 0x00 }
    , _command { 0x00 }
    , _clock { TS13102_ONE, TS13102_ONE, TS13102_ONE, TS13102_ONE, TS13102_ONE }
    , _any { TS13102_RESET }
{
}

bool TS13102Packet::setAddress(const uint8_t address)
{
    if (address > 7)
        return false;
    toTSBits(address, _address, 3);

    return true;
}

bool TS13102Packet::setCommand(const uint8_t command)
{
    switch (command) {
    case TS13102_COMMAND_OFF_IMMEDIATE:
    case TS13102_COMMAND_OFF_ZERO_CROSSING:
    case TS13102_COMMAND_ON_IMMEDIATE:
    case TS13102_COMMAND_ON_ZERO_CROSSING:
    case TS13102_COMMAND_ON_IMMEDIATE_DITHERING:
    case TS13102_COMMAND_ON_ZERO_CROSSING_DITHERING:
    case TS13102_COMMAND_HEARTBEAT:
    case TS13102_COMMAND_POLL:
        toTSBits(command, _command, 4);
        return true;
    default:
        return false;
    }
}

void TS13102Packet::toTSBits(const uint8_t n, uint16_t* out, const size_t len) const
{
    // TODO: convert to a for-loop
    int i = len;

    while (i > 0) {
        out[len - i] = (n >> (i - 1)) & 1 ? TS13102_ONE : TS13102_ZERO;
        i--;
    }
}

size_t TS13102Packet::getBytes(uint8_t* out)
{
    const uint8_t* _start = out;
    size_t _len;

    _len = sizeof(_reset);
    memcpy(out, (uint8_t*)&_reset, _len);
    out += _len;

    _len = sizeof(_preamble);
    memcpy(out, (uint8_t*)&_preamble, _len);
    out += _len;

    _len = sizeof(_address);
    memcpy(out, (uint8_t*)&_address, _len);
    out += _len;

    _len = sizeof(_command);
    memcpy(out, (uint8_t*)&_command, _len);
    out += _len;

    _len = sizeof(_clock);
    memcpy(out, (uint8_t*)&_clock, _len);
    out += _len;

    _len = sizeof(_any);
    memcpy(out, (uint8_t*)&_any, _len);
    out += _len;

    return out - _start;
}

size_t TS13102Packet::getWords(uint16_t* out)
{
    const uint16_t* _start = out;
    size_t _len;

    _len = sizeof(_reset);
    memcpy(out, _reset, _len);
    out += _len / 2;

    _len = sizeof(_preamble);
    memcpy(out, _preamble, _len);
    out += _len / 2;

    _len = sizeof(_address);
    memcpy(out, _address, _len);
    out += _len / 2;

    _len = sizeof(_command);
    memcpy(out, _command, _len);
    out += _len / 2;

    _len = sizeof(_clock);
    memcpy(out, _clock, _len);
    out += _len / 2;

    _len = sizeof(_any);
    memcpy(out, _any, _len);
    out += _len / 2;

    return out - _start;
}

size_t TS13102Packet::length()
{
    return sizeof(TS13102Packet);
}

size_t TS13102Packet::lengthWords()
{
    return sizeof(TS13102Packet) / 2;
}
