#include "funcoes.h"

int socket_s(char *device)
{
  int soquete = 0;
  struct ifreq ir;
  struct sockaddr_ll endereco;
  struct packet_mreq mr;
    char *entrada_buffer, *dados, *maybe;

  soquete = socket(AF_PACKET, SOCK_RAW, 0);  	/*cria socket*/
  if (soquete == -1) {
    perror("Erro no Socket");
    strerror (errno);
    exit(-1);
  }

  memset(&ir, 0, sizeof(struct ifreq));  	/*dispositivo eth0*/
  memcpy(ir.ifr_name, "lo", sizeof("lo"));
  printf("%s\n",ir.ifr_name );
  if (ioctl(soquete, SIOCGIFINDEX, &ir) == -1) {
    strerror (errno);
    perror("Erro no ioctl");
    exit(-1);
  }


  memset(&endereco, 0, sizeof(endereco)); 	/*IP do dispositivo*/
  endereco.sll_family = AF_PACKET;
  endereco.sll_protocol = htons(ETH_P_ALL);
  endereco.sll_ifindex = ir.ifr_ifindex;
  if (bind(soquete, (struct sockaddr *)&endereco, sizeof(endereco)) == -1) {
    printf("Erro no bind\n");
    exit(-1);
  }


  memset(&mr, 0, sizeof(mr));          /*Modo Promiscuo*/
  mr.mr_ifindex = ir.ifr_ifindex;
  mr.mr_type = PACKET_MR_PROMISC;
  if (setsockopt(soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1)	{
    printf("Erro ao fazer setsockopt\n");
    exit(-1);
  }

  return soquete;
}
