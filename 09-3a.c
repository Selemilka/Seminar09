#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

    printf("[Program1]: start sending messages to Program2.\n");
    for (int i = 0; i < 5; ++i) {
        mybuf.mtype = 1;
        mybuf.value = 42;
        len = sizeof(mybuf.value);

        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    printf("[Program1]: sent messages, recieving from Program2...\n");
    for (int i = 0; i < 5; ++i) {
        maxlen = sizeof(mybuf.value);
        if (len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 2, 0) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }

        printf("[Program1] received: message type = %ld, value = %d\n", mybuf.mtype, mybuf.value);
    }

    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

    printf("[Program1]: stop recieving messages.\n");

    return 0;
}
