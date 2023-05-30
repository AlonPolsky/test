#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"

#define ARGV_ARGUMENTS 3 + 1
#define SYSCALL_EXIT_FAILURE -1
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define PATH_LOC 1
#define CHANNEL_ID_LOC 2
#define MSG_LOC 3
#define ERROR_CHECK_SENDER(condition, message) ERROR_CHECK(condition, perror(message); exit(EXIT_FAILURE);, EXIT_FAILURE)

int main(int argc, char* argv[])
{
    int fd;

    ERROR_CHECK_SENDER(argc != ARGV_ARGUMENTS, "Wrong argumet's number.\n")
    
    fd = open(argv[PATH_LOC], O_WRONLY);

    ERROR_CHECK_SENDER(fd == SYSCALL_EXIT_FAILURE,"Error on open.\n")

    ERROR_CHECK_SENDER(ioctl(fd, MSG_SLOT_CHANNEL, atol(argv[CHANNEL_ID_LOC])) == SYSCALL_EXIT_FAILURE,"Error on ioctl.\n")

    ERROR_CHECK_SENDER(write(fd, argv[MSG_LOC], strlen(argv[MSG_LOC])) == SYSCALL_EXIT_FAILURE,"Error on write.\n")

    ERROR_CHECK_SENDER(close(fd) == SYSCALL_EXIT_FAILURE,"Error on close.\n")

    exit(EXIT_SUCCESS);
}