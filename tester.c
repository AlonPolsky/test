#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"

#define ERROR_CHECK_USERLEVEL(syscall, post_open, fd, message) ERROR_CHECK(syscall == SYSCALL_EXIT_FAILURE, perror(message); if(post_open){close(fd);} exit(EXIT_FAILURE);, EXIT_FAILURE)

int main(){
    int fd;
    char* idk = "idk";
    char a;

    fd = open("/dev/message_slot_0", O_RDWR);

    ERROR_CHECK_USERLEVEL(ioctl(fd, MSG_SLOT_CHANNEL, 1), 1, fd, IOCTL_ERR)
    ERROR_CHECK_USERLEVEL(write(fd, &(*idk), 3), 1, fd, WR_ERR)
    ERROR_CHECK_USERLEVEL(read(fd, &a, 1), 1, fd, RD_ERR)



    close(fd);
}