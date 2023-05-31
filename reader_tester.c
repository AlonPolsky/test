#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"

#define ARGV_ARGUMENTS 1 + 2
#define ERROR_CHECK_USERLEVEL(syscall, post_open, fd, message) ERROR_CHECK(syscall == SYSCALL_EXIT_FAILURE, perror(message); if(post_open){close(fd);} exit(EXIT_FAILURE);, EXIT_FAILURE)

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
    
    fd = open(argv[PATH_LOC], O_RDONLY);

    ERROR_CHECK_USERLEVEL(fd, 0, 0, OPEN_ERR)

    ERROR_CHECK_USERLEVEL(ioctl(fd, MSG_SLOT_CHANNEL, 2), 1, fd, IOCTL_ERR)

    ERROR_CHECK_USERLEVEL((len = read(fd, buf, 4)), 1, fd, RD_ERR)

    ERROR_CHECK_USERLEVEL(close(fd), 0, 0, CLOSE_ERR)

    ERROR_CHECK_USERLEVEL(write(STDOUT_FD, buf, len), 0, 0, WR_ERR)

    exit(EXIT_SUCCESS);
}