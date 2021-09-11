/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2020 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef MICROPY_INCLUDED_CODAL_PORT_DRV_PCA9685_H
#define MICROPY_INCLUDED_CODAL_PORT_DRV_PCA9685_H

#include <stdio.h>

#define PCA9684_ADDRESS             0x40

#define PCA9684_MODE1               0x00
#define PCA9684_MODE2               0x01
#define PCA9684_SUBADR1             0x02
#define PCA9684_SUBADR2             0x03
#define PCA9684_SUBADR3             0x04
#define PCA9684_LED0_ON_L           0x06
#define PCA9684_LED0_ON_H           0x07
#define PCA9684_LED0_OFF_L          0x08
#define PCA9684_LED0_OFF_H          0x09
#define PCA9684_ALL_LED_ON_L        0xFA
#define PCA9684_ALL_LED_ON_H        0xFB
#define PCA9684_ALL_LED_OFF_L       0xFC
#define PCA9684_ALL_LED_OFF_H       0xFD
#define PCA9684_PRESCALE            0xFE


#define PCA9684_RESTART             0x80
#define PCA9684_SLEEP               0x10
#define PCA9684_ALLCALL             0x01
#define PCA9684_INVRT               0x10
#define PCA9684_OUTDRV              0x04
#define PCA9684_RESET               0x00

void pca9685_init(void);
void pca9685_SetPwmFreq(int freq_hz);
void pca9685_SetPwm(uint8_t channel, uint16_t on, uint16_t off);
void pca9685_SetAllPwm(uint16_t on, uint16_t off);
void pca9685_SetDuty(uint8_t index, uint16_t value, uint8_t invert);
void pca9685_PwmOff(uint8_t index);

#endif // MICROPY_INCLUDED_CODAL_PORT_DRV_PCA9685_H
