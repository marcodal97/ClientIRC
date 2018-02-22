#define MAX_BUF 8192
#define MSG_KEY 6666
const char* KEYWORD="EXEC: ";

void *threadFW(void *arg);
void *threadFR(void *arg);

typedef struct{
char msg[MAX_BUF];
}tipo_dato;

typedef struct{
long int m_type; //1 visual, 2 executer, 3 client
tipo_dato mess;
}tipo_coda;



