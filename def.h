
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
	char porta[50];
	char nick[MAX_BUF];
	char user[MAX_BUF];
	char server[50];
}tipo_coda;


void login(int msg_id);
void loginserv(int sockid, int msg_id);


void *threadVW(void *arg);
void *threadVR(void *arg);
void login();

char irc_reg();
void mod_config();
int connection_menu();

void irc_nick(char *comando);

int PONG(int sock);
void QUIT();

void WHOIS(char *comando);
void irc_msg(char *comando, char *msg, char *nome);
void irc_join(char *comando);
void irc_part(char *comando);
void irc_whois(char *comando);

void toclient(char *comando);
void irc_free();
void funzione_stanza(char *nome);
void funzione_chatnick(char *nome);
int menu();

int msg_id;
