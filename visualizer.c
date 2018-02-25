#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include "def.h"
#include "Funzioni visualizer.h"



//int msg_id;



int main(){
	pthread_t threadW, threadR;
	tipo_coda coda;
	int scelta;
	char ch;

	msg_id = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
	if(msg_id < 0){
		printf("Errore msgget\n");
		exit(1);
	}
	
	coda.m_type = 3;
	
	
	//File di configurazione
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
	
	printf("Dati inviati\n");

	if(msgrcv(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 1, 0)<0){
		perror("Impossible receive.\n");
		exit(EXIT_FAILURE);
	}	
	if(strncmp(coda.msg, "ok", 2) != 0){
		perror("Errore connessione\n");
		exit(EXIT_FAILURE);
	}
	printf("Connessione riuscita\n\n\n");

	//creo thread di scrittura in client e lettura da client
	
	pthread_create(&threadR, NULL, threadVR, NULL);
	pthread_create(&threadW, NULL, threadVW, NULL);
	

	pause();

}

void toclient(char *comando){

	tipo_coda coda;
	strcpy(coda.msg, comando);
	coda.m_type = 3;
	if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
			perror("Errore msgsnd\n");
			exit(1);
		}			
	if(strcmp(coda.msg, "QUIT\n") == 0){
			printf("Uscita...\n");
			exit(0);
	}
		
}


void *threadVW(void *arg){

	int scelta;
	tipo_coda coda;	
	char nome[MAX_BUF];
	char stanza[MAX_BUF];
	

	do{
		scelta=menu();
	
		memset(coda.msg, '\0', MAX_BUF);
		memset(nome, '\0', MAX_BUF);
		memset(stanza, '\0', MAX_BUF);
		
		switch(scelta){
			case 1:		
				printf("Che nick vuoi utilizzare? ");
				scanf("%s", coda.msg);
				irc_nick(coda.msg);
				toclient(coda.msg);
				break;
			case 2:
				printf("Inserisci il nome della stanza in cui vuoi entrare: ");
				scanf("%s", coda.msg);
				strcpy(nome, coda.msg);
				irc_join(coda.msg);	
				toclient(coda.msg);			
				funzione_stanza(nome);
				break;				
			case 3:
				printf("Inserisci il nome della stanza che vuoi lasciare: ");
				scanf("%s", coda.msg);
				strcpy(nome, coda.msg);
				irc_part(coda.msg);
				toclient(coda.msg);
				printf("\n-----Hai lasciato la stanza \"%s\"-----\n", nome);
				break;
			case 4:
				printf("Inserisci il nome della stanza: ");
				scanf("%s", stanza);
				getchar();
				printf("Inserisci il topic da inserire: ");
				fgets(nome, MAX_BUF, stdin);
				strcpy(coda.msg, nome);
				irc_topic(stanza, coda.msg);
				toclient(coda.msg);
				printf("Topic mandato.\n");
				break;
			case 5:		
				printf("Inserisci il nickname: ");
				scanf("%s", coda.msg);
				strcpy(nome, coda.msg);
				funzione_chatnick(nome);				
				break;
			case 6:
				printf("Inserire il tipo di utenti da cercare: ");
				scanf("%s", nome);
				strcpy(coda.msg, nome);
				irc_who(coda.msg);				
				toclient(coda.msg);
				break;				
			case 7:
				printf("Inserisci il nickname: ");
				scanf("%s", coda.msg);	
				getchar();							
				printf("\n------ Per tornare al menu' principale premere un tasto qualsiasi ------\n");
				printf("------ Informazioni su \"%s\" ------\n", coda.msg);
				irc_whois(coda.msg);
				toclient(coda.msg);
				getchar();
				break;			
			case 8:
				QUIT();
				break;				
			case 9:
				irc_free();				
				break;
			case 10:
				printf("Inserisci la stanza: ");
				scanf("%s", stanza);
				getchar();
				printf("Inserire il comando da inviare: ");
				fgets(nome, MAX_BUF, stdin);
				irc_command(stanza, nome);
				strcpy(coda.msg, nome);
				toclient(coda.msg);
				break;
			default:
				printf("Hai sbagliato a selezionare il numero.");
				break;
				
		}	
	}while(1);

}

void *threadVR(void *arg){
char ch;
tipo_coda coda;


	while(1){	
		memset(coda.msg, '\0', MAX_BUF);
		
		if(msgrcv(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 1, 0) < 0){
			perror("Errore msgrcv\n");
			exit(EXIT_FAILURE);
		}		
		printf("%s", coda.msg);

	}

}
