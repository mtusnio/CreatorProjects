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
#include "sys/clock.h"
#include "dev/leds.h"


#define JOYSTICK_ADDRESS_READ (0x81)
#define JOYSTICK_ADDRESS_WRITE (0x80)

#define X_REG (0x10)
#define Y_REG (0x11)

PROCESS(led_process, "LED process");
AUTOSTART_PROCESSES(&led_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(led_process, ev, data)
{
  PROCESS_BEGIN();

  {
	printf("Start");
	int i;
	uint8_t reg_data[2] = { JOYSTICK_ADDRESS_READ, X_REG };

//	i2c1_init();
//	i2c1_set_frequency(100000);
//	i2c1_master_enable();

	while(1)
	{
		printf("Loop");
		int8_t x, y;
//		leds_on(LEDS_RED);
		for(i = 0; i < 5; i++)
			clock_delay_usec(65000);

		/*i2c1_send_start();
		i2c1_byte_send(JOYSTICK_ADDRESS_WRITE);
		i2c1_byte_send(X_REG);
		i2c1_send_repeated_start();
		i2c1_byte_send(JOYSTICK_ADDRESS_READ);
		i2c1_byte_receive((uint8_t*)&x);

		i2c1_send_stop();*/

//		if(x > 0)
//			leds_on(LEDS_RED);
//		else
//			leds_off(LEDS_RED);

//		printf("Test\n");
		leds_toggle(LEDS_RED);
//		leds_toggle(LEDS_BLUE);

		if(x > 10)
			leds_on(LEDS_GREEN);
		else
			leds_off(LEDS_GREEN);
	}
//	i2c1_master_disable();


  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
