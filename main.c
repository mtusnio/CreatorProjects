#include <stdio.h>
#include "core/i2c.h"
#include "click/common.h"
#include "click/joystick.h"


int main()
{
    i2c_init(MIKROBUS_I2C_BUS_2);
    i2c_select_bus(MIKROBUS_I2C_BUS_2);

    while(1)
    {
        int8_t x, y;

        joystick_click_get_position(&x, &y);
        printf("%i %i\n", (int)x, (int)y);
    }
    i2c_release(MIKROBUS_I2C_BUS_2);

    return 0;
}
