#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "letmecreate/core/common.h"
#include "letmecreate/core/network.h"
#include "letmecreate/click/motion.h"

#define SERVER_IP_ADDR "fe80::e01a:69ff:fe6c:4a97"

#define SERVER_PORT 3000
#define CLIENT_PORT 3001


static struct uip_udp_conn * conn;
static uint8_t network_data[2];

void callback(uint8_t value)
{
    network_data[0] = CLICKER_ID;
    network_data[1] = value;
    udp_packet_send(conn, network_data, 2);
}

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        // Due to the way Contiki protothreads work this needs to be static,
        // otherwise the data will be lost when switching to a different thread
        printf("===START===\n");

        conn = udp_new_connection(CLIENT_PORT, SERVER_PORT, SERVER_IP_ADDR);

        motion_click_enable(MIKROBUS_1);
        motion_click_attach_callback(MIKROBUS_1, callback);


        while(1) { PROCESS_WAIT_UDP_SENT(); }

        motion_click_disable(MIKROBUS_1);

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
