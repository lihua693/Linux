/*************************************************************************
	> File Name: 1.fork.c
	> Author: 好帅银
	> Mail: 909585830@qq.com
	> Created Time: Fri 04 Nov 2022 03:25:17 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define pexit(x) perror(x);\
        exit(1);

int main(){
    char *msg;
    int n = 1;
    pid_t pid = fork();
    if (pid < 0) {
        pexit("fork");
    }
    if (pid == 0) {
        msg = "chidl process %d\n";
        n = 5;
    } else {
        msg = "parrent process %d\n";
        n = 3;
    }
    while (n > 0) {
        printf(msg,  pid);
        usleep(5000);
        --n;
    }


    return 0;
}

