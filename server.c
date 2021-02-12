#include "funcoes.h"

void server_CD(estrutura_pacote *p, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  struct stat fileStat;

  estrutura_pacote *p1;

  if (chdir(p->dados) == 0) {
    printf("dd\n");
    p1 = protocolo_server("", ACK, 0, 0);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
    imprime_path();
  } else {
    p1 = protocolo_server("2", 15, strlen("2"), 0);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
    printf("saindo %s\n", string);
  }
}

void server_VER(estrutura_pacote *p, int socket) {
  /* code */
}

int server_LS(estrutura_pacote *p, int socket) {
    int cont =0;
    char *string = (char*)malloc( sizeof(char)* 256);
    estrutura_pacote *p1;
    struct dirent *de;
    DIR *dr = opendir(".");

    if (dr == NULL)
        printf("ERRO" );

    else
      while ((de = readdir(dr)) != NULL){
        printf("%s\n", de->d_name);
        p1 = protocolo_server(de->d_name, 1011, strlen(de->d_name), cont);
        string = protocolo_string(p1);
        envia_protocolo(string, socket);
        string = recebe_protocolo(socket);
      //  p1 = abre_protocolo(entrada_server);
        cont++;
      }
      p1 = protocolo_server("", 1101, 0, cont);
      string = protocolo_string(p1);
      envia_protocolo(string, socket);

    closedir(dr);
    return 0;

}

void funcoes_server(int socket_confirmado) {

  char *entrada_server = (char*)malloc( sizeof(char)* 256);
  while (1) {

    entrada_server = recebe_protocolo(socket_confirmado);
    if (strlen(entrada_server) > 5) {
      imprime_path();
      printf(" PACOTE RECEBIDO = %s\n",entrada_server );
      estrutura_pacote *p = abre_protocolo(entrada_server);
      switch (p -> tipo) {
        case 0:       //CD
          server_CD(p, socket_confirmado);
          break;
        case 1:       //LS
          server_LS(p, socket_confirmado);
          printf("ls\n");
          break;
        case 10:      //VER
          server_VER(p, socket_confirmado);
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
