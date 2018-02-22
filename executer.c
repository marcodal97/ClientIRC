#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define MAX_TEXT_COMMAND 1024
#define MAX_TEXT_OUTPUT 2048
#define MSG_KEY 1234
const char* KEYWORD="EXEC: ";

typedef struct{
	long int mtype;//0 per executer, 1 per scrivere, 2 per ricevere
	char text[MAX_TEXT_OUTPUT];
}data;


int main(void){
	int msg_id;
	char command [MAX_TEXT_COMMAND];
	char *com;
	FILE *fp;
	char output[MAX_TEXT_OUTPUT];
	data msg_r;
	
	
	msg_id=msgget(MSG_KEY, 0666 | IPC_CREAT);
	if(msg_id==-1){
		perror("Impossible to create the message queue.\n");
		exit(EXIT_FAILURE);
	}
	
	if(msgrcv(msg_id, (void *)&msg_r, MAX_TEXT_OUTPUT, 2, 0)==-1){
		perror("[EXECUTER]Impossible to receive a new message.\n");
		exit(EXIT_FAILURE);
	}
	strcpy(command, msg_r.text);
	memset(msg_r.text, '\0', MAX_TEXT_OUTPUT);
		
	if((com=strstr(command, KEYWORD))!=NULL){
			com=strtok(com, KEYWORD);
			
			
			fp=popen(com, "r");
			if(fp==NULL){
				perror("Impossible popen.\n");
				exit(EXIT_FAILURE);
			}
		
		
			fread(output, MAX_TEXT_OUTPUT, 1, fp);
		
			strcpy(msg_r.text, output);
			msg_r.mtype=1;
			if(msgsnd(msg_id, (void *)&msg_r, MAX_TEXT_OUTPUT, 0)==-1){
				perror("[EXECUTER]Impossible to send a message.\n");
				exit(EXIT_FAILURE);
			}
	}
	
}
