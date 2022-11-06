/*************************************************************************
	> File Name: 1.envOpt.c
	> Author: 好帅银
	> Mail: 909585830@qq.com
	> Created Time: Sat 05 Nov 2022 08:42:22 AM CST
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
    printf("path value = [%s]\n", getenv("PATH"));

    setenv("PATH", "hell", 1);

    printf("path value = [%s]\n", getenv("PATH"));
    /*
    extern char **environ;

    int i;
    for (i = 0; environ[i] != NULL; ++i) printf("%s\n", environ[i]);
    */
    return 0;
}

