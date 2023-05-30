
#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE

#include "message_slot.h"

// The msg_slot with minor number i has its data stored in msg_slots[i].
static Msg_Slot msg_slots[MAX_MSGSLOTS];

//================== DEVICE FUNCTIONS ===========================
static int device_open( struct inode* inode,
                        struct file*  file )
{
    // Here we fill file->private_data correctly.
    ERROR_CHECK((file->private_data = (void*) kmalloc(sizeof(file_data), GFP_KERNEL)) == NULL,,EMVSDYNALC)

    MINOR_INDX = iminor(inode);
    CHANNEL_INDX = ILLEGAL_INDX;

    return SUCCESS;
}

//---------------------------------------------------------------
static ssize_t device_read( struct file* file,
                            char __user* buffer,
                            size_t       length,
                            loff_t*      offset )
{
    // First we check arguments, then we transfer the message into the buffer, while continuing the verification.

    int i;
    char checker;

    ERROR_CHECK(CHANNEL_INDX == ILLEGAL_INDX, , EINVAL)
    ERROR_CHECK(msg_slots[MINOR_INDX].msg_length[CHANNEL_INDX] == 0, , EWOULDBLOCK)
    ERROR_CHECK(length < msg_slots[MINOR_INDX].msg_length[CHANNEL_INDX], , ENOSPC)

    for(i = 0; i < length; i++)
    {
        ERROR_CHECK(get_user(checker, buffer + i),,EINVAL)
    }

    for(i = 0; i < msg_slots[MINOR_INDX].msg_length[CHANNEL_INDX]; i++)
        ERROR_CHECK(put_user(msg_slots[MINOR_INDX].msgs[CHANNEL_INDX][i], buffer + i),, EINVAL)
    
    return i;
}

//---------------------------------------------------------------
// a processs which has already opened
// the device file attempts to write to it
static ssize_t device_write( struct file*       file,
                             const char __user* buffer,
                             size_t             length,
                             loff_t*            offset)
{
  ssize_t i;
  printk("Invoking device_write(%p,%ld)\n", file, length);
  for( i = 0; i < length && i < BUF_LEN; ++i ) {
  }
 
  // return the number of input characters used
  return i;
}

//----------------------------------------------------------------
static long device_ioctl( struct   file* file,
                          unsigned int   ioctl_command_id,
                          unsigned long  ioctl_param )
{
  // Switch according to the ioctl called
  if( MSG_SLOT_CHANNEL == ioctl_command_id ) {
    // Get the parameter given to ioctl by the process
    printk( "Invoking ioctl: setting encryption "
            "flag to %ld\n", ioctl_param );
  }

  return SUCCESS;
}

//==================== DEVICE SETUP =============================

// This structure will hold the functions to be called
// when a process does something to the device we created
struct file_operations Fops = {
  .owner	  = THIS_MODULE, 
  .read           = device_read,
  .write          = device_write,
  .open           = device_open,
  .unlocked_ioctl = device_ioctl,
  .release        = device_release,
};

//---------------------------------------------------------------
// Initialize the module - Register the character device
static int __init simple_init(void)
{
  int rc = -1;
  // init dev struct

  // Register driver capabilities. Obtain major num
  rc = register_chrdev( MAJOR_NUM, DEVICE_NAME, &Fops );

  // Negative values signify an error
  if( rc < 0 ) {
    printk( KERN_ALERT "%s registraion failed for  %d\n",
                       DEVICE_FILE_NAME, MAJOR_NUM );
    return rc;
  }

  printk( "Registeration is successful. ");
  printk( "If you want to talk to the device driver,\n" );
  printk( "you have to create a device file:\n" );
  printk( "mknod /dev/%s c %d 0\n", DEVICE_FILE_NAME, MAJOR_NUM );
  printk( "You can echo/cat to/from the device file.\n" );
  printk( "Dont forget to rm the device file and "
          "rmmod when you're done\n" );

  return 0;
}

//---------------------------------------------------------------
static void __exit simple_cleanup(void)
{
  // Unregister the device
  // Should always succeed
  unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}

//---------------------------------------------------------------
module_init(simple_init);
module_exit(simple_cleanup);

//========================= END OF FILE =========================
