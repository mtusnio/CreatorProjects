#include <stdio.h>
#include <unistd.h>

#include "core/i2c.h"
#include "click/common.h"

#define FAN_ADDRESS (0x2F)

#define STATUS_REGISTER (0x24)
#define FREQUENCY_REGISTER (0x2D)
#define SETTING_REGISTER (0x30)
#define CONFIGURATION_REGISTER (0x32)


int main()
{
    i2c_init(MIKROBUS_I2C_BUS_1);
    i2c_select_bus(MIKROBUS_I2C_BUS_1);
    
    uint8_t currentspeed = 0;
    while(1)
    {
//        sleep(5);
 
//        if(i2c_write_register(FAN_ADDRESS, SETTING_REGISTER, 0x01) == -1)        
//            return -1;

        uint8_t status, frequency, speed;


	printf("1\n");
        if(i2c_read_register(FAN_ADDRESS, STATUS_REGISTER, &status) == -1)
           return -1;
        
	printf("2\n");
	if(i2c_read_register(FAN_ADDRESS, FREQUENCY_REGISTER, (uint8_t*)&frequency) == -1)
            return -1;


	printf("3\n");        
        if(i2c_read_register(FAN_ADDRESS, CONFIGURATION_REGISTER, (uint8_t*)&speed) == -1)
            return -1;

        printf("%i %i %i\n", status, frequency, speed);

        currentspeed += 30;
    }
    i2c_release(MIKROBUS_I2C_BUS_1);

    return 0;
}
