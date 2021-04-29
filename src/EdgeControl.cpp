/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
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