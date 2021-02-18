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

#include "EdgeControl_Expander.h"

enum PowerRail: pin_size_t {
  PWR_VBAT = 0,
  PWR_3V3 = 1,
  PWR_19V = 2,
  PWR_MKR1 = 3,
  PWR_MKR2 = 4,
  PWR_RAILS_SIZE
};

class EdgeControl_PowerClass {

public:
    EdgeControl_PowerClass() = default;
    ~EdgeControl_PowerClass() = default;

    void begin();

    void set(const PowerRail rail, const bool status);
    void set(const pin_size_t rail, const bool status) { set((PowerRail)rail, status); };
    bool status(PowerRail rail) { return _statuses[rail]; };
    bool status(pin_size_t rail) { return _statuses[(PowerRail)rail]; };
    void on(PowerRail rail) { set(rail, true); };
    void on(pin_size_t rail) { set((PowerRail)rail, true); };
    void off(PowerRail rail) { set(rail, false); };
    void off(pin_size_t rail) { set((PowerRail)rail, false); };
    
    float getVBat(const int adcResolution) const ;
    float getVBat() const { return getVBat(10); };

    bool getSolarChargerStatus();
    bool get5VoltStatus();

    [[deprecated]] inline void enableVBat() { on(PWR_VBAT); };
    [[deprecated]] inline bool statusVBat() { return status(PWR_VBAT); };
    [[deprecated]] inline void disableVBat() { off(PWR_VBAT); };
    [[deprecated]] inline bool setVBat(bool st) { set(PWR_VBAT, st); return status(PWR_VBAT); };

    [[deprecated]] inline void enable5V() { enableVBat(); };
    [[deprecated]] inline bool status5V() { return statusVBat(); };
    [[deprecated]] inline void disable5V() { disableVBat();  };
    [[deprecated]] inline bool set5V(bool st) { return setVBat(st); };

    [[deprecated]] inline void enable12V() { enableVBat(); };
    [[deprecated]] inline bool status12V() { return statusVBat(); };
    [[deprecated]] inline void disable12V() { disableVBat(); };
    [[deprecated]] inline bool set12V(bool st) { return setVBat(st); };

    [[deprecated]] inline void enable3V3() { on(PWR_3V3); };
    [[deprecated]] inline bool status3V3() { return status(PWR_3V3); };
    [[deprecated]] inline void disable3V3() { off(PWR_3V3); };
    [[deprecated]] inline bool set3V3(bool st) { set(PWR_3V3, st); return status(PWR_3V3); };

    [[deprecated]] inline void enable19V() { on(PWR_19V); };
    [[deprecated]] inline bool status19V() { return status(PWR_19V); };
    [[deprecated]] inline void disable19V() { off(PWR_19V);  };
    [[deprecated]] inline bool set19V(bool st) { set(PWR_19V, st); return status(PWR_19V); };

private:
    bool _statusVBat;
    bool _status3V3;
    bool _status19V;
    bool _statusMkr1;
    bool _statusMkr2;
    bool _statuses[PWR_RAILS_SIZE];
};

extern EdgeControl_PowerClass Power;