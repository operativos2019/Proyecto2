#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");    // license type
MODULE_AUTHOR("Grupo2");     // author
MODULE_DESCRIPTION("Linux driver for a Checkers Player Robot.");  // description
MODULE_VERSION("0.1");    // version of the module

static int __init hello_init(void) {
    printk("<1> Hello world!\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk("<1> Bye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
