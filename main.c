#include "funcoes.h"

int main(int argc, char const *argv[]) {
  int socket_confirmado;

  linha_comando entrada;
  system ("clear");



  if (argc == 1) {
    printf("nada\n");
      //lista_comandos_ajuda();
  }else if (!strcmp( argv[1], "cliente")) {
    lista_comandos_ajuda();
    socket_confirmado = ConexaoRawSocket();
    printf("MODO CLIENTE ..\n");
    while(1){
      imprime_path();
      le_comando(&entrada, socket_confirmado);
}

  }else if (!strcmp( argv[1], "servidor")){
    socket_confirmado = ConexaoRawSocket();
    printf("MODO SERVIDOR ..\n");
    funcoes_server(socket_confirmado);
  }

}
