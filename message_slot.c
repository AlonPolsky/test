
#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE

#include "message_slot.h"
#include <linux/uaccess.h>
#include <linux/slab.h>


MODULE_LICENSE("GPL");

// The msg_slot with minor number i has its data stored in msg_slots[i].
static Msg_Slots* msg_slots;


//================== AUXILARY FUNCTIONS =========================

channel* channel_init(file_data* context)
{
  channel* chan = NULL;
  chan = (channel*) kzalloc(sizeof(channel), GFP_KERNEL);
  // Checking for failed allocation.
  if(chan == NULL)
    return NULL
  chan->minor = context->minor;
  chan->num = context->channel_num;
  return chan;
}

channel* find_channel(file_data* context, int write){
  // This function finds a channel relevant to the context's file struct,
  // In a case there is none as such, and write == 1 (device_write() called the function),
  // if msg_slots->head == NULL, the function initializes msg_slots->head, and returns it, otherwise it appends a new channel to msg_slots's tail, and returns that.
  // If write == 0 (device_read() called the function), it returns NULL.
  // In any case of failed allocation, the function returns NULL.
  channel* chan = NULL;
  if(context->prev_channel != NULL)
  {
    return context->prev_channel;
  }
  
  if(msg_slots->head == NULL)
  {
    if(write){
      msg_slots->head = channel_init(context);
    }
    return msg_slots->head;
  }
  
  chan = msg_slots->head;
  while ((chan->next != NULL) && ((chan->minor != context->minor)  || (chan->minor != context->minor)))
  {
    chan = chan->next;
  }
  if(!((chan->minor != context->minor)  || (chan->minor != context->minor)))
  {
    return chan;
  }
  
  if(write)
  {
    chan->next = channel_init(context);
  }
  return chan->next;
}

//===============================================================

//================== DEVICE FUNCTIONS ===========================
static int device_open(struct inode* inode,
                        struct file*  file )
{
  // Here we fill file->private_data correctly.
  ERROR_CHECK((file->private_data = (void*) kzalloc(sizeof(file_data), GFP_KERNEL)) == NULL,,ENOMEM)

  ((file_data*)(file->private_data))->minor = iminor(inode);
  ((file_data*)(file->private_data))->channel = FREE_CHANNEL;

  return SUCCESS;
}

//---------------------------------------------------------------
static int device_release( struct inode* inode,
                           struct file*  file)
{
    kfree(file->private_data);
    return SUCCESS;
}
//---------------------------------------------------------------
static ssize_t device_read( struct file* file,
                            char __user* buffer,
                            size_t       length,
                            loff_t*      offset )
{
  // First we find the relevant channel, then we transfer the message into the buffer, while continuing the verification. 
  // All while checking for errors.

  int i;
  char checker;
  channel* channel;
  file_data* context = (file_data*) (file_data->private_data);

  // Checking whether there's no channel set on opened file
  ERROR_CHECK(context->channel_num == FREE_CHANNEL,, EINVAL)

  channel = find_channel(context, 0);
  
  // Checking whether there's no message on channel.
  ERROR_CHECK(channel == NULL,, EWOULDBLOCK)
  
  // Checking whether the given length is too small.
  ERROR_CHECK(length < channel->len, , ENOSPC)

  for(i = 0; i < channel->len; i++)
  {
    // No direct access to user-space addresses in the kernel-level.
    ERROR_CHECK(put_user((channel->message)[i], buffer + i),, EINVAL)
  }
    
  return i;
}

//---------------------------------------------------------------

static ssize_t device_write( struct file*       file,
                             const char __user* buffer,
                             size_t             length,
                             loff_t*            offset)
{
  // First we find the relevant channel, if there's no as such, we create one, 
  // then check the buffer, then we transfer the message from the buffer into the channel, while continuing the verification. 
  // All while checking for errors.

  int i;
  char checker;
  channel* channel;
  file_data* context = (file_data*) (file_data->private_data);

  // Checking whether there's no channel set on opened file
  ERROR_CHECK(context->channel_num == FREE_CHANNEL, , EINVAL)

  // Checking whether the length is too long or too short.
  ERROR_CHECK(length > BUF_LEN || length < MIN_WRITE, , EMSGSIZE)

  for(i = 0; i < length; i++)
  {
    // No direct access to user-space addresses in the kernel-level.
    ERROR_CHECK(get_user(checker, buffer + i),,EINVAL)
  }

  channel = find_channel(context, 1);

  ERROR_CHECK(channel ==  NULL, ,ENOMEM)
  channel->len = length;

  for(i = 0; i < length; i++)
  {
    // No direct access to user-space addresses in the kernel-level.
    ERROR_CHECK(get_user((channel->message)[i], buffer + i),,EINVAL)
  }

  return i;
}

//----------------------------------------------------------------
static long device_ioctl( struct   file* file,
                          unsigned int   ioctl_command_id,
                          long unsigned int ioctl_param )
{
  // Here we update file->private_data according to input, all while checking and handalling errors.
  int min = ILLEGAL_INDX;
  file_data* context = (file_data*) (file_data->private_data);

  // Checking for wrong ioctl_command_id or wrong_ioctl_param.
  ERROR_CHECK(ioctl_command_id != MSG_SLOT_CHANNEL || !ioctl_param, ,EINVAL)

  context->channel_num = ioctl_param;
    
  return SUCCESS;
}

//==================== DEVICE SETUP =============================

struct file_operations Fops = {
  .owner	  = THIS_MODULE, 
  .read           = device_read,
  .write          = device_write,
  .open           = device_open,
  .unlocked_ioctl = device_ioctl,
  .release        = device_release,
};

//---------------------------------------------------------------

static int __init simple_init(void)
{
  // Initialize the module - Register the character device
  int rc;

  msg_slots = (Msg_Slot*) kmalloc(MAX_MINOR * sizeof(Msg_Slot), GFP_KERNEL);

  // Checks for allocation fail.
  ERROR_CHECK(msg_slots == NULL,,ENOMEM);

  rc = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

  ERROR_CHECK(rc < 0, printk(KERN_ERR "%s registraion failed for  %d\n", DEVICE_FILE_NAME, MAJOR_NUM); kfree(msg_slots);, rc)
    
  return SUCCESS;
}

//---------------------------------------------------------------
static void __exit simple_cleanup(void)
{
  // Unregisters the driver, destroies the msg_slots list on its way.
  unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
  channel* chan = msg_slots->head;
  channel* temp = NULL;
  while(chan != NULL)
  {
      temp = chan->next;
      free(chan);
      chan = temp;
  }
  kfree(msg_slots);
}

//---------------------------------------------------------------
module_init(simple_init);
module_exit(simple_cleanup);

//========================= END OF FILE =========================
