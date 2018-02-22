#define MAX_BUF 512
#define MSGKEY 6666

void *threadFW(void *arg);
void *threadFR(void *arg);

void *threadVW(void *arg);
void *threadVR(void *arg);




typedef struct{
long int m_type; //1 visual, 2 executer, 3 client
char msg[MAX_BUF];
int porta;
char nick[20];
char user[20];
char server[20];
}tipo_coda;



