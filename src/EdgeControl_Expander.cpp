/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0
  
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "EdgeControl_Expander.h"
#include "EdgeControl_Power.h"

bool EdgeControl_IOExpanderClass::begin()
{
    _status3V3 = Power.status(PWR_3V3);
    _statusVBAT = Power.status(PWR_VBAT);

    if(!_status3V3)
        Power.on(PWR_3V3);

    if(!_statusVBAT)
        Power.on(PWR_VBAT);

    Wire.begin();

    _tca.initialize();
    auto status = _tca.testConnection();

    // if (status)
    //     _tca.setAllDirection(TCA6424A_INPUT, TCA6424A_INPUT, TCA6424A_INPUT);
    
    return status;
}

EdgeControl_IOExpanderClass::operator bool()
{
    return _tca.testConnection();
}

void EdgeControl_IOExpanderClass::end()
{
    _tca.setAllDirection(TCA6424A_INPUT, TCA6424A_INPUT, TCA6424A_INPUT);

    Power.set(PWR_3V3, _status3V3);
    Power.set(PWR_VBAT, _statusVBAT);
}


bool EdgeControl_IOExpanderClass::pinMode(pin_size_t pin, PinMode direction)
{
    if (direction > OUTPUT)
        return false;

    _tca.setPinDirection(pin, direction == INPUT ? TCA6424A_INPUT : TCA6424A_OUTPUT);
    return true;
}

bool EdgeControl_IOExpanderClass::digitalWrite(pin_size_t pin, PinStatus status)
{
    if (status > HIGH)
        return false;

    _tca.writePin(pin, status == HIGH ? TCA6424A_HIGH : TCA6424A_LOW);
    return true;
}

PinStatus EdgeControl_IOExpanderClass::digitalRead(pin_size_t pin)
{
    return _tca.readPin(pin) == true ? HIGH : LOW;
}

EdgeControl_IOExpanderClass Expander {};
