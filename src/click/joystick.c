#include "click/joystick.h"
#include "click/common.h"


#define JOYSTICK_ADDRESS (0x40)

#define X_ADDRESS (0x10)
#define Y_ADDRESS (0x11)

int joystick_click_enable()
{


}



int joystick_click_get_position(int8_t * x, int8_t * y)
{
    if(i2c_read_register(JOYSTICK_ADDRESS, X_ADDRESS, (uint8_t*)x) == -1)
        return -1;
 
    if(i2c_read_register(JOYSTICK_ADDRESS, Y_ADDRESS, (uint8_t*)y) == -1)
        return -1;

    return 0;   
}



int joystick_click_disable()
{


}
