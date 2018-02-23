#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <netdb.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "def.h"





int sock, msg_id;

int main(){

	msgctl(msg_id, IPC_RMID, 0);


	msg_id = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
		if(msg_id < 0){
			printf("Errore msgget\n");
			exit(1);
		}
     
	tipo_coda coda;


	struct sockaddr_in server;
	struct hostent *hp, *gethostbyname();


	pthread_t threadW, threadR;


//aspetto dati da visual

	printf("Aspetto dati da visual\n");

	if(msgrcv(msg_id, &coda, sizeof(tipo_coda)-sizeof(long int), 3, 0)<0){
		perror("Errore msgrcv\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);
	}

	printf("Dati ricevuti\n");

	printf("%s\n", coda.server);
	printf("%d\n", coda.porta);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("Errore socket\n");
		exit(1);
	}


	hp = gethostbyname(coda.server);
	printf("Provo connessione\n");
	server.sin_family = AF_INET;
	server.sin_port = htons(coda.porta);
	bcopy((char*)hp->h_addr, (char*)&server.sin_addr, hp->h_length);



	if(connect(sock, (struct sockaddr*)&server, sizeof(server))< 0){
		perror("Errore connect\n");
		coda.m_type = 1;
		strcpy(coda.msg, "no");                  //se la connessione non riesce si fa sapere al visual che si chiude
		msgsnd(msg_id, &coda, sizeof(tipo_coda)- sizeof(long int), 0);
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);
	}




	printf("Connessione stabilita\n");

//Mando conferma di connessione a visual

	coda.m_type = 1;
	strcpy(coda.msg, "ok");
	if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0) < 0){
		perror("Errore msgsnd\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);
	}

	printf("Conferma inviata\n");


	if(send(sock, coda.nick, strlen(coda.nick), 0)<0){
		perror("Errore send\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);		
	}




loginserv(sock, msg_id);


pthread_create(&threadR, NULL, threadFR, (void*)&sock);
pthread_create(&threadW, NULL, threadFW, (void*)&sock);


pause();

}




void *threadFW(void *arg){

int sockid = *(int*)arg;
tipo_coda coda;
		
	while(1){						//tutto quello mandato dal visualizzatore lo invio al server
	
		memset(coda.msg, '\0', MAX_BUF);

		if(msgrcv(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 3, 0) < 0){
			perror("Errore msgrcv\n");
			msgctl(msg_id, IPC_RMID, 0);
			exit(1);
		}
		
		printf("Ric da vis -- %s\n", coda.msg);
		
		
		
		if(send(sockid, coda.msg, strlen(coda.msg), 0)<0){
			perror("Errore send\n");
			msgctl(msg_id, IPC_RMID, 0);
			exit(1);		
		}
		
		printf("Inv a serv %s\n", coda.msg);
		
		if(strcmp(coda.msg, "QUIT\n") == 0){
			printf("Chiudo connessione\n");
			msgctl(msg_id, IPC_RMID, 0);
			close(sockid);
			exit(0);
		}

	}

}








void *threadFR(void* arg){

int sockid = *(int*)arg;

tipo_coda coda;

	


	while(1){  // tutto quello che viene ricevuto dal server viene inserito nella coda di messaggi
	
	
	memset(coda.msg, '\0', MAX_BUF);
	
	if(recv(sockid, &coda.msg, MAX_BUF, 0) < 0){
		printf("Errore recv\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);	
	}
	
	printf("Ric da ser -- %s", coda.msg);
	coda.m_type = 1; //invio a visualizzatore il messaggio letto dal server
	if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
		perror("Errore msgsnd\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);
	}	
	printf("Inv a vis\n");
	

	}


}








void loginserv(int sockid, int msg_id){

tipo_coda coda;

if(msgrcv(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 3, 0) < 0){		//aspetto nick e user da visual
		perror("Errore msgrcv\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);
		}
		
		printf("Nick e user arrivati\n");
		
	if(send(sockid, coda.nick, strlen(coda.nick), 0)<0){   //invio nick e user a server
		perror("Errore send\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);		
		}
	
		
	if(send(sockid, coda.user, strlen(coda.user), 0)<0){
		perror("Errore send\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);		
		}
	
	if(send(sockid, coda.nick, strlen(coda.nick), 0)<0){   
		perror("Errore send\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(1);		
		}
		
	printf("Inviati\n");

	
		
		printf("%s", coda.nick);
		printf("%s", coda.user);

}



