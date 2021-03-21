#include "funcoes.h"

//https://codeforwin.org/2018/03/c-program-find-file-properties-using-stat-function.html


estrutura_pacote *protocolo(char *dado,int tipo, int tam, int seq) {
  char *novo = (char*)malloc(sizeof(char) * 15);
  strncpy(novo, dado,(strlen(dado)-1)); // retira o \0 do final da string
  estrutura_pacote *p1 = (estrutura_pacote*)malloc(sizeof(estrutura_pacote));

  p1 -> marcador = "01111110";
  p1 -> tamanho = acha_binario(tam);
  p1 -> endereco_origem = "10";
  p1 -> endereco_destino ="01";
  p1 -> sequencia = seq;
  p1 -> tipo = tipo;
  if (p1-> tipo == 1) {
      p1 -> dados = dado;
  } else {
      p1 -> dados = novo;
      if (tam > 0) {
        p1 -> tamanho = tam -1 ;
      }
  }
  p1 -> pariedade = cal_pariedade(tam, seq, tipo, dado, p1-> endereco_origem, p1->endereco_destino);
  return p1;
}

estrutura_pacote *protocolo_server(char *dado, int tipo, int tam, int seq) {
  estrutura_pacote *p1 = (estrutura_pacote*)malloc(sizeof(estrutura_pacote));
  char *binary = (char*)malloc( sizeof(char)* 4);

  p1 -> marcador = "01111110";
  p1 -> tamanho = acha_binario(tam);
//  printf("TAMANHO DO DADO DE POIS DA FUNCAO ACHA BINARIO %d\n", p1->tamanho);
  p1 -> endereco_origem = "01";
  p1 -> endereco_destino ="10";
  p1 -> sequencia = seq;
  p1 -> tipo = tipo;
  //printf("erro %s\n", int2bin(p1-> tamanho, 4));
  p1-> dados = dado;
  p1 -> pariedade = cal_pariedade(tam, seq, tipo, dado, p1-> endereco_origem, p1->endereco_destino);

//  mostra_protocolo(p1);
  //printf("TAMANHO DO DADO NA FUNCAO CONVERT BINARIO %s\n",  convert_binary(p1 -> tamanho, binary));
  return p1;
}

char *protocolo_string(estrutura_pacote * p1) {
  char *string = (char*)malloc( sizeof(char)* 256);
  char *binary = (char*)malloc( sizeof(char)* 4);

  snprintf(string,256,"%s%s%s%s%s%s%s%s", p1 -> marcador,  p1 -> endereco_origem, p1 -> endereco_destino, convert_binary(p1 -> tamanho, binary), int2bin(p1-> sequencia, 4), int2bin(p1-> tipo, 4), p1 -> dados, int2bin(p1 -> pariedade, 8));

//  printf(" string %s\n",string );
  return string;
}

int envia_protocolo(char *string, int socket) {
//  printf("ENVIANDO PACOTE\n");
  if (strlen(string) > 5) {
    if ((send (socket, string, 256, 0)) == -1) {
  		perror("send");
    }
  }
  return 0;
}

char *recebe_protocolo(int socket) {
  char *entrada_server = (char*)malloc(sizeof(char)*256);
  if ((recv (socket, entrada_server, 256, 0) == -1)) {
    perror("recv");
  }
  return entrada_server;
}

estrutura_pacote* abre_protocolo(char *entrada_server) {
  estrutura_pacote *p = (estrutura_pacote*)malloc(sizeof(estrutura_pacote));
  char marca [] = "01111110";
  char * aux = malloc(sizeof(char) * 4);
  char *dados1 = malloc(sizeof(char) * 15);
  char* pont = dados1;
  int num = 0;
  int soma = 0;
  int aux1 = 1000;
  int endereco;
  for (int i = 0; i < 8; i++){
    if (*entrada_server != marca[i]) //PACOTE NÃO VERIFICADO
      return 0;
    *entrada_server++;
  }

  for (int i = 0; i < 4; i++) {
    *aux = *entrada_server;
    num = *aux - '0';
    num = num * aux1;
    soma = soma + num;
    aux1 = aux1/10;
    *entrada_server++;
  }
  endereco = soma;
  if (endereco == 1001) {
    p -> endereco_origem = "10";
    p -> endereco_destino ="01";
  } else {
    p -> endereco_origem = "01";
    p -> endereco_destino ="10";
  }

  soma = 0;
  aux1 = 1000;
  for (int i = 0; i < 4; i++) {
    *aux = *entrada_server;
    num = *aux - '0';
    num = num * aux1;
    soma = soma + num;
    aux1 = aux1/10;
    *entrada_server++;
  }
  p -> tamanho = convert(soma);
  //printf("%d\n",  p -> tamanho );

  soma = 0;
  aux1 = 1000;
  for (int i = 0; i < 4; i++) {
    *aux = *entrada_server;
    num = *aux - '0';
    num = num * aux1;
    soma = soma + num;
    aux1 = aux1/10;
    *entrada_server++;
  }
  p -> sequencia = convert(soma);
  //printf(" sequencia %d\n", p-> sequencia);
  soma = 0;
  aux1 = 1000;
  for (int i = 0; i < 4; i++) {
    *aux = *entrada_server;
    num = *aux - '0';
    num = num * aux1;
    soma = soma + num;
    aux1 = aux1/10;
    *entrada_server++;
  }
  p -> tipo = convert(soma);

  // printf(" tipo %s\n", int2bin(p-> tipo, 4));
  // printf(" tipo %d\n", p-> tipo);

  soma = 0;
  for (int i = 0; i < p-> tamanho; i++) {
    *dados1 = *entrada_server;
    *dados1++;
    *entrada_server++;
  }
  p-> dados = strdup(pont);

  //printf( "dentro = %s\n", p->dados);
  soma = 0;
  aux1 = 10000000;
  for (int i = 0; i < 8; i++) {
    *aux = *entrada_server;
    num = *aux - '0';
    num = num * aux1;
    soma = soma + num;
    aux1 = aux1/10;
    *entrada_server++;
  }
  p -> pariedade = soma;
  return p;
}

int client_EDIT(linha_comando *entrada, int socket){
  int fd = 0;
  int time;
  int i = 0;
  int k;
  struct pollfd fds[1];
  char *linha = malloc( sizeof(int));
  char *quebra = (char*)malloc( sizeof(char)* 15);
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1, *p, *p2;
  p1 = protocolo_server(entrada -> nome_arq,5, strlen(entrada-> nome_arq), 0);
  printf("%d",p1->tamanho );

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
  p1 = protocolo_server(linha,5, strlen(linha), 1);
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
    p1 = protocolo_server(entrada->dados, 12, strlen(entrada->dados), 0);
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
      //printf( "chegando %s\n",string);
      p2 = abre_protocolo(string);
      if (p2 -> tipo == 8) {
        printf("ACK DO CLIENTE\n");
      }else if (p2->tipo == 9) {
        envia_protocolo(string, socket);
      }
    }
  } else {
    k = i % 15;
    for (int j = 0; j < i/15; j++) {
      strncpy(quebra, entrada->dados,15);
      printf("%s\n", quebra);
      p1 = protocolo_server(quebra, 12, strlen(quebra), j);
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
        //printf( "chegando %s\n",string);
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
      p1 = protocolo_server(entrada->dados, 12, strlen(entrada->dados), 0);
      //printf("FIM %s\n", entrada->dados);
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
        //printf( "chegando %s\n",string);
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
  p1 = protocolo_server("",13, 0, 0);
  string = protocolo_string(p1);
  printf("%s\n", string);
  envia_protocolo(string, socket);

}

int client_LINHAS(linha_comando *entrada, int socket){
  int fd = 0;
  int time;
  struct pollfd fds[1];
  char *linha = malloc( sizeof(int));
  char *saida = (char*)malloc( sizeof(char)* 256);
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p2,*p1, *p;

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
  p1 = protocolo_server(linha,4, strlen(linha), 1);
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
  p1 = protocolo_server(linha,4, strlen(linha), 1);
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
     //printf("%s\n", string);
        p1 = abre_protocolo(string);
        printf("%s", p1->dados);
        if (p1 -> tipo == 13) {
          printf("\nACABOU A TRANSMISSAO\n");
          p2 = protocolo_server("", ACK, strlen(""), 0);
          string = protocolo_string(p2);
          envia_protocolo(string, socket);
          return 0;
        }else if (p1 -> tipo == 15 && !(strcmp(p1->dados, "3")) )  {
            printf("\nERRO ENCONTRADO NO ARQUIVO\n");
            p2 = protocolo_server("", ACK, strlen(""), 0);
            string = protocolo_string(p2);
            envia_protocolo(string, socket);
            return 0;
        }else if (p1 -> tipo == 15 && !(strcmp(p1->dados, "4")) )  {
            printf("\nERRO ENCONTRADO %s\n NÃO EXISTE ESSA LINHA \n",p1->dados);
            p2 = protocolo_server("", ACK, strlen(""), 0);
            string = protocolo_string(p2);
            envia_protocolo(string, socket);
            return 0;
        }else  if (p1 -> tipo == 15 && !(strcmp(p1->dados, "5")) )  {
            p2 = protocolo_server("", ACK, strlen(""), 0);
            string = protocolo_string(p2);
            envia_protocolo(string, socket);
            printf("\nERRO ENCONTRADO, LINHA FINAL MENOR QUE A INICIAL\n");
            return 0;
        }

        p2 = protocolo_server("", ACK, strlen(""), 0);
        string = protocolo_string(p2);
        envia_protocolo(string, socket);
      }
    }
  }

}

int client_LINHA(linha_comando *entrada, int socket) {
  int fd = 0;
  int time;
  struct pollfd fds[1];
  char *linha = malloc( sizeof(int));
  char *saida = (char*)malloc( sizeof(char)* 256);
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
  p1 = protocolo_server(linha,3, strlen(linha), 1);
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
        // printf("%s\n", string);
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
      }
    }
  }

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
      p = protocolo_server("", 8, 0, 0);
      saida = protocolo_string(p);
      printf("%s\n", saida);

      envia_protocolo(saida, socket);
    }
  }
  p = protocolo_server("", 8, 0, 0);
  saida = protocolo_string(p);
  printf("%s\n", saida);
  envia_protocolo(saida, socket);
}

void client_CD(linha_comando *entrada, int socket) {
  int fd = 0;
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
      verifica(p1);
      printf("%s\n",string);
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
  int fd = 0;
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
        p = protocolo_server("", ACK, strlen(""), 0);
        string = protocolo_string(p);
        envia_protocolo(string, socket);
        return true;
      }else if (p1-> tipo == 13){
        printf("Fim da transmissao\n");
        p = protocolo_server("", ACK, strlen(""), 0);
        string = protocolo_string(p);
        envia_protocolo(string, socket);
        return false;
      }else {
          printf("Erro não é conteudo do ls \n");
          return false;
      }
    }
  }
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

void le_comando( linha_comando *entrada, int socket) {
  char *entrada_buffer, *dados, *quebra, *nome ;
  nome = (char*)malloc(sizeof(char));
  fgets (entrada_buffer, ENTRADA_MAX, stdin);// recebe a linha de comdo do usuário
  dados = strdup (entrada_buffer); //aloca memoria do tamanho da linha de comando mais \0
  strncpy(nome, dados,(strlen(dados)-1)); // retira o \0 do final da string, dá problema na comaparação do ls e lls
  quebra = strdup(strtok(dados," "));
  entrada->comando = quebra;
  if(!strcmp(entrada->comando, "cd")){
     quebra =  strdup(strtok(NULL, ""));//<nome_dir>
     entrada-> diretorio = quebra;
     client_CD(entrada, socket);   //efetua a troca de diretório - servidor
  }else if(!strcmp(quebra, "lcd")){
     quebra =  strdup(strtok(NULL, ""));//<nome_dir>
     entrada-> diretorio = quebra;
     client_LCD(entrada);   //efetua a troca de diretório - cliente
  }else if(!strcmp(nome, "ls")){
    client_LS(entrada, socket); //lista arquivos - servidor
  }else if(!strcmp(nome, "lls")){
    client_LLS(); //lista arquivos - cliente
  }else if(!strcmp(quebra, "ver")){
     quebra =  strdup(strtok(NULL, ""));//<nome_arq>
     entrada-> nome_arq = quebra;
     client_VER(entrada, socket); //mostra o arquivo com as linhas numeradas
  }else if(!strcmp(quebra, "linha")){
    entrada -> linha =  atoi(strdup(strtok(NULL, " ")));//<numero_linha>
    quebra =  strdup(strtok(NULL, ""));//<nome_arq>
    entrada-> nome_arq = quebra;
    client_LINHA(entrada, socket); // mostra a linha escolida do arquivo
  }
  else if(!strcmp(quebra, "linhas")){
    entrada -> linha_inicial =  atoi(strdup(strtok(NULL, " ")));//<numero_linha_inicial>
    entrada -> linha_final =  atoi(strdup(strtok(NULL, " ")));//<numero_linha_final>
    quebra =  strdup(strtok(NULL, ""));//<nome_arq>
    entrada-> nome_arq = quebra;
    client_LINHAS(entrada, socket);//mostra as linhas escolidas do arquivo
  }else if (!strcmp(quebra, "edit")){
    entrada -> linha =  atoi(strdup(strtok(NULL, " "))); //<numero_linha>
    quebra =  strdup(strtok(NULL, " "));//<nome_arq>
    entrada-> nome_arq = quebra;
    quebra =  strdup(strtok(NULL, ""));//“<NOVO_TEXTO>”
    entrada->dados = quebra;
    printf("%s\n", quebra);
   client_EDIT(entrada, socket);// troca a linha escolhida por um texto
  }

}
