#define MAX_BUF 8192
#define MSGKEY 6666

void *threadFW(void *arg);
void *threadFR(void *arg);

typedef struct{
char msg[MAX_BUF];
}tipo_dato;

typedef struct{
long int m_type; //1 visual, 2 executer, 3 client
tipo_dato mess;
}tipo_coda;



