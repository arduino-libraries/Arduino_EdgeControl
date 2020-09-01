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

class OutdoorCarrier_PowerClass {

public:
    OutdoorCarrier_PowerClass() = default;
    ~OutdoorCarrier_PowerClass() = default;

    void begin();

    void enable5V();
    bool status5V();
    void disable5V();

    void enable3V3();
    bool status3V3();
    void disable3V3();

    void enable19V();
    bool status19V();
    void disable19V();

    float getVBat(const int adcResolution) const ;
    float getVBat() const { return getVBat(10); };

private:
    bool _status5V;    
    bool _status3V3;    
    bool _status19V;    
    bool _status12V;    
};

extern OutdoorCarrier_PowerClass Power;