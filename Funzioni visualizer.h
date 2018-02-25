
/*PONG: di tanto in tanto il server pinga il client che deve rispondere con questo comando, altrimenti la connessione cade
NICK: imposta il nickname univoco
JOIN: joina una stanza
PART: lasciare il canale
QUIT: per quittare IRC
TOPIC: settare o rimuovere un topic
MSG: inviare un messaggio ad una stanza o un utente
WHO
WHOIS*/

int connection_menu(){
	int scelta;
	
	printf("\n\n************ Benvenuto nel miglior client IRC al mondo ************\n\n");//scherzo
	printf("\t\t------ Connection menu' ------\n");
	printf("1-Modifica il file di configurazione (server, porta, nickname e username).\n");
	printf("2-Connettiti al server.\n");
	printf("Inserisci la tua scelta: ");
	fflush(stdin);
	scanf("%d", &scelta);

	return scelta;
}
//tipo_coda coda;

void mod_config(){
	FILE * fp;
	char nome_server[MAX_BUF];
	char porta[50];
	char text[MAX_BUF];
	char *token;
	tipo_coda coda;
	
	fp=fopen("configurazione.txt", "w");
	if(fp==NULL){
		printf("Impossibile aprire il file.\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Inserire il nome del server: ");
	memset(nome_server, '\0', MAX_BUF);
	getchar();
	while(fgets(nome_server, 50, stdin)==NULL);	
	printf("Inserire il nome della porta: ");
	while(fgets(porta, 50, stdin)==NULL);	
	printf("Inserire il nickname: ");
	while(fgets(nick, MAX_BUF-1, stdin)==NULL);
	printf("Inserisci l'username: ");
	while(fgets(user, MAX_BUF-1, stdin)==NULL);
	
	strcpy(coda.server, nome_server);
	strcpy(coda.porta, porta);
	strcpy(nickname, "NICK ");	
	strcat(nickname, nick);
	strcpy(username, "USER ");
	strcat(username, "guest guest guest :");
	strcat(username, user);
	strcpy(text, nome_server);
	strcat(text, "\n");
	strcat(text, porta);
	strcat(text, nickname);
	strcat(text, username);
	if(fputs(text, fp)==0){
		perror("Impossibile to write in the file.\n");
		exit(EXIT_FAILURE);
	}
	fclose(fp);
	
	printf("File di configurazione modificato.\n");
}

char irc_reg(){
	FILE *fp;
	char text[MAX_BUF], output[MAX_BUF];
	char ch;
	int flag;
	tipo_coda coda;
	
	
	fp=fopen("configurazione.txt", "r");
	if(fp==NULL){
		perror("Impossibile aprire il file.\n");
		exit(EXIT_FAILURE);
	}
	while(fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]", coda.server, coda.porta, coda.nick, coda.user)>0);
	
	
	strcpy(output, coda.server);
	strcat(output, "\n");
	strcat(output, coda.porta);
	strcat(output, "\n");
	strcat(output, coda.nick);
	strcat(output, "\n");
	strcat(output, coda.user);
	getchar();
	printf("\n\nIl file di configurazione dice: \n\n");
	printf("---------------------\n");
	printf("%s", output);
	printf("\n---------------------\n");
	printf("\n\nConfermare il file?(S=si, N=no)\n");
	ch=getchar();
	if(ch=='S'){		
		coda.m_type=3;
		strcpy(coda.msg, output);
		if(msgsnd(msg_id, (void *)&coda, sizeof(tipo_coda), 0)==-1){
			perror("Impossible to send a message.\n");
			exit(EXIT_FAILURE);
		}
		
	}
	
	return ch;
}

void irc_nick(char *comando){  //Crea comando per cambiare nick
	char str[MAX_BUF];	
	strcpy(str, comando);
	memset(comando, '\0', MAX_BUF);
	strcpy(comando, "NICK ");
	strcat(comando, str);
	comando[strlen(comando)+1] = '\0';
	comando[strlen(comando)] = '\n';
	
}

void irc_join(char *comando){  //Crea comando per entrare in una stanza
	
	char str[MAX_BUF];	
	strcpy(str, comando);
	memset(comando, '\0', MAX_BUF);
	strcpy(comando, "JOIN ");
	strcat(comando, str);
	comando[strlen(comando)] = '\n';
	comando[strlen(comando)+1] = '\0';
	
}

void irc_part(char *comando){  //Crea comando per lasciare stanza

	char str[MAX_BUF];	
	strcpy(str, comando);
	memset(comando, '\0', MAX_BUF);
	strcpy(comando, "PART ");
	strcat(comando, str);
	comando[strlen(comando)] = '\n';
	comando[strlen(comando)+1] = '\0';
}


void irc_msg(char *comando, char *msg, char *nome){  //dato un messaggio e un canale/utente crea il comando per mandare il messaggio


	strcpy(comando, "PRIVMSG ");
	strcat(comando, nome);
	strcat(comando, " :");
	strcat(comando, msg);
	comando[strlen(comando)] = '\n';
	comando[strlen(comando)+1] = '\0';

}

void irc_whois(char *comando){

	char nome[MAX_BUF];

	strcpy(nome, comando);
	memset(comando, '\0', MAX_BUF);
	strcpy(comando, "WHOIS ");
	strcat(comando, nome);
	strcat(comando, "\n");
	comando[strlen(comando)] = '\0';

}


void funzione_stanza(char *nome){
	
	char messaggio[MAX_BUF];
	tipo_coda coda;
	printf("\n\n-----Sei entrato nella stanza \"%s\"------\n", nome);
	printf("\n-----Per tornare al menu' precedente digitare \"i\".-----\n");
	while(1){
		memset(coda.msg, '\0', MAX_BUF);
		memset(messaggio, '\0', MAX_BUF);
		scanf(" %[^\n]", messaggio);
		if(messaggio[0]!='i'){
			irc_msg(coda.msg, messaggio, nome);
			toclient(coda.msg);
		}
		else return;
		
	}

}

void irc_who(char *nome){
	char messaggio[MAX_BUF];
	
	strcpy(messaggio, "WHO ");
	strcat(messaggio, nome);
	memset(nome, '\0', MAX_BUF);
	strcat(messaggio, "\n");
	messaggio[strlen(messaggio)]='\0';
	strcpy(nome, messaggio);
}

void irc_topic(char *stanza, char *msg){
	char messaggio[MAX_BUF];
	
	strcpy(messaggio, "TOPIC ");
	strcat(messaggio, stanza);
	strcat(messaggio, " :");
	strcat(messaggio, msg);
	memset(msg, '\0', MAX_BUF);
	strcat(messaggio, "\n");	
	messaggio[strlen(messaggio)]='\0';
	strcpy(msg, messaggio);
}

void funzione_chatnick(char *nome){
	
	char messaggio[MAX_BUF];
	tipo_coda coda;
	printf("\n\n----- Puoi chattare con \"%s\"------\n", nome);
	printf("\n----- Per tornare al menu' precedente digitare \"i\".-----\n");
	while(1){
		memset(coda.msg, '\0', MAX_BUF);
		memset(messaggio, '\0', MAX_BUF);
		scanf(" %[^\n]", messaggio);
		if(messaggio[0]!='i'){
			irc_msg(coda.msg, messaggio, nome);
			toclient(coda.msg);
		}
		else return;		
	}

}


void irc_free(){  //funzione per la modalità libera
	tipo_coda coda;
	
	printf("\n----- Per tornare al menu' precedente digitare \"i\"-----\n");
	memset(coda.msg, '\0', MAX_BUF);
	while(1){				
		scanf(" %[^\n]", coda.msg);
		if(coda.msg[0]!='i'){
			coda.m_type = 3;
			coda.msg[strlen(coda.msg)] = '\n';
			coda.msg[strlen(coda.msg)+1] = '\0';				
			if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0)< 0){
				perror("Errore msgsnd\n");
				exit(1);
			}		
			if(strcmp(coda.msg, "QUIT\n") == 0){
				printf("Uscita...\n");
				exit(0);
			}
		}else {
		printf("CIAO\n");
		return;}
	}
}


void QUIT(){

	tipo_coda coda;

	strcpy(coda.msg, "QUIT\n");
	coda.m_type = 3;

	if(msgsnd(msg_id, &coda, sizeof(tipo_coda) - sizeof(long int), 0) < 0){
		perror("Errore sendmsg\n");
		exit(1);
	}
	
	printf("\n\nChiusura Visualizer...\n");
	exit(0);
}



int menu(){
	int scelta;
	
	printf("************Menu'************\n");
	printf("1-Cambia il tuo nick.\n");
	printf("2-Entra e scrivi all'interno di una stanza.\n");
	printf("3-Lascia una stanza.\n");
	printf("4-Setta o rimuovi un nuovo topic.\n");
	printf("5-Parla con un utente\n");
	printf("6-Cerca utenti.\n");
	printf("7-Informazioni su un utente.\n");
	printf("8-Esci da IRC e dal client.\n");
	printf("9-Inserisci da tastiera i comandi.\n");
	printf("10-Esegui un comando in una stanza.\n");
	printf("Inserisci la tua scelta: ");
	scanf("%d", &scelta);
	
	return scelta;
}
