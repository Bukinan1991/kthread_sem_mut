#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>

static int N = 0;
module_param(N, int, 0);

static int thread_fun1(void* data) {
	printk("start fun 1");
	struct task_struct *t1 = NULL;
	
	t1 = kthread_run(thread_fun1, (void*)N, "my_thread");
	printk("input while");
	printk("exit while");
	if(t1 != NULL) kthread_stop(t1);
	printk("stop");
	return 0;
}



static int test_thread(void) {
	struct task_struct *t1;
	t1 = kthread_run(thread_fun1, (void*)N, "my_thread");
	
	msleep(10000);

	kthread_stop(t1);

	return -1;
}

module_init(test_thread);
MODULE_LICENSE("GPL");
