#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "def.h"

#define MAX_TEXT_COMMAND 1024
const char* CHIOCCIOLA="@";

void trova_user(char *command, char *user_comandante){
	char *inizio, *fine;
	
	
	if((inizio=strstr(command, ":"))){
		inizio+=strlen(":");
		if((fine=strstr(inizio, "!"))){
			memcpy(user_comandante, inizio, fine-inizio);
		}
	}
	


}

int main(void){
	int msg_id;
	char command [MAX_TEXT_COMMAND];
	char *com;
	FILE *fp;
	char output[MAX_BUF];
	tipo_coda msg_r;
	char user_comandante[MAX_BUF];
	
	
	
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
	
	if(com=strstr(command, KEYWORD)){
			
			com=strtok(com, KEYWORD);						
			trova_user(command, user_comandante);			
			
			fp=popen(com, "r");
			if(fp==NULL){
				perror("Impossible popen.\n");
				exit(EXIT_FAILURE);
			}
		
			
			fread(output, MAX_BUF, 1, fp);
			printf("%s", output);
			strcpy(msg_r.mess.msg, output);
			msg_r.m_type=1;
			if(msgsnd(msg_id, (void *)&msg_r, sizeof(msg_r)-sizeof(long int), 0)==-1){
				perror("[EXECUTER]Impossible to send a message.\n");
				exit(EXIT_FAILURE);
			}
	}
	
}
