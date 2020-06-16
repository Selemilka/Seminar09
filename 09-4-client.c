#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {

    srand(time(NULL));

    int msqid;

    char pathname[] = "09-4-server.c";

    key_t key;

    int len, maxlen;

    struct clientmsgbuf {
        long mtype;
        struct {
            pid_t pid;
            char message[256];
        } msg;
    } clientbuf;

    struct servermsgbuf {
        long mtype;
        struct {
            char message[256];
        } msg;
    } serverbuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    clientbuf.mtype = 1;
    clientbuf.msg.pid = getpid();

    len = sizeof(clientbuf.msg);
    sprintf(clientbuf.msg.message, "%d", rand());

    printf("[Client %d]: sends message: %s\n", clientbuf.msg.pid, clientbuf.msg.message);
    if (msgsnd(msqid, (struct clientmsgbuf *) &clientbuf, len, 0) < 0) {
        printf("Can't send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("[Client %d]: Waiting for response...\n", clientbuf.msg.pid);

    maxlen = sizeof(serverbuf.msg);
    if (len = msgrcv(msqid, &serverbuf, maxlen, getpid(), 0) < 0) {
        printf("Can't receive message from queue\n");
        exit(-1);
    }
    printf("[Client %d]: Recieved from Server: %s\n", clientbuf.msg.pid, serverbuf.msg.message);

    return 0;

}





