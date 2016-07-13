/**
 * @file
 * Coffee File-system Test
 *
 * @author Imagination Technologies
 *
 * @copyright Copyright (c) 2015, Imagination Technologies Limited
 *
 * All rights reserved.
 *
 * Redistribution and use of the Software in source and binary forms, with or
 * without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. The Software (including after any modifications that you make to it) must
 *    support the FlowCloud Web Service API provided by Licensor and accessible
 *    at http://ws-uat.flowworld.com and/or some other location(s) that we
 *    specify.
 *
 * 2. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 3. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 4. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this Software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <contiki.h>
#include <stdlib.h>
#include <stdio.h>
#include <pic32_i2c.h>
#include <p32xxxx.h>
#include "sys/clock.h"
#include "dev/leds.h"


#define JOYSTICK_ADDRESS (0x40)

#define X_REG (0x10)
#define Y_REG (0x11)

#define M_CTRL_REG (0x2B)
#define J_CTRL_REG (0x2C)
#define T_CTRL_REG (0x2D)


#define I2C_DEFAULT_FREQUENCY 100000

uint8_t i2c_init()
{
    if(i2c1_init())
        return 1;

    if(i2c1_set_frequency(I2C_DEFAULT_FREQUENCY))
        return 1;

    if(i2c1_master_enable())
        return 1;


    return 0;
}

uint8_t i2c_release()
{
    return i2c1_master_disable();
}

uint8_t i2c_set_frequency(uint32_t frequency)
{
    return i2c1_set_frequency(frequency);
}

uint8_t i2c_write(uint8_t address, uint8_t reg_address, uint8_t * bytes, uint8_t length)
{
    if(!bytes)
    {
        printf("I2C: Cannot write null bytes\n");
        return 1;
    }

    if(!length)
    {
        printf("I2C: Length cannot be 0\n");
        return 1;
    }

    address = (address << 1);

    if(i2c1_send_start())
        return 1;

    if(i2c1_send_byte(address))
        return 1;

    if(i2c1_send_byte(reg_address))
        return 1;

    if(i2c1_send_bytes(bytes, length))
        return 1;

    if(i2c1_send_stop())
        return 1;

    return 0;
}

uint8_t i2c_read(uint8_t address, uint8_t reg_address, uint8_t * bytes, uint8_t length)
{
    if(!bytes)
    {
        printf("I2C: Cannot read to null bytes\n");
        return 1;
    }

    if(!length)
    {
        printf("I2C: Length cannot be 0\n");
        return 1;
    }


    address = address << 1;

    if(i2c1_send_start())
        return 1;

    if(i2c1_send_byte(address))
        return 1;

    if(i2c1_send_byte(reg_address))
        return 1;

    if(i2c1_send_repeated_start())
        return 1;

    address |= 0x01;

    if(i2c1_send_byte(address))
        return 1;

    if(i2c1_set_nack(1))
        return 1;

    if(i2c1_receive_bytes(bytes, length))
        return 1;

    if(i2c1_send_stop())
        return 1;

    return 0;
}

PROCESS(led_process, "LED process");
AUTOSTART_PROCESSES(&led_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(led_process, ev, data)
{
    PROCESS_BEGIN();

    {
        int i;

        printf("=====Start=====\n");


        i2c_init();

        while(1)
        {
            int8_t x = 2;
            int8_t y = 2;
//          leds_on(LEDS_RED);
            for(i = 0; i < 8; i++)
                    clock_delay_usec(65000);

            printf("Read X\n");
            i2c_read(JOYSTICK_ADDRESS, X_REG, (uint8_t*)&x, 1);
            printf("Read Y\n");
            i2c_read(JOYSTICK_ADDRESS, Y_REG, (uint8_t*)&y, 1);

            printf("X: %i, Y: %i\n", x, y);

            leds_toggle(LEDS_RED);

            if(x > 10)
                leds_on(LEDS_GREEN);
            else
                leds_off(LEDS_GREEN);

        }

        i2c_release();

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
