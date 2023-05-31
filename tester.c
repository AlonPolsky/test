#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"


int main(){
    int i = 0;
    int fd;
    int j = 3;

    fd = open("/dev/message_slot_0", O_WRONLY);

    for(i = 1; i < (1 << 20); i++)
    {
        ERROR_CHECK_USERLEVEL(write(fd, j&, i), 1, fd, WR_ERR)
    }

}