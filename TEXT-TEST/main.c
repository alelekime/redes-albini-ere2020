#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(int argc, char const *argv[]) {
  for (int i = 0; i < argc; i++) {
    printf("%d = %s\n",i,argv[i]);
  }
}

int main(int argc, char const *argv[]) {
  system ("clear");
  FILE *fp;
  int linha = 2;
  char *caracter = (char*)malloc(sizeof(char) );
  print(argc,argv);

  fp = fopen (argv[1],"r");
  if (fp == NULL) {
    printf("ERRO NO ARQUIVO\n");
  }else{
    printf("1. ");
    while (fread(caracter, sizeof(char), 1,fp)) {
      if (!strcmp(caracter, "\n")) {
          printf("%s%d",caracter,linha );
          linha++;
      }else{
        printf("%s",caracter);
      }
    }
  }

  return 0;
}
