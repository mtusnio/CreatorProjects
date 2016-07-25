#include <contiki.h>
#include <contiki-net.h>

#include <stdio.h>
#include <string.h>
#include <sys/clock.h>
#include <dev/leds.h>

#include <lmc/core/spi.h>
#include <lmc/core/network.h>


#define FRM_WIDTH (176)
#define FRM_HEIGHT (144)
#define SIZE (FRM_WIDTH * FRM_HEIGHT * 2)
#define OFFSET 10
#define FULL_SIZE (SIZE + OFFSET)

static uint8_t image_data[FULL_SIZE];

void get_data()
{
    memset(image_data, 0, FULL_SIZE);
    image_data[0] = 0x00;
    image_data[1] = 0x10;
    image_data[2] = FRM_HEIGHT;
    image_data[3] = FRM_HEIGHT>>8;
    image_data[4] = FRM_WIDTH;
    image_data[5] = FRM_WIDTH>>8;
    image_data[6] = 0xFF;
    image_data[7] = 0xFF;
    image_data[8] = 0xFF;
    image_data[9] = 0xFF;
}

#define SERVER_ADDRESS "fe80::e01a:69ff:fe6c:4a97"

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        static struct uip_udp_conn * conn;
        static struct etimer et;
        static int i;

        printf("INIT SPI\n");
        spi_init(1);
        spi_set_speed(20 * 1000 * 1000);

        conn = udp_new_connection(3000, 3001, SERVER_ADDRESS);

        if(!conn)
        {
            printf("No connection created\n");
            return 1;
        }
        uint8_t command = 0x05;

        get_data();
        printf("Start loop\n");
        while(1)
        {
            leds_toggle(LED1);
            for(i = 0; i < 25; i++)
                clock_delay_usec(65000);

            printf("Init transfer\n");
            spi_start_transfer();

            spi_write(&command, 1);
            spi_read(&image_data[OFFSET], SIZE);

            spi_end_transfer();

            printf("Transfer finished\n");

            // We need this timeout timer for the event wait. We need to
            // wait between each packet for UDP to work.
            etimer_set(&et, CLOCK_SECOND);

            printf("Sending data\n");
            uip_udp_packet_send(conn, image_data, SIZE + OFFSET);

            printf("Data sent\n");
            // Halt the process until the timer event finishes
            PROCESS_WAIT_EVENT();
        }

        spi_release();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
