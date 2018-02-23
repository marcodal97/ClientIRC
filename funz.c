#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "def.h"




void creaNICK(char *nick, char *comando){  //COMPONE COMANDO NICK (comando è la stringa finale che viene mandata al client)

memset(comando, '\0', MAX_BUF);

strcpy(comando, "NICK ");

strcat(comando, nick]);

comando[strlen(comando)+1] = '\0';
comando[strlen(comando)] = '\n';

}



void creaUSER(char *nome, char *comando){ //COMPONE COMANDO USER (comando è la stringa finale che viene mandata al client)


memset(comando, '\0', MAX_BUF);

strcpy(comando, "USER ");

strcat(comando, "guest guest guest :");
strcat(comando, nome);

comando[strlen(comando)+1] = '\0';
comando[strlen(comando)] = '\n';

}




int PONG(int sock){ //Quando arriva ping dal server si manda questa funzione in esecuzione

int ris;

str[MAX_BUF];

strcpy(str, "PONG");

str[strlen(str)] = '\n';
str[strlen(str)+1] = '\0';

ris = send(sock, str, strlen(str), 0);

return ris;
}







