#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "def.h"

#define MAX_TEXT_COMMAND 1024

int main(void){
	int msg_id;
	char command [MAX_TEXT_COMMAND];
	char *com;
	FILE *fp;
	char output[MAX_BUF];
	tipo_coda msg_r;
	
	
	msg_id=msgget(MSG_KEY, 0666 | IPC_CREAT);
	if(msg_id==-1){
		perror("Impossible to create the message queue.\n");
		exit(EXIT_FAILURE);
	}
	
	if(msgrcv(msg_id, (void *)&msg_r, sizeof(msg_r)-sizeof(long int), 2, 0)==-1){
		perror("[EXECUTER]Impossible to receive a new message.\n");
		exit(EXIT_FAILURE);
	}
	strcpy(command, msg_r.mess.msg);
	memset(msg_r.mess.msg, '\0', MAX_BUF);
		
	if((com=strstr(command, KEYWORD))!=NULL){
			com=strtok(com, KEYWORD);
			
			
			fp=popen(com, "r");
			if(fp==NULL){
				perror("Impossible popen.\n");
				exit(EXIT_FAILURE);
			}
		
		
			fread(output, MAX_BUF, 1, fp);
		
			strcpy(msg_r.mess.msg, output);
			msg_r.m_type=1;
			if(msgsnd(msg_id, (void *)&msg_r, sizeof(msg_r)-sizeof(long int), 0)==-1){
				perror("[EXECUTER]Impossible to send a message.\n");
				exit(EXIT_FAILURE);
			}
	}
	
}
