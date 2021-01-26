#include "funcoes.h"

int main(int argc, char const *argv[]) {
  int socket_confirmado;
  char *entrada_buffer, *dados, *maybe;

  system ("clear");
  if (argc == 1) {
    printf("nada\n");
      //lista_comandos_ajuda();
  }else if (!strcmp( argv[1], "cliente")) {
    socket_confirmado = socket_c();
    printf("MODO CLIENTE ..\n");
    __fpurge (stdin); // limpa buffer
    fgets (entrada_buffer, 4096, stdin);// recebe a linha de comdo do usuário
    dados = strdup (entrada_buffer); //aloca memoria do tamanho da linha de comando mais \0
    if ((send (socket_confirmado, dados, 256, 0)) == -1) {
		perror("send");
	}

  }else if (!strcmp( argv[1], "servidor")){
    socket_confirmado = socket_c();
    printf("MODO SERVIDOR ..\n");
    maybe = (char*)malloc(sizeof(char)*256);

    if ((recv (socket_confirmado, maybe, 256, 0) == -1)) {
      perror("recv");
    }
    printf("%s\n",maybe );
  }

}
