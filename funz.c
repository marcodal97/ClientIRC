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

char str[MAX_BUF];

strcpy(str, "PONG");

str[strlen(str)] = '\n';
str[strlen(str)+1] = '\0';

ris = send(sock, str, strlen(str), 0);

return ris;
}




void JOIN(char *comando){  //Crea comando per entrare in una stanza

char stanza[MAX_BUX];

printf("Inserisci nome stanza: \n");
scanf("%s", stanza);

memset(comando, '\0', MAX_BUF);

strcpy(comando, "JOIN ");

strcat(comando, stanza);

comando[strlen(comando)] = '\n';
comando[strlen(comando)+1] = '\0';

}



void PART(char *comando){  //Crea comando per lasciare stanza

char stanza[MAX_BUX];

printf("Inserisci nome stanza: \n");
scanf("%s", stanza);

memset(comando, '\0', MAX_BUF);

strcpy(comando, "PART ");

strcat(comando, stanza);

comando[strlen(comando)] = '\n';
comando[strlen(comando)+1] = '\0';



}



void MSG(char *comando, char *msg, char *nome){  //dato un messaggio e un canale/utente crea il comando per mandare il messaggio

memset(comando, '\0', MAX_BUF);

strcpy(comando, "PRIVMSG ");

strcat(comando, nome);
strcat(comando, " :");
strcat(comando, msg);

comando[strlen(comando)] = '\n';
comando[strlen(comando)+1] = '\0';

}


void WHOIS(char *comando){  //Crea comando per avere informazioni su un utente

char nick[MAX_BUF];

printf("Inserisci nick\n");
scanf("%s", nick);

memset(comando, '\0', MAX_BUF);

strcpy(comando, "WHOIS ");

strcat(comando, nick);

comando[strlen(comando)] = '\n';
comando[strlen(comando)+1] = '\0';


}






