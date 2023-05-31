#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/fs.h> 
#include <linux/string.h>  


#define MAJOR_NUM 235

#define DEVICE_NAME "msg_slot"
#define BUF_LEN 128
#define MAX_MINOR 257
// It may be needed to have more than one File name.
#define DEVICE_FILE_NAME "msg_slot_"
#define SUCCESS 0
#define FREE_CHANNEL 0 // We use that number to indicate free channel and illigal channel numbers because it's an illigal channel number.
#define MSG_SLOT_CHANNEL _IO(MAJOR_NUM, 0)
#define MIN_WRITE 1
#define ENOMEM 12

//================== USER LEVEL DEFINES ===========================

#define SYSCALL_EXIT_FAILURE -1
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define PATH_LOC 1
#define CHANNEL_ID_LOC 2
#define MSG_LOC 3
#define STDOUT_FD 1
#define WRONG_ARG_NUM_ERR "Wrong argumet's number.\n"
#define OPEN_ERR "Error on open"
#define IOCTL_ERR "Error on ioctl"
#define WR_ERR "Error on write"
#define RD_ERR "Error on read"
#define CLOSE_ERR "Error on close"

//================== END OF USER LEVEL DEFINES ====================

#define ERROR_CHECK(condition, extra_code, errno_value){\
    if(condition){\
        extra_code\
        return -errno_value;\
    }\
}

// A node of a linked-list that will provide abstraction of a channel, used in Msg_Slots.
typedef struct channel
{
    unsigned int minor;
    unsigned long num;
    char msg[BUF_LEN];
    int len;
    struct channel* next;
}channel;

// A linked-list that will provide abstraction of the massage slots.
typedef struct Msg_Slots{
    struct channel* head;
}Msg_Slots;

// This data structure stores the minor number of a struct file and the channel number it is currently set on.
// prev_channel is a pointer to the previous channel that the message was used on, for performance.
typedef struct file_data{
    unsigned int minor;
    long unsigned int channel_num;
    struct channel* prev_channel;
}file_data;

#endif
