#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include "message_slot.h"

#define ARGV_ARGUMENTS 3 + 1
#define WRONG_ARG_AMOUNT_ERR "Wrong argumet's number."
#define SYSCALL_EXIT_FAILURE -1
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define PATH_LOC 1
#define CHANNEL_ID_LOC 2
#define MSG_LOC
#define ERROR_CHECK(condition, message){\
    if(condition){\
        perror(message);\
        exit(EXIT_FAILURE);\
    }\
}

int main(int argc, char* argv[])
{
    int fd;
    int channel;

    ERROR_CHECK(argc != ARGV_ARGUMENTS, WRONG_ARG_AMOUNT_ERR)
    
    fd = open(argv[PATH_LOC], O_WRONLY);

    ERROR_CHECK(fd == SYSCALL_EXIT_FAILURE,)

    ERROR_CHECK(ioctl(fd, MSG_SLOT_CHANNEL, atol(argv[CHANNEL_ID_LOC])) == SYSCALL_EXIT_FAILURE,)

    ERROR_CHECK(write(fd,argv[MSG_LOC],strlen(argv[MSG_LOC])) == SYSCALL_EXIT_FAILURE,)

    ERROR_CHECK(close(fd) == SYSCALL_EXIT_FAILURE,)

    exit(EXIT_SUCCESS);
}