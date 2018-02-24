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


void *threadVW(void *arg);
void *threadVR(void *arg);
void login();

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
	printf("Connessione riuscita\n");

	//creo thread di scrittura in client e lettura da client
	
	pthread_create(&threadW, NULL, threadVW, NULL);
	pthread_create(&threadR, NULL, threadVR, NULL);

	pause();

}

void toclient(char *commando){
	strcpy(coda.msg, commando);
	printf("%s", coda.msg);
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
	char stanza[MAX_BUF];
	
	do{
		scelta=menu();
		switch(scelta){
			case 1:			
				printf("Che nick vuoi utilizzare? ");
				scanf("%s", coda.msg);
				irc_nick(coda.msg);
				toclient(coda.msg);
				break;
			case 2:
				printf("Inserisci il nome della stanza: ");
				scanf("%s", coda.msg);
				strcpy(stanza, coda.msg);
				irc_join(coda.msg);	
				toclient(coda.msg);			
				funzione_stanza(stanza);
				break;
			case 3:
				printf("Inserisci la stanza che vuoi lasciare: ");
				scanf("%s", coda.msg);
				irc_part(coda.msg);
				toclient(coda.msg);
			case 4:
				break;
			case 5:
				
			case 10:
				irc_free();
				break;
				
		}	
	}while(scelta!=9);

	/*while(1){
	
			
		while(fgets(coda.msg,MAX_BUF-1,stdin)==NULL);  //aspetto che si scriva qualcosa e lo mando al client
		coda.m_type = 3;
		
		
		
				
		if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
			perror("Errore msgsnd\n");
			exit(1);
		}	
		
		
		if(strcmp(coda.msg, "QUIT\n") == 0){
			printf("Uscita...\n");
			exit(0);
		}


	}*/	

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











