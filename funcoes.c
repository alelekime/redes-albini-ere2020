#include "funcoes.h"

void lista_comandos_ajuda() {
  printf("-----------------------------------------------------------\n");
  printf(" * cd <nome_dir>\n");
  printf(" * lcd <nome_dir> \n");
  printf(" * ls \n");
  printf(" * lls \n");
  printf(" * ver <nome_arq> \n");
  printf(" * ver <nome_arq> \n");
  printf(" * linha <numero_linha> <nome_arq>\n");
  printf(" * linhas <numero_linha_inicial> <numero_linha_final> <nome_arq> \n");
  printf(" * edit <numero_linha> <nome_arq> “<NOVO_TEXTO>”  \n");
  printf("-----------------------------------------------------------\n");
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
  printf("smms,s\n" );
  system ("ls");
}
void imprime_path() {
  char* diretorio_atual;
  char endereco[PATH_MAX +1];
  diretorio_atual = getcwd(endereco, PATH_MAX + 1 );
  printf( "%s$", diretorio_atual );
}


void le_comando( linha_comando *entrada) {
  char *entrada_buffer, *dados, *quebra, *nome ;
  __fpurge (stdin); // limpa buffer
  fgets (entrada_buffer, ENTRADA_MAX, stdin);// recebe a linha de comdo do usuário
  dados = strdup (entrada_buffer); //aloca memoria do tamanho da linha de comando mais \0
  quebra = strdup(strtok(dados," "));
  printf("%s\n", entrada_buffer);
  printf("%d\n",strlen(entrada_buffer) );
  nome = (char*)malloc(sizeof(char));
  // strncpy(dados, entrada_buffer,(strlen(entrada_buffer)-1));


  entrada->comando = quebra;
  strncpy(dados, entrada_buffer,(strlen(entrada_buffer)-2));
  printf("%s\n", dados);
  printf("%d\n",strlen(dados) );
  if(!strcmp(entrada->comando, "cd")){
     quebra =  strdup(strtok(NULL, ""));//<nome_dir>
     entrada-> diretorio = quebra;
     //funcaoCD(entrada);   //efetua a troca de diretório - servidor
  }else if(!strcmp(quebra, "lcd")){
     quebra =  strdup(strtok(NULL, ""));//<nome_dir>
     entrada-> diretorio = quebra;
     printf("%s\n", entrada-> diretorio);
     funcaoLCD(entrada);   //efetua a troca de diretório - cliente
  }else if(!strcmp(dados, "ls")){
    printf("msdmsdds\n" );
     //funcaoLS(); //lista arquivos - servidor
  }else if(!strcmp(dados, "lls")){
     quebra =  strdup(strtok(NULL, ""));//<nome_dir>
    printf("asdas\n");
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
  }else if(!strcmp(quebra, "linhas")){
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
