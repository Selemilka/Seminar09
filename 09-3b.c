#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>

int main() {
    int msqid;

    char pathname[] = "09-3a.c";

    key_t key;

    int len, maxlen;

    struct mymsgbuf {
        long mtype;
        int value;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    printf("[Program2]: Recieving messages from first program...\n");
    for (int i = 0; i < 5; ++i) {
        maxlen = sizeof(mybuf.value);
        if (len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }

        printf("[Program2]: recieve message type = %ld, iInfo = %i\n", mybuf.mtype, mybuf.value);
    }

    printf("[Program2]: finished receiving messages from Program1.\n");
    printf("[Program2]: sending messages to Program1.\n");

    for (int i = 0; i < 5; ++i) {
        mybuf.mtype = 2;
        mybuf.info.iinfo = 999;
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    printf("[Program2]: finished sending messages to Program1.\n");

    return 0;
}
