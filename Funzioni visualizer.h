#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
//#include "def.h"



/*PONG: di tanto in tanto il server pinga il client che deve rispondere con questo comando, altrimenti la connessione cade
NICK: imposta il nickname univoco
JOIN: joina una stanza
PART: lasciare il canale
QUIT: per quittare IRC
TOPIC: settare o rimuovere un topic
MSG: inviare un messaggio ad una stanza o un utente
WHO
WHOIS*/

int connection_menu(){
	int scelta;
	
	printf("\n\nBenvenuto nel miglior client IRC al mondo.\n\n");//scherzo
	printf("\t\tConnection menu'\n");
	printf("1-Modifica il file di configurazione (server, porta, nickname e username).\n");
	printf("2-Connettiti al server.\n");
	printf("Inserisci la tua scelta: ");
	fflush(stdin);
	scanf("%d", &scelta);

	return scelta;
}
tipo_coda coda;

void mod_config(){
	FILE * fp;
	char nome_server[MAX_BUF];
	char porta[5];
	char text[MAX_BUF];
	char *token;
	
	
	fp=fopen("configurazione.txt", "w");
	if(fp==NULL){
		printf("Impossibile aprire il file.\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Inserire il nome del server: ");
	memset(nome_server, '\0', MAX_BUF);
	getchar();
	while(fgets(nome_server, 50, stdin)==NULL);	
	//scanf("%s", nome_server);
	//printf("%ld\n", sizeof(nome_server));
	//nome_server[strlen(nome_server)]='\0';
	printf("Inserire il nome della porta: ");
	//scanf("%s", porta);
	while(fgets(porta, 50, stdin)==NULL);	
	printf("Inserire il nickname: ");
	while(fgets(nick, MAX_BUF-1, stdin)==NULL);
	printf("Inserisci l'username: ");
	while(fgets(user, MAX_BUF-1, stdin)==NULL);
	
	strcpy(coda.server, nome_server);
	strcpy(coda.porta, porta);
	strcpy(nickname, "NICK ");	
	strcat(nickname, nick);
	strcpy(username, "USER ");
	strcat(username, "guest guest guest :");
	strcat(username, user);
	strcpy(text, nome_server);
	strcat(text, "\n");
	strcat(text, porta);
	strcat(text, nickname);
	strcat(text, username);
	if(fputs(text, fp)==0){
		perror("Impossibile to write in the file.\n");
		exit(EXIT_FAILURE);
	}
	fclose(fp);
	
	printf("File di configurazione modificato.\n");
}
tipo_coda coda;

char irc_reg(){
	FILE *fp;
	char text[MAX_BUF], output[MAX_BUF];
	char ch;
	int flag;
	
	
	fp=fopen("configurazione.txt", "r");
	if(fp==NULL){
		perror("Impossibile aprire il file.\n");
		exit(EXIT_FAILURE);
	}
	while(fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]", coda.server, coda.porta, coda.nick, coda.user)>0);
	
	
	strcpy(output, coda.server);
	strcat(output, "\n");
	strcat(output, coda.porta);
	strcat(output, "\n");
	strcat(output, coda.nick);
	strcat(output, "\n");
	strcat(output, coda.user);
	
	//printf("%s %s %s %s", coda.server, coda.porta, coda.nick, coda.user);
	getchar();
	printf("\n\nIl file di configurazione dice: \n\n%s", output);
	printf("\nConfermare il file?(S=si, N=no)\n");
	ch=getchar();
	if(ch=='S'){		
		coda.m_type=3;
		strcpy(coda.msg, output);
		if(msgsnd(msg_id, (void *)&coda, sizeof(tipo_coda), 0)==-1){
			perror("Impossible to send a message.\n");
			exit(EXIT_FAILURE);
		}
		
	}
	
	return ch;
}

void irc_nick(char *comando){  //COMPONE COMANDO NICK (comando è la stringa finale che viene mandata al client)
	char str[MAX_BUF];	
	strcpy(str, comando);
	memset(comando, '\0', MAX_BUF);
	strcpy(comando, "NICK ");
	strcat(comando, str);
	comando[strlen(comando)+1] = '\0';
	comando[strlen(comando)] = '\n';
	
}

void irc_join(char *comando){  //Crea comando per entrare in una stanza
	
	char str[MAX_BUF];	
	strcpy(str, comando);
	memset(comando, '\0', MAX_BUF);
	strcpy(comando, "JOIN ");
	strcat(comando, str);
	comando[strlen(comando)] = '\n';
	comando[strlen(comando)+1] = '\0';
	
}

void irc_part(char *comando){  //Crea comando per lasciare stanza

	char str[MAX_BUF];	
	strcpy(str, comando);
	memset(comando, '\0', MAX_BUF);
	strcpy(comando, "PART ");
	strcat(comando, str);
	comando[strlen(comando)] = '\n';
	comando[strlen(comando)+1] = '\0';
}

void irc_msg(char *stanza, char *msg, char *comando){  //dato un messaggio e un canale/utente crea il comando per mandare il messaggi
	
	strcat(comando, stanza);
	strcat(comando, " : ");
	strcat(comando, msg);
	comando[strlen(comando)]='\n';
	comando[strlen(comando)+1]='\0';
	strcpy(msg, comando);
	/*strcpy(stanza, comando);
	memset(comando, '\0', MAX_BUF);
	strcpy(comando, "PRIVMSG ");
	strcat(comando, stanza);
	strcat(comando, " :");
	strcat(comando, msg);
	comando[strlen(comando)]='\n';
	comando[strlen(comando)+1]='\0';
	strcpy(msg, comando);*/
	//memset(comando, '\0', MAX_BUF);
	
}

void funzione_stanza(char *comando){
	int scelta;
	char messaggio[MAX_BUF];
	while(1){
		
		printf("Per tornare indietro scrivere \"i\".\n");
		memset(coda.msg, '\0', MAX_BUF);
		getchar();
		//while(fgets(coda.msg,MAX_BUF-1,stdin)==NULL); 
		scanf(" %[^\n]", coda.msg);
		if(coda.msg[0]!='i'){
			coda.msg[strlen(coda.msg)]='\0';
			coda.m_type = 3;
			strcpy(messaggio, "PRIVMSG ");	
			irc_msg(comando, coda.msg, messaggio);//coda.msg=ciao
			printf("Messaggio finale: %s", coda.msg);	
			if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
				perror("Errore msgsnd\n");
				exit(1);
			}		
			if(strcmp(coda.msg, "QUIT\n") == 0){
				printf("Uscita...\n");
				exit(0);
			}
		}else{
			return;
		}
	}
	



}


void irc_free(){
	char ch;
	while(1){
		
		printf("Per tornare indietro scrivere \"i\".\n");		
		while(fgets(coda.msg,MAX_BUF-1,stdin)==NULL);  //aspetto che si scriva qualcosa e lo mando al client
		if(coda.msg[0]!='i'){
			coda.m_type = 3;				
			if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
				perror("Errore msgsnd\n");
				exit(1);
			}		
			if(strcmp(coda.msg, "QUIT\n") == 0){
				printf("Uscita...\n");
				exit(0);
			}
		}else{
			return;
		}
	}
}



int menu(){
	int scelta;
	
	printf("\t\tMenu'\n");
	printf("1-Cambia il tuo nick.\n");
	printf("2-Entra all'intero di una stanza.\n");
	printf("3-Lascia una stanza.\n");
	printf("4-Setta o rimuovi un nuovo topic.\n");
	printf("5-Invia un messaggio privato.\n");
	printf("6-Invia un messaggio in un canale.\n");
	printf("7-Cerca utente.\n");
	printf("8-Informazioni su un utente.\n");
	printf("9-Esci da IRC e dal client.\n");
	printf("10-Inserisci da tastiera il comando.\n");
	printf("Inserisci la tua scelta: ");
	scanf("%d", &scelta);
	
	return scelta;
}



/*int irc_pong(int s, const char *pong){
	//:WiZ NICK Kilroy
	
	


}

int irc_join(int s, const char *channel);
int irc_part(int s, const char *data);
int irc_nick(int s, const char *nick);
int irc_quit(int s, const char *quit_msg);
int irc_topic(int s, const char *channel, const char *data);
int irc_action(int s, const char *channel, const char *data);
int irc_msg(int s, const char *channel, const char *data);*/


/*int main(void){
	int scelta;
	char ch;
	
	while(ch!='S'){
		scelta=connection_menu();
		switch(scelta){
			case 1:
				mod_config();
				break;
			case 2:
				ch=irc_reg();			
				break;	
		}
	}
	
	printf("Connessione stabilita.\n");
}*/
