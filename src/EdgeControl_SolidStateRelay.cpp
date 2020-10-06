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

#include "EdgeControl_SolidStateRelay.h"

EdgeControl_SolidStateRelaysClass::EdgeControl_SolidStateRelaysClass()
    : _relaySPI(digitalPinToPinName(CMD_TRIAC_DATA), digitalPinToPinName(CMD_TRIAC_CLK), P1_5)
    , _relayPkt()
{
}

void EdgeControl_SolidStateRelaysClass::begin()
{
    _relaySPI.begin();

    pin_size_t channels[] { RELAYS_CH01, RELAYS_CH02, RELAYS_CH03, RELAYS_CH04 };

    for (auto ch : channels)
        off(ch);
}

void EdgeControl_SolidStateRelaysClass::end()
{
    _relaySPI.end();
}

bool EdgeControl_SolidStateRelaysClass::on(pin_size_t address)
{
    return doRelayCommand(address, TS13102_COMMAND_ON_IMMEDIATE_DITHERING);
}

bool EdgeControl_SolidStateRelaysClass::off(pin_size_t address)
{
    return doRelayCommand(address, TS13102_COMMAND_OFF_IMMEDIATE);
}

bool EdgeControl_SolidStateRelaysClass::poll(pin_size_t address)
{
    return doRelayCommand(address, TS13102_COMMAND_POLL);
}

bool EdgeControl_SolidStateRelaysClass::doRelayCommand(pin_size_t address, TS13102_COMMANDS command)
{
    switch (address) {
    case RELAYS_CH01:
    case RELAYS_CH02:
    case RELAYS_CH03:
    case RELAYS_CH04:
        _relayPkt.setAddress(address);
        _relayPkt.setCommand(command);
        doSPITransfer();
        break;
    default:
        return false;
    }

    return true;
}

void EdgeControl_SolidStateRelaysClass::doSPITransfer()
{
    uint8_t data[sizeof(TS13102Packet)] { 0 };
    auto len = _relayPkt.getBytes(data);

    _relaySPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    _relaySPI.transfer((void*)data, len);
    _relaySPI.endTransaction();

    Serial.print("Data: ");
    for (auto i = 0; i < len; i++){
        if (data[i] < 16) Serial.print(0);
        Serial.print(data[i], HEX);
    }
    Serial.println();
}

EdgeControl_SolidStateRelaysClass Relays {};