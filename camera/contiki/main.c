#include <contiki.h>
#include <contiki-net.h>

#include <stdio.h>
#include <string.h>
#include <sys/clock.h>
#include <dev/leds.h>

#include <lmc/core/spi.h>
#include <lmc/core/network.h>

#define CEILING(x,y) (((x) + (y) - 1) / (y))

#define FRM_WIDTH (176)
#define FRM_HEIGHT (144)
#define SIZE (FRM_WIDTH * FRM_HEIGHT * 2)
#define OFFSET 10
#define FULL_SIZE (SIZE + OFFSET)
#define PACKET_SIZE 64

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
        static uint8_t command = 0x05;

        printf("INIT SPI\n");
        spi_init(1);
        spi_set_speed(1 * 1000 * 1000);

        conn = udp_new_connection(3000, 3001, SERVER_ADDRESS);

        if(!conn)
        {
            printf("No connection created\n");
            return 1;
        }

        get_data();
        printf("Start loop\n");
        while(1)
        {
            etimer_set(&et, CLOCK_SECOND * 2);

            PROCESS_WAIT_EVENT();
            leds_toggle(LED1);
            //for(i = 0; i < 25; i++)
            //    clock_delay_usec(65000);

            printf("Init transfer\n");
            spi_start_transfer();

            spi_write(&command, 1);
            spi_read(&image_data[OFFSET], SIZE);

            spi_end_transfer();

            printf("Transfer finished\n");

            for(i = 0; i < CEILING(FULL_SIZE, PACKET_SIZE); i++)
            {
                int send_size = PACKET_SIZE;
                if((FULL_SIZE - i * PACKET_SIZE) < PACKET_SIZE)
                    send_size = FULL_SIZE - i * PACKET_SIZE;


                printf("Sending data\n");
                uip_udp_packet_send(conn, &image_data[i * PACKET_SIZE], send_size);

                tcpip_poll_udp(conn);
                PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

                printf("Data sent\n");
            }
        }

        spi_release();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
