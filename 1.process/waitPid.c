/*************************************************************************
	> File Name: waitPid.c
	> Author: 好帅银
	> Mail: 909585830@qq.com
	> Created Time: Sun 06 Nov 2022 01:02:36 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(){
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        int n = 10;
        while (n > 0) {
            printf("this is child process %d\n", getpid());
            sleep(1);
            --n;
        }
        exit(3);
    } else {
        int stat_val;
        waitpid(pid, &stat_val, 0);
        if (WIFEXITED(stat_val)) {
            printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
        } else if (WIFSIGNALED(stat_val)) {
            printf("Child terminated abnormally, signal\n");
        }
    }


    return 0;
}

