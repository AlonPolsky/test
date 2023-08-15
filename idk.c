#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

#define ERR_CHECK(checked, message) { \
    if(checked)                         \
    {                                   \
        perror(message);                \
    }                                   \
}

static int fd;
static char c;

/*--SIGNAL-HANDLER--*/

void handler(int sigtype)
{
    char c = 'b';
    printf("%d\n", fd);
    write(fd, &c, 1);
    close(fd);
    exit(0);
}

/*-----------------*/

int main()
{
    int p[2];
    pid_t pid1, pid2;
    int a;
    char c;

    ERR_CHECK((pid1 = fork()) < 0, "fork")
    ERR_CHECK(pipe(p) < 0, "pipe")
    
    if(pid1)
    {
        c = 'a';
        close(p[0]);
        a = write(p[1], &c, 1);
        fprintf(stdout, "writen: %d\n", a);
        ERR_CHECK(close(p[1]), "close")
    }
    else
    {
        sleep(5);
        fprintf(stdout, "writen: %d\n", a);
    }
}