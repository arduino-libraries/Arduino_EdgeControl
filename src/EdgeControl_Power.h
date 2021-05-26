/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
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

    [[deprecated]] inline void enable5V() { on(PWR_VBAT); };
    [[deprecated]] inline bool status5V() { return status(PWR_VBAT); };
    [[deprecated]] inline void disable5V() { off(PWR_VBAT);  };
    [[deprecated]] inline bool set5V(bool st) { set(PWR_VBAT, st); return status(PWR_VBAT); };

    [[deprecated]] inline void enable12V() { on(PWR_VBAT); };
    [[deprecated]] inline bool status12V() { return status(PWR_VBAT); };
    [[deprecated]] inline void disable12V() { off(PWR_VBAT);; };
    [[deprecated]] inline bool set12V(bool st) { set(PWR_VBAT, st); return status(PWR_VBAT); };

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
