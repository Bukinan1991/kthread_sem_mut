#include <linux/fs.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/random.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

#define NUM_USER 3
#define NUM_LOADER 2
#define MAX 100
#define NUM_SCUD 5

static int scud[NUM_SCUD];
static int num_user;
static struct semaphore sem[NUM_SCUD + 1];
struct task_struct *uthr[NUM_USER], *lthr[NUM_LOADER];
static struct data {
	int flag;
}fl = {0};

int init_mod(void);
void cleanup_mod(void);

int user(void *data);
int loader(void *data);
int test(void *data);

int test(void *data)
{
	int i;
	for(i = 5; i >= 0; i--) {
		printk(KERN_INFO "i %d\n", i);
		num_user = i;
	}

	while(!kthread_should_stop()) {
		msleep(100);
		printk(KERN_INFO "stop\n");
	}
	printk(KERN_INFO "end func test\n");
	return 0;
}

int user(void *data)
{
	int i, tmp = 0, tmp2 = 0, need = 0, user = (int)data;
	down(&sem[NUM_SCUD - 1]);
		printk(KERN_INFO "User ok %d, num user %d\n", user, num_user);
		num_user++;
	up(&sem[NUM_SCUD - 1]);
	
	while(need <= MAX) {
		for(i = 0; i < NUM_SCUD - 1; i++) {
			down(&sem[i]);
				if(scud[i] == 0) {
					up(&sem[i]);
					continue;
				}
				if(tmp == 0)
					tmp = (get_random_int() % 50);
				if(scud[i] >= tmp) {
					scud[i] -= tmp;
					need += tmp;
					tmp = 0;
					up(&sem[i]);
					printk(KERN_INFO " scud[%d] = %d, user - %d, have - %d\n",
							 i, scud[i], user, need);
					break;
				} else {
					tmp2 = tmp - scud[i];
					need += scud[i];
					scud[i] = 0;
					tmp = tmp2;
				}
				printk(KERN_INFO " scud[%d] = %d, user - %d, have - %d\n",
							 i, scud[i], user, need);
				//scud[i] += tmp;
			up(&sem[i]);
		}
		msleep(100);
	}
	//fl.flag = 1;
	down(&sem[NUM_SCUD - 1]);
		num_user--;
		printk(KERN_INFO "User good by %d. Lost user %d\n", user, num_user);
	up(&sem[NUM_SCUD - 1]);

	while(!kthread_should_stop()) {
		msleep(1000);
	}

	/*for(i = 0; i < NUM_LOADER; i++) {
		kthread_stop(lthr[i]);
		printk(KERN_INFO "stop loader %d\n", i);
	}*/

	return 0;
}

int loader(void* data)
{
	int i, tmp;
	printk(KERN_INFO "work loader\n");
	while(num_user != fl.flag) {
		printk(KERN_INFO "GO loade\n");
		for(i = 0; i < NUM_SCUD - 1; i++) {
			down(&sem[i]);
				if(scud[i] >= MAX) {
					up(&sem[i]);
					continue;
				}
				tmp = (get_random_int() % 50);
				if((scud[i] + tmp) >= MAX) {
					scud[i] = MAX;
				} else {
					scud[i] += tmp;
				}
				printk(KERN_INFO " scud[%d] = %d, tmp - %d\n",
							 i, scud[i], tmp);
			up(&sem[i]);
			//break;
		}
		msleep(50);
	}
	
	while(!kthread_should_stop()) {
		msleep(1000);
	}
	return 0;
}

int __init init_mod(void)
{
	int i = 0;

	printk(KERN_INFO "thread %s\n", __FUNCTION__);

	for(i = 0; i < NUM_SCUD; i++) {
		scud[i] = (get_random_int() % 100);
		printk(KERN_INFO "scud[%d] = %d\n", i, scud[i]);
	}

	for(i = 0; i < NUM_SCUD; i++) {
		sema_init(&sem[i], 1);
		printk(KERN_INFO "init sema %d\n", i);
	}

	for(i = 0; i < NUM_USER; i++)      // work!!!!
		uthr[i] = kthread_run(user, (void*) i, "user_fuck_%d", i);

	for(i = 0; i < NUM_LOADER; i++)
		lthr[i] = kthread_run(loader, NULL, "loader_fuck_%d", i);
	
	for(i = 0; i < NUM_USER; i++) {
		kthread_stop(uthr[i]);
		printk(KERN_INFO "stop user %d\n", i);
	}

	for(i = 0; i < NUM_LOADER; i++) {
		kthread_stop(lthr[i]);
		printk(KERN_INFO "stop loader %d\n", i);
	}

	printk(KERN_INFO "END My function\n");
	return -1;
}


void __exit cleanup_mod(void)
{
	printk(KERN_INFO "end thread\n");
}

module_init(init_mod);
module_exit(cleanup_mod);

