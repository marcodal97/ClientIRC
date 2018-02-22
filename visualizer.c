//Visualizer

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <string.h>

#define MSG_KEY 1234
#define MAX_TEXT 2048


typedef struct{
	long int mtype;//0 per executer, 1 per scrivere, 2 per ricevere
	char text[MAX_TEXT];
}data;

int main(void){
	int msg_id;
	data msg_r, msg_s;
	char message [MAX_TEXT];
	char *token;
	
	/*msg_id=msgget(MSG_KEY, 0666 | IPC_CREAT);
	if(msg_id==-1){
		perror("Impossible to create the message queue.\n");
		exit(EXIT_FAILURE);
	}
	
	int running=1;
	while(running){
		
		if((msgrcv(msg_id, (void *)&msg_r, MAX_TEXT, 0, 0))==-1){
			exit(EXIT_FAILURE);
		}
		
		if(strstr(msg_r.text, "QUIT")!=NULL)
			running=0;*/
		strcpy(msg_r.text, ":oras!~asso@host119-111-dynamic.247-95-r.retail.telecomitalia.it QUIT :Client Quit");
		strcpy(message, msg_r.text);
		token=strtok(message, "@");
		strcpy(message, token);
		printf("%s\n", token);
		memset(token, '\0', sizeof(token));
		printf("%s\n", token);
		token=strstr(message, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		//strcat(message, token);
		printf("%s\n", token);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		/*int i=0;
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		for(i=0;i<strlen(token);i++){
			if(isupper(message[i])){
				token=strtok(NULL, token[i-1]);
				break;
			}
		}
		strcat(message, token);
		printf("%s", token);
	//}*/
	
		








}
