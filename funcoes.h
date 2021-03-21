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
#include <poll.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define ENTRADA_MAX 4096
#define MARCA_INICIO 01111110
#define PATH_MAX 4096
#define CD 0000
#define LS 0001
#define VER 10
#define LINHA 0011
#define EDIT 0101
#define ACK 1000
#define NACK 1001
#define CONTEUDO_LS 1011
#define ERR 1111


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

estrutura_pacote *protocolo(char *dado,int tipo, int tam, int seq);
estrutura_pacote* abre_protocolo(char *entrada_server);
char* recebe_protocolo(int socket) ;
void lista_comandos_ajuda();
int ConexaoRawSocket();
void client_LLS();
void imprime_path();
void le_comando( linha_comando *entrada, int socket);
int cal_pariedade(int tam, int seq, int tipo, char *dado, char *endereco_origem, char*endereco_destino);
long decimalToBinary(int decimalnum);
void mostra_protocolo(estrutura_pacote *pacote);
void client_CD(linha_comando *entrada, int socket) ;
char *protocolo_string(estrutura_pacote * p1) ;
void client_LS(linha_comando *entrada, int socket) ;
char * int2bin(unsigned i, int tam);
int convert(long long n);
int envia_protocolo(char *string, int socket) ;
void funcoes_server(int socket_confirmado);
void server_CD(estrutura_pacote *p, int socket);
int server_LS(estrutura_pacote *p, int socket) ;
int server_LINHA(estrutura_pacote *p, int socket);
int server_LINHAS(estrutura_pacote *p, int socket);
int server_EDIT(estrutura_pacote *p, int socket);
int client_LINHAS(linha_comando *entrada, int socket);
int client_EDIT(linha_comando *entrada, int socket);
estrutura_pacote *protocolo_server(char *dado,int tipo, int tam, int seq);
void server_VER(estrutura_pacote *p, int socket);
void client_VER(linha_comando *entrada, int socket);
bool recebe_ls(estrutura_pacote * p1, int socket);
int LINHAS(FILE *nome, int linha, int socket);
int split_string(char *string,int cont, int tam, int socket);
//char *protocolo_string_server(estrutura_pacote * p1);//
int strlcpy(char *dst, const char *src, size_t siz);
int acha_binario(int tam) ;
char * convert_binary(int dec, char *output);
int client_LINHA(linha_comando *entrada, int socket) ;
int stringToDecimal(char * str);
int verifica(estrutura_pacote *p);
