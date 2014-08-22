#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>

static int fun_1(void *data)
{
    printk(KERN_INFO "test \n");
    printk(KERN_INFO "test \n");
    printk(KERN_INFO "test \n");
    printk(KERN_INFO "test \n");
    printk(KERN_INFO "test \n");
    while(!kthread_should_stop()) {
	msleep(100);
    }
    return 0;
}

static int __init fun_init(void)
{
    struct task_struct *t1;

    t1 = kthread_run(fun_1, (void *)NULL, "id");

    msleep(5000);

    kthread_stop(t1);

    return -1;
}


static void __exit fun_exit(void)
{
}

module_init(fun_init);
module_exit(fun_exit);

MODULE_AUTHOR("Author");
MODULE_LICENSE("GPL");