#include "funcoes.h"


estrutura_pacote *protocolo_cliente(char *dado,int tipo, int tam, int seq) {
  estrutura_pacote *p1 = (estrutura_pacote*)malloc(sizeof(estrutura_pacote));
  p1 -> marcador = "01111110";
  p1 -> tamanho = decimalToBinary(tam);
  p1 -> endereco_origem = "10";
  p1 -> endereco_destino ="01";
  p1 -> sequencia = seq;
  p1 -> tipo = tipo;
  p1 -> dados = dado;
  p1 -> pariedade = cal_pariedade(tam, seq, tipo, dado);
  //printf("%d\n",strlen(p1 -> marcador )+ tam +strlen( p1 -> endereco_origem) +  strlen(p1 -> endereco_destino) + seq + tipo + strlen( p1 -> dados )+ p1 ->pariedade );
  mostra_protocolo(p1);
  protocolo_string(p1, tipo,tam, seq);
  return p1;
}
char *protocolo_string(estrutura_pacote * p1, int tipo, int tam, int seq) {
  char *string = (char*)malloc( sizeof(char)* 256);
  snprintf(string,256,"%s %d %s %s %d %d %s %d",p1 -> marcador, tam, p1 -> endereco_origem, p1 -> endereco_destino, seq, tipo, p1 -> dados, p1 ->pariedade);
  printf("%s\n",string );
  return 0;
}
//
// int envia_protocolo(estrutura_pacote * p, int socket) {
//   if ((send (socket_confirmado, dados, 256, 0)) == -1) {
// 		perror("send");
//   return 0;
// }

void funcaoCD(linha_comando *entrada, int socket) {
  estrutura_pacote *p = protocolo_cliente(entrada -> diretorio, CD, strlen(entrada->diretorio), 0);

}

void funcaoLCD(linha_comando *entrada) {
  char *nome = malloc (strlen(entrada->diretorio));
  strncpy(nome, entrada->diretorio,(strlen(entrada->diretorio)-1)); // retira o \0 do final da string, dá problema na comaparação se nao tirar
  if (!strcmp(nome, "..")){
    chdir("..");
  }else
    chdir(nome);
}

void funcaoLLS() {
  system ("ls");
}
void imprime_path() {
  char* diretorio_atual;
  char endereco[PATH_MAX +1];
  diretorio_atual = getcwd(endereco, PATH_MAX + 1 );
  printf( "%s$", diretorio_atual );
}


void le_comando( linha_comando *entrada, int socket) {
  char *entrada_buffer, *dados, *quebra, *nome ;
  nome = (char*)malloc(sizeof(char));
  __fpurge (stdin); // limpa buffer
  fgets (entrada_buffer, ENTRADA_MAX, stdin);// recebe a linha de comdo do usuário
  dados = strdup (entrada_buffer); //aloca memoria do tamanho da linha de comando mais \0
  strncpy(nome, dados,(strlen(dados)-1)); // retira o \0 do final da string, dá problema na comaparação do ls e lls
  quebra = strdup(strtok(dados," "));
  entrada->comando = quebra;
  //printf("%s\n", entrada->comando );
  if(!strcmp(entrada->comando, "cd")){
     quebra =  strdup(strtok(NULL, ""));//<nome_dir>
     entrada-> diretorio = quebra;
     funcaoCD(entrada, socket);   //efetua a troca de diretório - servidor
  }else if(!strcmp(quebra, "lcd")){
     quebra =  strdup(strtok(NULL, ""));//<nome_dir>
     entrada-> diretorio = quebra;
     printf("%s\n", entrada-> diretorio);
     funcaoLCD(entrada);   //efetua a troca de diretório - cliente
  }else if(!strcmp(nome, "ls")){
     //funcaoLS(); //lista arquivos - servidor
  }else if(!strcmp(nome, "lls")){
    funcaoLLS(); //lista arquivos - cliente
  }else if(!strcmp(quebra, "ver")){
     quebra =  strdup(strtok(NULL, ""));//<nome_arq>
     entrada-> nome_arq = quebra;
   //funcaoVER(); //mostra o arquivo com as linhas numeradas
  }else if(!strcmp(quebra, "linha")){
    entrada -> linha =  atoi(strdup(strtok(NULL, " ")));//<numero_linha>
    quebra =  strdup(strtok(NULL, ""));//<nome_arq>
    entrada-> nome_arq = quebra;
    //funcaoLINHA(); // mostra a linha escolida do arquivo
  }
  else if(!strcmp(quebra, "linhas")){
    entrada -> linha_inicial =  atoi(strdup(strtok(NULL, " ")));//<numero_linha_inicial>
    entrada -> linha_final =  atoi(strdup(strtok(NULL, " ")));//<numero_linha_final>
    quebra =  strdup(strtok(NULL, ""));//<nome_arq>
    entrada-> nome_arq = quebra;
    //funcaoLINHAS();//mostra as linhas escolidas do arquivo
  }else if (!strcmp(quebra, "edit")){
    entrada -> linha =  atoi(strdup(strtok(NULL, " "))); //<numero_linha>
    quebra =  strdup(strtok(NULL, " "));//<nome_arq>
    entrada-> nome_arq = quebra;
    quebra =  strdup(strtok(NULL, ""));//“<NOVO_TEXTO>”
    entrada->dados = quebra;
    //funcaoEDIT();// troca a linha escolhida por um texto
  }

}
