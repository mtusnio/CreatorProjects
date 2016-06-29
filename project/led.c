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


#define JOYSTICK_ADDRESS_READ (0x81)
#define JOYSTICK_ADDRESS_WRITE (0x80)

#define X_REG (0x10)
#define Y_REG (0x11)

#define M_CTRL_REG (0x2B)
#define J_CTRL_REG (0x2C)
#define T_CTRL_REG (0x2D)


PROCESS(led_process, "LED process");
AUTOSTART_PROCESSES(&led_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(led_process, ev, data)
{
  PROCESS_BEGIN();

  {
	int i;
	uint8_t registers[] = { M_CTRL_REG, J_CTRL_REG, T_CTRL_REG };
	uint8_t register_bytes[] = { 0x00, 0x06, 0x09 };

	printf("=====Start=====\n");

	for(i = 0; i < 35; i++)
		clock_delay_usec(65000);

//	uint8_t reg_data[2] = { JOYSTICK_ADDRESS_READ, X_REG };

	i2c1_init();
	i2c1_set_frequency(100000);
	i2c1_master_enable();

	for(i = 0; i < 3; i++)
	{
		i2c1_send_start();
		i2c1_byte_send(JOYSTICK_ADDRESS_WRITE);
		i2c1_byte_send(registers[i]);
		i2c1_byte_send(register_bytes[i]);
		i2c1_send_stop();
	}

	while(1)
	{
		int8_t x = 2;
//		leds_on(LEDS_RED);
		for(i = 0; i < 5; i++)
			clock_delay_usec(65000);

		if(i2c1_send_start())
		{
			printf("Issue when sending start\n");
			continue;
		}
		printf("Start sent\n");
		if(i2c1_byte_send(JOYSTICK_ADDRESS_WRITE))
		{
			printf("Issue when sending write address\n");
			continue;
		}
		printf("Write sent\n");
		if(i2c1_byte_send(X_REG))
		{
			printf("Issue when sending X registry read\n");
			continue;
		}
		printf("X reg sent\n");

		if(i2c1_send_repeated_start())
		{
			printf("Issue when sending repeated start\n");
			continue;
		}
		printf("Repeated start\n");
		if(i2c1_byte_send(JOYSTICK_ADDRESS_READ))
		{
			printf("Issue when sending read\n");
			continue;
		}

		I2C1CONbits.ACKDT = 1;
		printf("Read sent\n");
		if(i2c1_byte_receive((uint8_t*)&x))
		{
			printf("Issue when receiving a byte\n");
			continue;
		}
		I2C1CONbits.ACKDT = 0;
		printf("Byte received\n");
		if(i2c1_send_stop())
			printf("Failed to send stop\n");
		printf("Stop sent\n");


		printf("X: %i\n", x);
		leds_toggle(LEDS_RED);

		if(x > 10)
			leds_on(LEDS_GREEN);
		else
			leds_off(LEDS_GREEN);

	}

	i2c1_master_disable();

  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
