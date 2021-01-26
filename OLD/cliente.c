#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Endereço do Servidor */
#define ENDER_SERVER "127.0.0.1"
 
int main(int argc, char const *argv[])
{
    /* Socket Descriptor */
    int socket_cliente;

    /* Criando Socket: Cliente */
    if ((socket_cliente = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1){
        printf("Erro no Socket\n");
        exit(-1);
    }
    printf("%d", socket_cliente);

    
    return 0;
}
