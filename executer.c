#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include "def.h"

void irc_msg(char *comando, char *msg, char *nome){  //dato un messaggio e un canale/utente crea il comando per mandare il messaggio


	strcpy(comando, "PRIVMSG ");
	strcat(comando, nome);
	strcat(comando, " :");
	strcat(comando, msg);
	comando[strlen(comando)] = '\n';
	comando[strlen(comando)+1] = '\0';
}

void trova_user(char *command, char *user_scrivente){
	char *inizio, *fine;	
	
	if((inizio=strstr(command, ":"))){
		inizio+=strlen(":");
		for(int i=1;i<MAX_BUF;i++){
			if(inizio[i]=='!' && inizio[i+1]=='~'){
				inizio[i]='\0';
				break;
			}
		}
		strcpy(user_scrivente, inizio);
	}
}


int main(void){
	int msg_id;
	char command [MAX_BUF];
	char *com;
	FILE *fp;
	char output[MAX_BUF];
	char user_scrivente[MAX_BUF];
	tipo_coda coda;	
	char messaggio[MAX_BUF];
	char *str;
	
	
	msg_id=msgget(MSG_KEY, 0666 | IPC_CREAT);
	if(msg_id==-1){
		perror("Impossible to create the message queue.\n");
		exit(EXIT_FAILURE);
	}
	
	while(1){
		memset(output, '\0', MAX_BUF);
		memset(coda.msg, '\0', MAX_BUF);
		memset(messaggio, '\0', MAX_BUF);
		memset(user_scrivente, '\0', MAX_BUF);
		
		if(msgrcv(msg_id, (void *)&coda, sizeof(tipo_coda)-sizeof(long int), 2, 0)==-1){
			perror("[EXECUTER]Impossible to receive a new message.\n");
			exit(EXIT_FAILURE);
		}
		
		if(strcmp(coda.msg, "QUIT\n") == 0){
		exit(0);
		}
		
		strcpy(command, coda.msg);		
		memset(coda.msg, '\0', MAX_BUF);					
		com=strstr(command, KEYWORD);
		com=strstr(com, " :");
		com+=strlen(" :");	
		
		trova_user(command, user_scrivente);		
		
			
		com[strlen(com)-2]='\0';
		
		printf("\n\n******* EXECUTER *******\n");
		printf("E' stato richiesto il comando: %s\n\n", com);
		printf("Inviati i risultati:\n");
		fp=popen(com, "r");
		if(fp==NULL){
			perror("Impossible popen.\n");
			exit(EXIT_FAILURE);
		}
		

		fread(output,MAX_BUF,1,fp);
		
		strcpy(coda.msg, output);
		coda.m_type=1;			
		if(msgsnd(msg_id, (void *)&coda, sizeof(tipo_coda)-sizeof(long int), 0)==-1){
			perror("[EXECUTER]Impossible to send a message.\n");
			exit(EXIT_FAILURE);
		}			
		
		str=strtok(output, "\n");
		while(str!=NULL){
			memset(coda.msg, '\0', MAX_BUF);				
			irc_msg(coda.msg, str, user_scrivente);
			coda.m_type=3;
			if(msgsnd(msg_id, (void *)&coda, sizeof(tipo_coda)-sizeof(long int), 0)==-1){
				perror("[EXECUTER]Impossible to send a message.\n");
				exit(EXIT_FAILURE);
			}
			str=strtok(NULL, "\n");
			
			sleep(1);
		}
		memset(com, '\0', MAX_BUF);
		
		printf("\n\n");
		pclose(fp);
	}	
}
