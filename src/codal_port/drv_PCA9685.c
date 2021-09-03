#include "drv_PCA9685.h"
#include <stdio.h>
#include "py/mperrno.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "modmicrobit.h"
#include "microbithal.h"
#include <math.h>

void pca9685_WriteCmd(uint8_t addr, uint8_t data)
{
    uint8_t pCmd[2];
    pCmd[0] = addr;
    pCmd[1] = data;
    microbit_hal_i2c_writeto(PCA9684_ADDRESS, pCmd, 2, true);
}

uint8_t pca9685_ReadData(uint8_t addr)
{
    uint8_t pCmd[2];
    pCmd[0] = addr;
    microbit_hal_i2c_writeto(PCA9684_ADDRESS, pCmd, 1, true); 
    microbit_hal_i2c_readfrom(PCA9684_ADDRESS, pCmd, 1, true);    
    return pCmd[0]; 
}

void pca9685_init(void)
{

    const microbit_pin_obj_t *sda = &microbit_p20_obj;
    const microbit_pin_obj_t *scl = &microbit_p19_obj;

    int ret =microbit_hal_i2c_init(scl->name, sda->name, 1000);

    if (ret != 0) {
        mp_raise_OSError(ret);
    }

    pca9685_WriteCmd(PCA9684_MODE1, PCA9684_RESET);

    pca9685_SetAllPwm(0, 0);

    pca9685_WriteCmd(PCA9684_MODE2, PCA9684_OUTDRV);
    pca9685_WriteCmd(PCA9684_MODE1, PCA9684_ALLCALL);
    mp_hal_delay_ms(5);

    uint8_t mode = pca9685_ReadData(PCA9684_MODE1);
    mode = mode & ~PCA9684_SLEEP;
    pca9685_WriteCmd(PCA9684_MODE1, mode);
    mp_hal_delay_ms(5);

}

void pca9685_SetPwmFreq(int freq_hz)
{
    uint8_t oldMode;

    float prescaleval = 25000000.0 / 4096.0;
    prescaleval /= (freq_hz);
    prescaleval -= 1.0;
    int prescale = (int) floor(prescaleval + 0.5);
    printf("prescale=%d\r\n", prescale);

    oldMode = pca9685_ReadData(PCA9684_MODE1);
    pca9685_WriteCmd(PCA9684_MODE1, (oldMode & 0x7F) | 0x10);
    pca9685_WriteCmd(PCA9684_PRESCALE, prescale);
    pca9685_WriteCmd(PCA9684_MODE1, oldMode);
    mp_hal_delay_ms(5);

    pca9685_WriteCmd(PCA9684_MODE1, oldMode | 0x80);
}

void pca9685_SetPwm(int channel, int on, int off)
{
    printf("on=%d, off = %d\r\n", on, off);
    pca9685_WriteCmd(PCA9684_LED0_ON_L + 4*channel, on & 0xFF);
    pca9685_WriteCmd(PCA9684_LED0_ON_H + 4*channel, on >> 8);
    pca9685_WriteCmd(PCA9684_LED0_OFF_L + 4*channel, off & 0xFF);
    pca9685_WriteCmd(PCA9684_LED0_OFF_H + 4*channel, off >> 8);

    on = pca9685_ReadData(PCA9684_LED0_ON_L + 4*channel);
    on |= pca9685_ReadData(PCA9684_LED0_ON_H + 4*channel) << 8;
    off = pca9685_ReadData(PCA9684_LED0_OFF_L + 4*channel);
    off |= pca9685_ReadData(PCA9684_LED0_OFF_H + 4*channel) << 8;
    printf("on=%d, off = %d\r\n", on, off);
}

void pca9685_SetAllPwm(int on, int off)
{
    pca9685_WriteCmd(PCA9684_ALL_LED_ON_L, on & 0xFF);
    pca9685_WriteCmd(PCA9684_ALL_LED_ON_H, on >> 8);
    pca9685_WriteCmd(PCA9684_ALL_LED_OFF_L, off & 0xFF);
    pca9685_WriteCmd(PCA9684_ALL_LED_OFF_H, off >> 8);
}

/*
    def duty(self, index, value=None, invert=False):
        if value is None:
            pwm = self.set_pwm(index)
            if pwm == (0, 4096):
                value = 0
            elif pwm == (4096, 0):
                value = 4095
            value = pwm[1]
            if invert:
                value = 4095 - value
            return value
        if not 0 <= value <= 4095:
            raise ValueError("Out of range")
        if invert:
            value = 4095 - value
        if value == 0:
            self.set_pwm(index, 0, 4096)
        elif value == 4095:
            self.set_pwm(index, 4096, 0)
        else:
            self.set_pwm(index, 0, value)

    def PWMOFF(self, index):
        self.set_pwm(index, 0, 0)
*/