#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"
#include "py/smallint.h"
#include "drv_PCA9685.h"

// M1 : 8, 9
// M2 : 10, 11
// M3 : 12, 13
// M4 : 14, 15
uint8_t MotorsMapping[4] = {8, 10, 12, 14};

// Servo -> PCA9685
// S1: 0
// S2: 1
// S3: 2
// S4: 3
// S5: 4
// S6: 5
// S7: 6
// S8: 7
uint8_t ServoMapping[8] = {0, 1, 2, 3, 4, 5, 6, 7};

STATIC mp_obj_t mod_superbit_init(void) {
    pca9685_init();
    pca9685_SetPwmFreq(50);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(mod_superbit_init_obj, mod_superbit_init);

STATIC mp_obj_t mod_superbit_motor_control(mp_obj_t motor_in, mp_obj_t speed_in) {
    int motor = mp_obj_get_int(motor_in);
    int speed = mp_obj_get_int(speed_in);

    int a = MotorsMapping[motor - 1];
    int b = a + 1;

    //speed = speed * 16  # map 255 to 4096

    if (speed > 4096)
        speed = 4095;
    if (speed < -4096)
        speed = -4095;

    if (speed > 0)
    {
        pca9685_SetPwm(a, 0, speed);
        pca9685_SetPwm(b, 0, 0);
    }
    else 
    {
        pca9685_SetPwm(a, 0, 0);
        pca9685_SetPwm(b, 0, -speed);
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(mod_superbit_motor_control_obj, mod_superbit_motor_control);

STATIC mp_obj_t mod_superbit_motor_stop(mp_obj_t motor_in) {
    int motor = mp_obj_get_int(motor_in);

    int a = MotorsMapping[motor - 1];
    int b = a + 1;

    pca9685_SetPwm(a, 0, 0);
    pca9685_SetPwm(b, 0, 0);

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(mod_superbit_motor_top_obj, mod_superbit_motor_stop);

STATIC mp_obj_t mod_superbit_Servo180(mp_obj_t motor_in, mp_obj_t degree_in) {
    int motor = mp_obj_get_int(motor_in);
    int degree = mp_obj_get_int(degree_in);

    if (degree < 0)
        degree = 0;
    else if (degree > 180)
        degree = 180;

    float us = (degree*1800/180+600);
    float pwm = (us*4096/20000);

    pca9685_SetDuty(ServoMapping[motor - 1], (int) pwm, 0);

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_2(mod_superbit_Servo180_obj, mod_superbit_Servo180);

STATIC mp_obj_t mod_superbit_Servo270(mp_obj_t motor_in, mp_obj_t degree_in) {
    int motor = mp_obj_get_int(motor_in);
    int degree = mp_obj_get_int(degree_in);

    if (degree < 0)
        degree = 0;
    else if (degree > 270)
        degree = 270;
    float newdegree = degree/1.5;
    float us = (newdegree*1800/180+600);
    float pwm = (us*4096/20000);

    pca9685_SetDuty(ServoMapping[motor - 1], (int) pwm, 0);

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_2(mod_superbit_Servo270_obj, mod_superbit_Servo270);


STATIC mp_obj_t mod_superbit_Servo360(mp_obj_t motor_in, mp_obj_t pos_in, mp_obj_t degree_in) {
    int motor = mp_obj_get_int(motor_in);
    int pos = mp_obj_get_int(pos_in);
    int value = mp_obj_get_int(degree_in);
    float us, pwm;

    if (value < 0)
        value = 0;
    else if (value > 90)
        value = 90;

    if (pos == 1)
    {
        us = (float) (90*1800/180+600);
        pwm = (float) (us*4096/20000);
        pca9685_SetDuty(ServoMapping[motor - 1], (int) pwm, 0);
    }
    else if (pos == 2)
    {
        us = (float)((90-value)*1800/180+600);
        pwm = (float)(us*4096/20000);
        pca9685_SetDuty(ServoMapping[motor - 1], (int) pwm, 0);
    }
    else if (pos == 3)
    {
        us = (float)((90+value)*1800/180+600);
        pwm = (float)(us*4096/20000);
        pca9685_SetDuty(ServoMapping[motor - 1], (int) pwm, 0);
    }

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_3(mod_superbit_Servo360_obj, mod_superbit_Servo360);


// ****************************************
STATIC const mp_map_elem_t superbit_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_superbit) },
    { MP_OBJ_NEW_QSTR(MP_QSTR___init__), (mp_obj_t)&mod_superbit_init_obj },

    { MP_OBJ_NEW_QSTR(MP_QSTR_motor_control), (mp_obj_t)&mod_superbit_motor_control_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_motor_stop), (mp_obj_t)&mod_superbit_motor_top_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Servo180), (mp_obj_t)&mod_superbit_Servo180_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Servo270), (mp_obj_t)&mod_superbit_Servo270_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Servo360), (mp_obj_t)&mod_superbit_Servo360_obj },

    { MP_OBJ_NEW_QSTR(MP_QSTR_M1), MP_OBJ_NEW_SMALL_INT(1) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_M2), MP_OBJ_NEW_SMALL_INT(2) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_M3), MP_OBJ_NEW_SMALL_INT(3) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_M4), MP_OBJ_NEW_SMALL_INT(4) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_S1), MP_OBJ_NEW_SMALL_INT(1) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_S2), MP_OBJ_NEW_SMALL_INT(2) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_S3), MP_OBJ_NEW_SMALL_INT(3) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_S4), MP_OBJ_NEW_SMALL_INT(4) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_S5), MP_OBJ_NEW_SMALL_INT(5) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_S6), MP_OBJ_NEW_SMALL_INT(6) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_S7), MP_OBJ_NEW_SMALL_INT(7) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_S8), MP_OBJ_NEW_SMALL_INT(8) },

};

STATIC MP_DEFINE_CONST_DICT(superbit_module_globals, superbit_module_globals_table);

const mp_obj_module_t superbit_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&superbit_module_globals,
};
