/*************************************************************************
	> File Name: 2.execOpt.c
	> Author: 好帅银
	> Mail: 909585830@qq.com
	> Created Time: Sat 05 Nov 2022 08:55:40 AM CST
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
    
    execlp("ls", "ls", "-a", "-l", NULL);

    perror("exec");
    exit(1);
    return 0;
}

