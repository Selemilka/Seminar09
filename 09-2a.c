#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main() {
    int msqid;

    char pathname[] = "09-2a.c";

    key_t key;

    int len;

    struct mymsgbuf {
        long mtype;
        struct {
            int x;
            char c;
            double d;
        } msg;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    for (int i = 0; i < 5; ++i) {
        mybuf.mtype = 1;
        mybuf.msg.x = 42;
        mybuf.msg.c = 'w';
        mybuf.msg.d = 5.51;
        len = sizeof(mybuf.msg);

        if (msgsnd(msqid, &mybuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    mybuf.mtype = LAST_MESSAGE;
    len = 0;
    if (msgsnd(msqid, &mybuf, len, 0) < 0) {
        printf("Can't send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    return 0;
}
