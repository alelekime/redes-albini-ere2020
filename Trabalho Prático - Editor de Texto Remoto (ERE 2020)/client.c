#include "funcoes.h"

int client_EDIT(linha_comando *entrada, int socket){

  int time;
  int i = 0;
  int k;
  struct pollfd fds[1];
  char *linha = malloc( sizeof(int));
  char *quebra = (char*)malloc( sizeof(char)* 15);
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1, *p2;
  p1 = protocolo_client(entrada -> nome_arq,5, strlen(entrada-> nome_arq), 0);


  string = protocolo_string(p1);
  printf("%s\n", string);
  envia_protocolo(string, socket);
  fds[0].fd = socket;
  fds[0].events = 0;
  fds[0].events |= POLLIN;
  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("TIMEOUT!\n");
    return 0;
  } else {
    while (1) {
      string = recebe_protocolo(socket);
      if (strlen(string) > 5){
        p1 = abre_protocolo(string);
        if (p1 -> tipo == 8) {
          printf("ACK = RECEBEU O NOME DO ARQUIVO \n");
        }else if (p1 -> tipo == 9) {
          printf("\nNACK\n");
        } else if (p1 -> tipo == 15 ) {
          printf("\nERRO ENCONTRADO%s\n%sNÃO EXISTE ESSE ARQUIVO \n",p1->dados, entrada->nome_arq);
          return 0;
        }
        break;
      }
    }
  }
  snprintf(linha, 10, "%d",entrada -> linha);
  p1 = protocolo_client(linha,5, strlen(linha), 1);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
  fds[0].fd = socket;
  fds[0].events = 0;
  fds[0].events |= POLLIN;
  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("TIMEOUT!\n");
    return 0;
  }else {
    while (1) {
      string = recebe_protocolo(socket);
      if (strlen(string) > 5){
        p1 = abre_protocolo(string);
        if (p1 -> tipo == 8) {
          printf("ACK = RECEBEU O NUMERO DA LINHA\n");
        }else if (p1 -> tipo == 9) {
          printf("\nNACK\n");
        } else if (p1 -> tipo == 15) {
            printf("\nERRO ENCONTRADO %s\n%sNÃO EXISTE ESSE ARQUIVO \n",p1->dados, entrada->nome_arq);
        }
        break;
      }
    }
  }


  i = strlen(entrada->dados);
  if (i < 15 ) {
    p1 = protocolo_client(entrada->dados, 12, strlen(entrada->dados), 0);
    string = protocolo_string(p1);
    printf("%s\n", string);
    envia_protocolo(string, socket);
    fds[0].fd = socket;
    fds[0].events = 0;
    fds[0].events |= POLLIN;
    time = poll(fds, 1, 3500);
    if (time == 0){
      printf("TIMEOUT!\n");
      return 0;
    }else {
      string = recebe_protocolo(socket);
      p2 = abre_protocolo(string);
      if (p2 -> tipo == 8) {
        printf("ACK DO CLIENTE\n");
      }else if (p2->tipo == 9) {
        envia_protocolo(string, socket);
      }else if (p2 -> tipo == 15) {
        printf("\nERRO ENCONTRADO %d NÃO EXISTE ESSA LINHA \n", entrada->linha);
        return 0;
      }
    }
  } else {
    k = i % 15;
    for (int j = 0; j < i/15; j++) {
      strncpy(quebra, entrada->dados,15);
      printf("%s\n", quebra);
      p1 = protocolo_client(quebra, 12, strlen(quebra), j);
      string = protocolo_string(p1);
      printf("%s\n", string);
      envia_protocolo(string, socket);
      fds[0].fd = socket;
      fds[0].events = 0;
      fds[0].events |= POLLIN;
      time = poll(fds, 1, 3500);
      if (time == 0){
        printf("TIMEOUT!\n");
        return 0;
      }else {
        string = recebe_protocolo(socket);
        p2 = abre_protocolo(string);
        if (p2 -> tipo == 8) {
          printf("ACK DO CLIENTE\n");
        }else if (p2->tipo == 9) {
          envia_protocolo(string, socket);
        }
      }
      for (int l = 0; l < 15; l++) {
        entrada->dados++;
      }
    }
    if (k > 0) {
      p1 = protocolo_client(entrada->dados, 12, strlen(entrada->dados), 0);
      string = protocolo_string(p1);
      printf("%s\n", string);
      envia_protocolo(string, socket);
      fds[0].fd = socket;
      fds[0].events = 0;
      fds[0].events |= POLLIN;
      time = poll(fds, 1, 3500);
      if (time == 0){
        printf("TIMEOUT!\n");
        return 0;
      }else {
        string = recebe_protocolo(socket);
        p2 = abre_protocolo(string);

        if (p2 -> tipo == 8) {
          printf("ACK DO CLIENTE\n");
        }else if (p2->tipo == 9) {
          envia_protocolo(string, socket);
        }
      }
    }

  }
  printf("ACABOU\n");
  p1 = protocolo_client("",13, 0, 0);
  string = protocolo_string(p1);
  printf("%s\n", string);
  envia_protocolo(string, socket);
  return 1;
}

int client_LINHAS(linha_comando *entrada, int socket){

  int time;
  struct pollfd fds[1];
  char *linha = malloc( sizeof(int));
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p2,*p1;

  p1 = protocolo(entrada -> nome_arq,4, strlen(entrada-> nome_arq), 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);

  fds[0].fd = socket;
  fds[0].events = 0;
  fds[0].events |= POLLIN;

  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("TIMEOUT!\n");
    return 0;
  } else {
    while (1) {
      string = recebe_protocolo(socket);
      if (strlen(string) > 5){
        p1 = abre_protocolo(string);
        if (p1 -> tipo == 8) {
          printf("ACK = RECEBEU O NOME DO ARQUIVO \n");
        }else if (p1 -> tipo == 9) {
          printf("\nNACK\n");
        } else if (p1 -> tipo == 15 ) {
          printf("\nERRO ENCONTRADO%s\n%sNÃO EXISTE ESSE ARQUIVO \n",p1->dados, entrada->nome_arq);
          return 0;
        }
        break;
      }
    }
  }
  snprintf(linha, 10, "%d",entrada -> linha_inicial);
  p1 = protocolo_client(linha,4, strlen(linha), 1);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("TIMEOUT!\n");
    return 0;
  }else {
    while (1) {
      string = recebe_protocolo(socket);
      if (strlen(string) > 5){
        p1 = abre_protocolo(string);
        if (p1 -> tipo == 8) {
          printf("ACK = RECEBEU O NUMERO DA LINHA INICIAL\n");
        }else if (p1 -> tipo == 9) {
          printf("\nNACK\n");
        } else if (p1 -> tipo == 15) {
            printf("\nERRO ENCONTRADO %s\n%sNÃO EXISTE ESSE ARQUIVO \n",p1->dados, entrada->nome_arq);
        }
        break;
      }
    }
  }

  snprintf(linha, 10, "%d",entrada -> linha_final);
  p1 = protocolo_client(linha,4, strlen(linha), 1);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
  fds[0].fd = socket;
  fds[0].events = 0;
  fds[0].events |= POLLIN;
  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("TIMEOUT!\n");
    return 0;
  }else {
    while (1) {
      string = recebe_protocolo(socket);
      if (strlen(string) > 5){
        p1 = abre_protocolo(string);
        if (p1 -> tipo == 8) {
          printf("ACK = RECEBEU O NUMERO DA LINHA FINAL\n");
        }else if (p1 -> tipo == 9) {
          printf("\nNACK\n");
        } else if (p1 -> tipo == 15) {
            printf("\nERRO ENCONTRADO %s\n%sNÃO EXISTE ESSE ARQUIVO \n",p1->dados, entrada->nome_arq);
        }
        break;
      }
    }
  }
  printf("DADO = ");
  while (1) {

    fds[0].fd = socket;
    fds[0].events = 0;
    fds[0].events |= POLLIN;
    time = poll(fds, 1, 3500);
    if (time == 0){
      printf("TIMEOUT!\n");
      return 0;
    }else {

      string = recebe_protocolo(socket);
      if (strlen(string) > 5){
        p1 = abre_protocolo(string);
        printf("%s", p1->dados);
        if (p1 -> tipo == 13) {
          printf("\nACABOU A TRANSMISSAO\n");
          p2 = protocolo_client("", ACK, strlen(""), 0);
          string = protocolo_string(p2);
          envia_protocolo(string, socket);
          return 0;
        }else if (p1 -> tipo == 15 && !(strcmp(p1->dados, "3")) )  {
            printf("\nERRO ENCONTRADO NO ARQUIVO\n");
            p2 = protocolo_client("", ACK, strlen(""), 0);
            string = protocolo_string(p2);
            envia_protocolo(string, socket);
            return 0;
        }else if (p1 -> tipo == 15 && !(strcmp(p1->dados, "4")) )  {
            printf("\nERRO ENCONTRADO %s\n NÃO EXISTE ESSA LINHA \n",p1->dados);
            p2 = protocolo_client("", ACK, strlen(""), 0);
            string = protocolo_string(p2);
            envia_protocolo(string, socket);
            return 0;
        }else  if (p1 -> tipo == 15 && !(strcmp(p1->dados, "5")) )  {
            p2 = protocolo_client("", ACK, strlen(""), 0);
            string = protocolo_string(p2);
            envia_protocolo(string, socket);
            printf("\nERRO ENCONTRADO, LINHA FINAL MENOR QUE A INICIAL\n");
            return 0;
        }

        p2 = protocolo_client("", ACK, strlen(""), 0);
        string = protocolo_string(p2);
        envia_protocolo(string, socket);
      }
    }
  }

}

int client_LINHA(linha_comando *entrada, int socket) {

  int time;
  struct pollfd fds[1];
  char *linha = malloc( sizeof(int));
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1, *p;

  p1 = protocolo(entrada -> nome_arq,3, strlen(entrada-> nome_arq), 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);

  fds[0].fd = socket;
  fds[0].events = 0;
  fds[0].events |= POLLIN;

  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("TIMEOUT!\n");
    return 0;
  } else {
    while (1) {
      string = recebe_protocolo(socket);
      if (strlen(string) > 5){
        p1 = abre_protocolo(string);
        if (p1 -> tipo == 8) {
          printf("ACK = RECEBEU O NOME DO ARQUIVO \n");
        }else if (p1 -> tipo == 9) {
          printf("\nNACK\n");
        } else if (p1 -> tipo == 15 ) {
          printf("\nERRO ENCONTRADO%s\n%sNÃO EXISTE ESSE ARQUIVO \n",p1->dados, entrada->nome_arq);
          return 0;
        }
        break;
      }
    }
  }
  snprintf(linha, 10, "%d",entrada -> linha);
  p1 = protocolo_client(linha,3, strlen(linha), 1);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("TIMEOUT!\n");
    return 0;
  }else {
    while (1) {
      string = recebe_protocolo(socket);
      if (strlen(string) > 5){
        p1 = abre_protocolo(string);
        if (p1 -> tipo == 8) {
          printf("ACK = RECEBEU O NUMERO DA LINHA\n");
        }else if (p1 -> tipo == 9) {
          printf("\nNACK\n");
        } else if (p1 -> tipo == 15) {
            printf("\nERRO ENCONTRADO %s\n%sNÃO EXISTE ESSE ARQUIVO \n",p1->dados, entrada->nome_arq);
        }
        break;
      }
    }
  }

  printf("DADO = ");
  while (1) {

    fds[0].fd = socket;
    fds[0].events = 0;
    fds[0].events |= POLLIN;
    time = poll(fds, 1, 3500);
    if (time == 0){
      printf("TIMEOUT!\n");
      return 0;
    }else {

      string = recebe_protocolo(socket);

      if (strlen(string) > 5){
        p1 = abre_protocolo(string);
       printf("%s", p1->dados);
        if (p1-> tipo == 6) {
          printf("ESCOLHA OUTRA LINHA\n");
          break;
        }else if (p1 -> tipo == 13) {
          printf("\nACABOU A TRANSMISSAO\n");
          break;
        }else if (p1 -> tipo == 15) {
            printf("\nERRO ENCONTRADO %s\n%d NÃO EXISTE ESSA LINHA \n",p1->dados, entrada->linha);
            break;
        }
        p = protocolo_client("", ACK, 0, 0);
        string = protocolo_string(p);
        envia_protocolo(string, socket);
      }
    }
  }
  p = protocolo_client("", ACK, 0, 0);
  string = protocolo_string(p);
  envia_protocolo(string, socket);
  return 1;

}

void client_VER(linha_comando *entrada, int socket) {
  struct pollfd fds[1];
  int linha = 2;
  int time;
  char *string = (char*)malloc( sizeof(char)* 256);
  char *caracter = (char*)malloc(sizeof(char) );
  char *saida = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1, *p;
  p1 = protocolo(entrada -> nome_arq,2, strlen(entrada-> nome_arq), 0);
  string = protocolo_string(p1);
  printf("%s\n",string);
  envia_protocolo(string, socket);
  printf("1 ");
  while (1){
    fds[0].fd = socket;
    fds[0].events = 0;
    fds[0].events |= POLLIN;
    time = poll(fds, 1, 3500);
    if (time == 0){
      printf("TIMEOUT!\n");
      break;
    }else {
      string = recebe_protocolo(socket);
      if (strlen(string) > 5) {
        p1 = abre_protocolo(string);
        if (p1 -> tipo == 15 && !(strcmp(p1->dados, "3")) ) {
          printf("ERRO = %s\n",p1-> dados);
          printf("\nERRO ENCONTRADO\n%sNÃO EXISTE ESSE ARQUIVO\n", entrada-> nome_arq);
          break;
        }else if (p1-> tipo == 13){
          printf("\nFim da transmissao\n");
          break;
        }else {
          for (size_t i = 0; i < 15; i++) {
            strncpy(caracter, p1-> dados, 1);
              if (!strcmp(caracter, "\n")) {
                printf("%c%d ",*p1-> dados, linha);
                linha++;
              } else {
                  printf("%c",*p1-> dados);
              }
            *p1->dados++;
          }
        }
      }
      p = protocolo_client("", 8, 0, 0);
      saida = protocolo_string(p);
      envia_protocolo(saida, socket);
    }
  }
  p = protocolo_client("", 8, 0, 0);
  saida = protocolo_string(p);
  printf("%s\n", saida);
  envia_protocolo(saida, socket);
}

void client_CD(linha_comando *entrada, int socket) {

  int time;
  struct pollfd fds[1];
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1 = protocolo(entrada -> diretorio, CD, strlen(entrada->diretorio), 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
  while (1){
    fds[0].fd = socket;
    fds[0].events = 0;
    fds[0].events |= POLLIN;
    time = poll(fds, 1, 3500);
    if (time == 0){
      printf("TIMEOUT!\n");
      break;

    } else {
      string = recebe_protocolo(socket);
      if (strlen(string) > 5) {
      p1 = abre_protocolo(string);
      printf("%s\n",string);
      if(!strcmp(p1-> endereco_origem, "01"))
        break;
      }
    }
  }
  if (p1 -> tipo == 8) {
    printf("\nACK\n");
  }else if (p1 -> tipo == 9) {
    printf("\nNACK\n");
  } else if (p1 -> tipo == 7 && !(strcmp(p1->dados, "2")) ) {
    printf("\nERRO ENCONTRADO\n%sNÃO EXISTE ESSE DIRETÓRIO\n", entrada-> diretorio);

    printf("%s\n",p1-> dados);
  }
}

bool recebe_ls(estrutura_pacote * p1, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote * p;

  int time;
  struct pollfd fds[1];
  fds[0].fd = socket;
  fds[0].events = 0;
  fds[0].events |= POLLIN;
  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("TIMEOUT!\n");
    return false;
  } else {
    string = recebe_protocolo(socket);
    if (strlen(string) > 5){
      p1 = abre_protocolo(string);
      if (p1-> tipo == 11) {
        printf("%s", p1->dados);
        p = protocolo_client("", ACK, strlen(""), 0);
        string = protocolo_string(p);
        envia_protocolo(string, socket);
        return true;
      }else if (p1-> tipo == 13){
        printf("Fim da transmissao\n");
        p = protocolo_client("", ACK, strlen(""), 0);
        string = protocolo_string(p);
        envia_protocolo(string, socket);
        return false;
      }else {
          printf("Erro não é conteudo do ls \n");
          return false;
      }
    }
  }
  return 1;

}

void client_LS(linha_comando *entrada, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1 = protocolo("ls", LS, strlen("ls"), 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
  printf("CONTEÚDO DO LS\n");
  while (recebe_ls(p1,socket));

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
