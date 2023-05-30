#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"

#define ARGV_ARGUMENTS 1 + 2
#define ERROR_CHECK_USERLEVEL(syscall, message) ERROR_CHECK(syscall == SYSCALL_EXIT_FAILURE, perror(message); exit(EXIT_FAILURE);, EXIT_FAILURE)

int main(int argc, char* argv[])
{
    int fd;
    char buf[BUF_LEN];
    size_t len;

    if(argc != ARGV_ARGUMENTS)
    {
        // I intentionally don't use perror()/strerror(), because it'll lead to an uninformative error message, due to the fact that at this point of run-time,
        // strerr will be set to 0, which indicates success, even if argc != ARGV_ARGUMENTS.
        fprintf(stderr, WRONG_ARG_NUM_ERR);
        exit(EXIT_FAILURE);
    }
    
    fd = open(argv[PATH_LOC], O_WRONLY);

    ERROR_CHECK_USERLEVEL(fd, OPEN_ERR)

    ERROR_CHECK_USERLEVEL(ioctl(fd, MSG_SLOT_CHANNEL, atol(argv[CHANNEL_ID_LOC])), IOCTL_ERR)

    ERROR_CHECK_USERLEVEL(len = read(fd, buf, BUF_LEN), RD_ERR)

    ERROR_CHECK_USERLEVEL(close(fd), CLOSE_ERR)

    ERROR_CHECK_USERLEVEL(write(STDOUT_FD, buf, len), WR_ERR)


    exit(EXIT_SUCCESS);
}