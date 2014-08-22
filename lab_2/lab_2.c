#include <linux/list.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
int __init list_init(void)
{
    struct task_struct *ptr;
    for_each_process(ptr)
    {
	char comm[TASK_COMM_LEN];
	get_task_comm(comm, ptr);
	printk(KERN_INFO "Process name %s; pid %d\n", comm, ptr->pid);
    }
return 0;
}

void __exit list_exit(void)
{
}

module_init(list_init);
module_exit(list_exit);

MODULE_AUTHOR("Author");
MODULE_LICENSE("GPL");