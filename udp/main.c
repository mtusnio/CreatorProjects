#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>
#include "net/ip/simple-udp.h"


#define SERVER_IP_ADDR "2001:1418:100::1"
#define SERVER_PORT 5000

void receive_callback(struct simple_udp_connection *c,
                       const uip_ipaddr_t *source_addr,
                       uint16_t source_port,
                       const uip_ipaddr_t *dest_addr,
                       uint16_t dest_port, 
                       const uint8_t *data, uint16_t datalen)
{
}

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        int i;
        uip_ipaddr_t addr;
        uiplib_ip6addrconv(SERVER_IP_ADDR, &addr);

        printf("Awaiting...\n");
        for(i = 0; i < 20; i++)
            clock_delay_usec(65000);

        printf("===START===\n");
        struct simple_udp_connection connection;
        if(!simple_udp_register(&connection, SERVER_PORT,
                                &addr, SERVER_PORT, receive_callback))
        {
            printf("Could not register UDP!\n");
            return 1;
        }

        while(1)
        {
            char bytes[] = "Hello";
            for(i = 0; i < 10; i++)
                clock_delay_usec(65000);

            printf("Sending data\n");
            simple_udp_send(&connection, bytes, sizeof(bytes));
        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
