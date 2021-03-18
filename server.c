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
  }


}

void server_VER(estrutura_pacote *p, int socket) {
  estrutura_pacote *p1, *p2;
  int fd = 0;
  struct pollfd fds[1];
  int seq = 0;
  int time ;
  char *caracters = (char*)malloc(sizeof(char)*15);
  char *string = (char*)malloc( sizeof(char)* 256);
  char *entrada = (char*)malloc( sizeof(char)* 256);
  FILE *arquivo;
  printf("%s\n",p->dados);
  arquivo = fopen (p->dados,"r");
  if (arquivo != NULL) {
    while (1) {
      fds[0].fd = socket;
      fds[0].events = 0;
    	fds[0].events |= POLLIN;
      if (!fread(caracters, sizeof(char), 15, arquivo)) {
        break;
      }
      p1 = protocolo_server(caracters, 12, strlen(caracters), seq);
      string = protocolo_string(p1);
      printf("%s\n", string);
      printf("%d\n",seq );
      envia_protocolo(string, socket);
      time = poll(fds, 1, 3500);
      if (time == 0){
        printf("TIMEOUT!\n");
        break;
      } else {
        entrada = recebe_protocolo(socket);
        printf("%s\n", entrada);
        p2 = abre_protocolo(entrada);
        printf("%d\n", p2->tipo);
        if (p2-> tipo == 15) {
          printf("ERRO, REENVIANDO\n");
          envia_protocolo(string, socket);
        } else {
          printf("ACK DO CLIENTE\n");
        }
        seq++;
      }
    }
    p1 = protocolo_server("", 13, 0, seq);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
  }else{
    printf("erro arq\n");
    p1 = protocolo_server("3", 15, strlen("3"), 0);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
  }
  fds[0].fd = socket;
  fds[0].events = 0;
  fds[0].events |= POLLIN;
  printf("FINAL DA TRANSMISSÃO\n");
  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("TIMEOUT!\n");
  } else {
    entrada = recebe_protocolo(socket);
    p2 = abre_protocolo(entrada);
      if (p2-> tipo == 15) {
        printf("ERRO, REENVIANDO\n");
        envia_protocolo(string, socket);
      } else {
        printf("ACK DO CLIENTE\n");
      }
    }

}

int server_EDIT(estrutura_pacote *p, int socket) {
  int fd = 0;
  int time;
  struct pollfd fds[1];
  estrutura_pacote *p1, *p2;
  int linha;
  int linha_arquivo = 0;
  int i = 0;
  int k;
  int t;
  bool final_da_linha = false;
  char *caracter = (char*)malloc(sizeof(char));
  char *string = (char*)malloc( sizeof(char)* 256);
  char *string_linha = (char*)malloc( sizeof(char)* 256);
  char *quebra = (char*)malloc( sizeof(char)* 15);
  FILE *arquivo;
  FILE *aux;
  arquivo = fopen (p->dados,"r");
  aux = fopen ("d.txt","a + b");


  if ((arquivo != NULL) && (aux != NULL)){
    p1 = protocolo_server("", ACK, 0, 0);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
    printf("NOME DO ARQUVO = %s\n",p->dados);
    string = recebe_protocolo(socket);
    p2 = abre_protocolo(string);
    linha = stringToDecimal(p2->dados);
    printf("NUMERO DA LINHA = %d \n", linha);
    p1 = protocolo_server("", ACK, 0, 0);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
    t = 0;
    printf("DADO = ");
    while (1) {
      if (!fread(caracter, 1, sizeof(char), arquivo)) {
        break;
      }
      fprintf(aux, "%s",caracter);
      if (!fwrite(caracter, 1, sizeof(char),aux)) {
        printf("ERRO\n");
        break;
      }
      printf("%s", caracter);
      if (!strcmp(caracter, "\n")) {
        linha_arquivo++;
      }

      if (linha_arquivo == linha - 1 ) {
        printf("%d\n",linha_arquivo);
        while (1){

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
              printf("%s\n", string);
              p1 = abre_protocolo(string);
             printf("%s", p1->dados);
             //fprintf(aux, "%s",p1->dados);

             if (!fwrite(p1->dados, 1, sizeof(p1->dados),aux)) {
               printf("ERRO\n");
               return 0;
             }
             printf("%d \n", p1->tipo);
              if (p1 -> tipo == 13) {
                printf("\nACABOU A TRANSMISSAO\n");
                break;
              }
              p1 = protocolo_server("", ACK, 0, 0);
              string = protocolo_string(p1);
              envia_protocolo(string, socket);
            }
          }
        }
      }
    }



  }else {
    p1 = protocolo_server("3", 15, strlen("3"), 0);
    string = protocolo_string(p1);
    printf("%s\n", string);
    envia_protocolo(string, socket);
  }

  return 0;
}

int server_LINHA(estrutura_pacote *p, int socket) {
  estrutura_pacote *p1, *p2;
  int linha;
  int linha_arquivo = 0;
  int i = 0;
  int k;
  int t;
  bool final_da_linha = false;
  char *caracter = (char*)malloc(sizeof(char));
  char *string = (char*)malloc( sizeof(char)* 256);
  char *string_linha = (char*)malloc( sizeof(char)* 256);
  char *quebra = (char*)malloc( sizeof(char)* 15);
  FILE *arquivo;
  arquivo = fopen (p->dados,"r");
  if (arquivo != NULL){
    p1 = protocolo_server("", ACK, 0, 0);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
    printf("NOME DO ARQUVO = %s\n",p->dados);
    string = recebe_protocolo(socket);
    p2 = abre_protocolo(string);
    linha = stringToDecimal(p2->dados);
    printf("NUMERO DA LINHA = %d \n", linha);
    p1 = protocolo_server("", ACK, 0, 0);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
    t = 0;
    while (1) {
      if (!fread(caracter, sizeof(char), 1, arquivo)) {
        break;
      }
    //  printf("%s", caracter);
      if (!strcmp(caracter, "\n")) {
        linha_arquivo++;
      }

      if (linha_arquivo == linha - 1 ) {
        printf("LINHA NO ARQUIVO = %d\n",linha_arquivo );

        while (1) {
          if (!fread(caracter, sizeof(char), 1, arquivo)) {
            break;
          }

          printf("%s\n",caracter );
          if(!strcmp(caracter, "\n")){
            if (t == 0) {
              p1 = protocolo_server("", 6, strlen(""), 0);
              printf("PULA LINHA");
              string = protocolo_string(p1);
              envia_protocolo(string, socket);
              p1 = protocolo_server("", 1101, 0, 0);
              string = protocolo_string(p1);
              envia_protocolo(string, socket);
              return 0;
            }
            break;
          }
          string_linha[i] = caracter[0];
          i++;
          t++;
        }
        i = strlen(string_linha);
        if (i < 15 ) {
          p1 = protocolo_server(string_linha, 12, strlen(string_linha), 0);
          string = protocolo_string(p1);
          envia_protocolo(string, socket);
        } else {
          k = i % 15;
          for (int j = 0; j < i/15; j++) {
            strncpy(quebra, string_linha,15);
            printf("QUEBRA %s\n", quebra);
            p1 = protocolo_server(quebra, 12, strlen(quebra), j);
            string = protocolo_string(p1);
            envia_protocolo(string, socket);
            for (int l = 0; l < 15; l++) {
              string_linha++;
            }
          }
          if (k > 0) {
            p1 = protocolo_server(string_linha, 12, strlen(string_linha), 0);
            printf("FIM %s\n", string_linha);
            string = protocolo_string(p1);
            envia_protocolo(string, socket);
          }
          p1 = protocolo_server("", 1101, 0, 0);
          string = protocolo_string(p1);
          envia_protocolo(string, socket);

        }
        return 0;
      }

    }

    p1 = protocolo_server("4", 15, strlen("4"), 0);
    string = protocolo_string(p1);
    printf("%s\n", string);
    envia_protocolo(string, socket);


  }else {
    p1 = protocolo_server("3", 15, strlen("3"), 0);
    string = protocolo_string(p1);
    printf("%s\n", string);
    envia_protocolo(string, socket);
  }


}

int LINHAS(FILE *arquivo, int linha, int socket) {
  int fd = 0;
  int time;
  struct pollfd fds[1];
  estrutura_pacote *p1, *p2;
  int linha_arquivo = 0;
  int i = 0;
  int k;
  int t;
  char *caracter = (char*)malloc(sizeof(char));
  char *string = (char*)malloc( sizeof(char)* 256);
  char *string_linha = (char*)malloc( sizeof(char)* 256);
  char *quebra = (char*)malloc( sizeof(char)* 15);

    t = 0;
    while (1) {
      if (!fread(caracter, sizeof(char), 1, arquivo)) {
        break;
      }
    //  printf("%s", caracter);
      if (!strcmp(caracter, "\n")) {
        linha_arquivo++;
      }

      if (linha_arquivo == linha - 1 ) {
        printf("LINHA NO ARQUIVO = %d\n",linha_arquivo );

        while (1) {
          if (!fread(caracter, sizeof(char), 1, arquivo)) {
            break;
          }
          string_linha[i] = caracter[0];
          i++;
          if(!strcmp(caracter, "\n")){
            break;
          }

          t++;
        }
      //  fclose(arquivo);
        i = strlen(string_linha);
        if (i < 15 ) {
          p1 = protocolo_server(string_linha, 12, strlen(string_linha), 0);
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
            strncpy(quebra, string_linha,15);
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
              string_linha++;
            }
          }
          if (k > 0) {
            p1 = protocolo_server(string_linha, 12, strlen(string_linha), 0);
            //printf("FIM %s\n", string_linha);
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
        //fclose(arquivo);
        return 0;
      }

    }

    p1 = protocolo_server("4", 15, strlen("4"), 0);
    string = protocolo_string(p1);
    //printf("%s\n", string);
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
    return 1;

}

int server_LINHAS(estrutura_pacote *p, int socket){
  int fd = 0;
  int time;
  struct pollfd fds[1];
  estrutura_pacote *p1, *p2, *p3;
  int linha = 0;
  int linha1, linha2;
  int linha_arquivo = 0;
  int i = 0;
  int k;
  int t;
  int y = 0;
  int erro;
  char *caracter = (char*)malloc(sizeof(char));
  char *string = (char*)malloc( sizeof(char)* 256);
  char *string_linha = (char*)malloc( sizeof(char)* 256);
  char *quebra = (char*)malloc( sizeof(char)* 15);

  p1 = protocolo_server("", ACK, 0, 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
  printf("NOME DO ARQUVO = %s\n",p->dados);

  string = recebe_protocolo(socket);
  p2 = abre_protocolo(string);
  linha1 = stringToDecimal(p2->dados);
  printf("NUMERO DA LINHA INICIAL = %d \n", linha1);
  p1 = protocolo_server("", ACK, 0, 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);

  string = recebe_protocolo(socket);
  p2 = abre_protocolo(string);
  linha2 = stringToDecimal(p2->dados);
  printf("NUMERO DA LINHA FINAL = %d \n", linha2);
  p1 = protocolo_server("", ACK, 0, 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
  t = 0;
  if (linha2 < linha1) {
    p1 = protocolo_server("5", 15, strlen("5"), 0);
    string = protocolo_string(p1);
    printf("%s\n", string);
    envia_protocolo(string, socket);
  } else{
    FILE *arquivo = fopen (p->dados,"r");
    if (arquivo != NULL){
      if (linha1 == linha2) {
      erro = LINHAS( arquivo, linha1,socket);
      if ((erro == 1) || (erro == 2))
        return 0;
      } else {
        for (int i = 0; i < linha2 -linha1+1; i++) {
          rewind(arquivo);
          erro = LINHAS(arquivo, linha1+i, socket);
          if ((erro == 1) || (erro == 2)) {
            printf("%d \n", erro);
            return 0;
          }
        }
      }
    } else {
      perror("Error printed by perror");
      p1 = protocolo_server("3", 15, strlen("3"), 0);
      string = protocolo_string(p1);
      printf("ERRO %s\n", string);
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


  fds[0].fd = socket;
  fds[0].events = 0;
  fds[0].events |= POLLIN;
  time = poll(fds, 1, 3500);
  if (time == 0){
    printf("FIM: TIMEOUT!\n");
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

int split_string(char *string,int cont, int tam, int socket) {
  int tam2;
  char *quebra = (char*)malloc( sizeof(char)* 256);
  char *recebe = (char*)malloc( sizeof(char)* 256);
  char *ultima_mensagem = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1,*p2;

  while (tam > 15) {
    strncpy(quebra, string, 15);
    tam2= strlen(quebra);
    printf(" DADO DENTRO DO WHILE  %s %d\n ", quebra,tam2);
    p1 = protocolo_server(quebra, 11, tam2, cont);
    ultima_mensagem = protocolo_string(p1);
    printf(" MESANGEM INDO PARA O CLIENTE%s\n", ultima_mensagem);
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
    for (int i = 0; i < 15; i++)
      string++;
    tam -=15;
  }

  p1 = protocolo_server(string, 11, strlen(string), cont);
  ultima_mensagem = protocolo_string(p1);
  printf( "SAIU DO WHILE, FINAL DO DADO  %s\n", ultima_mensagem);
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
  p1 = protocolo_server("  ", 11, strlen("  "), cont);
  ultima_mensagem = protocolo_string(p1);
  printf( "ultima %s\n", ultima_mensagem);
  envia_protocolo(ultima_mensagem, socket);
  recebe = recebe_protocolo(socket);
  cont++;

  return cont;
}

int server_LS(estrutura_pacote *p, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  char *ultima = (char*)malloc( sizeof(char)* 256);
  struct pollfd fds[1];
  int time;
  int cont = 0;
  int tam = 0;
  int enter = 0;
  estrutura_pacote *p1, *p2;
  struct dirent *de;
  DIR *dr = opendir(".");

  if (dr == NULL)
      printf("ERRO" );
  else
    while ((de = readdir(dr)) != NULL){
      fds[0].fd = socket;
      fds[0].events = 0;
      fds[0].events |= POLLIN;
      enter++;
      printf("\n");
      printf("%s\n", de->d_name);
      tam = strlen(de->d_name);
      printf( "TAMANHO DO DADO %d\n",tam );
      if (tam > 15) {
        cont = split_string(de->d_name,cont, tam, socket);
      } else {
        p1 = protocolo_server(de->d_name, 11, tam, cont);
        ultima = protocolo_string(p1);
        printf("STRING MENOR QUE 15 %s %d\n", ultima,tam);
        envia_protocolo(ultima, socket);
        time = poll(fds, 1, 6000);
        if (time == 0){
            printf("TIMEOUT!\n");
            break;
        }else {
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

        if (enter == 5) {
          p1 = protocolo_server("\n", 11, strlen("\n"), cont);
          enter = 0;
        } else {
          p1 = protocolo_server("  ", 11, strlen("  "), cont);
        }
        ultima = protocolo_string(p1);
        printf( "%s\n", ultima);
        envia_protocolo(ultima, socket);
        ultima = recebe_protocolo(socket);
      }
    }
    p1 = protocolo_server("", 1101, 0, cont);
    string = protocolo_string(p1);
    envia_protocolo(string, socket);
    fds[0].fd = socket;
    fds[0].events = 0;
    fds[0].events |= POLLIN;
    time = poll(fds, 1, 6000);
    if (time == 0){
        printf("TIMEOUT!\n");
        return 0;
    }else {
      string = recebe_protocolo(socket);
      //printf( "chegando %s\n",string);
      p2 = abre_protocolo(string);
      cont++;

      if (p2 -> tipo == 8) {
        printf("ACK DO CLIENTE\n");
      }else if (p2->tipo == 9) {
        envia_protocolo(string, socket);
      }
    }
    printf("ACABOU A TRANSMISSAO\n");

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
      printf("%d\n", p-> tipo);
      switch (p -> tipo) {
        case 0:       //CD
          server_CD(p, socket_confirmado);
          break;
        case 1:       //LS
          server_LS(p, socket_confirmado);
          break;
        case 2:      //VER
          server_VER(p, socket_confirmado);
          break;
        case 3:      // LINHA
          server_LINHA(p, socket_confirmado);
          break;
        case 4:     //LINHAS
          server_LINHAS(p, socket_confirmado);
          break;
        case 5:     //EDIT
        server_EDIT(p, socket_confirmado);

          break;
        default:
          printf("ERRO NO TIPO \n");
      }
    }
  }
}
