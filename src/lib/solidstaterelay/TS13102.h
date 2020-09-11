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

#include <Arduino.h>

class TS13102Packet {
public:
    TS13102Packet() = default;
    ~TS13102Packet() = default;

    bool setAddress(const uint8_t address);
    bool setCommand(const TS13102_COMMANDS command);

    size_t getBytes(uint8_t* out);
    size_t length();

private:
    const uint16_t _reset[3];
    const uint16_t _preamble[3];
    uint16_t _address[3];
    uint16_t _command[4];
    const uint16_t _clock[5];
    const uint16_t _any[1];

    void toTSBits(const uint8_t n, uint16_t* out, const size_t len) const;
}
