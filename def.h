
#define MAX_BUF 512
#define MSG_KEY 6666

const char* KEYWORD="EXEC: ";


void *threadFW(void *arg);
void *threadFR(void *arg);

void *threadVW(void *arg);
void *threadVR(void *arg);




typedef struct{
long int m_type; //1 visual, 2 executer, 3 client
char msg[MAX_BUF];
int porta;
char nick[MAX_BUF];
char user[MAX_BUF];
char server[50];
}tipo_coda;


void login(int msg_id);
void loginserv(int sockid, int msg_id);
