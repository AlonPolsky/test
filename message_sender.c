#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"

#define ARGV_ARGUMENTS 3 + 1

// This error checker compares the syscall's return value and if it is equal to -1, it exists in an according manner.
#define ERROR_CHECK_USERLEVEL(syscall, post_open, fd, message) ERROR_CHECK(syscall == SYSCALL_EXIT_FAILURE, perror(message); if(post_open){close(fd);} exit(EXIT_FAILURE);, EXIT_FAILURE)

int main(int argc, char* argv[])
{
    int fd;

    if(argc != ARGV_ARGUMENTS)
    {
        // I intentionally don't use perror()/strerror(), because it'll lead to an uninformative error message, due to the fact that at this point of run-time,
        // strerr will be set to 0, which indicates success, even if argc != ARGV_ARGUMENTS.
        fprintf(stderr, WRONG_ARG_NUM_ERR);
        exit(EXIT_FAILURE);
    }
    
    fd = open(argv[PATH_LOC], O_WRONLY);

    ERROR_CHECK_USERLEVEL(fd, 0, 0,OPEN_ERR)

    ERROR_CHECK_USERLEVEL(ioctl(fd, MSG_SLOT_CHANNEL, atol(argv[CHANNEL_ID_LOC])), 1, fd, IOCTL_ERR)

    ERROR_CHECK_USERLEVEL(write(fd, argv[MSG_LOC], strlen(argv[MSG_LOC])), 1, fd, WR_ERR)

    ERROR_CHECK_USERLEVEL(close(fd), 0, 0, CLOSE_ERR)

    exit(EXIT_SUCCESS);
}