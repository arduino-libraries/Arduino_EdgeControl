/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <EdgeControl_Watermark.h>

void EdgeControl_WatermarkClass::begin()
{
    highPrecisionMode(OUTPUT);
    highPrecisionWrite(LOW);

    fastDischargeMode(OUTPUT);
    fastDischargeWrite(LOW);

    ::pinMode(SENSOR_CAPTURE, INPUT);
    ::pinMode(SENSOR_CAPTURE_A, INPUT);

    while (!Expander.begin())
        ;

    Expander.pinMode(EXP_05V_INPUT_EN, OUTPUT);
    Expander.pinMode(EXP_WATERMARK_EN, OUTPUT);

    Expander.pinMode(EXP_DEMUX_SEL0, OUTPUT);
    Expander.pinMode(EXP_DEMUX_SEL1, OUTPUT);
    Expander.pinMode(EXP_DEMUX_SEL2, OUTPUT);
    Expander.pinMode(EXP_DEMUX_SEL3, OUTPUT);

    _channel = 16;
}

void EdgeControl_WatermarkClass::end()
{
    _channel = 16;
}

void EdgeControl_WatermarkClass::enable()
{
    Expander.digitalWrite(EXP_05V_INPUT_EN, HIGH);
    Expander.digitalWrite(EXP_WATERMARK_EN, LOW);
}

void EdgeControl_WatermarkClass::disable()
{
    Expander.digitalWrite(EXP_WATERMARK_EN, HIGH);
}

void EdgeControl_WatermarkClass::commonWrite(PinStatus status)
{
    ::digitalWrite(SENSOR_COMMON, status);
}

void EdgeControl_WatermarkClass::commonMode(PinMode mode)
{
    ::pinMode(SENSOR_COMMON, mode);
}

void EdgeControl_WatermarkClass::calibrationWrite(PinStatus status)
{
    ::digitalWrite(SENSOR_CALIB, status);
}

void EdgeControl_WatermarkClass::calibrationMode(PinMode mode)
{
    ::pinMode(SENSOR_CALIB, mode);
}

void EdgeControl_WatermarkClass::highPrecisionWrite(PinStatus status)
{
    Expander.digitalWrite(EXP_SENSOR_HIGH_P, status);
}

void EdgeControl_WatermarkClass::highPrecisionMode(PinMode mode)
{
    Expander.pinMode(EXP_SENSOR_HIGH_P, mode);
}

void EdgeControl_WatermarkClass::fastDischargeWrite(PinStatus status)
{
    Expander.digitalWrite(EXP_SENSOR_DISCHARGE, status);
}

void EdgeControl_WatermarkClass::fastDischargeMode(PinMode mode)
{
    Expander.pinMode(EXP_SENSOR_DISCHARGE, mode);
}

void EdgeControl_WatermarkClass::fastDischarge(size_t duration)
{
    fastDischargeWrite(HIGH);
    delay(_highPrecision ? duration * 2 : duration);
    fastDischargeWrite(LOW);
}

void EdgeControl_WatermarkClass::setHighPrecision(bool precision)
{
    highPrecisionWrite(precision ? HIGH : LOW);
    _highPrecision = precision;
}

bool EdgeControl_WatermarkClass::selectSensor(pin_size_t channel)
{
    if (channel > 15)
        return false;

    if (_channel == channel)
        return true;

    _channel = channel;

    Expander.digitalWrite(EXP_DEMUX_SEL0, (_channel >> 0) & 1 ? HIGH : LOW);
    Expander.digitalWrite(EXP_DEMUX_SEL1, (_channel >> 1) & 1 ? HIGH : LOW);
    Expander.digitalWrite(EXP_DEMUX_SEL2, (_channel >> 2) & 1 ? HIGH : LOW);
    Expander.digitalWrite(EXP_DEMUX_SEL3, (_channel >> 3) & 1 ? HIGH : LOW);

    return true;
}

PinStatus EdgeControl_WatermarkClass::digitalRead(pin_size_t pin)
{
    selectSensor(pin);

    return ::digitalRead(SENSOR_CAPTURE);
}

int EdgeControl_WatermarkClass::analogRead(pin_size_t pin)
{
    selectSensor(pin);

    return ::analogRead(SENSOR_CAPTURE_A);
}

void EdgeControl_WatermarkClass::digitalWrite(pin_size_t pin, PinStatus status)
{
    ::pinMode(SENSOR_CAPTURE, OUTPUT);

    selectSensor(pin);

    ::digitalWrite(SENSOR_CAPTURE, status);
    ::pinMode(SENSOR_CAPTURE, INPUT);
}

void EdgeControl_WatermarkClass::analogWrite(pin_size_t pin, int val)
{
    ::pinMode(SENSOR_CAPTURE_A, OUTPUT);

    selectSensor(pin);

    ::analogWrite(SENSOR_CAPTURE_A, val);
    ::pinMode(SENSOR_CAPTURE_A, INPUT);
}

EdgeControl_WatermarkClass Watermark {};
