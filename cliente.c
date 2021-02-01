#include "funcoes.h"

void client_CD(linha_comando *entrada, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1 = protocolo_cliente(entrada -> diretorio, CD, strlen(entrada->diretorio), 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
}

void client_LS(linha_comando *entrada, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1 = protocolo_cliente("ls", LS, strlen("ls"), 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
}

void client_LCD(linha_comando *entrada) {
  char *nome = malloc (strlen(entrada->diretorio));
  strncpy(nome, entrada->diretorio,(strlen(entrada->diretorio)-1)); // retira o \0 do final da string, dá problema na comaparação se nao tirar
  if (!strcmp(nome, "..")){
    chdir("..");
  }else
    chdir(nome);
}

void client_LLS() {
  system ("ls");
}
