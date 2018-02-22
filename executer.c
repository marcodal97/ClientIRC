#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAX_TEXT_COMMAND 1024
#define MAX_TEXT_OUTPUT 2048
#define MSG_KEY 1234
const char* KEYWORD="EXEC: ";

typedef struct{
	long int mtype;//0 per executer, 1 per scrivere, 2 per ricevere
	char text[MAX_TEXT];
}data;


int main(void){
	int msg_id;
	char command [MAX_TEXT_COMMAND];
	char *com;
	FILE *fp;
	char output[MAX_TEXT_OUTPUT];
	data msg_r;
	
	/*printf("Digitare il comando: ");
	fgets(command, MAX_TEXT_COMMAND, stdin);*/
	
	msg_id=msgget(MSG_KEY, 0666 | IPC_CREAT);
	if(msg_id==-1){
		perror("Impossible to create the message queue.\n");
		exit(EXIT_FAILURE);
	}
	
	if(msgrcv(msg_id, (void *)&msg_r, MAX_TEXT_OUTPUT, 0, 0)==-1){
		exit(EXIT_FAILURE);
	}
	
	if(msg_r.mtype==1 | msg_r.mtype==2){
		if(msgsnd(msg_id, (void *)&msg_s, MAX_TEXT_OUTPUT, 0)==-1){
			exit(EXIT_FAILURE);
		}
	}else{	
		if((com=strstr(command, KEYWORD))!=NULL){
			com=strtok(com, KEYWORD);
		
		
			printf("%s\n\n", com);
		
			fp=popen(com, "r");
			if(fp==NULL){
				perror("Impossible popen.\n");
				exit(EXIT_FAILURE);
			}
		
		
			fread(output, MAX_TEXT_OUTPUT, 1, fp);
		
			printf("%s", output);	
		}
	}
}
