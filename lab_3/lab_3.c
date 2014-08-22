#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/cred.h>
static int you_pid = 0;
module_param(you_pid, int, S_IRUGO);

struct cred *cred_alloc_blank(void);

static int __init dr_init(void)
{
    struct task_struct *ptr;
    struct cred *new_cred;
    
    printk(KERN_INFO "nachalo\n");
    new_cred = kmalloc(sizeof(struct cred*), GFP_KERNEL);
    printk(KERN_INFO "Razmer structuri: %d\n", sizeof(new_cred));
    for_each_process(ptr)
    {
	if(ptr->pid == you_pid)
	{
	printk(KERN_INFO "Vhod v if\n");
	new_cred = prepare_creds();
	ptr->cred = new_cred;
	//printk(KERN_INFO "init_uid %d\n", init_cred.uid);
	}
    }
    return 0;
}

static void __exit dr_exit(void)
{

}

module_init(dr_init);
module_exit(dr_exit);

MODULE_AUTHOR("Author");
MODULE_LICENSE("GPL");

