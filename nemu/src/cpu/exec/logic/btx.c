#include "cpu/exec/helper.h"

#define DATA_BYTE 4
#include "btx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "btx-template.h"

make_helper_v(bt_r2rm)
make_helper_v(bt_i2rm)
