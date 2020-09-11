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

#include "TS13102.h"

enum : uint16_t {
    TS13102_RESET = 0x0000,
    TS13102_ZERO = 0x0050,
    TS13102_ONE = 0x0055
}

enum TS13102_COMMANDS : uint8_t {
    TS13102_COMMAND_OFF_FIRST = 1,
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
    int i = len;

    while (i > 0) {
        out[len - i--] = (n >> (i - 1)) & 1 ? TS13102_ONE : TS13102_ZERO;
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

size_t TS13102Packet::length()
{
    return sizeof(TS13102Packet);
}