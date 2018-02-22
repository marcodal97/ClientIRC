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

msg_id = msgget((key_t)MSGKEY, 0666 | IPC_CREAT);
if(msg_id < 0){
printf("Errore msgget\n");
exit(1);
}


strcpy(coda.server, "irc.freenode.net");
coda.porta = 6667;
coda.m_type = 3;



printf("Invio dati\n");

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

pthread_create(&threadW, NULL, threadVW, NULL);

pthread_create(&threadR, NULL, threadVR, NULL);


pause();

}
     






void *threadVW(void *arg){

	int scelta;
	tipo_coda coda;
/*
	do{
	printf("****Menu'****\n");
	printf("1. Cambia file di config\n");
	printf("2. Usa il file di config presente\n");
	
	scanf("%d", &scelta);
	
	
	
	}while(scelta == 1);
*/

	strcpy(coda.nick, "NICK marcodal97\n");                  //invio nick e user a client
	strcpy(coda.user, "USER marco marco marco marco\n");
	
	coda.m_type = 3;
	if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
	perror("Errore msgsnd\n");
	exit(1);
	}
	
		printf("******MENU'******\n");
		
	

		while(1){
		
			while(fgets(coda.msg,MAX_BUF-1,stdin)==NULL);    //aspetto che si scriva qualcosa e lo mando al client
			coda.m_type = 3;
					
			if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
				perror("Errore msgsnd\n");
				exit(1);
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



