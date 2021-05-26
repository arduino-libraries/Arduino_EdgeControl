/*
  This file is part of the Arduino Edge Control library.
  Copyright (C) 2021 Arduino AG (http://www.arduino.cc/)
  SPDX-License-Identifier: MPL-2.0

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "EdgeControl_LiquidCrystal.h"

#include "Arduino.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// EdgeControl_LiquidCrystalClass constructor is called).

EdgeControl_LiquidCrystalClass::EdgeControl_LiquidCrystalClass(int8_t pwr, uint8_t rs, uint8_t rw, uint8_t enable,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    init(1, pwr, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}

void EdgeControl_LiquidCrystalClass::init(uint8_t fourbitmode, int8_t pwr, uint8_t rs, uint8_t rw, uint8_t enable,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
    _pwr_pin = pwr;
    _rs_pin = rs;
    _rw_pin = rw;
    _enable_pin = enable;

    _data_pins[0] = d0;
    _data_pins[1] = d1;
    _data_pins[2] = d2;
    _data_pins[3] = d3;
    _data_pins[4] = d4;
    _data_pins[5] = d5;
    _data_pins[6] = d6;
    _data_pins[7] = d7;

    if (fourbitmode)
        _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else
        _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

void EdgeControl_LiquidCrystalClass::begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if (lines > 1) {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;

    setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
        _displayfunction |= LCD_5x10DOTS;
    }

    while (!Expander.begin())
        ;

    if (_pwr_pin > 0) {
        Expander.pinMode(_pwr_pin, OUTPUT);
        Expander.digitalWrite(_pwr_pin, HIGH);
    }

    Expander.pinMode(_rs_pin, OUTPUT);
    // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
    if (_rw_pin != 255) {
        Expander.pinMode(_rw_pin, OUTPUT);
    }
    Expander.pinMode(_enable_pin, OUTPUT);

    // Do these once, instead of every time a character is drawn for speed reasons.
    for (int i = 0; i < ((_displayfunction & LCD_8BITMODE) ? 8 : 4); ++i) {
        Expander.pinMode(_data_pins[i], OUTPUT);
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
    delayMicroseconds(50000);
    // Now we pull both RS and R/W low to begin commands
    Expander.digitalWrite(_rs_pin, LOW);
    Expander.digitalWrite(_enable_pin, LOW);
    if (_rw_pin != 255) {
        Expander.digitalWrite(_rw_pin, LOW);
    }

    //put the LCD into 4 bit or 8 bit mode
    if (!(_displayfunction & LCD_8BITMODE)) {
        // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        write4bits(0x03);
        delayMicroseconds(4500); // wait min 4.1ms

        // second try
        write4bits(0x03);
        delayMicroseconds(4500); // wait min 4.1ms

        // third go!
        write4bits(0x03);
        delayMicroseconds(150);

        // finally, set to 4-bit interface
        write4bits(0x02);
    } else {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        command(LCD_FUNCTIONSET | _displayfunction);
        delayMicroseconds(4500); // wait more than 4.1ms

        // second try
        command(LCD_FUNCTIONSET | _displayfunction);
        delayMicroseconds(150);

        // third go
        command(LCD_FUNCTIONSET | _displayfunction);
    }

    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);
}

void EdgeControl_LiquidCrystalClass::setRowOffsets(int row0, int row1, int row2, int row3)
{
    _row_offsets[0] = row0;
    _row_offsets[1] = row1;
    _row_offsets[2] = row2;
    _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void EdgeControl_LiquidCrystalClass::clear()
{
    command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
    delayMicroseconds(2000); // this command takes a long time!
}

void EdgeControl_LiquidCrystalClass::home()
{
    command(LCD_RETURNHOME); // set cursor position to zero
    delayMicroseconds(2000); // this command takes a long time!
}

void EdgeControl_LiquidCrystalClass::setCursor(uint8_t col, uint8_t row)
{
    const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
    if (row >= max_lines) {
        row = max_lines - 1; // we count rows starting w/0
    }
    if (row >= _numlines) {
        row = _numlines - 1; // we count rows starting w/0
    }

    command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void EdgeControl_LiquidCrystalClass::noDisplay()
{
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void EdgeControl_LiquidCrystalClass::display()
{
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void EdgeControl_LiquidCrystalClass::noCursor()
{
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void EdgeControl_LiquidCrystalClass::cursor()
{
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void EdgeControl_LiquidCrystalClass::noBlink()
{
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void EdgeControl_LiquidCrystalClass::blink()
{
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void EdgeControl_LiquidCrystalClass::scrollDisplayLeft(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void EdgeControl_LiquidCrystalClass::scrollDisplayRight(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void EdgeControl_LiquidCrystalClass::leftToRight(void)
{
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void EdgeControl_LiquidCrystalClass::rightToLeft(void)
{
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void EdgeControl_LiquidCrystalClass::autoscroll(void)
{
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void EdgeControl_LiquidCrystalClass::noAutoscroll(void)
{
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void EdgeControl_LiquidCrystalClass::createChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}

/*********** mid level commands, for sending data/cmds */

inline void EdgeControl_LiquidCrystalClass::command(uint8_t value)
{
    send(value, LOW);
}

inline size_t EdgeControl_LiquidCrystalClass::write(uint8_t value)
{
    send(value, HIGH);
    return 1; // assume success
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void EdgeControl_LiquidCrystalClass::send(uint8_t value, uint8_t mode)
{
    Expander.digitalWrite(_rs_pin, mode);

    // if there is a RW pin indicated, set it low to Write
    if (_rw_pin != 255) {
        Expander.digitalWrite(_rw_pin, LOW);
    }

    if (_displayfunction & LCD_8BITMODE) {
        write8bits(value);
    } else {
        write4bits(value >> 4);
        write4bits(value);
    }
}

void EdgeControl_LiquidCrystalClass::pulseEnable(void)
{
    Expander.digitalWrite(_enable_pin, LOW);
    delayMicroseconds(1);
    Expander.digitalWrite(_enable_pin, HIGH);
    delayMicroseconds(1); // enable pulse must be >450ns
    Expander.digitalWrite(_enable_pin, LOW);
    delayMicroseconds(100); // commands need > 37us to settle
}

void EdgeControl_LiquidCrystalClass::write4bits(uint8_t value)
{
    for (int i = 0; i < 4; i++) {
        Expander.digitalWrite(_data_pins[i], (value >> i) & 0x01);
    }

    pulseEnable();
}

void EdgeControl_LiquidCrystalClass::write8bits(uint8_t value)
{
    for (int i = 0; i < 8; i++) {
        Expander.digitalWrite(_data_pins[i], (value >> i) & 0x01);
    }

    pulseEnable();
}

void EdgeControl_LiquidCrystalClass::backlight()
{
    Expander.pinMode(EXP_LCD_BACKLIGHT, OUTPUT);
    Expander.digitalWrite(EXP_LCD_BACKLIGHT, HIGH);
}

void EdgeControl_LiquidCrystalClass::noBacklight()
{
    Expander.pinMode(EXP_LCD_BACKLIGHT, OUTPUT);
    Expander.digitalWrite(EXP_LCD_BACKLIGHT, LOW);
}

EdgeControl_LiquidCrystalClass LCD(EXP_ON_LCD, EXP_LCD_RS, EXP_LCD_RW, EXP_LCD_EN, EXP_LCD_D4, EXP_LCD_D5, EXP_LCD_D6, EXP_LCD_D7);
