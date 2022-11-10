/*************************************************************************
	> File Name: myshell.c
	> Author: 好帅银
	> Mail: 909585830@qq.com
	> Created Time: Sun 06 Nov 2022 03:17:15 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

void get_input(char *buf) {
    int len = 0;
    int ch;
    ch = getchar();
    while (len < 1024 && ch != '\n') {
        buf[len++] = ch;
        ch = getchar();
    }
    if (len == 1024) {
        printf("command too long\n");
        exit(-1);
    }
    buf[len] = '\n';
    len++;
    buf[len] = '\0';
    //printf("len = %d\n", len);
}

void explanOrder(char *buf, int *argcount, char arglist[100][1024]) {
    char *p = buf;
    char *q = buf;
    int number = 0;
    while (1) {
        if (p[0] == '\n') break;
        if (p[0] == ' ') p++;
        else {
            q = p;
            number = 0;
            while ((q[0] != ' ') && (q[0] != '\n')) {
                number++;
                q++;
            }
            strncpy(arglist[*argcount], p, number + 1);
            arglist[*argcount][number] = '\0';
            *argcount = *argcount + 1;
            p = q;
        }
    }
    
}
void doCmd(char arglist[100][1024], int argcount) {
    char *arg[argcount + 1];
    char *argafter[argcount + 1];
    char *file;
    char *filein = NULL, *fileout = NULL;

    pid_t pid;
    int status;
    int how = 0;
    int fd;
    int f = 0;
    int grep = 0;
    int indf = 0, inde = 0;
    for (int i = 0; i < argcount; ++i) {
        if (!strcmp(arglist[i], "<")) how += 1;
        if (!strcmp(arglist[i], ">")) how += 2;
        if (!strcmp(arglist[i], "|")) {
            how += 4;
            f = 1;
            continue;
        }
        if (!f) arg[indf++] = arglist[i];
        else argafter[inde++] = arglist[i];
    }
    arg[indf] = NULL;
    argafter[inde] = NULL;

    if (how == 3) {
        int t = 0, i = 0;
        while (strcmp(arg[i++], "<"))
        filein = arg[i + 1];
        t = i;
        i = 0;
        while (strcmp(arg[i++], ">"))
        fileout = arg[i + 1];
        t = t < i ? t : i;
        arg[i - 1] = NULL;
    } else {
        if (how & 1) {
            int i = 0;
            while (strcmp(arg[i++], "<"))
            filein = arg[i + 1];
            arg[i - 1] = NULL;
        }
        if (how & 2) {
            if (how < 4) {
                int i = 0;
                while (strcmp(arg[i++], ">"))
                fileout = arg[i + 1];
                arg[i - 1] = NULL;
            } else {
                int i = 0;
                while (strcmp(argafter[i++], ">"));
                fileout = argafter[i];
                argafter[i - 1] = NULL;
            }
        }
        if (how > 7){
            printf("wrong command ~~\n");
            return ;
        }
    }


    /* debug 
    printf("===============\n");
    */
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return ;
    }
    switch (how) {
        case 0:
            if (pid == 0) {
                execvp(arg[0], arg);
                exit(0);
            }
            break;
        case 1:
            if (pid == 0) {
                fd = open(filein, O_RDONLY);
                dup2(fd, 0);
                execvp(arg[0], arg);
                exit(0);
            }
            break;
        case 2:
            if (pid == 0) {
                fd = open(fileout, O_RDWR | O_TRUNC | O_CREAT, 0644);
                dup2(fd, 1);
                execvp(arg[0], arg);
                exit(0);
            }
            break;
        case 3:
            if (pid == 0) {
                fd = open(filein, O_RDONLY);
                dup2(fd, 0);
                fd = open(fileout, O_RDWR | O_TRUNC | O_CREAT, 0644);
                dup2(fd, 1);
                execvp(arg[0], arg);
                exit(0);
            }
            break;
        case 4:
            if (pid == 0) {
                pid_t pid2;
                int fd[2];
                if ((pipe(fd)) < 0) {
                    perror("pipe");
                    exit(-1);
                }
                if ((pid2 = fork()) < 0) {
                    perror("fork");
                    exit(-1);
                }
                if (pid2 == 0) {
                    close(fd[0]);
                    dup2(fd[1], 1);
                    execvp(arg[0], arg);
                    exit(0);
                } else if (pid2 > 0) {
                    close(fd[1]);
                    if (waitpid(pid, &status, 0) == -1) {
                        printf("| error");
                        exit(-1);
                    }
                    dup2(fd[0], 0);
                    execvp(argafter[0], argafter);
                    exit(0);
                }
            }
            break;
        case 5:
            if (pid == 0) {
                int fd[2];
                if ((pipe(fd)) < 0) {
                    perror("pipe");
                    exit(-1);
                }
                pid_t pid2;
                if ((pid2 = fork()) < 0) {
                    perror("fork");
                    exit(-1);
                }
                if (pid2 == 0) {
                    close(fd[0]);
                    dup2(fd[1], 1);
                    int fd = open(filein, O_RDONLY);
                    dup2(fd, 0);
                    execvp(arg[0], arg);
                    exit(0);
                } else if (pid2 != 0) {
                    if (waitpid(pid, &status, 0) == -1) {
                        printf("| error");
                        exit(-1);
                    }
                    close(fd[1]);
                    dup2(fd[0], 0);
                    execvp(argafter[0], argafter);
                    exit(0);
                }
            }
            break;
        case 6:
            if (pid == 0) {
                int fd [2];
                if ((pipe(fd)) < 0) {
                    perror("pipe");
                    exit(-1);
                }
                pid_t pid2 = fork();
                if (pid2 < 0) {
                    perror("fork");
                    exit(-1);
                }
                if (pid2 == 0) {
                    close(fd[0]);
                    dup2(fd[1], 1);
                    execvp(arg[0], arg);
                    exit(0);
                } else if (pid2 != 0) {
                    if (waitpid(pid, &status, 0) == -1) {
                        printf("| error");
                        exit(-1);
                    }
                    close(fd[1]);
                    dup2(fd[0], 0);
                    int fd = open(fileout, O_RDWR | O_TRUNC | O_CREAT, 0644);
                    dup2(fd, 1);
                    execvp(argafter[0], argafter);
                    exit(0);
                }
            }
            break;
        case 7:
            if (pid == 0) {
                int fd [2];
                if ((pipe(fd)) < 0) {
                    perror("pipe");
                    exit(-1);
                }
                pid_t pid2 = fork();
                if (pid2 < 0) {
                    perror("fork");
                    exit(-1);
                }
                if (pid2 == 0) {
                    close(fd[0]);
                    dup2(fd[1], 1);
                    int fd = open(filein, O_RDONLY);
                    dup2(fd, 0);
                    execvp(arg[0], arg);
                    exit(0);
                } else if (pid2 != 0) {
                    close(fd[1]);
                    dup2(fd[0], 0);
                    int fd = open(fileout, O_RDWR | O_TRUNC | O_CREAT, 0644);
                    dup2(fd, 1);
                    execvp(argafter[0], argafter);
                    exit(0);
                }

            }
        break;
    }

    if (waitpid(pid, &status, 0) == -1) {
        printf("wait for child process error\n");
    }
    
}


int main() {
    char *buf = NULL;
    buf = (char *)malloc(1024);
    if (buf == NULL) {
        perror("malloc failed");
        exit(-1);
    }
    while (1) {
        char arglist[100][1024];
        int argcount = 0;
        time_t timep;
        struct tm *p;
        time(&timep);
        p = localtime(&timep);

        memset(buf, 0, 1024);
        printf("%d:%d:%d: myshell$~ ", p->tm_hour, p->tm_min, p->tm_sec);
        get_input(buf);
        explanOrder(buf, &argcount, arglist);
        //printf("argcount = %d\n", argcount);
        //for (int i = 0; i < argcount; ++i) printf("%s \n", arglist[i]);

        if (!strcmp(arglist[0], "exit")) exit(0);
        doCmd(arglist, argcount);
    }
}
