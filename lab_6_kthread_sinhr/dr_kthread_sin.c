#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>
#include <linux/kernel.h>
#include <linux/init.h>

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

#define NUM 3
static struct completion finished[NUM];

#define DELAY 1000
static int thread_func(void* data)
{
    int num = (int)data;
    printk("! %s is running\n", st(num));
    msleep(DELAY - num);
    complete(finished + num);
    printk("! %s is finished\n", st(num));
    return 0;
}

#define IDENT "for_thread_%d"
static int test_mlock(void)
{
    struct task_struct *t[NUM];
    int i;
    for(i = 0; i < NUM; i++)
	init_completion(finished + i);
    for(i = 0; i < NUM; i++)
	t[i] = kthread_run(thread_func, (void *)i, IDENT, i);
    for(i = 0; i < NUM; i++)
	wait_for_completion(finished + 1);
    printk("! %s is finished\n", st(NUM));
    return -1;
}

module_init(test_mlock);
MODULE_LICENSE("GPL");