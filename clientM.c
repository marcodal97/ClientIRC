#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include "def.h"

#define MSGKEY 6666

typedef struct{
char msg[MAX_BUF];
}tipo_dato

typedef struct{
long int type; //1 visual, 2 executer, 3 client
tipo_dato mess;
}tipo_coda;


#define MSGKEY 6666

int main(int argc, char*argv[]){

int sock, msg_id;

msg_id = msgget((key_t)MSGKEY, 0666 | IPC_CREAT);
if(msg_id < 0){
printf("Errore msgget\n");
exit(1);
}
     


struct sockaddr_in server;
struct hostent *hp, *gethostbyname();
pthread_t threadW, threadR;

sock = socket(AF_INET, SOCK_STREAM, 0);
if(sock < 0){
perror("Errore socket\n");
exit(1);
}

hp = gethostbyname(argv[1]);

server.sin_family = AF_INET;
server.sin_port = htons(atoi(argv[2]));
bcopy((char*)hp->h_addr, (char*)&server.sin_addr, hp->h_length);

if(connect(sock, (struct sockaddr*)&server, sizeof(server))< 0){
perror("Errore connect\n");
exit(1);
}

printf("Connessione al Server riuscita\n\n");

pthreadcreate(&threadW, NULL, threadFW, (*void)sock);
pthreadcreate(&threadR, NULL, threadFR, (*void)sock);

pause();
close(sock);
}




void *threadFW(void *arg){

int sockid = *(int*)arg;
tipo_coda dato;

	while(1){
	
	
		memset(dato.msg, '\0', MAX_BUF);

		if(msgrcv(msg_id, &dato, sizeof(tipo_dato), 1, 0) < 0){
		perror("Errore msgrcv\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);
		}
		
		

		if(send(sockid, dato.msg, strlen(dato.msg, 0)<0){
		perror("Errore send\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);		
		}
		
		if(strcmp(dato.msg, QUIT) == 0){
		printf("Chiudo connessione\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(0);
		}

	}

}

void *threadFR(void* arg){

int sockid = *(int*)arg;
tipo_coda dato;


	while(1){
	
	memset(dato.msg, '\0', MAX_BUF);
	
	if(recv(sockid, &dato.msg, MAX_BUF, 0) < 0){
	printf("Errore recv\n");
	msgctl(msg_id, IPC_RMID, 0);
	exit(1);	
	}
	dato.tipo = 2; //invio a visualizzatore il messaggio letto dal server
	msgsnd(msg_id, &dato, sizeof(tipo_dato), 0);	


	}


}



