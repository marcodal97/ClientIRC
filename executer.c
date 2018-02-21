#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAX_TEXT_COMMAND 1024
#define MAX_TEXT_OUTPUT 2048
const char* KEYWORD="EXEC: ";

int main(void){
	char command [MAX_TEXT_COMMAND];
	char *com;
	FILE *fp;
	char output[MAX_TEXT_OUTPUT];
	
	printf("Digitare il comando: ");
	fgets(command, MAX_TEXT_COMMAND, stdin);
	
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
