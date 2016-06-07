#include <stdio.h>
#include <core/common.h>
#include <core/led.h>
#include <core/i2c.h>
#include <click/joystick.h>

#define OFFSET 98
#define MAXIMUM (OFFSET * 2)

int resolve_percentage(float perc)
{
    int mask = 0;
    int div = perc/0.14f;

    for(int i = 0; i < div; i++)
        mask |= (1 << i);

    return mask;
}

int main()
{
    i2c_init(MIKROBUS_2);
    i2c_select_bus(MIKROBUS_2);
    led_init();
    while(1)
    {
        int8_t x, y;
        int mask;

        joystick_click_get_position(&x, &y);
        printf("%i %i\n", (int)x, (int)y);

        mask = resolve_percentage((float)(x + OFFSET)/(float)MAXIMUM);
        led_switch_on(mask);
        led_switch_off(~mask);
    }

    led_release();
    i2c_release(MIKROBUS_2);

    return 0;
}
