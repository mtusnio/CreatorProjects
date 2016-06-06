#ifndef __LETMECREATE_CLICK_JOYSTICK_H__
#define __LETMECREATE_CLICK_JOYSTICK_H__

#include <stdint.h>

int joystick_click_enable(void);

int joystick_click_get_position(int8_t * x, int8_t * y);

int joystick_click_disable(void);


#endif
