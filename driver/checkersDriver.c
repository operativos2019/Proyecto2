#include <linux/fs.h>
#include <linux/usb.h>
#include <linux/slab.h>
#include <linux/kref.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

#define USB_VENDOR_ID	0x1A86
#define USB_PRODUCT_ID	0x7523
#define USB_ROBOTICFINGER_MINOR_BASE	192

MODULE_VERSION("0.1");
MODULE_AUTHOR("Grupo2");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Linux driver for an Arduino UNO.");

//file operations prototypes
static int     arduino_driver_open(struct inode*, struct file*);
static int     arduino_driver_release(struct inode*, struct file*);
static ssize_t arduino_driver_read(struct file*, char*, size_t, loff_t*);
static ssize_t arduino_driver_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations arduino_driver_operations = {
    .open = arduino_driver_open,
    .release = arduino_driver_release,
    .read = arduino_driver_read,
    .write = arduino_driver_write,
};

static struct usb_class_driver roboticFinger_class = {
	  .name = "arduino%d",
	  .fops = &arduino_driver_operations,
	  .minor_base = USB_ROBOTICFINGER_MINOR_BASE,
};


//driver operations prototypes
static int arduino_driver_probe(struct usb_interface*, const struct usb_device_id*);
static void arduino_driver_disconnect(struct usb_interface*);

// table of devices that work with this driver
static struct usb_device_id usb_device_table [] = {
  { USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID) },
  { }
};

// allows linux to load the driver when plugged in
MODULE_DEVICE_TABLE (usb, usb_device_table);

static struct usb_driver usb_device_driver = {
    .name = "arduino_driver",
    .id_table = usb_device_table,
    .probe = arduino_driver_probe,
    .disconnect = arduino_driver_disconnect,
};


//module operations prototypes
static int  __init arduino_driver_init(void);
static void __exit arduino_driver_exit(void);

module_init(arduino_driver_init);
module_exit(arduino_driver_exit);

struct usb_roboticFinger {
	struct usb_device*	  udev;			// the usb device
	struct usb_interface* interface;		// the interface for this device
  	unsigned char*		  bulk_in_buffer;		// the buffer to receive data
  	size_t			      bulk_in_size;		// the size of the receive buffer
    __u8			      bulk_in_endpointAddr;	// the address of the bulk in endpoint
  	__u8			      bulk_out_endpointAddr;	// the address of the bulk out endpoint
  	struct kref		      kref;
};

//used by arduino_driver_delete
#define to_roboticFinger_dev(d) container_of(d, struct usb_roboticFinger, kref)

static void arduino_driver_delete(struct kref* kref) {
    struct usb_roboticFinger *dev = to_roboticFinger_dev(kref);
	usb_put_dev(dev->udev);
	kfree (dev->bulk_in_buffer);
	kfree (dev);
}

/**
 * The device open function that is called each time the device is opened.
 */
static int arduino_driver_open(struct inode *inode, struct file *file){
    struct usb_roboticFinger *dev;
	struct usb_interface *interface;
	int subminor;
	int retval = 0;

	subminor = iminor(inode);

	interface = usb_find_interface(&usb_device_driver, subminor);
	if (!interface) {
        printk(KERN_INFO "arduino_driver: %s - error, can't find device for minor %d", __FUNCTION__, subminor);
		retval = -ENODEV;
		goto exit;
	}

	dev = usb_get_intfdata(interface);
	if (!dev) {
		retval = -ENODEV;
		goto exit;
	}

	kref_get(&dev->kref);

	file->private_data = dev;

    printk(KERN_INFO "arduino_driver: file opened\n");

    exit:
  	    return retval;
}

/**
 *  The device release function that is called whenever the device is closed/released by
 *  the userspace program.
 */
static int arduino_driver_release(struct inode* inode, struct file* file) {
    struct usb_roboticFinger* dev;
    dev = (struct usb_roboticFinger*)file->private_data;
    if (dev == NULL)
        return -ENODEV;
    kref_put(&dev->kref, arduino_driver_delete);
    printk(KERN_INFO "arduino_driver: file successfully closed\n");
    return 0;
}

/**
 *  This function is called whenever device is being read from user space.
 */
static ssize_t arduino_driver_read(struct file* file, char* buffer, size_t count, loff_t* offset){
    struct usb_roboticFinger* dev;
	int retval = 0;
	int count_int;
	dev = (struct usb_roboticFinger*)file->private_data;
    count_int = (int) count;
	retval = usb_bulk_msg(dev->udev,
			              usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
			              dev->bulk_in_buffer,
			              min(dev->bulk_in_size, count),
			              &count_int, HZ*10);
	if (!retval) {
        if (copy_to_user(buffer, dev->bulk_in_buffer, count))
			retval = -EFAULT;
		else
			retval = count;
	}
	printk(KERN_INFO "arduino_driver: Error reading retval=%d",retval);
	return retval;
}

/**
 * Function used by arduino_driver_write to send data to usb device.
 */
static void arduino_driver_write_bulk_callback(struct urb* urb) {
    if (urb->status && !(urb->status == -ENOENT ||
		urb->status == -ECONNRESET || urb->status == -ESHUTDOWN)) {
		printk(KERN_INFO "arduino_driver: %s - nonzero write bulk status received: %d",
		    __FUNCTION__, urb->status);
	}
	usb_free_coherent(urb->dev, urb->transfer_buffer_length,urb->transfer_buffer, urb->transfer_dma);
}

/**
 *  This function is called whenever the device is being written to from user space.
 */
static ssize_t arduino_driver_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *offset){
    struct usb_roboticFinger *dev;
    int retval = 0;
    struct urb *urb = NULL;
    char *buf = NULL;
    dev = (struct usb_roboticFinger *)file->private_data;

    if (count == 0)
        goto exit;

    urb = usb_alloc_urb(0, GFP_KERNEL);
    if (!urb) {
        retval = -ENOMEM;
        goto error;
    }

    buf =  usb_alloc_coherent(dev->udev, count, GFP_KERNEL, &urb->transfer_dma);
    if (!buf) {
    	retval = -ENOMEM;
    	goto error;
    }
    if (copy_from_user(buf, user_buffer, count)) {
        retval = -EFAULT;
        goto error;
    }
  	usb_fill_bulk_urb(urb, dev->udev, usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),buf, count, arduino_driver_write_bulk_callback, dev);
  	urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	retval = usb_submit_urb(urb, GFP_KERNEL);
  	if  (retval) {
    	printk(KERN_INFO "arduino_driver: %s - failed submitting write usb, error %d", __FUNCTION__, retval);
    	goto error;
  	}
  	printk(KERN_INFO "arduino_driver: successful write");
  	usb_free_urb(urb);
  	exit:
    	return count;

  	error:
    	usb_free_coherent(dev->udev, count, buf, urb->transfer_dma);
    	usb_free_urb(urb);
    	kfree(buf);
    	return retval;
}

/**
 * Function called at runtime when the device is plugged in.
 */
static int arduino_driver_probe(struct usb_interface* interface, const struct usb_device_id* id) {
    struct usb_roboticFinger* dev = NULL;
    struct usb_host_interface* iface_desc;
    struct usb_endpoint_descriptor* endpoint;
    size_t buffer_size;
    int i;
    int retval = -ENOMEM;

    dev = kmalloc(sizeof(struct usb_roboticFinger), GFP_KERNEL);
    if (dev == NULL) {
    	printk(KERN_INFO "arduino_driver: Out of memory");
      	goto error;
    }
    memset(dev, 0x00, sizeof (*dev));
    kref_init(&dev->kref);

    dev->udev = usb_get_dev(interface_to_usbdev(interface));
    dev->interface = interface;

    iface_desc = interface->cur_altsetting;
    for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
      	endpoint = &iface_desc->endpoint[i].desc;

      	if (!dev->bulk_in_endpointAddr &&
          	(endpoint->bEndpointAddress & USB_DIR_IN) &&
          	((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK)) {
	        buffer_size = endpoint->wMaxPacketSize;
	        dev->bulk_in_size = buffer_size;
	        dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
	        dev->bulk_in_buffer = kmalloc(buffer_size, GFP_KERNEL);
	        if (!dev->bulk_in_buffer) {
	          	printk(KERN_INFO "arduino_driver: Could not allocate bulk_in_buffer");
	          	goto error;
	        }
	    }

	    if (!dev->bulk_out_endpointAddr &&
			!(endpoint->bEndpointAddress & USB_DIR_IN) &&
			((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK)) {
	        	dev->bulk_out_endpointAddr = endpoint->bEndpointAddress;
	    }
    }
    if (!(dev->bulk_in_endpointAddr && dev->bulk_out_endpointAddr)) {
      printk(KERN_INFO "arduino_driver: Could not find both bulk-in and bulk-out endpoints");
      goto error;
    }
    usb_set_intfdata(interface, dev);
    retval = usb_register_dev(interface, &roboticFinger_class);
    if (retval) {
        printk(KERN_INFO "arduino_driver: Not able to get a minor for this device.");
        usb_set_intfdata(interface, NULL);
      	goto error;
    }

    printk(KERN_INFO "arduino_driver: arduino plugged, device now attached to arduino%d", interface->minor);
    return 0;

    error:
    	if (dev)
        kref_put(&dev->kref, arduino_driver_delete);
      	return retval;
}

/**
 * Function called on runtime when the device is disconected.
 */
static void arduino_driver_disconnect(struct usb_interface *interface) {
	struct usb_roboticFinger *dev;
	int minor = interface->minor;
	dev = usb_get_intfdata(interface);
	usb_set_intfdata(interface, NULL);
	usb_deregister_dev(interface, &roboticFinger_class);
	kref_put(&dev->kref, arduino_driver_delete);
	printk(KERN_INFO "arduino_driver: Arduino removed,  #%d now disconnected", minor);
}

/**
 * Function called to install the driver.
 */
static int  __init arduino_driver_init(void) {
    int result;
    printk(KERN_INFO "arduino_driver: Initializing the arduino_driver\n");
    result = usb_register(&usb_device_driver);
    if (result < 0) {
        printk(KERN_INFO "arduino_driver: usb_register failed. Error number %d", result);
        return -1;
    }
    printk(KERN_INFO "arduino_driver: arduino_driver successfully initialized\n");
    return 0;
}

/**
 * Function called to uninstall the driver.
 */
static void __exit arduino_driver_exit(void) {
    usb_deregister(&usb_device_driver);        // unregister the major number
    printk(KERN_INFO "arduino_driver: arduino_driver successfully removed\n");
}
