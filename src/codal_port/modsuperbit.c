#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"
#include "py/smallint.h"
#include "drv_PCA9685.h"

STATIC mp_obj_t mod_superbit_init(void) {
    pca9685_init();
    pca9685_SetPwmFreq(50);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(mod_superbit_init_obj, mod_superbit_init);

STATIC mp_obj_t mod_superbit_test(void) {
    pca9685_SetPwm(8, 0, 2000);
    pca9685_SetPwm(9, 0, 0);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(mod_superbit_test_obj, mod_superbit_test);


// ****************************************
STATIC const mp_map_elem_t superbit_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_superbit) },
    { MP_OBJ_NEW_QSTR(MP_QSTR___init__), (mp_obj_t)&mod_superbit_init_obj },

    //{ MP_OBJ_NEW_QSTR(MP_QSTR_reset), (mp_obj_t)&mod_superbit_reset },
    { MP_OBJ_NEW_QSTR(MP_QSTR_test), (mp_obj_t)&mod_superbit_test_obj },
};

STATIC MP_DEFINE_CONST_DICT(superbit_module_globals, superbit_module_globals_table);

const mp_obj_module_t superbit_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&superbit_module_globals,
};
