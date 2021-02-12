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


#include "EdgeControl.h"
#include "EdgeControl_Power.h"

void EdgeControlClass::begin()
{
    Power.begin();
}

String EdgeControlClass::serialNumber()
{
    auto utox8 = [](uint32_t val, char* s) {
        for (int i = 0; i < 8; i++) {
            int d = val & 0XF;
            val = (val >> 4);

            s[7 - i] = d > 9 ? 'A' + d - 10 : '0' + d;
        }
    };

    const uint32_t SERIAL_NUMBER_WORD_0 { NRF_FICR->DEVICEADDR[1] };
    const uint32_t SERIAL_NUMBER_WORD_1 { NRF_FICR->DEVICEADDR[0] };

    char name[17] { 0 };

    utox8(SERIAL_NUMBER_WORD_0, &name[0]);
    utox8(SERIAL_NUMBER_WORD_1, &name[8]);

    return name;
}

EdgeControlClass EdgeControl;