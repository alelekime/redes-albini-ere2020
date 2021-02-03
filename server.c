#include "funcoes.h"

void server_CD(estrutura_pacote *p, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  struct stat fileStat;

  estrutura_pacote *p1;
  // if ((mkdir(p -> dados, 0777)) == -1) {
  //   p1 = protocolo("2", ERR, 0, 0);
  //   string = protocolo_string(p1);
  //   envia_protocolo(string, socket);
  // } else
   if ((fileStat.st_mode & S_IRUSR) || (fileStat.st_mode & S_IRGRP) || (fileStat.st_mode & S_IROTH)){ // verifica se existe permissão para abrir o diretório
    printf("FAZENDO CD\n");
    if(!strcmp(p->dados, "..")){
      chdir("..");
  } else{
      printf("%s\n", p->dados);
      chdir(p->dados);
    }
    p1 = protocolo(" ", ACK, 0, 0);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
    imprime_path();
  } else{ // caso de não ter permissão
    printf("NÃO TEM PERMISSÃO\n" );
    p1 = protocolo("1", ERR, strlen("1"), 0);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
  }
}


void funcoes_server(int socket_confirmado) {

  char *entrada_server = (char*)malloc( sizeof(char)* 256);
  while (1) {

    entrada_server = recebe_protocolo(socket_confirmado);
    if (strlen(entrada_server) > 0) {
      imprime_path();
      printf(" PACOTE RECEBIDO = %s\n",entrada_server );
      estrutura_pacote *p = abre_protocolo(entrada_server);
      switch (p -> tipo) {
        case 0:       //CD
          server_CD(p, socket_confirmado);
          break;
        case 1:       //LS
          printf("ls\n");
          break;
        case 10:      //VER
          break;
        case 11:      // LINHA
          break;
        case 100:     //LINHAS
          break;
        case 101:     //EDIT
          break;
        case 1010:   //linha inicial e final
          break;
        case 1101:   //fim
          break;
      }
    }
  }
}
