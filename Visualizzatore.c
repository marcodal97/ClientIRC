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

	do{
		do{
		scelta=menu();
		}while(scelta > 9);
		
		switch(scelta){
			case 1:		
				memset(coda.msg, '\0', MAX_BUF);
				printf("Che nick vuoi utilizzare? ");
				scanf("%s", coda.msg);
				irc_nick(coda.msg);
				toclient(coda.msg);
				break;
			case 2:
				memset(coda.msg, '\0', MAX_BUF);
				memset(nome, '\0', MAX_BUF);
				printf("Inserisci il nome della stanza in cui vuoi entrare: ");
				scanf("%s", coda.msg);
				strcpy(nome, coda.msg);
				irc_join(coda.msg);	
				toclient(coda.msg);			
				funzione_stanza(nome);
				break;				
			case 3:
				memset(coda.msg, '\0', MAX_BUF);
				memset(nome, '\0', MAX_BUF);
				printf("Inserisci il nome della stanza che vuoi lasciare: ");
				scanf("%s", coda.msg);
				strcpy(nome, coda.msg);
				irc_part(coda.msg);
				toclient(coda.msg);
				printf("\n-----Hai lasciato la stanza \"%s\"-----\n", nome);
				break;
			case 5:
				memset(coda.msg, '\0', MAX_BUF);
				memset(nome, '\0', MAX_BUF);
				printf("Inserisci il nickname: ");
				scanf("%s", coda.msg);
				strcpy(nome, coda.msg);
				funzione_chatnick(nome);				
				break;
			case 7:
				memset(coda.msg, '\0', MAX_BUF);
				printf("Inserisci il nickname: ");
				scanf("%s", coda.msg);								
				printf("\n------ Per tornare al menu' principale premere un tasto qualsiasi ------\n");
				printf("------ Informazioni su \"%s\" ------\n", coda.msg);
				irc_whois(coda.msg);
				toclient(coda.msg);
				getchar();
				getchar();
				break;			
			case 8:
				QUIT();
				break;
				
			case 9:
				irc_free();
				
				break;
				
		}	
	}while(1);

}




void *threadVR(void *arg){

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










