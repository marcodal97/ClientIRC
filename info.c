//Creazione pass, nick, user

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_TEXT 1024


int main(void){
	char nickname[MAX_TEXT];
	char password[MAX_TEXT];
	char username[MAX_TEXT];
	char nick[MAX_TEXT*2];
	char user [MAX_TEXT*2];
	char pass[MAX_TEXT*2];
	
	strcpy(nick, "NICK: ");
	strcpy(pass, "PASS: ");
	printf("Inserire la password per collegarsi al server: ");
	fgets(password, MAX_TEXT-1, stdin);
	password[strlen(password)-1]='\0';
	strcat(pass, password);
	printf("Inserire il nickname che si vuole utilizzare: ");
	fgets(nickname, MAX_TEXT, stdin);
	nickname[strlen(nickname)-1]='\0';
	strcat(nick, nickname);
	//printf("Inserire l'username: ")
	
	printf("Pass: %s, Nick: %s.\n", pass, nick);

}
