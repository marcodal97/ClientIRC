
#define MAX_BUF 512
#define MSG_KEY 6666


const char* KEYWORD="EXEC: ";
char nickname[MAX_BUF];
char username[MAX_BUF];
char nick[MAX_BUF];
char user[MAX_BUF];

void *threadFW(void *arg);
void *threadFR(void *arg);

void *threadVW(void *arg);
void *threadVR(void *arg);


typedef struct{
	long int m_type; //1 visual, 2 executer, 3 client
	char msg[MAX_BUF];
	char porta[5];
	char nick[MAX_BUF];
	char user[MAX_BUF];
	char server[50];
}tipo_coda;


void login(int msg_id);
void loginserv(int sockid, int msg_id);

void creaNICK(char *nick, char *comando);
void creaUSER(char *nome, char *comando);

int PONG(int sock);

void WHOIS(char *comando);
void MSG(char *comando, char *msg, char *nome);
void JOIN(char *comando);
void PART(char *comando);

int msg_id;
