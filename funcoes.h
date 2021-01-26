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
#define CD 0000
#define LS 0001
#define VER 0010
#define LINHA 0011
#define LINHAS 0100
#define EDIT 0101
#define ACK 1000
#define NACK 1001

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
  char *marcador;
  int tamanho;
  char *endereco_origem;
  char *endereco_destino;
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
void le_comando( linha_comando *entrada, int socket);
estrutura_pacote *protocolo_cliente(char *dado,int tipo, int tam, int seq);
int cal_pariedade(int tam, int seq, int tipo, char *dado);
long decimalToBinary(int decimalnum);
void mostra_protocolo(estrutura_pacote *pacote);
void funcaoCD(linha_comando *entrada, int socket) ;
char *protocolo_string(estrutura_pacote * p1, int tipo, int tam, int seq) ;
