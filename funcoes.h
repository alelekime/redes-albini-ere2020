#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define ENTRADA_MAX 4096
#define MARCA_INICIO 01111110
#define PATH_MAX 4096
#define SOCK_PATH    "/home/username/echo_socket"

typedef struct linha_comando {
  char *comando;
  char *diretorio;
  char *nome_arq;
  char *dados;
  int linha;
  int linha_inicial;
  int linha_final;
}linha_comando;

typedef struct estrutura_pacote {
  int marcador;
  int tamanho;
  int endereco_origem;
  int endereco_destino;
  int sequencia;
  int tipo;
  char *dados;
  int pariedade;
}estrutura_pacote;

void lista_comandos_ajuda();
int ConexaoRawSocket();
void funcaoLCD(linha_comando *entrada);
void funcaoLLS();
void imprime_path();
void le_comando( linha_comando *entrada);
