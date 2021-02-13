#include "funcoes.h"

void server_CD(estrutura_pacote *p, int socket) {
  char *novo = (char*)malloc(sizeof(char) * 15);
  strncpy(novo, p-> dados, p->tamanho);
  char *string = (char*)malloc( sizeof(char)* 256);
  int tam;
  estrutura_pacote *p1;
  tam = p-> tamanho;
  printf("%d\n", tam);
  printf("%s\n", novo);
  if (chdir(novo) == 0) {

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

int split_string(char *string,int cont, int tam, int socket) {
  int tam2;
  char *quebra = (char*)malloc( sizeof(char)* 256);
  char *recebe = (char*)malloc( sizeof(char)* 256);
  char *ultima_mensagem = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1,*p2;

  while (tam > 15) {
    strncpy(quebra, string, 15);
    tam2= strlen(quebra);
    printf(" while %s %d\n ", quebra,tam2);
    p1 = protocolo_server(quebra, 11, tam2, cont);
    ultima_mensagem = protocolo_string(p1);
    printf("%s\n", ultima_mensagem);
    envia_protocolo(ultima_mensagem, socket);
    recebe = recebe_protocolo(socket);
    //printf( "chegando %s\n",string);
    p2 = abre_protocolo(recebe);
    if (p2->tipo == 8) {
      printf("ACK DO CLIENTE\n");
    }else if (p2->tipo == 9) {
      envia_protocolo(ultima_mensagem, socket);
    }
    cont++;
    for (int i = 0; i < 15; i++) string++;
    tam -=15;
  }

  p1 = protocolo_server(string, 11, strlen(string), cont);
  ultima_mensagem = protocolo_string(p1);
  printf( "ultima %s\n", ultima_mensagem);
  envia_protocolo(ultima_mensagem, socket);
  recebe = recebe_protocolo(socket);
  //printf( "chegando %s\n",string);
  p2 = abre_protocolo(recebe);
  if (p2->tipo == 8) {
    printf("ACK DO CLIENTE\n");
  }else if (p2->tipo == 9) {
    envia_protocolo(ultima_mensagem, socket);
  }
  cont++;

  return cont;
}

int server_LS(estrutura_pacote *p, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  char *ultima = (char*)malloc( sizeof(char)* 256);
    int cont = 0;
    int tam = 0;
    estrutura_pacote *p1, *p2;
    struct dirent *de;
    DIR *dr = opendir(".");

    if (dr == NULL)
        printf("ERRO" );

    else
      while ((de = readdir(dr)) != NULL){
        printf("%s\n", de->d_name);
        tam = strlen(de->d_name);
        printf( "tam = %d\n",tam );
        if (tam > 15) {
          cont = split_string(de->d_name,cont, tam, socket);
        } else {
          p1 = protocolo_server(de->d_name, 11, tam, cont);
          ultima = protocolo_string(p1);
          printf("%s\n", ultima);
          envia_protocolo(ultima, socket);
          string = recebe_protocolo(socket);
          //printf( "chegando %s\n",string);
          p2 = abre_protocolo(string);
          cont++;
          if (p2->tipo == 8) {
            printf("ACK DO CLIENTE\n");
          }else if (p2->tipo == 9) {
            envia_protocolo(ultima, socket);
          }
        }
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
