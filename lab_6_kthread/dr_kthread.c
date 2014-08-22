#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int N = 2;
module_param(N, int, 0);

static char *sj(void)
{
    static char s[40];
    sprintf(s, "%08ld :", jiffies);
    return s;
}

static char *st(int lvl)
{
    static char s[40];
    sprintf(s, "%skthread [%05d:%d]", sj(), current->pid, lvl);
    return s;
}

static int thread_fun1(void* data)
{
    int N = (int)data - 1;
    struct task_struct *t1 = NULL;
    printk(KERN_INFO "%s is parent [%05d]\n", st(N), current->parent->pid);
    if(N > 0)
	t1 = kthread_run(thread_fun1, (void *)N, "my_thread_%d", N);
    while(!kthread_should_stop())
    {
	msleep(100);
    }
    printk(KERN_INFO "%s find signal!\n", st(N));
    if(t1 != NULL) kthread_stop(t1);
    printk(KERN_INFO "%s is completed\n", st(N));
    return 0;
}

static int test_thread(void)
{
    struct task_struct *t1;
    printk(KERN_INFO "%smain process [%d] is running\n", sj(), current->pid);
    t1 = kthread_run(thread_fun1, (void *)N, "my_thread_%d", N);
    msleep(10000);
    kthread_stop(t1);
    printk(KERN_INFO "%smain process [%d] is completed\n", sj(), current->pid);
    return -1;
}

module_init(test_thread);
MODULE_LICENSE("GPL");