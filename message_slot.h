#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/fs.h> 
#include <linux/string.h>  
#include <linux/slab.h>


#define MAJOR_NUM 235

#define DEVICE_NAME "msg_slot"
#define BUF_LEN 128
#define MAX_MINOR 257
// It may be needed to have more than one File name.
#define DEVICE_FILE_NAME "msg_slot_"
#define MAX_CHANNELS 1 < 20
#define SUCCESS 0
#define FREE_CHANNEL 0 // We use that number to indicate free channel and illigal channel numbers because it's an illigal channel number.
#define ILLEGAL_INDX -1
#define MSG_SLOT_CHANNEL _IO(MAJOR_NUM, 0)
#define MIN_WRITE 1
#define CHANNEL_INDX ((file_data*)file->private_data)->channel_indx
#define MINOR_INDX ((file_data*)file->private_data)->minor
#define ENOMEM 12
#define ERROR_CHECK(condition, extra_code, errno_value){\
    if(condition){\
        extra_code\
        return errno_value;\
    }\
}

// A data structure that will provide abstraction of a massage slot, channel_array will store the channel numbers that are cureently used.
// channels[i] != 0 iff channels[i] is the number of the channel that has its message stored at msgs[i],
// in that case, msgs_length[i] is msgs[i]'s length.
typedef struct Msg_Slot{
    unsigned int channels[MAX_CHANNELS];
    size_t msgs_length[MAX_CHANNELS];
    char msgs[MAX_CHANNELS][BUF_LEN];
}Msg_Slot;

// This data structure stores the minor number of a struct file and the channel indx of the struct file in msg_slot[MINOR_INDX].channels.
typedef struct file_data{
    int minor;
    int channel_indx;
}file_data;

#endif
