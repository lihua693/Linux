/*************************************************************************
	> File Name: 1.pipeOpt.c
	> Author: 好帅银
	> Mail: 909585830@qq.com
	> Created Time: Sun 06 Nov 2022 01:18:09 PM CST
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    pid_t pid;
    int fd[2], n;
    char buff[20];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) {
        close(fd[0]);
        write(fd[1], "hello my child\n", 15);
        wait(NULL);
    } else {
        close(fd[1]);
        n = read(fd[0], buff, 20);
        write(1, buff, n);
    }
    return 0;
}

