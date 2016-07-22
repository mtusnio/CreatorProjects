#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"

#define FRM_WIDTH (176)
#define FRM_HEIGHT (144)
#define SIZE (FRM_WIDTH * FRM_HEIGHT * 2)

int camera_click_get_image(uint8_t * data, const uint32_t size)
{
    uint8_t * tx_buffer = (uint8_t*)malloc(size * sizeof(uint8_t));

    memset(tx_buffer, 0, size);
    tx_buffer[0] = 0x05;

    if(spi_transfer(tx_buffer, data, 100) < 0)
        return -1;
   return 0;
}


int main()
{
    FILE * f;
    uint8_t * data = (uint8_t*)malloc(sizeof(uint8_t) * (10 + SIZE + 1));
    data[0] = 0x00;
    data[1] = 0x10;
    data[2] = FRM_HEIGHT;
    data[3] = FRM_HEIGHT>>8;
    data[4] = FRM_WIDTH;
    data[5] = FRM_WIDTH>>8;
    data[6] = 0xFF;
    data[7] = 0xFF;
    data[8] = 0xFF;
    data[9] = 0xFF;

    spi_init();
    spi_select_bus(MIKROBUS_1);
    spi_set_mode(MIKROBUS_1, 1);
    spi_set_speed(MIKROBUS_1, 20 * 1000 * 1000);
    while(1)
    {
        if(camera_click_get_image(data + 10, SIZE + 1) < 0)
        {
            printf("Failed to get data\n");
            break;
        }

        f = fopen("image.bmp", "wb");
        fwrite(data, 1, 10 + SIZE, f);
        fclose(f);
        sleep(2);
    }

    spi_release();
    return 0;
}



