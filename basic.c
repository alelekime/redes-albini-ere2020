#include "funcoes.h"

long decimalToBinary(int decimalnum){
    long binarynum = 0;
    int rem, temp = 1;
    while (decimalnum!=0)
    {
        rem = decimalnum%2;
        decimalnum = decimalnum / 2;
        binarynum = binarynum + rem*temp;
        temp = temp * 10;
    }
    return binarynum;
}

int cal_pariedade(int tam, int seq, int tipo, char *dado) {
  int par = 0;
  int binary;
  int soma = tam + seq + tipo;
  while (soma > 0) {
      par = par+ soma %10;
      soma = soma / 10;
  }
  int tamanho =  strlen(dado);
  for(int i = 0; i < tamanho; i++){
      par = par ^ *dado;
      *dado++;
  }
  binary = decimalToBinary(par);


  return binary;
}

void imprime_path() {
  char* diretorio_atual;
  char endereco[PATH_MAX +1];
  diretorio_atual = getcwd(endereco, PATH_MAX + 1 );
  printf( "%s$", diretorio_atual );
}

char * int2bin(unsigned i, int tam)
{
    size_t bits = tam;

    char * str = malloc(bits + 1);
    if(!str) return NULL;
    str[bits] = 0;

    // type punning because signed shift is implementation-defined
    unsigned u = *(unsigned *)&i;
    for(; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';

    return str;
}


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

void mostra_protocolo(estrutura_pacote *p1) {
  printf(" MARCADOR %s\n", p1 -> marcador);
  printf(" TAMANHO %d\n", p1 -> tamanho);
  printf(" ENDEREÇO DE ORIGEM %s\n", p1 -> endereco_origem);
  printf(" ENDEREÇO DE DESTINO %s\n", p1 -> endereco_destino);
  printf(" SEQUÊNCIA %d\n", p1 -> sequencia);
  switch (p1 -> tipo) {
    case 0:
    printf(" TIPO 0000\n");
    break;
    case 1:
    printf(" TIPO 0001\n");
    break;
    case 10:
    printf(" TIPO 0010\n");
    break;
    case 11:
    printf(" TIPO 0011\n");
    break;
    case 100:
    printf(" TIPO 0100\n");
    break;
    case 101:
    printf(" TIPO 0101\n");
    break;
  }
  printf(" DADO %s", p1 -> dados);
  printf(" PARIEDADE %d\n", p1 -> pariedade );
}
