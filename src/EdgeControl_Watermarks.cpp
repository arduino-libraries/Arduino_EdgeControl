/*
  This file is part of the Arduino Edge Control library.
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

#include <EdgeControl_Watermarks.h>

void EdgeControl_WatermarksClass::begin()
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

void EdgeControl_WatermarksClass::end()
{
    _channel = 16;
}

void EdgeControl_WatermarksClass::enable()
{
    Expander.digitalWrite(EXP_05V_INPUT_EN, HIGH);
    Expander.digitalWrite(EXP_WATERMARK_EN, LOW);
}

void EdgeControl_WatermarksClass::disable()
{
    Expander.digitalWrite(EXP_WATERMARK_EN, HIGH);
}

void EdgeControl_WatermarksClass::commonWrite(PinStatus status)
{
    ::digitalWrite(SENSOR_COMMON, status);
}

void EdgeControl_WatermarksClass::commonMode(PinMode mode)
{
    ::pinMode(SENSOR_COMMON, mode);
}

void EdgeControl_WatermarksClass::calibrationWrite(PinStatus status)
{
    ::digitalWrite(SENSOR_CALIB, status);
}

void EdgeControl_WatermarksClass::calibrationMode(PinMode mode)
{
    ::pinMode(SENSOR_CALIB, mode);
}

void EdgeControl_WatermarksClass::highPrecisionWrite(PinStatus status)
{
    Expander.digitalWrite(EXP_SENSOR_HIGH_P, status);
}

void EdgeControl_WatermarksClass::highPrecisionMode(PinMode mode)
{
    Expander.pinMode(EXP_SENSOR_HIGH_P, mode);
}

void EdgeControl_WatermarksClass::fastDischargeWrite(PinStatus status)
{
    Expander.digitalWrite(EXP_SENSOR_DISCHARGE, status);
}

void EdgeControl_WatermarksClass::fastDischargeMode(PinMode mode)
{
    Expander.pinMode(EXP_SENSOR_DISCHARGE, mode);
}

void EdgeControl_WatermarksClass::fastDischarge(size_t duration)
{
    fastDischargeWrite(HIGH);
    delay(_highPrecision ? duration * 2 : duration);
    fastDischargeWrite(LOW);
}

void EdgeControl_WatermarksClass::setHighPrecision(bool precision)
{
    highPrecisionWrite(precision ? HIGH : LOW);
    _highPrecision = precision;
}

bool EdgeControl_WatermarksClass::selectSensor(pin_size_t channel)
{
    if (0 > channel > 15)
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

PinStatus EdgeControl_WatermarksClass::digitalRead(pin_size_t pin)
{
    selectSensor(pin);

    return ::digitalRead(SENSOR_CAPTURE);
}

int EdgeControl_WatermarksClass::analogRead(pin_size_t pin)
{
    selectSensor(pin);

    return ::analogRead(SENSOR_CAPTURE_A);
}

void EdgeControl_WatermarksClass::digitalWrite(pin_size_t pin, PinStatus status)
{
    ::pinMode(SENSOR_CAPTURE, OUTPUT);

    selectSensor(pin);

    ::digitalWrite(SENSOR_CAPTURE, status);
    ::pinMode(SENSOR_CAPTURE, INPUT);
}

void EdgeControl_WatermarksClass::analogWrite(pin_size_t pin, int val)
{
    ::pinMode(SENSOR_CAPTURE_A, OUTPUT);

    selectSensor(pin);

    ::analogWrite(SENSOR_CAPTURE_A, val);
    ::pinMode(SENSOR_CAPTURE_A, INPUT);
}

EdgeControl_WatermarksClass Watermarks {};
