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

#include "OutdoorCarrier_Power.h"

void OutdoorCarrier_PowerClass::begin()
{
    pinMode(GATED_VBAT_ENABLE, OUTPUT);
    pinMode(GATED_3V3_ENABLE_N, OUTPUT);
    pinMode(GATED_19V_ENABLE, OUTPUT);
}

void OutdoorCarrier_PowerClass::enable5V()
{
    digitalWrite(GATED_VBAT_ENABLE, HIGH);
    
    _status5V = true;
}

bool OutdoorCarrier_PowerClass::status5V()
{
    return _status5V;
}

void OutdoorCarrier_PowerClass::disable5V()
{
    digitalWrite(GATED_VBAT_ENABLE, LOW);
    
    _status5V = false;
}

void OutdoorCarrier_PowerClass::enable3V3()
{
    digitalWrite(GATED_3V3_ENABLE_N, LOW);

    _status3V3 = true;
}

bool OutdoorCarrier_PowerClass::status3V3()
{
    return _status3V3;
}

void OutdoorCarrier_PowerClass::disable3V3()
{
    digitalWrite(GATED_3V3_ENABLE_N, HIGH);

    _status3V3 = false;
}

void OutdoorCarrier_PowerClass::enable19V()
{
    digitalWrite(GATED_19V_ENABLE, HIGH);

    _status19V = true;
}

bool OutdoorCarrier_PowerClass::status19V()
{
    return _status19V;
}

void OutdoorCarrier_PowerClass::disable19V()
{
    digitalWrite(GATED_19V_ENABLE, LOW);

    _status19V = false;
}

float OutdoorCarrier_PowerClass::getVBat(const int adcResolution) const
{
    constexpr float vbatVD { 100.0f / (100.0f + 475.0f) }; // Voltage Divider on VBAT_PROBE
    float valToV { 3.3f / float { (1 << adcResolution) - 1 }};
    constexpr unsigned int count { 100 };

    int val { 0 };
    for (auto i = 0; i < count; i++) {
        val += analogRead(VBAT_PROBE);
        delay(10);
    }

    float vbat = val * valToV / count / 1.2f / vbatVD;
    // auto vbat = (float { val / count } * valToV) / vbatVD;    

    return vbat;
}

OutdoorCarrier_PowerClass Power {};
