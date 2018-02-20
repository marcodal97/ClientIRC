#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

int main(void){
	int client_sock, client_len;
	struct hostent *hp;
	struct sockaddr_in client_address;
	char nomehost[1024];
	int num_porta;
	
	client_sock=socket(AF_INET, SOCK_STREAM, 0);
	if(client_sock==-1){
		perror("Impossible create a new socket.\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Inserire il nome del server a cui ci si vuole connettere?\n");
	fgets(nomehost, 1024, stdin);
	printf("Inserire il numero di porta del server: ");
	scanf("%d", &num_porta);
	
	
	client_address.sin_family=AF_INET;
	hp=gethostbyname(nomehost);
	client_address.sin_addr=*(struct in_addr *)*hp->h_addr_list;
	client_address.sin_port=htons(num_porta);
	client_len=sizeof(client_address);
	
	
}
