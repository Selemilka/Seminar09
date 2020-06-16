#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
    int msqid;

    char pathname[] = "09-4_Server.c";

    key_t key;

    int len, maxlen;

    struct clientmsgbuf {
        long mtype;
        struct {
            pid_t pid;
            char message[256];
        } msg;
    } clientbuf;

    struct sermsgbuf {
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

    while (1) {
        maxlen = sizeof(clientbuf.msg);
        if (len = msgrcv(msqid, (struct clientmsgbuf *) &clientbuf, maxlen, 1, 0) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }

        printf("Client %d: %s\n", clientbuf.msg.pid, clientbuf.msg.message);

        serverbuf.mtype = clientbuf.msg.pid;

        strcpy(serverbuf.msg.message, clientbuf.msg.message);
        len = sizeof(serverbuf.msg);

        if (msgsnd(msqid, (struct sermsgbuf *) &serverbuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("Sent response\n");
    }
    return 0;
}
