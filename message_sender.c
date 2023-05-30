#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"

#define ARGV_ARGUMENTS 3 + 1

#define RROR_CHECK_USERLEVEL(condition, message) ERROR_CHECK(condition, perror(message); exit(EXIT_FAILURE);, EXIT_FAILURE)

int main(int argc, char* argv[])
{
    int fd;

    RROR_CHECK_USERLEVEL(argc != ARGV_ARGUMENTS, WRONG_ARG_NUM_ERR)
    
    fd = open(argv[PATH_LOC], O_WRONLY);

    RROR_CHECK_USERLEVEL(fd == SYSCALL_EXIT_FAILURE,OPEN_ERR)

    RROR_CHECK_USERLEVEL(ioctl(fd, MSG_SLOT_CHANNEL, atol(argv[CHANNEL_ID_LOC])) == SYSCALL_EXIT_FAILURE, IOCTL_ERR)

    printf("%l", write(fd, argv[MSG_LOC], strlen(argv[MSG_LOC])));

    RROR_CHECK_USERLEVEL(write(fd, argv[MSG_LOC], strlen(argv[MSG_LOC])) == SYSCALL_EXIT_FAILURE, WR_ERR)

    RROR_CHECK_USERLEVEL(close(fd) == SYSCALL_EXIT_FAILURE, CLOSE_ERR)

    exit(EXIT_SUCCESS);
}