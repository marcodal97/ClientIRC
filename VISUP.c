#include <sys/ipc.h>
#include <sys/msg.h>
#include "def.h"
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


int msg_id;


int main(){


tipo_coda coda;

msg_id = msgget((key_t)MSGKEY, 0666 | IPC_CREAT);
if(msg_id < 0){
printf("Errore msgget\n");
exit(1);
}



coda.type = 3;
strcpy(coda.mess.msg, "NICK nickprova");
printf("Invio nick\n");
msgsnd(msg_id, (void*)&coda, sizeof(tipo_coda)-sizeof(long int), 0);

printf("Inviato NICK\n");

memset(coda.mess.msg, '\0', MAX_BUF);

strcpy(coda.mess.msg, "USER prova prova prova prova");
msgsnd(msg_id, (void*)&coda, sizeof(tipo_dato), 0);

printf("Inviato USER\n");

while(1){
memset(coda.mess.msg, '\0', MAX_BUF);
msgrcv(msg_id, (void*)&coda, sizeof(tipo_dato), 1, 0);
printf("%s", coda.mess.msg);
sleep(1);
}
     



}
