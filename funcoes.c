#include "funcoes.h"

//https://codeforwin.org/2018/03/c-program-find-file-properties-using-stat-function.html


estrutura_pacote *protocolo(char *dado,int tipo, int tam, int seq) {
  char *novo = (char*)malloc(sizeof(char) * 15);
  strncpy(novo, dado,(strlen(dado)-1)); // retira o \0 do final da string
  estrutura_pacote *p1 = (estrutura_pacote*)malloc(sizeof(estrutura_pacote));
  printf("here\n");
  p1 -> marcador = "01111110";
  p1 -> tamanho = convert(tam);
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
  p1 -> pariedade = cal_pariedade(tam, seq, tipo, dado);
  return p1;
}

estrutura_pacote *protocolo_server(char *dado, int tipo, int tam, int seq) {
  estrutura_pacote *p1 = (estrutura_pacote*)malloc(sizeof(estrutura_pacote));
  p1 -> marcador = "01111110";
  p1 -> tamanho = convert(tam);
  p1 -> endereco_origem = "01";
  p1 -> endereco_destino ="10";
  p1 -> sequencia = seq;
  p1 -> tipo = convert(tipo);
  p1-> dados = dado;
  p1 -> pariedade = cal_pariedade(tam, seq, tipo, dado);
//  mostra_protocolo(p1);

  return p1;
}

char *protocolo_string(estrutura_pacote * p1) {
  char *string = (char*)malloc( sizeof(char)* 256);
  snprintf(string,256,"%s%s%s%s%s%s%s%s", p1 -> marcador, int2bin(p1 -> tamanho, 4), p1 -> endereco_origem, p1 -> endereco_destino, int2bin(p1-> sequencia, 4), int2bin(p1-> tipo, 4), p1 -> dados, int2bin(p1 -> pariedade, 8));
  //printf("%s\n",string );
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
  p -> tamanho = convert(soma);
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
  p-> dados = pont;
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

void client_CD(linha_comando *entrada, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote *p1 = protocolo(entrada -> diretorio, CD, strlen(entrada->diretorio), 0);
  string = protocolo_string(p1);
  envia_protocolo(string, socket);
  while (1){
    string = recebe_protocolo(socket);
    if (strlen(string) > 5) {
      p1 = abre_protocolo(string);
      printf("%s\n",string);
      if(!strcmp(p1-> endereco_origem, "01"))
        break;
    }
  }
  if (p1 -> tipo == 8) {
    printf("\nACK\n");
  }else if (p1 -> tipo == 9) {
    printf("\nNACK\n");
  } else if (p1 -> tipo == 7) {
    printf("\nERRO ENCONTRADO\n");
    printf("%s\n",p1-> dados);
  }
}

bool recebe_ls(estrutura_pacote * p1, int socket) {
  char *string = (char*)malloc( sizeof(char)* 256);
  estrutura_pacote * p;
  string = recebe_protocolo(socket);

  if (strlen(string) > 5){
    p1 = abre_protocolo(string);

    if (p1-> tipo == 11) {
      //printf("%s\n",string);
      printf("%s\n", p1->dados);
      p = protocolo_server("", ACK, strlen(""), 0);
      //printf("%s\n",string);
      string = protocolo_string(p);
      //printf("ENVIANDO\n" );
      envia_protocolo(string, socket);
      return true;

    }else if (p1-> tipo == 13){
      printf("Fim da transmissao\n");
      return false;
    }
  } else {
        printf("Erro não é conteudo do ls \n");
        return false;
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
   //client_VER(); //mostra o arquivo com as linhas numeradas
  }else if(!strcmp(quebra, "linha")){
    entrada -> linha =  atoi(strdup(strtok(NULL, " ")));//<numero_linha>
    quebra =  strdup(strtok(NULL, ""));//<nome_arq>
    entrada-> nome_arq = quebra;
    //client_LINHA(); // mostra a linha escolida do arquivo
  }
  else if(!strcmp(quebra, "linhas")){
    entrada -> linha_inicial =  atoi(strdup(strtok(NULL, " ")));//<numero_linha_inicial>
    entrada -> linha_final =  atoi(strdup(strtok(NULL, " ")));//<numero_linha_final>
    quebra =  strdup(strtok(NULL, ""));//<nome_arq>
    entrada-> nome_arq = quebra;
    //client_LINHAS();//mostra as linhas escolidas do arquivo
  }else if (!strcmp(quebra, "edit")){
    entrada -> linha =  atoi(strdup(strtok(NULL, " "))); //<numero_linha>
    quebra =  strdup(strtok(NULL, " "));//<nome_arq>
    entrada-> nome_arq = quebra;
    quebra =  strdup(strtok(NULL, ""));//“<NOVO_TEXTO>”
    entrada->dados = quebra;
    //client_EDIT();// troca a linha escolhida por um texto
  }

}
