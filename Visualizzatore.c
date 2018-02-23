#include <sys/ipc.h>
#include <sys/msg.h>
#include "def.h"
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


int msg_id;



int main(){

pthread_t threadW, threadR;
tipo_coda coda;

msg_id = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
if(msg_id < 0){
printf("Errore msgget\n");
exit(1);
}





strcpy(coda.server, "irc.freenode.net");
coda.porta = 6667;
coda.m_type = 3;



printf("Invio dati\n%s\n%d", coda.server, coda.porta);

if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
perror("Errore msgsnd\n");
exit(1);
}


printf("Dati inviati\n");

msgrcv(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 1, 0);

if(strcmp(coda.msg, "ok") != 0){
	perror("Errore connessione\n");
	exit(1);
}

printf("Connessione riuscita\n");


//creo thread di scrittura in client e lettura da client


login(msg_id);


pthread_create(&threadW, NULL, threadVW, NULL);

pthread_create(&threadR, NULL, threadVR, NULL);


pause();

}
     






void *threadVW(void *arg){

	int scelta;
	tipo_coda coda;

	

		while(1){
		
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
	
	
		}	

}




void *threadVR(void *arg){

tipo_coda coda;


	while(1){
	
	memset(coda.msg, '\0', MAX_BUF);
		
		if(msgrcv(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 1, 0) < 0){
			perror("Errore msgrcv\n");
			exit(1);
		}	
	
	printf("%s", coda.msg);

	}

}





void login(int msg_id){

char nick[50];
char user[50];
int scelta;

tipo_coda coda;

strcpy(nick, "NICK marcodal97\n");
strcpy(user, "USER marco marco marco marco\n");


do{
	printf("************Menu'************\n");
	printf("1. Cambia file di config\n");
	printf("2. Usa il file di config presente\n");
	
	scanf("%d", &scelta);
	
		if(scelta == 1){
			
			memset(nick, '\0', 50);
			memset(user, '\0', 50);
		
			printf("Inserisci NICK: ");
			scanf(" %[^\n]s", nick);
			printf("Inserisci USER: ");
			scanf(" %[^\n]s", user);	
			
			nick[strlen(nick)] = '\n';		
			user[strlen(user)] ='\n';
			
			printf("%s%s", nick, user);	
		}
	}while(scelta == 1);
	
	
	printf("Invio dati login a client\n");
	
	coda.m_type = 3;
	strcpy(coda.nick, nick);
	strcpy(coda.user, user);
	
	if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
		perror("Errore msgsnd\n");
		exit(1);
	}
	
	printf("Inviati nick e user\n");

}






