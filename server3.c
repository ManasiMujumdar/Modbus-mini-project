#include <stdio.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>       /* for setitimer */
#include <signal.h>     /* for signal */
#include <modbus.h>
#include <unistd.h>
#include "unittest.h"

#if defined(_WIN32)
#include <ws2tcpip.h>
#else
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#define NB_CONNECTION    5
#define ADDRESS_START_DI    10000
#define ADDRESS_END_DI     10010
#define ADDRESS_START_DO    30000
#define ADDRESS_END_DO     30010
#define ADDRESS_START_AI    20000
#define ADDRESS_END_AI     20010
#define ADDRESS_START_AO    40000
#define ADDRESS_END_AO     40010

//discreteinput(read-only)
//discreteoutput(coil, read and write)
//analog input (input register, read only)
//analog output(holding register, read-write)

int server_socket = -1;
 modbus_t *ctx = NULL;
  modbus_mapping_t *mb_mapping;
static void close_sigint(int dummy)
{
    if (server_socket != -1) {
        close(server_socket);
    }
    modbus_free(ctx);
    modbus_mapping_free(mb_mapping);

    exit(dummy);
}




int main(void)
{

 uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];



    int master_socket;
    int rc;
    int nb1,nb2,nb3,nb4;
   int header_length;
   
    fd_set refset;
    fd_set rdset;
    /* Maximum file descriptor number */
    int fdmax;
    
   
    
     ctx = modbus_new_tcp("127.0.0.1", 1502);
    
    
    header_length = modbus_get_header_length(ctx);

    modbus_set_debug(ctx, TRUE);
    
    mb_mapping = modbus_mapping_new_start_address(
        UT_BITS_ADDRESS, UT_BITS_NB,
        UT_INPUT_BITS_ADDRESS, UT_INPUT_BITS_NB,
        UT_REGISTERS_ADDRESS, UT_REGISTERS_NB_MAX,
        UT_INPUT_REGISTERS_ADDRESS, UT_INPUT_REGISTERS_NB);
        
        if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
        
      modbus_set_bits_from_bytes(mb_mapping->tab_input_bits, 0, UT_INPUT_BITS_NB,
                               UT_INPUT_BITS_TAB);
      modbus_set_bits_from_bytes(mb_mapping->tab_bits, 0, UT_BITS_NB,
                               UT_BITS_TAB);  
                               int i;
        for (i=0; i < UT_INPUT_REGISTERS_NB; i++) {
        mb_mapping->tab_input_registers[i] = UT_INPUT_REGISTERS_TAB[i];
    }
    for (i=0; i < UT_REGISTERS_NB; i++) {
        mb_mapping->tab_registers[i] = UT_REGISTERS_TAB[i];
    }
   
    server_socket = modbus_tcp_listen(ctx, NB_CONNECTION);
    if (server_socket == -1) {
        fprintf(stderr, "Unable to listen TCP connection\n");
        modbus_free(ctx);
        return -1;
    }
    else{
    printf("server now listening for any new connections ! \n");
    }
    
    
    
    signal(SIGINT, close_sigint);

    /* Clear the reference set of socket */
    FD_ZERO(&refset);
    /* Add the server socket */
    FD_SET(server_socket, &refset);

    /* Keep track of the max file descriptor */
    fdmax = server_socket;

    for (;;) {
        rdset = refset;
        if (select(fdmax+1, &rdset, NULL, NULL, NULL) == -1) {
            perror("Server select() failure.");
            close_sigint(1);
        }

        /* Run through the existing connections looking for data to be
         * read */
        for (master_socket = 0; master_socket <= fdmax; master_socket++) {

            if (!FD_ISSET(master_socket, &rdset)) {
                continue;
            }

            if (master_socket == server_socket) {
                /* A client is asking a new connection */
                socklen_t addrlen;
                struct sockaddr_in clientaddr;
                int newfd;

                /* Handle new connections */
                addrlen = sizeof(clientaddr);
                memset(&clientaddr, 0, sizeof(clientaddr));
                newfd = accept(server_socket, (struct sockaddr *)&clientaddr, &addrlen);
                if (newfd == -1) {
                    perror("Server accept() error");
                } else {
                    FD_SET(newfd, &refset);

                    if (newfd > fdmax) {
                        /* Keep track of the maximum */
                        fdmax = newfd;
                    }
                    printf("New connection from %s:%d on socket %d\n",
                           inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
                }
            } else {
                modbus_set_socket(ctx, master_socket);
                rc = modbus_receive(ctx, query);
                if (rc > 0) {
                    modbus_reply(ctx, query, rc, mb_mapping);
                } else if (rc == -1) {
                    /* This example server in ended on connection closing or
                     * any errors. */
                    printf("Connection closed on socket %d\n", master_socket);
                    close(master_socket);

                    /* Remove from reference set */
                    FD_CLR(master_socket, &refset);

                    if (master_socket == fdmax) {
                        fdmax--;
                    }
                }
            }
        }
    }

    return 0;
}
