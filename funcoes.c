#include "funcoes.h"


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
  p1 -> marcador = "01111110";
  p1 -> tamanho = acha_binario(tam);
  p1 -> endereco_origem = "01";
  p1 -> endereco_destino ="10";
  p1 -> sequencia = seq;
  p1 -> tipo = tipo;
  p1-> dados = dado;
  p1 -> pariedade = cal_pariedade(tam, seq, tipo, dado, p1-> endereco_origem, p1->endereco_destino);

  return p1;
}

estrutura_pacote *protocolo_client(char *dado, int tipo, int tam, int seq) {
  estrutura_pacote *p1 = (estrutura_pacote*)malloc(sizeof(estrutura_pacote));

  p1 -> marcador = "01111110";
  p1 -> tamanho = acha_binario(tam);
  p1 -> endereco_origem = "10";
  p1 -> endereco_destino ="01";
  p1 -> sequencia = seq;
  p1 -> tipo = tipo;
  p1-> dados = dado;
  p1 -> pariedade = cal_pariedade(tam, seq, tipo, dado, p1-> endereco_origem, p1->endereco_destino);

  return p1;
}

char *protocolo_string(estrutura_pacote * p1) {
  char *string = (char*)malloc( sizeof(char)* 256);
  char *binary = (char*)malloc( sizeof(char)* 4);

  snprintf(string,256,"%s%s%s%s%s%s%s%s", p1 -> marcador,  p1 -> endereco_origem, p1 -> endereco_destino, convert_binary(p1 -> tamanho, binary), int2bin(p1-> sequencia, 4), int2bin(p1-> tipo, 4), p1 -> dados, int2bin(p1 -> pariedade, 8));
  return string;
}

int envia_protocolo(char *string, int socket) {
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

  soma = 0;
  for (int i = 0; i < p-> tamanho; i++) {
    *dados1 = *entrada_server;
    *dados1++;
    *entrada_server++;
  }
  p-> dados = strdup(pont);

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
