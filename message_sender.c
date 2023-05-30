#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"

#define ARGV_ARGUMENTS 3 + 1

#define ERROR_CHECK_USERLEVEL(condition, message) ERROR_CHECK(condition, perror(message); exit(EXIT_FAILURE);, EXIT_FAILURE)

int main(int argc, char* argv[])
{
    int fd;

    ERROR_CHECK_USERLEVEL(argc != ARGV_ARGUMENTS, WRONG_ARG_NUM_ERR)
    
    fd = open(argv[PATH_LOC], O_WRONLY);

    ERROR_CHECK_USERLEVEL(fd == SYSCALL_EXIT_FAILURE,OPEN_ERR)

    ERROR_CHECK_USERLEVEL(ioctl(fd, MSG_SLOT_CHANNEL, atol(argv[CHANNEL_ID_LOC])) == SYSCALL_EXIT_FAILURE, IOCTL_ERR)

    printf("%zu \n", write(fd, argv[MSG_LOC], strlen(argv[MSG_LOC])));

    ERROR_CHECK_USERLEVEL(write(fd, argv[MSG_LOC], strlen(argv[MSG_LOC])) == SYSCALL_EXIT_FAILURE, WR_ERR)

    ERROR_CHECK_USERLEVEL(close(fd) == SYSCALL_EXIT_FAILURE, CLOSE_ERR)

    exit(EXIT_SUCCESS);
}