#include "funcoes.h"

int main(int argc, char const *argv[]) {
  int socket_confirmado;

  linha_comando entrada;
  system ("clear");


  lista_comandos_ajuda();
  if (argc == 1) {
    printf("nada\n");
      //lista_comandos_ajuda();
  }else if (!strcmp( argv[1], "cliente")) {
    socket_confirmado = ConexaoRawSocket();
    printf("MODO CLIENTE ..\n");
    while(1){
      imprime_path();
      le_comando(&entrada);
}

  }else if (!strcmp( argv[1], "servidor")){
    socket_confirmado = ConexaoRawSocket();
    printf("MODO SERVIDOR ..\n");

  }

}
